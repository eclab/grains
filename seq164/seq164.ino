// Copyright 2025 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// SEQ164
///
/// Seq164 turns TRIQ164 into a quantized note sequencer.  Seq164 is meant to run on the AE Modular GRAINS, 
/// but it could be adapted to any Arduino.
///
/// Seq164 takes all four of the trigger outputs of TRIQ164 and interprets them as one of 15 notes, outputting
/// the note CV and a trigger.  This allows you to enter note patterns into TRIQ16 and play them as notes or
/// as rests.
/// 
/// To use Seq164, you connect Outs 1, 2, 3, and 4 on your TRIQ164 to trigger inputs "1, 2, 3, and 4" on the
/// GRAINS (namely IN1, IN2, IN3, and AUDIO IN respectively).  GRAINS will produce CV output as AUDIO OUT and
/// will send a trigger on DIGITAL OUT whenever it receives triggers from Seq164.  
///
/// Seq164 can output 15 different notes.  These notes are selected from a SCALE (or CHORD).  You choose the 
/// Scale with POT 3.  Seq164 will choose the Nth note, going up octaves if necessary.  For example, if you
/// chose MAJOR -- let's imagine it was C major for discussion here -- then the 15 notes would be
/// C, D, E, F, G, A, B,  then [from the next octave] C, D, E, F, G, A, B, then [from the next octave] C.
///
/// Additionally, if NO TRIGGERS are being output from TRIQ164, then Seq164 will not change its chord and will 
/// not trigger a note.  You can think of this as a rest.
///
/// The four triggers determine the note (or rest) as follows (0 means OFF and 1 means ON, that is, TRIGGERED):
///
/// OUT 1	OUT 2	OUT 3	OUT 4	VALUE
///	0		0		0		0		Rest	[No trigger, CV doesn't change]
///	1		0		0		0		Note 1
///	0		1		0		0		Note 2
/// 1		1		0		0		Note 3
///	0		0		1		0		Note 4
///	1		0		1		0		Note 5
///	0		1		1		0		Note 6
///	1		1		1		0		Note 7
/// 0		0		0		1		Note 8
///	1		0		0		1		Note 9
///	0		1		0		1		Note 10
/// 1		1		0		1		Note 11
///	0		0		1		1		Note 12
///	1		0		1		1		Note 13
///	0		1		1		1		Note 14
///	1		1		1		1		Note 15
///
/// You will find this pretty cumbersome to program because TRIQ164 can only show one output at a time.
/// You might find Seq164 more useful as a tool for happy accidents: just plug in random stuff to
/// TRIQ164 and see what happens
///
///
/// SCALES
///
/// The scale or chord is chosen by POT 3.  They are at present (left to right):
///
/// Major
/// Melodic Minor 
/// Dorian
/// Mixolydian
/// Aeolian (Relative Minor)
/// Blues Minor
/// Pentatonic
/// Minor Pentatonic
/// Japanese Pentatonic
/// Chromatic
/// Major Triad + Major 2nd + Major 6th
/// Major Triad + Major 2nd + Major 7th
/// Minor Triad + Major 2nd + Minor 6th
/// Minor Triad + Major 2nd + Minor 7th
/// Minor Triad + Major 2nd + Minor 6th + Minor 7th
/// Fully Diminished 7th
///
///
/// REPEATS
///
/// Seq164 might work with TRIQ164 repeats or it might not, because repeats are individual triggers,
/// and if you don't have all the triggers of a given step repeating, the note might change during
/// the repeat.  At least that's what I presume: so far I've not gotten Seq164 and TRIQ164 to behave
/// badly in this way.
///
///
/// LATENCY
///
/// Seq164 is a little pokey.  It needs about 1/128 of a second to determine that all the triggers
/// have arrived from TRIQ164 and it can buid the note.  Then it takes about 1/80 of a second to
/// get GRAINS to output the right note pitch before it can issue its trigger output on Digital Out.
/// This is the nature of the GRAINS hardware.  To improve on this latency would start creating clicks
/// and (theoretically) missed triggers. 
///
///
/// MIXING AND MATCHING
///
/// You don't need to use all four tracks of TRIQ164 for melody.  You could use, for example, just 3
/// (providing 7 different notes) and reserve the fourth track for its usual drum duties.  If you
/// attached the 3 tracks to trigger inputs 1, 2, and 3 on GRAINS you'd get notes 1...7.  That's the
/// most useful configuration.  But if you attached them to inputs 2, 3, and 4 you'd get notes 
/// 2, 4, 6, 8, 10, 12, and 14, which would be odd but perhaps interesting.  
///
/// Similarly you could just use 2 tracks for melody.  If you attached
/// them to trigger inputs 1 and 2, you'd get notes 1, 2, and 3 and that's it.  If you attached
/// them to inputs 2 and 3 you'd get notes 2, 4, and 6.  If you attached them to inputs 3 and 4 you'd
/// get inputs 4, 8, and 12.
/// 
///
/// OUTPUT PITCH RANGE AND RESOLUTION
/// 
/// Mozzi cannot go down to 0V.  Its minimum is a little more, transposing up by about a half
/// of a semitone.   Most oscillators can be tuned to deal with this.
/// But GRAINS oscillators have to be manually adjusted.  So for example, if you're attaching 
/// this program to a GRAINS oscillator like DX, you might want to change the TRANPOSE_BITS
/// to about -6.
///
/// Mozzi's output is capable of a range of betwen 42 and 48 notes (betwen 3.5 and 4 octaves).  
/// This is also the quantizer's range: values above that will just get quantized to the 
/// top note.
///
/// One of the issues in using this firmware is that
/// the voltage GRAINS's output will produce is strongly affected by the amperage being pulled
/// by the oscillator it's plugged into, and different AE oscillators pull different amounts.
/// VCO is particularly bad here -- it pulls a lot of voltage, thus scaling down GRAINS's output
/// so it's no longer v/oct.  555 is much better (its inputs are buffered).  And you can
/// generally fix matters by plugging GRAINS into a buffered mult, and then attaching
/// the buffered mult to your oscillator.  But even the buffered mults differ a bit!
///
/// I have made a few tables to match different scenarios:
///
/// 1. You are plugged directly into a VCO
/// 2. You are plugged directly into a 555
/// 3. You are plugged directly into a uBUFFER.  The uBUFFER is attached to your oscillator(s).
/// 4. You are plugged directly into a 4BUFFER.  The 4BUFFER is attached to your oscillator.
/// 5. You are plugged directly into another GRAINS oscillator.  In this case, I suggest
///    using the OUTPUT_UBUFFER #define below, setting TRANSPOSE_BITS on the GRAINS oscillator 
///    to about -6, and tweaking the tracking via POT1.  It should work.
/// 6. You are plugged directly into a 2OSC/d
/// 7. You are plugged into a 2OSC: in this case, may your god have mercy on your soul.
///
/// You need to specify what you're plugged into, which will change the pitch table:

