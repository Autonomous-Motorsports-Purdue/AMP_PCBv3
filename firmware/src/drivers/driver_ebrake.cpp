/*
 * driver_ebrake.c
 *
 * E-Brake servo driver for Arduino/STM32duino
 */

#include "driver_ebrake.h"
#include "config.h"

/**
 * @brief Initialize the e-brake servo
 *
 * Performs a full cycle: retract -> extend -> off
 * This ensures the e-brake is in a known state at startup
 */
void Driver_EBrake_Init() {
  // Configure all e-brake pins as outputs
  pinMode(BREAK_INA, OUTPUT);
  pinMode(BREAK_INB, OUTPUT);
  pinMode(BREAK_SEL0, OUTPUT);
  pinMode(BREAK_PWM, OUTPUT);

  // Retract e-brake to released position
  Driver_EBrake_Retract();

  // Allow time for e-brake to fully retract (2 seconds)
  delay(2000);

  // Extend e-brake to engaged position
  Driver_EBrake_Extend();

  // Allow time for e-brake to fully extend (1 second)
  delay(1000);

  // Turn off e-brake motor (no drive command)
  Driver_EBrake_Off();
}

/**
 * @brief Extend the e-brake servo (engage brake)
 *
 * Sets motor direction to extend/engage the brake
 * INA=1, INB=0 controls motor direction
 * SEL0=0, PWM=1 enables full speed drive
 */
void Driver_EBrake_Extend() {
  digitalWrite(BREAK_INA, HIGH);
  digitalWrite(BREAK_INB, LOW);
  digitalWrite(BREAK_SEL0, LOW);
  digitalWrite(BREAK_PWM, HIGH);
}

/**
 * @brief Retract the e-brake servo (release brake)
 *
 * Sets motor direction to retract/release the brake
 * INA=0, INB=1 controls motor direction (opposite of extend)
 * SEL0=0, PWM=1 enables full speed drive
 */
void Driver_EBrake_Retract() {
  digitalWrite(BREAK_INA, LOW);
  digitalWrite(BREAK_INB, HIGH);
  digitalWrite(BREAK_SEL0, LOW);
  digitalWrite(BREAK_PWM, HIGH);
}

/**
 * @brief Turn off the e-brake servo motor
 *
 * Disables all motor control signals
 * Motor stops and holds position (does not actively drive)
 * INA=0, INB=0, SEL0=0, PWM=0 = no drive command
 */
void Driver_EBrake_Off() {
  digitalWrite(BREAK_INA, LOW);
  digitalWrite(BREAK_INB, LOW);
  digitalWrite(BREAK_SEL0, LOW);
  digitalWrite(BREAK_PWM, LOW);
}
