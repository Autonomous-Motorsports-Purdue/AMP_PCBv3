/*
 * driver_steering.c
 *
 *  Created on: Apr 21, 2024
 *      Author: Matthew Briggs
 */
#include <Arduino.h>

#include "HardwareTimer.h"
#include "config.h"
#include "driver_steering.h"

// Duty cycle limits to prevent extreme positions that the motor doesn't handle
// well
#define STEERING_MIN_DUTY_CYCLE 5
#define STEERING_MAX_DUTY_CYCLE 250

// Timer object for generating PWM signal
HardwareTimer *steering_tim;

/*
 * Steering Servo Input Descriptions
 *
 * Enable:
 * Used as output - energizes the motor shaft when set high
 *
 * Input A+:
 * If used as output, asserting will cause servo to ignore changes to B+
 * If used as input, it is used as home position sensor
 * Set low to allow PWM control via Input B+
 *
 * Input B+:
 * Used as output (PWM/frequency command for position)
 * Connected to Timer 3 Channel 3
 * 50% duty cycle = straight, extremes turn motor left/right
 */

/**
 * @brief Initialize the steering servo motor
 * @param duty_cycle Initial duty cycle value (0-255), typically 128 for
 * straight
 *
 * This function configures the GPIO pins for the level shifters and motor
 * control, sets up Timer 3 Channel 3 for 1kHz PWM output, and starts the PWM
 * signal.
 */
void Driver_Steering_Init(uint8_t duty_cycle) {

  // Configure all pin modes first (except PWM pin)
  pinMode(STR_INPUTA_DIR_PIN, OUTPUT);
  pinMode(STR_INPUTB_DIR_PIN, OUTPUT);
  pinMode(STR_EN_PIN, OUTPUT);
  pinMode(STR_INPUTA_LOGIC_PIN, OUTPUT);
  // DON'T set pinMode for STR_INPUTB_LOGIC_PIN - timer will handle it

  digitalWrite(STR_INPUTA_DIR_PIN, 1);
  digitalWrite(STR_INPUTB_DIR_PIN, 1);

  digitalWrite(STR_EN_PIN, 1);
  digitalWrite(STR_INPUTA_LOGIC_PIN, 0);

  steering_tim = new HardwareTimer(TIM3);

  // Use PB0_ALT1 instead of PB0 for TIM3_CH3
  steering_tim->setMode(3, TIMER_OUTPUT_COMPARE_PWM1, PB0_ALT1);

  steering_tim->setOverflow(1000, HERTZ_FORMAT);

  steering_tim->setCaptureCompare(3, 128, RESOLUTION_8B_COMPARE_FORMAT);

  steering_tim->resume();

  Driver_Steering_SetDutyCycle(duty_cycle);
}

/**
 * @brief Set the duty cycle for the steering PWM signal
 * @param duty_cycle Desired duty cycle (0-255)
 *                   0 = full left turn
 *                   128 = straight ahead (50%)
 *                   255 = full right turn
 *
 * The function clamps the input to safe bounds (5-250) to avoid extreme
 * positions that could damage the motor. Uses 8-bit resolution where the
 * duty_cycle value directly maps to the compare register.
 */
void Driver_Steering_SetDutyCycle(uint8_t duty_cycle) {
  if (duty_cycle > STEERING_MAX_DUTY_CYCLE) {
    duty_cycle = STEERING_MAX_DUTY_CYCLE;
  } else if (duty_cycle < STEERING_MIN_DUTY_CYCLE) {
    duty_cycle = STEERING_MIN_DUTY_CYCLE;
  }

  steering_tim->setCaptureCompare(3, duty_cycle, RESOLUTION_8B_COMPARE_FORMAT);
}
