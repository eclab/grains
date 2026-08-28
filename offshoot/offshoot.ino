// Copyright 2026 Sean Luke 
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License



/// OFFSHOOT
///
/// Offshoot is more or less a copy of Mutable Instruments Branches.  That is, it is a dual
/// Bournoulli Gate.  Offshoot was written by Sean Luke [sean@cs.gmu.edu]
///
/// SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)
///
/// Offshoot has two inputs for triggers or gates (1 and 2).  Each input trigger/gate is randomly routed 
/// to one of two outputs (A and B).  Thus trigger/gate 1 gets routed to either output 1A or 1B,
/// and trigger/gate 2 gets routed to either output 2A or 2B.  The probability of routing
/// is determined by the PROBABILITY knob (Pot 3).
///
/// The probability knob works as follows.  If the knob is turned fully to the left,
/// then the output will always be A.  As you turn the knob to the right, the probability
/// increases that B will be chosen instead of A, until you reach the halfway point, where
/// the probability is equal.  Now if you continue to turn the knob to the right, the
/// behavior changes: the probability is now the likelihood that the output will SWITCH from
/// whatever it was last time.  Initially this is high, and as you turn the knob to the far
/// right the probability decreases to the point where it stays with its previous value
/// for a long time and ultimately permanently.
///
/// NOTE: triggers/gates sent to Audio Out are sloppy and slow.
///
/// FOUR-TRIGGER-GATE / CV OPTION
////
/// You can change Offshoot to select among all four outputs from a single input
/// trigger/gate.  To do this, uncomment (remove the //) from the following #define:

#define FOUR_OUTPUTS 

/// If you do this, then Pot 2 / CV 2 will be used as a SECOND probability knob, to determine 
/// whether Outputs 1A / 1B will be used or if Outputs 2A / 2B will be used.  Then Pot 3 is 
/// used to choose between A and B.  In addition to allowing four outputs, this also allows 
/// CV control over the probability.  For example, if you set POT 3 to FULL LEFT, so A is 
/// always chosen, then you can send CV in to IN2 (set POT 2 to about 2 o'clock) and the CV 
/// will choose between outputs 1A and 2A.
///
/// LATCH MODE
///
/// You can also cause Offshoot to only reset an output (set it to 0) upon triggering a new
/// output.  Mutable Instruments Branches calls this "Latch Mode".  To turn on Latch Mode,
/// uncomment (remove the //) from the following #define:

// #define LATCH_MODE
 


/// CONFIGURATION
///
/// IN 1            Input 1
/// IN 2            Input 2		OR (if Four Outputs) probability for Output Set 1 vs 2
/// IN 3            Output 1B
/// AUDIO IN (A)    Output 2A
/// AUDIO OUT       Output 2B
/// DIGITAL OUT (D) Output 1A
///
/// POT 1           UNUSED.  Set to IN1 and turn dial to approximately 2 o'clock.
/// POT 2           UNUSED IF NOT FOUR OUTPUTS.  Set to IN2 and turn dial to approximately 2 o'clock.
/// POT 3           Probability (Selecting between A and B)




////////// PINOUTS

#define CV_POT_IN1    A2  // Input 1 
#define CV_POT_IN2    A1  // Input 2				OR choice of output 1 set versus 2 set
#define CV_POT3       A0  // Probability			
#define CV_IN3        A3  // Output 1B
#define CV_AUDIO_IN   A4  // Output 2A
#define CV_AUDIO_OUT  11  // Output 2B
#define CV_GATE_OUT   8   // Output 1A
#define RANDOM_PIN    A5

uint8_t prev_in1_state = 255;	// 0 or 1 depending on what the current gate is.  Or 255 initially.
uint8_t prev_in2_state = 255;	// 0 or 1 depending on what the current gate is.  Or 255 initially.
uint8_t out1;               	// 0 or 1 depending on which port is being used.  Or 255 initially.
uint8_t out2;               	// 0 or 1 depending on which port is being used.  Or 255 initially.
uint8_t prev_out1_port = 255;	// this is the actual last port, or 255 initially
uint8_t prev_out2_port = 255;	// this is the actual last port, or 255 initially

#include "rnd.c"

void setup()
    {
    pinMode(CV_AUDIO_IN, OUTPUT);
    pinMode(CV_IN3, OUTPUT);
    pinMode(CV_AUDIO_OUT, OUTPUT);
    pinMode(CV_GATE_OUT, OUTPUT);
    out1 = (rnd4294967294() > 127);
    out2 = (rnd4294967294() > 127);
    }

