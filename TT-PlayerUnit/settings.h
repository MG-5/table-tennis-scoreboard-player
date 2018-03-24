#pragma once

#define UART_BAUD 1000000UL

// DISPLAY
#define DIGIT_DDR DDRB
#define DIGIT_PORT PORTB // 0-4
#define STORE_PORT PORTD

#define DATA (1 << PB6)  // DO
#define STORE (1 << PD5) // (SS)
#define CLOCK (1 << PB7) // USCK
#define MASTER_RESET (1 << PD6)
#define DP_PIN (1 << PB4) // doubepoint
