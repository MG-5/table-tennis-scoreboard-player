#include "main.h"

uint32_t counts = 0;
bool test = false;

int main(void)
{
  timer0_init();
  uart_init(UART_BAUD_SELECT_DOUBLE_SPEED(UART_BAUD, F_CPU));

  A1001Display display;
  display.initDisplay();
  display.resetOnce();

  sei();

  display.startupSequence();

  while (1)
  {
    /*
      if (counts >= 4000)
      {
        test = !test;
        counts = 0;
      }

      if (test)
        display.setSegments(true, digitToSegment[2], digitToSegment[1], digitToSegment[1], digitToSegment[9]);
      else
        display.setSegments(false, 109, 6, 121, 125);

    */

    score_myself = 21;
    score_other = 19;

    // display.showScoreline(score_myself, score_other);

    display.setSegments(false, 0, 0, 0, 0);

    for (uint8_t i = 0; i <= 3; i++)
    {
      display.update(i);
      _delay_us(100);
    }

    display.turnOffDigits();
    //_delay_us(100);

    // counts++;
  }
}
