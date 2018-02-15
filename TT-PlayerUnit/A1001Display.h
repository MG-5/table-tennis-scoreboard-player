#ifndef __A1001DISPLAY__
#define __A1001DISPLAY__

#include "settings.h"
#include <stdint.h>

class A1001Display
{

public:
  //! Initialize a A1001Display object.
  void initDisplay();

  //! Sets the brightness of the display.
  //!
  //! @param brightness A number from 0 (lowes brightness) to 255 (highest brightness)
  void setBrightness(uint8_t brightness);

  //! Refresh display
  void update(uint8_t currentDigit);

  void tick(uint32_t now);

  void showScoreline();

  uint8_t *scoreline_format(uint8_t firstPlayer, uint8_t secondPlayer);

  void showServes(uint8_t serves = 0);

  void runningText(const uint8_t text[], uint16_t speed);

  void startupSequence();

  bool textIsRunning = false;

  void setSegments(const bool doublepoint, const uint8_t digits[4]);

  //! Display arbitrary data on the module

  //! @param digit0 - first digit
  //! @param digit1 - second digit
  //! @param digit2 - third digit
  //! @param digit3 - fourth digit
  //! @param doublepoint - show doublepoint or not
  void setSegments(const bool doublepoint, const uint8_t firstDigit, const uint8_t secondDigit,
                   const uint8_t thirdDigit, const uint8_t forthDigit);

private:
  uint8_t _score_myself;
  uint8_t _score_other;
  uint8_t _brightness;
  uint8_t _digit[5];
  uint8_t letter[4];
  uint8_t textLength;
  uint8_t counter1;
  unsigned long prevTime = 0;

  uint16_t _speed;
  uint8_t _text[5];

  void _runningText();

  void _sendByte(uint8_t shiftByte);
};

#endif // __A1001DISPLAY__
