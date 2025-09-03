// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// SPECTACLE
///
/// SPECTACLE is a light show for your V2 GRAINS.
///
/// SPECTACLE iterates through a random assortment of LED patterns.  You select how fast the
/// LEDs blink in the patterns with POT 3.  If POT3 is at far left, the light show will turn off.

/// CONFIGURATION
///
/// IN 1            [Unused] 
/// IN 2            [Unused] 
/// IN 3            [Unused] 
/// AUDIO IN (A)    [Unused]                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
/// AUDIO OUT       [Unused] 
/// DIGITAL OUT (D) [Unused] 
///
/// POT 1           [Unused] 
///
/// POT 2           [Unused] 
///
/// POT 3           Pattern Speed, or OFF if at far left



/// LEFT SOCKETS IN ORDER
#define CV_POT_IN1    A2
#define CV_POT_IN2    A1
#define CV_IN3        A3
#define CV_AUDIO_IN   A4
#define CV_IN5	      A5

/// ADDITIONAL INPUTS
#define CV_POT3       A0
#define CV_SW1		  3				// Has no pull down resistor.  Use as INPUT_PULLUP.  *NEVER* use as OUTPUT, will destroy it.
#define CV_SW2		  4				// Has no pull down resistor.  Use as INPUT_PULLUP.  *NEVER* use as OUTPUT, will destroy it.
#define CV_SW3		  7				// Has no pull down resistor.  Use as INPUT_PULLUP.  *NEVER* use as OUTPUT, will destroy it.

/// RIGHT SOCKETS IN ORDER
#define CV_AUDIO_OUT  9				// or 11 if GRAINS
#define CV_D10		  10			// Second from top
#define CV_GATE_OUT   8				// "D"
#define CV_RX		  0				// UART
#define CV_TX		  1				// UART
#define CV_LED_1	  2				// "D2" output, third from bottom
#define CV_LED_2	  5				// "D5" output, second from bottom.  Appears to go through a filter
#define CV_LED_3	  6				// "D6" output, bottom.  Appears to go through a filter

/// ADDITIONAL PINOUTS
#define RANDOM_1      A6
#define RANDOM_2      A7


/// LED FUNCTIONS
/// The LEDs are, top to bottom, LED 1, 2, and 3


#define LED_BAR_BLINK_STYLE_FULL 0			// 000 vs 111
#define LED_BAR_BLINK_STYLE_ALTERNATE 1		// 010 vs 101
#define LED_BAR_BLINK_STYLE_UP_DOWN 2		// 100 vs 001
#define LED_BAR_BLINK_STYLE_SINGLE 2		// 000 vs 010

uint8_t ledBarBlink;

inline void clearLEDBar()
	{
	digitalWrite(CV_LED_1, 0);
	digitalWrite(CV_LED_2, 0);
	digitalWrite(CV_LED_3, 0);
	}

inline void resetAndBlinkLEDBar(uint8_t style)
	{
	ledBarBlink = 1;
	blinkLEDBar(style);
	}

inline void blinkLEDBar(uint8_t style)
	{
	ledBarBlink = !ledBarBlink;
	if (style == LED_BAR_BLINK_STYLE_FULL)
		{
		digitalWrite(CV_LED_1, ledBarBlink);
		digitalWrite(CV_LED_2, ledBarBlink);
		digitalWrite(CV_LED_3, ledBarBlink);
		}
	else if (style == LED_BAR_BLINK_STYLE_ALTERNATE)
		{
		digitalWrite(CV_LED_1, ledBarBlink);
		digitalWrite(CV_LED_2, !ledBarBlink);
		digitalWrite(CV_LED_3, ledBarBlink);
		}
	else if (style == LED_BAR_BLINK_STYLE_UP_DOWN)
		{
		digitalWrite(CV_LED_1, !ledBarBlink);
		digitalWrite(CV_LED_2, 0);
		digitalWrite(CV_LED_3, ledBarBlink);
		}
	else	// style == LED_BAR_BLINK_STYLE_SINGLE
		{
		digitalWrite(CV_LED_1, 0);
		digitalWrite(CV_LED_2, ledBarBlink);
		digitalWrite(CV_LED_3, 0);
		}
	}

/// Bar goes 001, 010, 100
inline void setLEDBar3(uint8_t val)
	{
	digitalWrite(CV_LED_1, val >= 2);
	digitalWrite(CV_LED_2, val == 1);
	digitalWrite(CV_LED_3, val == 0);
	}

/// Bar goes 000, 001, 011, 111, 110, 100
/// Also useful as a VU-meter like 000, 001, 011, 111
inline void setLEDBar6(uint8_t val)
	{
	if (val <= 3)
		{
		digitalWrite(CV_LED_1, val > 2);
		digitalWrite(CV_LED_2, val > 1);
		digitalWrite(CV_LED_3, val > 0);
		}
	else if (val == 4)
		{
		digitalWrite(CV_LED_1, 1);
		digitalWrite(CV_LED_2, 1);
		digitalWrite(CV_LED_3, 0);
		}
	else
		{
		digitalWrite(CV_LED_1, 1);
		digitalWrite(CV_LED_2, 0);
		digitalWrite(CV_LED_3, 0);
		}
	}

// Bar is binary: 000, 001, 010, 011, 100, 101, 110, 111
inline void setLEDBar8(uint8_t val)
	{
	digitalWrite(CV_LED_1, (val >> 2) >= 1);
	digitalWrite(CV_LED_2, ((val >> 1) & 1));
	digitalWrite(CV_LED_3, ((val >> 0) & 1));
	}


