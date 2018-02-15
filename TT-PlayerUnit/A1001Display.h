#ifndef __A1001DISPLAY__
#define __A1001DISPLAY__

#include "settings.h"
#include <stdint.h>

class A1001Display {

	public:
	//! Initialize a A1001Display object.
	void initDisplay();
	
	//! Sets the brightness of the display.
	//!
	//! @param brightness A number from 0 (lowes brightness) to 255 (highest brightness)
	void setBrightness(uint8_t brightness);
	
	//! Refresh display
	void tick(uint32_t now);
	
	void sendByte(uint8_t shiftByte);
	
	void showScoreline_playerOne(uint8_t scoreFirstPlayer=0,uint8_t scoreSecondPlayer=0);
	
	//! Display arbitrary data on the module
	
	//! @param digit0 - first digit
	//! @param digit1 - second digit
	//! @param digit2 - third digit
	//! @param digit3 - fourth digit
	//! @param doublepoint - show doublepoint or not
	
	void showServes(uint8_t serves=0);
	
	void setSegments_playerOne(const bool doublepoint, const uint8_t digit1, const uint8_t digit2, const uint8_t digit3, const uint8_t digit4);
	
	void runningText(const uint8_t text[],uint16_t speed);
	
	void startupSequence();
	
	bool textIsRunning=false;
	
	private:
	uint8_t _score_playerOne;
	uint8_t _score_playerTwo;
	uint8_t _brightness;
	uint8_t _digit_playerOne[5];
	uint8_t _digit_playerTwo[5];
	uint8_t letter[4];
	uint8_t textLength;
	uint8_t counter1;
	unsigned long prevTime=0;
	
	uint16_t _speed;
	uint8_t _text[5];
	
	void _runningText();
};

#endif // __A1001DISPLAY__
