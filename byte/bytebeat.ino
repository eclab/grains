// Copyright 2024 Sean Luke
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


/// BYTE
///
/// Byte is a bytebeat synthesizer similar to ALGODRONE or ByteBeat-Xfade. You can provide 
/// up to 16 bytebeat expressions (defined as BYTEBEAT_1 through BYTEBEAT_16 below), and 
/// then select and play them.  Feel free to change them!  But they must be valid C/C++ 
/// expressions or they won't compile. You can change the pitch and tuning, as well as the 
/// volume.  Additionally you can reset the bytebeat algorithm at any time by sending a
/// trigger or gate to DIGITAL OUT.  
///
/// BUILDING BYTEBEATS
/// A bytebeat is any C expression using the 32-bit variable 't'.  It's called 16384 times a second
/// and the result, mod 255, is output as an audio sample.
///
/// Byte has a special feature: a special variable called x which ranges from 0 to 255.  It's
/// controlled by CV coming in to IN 3, and is by default 0.  You could stick x in your expressions
/// to control them in real-time via CV.
///
/// Byte is fragile: if in your expression you modify Byte's own variables (like triggered or pitchCV
/// or even t!) you can seriously break Byte.  So don't do that.
/// 
///
/// TUNING
///
/// You can't really tune BYTE: to do so would require changing the sampling rate, and Mozzi doesn't
/// make it easy to do that without modifying Mozzi's config in its library.  Maybe later.
///
/// On Pot 1 I have set things up so you can change the pitch to some degree.  There are 16
/// settings.  Setting 8 (typically a bit to the left of due center) is the default setting.  
/// Since Mozzi's sampling rate is 16384 Hz, this is about one octave above and twice as fast
/// as the traditional pitch, which normally corresponds to 8000 Hz.  If you turn the knob 
/// to the LEFT of this, Byte will get slower and lower in pitch. It does this by just playing 
/// fewer times when Mozzi asks it to. It's pretty crude but it works.  If you turn the knob to 
/// the RIGHT, Byte will get higher in pitch but also change how it plays.  This is because it 
/// is incrementing the "t" variable by more and more (by default it's incremented by 1).
///
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.


/// CONFIGURATION
///
/// IN 1            Pitch CV
/// IN 2           	Bytebeat CV
/// IN 3           	Auxilliary Variable 1 CV  (x)
/// AUDIO IN (A)    UNUSED
/// AUDIO OUT       Out
/// DIGITAL OUT (D) Reset
///
/// POT 1           Pitch Scaling       [Set the switch to MAN probably]
///
/// POT 2           Bytebeat            [Set the switch to MAN probably]
///
/// POT 3           Volume 


#include <math.h>


// FOR MORE BYTEBEAT EXAMPLES SEE 	http://viznut.fi/demos/unix/bytebeat_formulas.txt

#define BYTEBEAT_1 \
	t*(42&t>>10)
	
#define BYTEBEAT_2 \
	t|t%255|t%257

#define BYTEBEAT_3 \
	t*(((t>>11)&(t>>8))&(123&(t>>3)))

#define BYTEBEAT_4 \
	t*(t>>((t>>9)|(t>>8))&(63&(t>>4)))

#define BYTEBEAT_5 \
	(t>>6|t|t>>(t>>16))*10+((t>>11)&7)

#define BYTEBEAT_6 \
	(t|(t>>9|t>>7))*t&(t>>11|t>>9)

#define BYTEBEAT_7 \
	t*5&(t>>7)|t*3&(t*4>>10)

#define BYTEBEAT_8 \
	(t>>7|t|t>>6)*10+4*(t&t>>13|t>>6)

#define BYTEBEAT_9 \
	((t&4096)?((t*(t^t%255)|(t>>4))>>1):(t>>3)|((t&8192)?t<<2:t))

#define BYTEBEAT_10 \
	((t*(t>>8|t>>9)&46&t>>8))^(t&t>>13|t>>6)

#define BYTEBEAT_11 \
	(t*5&t>>7)|(t*3&t>>10)

#define BYTEBEAT_12 \
	(int)(t/1e7*t*t+t)%127|t>>4|t>>5|t%127+(t>>16)|t

