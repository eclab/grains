// Copyright 2024 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// CHRONO
///
/// CHRONO is a tappable tempo clock.  CHRONO is meant to run on the AE Modular GRAINS, but
/// it could be adapted to any Arduino.
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.
///
/// CHRONO is as simple as it gets: it provides triggers at a constant rate out DIGITAL OUT.
/// You provide taps (gates, triggers) in IN 1 and it will adjust its trigger output rate 
/// appropriately.  On top of it, it'll save that rate so that next time you power up, the
/// rate will still be there.  NOTE: depending on what kind of data was in your EEPROM before,
/// your FIRST rate may be crazy.  Don't worry about it, just tap in a new rate.
///
/// You can also "tap" a rate by twisting POT 1 from a position lower than 50% to a position
/// greater than 50% (and back), if you have the switch set to MAN.
///
/// CHRONO's tempo is taken directly from the difference in your last two taps.  Maybe at some
/// point we might change this to have it average over several taps.  But not for now.  So your
/// tapping will have to be accurate.


/// GRAINS BUG
/// 
/// There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the 
/// switch to "Man", then the range of the Pot is correct.  But if you set the switch 
/// to "In 1" (or "In 2"), then the range of the Dial is maximum (1.0 probability for 
/// our purposes) at about the 2 o'clock position on the Dial.  Beyond that it stays at 1.0
///
///


/// CONFIGURATION 
///
/// IN 1            Tap Tempo
/// IN 2            [Unused]
/// IN 3            [Unused]
/// AUDIO IN (A)    Reset
/// AUDIO OUT       [Unused]
/// DIGITAL OUT (D) Output Trigger
///
/// POT 1           Switch is MAN: Tap Tempo (twist right, then left to "tap")
///                 Switch is IN1: Keep at roughly 2 o'clock position, and use IN1
///
/// POT 2          	[Unused]
///                 
///
/// POT 3           [Unused]
        

#include <EEPROM.h>

#define CONTROL_RATE 128

#define CV_POT_IN1    A2    
#define CV_POT_IN2    A1   
#define CV_POT3       A0 
#define CV_IN3        A3
#define CV_AUDIO_IN   A4 
#define CV_AUDIO_OUT  9 
#define CV_GATE_OUT   8  
#define RANDOM_PIN    A5


#define TRIGGER_COUNTDOWN 96    // sufficieint to successfully trigger the 2ENV
#define MAX_RATE 4294967295

uint32_t rate = MAX_RATE;
uint16_t triggerCountdown = 0;
uint32_t lastTime = 0;
uint32_t lastTapTime = 0;

uint8_t reset = 0;
uint8_t tap = 0;



////////// EEPROM HANDLING

// Loads parameters from EEPROM
void loadEEPROM()
    {
    rate = (EEPROM.read(0) << 0) |
    	   (EEPROM.read(1) << 8) |
    	   (EEPROM.read(2) << 16) |
    	   (EEPROM.read(3) << 24);
    }

// Saves parameters to EEPROM.  Don't do this more than 100,000 times.
void saveEEPROM()
    {
    Serial.println("Save");
    EEPROM.update(0, (rate >> 0) & 255);
    EEPROM.update(1, (rate >> 8) & 255);
    EEPROM.update(2, (rate >> 16) & 255);
    EEPROM.update(3, (rate >> 24) & 255);
    }



void setup() 
    {
    Serial.begin(9600);
    pinMode(CV_GATE_OUT, OUTPUT);
    lastTime = lastTapTime = millis();
    loadEEPROM();
    }

void doReset()
	{
	lastTime = millis();
	digitalWrite(CV_GATE_OUT, 1);
	triggerCountdown = TRIGGER_COUNTDOWN;
	}
	
void doTap()
	{
    uint32_t time = millis();
    rate = time - lastTapTime;
    lastTapTime = time;
    saveEEPROM();
	}

void loop() 
    {
    if (triggerCountdown > 0)
    	{
    	if (triggerCountdown == 1)
    		{
    		digitalWrite(CV_GATE_OUT, 0);
    		}
    	triggerCountdown--;
    	}
    
    uint32_t time = millis();

  if (time >= rate + lastTime)		// not super accurate
    	{
    	lastTime = time;
    	digitalWrite(CV_GATE_OUT, 1);
    	triggerCountdown = TRIGGER_COUNTDOWN;
    	}
    	
    uint8_t _reset = digitalRead(CV_AUDIO_IN);
    if (_reset == 0) 
    	{
    	reset = 0;
    	}
    else if (reset == 0)
    	{
    	reset = 1;
    	doReset();
    	}
    	
    // we do an analog read here rather than a digital read because
    // of switch bounce when turning the knob
    uint16_t _tap = analogRead(CV_POT_IN1);
    if (_tap > 600)
    	{
    	if (tap == 0)
    		{
	    	tap = 1;
    		doTap();
    		}
    	}
    else if (_tap < 400)
    	{
    	tap = 0;
    	}
    }

    
