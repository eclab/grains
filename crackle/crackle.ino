// Copyright 2024 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// CRACKLE
///
/// Crackle produces random pops and crackles like a record player or an old-time radio.
/// Crackle is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// Crackle has two outputs: audio and digital.  Audio output generates pops at different volumes,
/// but suffers from GRAINS's low-level PWM whine.  Digital generates clean pops, but they are all at
/// maximum volume and the default value is 0V with the pop at 1V, which means it's not centered for
/// audio and perhaps less useful as a result -- maybe more useful for CV?
///
/// You can control the RATE of crackling, the VARIANCE in crackling volume, and the LENGTH of a crackle.
///
/// VARIANCE 
///
/// At minimum, the variance sets all crackles to zero volume.  As you turn right, crackles will be soft
/// but random in volume from one another.  As you continue to turn right, they get louder until
/// some of them reach maximum volume.  Continuing, the maximum volume crackles will start to
/// prevail until, at far right, about 3/4 of them are maximum volume.
///
/// NOTE
///
/// Crackle is intentionally very slow in responding to pots, in an attempt to cut down a bit on the PWM
/// whine in analog.  It'll take a bit for it to ramp up to your new request.  If you want to speed it up,
/// try setting CONTROL_RATE to 64.

/// CONFIGURATION
///
/// IN 1            Rate CV
/// IN 2            Gain CV
/// IN 3            [UNUSED]
/// AUDIO IN (A)    [UNUSED]
/// AUDIO OUT       Audio Crackles
/// DIGITAL OUT (D) Digital Crackles
///
/// POT 1           Rate
///
/// POT 2           Gain
///
/// POT 3           Length



#define CV_POT_IN1    A2    // Rate
#define CV_POT_IN2    A1    // Gain
#define CV_POT3       A0    // Length
#define CV_IN3        A3    // [Unused]
#define CV_AUDIO_IN   A4    // [Unused]
#define CV_AUDIO_OUT  9     // Analog Crackles
#define CV_GATE_OUT   8     // Digital Crackles
#define RANDOM_PIN    A5

#define CONTROL_RATE 16 		// Very slow!

#include <MozziGuts.h>
#include <mozzi_rand.h>

uint16_t rate = 0;
int16_t gain;
uint16_t length;
int16_t out = 0;
uint16_t countdown = 0;

void setup() 
    {
    // some reasonable initial defaults
    randSeed(RANDOM_PIN);
    startMozzi();
    //Serial.begin(115200);
    }

void updateControl() 
    {
    uint16_t r = mozziAnalogRead(CV_POT_IN1) >> 2;
    rate = (rate + r) >> 1;
    rate += 1;
    gain = mozziAnalogRead(CV_POT_IN2);
    length = mozziAnalogRead(CV_POT3) >> 2;
    }

union _cast { uint16_t unsign; int16_t sign; } cast;
uint8_t crackle = 0;
int updateAudio()    
    {
    if (countdown >= 1)
    	{
    	if (countdown == 1)
			{
			digitalWrite(CV_GATE_OUT, 0);
			crackle = 0;
			}
    	countdown--;
    	}
    else
    	{
		uint32_t rnd = rand((unsigned int)0, (unsigned int)(rate * 64));
		if (rnd == 0)
			{
			crackle = 1;
			digitalWrite(CV_GATE_OUT, 1);
			countdown = 1024;		// bigger than biggest...
			for(int i = 0; i < 4; i++)
				{
				uint16_t c = rand((unsigned int)0, (unsigned int)length) + 1;
				if (countdown > c) countdown = c;
				}
			}
		}
	
	if (crackle)
		{
    	uint32_t rnd = xorshift96();
    	cast.unsign = rnd & 255; // 65535;
    	int16_t r = cast.sign;		// ugh
		r = ((r * rand(gain)) >> 8);
		if (r > 127) r = 127;
		if (r < -128) r = -128;
		return r;
		}
	else return 0;
    }

void loop() 
    {
    audioHook();
    }
