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


/// MULTILEVEL
///
/// Multilevel is a multistage envelope with exponential rate-based transitions, sustain
/// loops, whole-envelope loops, resetting, and per-stage triggering as options.   Multilevel  
/// is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode, though it could probably be adapted to it.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.
///
/// Note: Multilevel is an **exponential** rate-based envelope.  This means it'll sound best 
/// with stuff like filters, and won't be amazing for VCAs.  I might build a **linear** 
/// time-based envelope later for the VCA but not now.
///
/// How many stages are we talking here?  At present you can probably have about 145
/// stages.  If you needed more, we could probably convert the stages to PROGMEM and
/// that would get you another 3000 stages or so.  But I'm guessing 145 would be
/// more than plenty!
///
/// In Multilevel you define the envelope here in code.  It's not too hard.  Then in 
/// real time you can change the OPTIONS, the MAXIMUM LEVEL, and the MINIMUM LEVEL.
/// (If the maximum level is less than the minimum level, the two are swapped).
/// When the envelope is GATED it starts its run.  When it completes the SUSTAIN stage,
/// it either holds there, or it loops back to the LOOP START STAGE, depending on 
/// whether we have turned LOOPING on.  When the gate is RELEASED, the envelope jumps
/// jumps to the stage just beyond the SUSTAIN stage, and continues from there until
/// it has terminated.
///
/// Multilevel has three MODES.  The first mode is REGULAR, which acts like a regular
/// envelope with sustain and possibly a looping sustain.  The second mode is ONE SHOT, 
/// which ignores sustain and looping and just progresses through the envelope until it reaches the
/// end.  The third mode is STAGE TRIGGERING, which does not advance from stage to stage
/// unless it receives a STAGE TRIGGER. There is no sustain or sustain loop.  
/// STAGE TRIGGERING does not use GATE so the trigger is located in that socket.  
///
/// You can also RESET the envelope.  As long as the GATE is ON, resetting will cause the
/// envelope to reset to its starting position and continue from there.  If the mode
/// is SUSTAIN TRIGGERING then reset works at any time (since there is no gate).
///
/// When the envelope terminates, it triggers the ENVELOPE COMPLETION TRIGGER.  Among other
/// things, you can connect the envelope completion trigger to the reset in a ONE SHOT 
/// envelope, and it will loop forever as long as the gate is ON.
///
/// We can also state whether our stages are FLAT or CHANGING.  A FLAT stage is kind of like
/// a Sample and Hold: the stage immediately goes to and outputs its target value until the
/// underlying curve has reached the target value as usual. 
///
/// Pot 3 is used to change the mode and to set whether we are flat or not.

/// When setting up the envelope, the first 
/// thing you have to define is the number of STAGES.  For example, in an ADSR envelope
/// there are THREE STAGES: ATTACK, DECAY, and RELEASE.  SUSTAIN is not a stage in and of
/// itself
 
#define NUM_STAGES 			3				// You can make as large as you saw fit, but not less than 1

/// Next you need to define the SUSTAIN STAGE.  This is the stage which, after it completes,
/// the system either SUSTAINS or LOOPS BACK to an earlier stage.  In a simple ADSR envelope,
/// the sustain stage is the DELAY STAGE, which is stage #1 (ATTACK is stage #0 -- we start at 0).
/// When STAGE TRIGGERING or ONE SHOT, we do not sustain, but you should still define this.

#define SUSTAIN_STAGE 		1				// This value must be >= 0 and < NUM_STAGES

/// Next you need to define the LOOP START STAGE.  If we are looping, then when the sustain
/// stage is COMPLETED, we loop back to the LOOP START STAGE and go from there.  If we are NOT
/// looping, this is indicated by setting the loop start stage to be equal to the SUSTAIN STAGE.
/// Do not set the loop start stage to a value greater than the SUSTAIN STAGE.
///
/// Since we are not looping, we have set the LOOP_START_STAGE to SUSTAIN_STAGE.  If we set it to, 
/// say, 0 in this example, we'd loop back and forth between the decay and attack stages until 
/// the musician released.

#define LOOP_START_STAGE	SUSTAIN_STAGE	// set this to SUSTAIN_STAGE, or to a value >= 0 and <= SUSTAIN_STAGE

/// Now we define our envelope, which consists of NUM_STAGES stages.  Each stage has two numbers:
///
/// 1. The RATE at which we complete the stage.  This is a value from 0.0 to 1.0, not including 1.0.
///    Higher values are SLOWER.  For the time being the rate is exponential.  Here's a rough guide:
///
///    0.0				Instantaneous
///    0.9				7 ms
///    0.99				0.07 Seconds
///    0.999			0.7 Seconds
///    0.9999			7 Seconds
///    0.99999			70 Seconds
///    0.999999			700	Seconds (about 11.6 minutes)
///    0.9999999		You're kidding, right?  700 Seconds, a bit less than 2 hours
///    0.99999991		Roughly the limit, after this it's just equal to 1.0
///    1.0 				(Forever.  This isn't super useful.)
///
///    I'll work on a more useful way to enter rates.  Keep in mind that the actual value will also vary depending on
///	   how far the stage has to travel to get to the next target level (below).  Short distances are traversed
///    slightly faster (it's not linear).  [I'll work on eliminating this constraint]
///
/// 2. The TARGET LEVEL at the end of the stage.  This is a value from 0.0 to 1.0.  In a typical
///    ADSR envelope, Attack's target is often 1.0 (maximum), and then the Decay's target is the
///    sustain level.  Then the Release's target is often 0.0 (minimum).
///
///
/// ADSR EXAMPLE
/// In this default example, our sustain level is 0.5, we have an immediate attack and a long decay and
/// slightly longer release.  Decay is the sustain stage.  Thus we have:

