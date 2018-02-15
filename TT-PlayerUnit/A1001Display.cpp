
#include "A1001Display.h"
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#define MAX_VALUE 5 // HALLO -> 5

#define DELAY 5

//
//      A
//     ---
//  F |   | B    P - Punkt
//     -G-
//  E |   | C
//     ---
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

void A1001Display::initDisplay()
{

  // https://playground.arduino.cc/Code/USI-SPI

  // USI
  //set control pins as outputs and low
  DDRB|=(DATA|CLOCK);
  DDRD|=(STORE|MASTER_RESET);
  PORTB &= ~(DATA|CLOCK);
  PORTD &= ~(STORE|MASTER_RESET);
   


  // SPI
  /*
          SHIFT_REGISTER |= (DATA | STORE_PIN_P1 | CLOCK);	//Set control pins as outputs
          SHIFT_PORT &= ~(DATA | STORE_PIN_P1 | CLOCK);		//Set control pins low

          SPI_CONTROL = (1 << SPE) | (1 << MSTR);	//Start SPI as Master
          SPSR |= 1;
  */
}
/*
void A1001Display::startupSequence()
{
uint8_t secondShiftByte = 1;

for (uint8_t i = 0; i <= 5; i++)
{
uint8_t firstShiftByte = ~(0b11110);

SHIFT_PORT &= ~STORE_PIN_P1;

sendByte(secondShiftByte);
sendByte(firstShiftByte);

SHIFT_PORT |= STORE_PIN_P1;
SHIFT_PORT &= ~STORE_PIN_P1;

secondShiftByte = (secondShiftByte << 1);

_delay_ms(150);
}

secondShiftByte = 1;
for (int i = 0; i <= 7; i++)
{

uint8_t firstShiftByte = ~(0b11110);

if (i == 7)
firstShiftByte = ~(0b11111);

SHIFT_PORT &= ~STORE_PIN_P1;

sendByte(secondShiftByte);
sendByte(firstShiftByte);

SHIFT_PORT |= STORE_PIN_P1;
SHIFT_PORT &= ~STORE_PIN_P1;

secondShiftByte = (secondShiftByte << 1);

_delay_ms(150);
}

sendByte(0);
sendByte(~0);

SHIFT_PORT |= STORE_PIN_P1;
SHIFT_PORT &= ~STORE_PIN_P1;

_delay_ms(500);
}
*/
void A1001Display::update(uint8_t currentDigit)
{
  DIGIT_PORT &= 0b0000; // alle 4 digits abschalten

  SHIFT_PORT &= ~STORE_PIN_P1;
  SHIFT_PORT &= ~STORE_PIN_P2;

  // Player1 display
  sendByte(_digit_playerOne[currentDigit]);
  SHIFT_PORT |= STORE_PIN_P1;
  SHIFT_PORT &= ~STORE_PIN_P1;

  // Player2 display
  sendByte(_digit_playerTwo[currentDigit]);
  SHIFT_PORT |= STORE_PIN_P2;
  SHIFT_PORT &= ~STORE_PIN_P2;

  // Doppelpunkt einbauen
  if (currentDigit == 1)
  {
    if (_digit_playerOne[4] == true)
    {
      SHIFT_PORT |= (1 << DP_PIN_P1);
    }
    else
    {
      SHIFT_PORT &= ~(1 << DP_PIN_P1);
    }

    if (_digit_playerTwo[4] == true)
    {
      SHIFT_PORT |= (1 << DP_PIN_P2);
    }
    else
    {
      SHIFT_PORT &= ~(1 << DP_PIN_P2);
    }
  }

  // digit einschalten
  DIGIT_PORT |= (1 << currentDigit);
}

void A1001Display::tick(uint32_t now)
{

  // if(!true)
  //{
  // sendByte(0);
  // sendByte(~0);

  // SHIFT_PORT |= STORE;
  // SHIFT_PORT &= ~STORE;

  //_delay_ms(DELAY);

  //}

  if (textIsRunning)
  {
    if ((now - prevTime) >= _speed)
    {
      prevTime = now;
      _runningText();
    }
  }
}

void A1001Display::sendByte(uint8_t shiftByte)
{
  SPDR = shiftByte; // data
  while (!(SPSR & (1 << SPIF)))
    ; // Wait for SPI process to finish
}

uint8_t[] A1001Display::scoreline_format(uint8_t firstPlayer, uint8_t secondPlayer)
{
  uint8_t players[2] = {firstPlayer, secondPlayer};
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

  return score;
}

void A1001Display::showScoreline_playerOne()
{
  setSegments_playerOne()

      uint8_t players[2] = {_score_playerOne, _score_playerTwo};
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
  setSegments_playerOne(true, score[0], score[1], score[2], score[3]);
}

void A1001Display::showScoreline_playerTwo()
{
  uint8_t players[] = {_score_playerTwo, _score_playerOne};
  uint8_t score[] = {0, 0, 0, 0};
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
  setSegments_playerOne(true, score[0], score[1], score[2], score[3]);
}

void A1001Display::showServes(uint8_t serves)
{
  setSegments_playerOne(true, 0, digitToSegment[0xA], 0, digitToSegment[serves]);
}

void A1001Display::setBrightness(uint8_t brightness) { _brightness = brightness; }
void A1001Display::setSegments_playerOne(const bool doublepoint, const uint8_t digits[5])
{
  setSegments_playerOne(doublepoint, digits[0], digits[1], digits[2], digits[3]);
}

void A1001Display::setSegments_playerOne(const bool doublepoint, const uint8_t firstDigit, const uint8_t secondDigit,
                                         const uint8_t thirdDigit, const uint8_t forthDigit)
{
  if (doublepoint)
    _digit_playerOne[4] = true;
  else
    _digit_playerOne[4] = false;

  _digit_playerOne[0] = firstDigit;
  _digit_playerOne[1] = secondDigit;
  _digit_playerOne[2] = thirdDigit;
  _digit_playerOne[3] = forthDigit;
}

void A1001Display::runningText(const uint8_t text[MAX_VALUE], uint16_t speed)
{
  for (int i = 0; i <= 5; i++)
  {
    _text[i] = text[i];
  }

  _speed = speed;
  textLength = MAX_VALUE;
  counter1 = 0;
  textIsRunning = true;
}

void A1001Display::_runningText()
{
  if (counter1 < textLength)
  {
    letter[0] = letter[1];
    letter[1] = letter[2];
    letter[2] = letter[3];
    letter[3] = _text[counter1];
  }

  else if (counter1 < textLength + 4)
  {
    letter[0] = letter[1];
    letter[1] = letter[2];
    letter[2] = letter[3];
    letter[3] = 0;
  }
  else
  {
    textIsRunning = false;
  }

  setSegments_playerOne(false, letter[0], letter[1], letter[2], letter[3]);
  counter1++;
}
