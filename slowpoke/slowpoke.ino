// Copyright 2026 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// SLOWPOKE
///
/// Slowpoke is an EXTREMELY slow loopable AR envelope with a random option.  How slow?  Its fastest attack 
/// or release rate (besides instantaneous) is a bit over 3 seconds, and its slowest rate is 29.5 hours.
/// Stochastic is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.
///
/// You set the attack and release rates with the POT 1 and POT 2.  Thereafter when it is triggered,
/// Slowpoke will slowly start rising linearly from a MINIMUM value until it reaches a MAXIMUM value, then 
/// will slowly drop back down linearly to the MINIUMUM value.  When it reaches the MINIMUM value again, it
/// issues a trigger out to END and stays at the MINIUMUM value.
///
/// If you would like a one-shot envelope, you can do this by sending a trigger to TRIGGER.  If you would like a 
/// looping envelope, attach the END to one of the TRIGGERs, and send a trigger to the other TRIGGER.
/// When the envelope has finished, END will trigger the envelope to start again.  You stop the looping by 
/// disconnecting END from the TRIGGER -- the envelope will stop after it finishes its current attack and decay. 
///
/// You specify the MINIMUM and MAXIMUM with POT 3.  If POT3 is low, then the MINIMUM is 0, and POT3
/// sets the MAXIMUM value.  When the pot is roughly 40% the highest possible MAXIMUM value (about 4.25 volts)
/// is reached.  Between 40% and 60%, the minimum stays at 0 and the maximum stays at the highest value.
/// Then beyond 60% the MINIMUM begins to rise from 0 until it too reaches 4.25 volts when the pot is 100%.
///
/// If POT 1 and POT 2 are BOTH set to MAXIMUM ("Instantaneous"), then Slowpoke turns into a RANDOM RAMP:
/// When you trigger it to start, it will pick a random voltage value and slowly move towards that value.  When
/// it reaches the value, it will issue an END trigger.  If you trigger it gain (looping for example), it
/// will start at that position and move towards a new random value, and so on.  The rate of the ramp is
/// now determined by POT 3.
///
/// The rate of the attack and release (or ramp) depend in part on the settings of POT 1 and POT 2, and in
/// part on the distance that Slowpoke must cover between the current MINIMUM and MAXIMUM.  If they are
/// far apart (MINIMUM = 0, MAXIMUM = 4.25 volts or so, then the rate values that POT 3 sets are as follows:

/// 6.4843750 Seconds
/// 12.968750 Seconds
/// 25.937500 Seconds
/// 51.875000 Seconds
/// 1.7291666 Minutes
/// 3.4583333 Minutes
/// 6.9166665 Minutes
/// 13.833333 Minutes
/// 27.666666 Minutes
/// 55.333332 Minutes
/// 1.8444444 Hours
/// 3.6888888 Hours
/// 7.3777776 Hours
/// 14.755555 Hours
/// 29.511110 Hours
/// Instantaneous

/// If the distance between MINIMUM and MAXIMUM is smaller than this, then the rates are proportionally less.

/// GRAINS BUG
/// 
/// There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the 
/// switch to "Man", then the range of the Pot is correct.  But if you set the switch 
/// to "In 1" (or "In 2"), then the range of the Dial is at maximum at about the 
/// 2 o'clock position on the Dial.  Beyond that it stays at maximum.


/// CONFIGURATION
///
/// IN 1            Attack Rate CV
/// IN 2            Decay Rate CV
/// IN 3            Trigger 1
/// AUDIO IN (A)    Trigger 2
/// AUDIO OUT       Output
/// DIGITAL OUT (D) End
///
/// POT 1           Attack Rate  [If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
///
/// POT 2           Decay Rate  [If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
///
/// POT 3           Shape:   [Max = 0V, Min = 0V <-------> Max = 4.25V, Min = 0 <--------> Max = 4.25V, Min = 4.25V]
///                 Or Random Ramp Rate (if Attack and Decay both Max)




#define CONTROL_RATE 256

#include <MozziGuts.h>

#define CV_POT_IN1    A2    // Rise
#define CV_POT_IN2    A1    // Fall
#define CV_POT3       A0    // Shape
#define CV_IN3        A3    // Trigger 1
#define CV_AUDIO_IN   A4    // Trigger 2
#define CV_AUDIO_OUT  9     // Output
#define CV_GATE_OUT   8     // End
#define RANDOM_PIN    A5

uint32_t ticks[16] = { 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 0 };

void setup() 
    {
    startMozzi();
    }