void loop()
    {
#ifdef FOUR_OUTPUTS
    // Read inputs
    uint16_t in1 = digitalRead(CV_POT_IN1);
    uint16_t prob2 = analogRead(CV_POT_IN2);
    uint16_t prob1 = analogRead(CV_POT3);
    
    // compute probability 
    uint8_t p1 = (uint8_t)(prob1 > 511 ? ((1023 - prob1) >> 2) : (prob1 >> 2));  // 0...511 -> 0...127.  1023...512 -> 0...127
    if (p1 < 8) p1 = 0;		// shunt to 0 to reduce noise for complete shutoff
    uint8_t p2 = (uint8_t)(prob2 > 511 ? ((1023 - prob2) >> 2) : (prob2 >> 2));  // 0...511 -> 0...127.  1023...512 -> 0...127
    if (p2 < 8) p2 = 0;		// shunt to 0 to reduce noise for complete shutoff
    uint8_t evt1 = rnd4294967294();				// random number value
    uint8_t evt2 = rnd4294967294();				// random number value
    
    if (in1 != prev_in1_state)
    	{
    	prev_in1_state = in1;
    	if (in1 == 1)	// trigger
    		{
    		if (prob1 > 511)		// we're alternating
    			{
    			if (evt1 < p1) out1 = !out1;
    			}
    		else				// we're selecting at random
    			{
	    		out1 = (evt1 < p1);
    			}

    		if (prob2 > 511)		// we're alternating
    			{
    			if (evt2 < p2) out2 = !out2;
    			}
    		else				// we're selecting at random
    			{
	    		out2 = (evt2 < p2);
    			}

#ifdef LATCH_MODE
    		if (prev_out1_port != 255) digitalWrite(prev_out1_port, 0);
#endif

			if (!out2)
				{
    			digitalWrite(prev_out1_port = (out1 ? CV_IN3 : CV_GATE_OUT), 1);
				}
			else 
				{
    			digitalWrite(prev_out1_port = (out1 ? CV_AUDIO_OUT : CV_AUDIO_IN), 1);
				}
    		}
    	else
    		{
#ifndef LATCH_MODE
    		if (prev_out1_port != 255) digitalWrite(prev_out1_port, 0);
#endif
    		}
    	}
#else

    // Read inputs
    uint16_t in1 = digitalRead(CV_POT_IN1);
    uint16_t in2 = digitalRead(CV_POT_IN2);
    uint16_t prob = analogRead(CV_POT3);
    
    // compute probability 
    uint8_t p = (uint8_t)(prob > 511 ? ((1023 - prob) >> 2) : (prob >> 2));  // 0...511 -> 0...127.  1023...512 -> 0...127
    if (p < 8) p = 0;		// shunt to 0 to reduce noise for complete shutoff
    uint8_t evt = rnd4294967294();				// random number value
    
    if (in1 != prev_in1_state)
    	{
    	prev_in1_state = in1;
    	if (in1 == 1)	// trigger
    		{
    		if (prob > 511)		// we're alternating
    			{
    			if (evt < p) out1 = !out1;
    			}
    		else				// we're selecting at random
    			{
	    		out1 = (evt < p);
    			}
#ifdef LATCH_MODE
    		if (prev_out1_port != 255) digitalWrite(prev_out1_port, 0);
#endif
    		digitalWrite(prev_out1_port = (out1 ? CV_IN3 : CV_GATE_OUT), 1);
    		}
    	else
    		{
#ifndef LATCH_MODE
    		if (prev_out1_port != 255) digitalWrite(prev_out1_port, 0);
#endif
    		}
    	}
    	
    if (in2 != prev_in2_state)
    	{
    	prev_in2_state = in2;
    	if (in2 == 1)	// trigger
    		{
    		if (prob > 511)		// we're alternating
    			{
    			if (evt < p) out2 = !out2;
    			}
    		else
    			{
	    		out2 = (evt < p);
    			}
#ifdef LATCH_MODE
    		if (prev_out2_port != 255) digitalWrite(prev_out2_port, 0);
#endif
    		digitalWrite(prev_out2_port = (out2 ? CV_AUDIO_OUT : CV_AUDIO_IN), 1);
    		}
    	else
    		{
#ifndef LATCH_MODE
    		if (prev_out2_port != 255) digitalWrite(prev_out2_port, 0);
#endif
    		}
    	}
#endif
	 }
