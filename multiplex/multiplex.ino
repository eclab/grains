// Copyright 2024 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// MULTIPLEX is a simple 1 or 2 input, 4 output digital multiplexer.  MULTIPLEX was meant
/// to run on the AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// SET GRAINS TO GRAINS MODE.  If you'd like to do Mozzi mode, change CV_AUDIO_OUT to 11.
///
/// MULTIPLEX takes 1 or 2 input signals and sends out 4 signals based on their values.
/// It can be used, for example, to turn on one of 4 output signals (to change the length
/// of SEQ16 to 4 different values for example), or to choose between four different
/// drum sounds in the DRUMKIT010.
///
/// MULTIPLEX has four modes, which are set (left to right) with POT 3.
///
///
/// MODE 1: SIMPLE MODE
/// In this mode, the value of POT 2 determines which outputs will output the incoming
/// signal.  There are only eight options:
///
/// NONE, OUT 1, OUT 2, OUT 3, OUT 4, OUTS 1 AND 2, OUTS 3 AND 4, ALL 
///
/// IN 1 is the incoming digital signal (set the switch to IN1 and set POT1 to maximum).  
/// Or set the switch to MAN and turn POT1 to change the signal.
///
///
/// MODE 2: BINARY MODE
/// In this mode, the value of POT 2 determines which outputs will output the incoming
/// signal.  There are 16 options, in binary order:
///
/// NONE,  OUT 1,  OUT 2,  OUTS 1, 2,  OUT 3,  OUTS 1, 3,  OUTS 2, 3,  OUTS 1, 2, 3
/// OUT 4,  OUTS 1, 4,  OUTS 2, 4, OUTS 1, 2, 4,  OUTS 3, 4,  OUTS 1, 3, 4,   OUTS 2, 3, 4,  OUTS 1, 2, 3, 4
///
/// Again IN 1 is the incoming digital signal (set the switch to IN1 and set POT1 to maximum).  
/// Or set the switch to MAN and turn POT1 to change the signal.
///
///
/// MODE 3: MULTIPLEX MODE
/// In this mode, IN2/POT2 is now a second digital signal.  The combination of IN1/POT1 and IN2/POT2
/// determines which single output will output high (the others will output low):
///
/// POT1 LOW  POT2 LOW		Output 1
/// POT1 HIGH POT2 LOW		Output 2
/// POT1 LOW  POT2 HIGH		Output 3
/// POT1 HIGH POT2 HIGH		Output 4
///
///
/// MODE 4: PASS MODE
/// In this mode, IN2/POT2 is now a second digital signal.  Whatever is passed into IN1/POT1 gets sent
/// out OUTPUT 1, and whatever gets passed into IN2/POT2 gets sent out OUTPUT 2: and the inverse
/// of IN1/POT1 goes to OUTPUT3, while the inverse of IN2/POT2 goes to OUTPUT 4.  
/// 
/// What's the point of this, you ask?  Well, you can use the pots to shut off or allow IN1 and IN2
/// to pass through.


/// CONFIGURATION 
///
/// IN 1            Input Signal 1
/// IN 2            Multiplexer or Input Signal 2
/// IN 3            Output 3
/// AUDIO IN (A)    Output 4
/// AUDIO OUT       Output 1
/// DIGITAL OUT (D) Output 2
///
/// POT 1           Input Signal 1
///					[If in Modes 1, 2, or 3, set Switch to IN1 and keep at roughly 2 o'clock position]
///                 [If in Mode 4, set Switch to IN3 and use pot to cut off signal]
///
/// POT 2          	Multiplexer or Input Signal 2
///					[If in Mode 1 or 2, set Switch to MAN]
///					[If in Mode 3, set Switch to IN2 and keep at roughly 2 o'clock position]
///                 [If in Mode 4, set Switch to IN2 and use pot to cut off signal]
///
/// POT 3           Method
        


