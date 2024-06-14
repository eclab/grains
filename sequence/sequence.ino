// Copyright 2024 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// SEQUENCE
///
/// SEQUENCE is 4, 8, or 16-step, 8-pattern step sequencer for values and rests.
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode, though it could probably be adapted to it.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.
///
/// GRAINS OUTPUT WARNING: this program outputs note CV information from GRAINS.  If you feed this into
/// (for example) a VCO, 2OSC/d, or 2OSC, it will not be proper 1V/oct because they pull too much amperage 
/// and this causes the GRAINS to output incorrect voltages.  Even feeding into another GRAINS will cause 
/// the pitch to drop a bit.  You can fix this by feeding into a buffered mult first.  The 555 does not have
/// this issue (its inputs are buffered).
///
/// NOTE: Normally a step sequencer has a lot of knobs.  But GRAINS has only 3.  It can still do all the
/// functions of a step sequencer, but things are gonna be pretty fiddly.  That's life with three knobs.
///
/// The sequencer is CLOCKED and can be RESET.  It contains 8 patterns, each of up to 16 steps (8 by default).
/// You can shorten the number of steps of any of the patterns as you like.  Each clock, the sequencer
/// will output the next CV value, and it will also output a GATE which will be high for a period;
/// or if the step is a REST, it the GATE will be low for the entire step.  Your changes can be saved
/// and made available next time you power up SEQUENCE.
///
/// Programming and using Sequence is fiddly because Grains only has 3 knobs.  But you'll get the hang
/// of it!  Sequence has four MODES, set by POT 3:
///
///
/// Lock Mode
/// In this mode, turning the knobs does nothing.  It's used to turn the other knobs in preparation
/// for a different mode.  Sequence will also automatically save its settings when you ENTER LOCK MODE.
///
/// 
///
/// Entry Mode
/// In this mode, Pot 1 specifies the STEP (1...16) and Pot 2 specifies its VALUE.  The minimum
/// value is designated to be a REST (the gate is dropped) and the maximum value is designated to
/// indicate the END of the SEQUENCE -- that way you can stipulate how long the sequence is.
/// Between these two extremes lie the remaining values, namely 0V ... about 4V.
///
///
/// Pattern Mode
/// In this mode, Pot 1 specifies the PATTERN (1...8).  Pot 2 sets the GATE LENGTH.  This mode is meant for
/// real-time manual performance.
///
///
///
/// Pattern Step Mode
/// In this mode, a trigger sent to IN 1 increments the PATTERN, and Reset also resets the pattern to 0.  
/// Pot 2 sets the GATE LENGTH.  This mode is meant for automatic or triggered incrementing of the pattern.




