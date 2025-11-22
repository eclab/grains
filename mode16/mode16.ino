// Copyright 2024 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// MODE16
///
/// Mode16 adds patterns to the SEQ16 sequencer similar to SEQ8's "MODE CV" feature.   
/// You can make the sequencer go backwards, go once and end, ping-pong, do random sequences,
/// skip steps, insert rests, and more.
///
///
/// SET GRAINS TO GRAINS MODE.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.
///
/// How does Mode16 perform this magic?  By controlling SEQ16's gate and manually stepping it.
/// Mode16 can make the SEQ16 move to any location, then play its note; or it can make the SEQ16
/// play a rest.  For example, to make the SEQ16 play location 10, Mode16 will reset SEQ16,
/// then step it 9 times, then step it once to play it.  (Yes coders, this is O(n^2), not cheap).
///
/// But during those 9 steps, how does Mode16 prevent SEQ16 from playing?  This is done by filtering
/// out the output gate.  You attach the SEQ16's gate out to Mode16, and then Mode16 provides the
/// actual gate.  
///
/// Mode16 comes with 12 repeating or one-shot PATTERNS.  You can change them if you are careful.
/// The patterns at present are:
///
/// Play 16 steps, looping
/// Play 16 steps, play them again, and then end
/// Play 16 steps backwards, looping
/// Ping-Pong, looping
/// Random walk, looping
/// Random notes, looping
/// Play the first 8 notes, then 8 random notes from back 8, looping
/// AABA: Play the first 8 notes twice, then the second 8, then the first 8 again, looping
/// Rests: Play the first 8 notes with interspersed rests, then the back 8 (without rests) twice, looping
/// Play 16 steps with 1/2 probability random dropouts, looping
/// Play a random note, or a rest, looping
/// Play 16 steps with swing, looping
///
/// 
/// SETTING UP
///
/// 0. Be sure to set your GRAINS to "GRAINS" mode, not Mozzi
/// 1. Connect Mode16's Clock Out (Digital Out or "D") to SEQ16's Clock
/// 2. Connect Mode16's Reset Out (Audio Out) to SEQ16's Reset
/// 3. Connect SEQ16's Gate Out to Mode16's Gate In (IN 3)
/// 4. Connect your clock source to Mode16's Clock In (IN 1), set POT 1 to max, and set its switch to IN 1
/// 5. Connect your reset source to Mode16's Reset In (IN 2), set POT 2 to max, and set its switch to IN 2
/// 6. Connect Mode16's Gate Out (Audio In) to your gate output
/// 7. Turn POT 3 to select the pattern.
/// 8. Send Reset to Mode16 (IN 1) to reset SEQ16 rather than directly to the SEQ16.  Similarly, send
///    Clock to Mode16 (IN 2) to clock SEQ16 rather than directly to the SEQ16.
/// 9. Take the CV from SEQ16 but the Gate from Mode16's Gate Out (Audio In).
///
///
/// GOTCHAS
///
/// Mode16 expects a square-wave-ish clock.  During the LOW PART of the square wave, it does all
/// the clocks to forward to the proper step, then on the RISE of the square wave it plays the note.
/// The problem with this approach is that during the low part of the square wave is when the release of your
/// note plays, so if you have anything other than a very short release, you'll hear the other pitches as 
/// Mode16 advances to the proper step.  You can adjust things by tweaking the PWM of the square
/// wave you send to Mode16.
///
/// Mode16 is incompatible with Accent Repeats, because they conflict with Mode16's because SEQ16 determines how fast 
/// to play them (even a single Repeat) based on the speed of previous pulses, and Mode16 pulses very fast to move
/// to different locations.  So Mode16 has special code to ignore Accent Repeats and treat them as a single standard
/// gate.
///
/// Note that if you set the Pattern Knob to in-between two patterns, it won't work right, constantly getting 
/// reset.  So make sure the Pattern Knob is definitely on the pattern you want.
///
/// PROGRAMMING
///
/// Mode16 has 12 patterns in the array called "patterns" below.  Each pattern can be at most 64
/// elements long.  A pattern MUST end with either "END" or "LOOP" -- if it does not, bad things will happen.
/// Likewise, don't have a LOOP without any PLAY or REST elements prior to it at all, else you'll have an infinite loop.
/// Mode16 runs through all the elements of the chosen pattern in order.  The elements are:
///
/// END			Stop and do not continue
///	LOOP		Go back to the start of the pattern.
/// PLAY		Play the current step and advance it.
/// REST		Don't play anything, don't advance.  Just wait.
/// RR			If this appears before PLAY, then with 1/2 probability we won't play the note (but still advance).
/// SKIP		If this appears before PLAY, we skip one step first.  Multiple SKIPs or SSs or RSs will skip multiple steps.
/// SS			If this appears before PLAY, we skip one step first with a 1/2 probability.   Multiple SKIPs or SSs or RSs will skip multiple steps.
/// RS			If this appears before PLAY, we skip two steps BACKWARD with a 1/2 probability.  This is used for random walks.  Multiple SKIPs or SSs or RSs will skip multiple steps.
/// J1 ... J16	If this appears before PLAY, then we jump to step 1...16 and play *that* note instead of the current note.
/// JRND		If this appears before PLAY, then we jump to a random step and play *that* note instead of the current note.
/// JR1 		If this appears before PLAY, then we jump to a random step between 1...8 and play *that* note instead of the current note.
/// JR9 		If this appears before PLAY, then we jump to a random step between 9...16 and play *that* note instead of the current note.
/// JLO1 ... JLO16	If this appears before JRND, then it changes the low bound for the next JRND
/// JHI1 ... JHI16	If this appears before JRND, then it changes the high bound for the next JRND

