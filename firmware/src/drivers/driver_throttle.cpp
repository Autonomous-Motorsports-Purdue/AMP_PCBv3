/*
 * driver_throttle.cpp
 *
 * Arduino Framework Implementation
 * TIM8 Channel 4 on PC9
 */

#include "driver_throttle.h"
#include <Arduino.h>

// Clock at 180 MHz, PSC at 179 (division by 180), frequency range is 1 Hz to 1
// MHz Below values are in microseconds
#define THROTTLE_MIN_PERIOD_US 1000
#define THROTTLE_MID_PERIOD_US 1500
#define THROTTLE_MAX_PERIOD_US 2000

#define THROTTLE_MIN_FREQUENCY_ARR                                             \
  (1000000 / (1000000 / THROTTLE_MIN_PERIOD_US) - 1)
#define THROTTLE_MID_FREQUENCY_ARR                                             \
  (1000000 / (1000000 / THROTTLE_MID_PERIOD_US) - 1)
#define THROTTLE_MAX_FREQUENCY_ARR                                             \
  (1000000 / (1000000 / THROTTLE_MAX_PERIOD_US) - 1)

// Function to initialize throttle default state
void Driver_Throttle_Init(uint8_t magnitude, Throttle_Direction_T direction) {
  // Enable TIM8 clock
  RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;

  // Enable GPIOC clock
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

  // Configure PC9 as alternate function (AF3 for TIM8_CH4)
  GPIOC->MODER &= ~(3 << (9 * 2));          // Clear mode bits
  GPIOC->MODER |= (2 << (9 * 2));           // Alternate function mode
  GPIOC->AFR[1] &= ~(0xF << ((9 - 8) * 4)); // Clear AF bits
  GPIOC->AFR[1] |= (3 << ((9 - 8) * 4));    // AF3 (TIM8)
  GPIOC->OSPEEDR |= (3 << (9 * 2));         // High speed

  // Configure TIM8
  TIM8->PSC = 179; // Prescaler: 180 MHz / 180 = 1 MHz timer clock
  TIM8->ARR = THROTTLE_MID_FREQUENCY_ARR;      // Auto-reload value
  TIM8->CCR4 = THROTTLE_MID_FREQUENCY_ARR / 2; // 50% duty cycle

  // Configure Channel 4 as PWM mode 1
  TIM8->CCMR2 &= ~TIM_CCMR2_OC4M;             // Clear output compare mode
  TIM8->CCMR2 |= (0x6 << TIM_CCMR2_OC4M_Pos); // PWM mode 1
  TIM8->CCMR2 |= TIM_CCMR2_OC4PE;             // Enable preload

  // Enable Channel 4 output
  TIM8->CCER |= TIM_CCER_CC4E;

  // Main output enable (required for TIM8)
  TIM8->BDTR |= TIM_BDTR_MOE;

  // Enable auto-reload preload
  TIM8->CR1 |= TIM_CR1_ARPE;

  // Set initial value
  Driver_Throttle_SetFrequency(magnitude, direction);

  // Start timer
  TIM8->CR1 |= TIM_CR1_CEN;
}

void Driver_Throttle_SetFrequency(uint8_t magnitude,
                                  Throttle_Direction_T direction) {
  // Map function:
  // (input - in_min) * (out_max - out_min) / (in_max - in_min) + out_min
  // Maps 'input' with range ('in_min' to 'in_max') to a range ('out_min' to
  // 'out_max')

  uint32_t mapped_throttle;

  // Map throttle taking direction into account
  if (direction == THROTTLE_DIRECTION_FORWARD) {
    mapped_throttle =
        (magnitude - 0) *
            (THROTTLE_MAX_FREQUENCY_ARR - THROTTLE_MID_FREQUENCY_ARR) /
            (255 - 0) +
        THROTTLE_MID_FREQUENCY_ARR;
  } else if (direction == THROTTLE_DIRECTION_REVERSE) {
    mapped_throttle =
        (magnitude - 0) *
            (THROTTLE_MIN_FREQUENCY_ARR - THROTTLE_MID_FREQUENCY_ARR) /
            (255 - 0) +
        THROTTLE_MID_FREQUENCY_ARR;
  } else {
    mapped_throttle = THROTTLE_MID_FREQUENCY_ARR;
  }

  // Set ARR to calculated value
  TIM8->ARR = mapped_throttle;
  TIM8->CCR4 = mapped_throttle / 2; // 50% duty cycle
  TIM8->CNT = 0;                    // Reset counter

  // Generate update event to reload registers
  TIM8->EGR |= TIM_EGR_UG;
}
