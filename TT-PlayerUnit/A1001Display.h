#pragma once

#include "common/timer0.h"
#include "common/utils.h"
#include "settings.h"
#include <avr/io.h>
#include <stdint.h>

//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---     P - Punkt
//      D

const uint8_t digitToSegment[] = {
    // 0bPGFEDCBA
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
    0b01110111, // A
    0b01111100, // b
    0b00111001, // C
    0b01011110, // d
    0b01111001, // E
    0b01110001  // F
};
#if defined(__AVR_ATtiny4313__)
class A1001Display
{

public:
  //! Initialize a A1001Display object.
  void initDisplay();

  //! Sets the brightness of the display.
  //!
  //! @param brightness A number from 0 (lowes brightness) to 255 (highest brightness)

  //! Refresh display
  void update(uint8_t currentDigit);

  void updateAllDigits();

  void tick(uint32_t now);

  void resetOnce();

  void turnOffDigits();

  void startupSequence();

  void setSegments(const bool doublepoint, const uint8_t digits[4]);

  //! Display arbitrary data on the module

  //! @param doublepoint - show doublepoint or not
  void setSegments(const bool doublepoint, const uint8_t firstDigit = 0, const uint8_t secondDigit = 0,
                   const uint8_t thirdDigit = 0, const uint8_t forthDigit = 0);

private:
  uint8_t _digit[5] = {0, 0, 0, 0, 0};

  void _sendByte(uint8_t shiftByte);
};
#endif
