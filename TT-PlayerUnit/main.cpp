#include "main.h"

A1001Display display;
bool test = false;

int main(void)
{
  timer0_init();
  uart_init(UART_BAUD_SELECT_DOUBLE_SPEED(UART_BAUD, F_CPU));

  display.initDisplay();
  display.resetOnce();

  sei();

  display.startupSequence();
  display.setSegments(false, 64, 64, 64, 64);

  while (true)
  {
    checkNewInfos();

    for (uint8_t i = 0; i <= 3; i++)
    {
      display.update(i);
      wait_us(150);
    }
    display.turnOffDigits();
    //_delay_us(100);
  }
}

void checkNewInfos()
{
  uint8_t digits[5];

  unsigned int c = uart_getc();

  if (c & UART_NO_DATA)
    return; // no new infos available

  while (!(c & UART_NO_DATA))
  {
    if (c == 0x42)
    {
      // new packet arrived - 5 bytes will incoming
      uint8_t count = 0;
      bool noError = true;

      while (noError && count < 5)
      {
        c = uart_getc();

        if ((c & UART_NO_DATA) || (c & UART_FRAME_ERROR) || (c & UART_PARITY_ERROR))
        {
          // reject packets
          noError = false;
          break;
        }
        digits[count] = c;
        count++;
      }

      if (noError)
        display.setSegments(digits[4], digits[0], digits[1], digits[2], digits[3]);

      else
      {
        while (!noError)
        {
          // delete buffer completely
          c = uart_getc();
          while (!(c & UART_NO_DATA))
          {
            c = uart_getc();
          }
          noError = true;
        }
      }
    }
    else
      // not equal to start - reject frame
      c = uart_getc();
  }
}
