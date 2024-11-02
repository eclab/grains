// Copyright 2024 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// ITERATE
///
/// ITERATE is a multiplexer, related to but not the same as the Doepfer A151.  It takes a series of gates or
/// triggers and breaks them up among up to four digital outputs.  You can use this to trigger
/// different drums, for example, based on a single input.  You can also RESET the trigger
/// sequence at any time.  Note: ITERATE is not debounced, so noise on the gates or triggers might
/// cause it to advance prematurely.
///
/// You can set the number of outputs to 2, 3, or 4 outputs, or to set them to 3 or 4 outputs
/// where a randomly different output is chosen each time (the same output is never chosen twice).
///
/// SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)

/// GRAINS BUG
/// 
/// There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the 
/// switch to "Man", then the range of the Pot is correct.  But if you set the switch 
/// to "In 1" (or "In 2"), then the range of the Dial is maximum (1.0 probability for 
/// our purposes) at about the 2 o'clock position on the Dial.  Beyond that it stays at 1.0


/// CONFIGURATION 
///
/// IN 1            Input
/// IN 2            Reset
/// IN 3            Output 2
/// AUDIO IN (A)    Output 3
/// AUDIO OUT       Output 4
/// DIGITAL OUT (D) Output 1
///
/// POT 1           [Unused]: Keep switch on IN1 and set knob to roughly 2'oclock position
///
/// POT 2           [Unused]: Keep switch on IN2 and set knob to roughly 2'oclock position
///                 
/// POT 3           Number of Outputs (left to right: 2, 3, 4, 3 random, 4 random)


#define CV_POT_IN1    A2		// In A
#define CV_POT_IN2    A1		// In B
#define CV_POT3       A0		// Function
#define CV_IN3        A3		// Out 1
#define CV_AUDIO_IN   A4		// Out 2
#define CV_AUDIO_OUT  11		// Out 3
#define CV_GATE_OUT   8			// Inverted Out
#define RANDOM_PIN    A5

#define TWO 0
#define THREE 1
#define FOUR 2
#define THREE_RAND 3
#define FOUR_RAND 4

void setup() 
    {
    pinMode(CV_AUDIO_OUT, OUTPUT);
    pinMode(CV_GATE_OUT, OUTPUT);
    pinMode(CV_AUDIO_IN, OUTPUT);
    pinMode(CV_IN3, OUTPUT);
    pinMode(CV_POT_IN1, INPUT);
    pinMode(CV_POT_IN2, INPUT);
    randomSeed(analogRead(A5));
    }

uint8_t oldOutputs = 255;
uint8_t pos = 0;

void reset()
	{
	switch (oldOutputs)
		{
		case TWO:
			pos = 0;
		break;
		case THREE:
			pos = 0; 
		break;
		case FOUR:
			pos = 0; 
		break;
		case THREE_RAND:
			pos = random(3); 
		break;
		case FOUR_RAND:
			pos = random(4); 
		break;
		}
	}

uint8_t state = 0;

void loop() 
    {
	uint8_t outputs = (analogRead(CV_POT3) * 5) >> 10;
	if (oldOutputs != outputs) { oldOutputs = outputs; reset(); }
	if (digitalRead(CV_POT_IN2)) { reset(); }

	uint8_t input = digitalRead(CV_POT_IN1);
	if (state != input)
		{
		state = input;
		
		switch (outputs)
			{
			case TWO:
				digitalWrite(CV_GATE_OUT, pos == 0 ? state : 0);
				digitalWrite(CV_IN3, pos == 1 ? state : 0);
				if (state == 0) pos++;
				if (pos >= 2) pos = 0;
			break;
			case THREE:
				digitalWrite(CV_GATE_OUT, pos == 0  ? state : 0);
				digitalWrite(CV_IN3, pos == 1 ? state : 0);
				digitalWrite(CV_AUDIO_IN, pos == 2 ? state : 0);
				if (state == 0) pos++;
				if (pos >= 3) pos = 0;
			break;
			case FOUR:
				digitalWrite(CV_GATE_OUT, pos == 0 ? state : 0);
				digitalWrite(CV_IN3, pos == 1 ? state : 0);
				digitalWrite(CV_AUDIO_IN, pos == 2 ? state : 0);
				digitalWrite(CV_AUDIO_OUT, pos == 3 ? state : 0);
				if (state == 0) pos++;
				if (pos >= 4) pos = 0;
			break;
			case THREE_RAND:
				if (state == 0) 
					{
					uint8_t val = random(2);
					if (pos == 0)
						{
						if (val == 0) pos = 1;
						else pos = 2;
						}
					else if (pos == 1)
						{
						if (val == 0) pos = 0;
						else pos = 2;
						}
					else if (pos == 2)
						{
						if (val == 0) pos = 1;
						else pos = 2;
						}
					}
				
				digitalWrite(CV_GATE_OUT, pos == 0 ? state : 0);
				digitalWrite(CV_IN3, pos == 1 ? state : 0);
				digitalWrite(CV_AUDIO_IN, pos == 2 ? state : 0);
			break;
			case FOUR_RAND:
				if (state == 0) 
					{
					uint8_t val = random(3);
					if (pos == 0)
						{
						if (val == 0) pos = 1;
						else if (val == 1) pos = 2;
						else pos = 3;
						}
					else if (pos == 1)
						{
						if (val == 0) pos = 0;
						else if (val == 1) pos = 2;
						else pos = 3;
						}
					else if (pos == 2)
						{
						if (val == 0) pos = 0;
						else if (val == 1) pos = 1;
						else pos = 3;
						}
					else if (pos == 3)
						{
						if (val == 0) pos = 0;
						else if (val == 1) pos = 1;
						else pos = 2;
						}
					}
					
				digitalWrite(CV_GATE_OUT, pos == 0 ? state : 0);
				digitalWrite(CV_IN3, pos == 1 ? state : 0);
				digitalWrite(CV_AUDIO_IN, pos == 2 ? state : 0);
				digitalWrite(CV_AUDIO_OUT, pos == 3 ? state : 0);
			break;
			}
		}
	}
