#include "main.h"

int main(void)
{
  timer0_init();
  uart_init(UART_BAUD_SELECT_DOUBLE_SPEED(UART_BAUD, F_CPU));
  // Timer 0 konfiguieren
  // UART konfiguieren

  // Display initialisieren

  while (1)
  {
  }
}
