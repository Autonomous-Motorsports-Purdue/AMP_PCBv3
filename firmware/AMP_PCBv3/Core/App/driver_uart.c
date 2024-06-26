/*
 * driver_uart.c
 *
 *  Created on: Apr 21, 2024
 *      Author: benjaminowen
 */

#include "string.h"

#include "usart.h"

#include "driver_uart.h"

#define UART_BUF_LENGTH	80

char uart_send_buf[UART_BUF_LENGTH];	// buffer for messages to send over UART
char uart2_recv_buf[UART_BUF_LENGTH];	// buffer for messages received over UART2
char uart4_recv_buf[UART_BUF_LENGTH];	// buffer for messages received over UART4
uint8_t uart2_recv_buf_tail;			// location of next empty byte in recv buffer
uint8_t uart4_recv_buf_tail;			// location of next empty byte in recv buffer

char uart_sending = 0;	// flag to indicate still sending

// function to initialize UART driver
void Driver_UART_Init()
{
	// default variable values
	uart2_recv_buf_tail = 0;
	uart4_recv_buf_tail = 0;
	// start listening for interrupts
	HAL_UART_Receive_IT(&huart2, (unsigned char *) uart2_recv_buf, 1);
	HAL_UART_Receive_IT(&huart4, (unsigned char *) uart4_recv_buf, 1);
	// "hello" message
	Driver_UART_Transmit(NUCLEO, "\e[2J\e[HAMP Kart UART Interface\r\n=======================\r\n");
}

// function to send a string message over UART
void Driver_UART_Transmit(UART_Location_T dest, char * message)
{
	// mark sending flag
	uart_sending = 1;
	// send to correct destination
	if (dest == NUCLEO)
	{
		if (message == NULL)
		{
			// place buffer into message
			uart_send_buf[0] = uart2_recv_buf[uart2_recv_buf_tail - 1];
//			uart_send_buf[1] = '\r';
//			uart_send_buf[2] = '\n';
			// send
			HAL_UART_Transmit_DMA(&huart2, (unsigned char *) uart_send_buf, 1);
		}
		else
		{
			// copy message into buffer
			strcpy(uart_send_buf, message);
			// send
			HAL_UART_Transmit_DMA(&huart2, (unsigned char *) uart_send_buf, strlen(uart_send_buf));
		}
	}
	else if (dest == JETSON)
	{
		if (message == NULL)
		{
			// place buffer into message
			uart_send_buf[0] = uart4_recv_buf[uart4_recv_buf_tail - 1];
//			uart_send_buf[1] = '\r';
//			uart_send_buf[2] = '\n';
			// send
			HAL_UART_Transmit_DMA(&huart4, (unsigned char *) uart_send_buf, 1);
		}
		else
		{
			// copy message into buffer
			strcpy(uart_send_buf, message);
			// send
			HAL_UART_Transmit_DMA(&huart4, (unsigned char *) uart_send_buf, strlen(uart_send_buf));
		}
	}
}

// function to get char buffer
unsigned char * Driver_UART_GetBuffer(UART_Location_T src)
{
	if (src == NUCLEO)
	{
		return (unsigned char *) uart2_recv_buf;
	}
	else if (src == JETSON)
	{
		return (unsigned char *) uart4_recv_buf;
	}

	// error condition
	return NULL;
}

// function to get tail value for a buffer
uint8_t Driver_UART_GetTail(UART_Location_T src)
{
	if (src == NUCLEO)
	{
		return uart2_recv_buf_tail;
	}
	else if (src == JETSON)
	{
		return uart4_recv_buf_tail;
	}

	// error condition
	return 0;
}

// function to "clear" a buffer
void Driver_UART_ClearBuffer(UART_Location_T src)
{
	if (src == NUCLEO)
	{
		uart2_recv_buf_tail = 0;
		HAL_UART_AbortReceive_IT(&huart2);
		HAL_UART_Receive_IT(&huart2, Driver_UART_GetBuffer(NUCLEO) + uart2_recv_buf_tail, 1);
	}
	else if (src == JETSON)
	{
		uart4_recv_buf_tail = 0;
		HAL_UART_AbortReceive_IT(&huart4);
		HAL_UART_Receive_IT(&huart4, Driver_UART_GetBuffer(JETSON) + uart4_recv_buf_tail, 1);
	}
}

// UART TX interrupt callback
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	// clear sending flag
	uart_sending = 0;
}

// UART RX interrupt callback
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart2)
	{
		uart2_recv_buf_tail++;
		Driver_UART_Transmit(NUCLEO, NULL);
		HAL_UART_Receive_IT(&huart2, Driver_UART_GetBuffer(NUCLEO) + uart2_recv_buf_tail, 1);
	}
	else if (huart == &huart4)
	{
		uart4_recv_buf_tail++;
		Driver_UART_Transmit(JETSON, NULL);
		HAL_UART_Receive_IT(&huart4, Driver_UART_GetBuffer(JETSON) + uart4_recv_buf_tail, 1);
	}
}
