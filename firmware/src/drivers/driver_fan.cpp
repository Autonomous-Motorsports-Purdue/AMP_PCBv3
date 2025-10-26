#include "driver_fan.h"
#include "config.h"
#include "wiring_constants.h"
#include "wiring_digital.h"
#include <Arduino.h>

// function to turn a fan on
void Driver_Fan_On(uint32_t fan) { digitalWrite(fan, HIGH); }

// function to turn a fan off
void Driver_Fan_Off(uint32_t fan) { digitalWrite(fan, LOW); }

// function to turn all fans on
void Driver_Fan_All_On() {
  Driver_Fan_On(FAN1);
  Driver_Fan_On(FAN2);
  Driver_Fan_On(FAN3);
  Driver_Fan_On(FAN4);
}

// toggle to turn all fans off
void Driver_Fan_All_Off() {
  Driver_Fan_Off(FAN1);
  Driver_Fan_Off(FAN2);
  Driver_Fan_Off(FAN3);
  Driver_Fan_Off(FAN4);
}