/// CONFIGURATION
///
/// IN 1            Clock In
/// IN 2            Reset In
/// IN 3            Gate In [from the SEQ16 Gate]
/// AUDIO IN (A)    Gate Out
/// AUDIO OUT       Reset Out [to the SEQ16 Reset]
/// DIGITAL OUT (D) Clock Out [to the SEQ16 Clock]
///
/// POT 1           Set to maximum and set switch to IN1 
///
/// POT 2           Set to maximum and set switch to IN2
///
/// POT 3           Pattern


#define END 0			// Finish
#define LOOP 1			// Loop
#define PLAY 2			// Play
#define REST 3			// Rest
#define RR 4			// If appearing before PLAY, play but with 1/2 probability convert the note to a rest
#define SKIP 5			// If appearing before PLAY, skip 1 before playing
#define SS 6			// If appearing before PLAY, skip 1 with 1/2 probability before playing 
#define J1 7			// Jump to 1
#define J2 8
#define J3 9
#define J4 10
#define J5 11
#define J6 12
#define J7 13
#define J8 14
#define J9 15
#define J10 16
#define J11 17
#define J12 18
#define J13 19
#define J14 20
#define J15 21
#define J16 22			// Jump to 16
#define JRND 23			// Jump to Random in range 1-16
#define JR1 24			// Jump to Random in range 1-8
#define JR9 25			// Jump to Random in range 9-16
#define JLO1 26			// Set low bound for next JRND to 1
#define JLO2 27	
#define JLO3 28
#define JLO4 29
#define JLO5 30
#define JLO6 31
#define JLO7 32
#define JLO8 33
#define JLO9 34
#define JLO10 35
#define JLO11 36
#define JLO12 37
#define JLO13 38
#define JLO14 39
#define JLO15 40
#define JLO16 41		// Set low bound for next JRND to 16
#define JHI1 42			// Set high bound for next JRND to 1
#define JHI2 43	 
#define JHI3 44	 
#define JHI4 45	 
#define JHI5 46	 
#define JHI6 47	 
#define JHI7 48	 
#define JHI8 49	 
#define JHI9 50	 
#define JHI10 51 
#define JHI11 52 
#define JHI12 53 
#define JHI13 54 
#define JHI14 55 
#define JHI15 56 
#define JHI16 57		// Set high bound for next JRND to 16
#define RS 58			// If appearing before PLAY, randomly stay where you are or back up two steps

#define CV_POT_IN1    A2    // Clock In
#define CV_POT_IN2    A1    // Reset In
#define CV_POT3       A0    // Pattern
#define CV_IN3        A3    // Gate In
#define CV_AUDIO_IN   A4    // Gate Out
#define CV_AUDIO_OUT  11    // Reset Out
#define CV_GATE_OUT   8     // Clock Out
#define RANDOM_PIN    A5