#define BYTEBEAT_13 \
	((t/2*(15&(0x234568a0>>(t>>8&28))))|t/2>>(t>>11)^t>>12)+(t/16&t&24)

#define BYTEBEAT_14 \
	(t&t%255)-(t*3&t>>13&t>>6)

#define BYTEBEAT_15 \
	t>>4|t&((t>>5)/(t>>7-(t>>15)&-t>>7-(t>>15)))

#define BYTEBEAT_16 \
	t*(((t>>9)&10)|((t>>11)&24)^((t>>10)&15&(t>>15)))

#define CV_POT_IN1    A2    // Note In, Pitch Scaling                   // Filter Cutoff
#define CV_POT_IN2    A1    // Filter Cutoff
#define CV_POT3       A0    // Resonance
#define CV_IN3        A3    // Amplitude CV
#define CV_AUDIO_IN   A4    // Pitch Tune                                               // Resonance CV
#define CV_AUDIO_OUT  9     // Out
#define CV_GATE_OUT   8     // [Unused]
#define RANDOM_PIN    A5

#define CONTROL_RATE 64


#include <MozziGuts.h>

void setup() 
    {
    pinMode(CV_GATE_OUT, INPUT);
    startMozzi();
    }

uint8_t triggered = false;
uint8_t x;
uint8_t frequency = 1;
uint8_t gain;
uint8_t expression;
uint32_t t = 0;
uint8_t oldExpression = 0;
uint8_t maxCount = 1;
uint8_t increment = 1;

void updateControl() 
    {
    frequency = (mozziAnalogRead(CV_POT_IN1) >> 6) + 1;
	if (frequency >= 8)
		{
		maxCount = 1;
		increment = frequency - 7;
		}
	else
		{
		maxCount = 9 - frequency;
		increment = 1;
		}

    gain = mozziAnalogRead(CV_POT3) >> 5;		// 0...31
    x = mozziAnalogRead(CV_IN3) >> 2;			// 0...255
    expression = mozziAnalogRead(CV_POT_IN2) >> 6;	// 0...15
    if (digitalRead(CV_GATE_OUT))
    	{
    	if (!triggered)
    		{
    		triggered = true;
    		t = 0;
    		}
    	}
    else
    	{
    	triggered = false;
    	}

	if (expression != oldExpression)
		{
		t = 0;
		oldExpression = expression;
		}
    }


uint8_t count = 1;
int16_t last = 0;

int updateAudio()    
    {
    count--;
    if (count == 0)
    	{
    	count = maxCount;
    	
		// shift pitch
		t += increment;
	
		// select expression
		uint8_t result;
		switch(expression)
			{
			case 0: result = (uint8_t)(255 & (BYTEBEAT_1)); break;
			case 1: result = (uint8_t)(255 & (BYTEBEAT_2)); break;
			case 2: result = (uint8_t)(255 & (BYTEBEAT_3)); break;
			case 3: result = (uint8_t)(255 & (BYTEBEAT_4)); break;
			case 4: result = (uint8_t)(255 & (BYTEBEAT_5)); break;
			case 5: result = (uint8_t)(255 & (BYTEBEAT_6)); break;
			case 6: result = (uint8_t)(255 & (BYTEBEAT_7)); break;
			case 7: result = (uint8_t)(255 & (BYTEBEAT_8)); break;
			case 8: result = (uint8_t)(255 & (BYTEBEAT_9)); break;
			case 9: result = (uint8_t)(255 & (BYTEBEAT_10)); break;
			case 10: result = (uint8_t)(255 & (BYTEBEAT_11)); break;
			case 11: result = (uint8_t)(255 & (BYTEBEAT_12)); break;
			case 12: result = (uint8_t)(255 & (BYTEBEAT_13)); break;
			case 13: result = (uint8_t)(255 & (BYTEBEAT_14)); break;
			case 14: result = (uint8_t)(255 & (BYTEBEAT_15)); break;
			case 15: result = (uint8_t)(255 & (BYTEBEAT_16)); break;
			}
			
		// convert to signed and change amplitude
		last = ((result - (int16_t)128) * gain) >> 4;			// gain should go 0...31
		}
	return last;
    }


void loop() 
    {
    audioHook();
    }
    
