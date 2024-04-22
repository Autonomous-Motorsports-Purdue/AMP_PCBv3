/*
 * driver_uart.h
 *
 *  Created on: Apr 21, 2024
 *      Author: benjaminowen
 */

#ifndef APP_INC_DRIVER_UART_H_
#define APP_INC_DRIVER_UART_H_

extern char uart_sending;

typedef enum
{
	NUCLEO, JETSON
} UART_Location_T;

void Driver_UART_Init();
void Driver_UART_Transmit(UART_Location_T, char *);
unsigned char * Driver_UART_GetBuffer(UART_Location_T);
uint8_t Driver_UART_GetTail(UART_Location_T);
void Driver_UART_ClearBuffer(UART_Location_T);

#endif /* APP_INC_DRIVER_UART_H_ */