#define OUTPUT_555
//#define OUTPUT_VCO
//#define OUTPUT_2OSCD
//#define OUTPUT_UBUFFER
//#define OUTPUT_4BUFFER



/// CONFIGURATION
///
/// IN 1            Trigger Input 1
/// IN 2            Trigger Input 2
/// IN 3            Trigger Input 3
/// AUDIO IN (A)    Trigger Input 4
/// AUDIO OUT       Quantized Pitch CV Out
/// DIGITAL OUT (D) Trigger Out
///
/// POT 1           [Set to Maximum and Set switch to IN1]
///
/// POT 2           [Set to Maximum and Set switch to IN2]
///
/// POT 3           Scale/Chord



#define CONTROL_RATE 256

#include <MozziGuts.h>
#include "EEPROM.h"

#define CV_POT_IN1    A2    // Trigger Input 1
#define CV_POT_IN2    A1    // Trigger Input 2
#define CV_POT3       A0    // Scale 
#define CV_IN3        A3    // Trigger Input 3
#define CV_AUDIO_IN   A4    // Trigger Input 4
#define CV_AUDIO_OUT  9     // Pitch Out
#define CV_GATE_OUT   8     // Trigger Out
#define RANDOM_PIN    A5


#define NUM_SCALES 16
#define NUM_NOTES 12
const uint8_t scales[NUM_SCALES][NUM_NOTES] = 
    {
	/// SCALES
//    C  Db D  Eb E  F  Gb G  Ab A  Bb B
	{ 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1 },		// Major
	{ 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1 },		// Melodic Minor
	{ 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0 },		// Dorian
	{ 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0 },		// Mixolydian
	{ 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0 },		// Aeolian (Relative Minor)
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },		// Chromatic
	{ 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0 },		// Blues Minor
	{ 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0 },		// Pentatonic
	{ 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0 },		// Minor Pentatonic
	{ 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0 },		// Japanese Pentatonic

	// CHORDS