void setup() 
    {
    randomSeed(RANDOM_PIN);                                             // FIXME: We're using randomSeed() and random() when we could be using the faster Mozzi versions
    pinMode(CV_AUDIO_IN, OUTPUT);
    pinMode(CV_AUDIO_OUT, OUTPUT);
    //Serial.begin(115200);
    }

#define NUM_PATTERNS 12
#define MAX_PATTERN_LENGTH 64		// Is this long enough?

uint8_t patterns[NUM_PATTERNS][MAX_PATTERN_LENGTH]
    {
    // Play 16 steps, looping
    { PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, J1, LOOP },
	
	// Play 16 steps, play them again, and then end
	{ PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, J1, 
	  PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, END },
	
	// Play 16 steps backwards, looping
	{ J16, PLAY, J15, PLAY, J14, PLAY, J13, PLAY, J12, PLAY, J11, PLAY, J10, PLAY, J9, PLAY,
	  J8, PLAY, J7, PLAY, J6, PLAY, J5, PLAY, J4, PLAY, J3, PLAY, J2, PLAY, J1, PLAY, LOOP },
	  
	// Ping-Pong, looping
	{ PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, 
	  J16, PLAY, J15, PLAY, J14, PLAY, J13, PLAY, J12, PLAY, J11, PLAY, J10, PLAY, J9, PLAY,
	  J8, PLAY, J7, PLAY, J6, PLAY, J5, PLAY, J4, PLAY, J3, PLAY, J2, PLAY, J1, PLAY, J1, LOOP },

	// Play with a random walk, looping
	{ PLAY, RS, LOOP },
	
	// Entirely random, looping
	{ JRND, PLAY, LOOP },
	
	// Play first 8, then 8 random from back 8, looping
	{ PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, 
	  JR9, PLAY, JR9, PLAY, JR9, PLAY, JR9, PLAY, JR9, PLAY, JR9, PLAY, JR9, PLAY, JR9, PLAY, J1, LOOP },

	// AABA: Play the first 8 twice, then the second 8, then the first 8 again, looping
	{ PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, J1, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, 
	  PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, J1, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, J1, LOOP },

	// Rests: Play the first 8 with interspersed rests, then the back 8 twice, looping
	{ PLAY, REST, PLAY, REST, PLAY, REST, PLAY, REST, PLAY, REST, PLAY, REST, PLAY, REST, PLAY, REST, 
	  PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, J9, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, PLAY, J1, LOOP },

    // Random Rests, looping
	{ RR, PLAY, RR, PLAY, RR, PLAY, RR, PLAY, RR, PLAY, RR, PLAY, RR, PLAY, RR, PLAY, 
	  RR, PLAY, RR, PLAY, RR, PLAY, RR, PLAY, RR, PLAY, RR, PLAY, RR, PLAY, RR, PLAY, J1, LOOP },
	
	// Either a random rest, or a random note, looping
	{ JRND, RR, PLAY, LOOP },
	 
	// Play 16 steps, looping, with swing
    { PLAY, REST, PLAY, PLAY, REST, PLAY, PLAY, REST, PLAY, PLAY, REST, PLAY, 
      PLAY, REST, PLAY, PLAY, REST, PLAY, PLAY, REST, PLAY, PLAY, REST, PLAY, J1, LOOP },
	
/*
	// Fun Little Pattern, looping
	{ PLAY, PLAY, J1, PLAY, PLAY, J1, PLAY, PLAY, PLAY, J2, PLAY, J1,
	  PLAY, PLAY, PLAY, PLAY, PLAY, J4, PLAY, J3, PLAY, J2, PLAY, J9,
	  PLAY, PLAY, J9, PLAY, PLAY, J9, PLAY, PLAY, PLAY, J10, PLAY, J9,
	  PLAY, PLAY, PLAY, PLAY, PLAY, J12, PLAY, J11, PLAY, J10, PLAY, J1, LOOP },
*/
	};


#define TRIGGER_LENGTH 200			// 1/5 of a millisecond is about the fastest SEQ16 can go
#define PLAY_LENGTH (TRIGGER_LENGTH)


