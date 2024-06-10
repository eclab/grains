// Copyright 2024 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// TANGLE
///
/// TANGLE is a combined digital logic function, merge, inverter, and buffered mult.  
/// You can use it to combine two digital signals, or to take two inputs and see to it that they both
/// go to a single output (such as multiple ways to reset the SEQ16), or take a signal and buffer it.
///
/// SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)
///
/// Tangle has two INPUTS, three BUFFERED OUTPUTS, and an INVERTED OUTPUT.
///
/// It takes the values of the two inputs, runs them through a function you select via POT 3,
/// then outputs the result to the three outputs, and outputs its inverse on the inverted output.
///
/// Given the two inputs A and B, the functions on POT 3 are (left to right):
///
/// - A					[B is ignored]
/// - A or B			[If either is HIGH, we output HIGH]
/// - A and B			[Both must be HIGH to output HIGH]
/// - A nequal (xor) B	[If A and B differ we output HIGH]


/// HOW YOU COULD USE TANGLE
///
/// 1. As a buffered mult.  Set the function to just "A", and the input on A gets
/// routed to buffered outputs 1, 2, and 3.
///
/// 2. As an inverter.  Set the function to just "A", and the input on A gets inverted
/// and set out the inverted output.
///
/// 3. As a trigger merge.  Set the function to "A or B", and the inputs on A and B both
/// get sent out the buffered outputs 1, 2, 3.  I use this to reset the SEQ16 while still
/// allowing it to have only 7 steps.  SEQ16 step 7 output goes into B on TANGLE, and TANGLE's 
/// Output 1 goes to RESET on SEQ16.  My manual reset goes into A on TANGLE.
///
/// 4. As a threshold.  You can test for when an analog CV is over a value, and only output
/// high when that happens.  To do this, you set the function to "A" and change POT2 to your
/// desired threshold (the maximum threshold, about 5V, is at the 2'oclock position, the minimum
/// is far left).  
///
/// 5. As a gate difference tester.  Set the function to "A xor B".  We output HIGH when 
/// either A or B are high, but if both are high, we output LOW to cut them off.  
///
/// 6. As a tester for when two gates are high at the same time.  Set to "A and B".  You could
/// threshold both of them as well (see "threshold" above).


/// GRAINS BUG
/// 
/// There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the 
/// switch to "Man", then the range of the Pot is correct.  But if you set the switch 
/// to "In 1" (or "In 2"), then the range of the Dial is maximum (1.0 probability for 
/// our purposes) at about the 2 o'clock position on the Dial.  Beyond that it stays at 1.0


/// CONFIGURATION 
///
/// IN 1            Input A
/// IN 2            Input B
/// IN 3            Output 1
/// AUDIO IN (A)    Output 2
/// AUDIO OUT       Output 3
/// DIGITAL OUT (D) Inverted Output
///
/// POT 1           [Unused]: Keep switch on IN1 and set knob to roughly 2'oclock position
///
/// POT 2           [Unused]: Keep switch on IN1 and set knob to roughly 2'oclock position
///                 
/// POT 3           Function
        

#define FUNC_A 0
#define FUNC_OR 1
#define FUNC_AND 2
#define FUNC_XOR 3

// More maybe later
#define FUNC_EQUAL 4
#define FUNC_NOR 5
#define FUNC_NAND 6
#define FUNC_AND_NOT_B 7

uint8_t function;

#define CV_POT_IN1    A2		// In A
#define CV_POT_IN2    A1		// In B
#define CV_POT3       A0		// Function
#define CV_IN3        A3		// Out 1
#define CV_AUDIO_IN   A4		// Out 2
#define CV_AUDIO_OUT  11		// Out 3
#define CV_GATE_OUT   8			// Inverted Out
#define RANDOM_PIN    A5

void setup() 
    {
    pinMode(CV_AUDIO_OUT, OUTPUT);
    pinMode(CV_AUDIO_IN, OUTPUT);
    pinMode(CV_IN3, OUTPUT);
    pinMode(CV_GATE_OUT, OUTPUT);
     
    pinMode(CV_POT_IN1, INPUT);
    pinMode(CV_POT_IN2, INPUT);
    }

// Others we could include
/// - NOT A or B	[Both must be LOW to output HIGH]
/// - NOT A and B	[If either is LOW, we output HIGH]
/// - A equal B		[If A and B are the same we output HIGH]
/// - A and NOT B	[A must be HIGH and B must be LOW to output HIGH]

void loop() 
    {
	uint8_t function = (analogRead(CV_POT3) >> 8);		// 0...3
	uint8_t in1 = digitalRead(CV_POT_IN1);
	uint8_t in2 = digitalRead(CV_POT_IN2);
	uint8_t out = 0;
	
	switch(function)
		{
		case FUNC_A:
			out = in1;
		break;
		case FUNC_OR:
			out = (in1 || in2);
		break;
		case FUNC_AND:
			out = (in1 && in2);
		break;
		case FUNC_XOR:
			out = (in1 != in2);
		break;
/*
		case FUNC_EQUAL:
			out = (in1 == in2);
		break;
		case FUNC_NOR:
			out = !(in1 || in2);
		break;
		case FUNC_NAND:
			out = !(in1 && in2);
		break;
		case FUNC_AND_NOT_B:
			out = (in1 && !in2);
		break;
*/
		}
		
	digitalWrite(CV_AUDIO_OUT, out);
	digitalWrite(CV_AUDIO_IN, out);
	digitalWrite(CV_IN3, out);
	digitalWrite(CV_GATE_OUT, !out);
    }