#define CV_POT_IN1    A2    // Input Signal 1
#define CV_POT_IN2    A1    // Multiplexer or Input Signal 2
#define CV_POT3       A0    // Method
#define CV_IN3        A3    // Output 3
#define CV_AUDIO_IN   A4    // Output 4
#define CV_AUDIO_OUT  11    // Output 1, GRAINS MODE
#define CV_GATE_OUT   8     // Output 2
#define RANDOM_PIN    A5

void setup() 
    {
    pinMode(CV_AUDIO_OUT, OUTPUT);
    pinMode(CV_GATE_OUT, OUTPUT);
    pinMode(CV_IN3, OUTPUT);
    pinMode(CV_AUDIO_IN, OUTPUT);
    digitalWrite(CV_AUDIO_OUT, 0);
    digitalWrite(CV_GATE_OUT, 0);
    digitalWrite(CV_IN3, 0);
    digitalWrite(CV_AUDIO_IN, 0);
 	analogWrite(CV_AUDIO_OUT, 0);
    }        

uint8_t simple[8][4] = {{0, 0, 0, 0}, {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1},
					  	{1, 1, 0, 0}, {0, 0, 1, 1}, {1, 1, 1, 1}};
					  
uint8_t binary[16][4] = { {0, 0, 0, 0}, {1, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, 
						{0, 0, 1, 0}, {1, 0, 1, 0}, {0, 1, 1, 0}, {1, 1, 1, 0},
						{0, 0, 0, 1}, {1, 0, 0, 1}, {0, 1, 0, 1}, {1, 1, 0, 1}, 
						{0, 0, 1, 1}, {1, 0, 1, 1}, {0, 1, 1, 1}, {1, 1, 1, 1}};

void loop() 
    {
    uint8_t method = analogRead(CV_POT3) >> 8;
    uint8_t trigger = digitalRead(CV_POT_IN1);
    if (method == 0)		// simple
    	{
		uint8_t output = analogRead(CV_POT_IN2) >> 7;
		if (simple[output][0]) digitalWrite(CV_AUDIO_OUT, trigger); else digitalWrite(CV_AUDIO_OUT, 0);
		if (simple[output][1]) digitalWrite(CV_GATE_OUT, trigger); else digitalWrite(CV_GATE_OUT, 0);
		if (simple[output][2]) digitalWrite(CV_IN3, trigger); else digitalWrite(CV_IN3, 0);
		if (simple[output][3]) digitalWrite(CV_AUDIO_IN, trigger); else digitalWrite(CV_AUDIO_IN, 0);
		}
    else if (method == 1)	// binary
    	{
		uint8_t output = analogRead(CV_POT_IN2) >> 6;
		if (binary[output][0]) digitalWrite(CV_AUDIO_OUT, trigger);  else digitalWrite(CV_AUDIO_OUT, 0);
		if (binary[output][1]) digitalWrite(CV_GATE_OUT, trigger);  else digitalWrite(CV_GATE_OUT, 0);
		if (binary[output][2]) digitalWrite(CV_IN3, trigger);  else digitalWrite(CV_IN3, 0);
		if (binary[output][3]) digitalWrite(CV_AUDIO_IN, trigger); else digitalWrite(CV_AUDIO_IN, 0);
		}
    else if (method == 2)	// multiplex
    	{
		uint8_t trigger2 = digitalRead(CV_POT_IN2);
		digitalWrite(CV_AUDIO_OUT, !trigger && !trigger2);
		digitalWrite(CV_GATE_OUT, trigger && !trigger2);
		digitalWrite(CV_IN3, !trigger && trigger2);
		digitalWrite(CV_AUDIO_IN, trigger && trigger2);
		}
    else if (method == 3)	// pass
    	{
		uint8_t trigger2 = digitalRead(CV_POT_IN2);
		digitalWrite(CV_AUDIO_OUT, trigger);
		digitalWrite(CV_GATE_OUT, trigger2);
		digitalWrite(CV_IN3, !trigger);
		digitalWrite(CV_AUDIO_IN, !trigger2);
		}
    }
