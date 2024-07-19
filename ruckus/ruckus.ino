// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// RUCKUS
///
/// Ruckus is a noise and sample and hold generator with optional smoothing. 
/// Ruckus is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// MAKING NOISE 
///
/// Ruckus can make pure white noise, pulse noise,
//  or white noise pushed through a 12DB lowpass, high pass, or bandpass filter where you can specify the cutoff
/// frequency and resonance.  Not a lot of resonance, mind you, or the filter is overwhelmed.
///
/// PULSE NOISE
///
/// This is just noise which consists entirely of maximum (5V) or minimum (0V) values.  It sounds
/// something like white noise but I can make it much louder.
///
/// SAMPLE AND HOLD
/// 
/// Ruckus can do three kinds of sample and hold:
///
/// - Random Sample and Hold: when the TRIGGER/GATE is raised, it will sample and hold a RANDOM value
/// - Sample and Hold: when the TRIGGER/GATE is raised, it will sample and hold a value from IN 3
/// - Track and Hold: as long as the TRIGGER/GATE is raised, it will sample and use the value from IN3.
///   as soon as the TRIGGER/GATE is lowered, it will use the last sampled value.
///
/// In these cases, IN1 serves to SCALE the sample and hold signal, and IN2 serves to SHIFT it.
/// A shift of 0 is dead center.
///
/// Note that Sample and Hold only goes up to under 4V or so, due to limitations of Mozzi.  If IN3
/// is inputting a value larger than this, it will be clipped.



/// CONFIGURATION
///
/// IN 1            12DB Filter Cutoff Frequency CV, or Scale
/// IN 2            12DB Resonance CV, or Shift
/// IN 3            Sample and Hold Input
/// AUDIO IN (A)    [UNUSED]
/// AUDIO OUT       Out
/// DIGITAL OUT (D) Trigger / Gate
///
/// POT 1           12DB Filter Cutoff Frequency, or Shift
///					[If unused, set the switch to MAN]
///
/// POT 2           12DB Resonance, or Scale
///					[If unused, set the switch to MAN]
///
/// POT 3           Noise Type: 
///                 White, Pulse, Low Pass, High Pass, Band Pass, 
///                 Random Sample and Hold, Input Sample and Hold, Input Track and Hold



#define CV_POT_IN1    A2    // Filter Cutoff
#define CV_POT_IN2    A1    // Filter Resonance
#define CV_POT3       A0    // Filter Choice
#define CV_IN3        A3    // [Unused]
#define CV_AUDIO_IN   A4    // [Unused]
#define CV_AUDIO_OUT  9     // Out
#define CV_GATE_OUT   8     // [Unused]
#define RANDOM_PIN    A5

#define CONTROL_RATE 256		// Or higher, for T&H?

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
#define CHOICE_RANDOM_SAMPLE_AND_HOLD 5
#define CHOICE_SAMPLE_AND_HOLD 6
#define CHOICE_TRACK_AND_HOLD 7

uint8_t choice = CHOICE_WHITE;
uint16_t cutoff = 65535;
uint16_t resonance = 0;

void setup() 
    {
    // some reasonable initial defaults
    randSeed(RANDOM_PIN);
    pinMode(CV_IN3, INPUT);
    //pinMode(CV_AUDIO_IN, INPUT);
    pinMode(CV_GATE_OUT, INPUT);
    startMozzi();
    filterlp.setCutoffFreqAndResonance(cutoff, resonance);
    filterbp.setCutoffFreqAndResonance(cutoff, resonance);
    filterhp.setCutoffFreqAndResonance(cutoff, resonance);
    }

int16_t toQuasi9Bit(int16_t val)
	{
	return ((val >> 6) * 61) >> 7;
	}

// Scale from -32768...+32767 to -240 ... +240
inline int16_t scaleAudio(int16_t val)
  {
  return ((val >> 4) * 15) >> 7;
  }

// Scale from -32768...+32767 to -240 ... +240
inline int16_t scaleAudioSmall(int16_t val)		// val ranges 0...1023
  {
  return (val * 15) >> 6;
  }

// Scale from 0...+1023 to -244 ... +156, probably close enough?
inline int16_t scaleAudioSmallCV(int16_t val)		// val ranges 0...1023
  {
  return ((val * 25) >> 6) - 244;
  }

