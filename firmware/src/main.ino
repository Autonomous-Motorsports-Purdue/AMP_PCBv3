#include "PinNames.h"
#include "SPI.h"
#include "WSerial.h"
#include "app_statemachine.h"
#include "config.h"
#include <Arduino.h>
#include <HardwareTimer.h>
#include <cstdint>

bool can_tick_state_machine = false;

void setup() {
  Serial.begin(115200);

  SPI.setMISO(SPI_MISO);
  SPI.setMOSI(SPI_MOSI);
  SPI.setSCLK(SPI_SCK);

  App_StateMachine_Init();

  HardwareTimer *statemachine_timer = new HardwareTimer(TIM6);
  statemachine_timer->setOverflow(TICKS_PER_SEC, HERTZ_FORMAT);
  statemachine_timer->attachInterrupt([]() { can_tick_state_machine = true; });
  statemachine_timer->resume();
}

void loop() {
  if (can_tick_state_machine) {
    App_StateMachine_Tick();
    can_tick_state_machine = false;
  }
}