float stages[NUM_STAGES][2] =
	{
	//RATE //TO LEVEL
	{ 0.0, 1.0 },				// Stage 0 (Attack) Immediate attack to a level of 1.0.
	{ 0.9999, 0.5 },				// Stage 1 (Decay) Long decay to a sustain level of 0.5.  Thereafter we sustain because this is the SUSTAIN STAGE
	{ 0.99992, 0.0 },			// Stage 2 (Release) Longer release to a level of 0.0.
	};

/// A MORE ELABORATE EXAMPLE
///
/// Here we have 6 stages, and while we are sustained, we loop through four of them.
/// Important: notice that 0 <= LOOP_START_STAGE <= SUSTAIN_STAGE < NUM_STAGES
/// 
///   #define NUM_STAGES		6
///   #define LOOP_START_STAGE	1
///   #define SUSTAIN_STAGE		4
///
///   float stages[NUM_STAGES][2] =
///	    {
///	    //RATE //TO LEVEL
///	    { 0.0, 1.0 },				// INITIAL ATTACK
///	    { 0.999, 0.5 },				// LOOP_START_STAGE
///	    { 0.999, 0.9 },
///	    { 0.999, 0.3 },
///	    { 0.999, 0.75 },			// SUSTAIN_STAGE
///	    { 0.9995, 0.0 },			// RELEASE 
///   };

///
/// GRAINS BUG
/// 
/// There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the 
/// switch to "Man", then the range of the Pot is correct.  But if you set the switch 
/// to "In 1" (or "In 2"), then the range of the Dial is at maximum at about the 
/// 2 o'clock position on the Dial.  Beyond that it stays at maximum.


/// CONFIGURATION
///
/// IN 1            Maximum Level CV
/// IN 2            Minimum Level CV
/// IN 3            Gate or Stage Trigger Input
/// AUDIO IN (A)    Reset
/// AUDIO OUT       Output
/// DIGITAL OUT (D) Envelope Completion Trigger Output
///
/// POT 1           Maximum Level
///                 [If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
///
/// POT 2           Minimum Level
///					[If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
///
/// POT 3           Options. Left to right: CHANGING REGULAR, CHANGING ONE SHOT, CHANGING STAGE TRIGGER,  
///                                         FLAT REGULAR, FLAT ONE SHOT, FLAT STAGE TRIGGER
	
	
	

#define CONTROL_RATE 256

#include <MozziGuts.h>

#define CV_POT_IN1    A2    // Maximum Level
#define CV_POT_IN2    A1    // Minimum Level
#define CV_POT3       A0    // Mode and Flat/Changing
#define CV_IN3        A3    // Gate or Stage Trigger Input
#define CV_AUDIO_IN   A4    // Reset 
#define CV_AUDIO_OUT  9     // Output
#define CV_GATE_OUT   8     // Envelope Completion Trigger
#define RANDOM_PIN    A5

#define TRIGGER_OFF_COUNT 100
uint8_t terminationCounter = 0;


void setup() 
    {
    Serial.begin(9600);
    pinMode(CV_IN3, INPUT);
    pinMode(CV_GATE_OUT, OUTPUT);
    pinMode(CV_AUDIO_IN, INPUT);
    startMozzi();
    }
	
#define DONE (-1)
int8_t stage = DONE;

uint8_t reset = 0;
uint8_t gate = 0;
uint8_t stageTriggered = 0;
float target = 0;

// When smaller, stages take longer to focus in on their targets
// But since our minimum resolution is 1/448, this seems to be
// a reasonable value without jumping.
#define EPSILON (0.001)
float state = 0;
float maximum = 1.0;
float minimum = 0.0;

#define MODE_REGULAR 0
#define MODE_ONE_SHOT 1
#define MODE_STAGE_TRIGGERING 2
uint8_t mode = MODE_REGULAR;
uint8_t flat = 0;

void doReset()
	{
	stage = 0;
	state = 0.0;
	}

void doGate()
	{
	gate = true;
	stage = 0;
	}

void doRelease()
	{
	Serial.println("Release");
	gate = false;
	}
	
void doStageTrigger()
	{
	if (stage != DONE)
		{
		stage++;
		}
	else
		{
		stage = 0;
		}
	}
	
inline float getTarget(uint8_t stage)
	{
	target = stages[stage][1] * (maximum - minimum) + minimum;
	return target;
	}
	
