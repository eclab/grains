// Copyright 2024 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// BIT
///
/// BIT is a bitcrusher.  It takes an audio input at AUDIO IN, and reduces its bits
/// via POT 1 and its sampling rate via POT 2.  Then it changes the volume, likely clipping,
/// by POT 3.  The output is sent to AUDIO OUT.  That's it!
///
/// BIT is pretty filthy, aliased, and anharmonic.  If you need to mess something up, it's a good choice.
/// Note that Grains inputs at a resolution of 1024, but outputs at most at a resolution of 488.
/// Thus we're already bitcrushing in the output to begin with!
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.


/// CONFIGURATION
///
/// IN 1            Bits CV
/// IN 2           	Frequency CV
/// IN 3           	UNUSED
/// AUDIO IN (A)    Sound Input
/// AUDIO OUT       Sound Output
/// DIGITAL OUT (D) UNUSED
///
/// POT 1           Bits	[If using Bits CV, set switch to IN1 and set pot to about 2'oclock]
///
/// POT 2           Frequency		[If using Frequency CV, set switch to IN1 and set pot to about 2'oclock]
///
/// POT 3           Volume 


#define CV_POT_IN1    A2    
#define CV_POT_IN2    A1    
#define CV_POT3       A0    
#define CV_IN3        A3    
#define CV_AUDIO_IN   A4    
#define CV_AUDIO_OUT  9     
#define CV_GATE_OUT   8     
#define RANDOM_PIN    A5


#define CONTROL_RATE 32

#define USE_AUDIO_INPUT true
#define AUDIO_INPUT_PIN A4

#include <MozziGuts.h>

void setup() 
    {
    startMozzi();
	setupFastAnalogRead(FASTEST_ADC);
    }

uint8_t bits;		// 0...7
uint8_t freq;		// 0...7
uint8_t gain;		// 0...127
static uint8_t mask[8] = { 255, 255 - 1, 255 - 3, 255 - 7, 255 - 15, 255 - 31, 255 - 63, 255 - 127 };
static uint8_t counters[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };
uint8_t counter = 1;
int16_t lastSample = 0;



void updateControl() 
    {
    bits = mozziAnalogRead(CV_POT_IN1) >> 7;		// 0...7
    freq = mozziAnalogRead(CV_POT_IN2) >> 7;		// 0...7
    gain = mozziAnalogRead(CV_POT3) >> 3;			// 0...127
    }

int updateAudio()    
    {
    // Count it!
    if (--counter > 0)
    	{
    	return lastSample;
    	}
    else
    	{
    	counter = counters[freq];
    	
	    uint8_t input = getAudioInput() >> 2;
    
    	// Crush it!
    	input = input & mask[bits];

		// Gain it!
		lastSample = (((int16_t)(input - 128)) * gain) >> 5;	
		
		int16_t i = lastSample;
		if (i > 243) i = 243;
		if (i < -244) i = -244;
		return i;
		}
    }


void loop() 
    {
    audioHook();
    }
    