//    C  Db D  Eb E  F  Gb G  Ab A  Bb B
	{ 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0 },		// Major + 2 + 6
	{ 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1 },		// Major + 2 + M7
	{ 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0 },		// minor + 2 + 6
	{ 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0 },		// minor + 2 + m7
	{ 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0 },		// minor + 2 + 6 + m7
	{ 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0 },		// dim7
	};
	
const uint8_t numNotes[NUM_SCALES] =  { 7, 7, 7, 7, 7, 12, 6, 5, 5, 5, 5, 5, 5, 5, 6, 4 };

#if defined(OUTPUT_555)
// VALUES FOR 555
uint16_t positions[48] = 
{  
// C    C#   D    D#   E    F    F#   G    G#   A     A#   B
   0,   13,  22,  30,  38,  47,  55,  64,  72,  81,  89,  98,
   107, 116, 124, 133, 141, 150, 159, 167, 176, 184, 193, 202,
   210, 219, 227, 236, 244, 253, 262, 270, 279, 287, 296, 304,
   313, 321, 330, 338, 346, 355, 363, 372, 380, 388, 399, 399		// 555 has 47 notes
};
#elif defined(OUTPUT_VCO)
// VALUES FOR VCO
uint16_t positions[48] = 
{  
// C    C#   D    D#   E    F    F#   G    G#   A     A#   B
   0,   14,  24,  33,  42,  51,  60,  69,  78,  88,  97,  106,
   115, 124, 134, 143, 152, 161, 171, 180, 189, 198, 208, 217,
   226, 235, 244, 254, 263, 272, 281, 290, 299, 308, 318, 327,
   350, 359, 368, 377, 385, 394, 394, 394, 394, 394, 394, 394			// VCO has only 42 notes
};
#elif defined(OUTPUT_UBUFFER)
// VALUES FOR uBUFFER
uint16_t positions[48] = 
{  
// C    C#   D    D#   E    F    F#   G    G#   A     A#   B
   0,   13,  21,  29,  38,  46,  55,  63,  72,  80,  90,  97,
   106, 114, 123, 141, 139, 148, 156, 165, 173, 182, 190, 199,
   208, 216, 225, 233, 242, 250, 259, 267, 276, 284, 293, 301,
   309, 318, 326, 335, 343, 351, 359, 368, 376, 384, 392, 392			// uBuffer has 47 notes
};
#elif defined(OUTPUT_4BUFFER)
// VALUES FOR 4BUFFER
uint16_t positions[48] = 
{  
// C    C#   D    D#   E    F    F#   G    G#   A     A#   B
   0,   13,  21,  29,  38,  46,  55,  63,  72,  80,  89,  98,
   107, 115, 124, 142, 140, 149, 157, 166, 174, 183, 191, 200,
   209, 217, 226, 234, 243, 251, 260, 268, 277, 285, 294, 302,
   311, 320, 328, 337, 345, 353, 361, 370, 378, 386, 394, 394			// 4Buffer has 47 notes
};
#elif defined(OUTPUT_2OSCD)
uint16_t positions[48] = 
{  
// C    C#   D    D#   E    F    F#   G    G#   A     A#   B
   0,   13,  22,  30,  39,  47,  56,  64,  73,  82,  90,  99,
   107, 116, 125, 133, 142, 151, 159, 168, 176, 185, 194, 202,
   211, 220, 228, 237, 245, 254, 262, 271, 280, 288, 297, 306,
   314, 323, 331, 340, 348, 356, 365, 373, 382, 390, 403, 403			// 2OSC2 has 47 notes
};
#endif


