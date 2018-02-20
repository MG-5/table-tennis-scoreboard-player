#include "A1001Display.h"

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

void A1001Display::startupSequence()
{
  uint8_t segment = 1;
  const uint8_t timePassed = 125;
  const uint8_t sequence[7] = {66, 67, 99, 115, 123, 127, 255};
  uint32_t prevTime2 = millis();
  uint8_t i = 0;

  while (i <= 6)
  {
    setSegments(false, segment);

    updateAllDigits();
    _delay_us(100);

    turnOffDigits();
    _delay_us(200);

    if (millis() - prevTime2 >= timePassed)
    {
      segment <<= 1;
      i++;
      prevTime2 = millis();
    }
  }

  i = 0;
  while (i <= 6)
  {
    segment = sequence[i];

    if (i == 6)
      setSegments(true, segment);
    else
      setSegments(false, segment);

    updateAllDigits();
    _delay_us(100);

    turnOffDigits();
    _delay_us(200);

    if (millis() - prevTime2 >= timePassed)
    {
      i++;
      prevTime2 = millis();
    }
  }
  _delay_ms(100);

  // print running "HALLO"
  setSegments(false);
  const uint8_t hallo[] = {118, 119, 56, 56, 63};
  // const uint8_t hallo[] = {64, 0, 0, 0, 0};
  const uint8_t halLength = 5;
  uint8_t idxHal = 0;
  bool running = true;
  prevTime2 = millis();

  while (running)
  {
    if (millis() - prevTime2 >= timePassed * 2)
    {
      _digit[0] = _digit[1];
      _digit[1] = _digit[2];
      _digit[2] = _digit[3];

      if (idxHal < halLength)
        _digit[3] = hallo[idxHal];

      else if (idxHal < halLength + 4)
        _digit[3] = 0;

      else
        running = false;

      prevTime2 = millis();
      idxHal++;
    }

    for (uint8_t i = 0; i <= 3; i++)
    {
      update(i);
      _delay_us(100);
    }
    turnOffDigits();
  }
  setSegments(false);
  resetOnce();
}

void A1001Display::update(uint8_t currentDigit)
{
  turnOffDigits();

  STORE_PORT &= ~STORE;
  _sendByte(_digit[currentDigit]);
  STORE_PORT |= STORE;
  STORE_PORT &= ~STORE;

  _delay_us(10); // source driver maybe to slow so we wait a time

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

void A1001Display::showScoreline(uint8_t left, uint8_t right)
{
  uint8_t players[2] = {left, right};
  uint8_t score[4] = {0, 0, 0, 0};

  for (uint8_t i = 0; i <= 1; i++)
  {
    if (players[i] > 9)
    {
      score[2 * i] = digitToSegment[players[i] / 10];
      score[2 * i + 1] = digitToSegment[players[i] % 10];
    }
    else if (players[i] <= 9)
    {
      if (i == 1)
      {
        score[2 * i] = digitToSegment[players[i]];
        score[2 * i + 1] = 0;
      }
      else
      {
        score[2 * i] = 0;
        score[2 * i + 1] = digitToSegment[players[i]];
      }
    }
  }
  setSegments(true, score[0], score[1], score[2], score[3]);
}

void A1001Display::showServes(uint8_t serves, bool myself)
{
  uint8_t sign = 0;
  if (!myself)
    sign = 64; // Minusstrich

  setSegments(true, 0, digitToSegment[0xA], sign, digitToSegment[serves]);
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