/// HOW TO UNLOCK POT 1 OR POT 2
///
/// In ENTRY MODE, POT 2 is locked.  In PATTERN MODE, Pots 1 and 2 are locked.  In PATTERN STEP MODE,
/// Pot 2 is locked.  What does this mean?  It means that when first go into one of these modes, the
/// values of the pot are ignored until you turn the pot a little bit (about 1/32 its total distance).
/// You'll need to do that to get Sequence to start listening to the pot.
///
///
/// HOW TO START PROGRAMMING A PATTERN
///
/// Set to PATTERN MODE.  Unlock POT 1.  Set POT 1 to the desired pattern (pattern 1, for example, is far left).
/// For good measure, unlock and turn POT 2 to far right (maximum gate).   Then turn back to LOCK MODE.
///
///
/// HOW TO CLEAR OUT A PATTERN (SETTING THEM ALL TO THE SAME VALUE)
///
/// Once you have set your pattern, go to LOCK MODE.  Set POT 1 to the far left position (which will be step 1).
/// Only after you have done this, switch to ENTRY MODE.  Unlock Pot 2 and turn it to, let's say, the dead center
/// position, which is the medium CV value, around 2V.  (Remember that far left is NOT 0 V, but it a REST,
/// and far right is NOT maximum volts, but is END).  Now let's sweep the steps: turn Pot 1 moderately slowly 
/// from far left clear to far right.   Now they're all set to 2V.  Go back to LOCK MODE.
///
///
/// HOW TO PROGRAM A SINGLE STEP
///
/// Set to LOCK MODE.  Turn POT1 to the STEP you wish to set.  Then set to ENTRY MODE.
/// Unlock and turn POT2 to the VALUE you wish to set the step to.  Then turn back to LOCK MODE.
///
///
/// HOW TO PROGRAM A SERIES OF STEPS
///
/// Set to LOCK MODE.  Turn POT1 to the the first STEP to set.  Then set to ENTRY MODE.
/// Unlock and turn POT2 to the VALUE you wish to set the step to.  Then change POT1 to the next step.
/// Turn POT2 to the VALUE you wish to set the next step to.  And so on.  When you are done,
/// Set to LOCK MODE again.
///
///
/// REAL-TIME PERFORMANCE USAGE
///
/// Usually the two things you want to do in real-time is [a] change the pattern and [b]
/// change the gate.  For this reason, I suggest setting to PATTERN MODE and keeping it there.
/// While the sequencer is running you can unlock and use POT 2 to change the gate, and unlock
/// and use POT 1 to change the pattern.  Both of these can be changed via CV using IN 1 and 
/// IN 2 respectively as well (once unlocked).
///
///
/// AUTOMATICALLY TRIGGERING PATTERNS
///
/// You might want to increment patterns automatically.  You can do this by switching to
/// PATTERN STEP MODE, and set POT1 to maximum (and set the switch to IN1).  This allows you
/// to send a trigger to IN1, and when you do so, the pattern will be incremented.  Additionally,
/// when you trigger a RESET, it will not only reset the steps but also set the pattern to 1.



/// MAX NUMBER OF STEPS
///
/// By default Sequence has a maximum of 8 steps per pattern.  You can change this to 16 or to 4
/// but nothing else.  Note that 16 is extremel fiddly.  To do so, just change the #define below:

#define NUM_STEPS 8



/// INITIALIZATION
///
/// Initially Sequence will have garbage in its patterns because it has loaded from your EEPROM,
/// and who knows what's in there the first time.  You can change these values manually, but
/// it might be easier to just set everything to medium (about 2V), set the current pattern to pattern 1,
/// and set the gate length to maximum.   To do this, just uncomment the following #define

// #define INITIALIZE

/// Now upload Sequence into your Grains and turn it on.  This will clear the EEPROM.  
/// Thereafter re-comment that #define and reupload Sequence into your Grains.


/// CONFIGURATION 
///
/// IN 1           	Step / Pattern CV	-- In ENTRY MODE, specifies which step.  In PATTERN MODE, specifies which pattern.  In PATTERN STEP MODE, increments the pattern.
/// IN 2            Value / Gate CV		-- In ENTRY MODE, specifies step value.  In PATTERN / PATTERN STEP MODE, specifies gate length
/// IN 3            Clock
/// AUDIO IN (A)    Reset
/// AUDIO OUT       Output
/// DIGITAL OUT (D) Gate Output
///
/// POT 1           Step / Pattern 		-- In ENTRY MODE, specifies which step.  In PATTERN MODE, specifies which pattern
/// 				[Set the switch to MAN if not using the CV, else set to IN 1 and set POT 1 to about 2 o'clock]
///
/// POT 2           Value / GATE		-- In ENTRY MODE, specifies step value.  In PATTERN MODE, specifies gate length
///                	[Set the switch to MAN if not using the CV, else set to IN 2 and set POT 2 to about 2 o'clock]
///
/// POT 3           Mode 		(Left: ENTRY  Middle Left: LOCK  Middle Right: PATTERN  Right: PATTERN STEP)
        
        

#define CONTROL_RATE 256

#include <MozziGuts.h>
#include <EEPROM.h>

#define CV_POT_IN1    A2    // Step
#define CV_POT_IN2    A1    // Value
#define CV_POT3       A0    // Mode
#define CV_IN3        A3    // Clock In
#define CV_AUDIO_IN   A4    // Reset 
#define CV_AUDIO_OUT  9     // Output
#define CV_GATE_OUT   8     // Gate Out
#define RANDOM_PIN    A5

// This can't be changed so easily, it's fixed in value 
#define NUM_PATTERNS 8 


