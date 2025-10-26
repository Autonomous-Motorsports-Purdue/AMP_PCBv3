#ifndef __FAN_H
#define __FAN_H

#include <cstdint>

void Driver_Fan_On(uint32_t);
void Driver_Fan_Off(uint32_t);
void Driver_Fan_All_On();
void Driver_Fan_All_Off();

#endif