uint8_t low = 1;
uint8_t high = 16;
uint8_t restInstead = 0;
uint8_t patternPos = 0;		// start
uint8_t pattern = 255;
uint8_t gateForwarding = 0;
uint8_t oldReset = 0;
uint8_t reset = 0;
uint8_t oldClock = 255;
uint8_t played = false;				// used for double-checks
uint8_t gateLock = false;
#define MAX_CLOCK_TIMER 255			// too long?  ~64ms

#define UKNOWN_GATE_OUT 2

void setGateForwarding(uint8_t forward)
    {
    gateOut = UKNOWN_GATE_OUT;
    if (forward == 0 && gateForwarding != 0)
		{
		digitalWrite(CV_AUDIO_IN, 0);				// Lower Gate just in case
		}
    gateForwarding = forward;
    gateLock = false;
    }
    
void doReset()
    {
    setGateForwarding(false);
    digitalWrite(CV_AUDIO_OUT, 1);					// Send Reset
    delayMicroseconds(TRIGGER_LENGTH);
    digitalWrite(CV_AUDIO_OUT, 0);					// End Reset
    }

void doClocks(uint8_t clocks)
    {
    setGateForwarding(false);
    for(uint8_t i = 0 ; i < clocks; i++)
		{
		digitalWrite(CV_GATE_OUT, 1);				// Send Clock
		delayMicroseconds(TRIGGER_LENGTH);
		digitalWrite(CV_GATE_OUT, 0);				// End Clock
		delayMicroseconds(TRIGGER_LENGTH);
		}
    }
	
void playNote()
    {
    //Serial.println("Play");
    setGateForwarding(true);
    digitalWrite(CV_AUDIO_IN, 1);					 // Raise Gate
    digitalWrite(CV_GATE_OUT, 1);					 // Send Clock
    delayMicroseconds(PLAY_LENGTH);
    digitalWrite(CV_GATE_OUT, 0);					 // End Clock
    delayMicroseconds(TRIGGER_LENGTH);
    }
	
void playRest()
    {
    setGateForwarding(false);
    digitalWrite(CV_AUDIO_IN, 0);					 // Lower Gate
    }
	
	