void setup() 
    {
	pinMode(CV_LED_1, OUTPUT);
	pinMode(CV_LED_2, OUTPUT);
	pinMode(CV_LED_3, OUTPUT);
    }
    
uint16_t DELAY = 300;
uint16_t TIMES = 2;

void up()
	{
	for(int i = 0; i < 4 * TIMES; i++)
		{
		digitalWrite(CV_LED_1, 0);
		digitalWrite(CV_LED_2, 0);
		digitalWrite(CV_LED_3, 1);
		delay(DELAY);
		digitalWrite(CV_LED_1, 0);
		digitalWrite(CV_LED_2, 1);
		digitalWrite(CV_LED_3, 0);
		delay(DELAY);
		digitalWrite(CV_LED_1, 1);
		digitalWrite(CV_LED_2, 0);
		digitalWrite(CV_LED_3, 0);
		delay(DELAY);
		}
	}
	
void down()
	{
	for(int i = 0; i < 4 * TIMES; i++)
		{
		digitalWrite(CV_LED_1, 1);
		digitalWrite(CV_LED_2, 0);
		digitalWrite(CV_LED_3, 0);
		delay(DELAY);
		digitalWrite(CV_LED_1, 0);
		digitalWrite(CV_LED_2, 1);
		digitalWrite(CV_LED_3, 0);
		delay(DELAY);
		digitalWrite(CV_LED_1, 0);
		digitalWrite(CV_LED_2, 0);
		digitalWrite(CV_LED_3, 1);
		delay(DELAY);
		}
	}

void updown()
	{
	for(int i = 0; i < 3 * TIMES; i++)
		{
		digitalWrite(CV_LED_1, 1);
		digitalWrite(CV_LED_2, 0);
		digitalWrite(CV_LED_3, 0);
		delay(DELAY);
		digitalWrite(CV_LED_1, 0);
		digitalWrite(CV_LED_2, 1);
		digitalWrite(CV_LED_3, 0);
		delay(DELAY);
		digitalWrite(CV_LED_1, 0);
		digitalWrite(CV_LED_2, 0);
		digitalWrite(CV_LED_3, 1);
		delay(DELAY);
		digitalWrite(CV_LED_1, 0);
		digitalWrite(CV_LED_2, 1);
		digitalWrite(CV_LED_3, 0);
		delay(DELAY);
		}
	}

void two()
	{
	for(int i = 0; i < 6 * TIMES; i++)
		{
		digitalWrite(CV_LED_1, 1);
		digitalWrite(CV_LED_2, 1);
		digitalWrite(CV_LED_3, 0);
		delay(DELAY);
		digitalWrite(CV_LED_1, 0);
		digitalWrite(CV_LED_2, 1);
		digitalWrite(CV_LED_3, 1);
		delay(DELAY);
		}
	}

void full()
	{
	for(int i = 0; i < 6 * TIMES; i++)
		{
		digitalWrite(CV_LED_1, 1);
		digitalWrite(CV_LED_2, 0);
		digitalWrite(CV_LED_3, 1);
		delay(DELAY);
		digitalWrite(CV_LED_1, 0);
		digitalWrite(CV_LED_2, 1);
		digitalWrite(CV_LED_3, 0);
		delay(DELAY);
		}
	}

void wander()
	{
	int8_t pos = 1;
	for(int i = 0; i < 12 * TIMES; i++)
		{
		if (random() & 1) pos++;
		else pos--;
		if (pos < 0) pos = 2;
		if (pos > 2) pos = 0;

		digitalWrite(CV_LED_1, pos == 0);
		digitalWrite(CV_LED_2, pos == 1);
		digitalWrite(CV_LED_3, pos == 2);
		delay(DELAY);
		}
	}


void randnorm()
	{
	int8_t pos = 0;
	for(int i = 0; i < 12 * TIMES; i++)
		{
		if (pos == 0) pos = (random() & 1) ? 1 : 2;
		else if (pos == 1) pos = (random() & 1) ? 0 : 2;
		else pos = (random() & 1) ? 0 : 1;

		digitalWrite(CV_LED_1, pos == 0);
		digitalWrite(CV_LED_2, pos == 1);
		digitalWrite(CV_LED_3, pos == 2);
		delay(DELAY);
		}
	}


void randblink()
	{
	int8_t pos = 0;
	for(int i = 0; i < 6 * TIMES; i++)
		{
		if (pos == 0) pos = (random() & 1) ? 1 : 2;
		else if (pos == 1) pos = (random() & 1) ? 0 : 2;
		else pos = (random() & 1) ? 0 : 1;

		digitalWrite(CV_LED_1, pos == 0);
		digitalWrite(CV_LED_2, pos == 1);
		digitalWrite(CV_LED_3, pos == 2);
		delay(DELAY);
		digitalWrite(CV_LED_1, 0);
		digitalWrite(CV_LED_2, 0);
		digitalWrite(CV_LED_3, 0);
		delay(DELAY);
		}
	}


void loop() 
    {
    DELAY = analogRead(CV_POT3);
    if (DELAY < 32)
    	{
    	clearLEDBar();
    	delay(5000);
    	}
    else
    	{
		switch(random() & 7)
			{
			case 0: randblink(); break;
			case 1: randnorm(); break;
			case 2: wander(); break;
			case 3: full(); break;
			case 4: two(); break;
			case 5: updown(); break;
			case 6: down(); break;
			case 7: up(); break;
			}
		}
    }
    