// ugh, cast unsigned to signed...	
union _cast { uint16_t unsign; int16_t sign; } cast;
uint16_t hold;
uint16_t lastHold = 0;
uint8_t trigger; 

void updateControl() 
    {
    // Not sure we should bother filtering these, probably not
    choice = mozziAnalogRead(CV_POT3) >> 7;		// 0...7
    switch (choice)
    	{
    	case CHOICE_WHITE:
    	case CHOICE_PULSE:
    		{
    		// Do NOTHING
    		}
    	break;
    	case CHOICE_12DB_LOWPASS:
    	case CHOICE_12DB_HIGHPASS:
    	case CHOICE_12DB_BANDPASS:
			{
			cutoff = mozziAnalogRead(CV_POT_IN1) << 6;
			resonance = mozziAnalogRead(CV_POT_IN2)  << 4;		// high resonance overwhelms the filter
			filterlp.setCutoffFreqAndResonance(cutoff, resonance);
			filterbp.setCutoffFreqAndResonance(cutoff, resonance);
			filterhp.setCutoffFreqAndResonance(cutoff, resonance);
			}
		break;
		case CHOICE_RANDOM_SAMPLE_AND_HOLD:
			{
			uint8_t high = digitalRead(CV_GATE_OUT);
			if (high && !trigger)
				{
        uint8_t scale = mozziAnalogRead(CV_POT_IN1) >> 5;
        int16_t shift = mozziAnalogRead(CV_POT_IN2);
        int16_t val = ((((xorshift96() & 1023) - 512) * scale) >> 4) + shift;
        if (val < 0) val = 0; 
        if (val > 1023) val = 1023;
        	lastHold = hold;
				hold = val;
        trigger = 1;
				}
			else if (!high) trigger = 0;
			}
		break;
		case CHOICE_SAMPLE_AND_HOLD:
			{
			uint8_t high = digitalRead(CV_GATE_OUT);
			if (high && !trigger)
				{
        uint8_t scale = mozziAnalogRead(CV_POT_IN1) >> 5;
        int16_t shift = mozziAnalogRead(CV_POT_IN2);
        int16_t val = (((mozziAnalogRead(CV_IN3) - 512) * scale) >> 4) + shift;
        if (val < 0) val = 0; 
        if (val > 1023) val = 1023;
        lastHold = hold;
        hold = val;
        trigger = 1;
				}
			else if (!high) trigger = 0;
			}
		break;
		case CHOICE_TRACK_AND_HOLD:
			{
			uint8_t high = digitalRead(CV_GATE_OUT);
			if (high)
				{
        uint8_t scale = mozziAnalogRead(CV_POT_IN1) >> 5;
        int16_t shift = mozziAnalogRead(CV_POT_IN2);
        int16_t val = (((mozziAnalogRead(CV_IN3) - 512) * scale) >> 4) + shift;
        if (val < 0) val = 0; 
        if (val > 1023) val = 1023;
        hold = val;
				}
			}
		break;
		
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

int updateAudio()    
    {
    uint32_t rnd = xorshift96();
        	
    cast.unsign = rnd & 65535;
    int16_t r = cast.sign;		// ugh

    switch(choice)
    	{
    	case CHOICE_WHITE:
    	return scaleAudio(r);
    	case CHOICE_PULSE:
    	return (r >= 0 ? 243 : -244);
    	break;
    	case CHOICE_12DB_LOWPASS:
    	return scaleAudio(filterlp.next(r >> 1) * 2);
    	break;
    	case CHOICE_12DB_HIGHPASS:
    	return scaleAudio(filterhp.next(r >> 1) * 2);
    	break;
    	case CHOICE_12DB_BANDPASS:
    	return scaleAudio(filterbp.next(r >> 1) * 2);
    	break;
    	case CHOICE_RANDOM_SAMPLE_AND_HOLD:
    	case CHOICE_SAMPLE_AND_HOLD:
    	case CHOICE_TRACK_AND_HOLD:
    	return scaleAudioSmallCV(hold);
    	break;
    	}
    return 0;		// uh....
    }

void loop() 
    {
    audioHook();
    }
