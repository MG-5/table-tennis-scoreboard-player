#include "main.h"

A1001Display display;
uint32_t prevTime = 0;
uint32_t prevTime2 = 0;

int main(void)
{
  timer0_init();
  uart_init(UART_BAUD_SELECT_DOUBLE_SPEED(UART_BAUD, F_CPU));

  display.initDisplay();
  display.resetOnce();

  sei();

  // display.startupSequence();
  display.setSegments(false, 0, 0, 0, 0);

  while (true)
  {
    uint32_t now = millis();
    if (checkNewInfos())
      prevTime = now;

    else if ((now - prevTime) >= 50)
      display.setSegments(false, 64, 64, 64, 64);

    for (uint8_t i = 0; i <= 3; i++)
    {
      display.update(i);
      wait_us(150);
    }

    display.turnOffDigits();
    //_delay_us(100);
  }
}

bool checkNewInfos()
{
  uint8_t digits[5];

  unsigned int c = uart_getc();

  if (c & UART_NO_DATA)
    return false; // no new infos available

  do
  {
    if (c == START_FRAME)
    {
      // new packet arrived - 5 bytes will incoming
      for (uint8_t count = 0; count < 5; count++)
      {
        c = uart_getc();

        if ((c & UART_NO_DATA) || (c & UART_FRAME_ERROR) || (c & UART_PARITY_ERROR))
        {
          // reject packets by deleting the ring buffer completely
          uart_clearBuffer();
          return false;
        }
        digits[count] = c;
      }

      display.setSegments(digits[4], digits[0], digits[1], digits[2], digits[3]);
      uart_putc(ANSWER_CODE);
      return true;
    }
    else if (c == RQ_CODE)
    {
      uart_putc(ANSWER_CODE);
      return true;
    }

    else
      // not equal to start - reject frame
      c = uart_getc();

  } while (!(c & UART_NO_DATA));

  return false;
}
