#pragma once

#define PLAYERONE 1
#define UART_BAUD 921600

// DISPLAY
#define DIGIT_PORT PORTB //0-4

#define DATA (1 << PB6)  // DO
#define STORE (1 << PD5)  // (SS)
#define CLOCK (1 << PB7) // USCK
#define MASTER_RESET (1 << PD6)
