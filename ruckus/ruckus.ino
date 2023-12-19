// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License
//
// WARNING: Mozzi itself is released under a broken non-open-source license, namely the 
// Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
// This license is not compatible with the LGPL (used by the Arduino itself!) and is
// also viral, AND is non-commercial only.  What a mess.  I am pushing them to change 
// their license to something reasonable like Apache or GPL but in the meantime I don't 
// have much choice but to turn my head and ignore the broken license.  So I'm releasing
// under Apache for the time being.


/// RUCKUS
///
/// Ruckus is a noise generator.  
/// Ruckus is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// Ruckus can make pure white noise, pulse noise,
//  or white noise pushed through a 12DB lowpass, high pass, or bandpass filter where you can specify the cutoff
/// frequency and resonance.  Not a lot of resonance, mind you, or the filter is overwhelmed.
///
///
/// PULSE NOISE
///
/// This is just noise which consists entirely of maximum (5V) or minimum (0V) values.  It sounds
/// something like white noise but I can make it much louder.

/// CONFIGURATION
///
/// IN 1            12DB Filter Cutoff Frequency
/// IN 2            12DB Resonance
/// IN 3            [UNUSED]
/// AUDIO IN (A)    [UNUSED]
/// AUDIO OUT       Out
/// DIGITAL OUT (D) [UNUSED]
///
/// POT 1           12DB Filter Cutoff Frequency CV
///					[If unused, set the switch to MAN]
///
/// POT 2           12DB Resonance CV
///					[If unused, set the switch to MAN]
///
/// POT 3           Filter Choice: White, Pulse, Low Pass, High Pass, Band Pass


#define CV_POT_IN1    A2    // Filter Cutoff
#define CV_POT_IN2    A1    // Filter Resonance
#define CV_POT3       A0    // Filter Choice
#define CV_IN3        A3    // [Unused]
#define CV_AUDIO_IN   A4    // [Unused]
#define CV_AUDIO_OUT  9     // Out
#define CV_GATE_OUT   8     // [Unused]
#define RANDOM_PIN    A5

#define CONTROL_RATE 64

#include <MozziGuts.h>
#include <mozzi_rand.h>
#include <ResonantFilter.h>

ResonantFilter<LOWPASS,uint16_t> filterlp;
ResonantFilter<BANDPASS,uint16_t> filterbp;
ResonantFilter<HIGHPASS,uint16_t> filterhp;

#define CHOICE_WHITE 0
#define CHOICE_PULSE 1
#define CHOICE_12DB_LOWPASS 2
#define CHOICE_12DB_HIGHPASS 3
#define CHOICE_12DB_BANDPASS 4

uint8_t choice = CHOICE_WHITE;
uint16_t cutoff = 65535;
uint16_t resonance = 0;

void setup() 
    {
    // some reasonable initial defaults
    randSeed(RANDOM_PIN);
    pinMode(CV_IN3, OUTPUT);
    pinMode(CV_AUDIO_IN, OUTPUT);
    pinMode(CV_GATE_OUT, OUTPUT);
    startMozzi();
    filterlp.setCutoffFreqAndResonance(cutoff, resonance);
    filterbp.setCutoffFreqAndResonance(cutoff, resonance);
    filterhp.setCutoffFreqAndResonance(cutoff, resonance);
    }

void updateControl() 
    {
    // Not sure we should bother filtering these, probably not
    choice = (mozziAnalogRead(CV_POT3) * 5) >> 10;
    if (choice >= CHOICE_12DB_LOWPASS)
    	{
	    cutoff = mozziAnalogRead(CV_POT_IN1) << 6;
    	resonance = mozziAnalogRead(CV_POT_IN2)  << 4;		// high resonance overwhelms the filter
		filterlp.setCutoffFreqAndResonance(cutoff, resonance);
		filterbp.setCutoffFreqAndResonance(cutoff, resonance);
		filterhp.setCutoffFreqAndResonance(cutoff, resonance);
		}
    }
    
/**
// I tried do pink noise via 3DB filter, but it ain't working....

float b0 = 0.0;
float b1 = 0.0;
float b2 = 0.0;

// Not fast enough for Mozzi?  Dunno.  Would really rather not do fixnum math.
float filter3DB(float val)
	{
	b0 = 0.99765 * b0 + 0.0990460 * val;
	b1 = 0.96300 * b1 + 0.2965164 * val;
	b2 = 0.57000 * b2 + 1.0526913 * val;
	return (b0 + b1 + b2 + 0.1848 * val);
	}
*/

int16_t toQuasi9Bit(int16_t val)
	{
	return ((val >> 6) * 61) >> 7;
	}


// ugh, cast unsigned to signed...	
union _cast { uint16_t unsign; int16_t sign; } cast;

int updateAudio()    
    {
    uint32_t rnd = xorshift96();
        	
    cast.unsign = rnd & 65535;
    int16_t r = cast.sign;		// ugh

    switch(choice)
    	{
    	case CHOICE_WHITE:
    	return MonoOutput::from16Bit(r); // MonoOutput::from16Bit(r);
    	case CHOICE_PULSE:
    	return (r >= 0 ? 243 : -244);
    	break;
    	case CHOICE_12DB_LOWPASS:
    	return MonoOutput::from16Bit(filterlp.next(r >> 1) * 2);
    	break;
    	case CHOICE_12DB_HIGHPASS:
    	return MonoOutput::from16Bit(filterhp.next(r >> 1) * 2);
    	break;
    	case CHOICE_12DB_BANDPASS:
    	return MonoOutput::from16Bit(filterbp.next(r >> 1) * 2);
    	break;
    	}
    return 0;		// uh....
    }

void loop() 
    {
    audioHook();
    }
