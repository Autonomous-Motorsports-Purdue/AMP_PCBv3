/*
 * driver_throttle.h
 *
 *  Created on: Apr 23, 2024
 *      Author: benjaminowen
 */

#ifndef APP_INC_DRIVER_THROTTLE_H_
#define APP_INC_DRIVER_THROTTLE_H_

#include <cstdint>
typedef enum {
  THROTTLE_DIRECTION_FORWARD,
  THROTTLE_DIRECTION_REVERSE
} Throttle_Direction_T;

void Driver_Throttle_Init(uint16_t, Throttle_Direction_T);
void Driver_Throttle_SetFrequency(uint16_t, Throttle_Direction_T);

#endif /* APP_INC_DRIVER_THROTTLE_H_ */