void updateStateMachine()
	{
    if (terminationCounter == 1) digitalWrite(CV_GATE_OUT, 0);
    if (terminationCounter > 0) terminationCounter--;

	if (mode == MODE_STAGE_TRIGGERING)
		{
		if (stage == DONE)			
			{
			// do nothing
			}
		else if (stage >= NUM_STAGES)
			{
			stage = 0;
			digitalWrite(CV_GATE_OUT, 1);
			terminationCounter = TRIGGER_OFF_COUNT;
			}
		else
			{
			float target = getTarget(stage);
			float alpha = stages[stage][0];
			state =  alpha * state + (1.0 - alpha) * target;

			if (abs(state - target) <= EPSILON)
				{
				// Completed the stage  But we don't advance
				state = target;
				}
			}
		}
	else if (gate)
		{
		if (stage == DONE)			
			{
			// do nothing
			}
		else if (stage == SUSTAIN_STAGE + 1 && mode != MODE_ONE_SHOT)
			{
			// we're at the sustain 
			if (LOOP_START_STAGE < SUSTAIN_STAGE)
				{
				// loop back to the loop start
				stage = LOOP_START_STAGE;
				}
			// else do nothing, we're sustaining
			}
		else if (stage >= NUM_STAGES)
			{
			// all done: probably one-shot
			stage = DONE;
			digitalWrite(CV_GATE_OUT, 1);
			terminationCounter = TRIGGER_OFF_COUNT;
			}
		else
			{
			float target = getTarget(stage);
			float alpha = stages[stage][0];
			state =  alpha * state + (1.0 - alpha) * target;
			
			if (abs(state - target) <= EPSILON)
				{
				// Completed the stage
				state = target;
				stage++;
				}
			}
		}
	else			// GATE OFF
		{
		if (stage == DONE)
			{
			// Do nothing
			}
		else if (stage >= NUM_STAGES)
			{
			// all done
			stage = DONE;
			digitalWrite(CV_GATE_OUT, 1);
			terminationCounter = TRIGGER_OFF_COUNT;
			}
		else if (stage <= SUSTAIN_STAGE)		// immediately advance
			{
			stage = SUSTAIN_STAGE + 1;
			}
		else
			{
			float target = getTarget(stage);
			float alpha = stages[stage][0];
			state =  alpha * state + (1.0 - alpha) * target;
			
			if (abs(state - target) <= EPSILON)  // we're done
				{
				// Completed the stage
				state = target;
				stage++;
				}
			}
		}
	stageTriggered = false;		// only trigger once
	}

#define INITIAL (1024)
uint16_t mx = INITIAL;
uint16_t mn = INITIAL;

#define TRIGGER_UP (800)
#define TRIGGER_DOWN (200)

void updateControl() 
    {
    // Revise mode
	uint8_t val = (mozziAnalogRead(CV_POT3) * 6) >> 10;
	if (val == 0 || val == 3) mode = MODE_REGULAR;
	else if (val == 1 || val == 4) mode = MODE_ONE_SHOT;
	else mode = MODE_STAGE_TRIGGERING;
	flat = (val >= 3);
		    
    // Test for gate changes
	uint16_t g = mozziAnalogRead(CV_IN3);
	// Test for reset
	uint16_t r = mozziAnalogRead(CV_AUDIO_IN);
	
	// Get minimum and maximum
	mx = (mx == INITIAL ? mozziAnalogRead(CV_POT_IN1) : (mx * 7 + mozziAnalogRead(CV_POT_IN1)) >> 3);
	mn = (mn == INITIAL ? mozziAnalogRead(CV_POT_IN2) : (mn * 7 + mozziAnalogRead(CV_POT_IN2)) >> 3);
	maximum = mx * (1.0 / 1023.0);
	minimum = mn * (1.0 / 1023.0);
	if (minimum > maximum) { float temp = minimum; minimum = maximum; maximum = temp; }
	
	if (mode == MODE_STAGE_TRIGGERING)
		{
		// Always reset regardless of gate
		if (r > TRIGGER_UP && !reset)
			{
			reset = 1;
			doReset();
			}
		else if (r < TRIGGER_DOWN)
			{
			reset = 0;
			}

		if (g > TRIGGER_UP && !gate)
			{
			gate = 1;
			doStageTrigger();
			}
		else if (g < TRIGGER_DOWN && gate)
			{
			gate = 0;
			}
		}
	else
		{
		if (g > TRIGGER_UP && !gate)
			{
			gate = 1;
			doGate();
			}
		else if (g < TRIGGER_DOWN && gate)
			{
			gate = 0;
			doRelease();
			}

		// Only reset if gate is on
		if (r > TRIGGER_UP && !reset && gate)
			{
			reset = 1;
			doReset();
			}
		else if (r < TRIGGER_DOWN)
			{
			reset = 0;
			}
		}
    }


int updateAudio()    
    {
	updateStateMachine();
if (flat)
    return ((int16_t)(target * 487)) - 244;
else
    return ((int16_t)(state * 487)) - 244;
    }

void loop() 
    {
    audioHook();
    }