uint16_t steps[NUM_PATTERNS][NUM_STEPS];    
uint8_t currentPattern = 0;
uint16_t currentGate = 65535;		// Maximal value

uint8_t clocked = NUM_STEPS;
uint16_t pot1;
uint16_t pot2;
uint16_t lastPot1;
uint16_t lastPot2;
uint8_t pot1_changed = false;
uint8_t pot2_changed = false;
uint8_t stepShift;
uint8_t currentStep = 0;
uint8_t trigger = 0;

uint16_t gateLength;

void initialize()
	{
	currentPattern = 0;
	currentGate = 65535;

	for(int i = 0; i < NUM_PATTERNS; i++)
		{
		for(int j = 0; j < NUM_STEPS; j++)
			{
			steps[i][j] = 512;
			}
		}
	save();
	}
	
void load()
	{
	// If in INITIALIZE mode, we save once first

#ifdef INITIALIZE
initialize();
#endif
	
	// Load the current pattern and gate from positions 0..2
	int pos = 0;
	currentPattern = EEPROM.read(pos++);
	currentGate = (EEPROM.read(pos++) << 8) + EEPROM.read(pos++);

	// Load the steps from positions 3 onward
	for(int i = 0; i < NUM_PATTERNS; i++)
		{
		for(int j = 0; j < NUM_STEPS; j++)
			{
			steps[i][j] = (EEPROM.read(pos++) << 8) + EEPROM.read(pos++);
			}
		}

	// load the last pot so we don't automatically make changes
    lastPot1 = (3 * pot1 + mozziAnalogRead(CV_POT_IN1)) >> 2;
	lastPot2 = (3 * pot2 + mozziAnalogRead(CV_POT_IN2)) >> 2;
	}
	
void save()
	{
	// Save the current pattern and gate to positions 0...2
	int pos = 0;
	EEPROM.update(pos++, currentPattern);
	EEPROM.update(pos++, currentGate >> 8);
	EEPROM.update(pos++, currentGate & 0xFF);

	// Save the steps to positions 3 onward
	for(int i = 0; i < NUM_PATTERNS; i++)
		{
		for(int j = 0; j < NUM_STEPS; j++)
			{
			EEPROM.update(pos++, steps[i][j] >> 8);
			EEPROM.update(pos++, steps[i][j] & 0xFF);
			}
		}
	}



void setup() 
    {
    pinMode(CV_IN3, INPUT);
    pinMode(CV_AUDIO_IN, INPUT);
    pinMode(CV_POT_IN1, INPUT);
    pinMode(CV_POT_IN2, INPUT);
    pinMode(CV_POT3, INPUT);
    pinMode(CV_AUDIO_OUT, OUTPUT);
    pinMode(CV_GATE_OUT, OUTPUT);
    
    // We need to know how far to shift depending on whether the user selected 4, 8, or 16 steps
    if (NUM_STEPS == 16) stepShift = 6;
    else if (NUM_STEPS == 8) stepShift = 7;
    else stepShift = 8;		//if NUM_STEPS = 4
    
    load();
    startMozzi();    
    }

/// MODES
#define ENTRY 0
#define LOCK 1
#define PATTERN 2
#define PATTERN_STEP 3

/// POT POSITION BOUNDARIES
/// This is set up so the range between MINIMUM and MAXIMUM is equal
/// to 780, and (780 * 5) / 8 is about 488.
#define REST 61					// <= this defines a REST
#define MINIMUM 122				// <= this is always 0 (it's a border between 0 and REST)
#define MAXIMUM 902				// > this is always 1023 (it's a border between 1023 and END)
#define END 963					// > this is always END


#define POT_EPSILON 64
uint8_t pot2Changed()
	{
	if (pot2_changed) { lastPot2 = pot2; return true; }
	if (pot2 >= lastPot2)
		{
		if (pot2 - lastPot2 >= POT_EPSILON)	
			{
			pot2_changed = true;
			lastPot2 = pot2;
			return true;
			}
		}
	else
		{
		if (lastPot2 - pot2 >= POT_EPSILON)	
			{
			pot2_changed = true;
			lastPot2 = pot2;
			return true;
			}
		}
	return false;
	}

