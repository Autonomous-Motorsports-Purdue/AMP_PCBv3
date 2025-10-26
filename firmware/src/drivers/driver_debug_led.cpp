/*
 * driver_Debug_led.c
 *
 *  Created on: Apr 19, 2024
 *      Author: benjaminowen
 */

#include "driver_debug_led.h"
#include "config.h"
#include "wiring_digital.h"
#include <Arduino.h>

// constant LED struct values
Stat_LED_T STAT1 = {.pin = DEBUG_LED_STAT1};
Stat_LED_T STAT2 = {.pin = DEBUG_LED_STAT2};
Stat_LED_T STAT3 = {.pin = DEBUG_LED_STAT3};
Stat_LED_T STAT4 = {.pin = DEBUG_LED_STAT4};

// function to turn a Debug LED on
void Driver_Debug_LED_On(Stat_LED_T led) { digitalWrite(led.pin, HIGH); }

// function to turn a Debug LED off
void Driver_Debug_LED_Off(Stat_LED_T led) { digitalWrite(led.pin, LOW); }

// function to convert a 4-bit hex value into LED Debug function calls
void Driver_Debug_LED_SetHex(uint8_t value) {
  if (value & 0x1) {
    Driver_Debug_LED_On(STAT1);
  } else {
    Driver_Debug_LED_Off(STAT1);
  }

  if (value & 0x2) {
    Driver_Debug_LED_On(STAT2);
  } else {
    Driver_Debug_LED_Off(STAT2);
  }

  if (value & 0x4) {
    Driver_Debug_LED_On(STAT3);
  } else {
    Driver_Debug_LED_Off(STAT3);
  }

  if (value & 0x8) {
    Driver_Debug_LED_On(STAT4);
  } else {
    Driver_Debug_LED_Off(STAT4);
  }
}
