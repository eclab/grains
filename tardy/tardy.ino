// Copyright 2023 Sean Luke 
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License



/// TARDY
///
/// Tardy takes gates or triggers at its three inputs and re-echoes them to its outputs
/// with a small and settable amount of delay (up to about a half-second).
/// This is primarily meant to get drums etc. in sync with other devices 
/// that have some degree of latency.  Tardy was written by Sean Luke [sean@cs.gmu.edu]
///
/// SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)
///
/// If you turn the Delay knob, it'll take Tardy a few seconds to converge to the new
/// delay value: this is a consequence of Tardy's way of dealing with noise in the delay
/// knob.  Be patient.
///
/// NOTE: output to Digital Out is likely to be SLIGHTLY FASTER than output to Audio In
/// or Audio Out.  If Output 1 is a bit ahead of Outputs 2 and 3, let me know and I can
/// try to tweak things.

/// CONFIGURATION
///
/// IN 1            Input 1
/// IN 2            Input 2
/// IN 3            Input 3
/// AUDIO IN (A)    Output 3
/// AUDIO OUT       Output 1
/// DIGITAL OUT (D) Output 2
///
/// POT 1           UNUSED.  Set to IN1 and turn dial to approximately 2 o'clock.
/// POT 2           UNUSED.  Set to IN2 and turn dial to approximately 2 o'clock.
/// POT 3           Degree of Delay



////////// PINOUTS

#define CV_POT_IN1    A2  // Input 1  
#define CV_POT_IN2    A1  // Input 2
#define CV_POT3       A0  // Delay Amount
#define CV_IN3        A3  // Input 3
#define CV_AUDIO_IN   A4  // Output 3
#define CV_AUDIO_OUT  11  // Output 1
#define CV_GATE_OUT   8   // Output 2
#define RANDOM_PIN    A5

uint8_t bittest(uint8_t* buffer, uint16_t pos)
	{
	return buffer[pos / 8] & (1 << (pos % 8)) != 0;
	}

void bitset(uint8_t* buffer, uint16_t pos)
	{
	buffer[pos / 8] |= (1 << (pos % 8));
	}
	
void bitclear(uint8_t* buffer, uint16_t pos)
	{
	buffer[pos / 8] &= ~(1 << (pos % 8));
	}

#define BUFFERLEN 4096
uint8_t buffer1[BUFFERLEN / 8];
uint8_t buffer2[BUFFERLEN / 8];
uint8_t buffer3[BUFFERLEN / 8];

void setup()
    {
    pinMode(CV_AUDIO_IN, OUTPUT);
    }

uint16_t _delay;

#define HIGH 600
#define LOW 400

uint16_t time = 0;

void loop()
    {
    // An analog read can be done 9615 times a sec, so four of them can be done 2404 times a sec.
    // Thus we will go through a buffer of size 2400 in almost exactly 1 sec.  Let's just 
    // do 2048.
    
    // Read inputs
    uint16_t in1 = analogRead(CV_POT_IN1);
    uint16_t in2 = analogRead(CV_POT_IN2);
    uint16_t in3 = analogRead(CV_IN3);
   	_delay = (_delay * 15 + analogRead(CV_POT3)) >> 4;

	// Emit current values
	digitalWrite(CV_AUDIO_OUT, bittest(buffer1, time));
	digitalWrite(CV_GATE_OUT, bittest(buffer2, time));
	digitalWrite(CV_AUDIO_IN, bittest(buffer3, time));
	
	// Write inputs
	uint16_t tdelay = time + _delay * 2 + 1;
	if (tdelay >= BUFFERLEN) tdelay = tdelay - BUFFERLEN;
	
	if (in1 >= 512) bitset(buffer1, tdelay);
	else bitclear(buffer1, tdelay);
	if (in2 >= 512) bitset(buffer2, tdelay);
	else bitclear(buffer2, tdelay);
	if (in3 >= 512) bitset(buffer3, tdelay);
	else bitclear(buffer3, tdelay);

	// Increment time
	time++;
	if (time >= BUFFERLEN) time = time - BUFFERLEN;
 }
