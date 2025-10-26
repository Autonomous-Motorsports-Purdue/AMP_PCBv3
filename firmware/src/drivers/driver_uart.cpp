#include "driver_uart.h"
#include "HardwareSerial.h"
#include <Arduino.h>
#include <string.h>

#define UART_BUF_LENGTH 80

// Define which hardware serial ports to use
// Adjust these based on your Arduino board

HardwareSerial Serial4(PA0, PA1);
#define SERIAL_NUCLEO Serial  // USB Serial or Serial
#define SERIAL_JETSON Serial4 // Hardware Serial4

char uart_send_buf[UART_BUF_LENGTH]; // buffer for messages to send over UART
char
    uart2_recv_buf[UART_BUF_LENGTH]; // buffer for messages received from NUCLEO
char
    uart4_recv_buf[UART_BUF_LENGTH]; // buffer for messages received from JETSON
uint8_t uart2_recv_buf_tail; // location of next empty byte in recv buffer
uint8_t uart4_recv_buf_tail; // location of next empty byte in recv buffer

char uart_sending = 0; // flag to indicate still sending

// function to initialize UART driver
void Driver_UART_Init() {
  // default variable values
  uart2_recv_buf_tail = 0;
  uart4_recv_buf_tail = 0;

  // Clear buffers
  memset(uart2_recv_buf, 0, UART_BUF_LENGTH);
  memset(uart4_recv_buf, 0, UART_BUF_LENGTH);

  // Initialize serial ports
  SERIAL_NUCLEO.begin(115200); // Adjust baud rate as needed
  SERIAL_JETSON.begin(115200); // Adjust baud rate as needed

  // Wait for serial ports to initialize
  while (!SERIAL_NUCLEO)
    ;
  delay(100);

  // "hello" message
  Driver_UART_Transmit(
      NUCLEO,
      "\e[2J\e[HAMP Kart UART Interface\r\n=======================\r\n");
}

// function to send a string message over UART
void Driver_UART_Transmit(UART_Location_T dest, const char *message) {
  // mark sending flag
  uart_sending = 1;

  // send to correct destination
  if (dest == NUCLEO) {
    if (message == NULL) {
      // Echo received character
      if (uart2_recv_buf_tail > 0) {
        SERIAL_NUCLEO.write(uart2_recv_buf[uart2_recv_buf_tail - 1]);
      }
    } else {
      // copy message into buffer
      strcpy(uart_send_buf, message);
      // send
      SERIAL_NUCLEO.print(uart_send_buf);
    }
  } else if (dest == JETSON) {
    if (message == NULL) {
      // Echo received character
      if (uart4_recv_buf_tail > 0) {
        SERIAL_JETSON.write(uart4_recv_buf[uart4_recv_buf_tail - 1]);
      }
    } else {
      // copy message into buffer
      strcpy(uart_send_buf, message);
      // send
      SERIAL_JETSON.print(uart_send_buf);
    }
  }

  // clear sending flag (Arduino serial is buffered)
  uart_sending = 0;
}

// function to get char buffer
unsigned char *Driver_UART_GetBuffer(UART_Location_T src) {
  if (src == NUCLEO) {
    return (unsigned char *)uart2_recv_buf;
  } else if (src == JETSON) {
    return (unsigned char *)uart4_recv_buf;
  }

  // error condition
  return NULL;
}

// function to get tail value for a buffer
uint8_t Driver_UART_GetTail(UART_Location_T src) {
  if (src == NUCLEO) {
    return uart2_recv_buf_tail;
  } else if (src == JETSON) {
    return uart4_recv_buf_tail;
  }

  // error condition
  return 0;
}

// function to "clear" a buffer
void Driver_UART_ClearBuffer(UART_Location_T src) {
  if (src == NUCLEO) {
    uart2_recv_buf_tail = 0;
    memset(uart2_recv_buf, 0, UART_BUF_LENGTH);
    // Flush any pending data
    while (SERIAL_NUCLEO.available()) {
      SERIAL_NUCLEO.read();
    }
  } else if (src == JETSON) {
    uart4_recv_buf_tail = 0;
    memset(uart4_recv_buf, 0, UART_BUF_LENGTH);
    // Flush any pending data
    while (SERIAL_JETSON.available()) {
      SERIAL_JETSON.read();
    }
  }
}

// Function to process incoming UART data
// Call this in your main loop()
void Driver_UART_Process() {
  // Process NUCLEO serial port
  while (SERIAL_NUCLEO.available() &&
         uart2_recv_buf_tail < UART_BUF_LENGTH - 1) {
    uart2_recv_buf[uart2_recv_buf_tail] = SERIAL_NUCLEO.read();
    uart2_recv_buf_tail++;
    // Echo character back
    Driver_UART_Transmit(NUCLEO, NULL);
  }

  // Process JETSON serial port
  while (SERIAL_JETSON.available() &&
         uart4_recv_buf_tail < UART_BUF_LENGTH - 1) {
    uart4_recv_buf[uart4_recv_buf_tail] = SERIAL_JETSON.read();
    uart4_recv_buf_tail++;
    // Echo character back
    Driver_UART_Transmit(JETSON, NULL);
  }
}

// Check if UART is currently sending
bool Driver_UART_IsSending() { return uart_sending; }
