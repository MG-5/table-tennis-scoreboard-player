#include "A1001Display.h"

#if defined(__AVR_ATtiny4313__)
void A1001Display::initDisplay()
{
  // USI - https://playground.arduino.cc/Code/USI-SPI

  // set control pins as outputs and low
  DDRB |= (DATA | CLOCK);
  DDRD |= (STORE | MASTER_RESET);
  PORTB &= ~(DATA | CLOCK);
  PORTD &= ~(STORE | MASTER_RESET);
  DIGIT_DDR |= (0b11111); // 4 Digits + DP
  turnOffDigits();
}

void A1001Display::update(uint8_t currentDigit)
{
  turnOffDigits();

  STORE_PORT &= ~STORE;
  _sendByte(_digit[currentDigit]);
  STORE_PORT |= STORE;
  STORE_PORT &= ~STORE;

  wait_us(10); // source driver maybe to slow so we wait a time

  uint8_t reg = (1 << currentDigit);

  // Doppelpunkt einbauen
  if (currentDigit == 1 && _digit[4])
    reg |= DP_PIN;

  // digit einschalten
  DIGIT_PORT |= reg;
}

void A1001Display::updateAllDigits()
{
  turnOffDigits();

  STORE_PORT &= ~STORE;
  _sendByte(_digit[0]);
  STORE_PORT |= STORE;
  STORE_PORT &= ~STORE;

  uint8_t reg = 0b1111;

  // Doppelpunkt einbauen
  if (_digit[4])
    reg |= DP_PIN;

  // digit einschalten
  DIGIT_PORT |= reg;
}

void A1001Display::resetOnce()
{
  PORTD &= ~MASTER_RESET;
  PORTD |= MASTER_RESET;
}

void A1001Display::turnOffDigits()
{
  DIGIT_PORT &= ~(0b11111); // alle 4 digits + DP abschalten
}

void A1001Display::_sendByte(uint8_t shiftByte)
{
  USIDR = shiftByte;
  USISR = (1 << USIOIF); // clear flag

  while (!(USISR & (1 << USIOIF)))
    USICR = ((1 << USIWM0) | (1 << USICS1) | (1 << USICLK) | (1 << USITC));
}

void A1001Display::setSegments(const bool doublepoint, const uint8_t digits[4])
{
  setSegments(doublepoint, digits[0], digits[1], digits[2], digits[3]);
}

void A1001Display::setSegments(const bool doublepoint, const uint8_t firstDigit, const uint8_t secondDigit,
                               const uint8_t thirdDigit, const uint8_t forthDigit)
{
  _digit[0] = firstDigit;
  _digit[1] = secondDigit;
  _digit[2] = thirdDigit;
  _digit[3] = forthDigit;
  _digit[4] = doublepoint;
}
#endif
