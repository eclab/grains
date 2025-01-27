// Copyright 2024 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// TANGLE
///
/// TANGLE is a combined digital logic function, merge, and inverter.  
/// You can use it to combine two digital signals, or to take two inputs and see to it that they both
/// go to a single output (such as multiple ways to reset the SEQ16).
///
/// SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)
///
/// By default Tangle has two pairs of INPUTS, and one OUTPUT for each input pair.
///
/// It takes the values of the two inputs in a pair, runs them through a function you select via POT 3,
/// then outputs the result to the pair's output.
///
/// Given two inputs A and B, the functions on POT 3 are (left to right) output as:
///
/// - A					[A is passed through, B is ignored]
/// - !A				[A is inverted, B is ignored]
/// - A or B			[If either is HIGH, we output HIGH]
/// - !(A or B)			[If either is HIGH, we output LOW]
/// - !A or B			[If A is HIGH or B is NOT HIGH, we output HIGH]
/// - A and B			[Both must be HIGH to output HIGH]
/// - !(A and B)		[Both must be HIGH to output LOW]
/// - !A and B			[If A is HIGH and B is NOT HIGH, we output HIGH]
/// - A equal B			[If A and B are the same we output HIGH]
/// - A nequal (xor) B	[If A and B are the same we output LOW]


/// HOW YOU COULD USE TANGLE
///
/// 1. As a track mute for TRIQ164.  TRIQ164 has four tracks, but only the first two can be muted by sending
/// a high signal to input sockets on the module.  But you can mute the third and fourth tracks as follows.
/// Take the trigger output of the third track and 
/// feed it into INPUT A 1 (IN 1).  Take the trigger output of the fourth track and feed it into
/// INPUT A 2 (IN 3).  Output 1 (Audio Out) is now the revised trigger output for track 3, and
/// Output 2 (Digital Out) is now the revised trigger output for track 4.  Set the function to 
/// "A and !B" (the fifth setting).  Now if you send a high signal to Input B 1 (IN 2) you will 
/// mute track 3.  And if you send a high signal to Input B 2 (Audio In) you will mute track 4.
///
/// 2. As an inverter.  Set the function to "not A", and the input on A gets inverted.
///
/// 3. As a trigger merge.  Set the function to "A or B", and the inputs on A and B both
/// get sent out to the output.  I use this to reset the SEQ16 while still
/// allowing it to have only 7 steps.  SEQ16 step 7 output goes into B on TANGLE, and TANGLE's 
/// Output 1 goes to RESET on SEQ16.  My manual reset goes into A on TANGLE.
///
/// 4. As a threshold.  You can test for when an analog CV is over a value, and only output
/// high when that happens.  To do this, you set the function to "A" and change POT1 to your
/// desired threshold (the maximum threshold, about 5V, is at the 2'oclock position, the minimum
/// is far left).  
///
/// 5. As a gate difference tester.  Set the function to "A not equal B".  We output HIGH when 
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
/// IN 1            Input A 1
/// IN 2            Input B 1
/// IN 3            Input A 2
/// AUDIO IN (A)    Input B 2
/// AUDIO OUT       Output 1
/// DIGITAL OUT (D) Output 2
///
/// POT 1           [Unused]: Keep switch on IN1 and set knob to roughly 2'oclock position
///
/// POT 2           [Unused]: Keep switch on IN1 and set knob to roughly 2'oclock position
///                 
/// POT 3           Function


#define FUNC_A 0
#define FUNC_INV_A 1
#define FUNC_A_OR_B 2
#define FUNC_INV_A_OR_B 3
#define FUNC_NOT_A_OR_B 4
#define FUNC_A_AND_B 5
#define FUNC_INV_A_AND_B 6
#define FUNC_NOT_A_AND_B 7
#define FUNC_EQUAL 8
#define FUNC_NEQUAL 9

uint8_t function;

#define CV_POT_IN1    A2		// In A 1
#define CV_POT_IN2    A1		// In B 1
#define CV_POT3       A0		// Function
#define CV_IN3        A3		// In A 2
#define CV_AUDIO_IN   A4		// In B 2
#define CV_AUDIO_OUT  11		// Out 1
#define CV_GATE_OUT   8			// Out 2
#define RANDOM_PIN    A5

void setup() 
    {
    pinMode(CV_AUDIO_OUT, OUTPUT);
    pinMode(CV_GATE_OUT, OUTPUT);
    pinMode(CV_AUDIO_IN, INPUT);
    pinMode(CV_IN3, INPUT);
    pinMode(CV_POT_IN1, INPUT);
    pinMode(CV_POT_IN2, INPUT);
    }

void loop() 
    {
	uint8_t function = (analogRead(CV_POT3) * 10 >> 10);		// 0...9

	uint8_t in1 = digitalRead(CV_POT_IN1);
	uint8_t in2 = digitalRead(CV_POT_IN2);
	uint8_t in3 = digitalRead(CV_IN3);
	uint8_t in4 = digitalRead(CV_AUDIO_IN);
	uint8_t out1 = 0;
	uint8_t out2 = 0;
	
	switch(function)
		{
		case FUNC_A:
			out1 = in1;
			out2 = in3;
		break;
		case FUNC_INV_A:
			out1 = !in1;
			out2 = !in3;
		break;
		case FUNC_A_OR_B:
			out1 = (in1 || in2);
			out2 = (in3 || in4);
		break;
		case FUNC_INV_A_OR_B:
			out1 = !(in1 || in2);
			out2 = !(in3 || in4);
		break;
		case FUNC_NOT_A_OR_B:
			out1 = ((!in1) || in2);
			out2 = ((!in3) || in4);
		break;
		case FUNC_A_AND_B:
			out1 = (in1 && in2);
			out2 = (in3 && in4);
		break;
		case FUNC_INV_A_AND_B:
			out1 = !(in1 && in2);
			out2 = !(in3 && in4);
		break;
		case FUNC_NOT_A_AND_B:
			out1 = ((!in1) && in2);
			out2 = ((!in3) && in4);
		break;
		case FUNC_EQUAL:
			out1 = (in1 == in2);
			out2 = (in3 == in4);
		break;
		case FUNC_NEQUAL:
			out1 = (in1 != in2);
			out2 = (in3 != in4);
		break;
		}
		
	digitalWrite(CV_AUDIO_OUT, out1);
	digitalWrite(CV_GATE_OUT, out2);
    }