uint8_t pot1Changed()
	{
	if (pot1_changed) { lastPot1 = pot1; return true; }
	if (pot1 >= lastPot1)
		{
		if (pot1 - lastPot1 >= POT_EPSILON)	
			{
			pot1_changed = true;
			lastPot1 = pot1;
			return true;
			}
		}
	else
		{
		if (lastPot1 - pot1 >= POT_EPSILON)	
			{
			pot1_changed = true;
			lastPot1 = pot1;
			return true;
			}
		}
	return false;
	}

#define GATE_COUNTDOWN 3
uint8_t gateCountdown = 0;
void scheduleGate()
	{
	gateCountdown = GATE_COUNTDOWN;
	}

void updateControl() 
    {
    if (gateCountdown > 0)
    	{
    	if (gateCountdown == 1)
    		{
    		digitalWrite(CV_GATE_OUT, 1);
    		currentGate = 0;    		
    		}
    	gateCountdown--;
    	}
    	
    uint8_t clock = (mozziAnalogRead(CV_IN3) > 511);
    uint8_t reset = (mozziAnalogRead(CV_AUDIO_IN) > 511);
    uint16_t mode = mozziAnalogRead(CV_POT3) >> 8;
    pot1 = (3 * pot1 + mozziAnalogRead(CV_POT_IN1)) >> 2;
	pot2 = (3 * pot2 + mozziAnalogRead(CV_POT_IN2)) >> 2;
    
    currentGate++;
    if (reset) 
    	{
    	currentStep = NUM_STEPS - 1;
		digitalWrite(CV_GATE_OUT, 0);
		currentGate = gateLength;
		if (mode == PATTERN_STEP) currentPattern = 0;
    	}
    else if (currentGate >= gateLength)
    	{
    	digitalWrite(CV_GATE_OUT, 0);
    	}
    
    // load new values
    if (mode == ENTRY)
    	{
    	if (pot2Changed())
    		{
    		steps[currentPattern][pot1 >> stepShift] = pot2;
    		}
    	trigger = 0;
    	}
    else if (mode == LOCK)
    	{
    	if (pot1_changed || pot2_changed) save();
    	pot1_changed = false;
    	pot2_changed = false;
		lastPot1 = pot1;
		lastPot2 = pot2;
    	trigger = 0;
    	}
    else if (mode == PATTERN)
    	{
    	if (pot2Changed())
    		{
	    	gateLength = pot2 >> 2;
	    	}
    	if (pot1Changed())
    		{
    		currentPattern = (pot1 >> 7);
    		}
    	trigger = 0;
    	}
    else if (mode == PATTERN_STEP)
    	{
    	if (pot2Changed())
    		{
	    	gateLength = pot2 >> 2;
	    	}
	    	
	    if (pot1 >= 512 && !trigger)
	    	{
	    	trigger = 1;
	    	currentPattern++;
	    	if (currentPattern >= NUM_PATTERNS) currentPattern = 0;
	    	}
	    else
	    	{
	    	trigger = 0;
	    	}
    	}
    
    // Do we step?
    if (clock)
    	{
    	if (!clocked)
    		{
    		currentStep++;
    		if (currentStep >= NUM_STEPS || steps[currentPattern][currentStep] >= END)
    			{
    			currentStep = 0;
    			}
    		if (steps[currentPattern][currentStep] < REST || reset) 
    			{
    			digitalWrite(CV_GATE_OUT, 0);
    			}
    		else 
    			{
    			scheduleGate();
    			}
    		clocked = true;
    		}
    	}
    else	
		{
		clocked = false;
		}
    }

uint16_t lastStep = 0;
int updateAudio()    
    {
    uint16_t step = steps[currentPattern][currentStep];
    if (step >= MINIMUM && step < MAXIMUM)
    	{
    	uint16_t range = ((steps[currentPattern][currentStep] - MINIMUM) * 5) >> 3; 			//  0...487
    	return lastStep = (((int16_t) range) - 244);
    	}
    else if (step < REST || step >= END)
    	{
    	return lastStep;
    	}
    else if (step < MINIMUM)
    	{
    	return -244;
    	}
    else // if (step >= MAXIMUM)
    	{
    	return 243;
    	}
    }

void loop() 
    {
    audioHook();
    }