uint8_t getPitch(uint8_t pos, uint8_t scale)
	{
	uint8_t nn = numNotes[scale];
	uint8_t octave = 0;

	while(pos >= nn)		// poor man's % and /
		{
		pos -= nn;
		octave++;
		}
		
	// now find the note in the scale
	uint8_t note = 0;
	for(uint8_t i = 0; i < 12; i++)
		{
		if (scales[scale][i])
			{
			if (pos == 0)
				{
				// we got it
				return octave * 12 + i;
				}
			else
				{
				pos--;
				}
			}
		}
	return 0;		// should never happen
	}
	

// output CV
int16_t pitchOut;

// Trigger indicators.  These are either 0 (not triggered), 1 (triggered last time and this time), or 2 (triggered this time)
// Someone has to be a 1 in order for us to consider all four to have been fairly triggered
uint8_t a = 0;
uint8_t b = 0;
uint8_t c = 0;
uint8_t d = 0;

// indicates that we should trigger output during updateAudio
uint8_t go = 0;

#define HIGH 200
#define LOW 100

uint8_t countdown = 0;
uint8_t countdown2 = 0;
void updateControl() 
    {
    // this may not work as we have to read in parallel...
    
    if (countdown > 0)
    	{
    	countdown--;
    	if (countdown == 0)
    		{
        		    digitalWrite(CV_GATE_OUT, 0);
		    }
		}
		
    uint8_t ain = mozziAnalogRead(CV_POT_IN1);
    if (ain > HIGH && !a)
    	{
    	a = 2;
    	}
    else if (ain < LOW)
    	{
    	a = 0;
    	}
    else
    	{
    	if (a == 2) a--;
    	}
    	
    uint8_t bin = mozziAnalogRead(CV_POT_IN2);
    if (bin > HIGH && !b)
    	{
    	b = 2;
    	}
    else if (bin < LOW)
    	{
    	b = 0;
    	}
    else
    	{
    	if (b == 2) b--;
    	}
    
    uint8_t cin = mozziAnalogRead(CV_IN3);
    if (cin > HIGH && !c)
    	{
    	c = 2;
    	}
    else if (cin < LOW)
    	{
    	c = 0;
    	}
    else
    	{
    	if (c == 2) c--;
    	}
    	
    uint8_t din = mozziAnalogRead(CV_AUDIO_IN);
    if (din > HIGH && !d)
    	{
    	d = 2;
    	}
    else if (din < LOW)
    	{
    	d = 0;
    	}
    else
    	{
    	if (d == 2) d--;
    	}
    	
	if (a == 1 || b == 1 || c == 1 || d == 1)
		{
		uint8_t scale = mozziAnalogRead(CV_POT3) >> 6;		// 0...16
		uint8_t note = (a>0) + (b>0) * 2 + (c>0) * 4 + (d>0) * 8;		// we assume x>0 is 0 or 1
		pitchOut = positions[getPitch(note - 1, scale)];		// compensate for all zeros being rest
        a = 3;
        b = 3;
        c = 3;
        d = 3;
		countdown2 = 220;			// this seems to be the minimum amount of time for the pitch CV 
									// to get to its correct position before we do a trigger.  
									// If we go clear to 255, then we're too slow and start missing triggers, which is weird.
		}
    }

int updateAudio()    
    {
    if (countdown2 > 0)
    	{
    	countdown2--;
    	if (countdown2 == 0)
    		{
    		digitalWrite(CV_GATE_OUT, 1);
       	 	countdown = 1;						// looks like we don't need a countdown at all for the trigger
       	 	}
       	 }
    return pitchOut - 244;
    }

void setup() 
    {
    startMozzi();
    //Serial.begin(115200);
    }

void loop() 
    {
    audioHook();
    }