uint32_t tick = 0;
uint32_t position = 0;
uint32_t target = 0;
uint8_t rising = true;
uint8_t endCountdown = 0;
uint8_t reset = 0;
uint8_t running = false;

uint8_t trigger1 = 0;
uint8_t trigger2 = 0;

#define END_COUNTDOWN (4)


uint16_t randomPosition()
	{
	for(uint8_t i = 0; i < 8; i++)
		{
		uint8_t rnd = random(294) - 147;
		int16_t newPosition = position + rnd;
		if (newPosition > 294 || newPosition < 0) 
			{
			newPosition = position - rnd;
			}
		if (newPosition <= 294 && newPosition >= 0) 
			{
			return newPosition;
			}
		}
	return random(294);
	}

void updateControl() 
    {
    // Lower the END trigger if needed
    if (endCountdown > 0)
    	{
    	endCountdown--;
    	if (endCountdown == 0)
    		{
	    	digitalWrite(CV_GATE_OUT, 0);
	    	}
    	}
    
    // Get the three pot values
    uint32_t attack = ticks[mozziAnalogRead(CV_POT_IN1) >> 6];
    uint32_t release = ticks[mozziAnalogRead(CV_POT_IN2) >> 6];
    uint16_t shape = mozziAnalogRead(CV_POT3);

	// How long do we stay on any one value?
    uint32_t count = 0;
    if (attack == 0 && release == 0)
    	{
    	count = ticks[shape >> 6];
    	}
    else if (rising)
    	{
    	count = attack;
    	}
    else
    	{
    	count = release;
    	}
    
    // Update min and max
    uint16_t max = 0;
    uint16_t min = 0;
    if (shape <= 471)
    	{
    	max = (shape * 5) >> 3;			// [0...471 -> 0...294]
    	min = 0;
    	}
    else if (shape < 552)
    	{
    	max = 294;
    	min = 0;
    	}
    else
    	{
    	max = 294;
    	min = 294 - (((1023 - shape) * 5) >> 3);		// [552...1023 -> 0...294]
    	}

	// revise target
	if (attack != 0 && release != 0)
		{
		if (rising)
			{
			target = max;
			}
		else
			{
			target = min;
			}
		}
    
    // Advance the count
    tick++;
    if (tick >= count)		// we need to move the value towards the target
    	{
    	tick = 0;
    	
    	// move position
    	if (!running) { } 	// do nothing
    	else if (position > target && 
    		((attack == 0 && release == 0) || 		// do this when random 
    		((attack != 0 || release != 0) && count != 0)))		// do this when NOT random but the count is != 0
    		position--;		// not instantaneous, not random
    	else if (position < target && ((attack == 0 && release == 0) || ((attack != 0 || release != 0) && count != 0))) position++;		// not instantaneous
    	else
    		{
    		if ((attack != 0 || release != 0) && count == 0) position = target;
    		
    		// We need to pick a new target
    		if (attack == 0 && release == 0)
    			{
    			digitalWrite(CV_GATE_OUT, 1);
    			endCountdown = END_COUNTDOWN;
    			running = false;
    			}
    		else if (rising)
    			{
    			target = min;
    			rising = false;
    			}
    		else
    			{
    			target = max;
    			rising = true;
    			digitalWrite(CV_GATE_OUT, 1);
    			endCountdown = END_COUNTDOWN;
    			running = false;
    			}
    		}
    	}

	// Did we receive a reset trigger?  If so, reset.
    
    uint8_t newTrig1 = 0;
    uint8_t newTrig2 = 0;
    uint16_t tr1 = mozziAnalogRead(CV_IN3);
    if (tr1 >= 512 && !trigger1)
    	{
    	trigger1 = 1;
    	newTrig1 = 1;
    	}
    else if (tr1 < 256)
    	{
    	trigger1 = 0;
    	}
    	
    uint16_t tr2 = mozziAnalogRead(CV_AUDIO_IN);
    if (tr2 >= 512 && !trigger2)
    	{
    	trigger2 = 1;
    	newTrig2 = 1;
    	}
    else if (tr2 < 256)
    	{
    	trigger2 = 0;
    	}
    
    if (newTrig1 || newTrig2)
    	{
    	// reset but don't issue a trigger out
    	tick = 0;
    	running = true;
		// We need to pick a new target
		if (attack == 0 && release == 0)
			{
			target = randomPosition();
			rising = true;
			}
		else
			{
			target = max;
			rising = true;
			}
    	}
    }

int updateAudio()    
    {
    return position - 128;
    }

void loop() 
    {
    audioHook();
    }