void updateStateMachine(uint8_t rising)
    {
    if (rising == 1)
		{
		setGateForwarding(true);
		}
    else 
    	{
		setGateForwarding(false);
		digitalWrite(CV_AUDIO_IN, 0);				// Lower Gate
		//Serial.println("Off");
		}
		
    uint8_t done = 0;
    while(!done)
		{
		switch (patterns[pattern][patternPos])
			{
			case END:
				{
				// do nothing, we're done
				setGateForwarding(0);		// disable forward gate
				done = 1;
				break;
				}
			case LOOP:
				{
				if (!played)							// we reached a loop without a PLAY or REST happening.  That's bad, let's end.
					{
					// do nothing, we're done
					setGateForwarding(0);		// disable forward gate
					done = 1;
					}
				else if (!rising)
					{
					setGateForwarding(0);					// disable forward gate
					patternPos = 0;
					}
				else done = 1;
				}
			break;
			case PLAY:
				{
				played = true;
				if (rising)
					{
					if (restInstead)
						{
						doClocks(1);
						playRest();
						restInstead = 0;
						}
					else
						{
						playNote();
						}
					done = 1;
					patternPos++;
					}
				else done = 1;
				}
			break;
			case REST:
				{
				played = true;
				if (rising)
					{
					playRest();
					done = 1;
					patternPos++;
					}
				else done = 1;
				}
			break;
			case RR:
				{
				if (!rising)
					{
					if (random(2)) 
						{
						restInstead = 1;
						}
					patternPos++;
					}
				else done = 1;
				}
			break;
			case SKIP:
				{
				if (!rising)
					{
					doClocks(1);
					patternPos++;
					}
				else done = 1;
				}
			break;
			case SS:
				{
				if (!rising)
					{
					if (random(2)) 
						{
						doClocks(1);
						}
					patternPos++;
					}
				else done = 1;
				}
			break;
			case J1:
				{
				if (!rising)
					{
					doReset();
					// If we don't clock at least one, SEQ16 gets confused.  So we clock 16 times
					doClocks(16);
					patternPos++;
					}
				else done = 1;
				}
			break;
			case J2:
				{
				if (!rising)
					{
					doReset();
					doClocks(1);
					patternPos++;
					}
				else done = 1;
				}
			break;
			case J3:
				{
				if (!rising)
					{
					doReset();
					doClocks(2);
					patternPos++;
					}
				else done = 1;
				}
			break;
			case J4:
				{
				if (!rising)
					{
					doReset();
					doClocks(3);
					patternPos++;
					}
				else done = 1;
				}
			break;
			case J5:
				{
				if (!rising)
					{
					doReset();
					doClocks(4);
					patternPos++;
					}
				else done = 1;
				}
			break;
			case J6:
				{
				if (!rising)
					{
					doReset();
					doClocks(5);
					patternPos++;
					}
				else done = 1;
				}
			break;
			case J7:
				{
				if (!rising)
					{
					doReset();
					doClocks(6);
					patternPos++;
					}
				else done = 1;
				}
			break;
			case J8:
				{
				if (!rising)
					{
					doReset();
					doClocks(7);
					patternPos++;
					}
				else done = 1;
				}
			break;
			case J9:
				{
				if (!rising)
					{
					doReset();
					doClocks(8);
					patternPos++;
					}
				else done = 1;
				}
			break;
			case J10:
				{
				if (!rising)
					{
					doReset();
					doClocks(9);
					patternPos++;
					}
				else done = 1;
				}
			break;
			case J11:
				{
				if (!rising)
					{
					doReset();
					doClocks(10);
					patternPos++;
					}
				else done = 1;
				}
			break;
			case J12:
				{
				if (!rising)
					{
					doReset();
					doClocks(11);
					patternPos++;
					}
				else done = 1;
				}
			break;
			case J13:
				{
				if (!rising)
					{
					doReset();
					doClocks(12);
					patternPos++;
					}
				else done = 1;
				}
			break;
			case J14:
				{
				if (!rising)
					{
					doReset();
					doClocks(13);
					patternPos++;
					}
				else done = 1;
				}
			break;
			case J15:
				{
				if (!rising)
					{
					doReset();
					doClocks(14);
					patternPos++;
					}
				else done = 1;
				}
			break;
			case J16:
				{
				if (!rising)
					{
					doReset();
					doClocks(15);
					patternPos++;
					}
				else done = 1;
				}
			break;
			case JRND:
				{
				if (!rising)
					{
					doReset();
					if (low > high) { uint8_t temp = low; low = high; high = temp; } 		// swap
							
					uint8_t numClocks = random(high - low + 1) + low;
					// If we don't clock at least one, SEQ16 gets confused.  So we clock 16 times for 0
					if (numClocks == 0) numClocks = 16;
					
					doClocks(numClocks);
					patternPos++;

					// reset
					low = 1;
					high = 16;
					}
				else done = 1;
				}
			break;
			case JR1:
				{
				if (!rising)
					{
					doReset();
					uint8_t numClocks = random(8);
					// If we don't clock at least one, SEQ16 gets confused.  So we clock 16 times for 0
					if (numClocks == 0) numClocks = 16;
					
					doClocks(numClocks);
					patternPos++;
					}
				else done = 1;
				}
			break;
			case JR9:
				{
				if (!rising)
					{
					doReset();
					uint8_t numClocks = random(8) + 8;
					// If we don't clock at least one, SEQ16 gets confused.  So we clock 16 times for 0
					if (numClocks == 0) numClocks = 16;
					
					doClocks(numClocks);
					patternPos++;
					}
				else done = 1;
				}
			break;
			case JLO1:
				{
				if (!rising)
					{
					low = 1;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JLO2:
				{
				if (!rising)
					{
					low = 2;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JLO3:
				{
				if (!rising)
					{
					low = 3;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JLO4:
				{
				if (!rising)
					{
					low = 4;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JLO5:
				{
				if (!rising)
					{
					low = 5;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JLO6:
				{
				if (!rising)
					{
					low = 6;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JLO7:
				{
				if (!rising)
					{
					low = 7;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JLO8:
				{
				if (!rising)
					{
					low = 8;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JLO9:
				{
				if (!rising)
					{
					low = 9;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JLO10:
				{
				if (!rising)
					{
					low = 10;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JLO11:
				{
				if (!rising)
					{
					low = 12;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JLO12:
				{
				if (!rising)
					{
					low = 12;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JLO13:
				{
				if (!rising)
					{
					low = 13;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JLO14:
				{
				if (!rising)
					{
					low = 14;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JLO15:
				{
				if (!rising)
					{
					low = 15;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JLO16:
				{
				if (!rising)
					{
					low = 16;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JHI1:
				{
				if (!rising)
					{
					high = 1;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JHI2:
				{
				if (!rising)
					{
					high = 2;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JHI3:
				{
				if (!rising)
					{
					high = 3;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JHI4:
				{
				if (!rising)
					{
					high = 4;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JHI5:
				{
				if (!rising)
					{
					high = 5;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JHI6:
				{
				if (!rising)
					{
					high = 6;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JHI7:
				{
				if (!rising)
					{
					high = 7;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JHI8:
				{
				if (!rising)
					{
					high = 8;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JHI9:
				{
				if (!rising)
					{
					high = 9;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JHI10:
				{
				if (!rising)
					{
					high = 10;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JHI11:
				{
				if (!rising)
					{
					high = 11;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JHI12:
				{
				if (!rising)
					{
					high = 12;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JHI13:
				{
				if (!rising)
					{
					high = 13;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JHI14:
				{
				if (!rising)
					{
					high = 14;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JHI15:
				{
				if (!rising)
					{
					high = 15;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case JHI16:
				{
				if (!rising)
					{
					high = 16;
					patternPos++;
					}
				else done = 1;
				}
			break;

			case RS:
				{
				if (!rising)
					{
					if (random(2)) 
						{
						doClocks(14);		// go backwards two steps
						}
					patternPos++;
					}
				else done = 1;
				}
			break;
			
			}
		}
    }
	
	
#define HIGH 128
#define LOW  64

uint8_t oldGate = 0;
uint8_t gateOut = 0;

uint16_t patternKnob = 0;
void loop()
    {
    // Deal with Reset
    reset = 0;
    uint16_t r = analogRead(CV_POT_IN2);
    if (r > 256)
		{
		if (!oldReset)	// we just went high, RESET
			{
			doReset();							// send an output reset
			// If we don't clock at least one, SEQ16 gets confused.  So we clock 16 times for 0
			doClocks(16);	
							
			patternPos = 0;						// reset pattern position
			played = false;
			updateStateMachine(0);				// go through the state machine as if we had just fallen
			}
		oldReset = 1;
		}
    else if (r < 128)
		{
		oldReset = 0;
		}
		
    // Change Current Pattern?
    patternKnob = (patternKnob * 7 + analogRead(CV_POT3)) >> 3;		// do some smoothing
    uint8_t currentPattern = (patternKnob * NUM_PATTERNS) >> 10;
    if (currentPattern != pattern)
		{
		doReset();					// send an output reset
			// If we don't clock at least one, SEQ16 gets confused.  So we clock 16 times for 0
			doClocks(16);	
							
		patternPos = 0;				// reset pattern position
		pattern = currentPattern;
		played = false;
		updateStateMachine(0);		// go through the state machine as if we had just fallen
		}


    uint16_t gate = analogRead(CV_IN3);			// Gate In from SEQ16

    // Forward the gate until our external clock goes down
    if (gateForwarding)
    	{
    	if (gate > 256 && !gateLock)
			{
			if (gateOut != 1)
				{
				digitalWrite(CV_AUDIO_IN, 1);			// Raise Gate
				gateOut = 1;
				}
			} 
		/*
    	else if (gate < 128)
			{
			if (gateOut != 0)
				{
				digitalWrite(CV_AUDIO_IN, 0);			// Lower Gate
				gateOut = 0;
				}
			}
		gateLock = true;
		*/
    	}
	else 
		{
		if (gateOut != 0)
			{
			digitalWrite(CV_AUDIO_IN, 0);				// Lower Gate
			gateOut = 0;
			}
		}

    // Get Clock in
    uint16_t clock = analogRead(CV_POT_IN1);

    if (clock > HIGH)
		{
		if (oldClock == 0)			// we just went high
			{
			updateStateMachine(1);
			}
		oldClock = 1;
		}
    else if (clock < LOW)
		{
		if (oldClock == 1)			// we just went low
			{
			// First, lower the gate if our external clock has gone down
			digitalWrite(CV_AUDIO_IN, 0);
			gateLock = false;
			// now update state  machine
			updateStateMachine(0);
			}
		oldClock = 0;
		}
    }
    
