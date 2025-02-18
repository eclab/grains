// Copyright 2025 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// ARP
///
/// Arp is an arpeggiator.  Arp is meant to run on the AE Modular GRAINS, but it could be 
/// adapted to any Arduino.
///
/// To use Arp, you specify the CHORD to arpeggiate and the STYLE with which to arpeggiate it.
/// You can also optionally specify an INVERSION of the chord to shift the arpeggio.
///
/// Whenever you provide a CLOCK, the arpeggio will output as CV the next note in the STYLE.
/// The CV will be transposed to match the scale of the current input PITCH you have provided.
/// (the PITCH is thus the root note of scale).  You can RESET the style back to 0 at any point.
///
/// Because Arp has a bit of latency in responding to a clock, it provides its own clock out
/// to precisely indicate when it has changed its note.  You can attach this to envelopes.  The
/// clock out will go HIGH when Arp has changed its note after your clock in has gone high, 
/// and will go LOW as soon as Arp detects that your clock in has gone low.
///
/// CHORDS
/// There are 16 chords, chosen by POT 2:
///
/// CHORD			NUM NOTES IN THE CHORD (see INVERSIONS below)
/// Maj 5 + 2		4 notes
/// M6 + 2			5 notes
/// M7 + 2			5 notes
/// M7				4 notes
/// 7				4 notes
/// Maj 5			3 notes
/// Root and 5th	2 notes
/// min 5			3 notes
/// m7				4 notes
/// m7 + 2			5 notes
/// m7 + 6 + 2		6 notes
/// m6 + 2			5 notes
/// min 5 + 2		4 notes
/// dim7			4 notes
/// Major Scale		7 notes	[good for random arpeggios]
/// Minor Scale 	7 notes	[good for random arpeggios]
///
///
/// STYLES
/// There are 12 styles:
///
/// ONE OCTAVE
/// Up
/// Down
/// Up-Down
/// Up-Down-+	[Plays the root at the top of the arpeggio as well as the bottom]
/// Random		[Plays random notes in the chord
/// Random Walk	[Randomly wanders up or down through the chord]
///
/// TWO OCTAVES
/// Up
/// Down
/// Up-Down
/// Up-Down-+	[Plays the root at the top of the arpeggio as well as the bottom]
/// Random		[Plays random notes in the chord
/// Random Walk	[Randomly wanders up or down through the chord]
///
///
/// INVERSIONS
/// You can invert the chord up, all the way through all its notes in the first octave.
/// This is done by setting a #define here (sorry, there just aren't enough pots and inputs).
/// The default is 0, which is "no inversion".  1 is the first inversion, and so on.  Don't
/// go more than the number of notes in your chord (the maximum number of notes of any chord
/// is 7, for the m7 + 6 + 2 chord and for the two scales).  See NUM NOTES IN THE CHORD in the
/// CHORDS section above.

#define INVERSION 0

/// EXCEEDING MAXIMUM PITCH
/// If your arpeggio note exceeds the maximum pitch, the note will automaticlly be transposed
/// down one or more octaves until it is within the pitch range.
///
///
/// INPUT PITCH
/// Grains is odd in that the input to IN1 isn't necessarily 1V/oct.  You'll have to tune it.
/// This is done by adjusting POT1.  I have found that if POT1 is in approximately the 2'oclock
/// position, you'll get close to 1V/oct and you can tweak it from there.


/// OUTPUT PITCH RANGE AND RESOLUTION
/// 
/// Mozzi cannot go down to 0V.  Its minimum is a little more, transposing up by about a half
/// of a semitone.   Most oscillators can be tuned to deal with this.
/// But GRAINS oscillators have to be manually adjusted.  So for example, if you're attaching 
/// this program to a GRAINS oscillator like DX, you might want to change the TRANPOSE_BITS
/// to about -6 on the GRAINS oscillator.
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
/// IN 1            Pitch CV
/// IN 2            Chord CV
/// IN 3            Clock
/// AUDIO IN (A)    Reset
/// AUDIO OUT       Pitch Out
/// DIGITAL OUT (D)	Clock Out
///
/// POT 1           Pitch	[If you're using Pitch CV, set the switch to IN1 and set this value to about 2'oclock]
///
/// POT 2           Chord	[If you're using Chord CV, set the switch to IN2, else to MAN]
///					
/// POT 3           Style





#define CONTROL_RATE 256

#include <MozziGuts.h>
#include "EEPROM.h"


#define CV_POT_IN1    A2    // Pitch
#define CV_POT_IN2    A1    // Chord
#define CV_POT3       A0    // Style
#define CV_IN3        A3    // Clock
#define CV_AUDIO_IN   A4    // Reset
#define CV_AUDIO_OUT  9     // Pitch Out
#define CV_GATE_OUT   8     // Clock Out
#define RANDOM_PIN    A5

	
	
	
	
////// COMPUTING PITCH IN
////// This is for transposition, nothing more


#define MEDIAN_OF_THREE(a,b,c) (((a) <= (b)) ? (((b) <= (c)) ? (b) : (((a) < (c)) ? (c) : (a))) : (((a) <= (c)) ? (a) : (((b) < (c)) ? (c) : (b))))
uint16_t pitchCV;
uint16_t pA;
uint16_t pB;

// This is a modified version of my standard initializeFrequency(...) function
void initializePitch(uint8_t pitch)
    {
    pitchCV = mozziAnalogRead(pitch);
    }
        
#define LARGE_JUMP 32
#define SEMITONE 17
#define FREQ_COUNTER_MAX 4
uint8_t freqCounter = 0;
// This is a modified version of my standard getFrequency(...) function
// which returns a PITCH from 0...35 or so.
inline uint8_t getPitchCV(uint8_t pitch)
    {
    uint16_t p = mozziAnalogRead(pitch);

    uint16_t diff = (p > pitchCV ? p - pitchCV : pitchCV - p);
    if (diff >= LARGE_JUMP)
        {
        pitchCV = p;            // jump right there
        freqCounter = FREQ_COUNTER_MAX;
        }
    else if (freqCounter > 0)
        {
        freqCounter--;
        pitchCV = (pitchCV + p) >> 1;
        pB = pA;
        pA = pitchCV;
        }
    else
        {
        uint16_t p1 = MEDIAN_OF_THREE(p, pA, pB);
        pB = pA;
        pA = p;
        pitchCV = (pitchCV * 7 + p1) >> 3;
        }
    return (uint8_t) ((pitchCV * 36) >> 10);
    }





//// COMPUTING PITCH OUT

// GRAINS's PWM output is not quite uniform.  This table is tweaked
// specifically for my unit.  I do not know if it is consistent for
// other units, but I hope it is!  The range is just under 3.5 octaves,
// and starts at slightly above 0V.
/*
uint16_t positions[42] = 
{  
   0,   13,  23,  32,  41,  50,  59,  68,  78,  87,  96,  106, 	// by 9.3, except first which jumps by 13
   116, 125, 135, 144, 154, 163, 173, 182, 192, 202, 212, 222,	// by 9.666
   232, 242, 251, 261, 271, 281, 291, 300, 310, 320, 330, 339, 	// by 9.75
   349, 358, 368, 377, 386, 397		// by 9.25, except last which jumps by 11.  One more pitch is exists but is highly nonlinear	
};
*/



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
uint8_t numPositions = 47;
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
uint8_t numPositions = 42;
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
uint8_t numPositions = 47;
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
uint8_t numPositions = 47;
#elif defined(OUTPUT_2OSCD)
uint16_t positions[48] = 
{  
// C    C#   D    D#   E    F    F#   G    G#   A     A#   B
   0,   13,  22,  30,  39,  47,  56,  64,  73,  82,  90,  99,
   107, 116, 125, 133, 142, 151, 159, 168, 176, 185, 194, 202,
   211, 220, 228, 237, 245, 254, 262, 271, 280, 288, 297, 306,
   314, 323, 331, 340, 348, 356, 365, 373, 382, 390, 403, 403			// 2OSC2 has 47 notes
};
uint8_t numPositions = 47;
#endif


uint8_t chords[16][12] = 
{ 
//    C  C# D  D# E  F  F# G  G# A  A# B
// MAJOR
	{ 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0 },		// 2
	{ 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0 },		// 2 + 6 [Pentatonic]
	{ 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1 },		// 2 + M7
//	{ 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0 },		// 2 + 7				NO
	{ 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1 },		// M7
	{ 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 },		// 7
	{ 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0 },		// M
// MINOR
	{ 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 },		// 5
	{ 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0 },		// m
	{ 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0 },		// m7
	{ 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0 },		// 2 + m7
	{ 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0 },		// 2 + 6 + m7
	{ 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0 },		// 2 + 6 [Pentatonic]
	{ 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0 },		// 2
// OTHER
	{ 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0 },		// dim7
	{ 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1 },		// Major
	{ 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0 },		// minor
//	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },		// chromatic			NO
//	{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 },		// whole tone			NO
};

uint8_t numNotes[16] = { 4, 5, 5, 4, 4, 3, 2, 3, 4, 5, 6, 5, 4, 4, 7, 7 };

uint8_t pitch;
uint16_t chord;
uint16_t style;

#define UP					0
#define DOWN				1
#define UPDOWN				2
#define UPDOWN_PLUS			3
#define RANDOM				4
#define RANDOM_WALK			5
#define UP2					6
#define DOWN2				7
#define UPDOWN2				8
#define UPDOWN_PLUS2		9
#define RANDOM2				10
#define RANDOM2_WALK		11



uint8_t lastS = 0;
uint8_t lastC = 0;

#define MAX_CHORD_MAP_SIZE (7 * 2 + 3)
int8_t chordMap[MAX_CHORD_MAP_SIZE];
uint8_t chordMapLen = 0;

void buildMap()
	{
	for(uint8_t i = 0; i < MAX_CHORD_MAP_SIZE; i++)
		{
		chordMap[i] = -1;
		}
		
	uint8_t inv = INVERSION;
	uint8_t pos = 0;
	for(uint8_t oct = 0; oct < 3; oct++)
		{
		for(uint8_t j = 0; j < 12; j++)
			{
			if (inv > 0)
				{
				inv--;		// skip the first notes in the inversion
				}
			else if (chords[lastC][j])
				{
				chordMap[pos] = oct * 12 + j;
				pos++;
				if (lastS == UPDOWN_PLUS)
					{
					if (pos >= numNotes[lastC] + 1) { chordMapLen = pos; return; }
					}
				else if (lastS == UPDOWN_PLUS2)
					{
					if (pos >= numNotes[lastC] * 2 + 1) { chordMapLen = pos; return; }
					}
				else if (lastS >= UP2)
					{
					if (pos >= numNotes[lastC] * 2) { chordMapLen = pos; return; }
					}
				else
					{
					if (pos >= numNotes[lastC]) { chordMapLen = pos; return; }
					}
				}
			}
		}
	}


int8_t idx;
uint8_t direction;
uint8_t clock;
uint8_t reset;

void doReset()
	{
	buildMap();
	idx = -1;
	direction = 0;		// up
	clock = false;
	digitalWrite(CV_GATE_OUT, 0);
	}
	
#define COUNTDOWN 3
uint8_t countdown = 0;

void updateControl() 
    {
    if (countdown >= 1)
    	{
    	if (countdown == 1)
    		{
			digitalWrite(CV_GATE_OUT, 1);
			}
		countdown--;
    	}
    
		// smooth
		pitch = getPitchCV(CV_POT_IN1);
		chord = (chord * 3 + mozziAnalogRead(CV_POT_IN2)) >> 2;
		style = (style * 3 + mozziAnalogRead(CV_POT3)) >> 2;

    	// reset?
		uint16_t r = mozziAnalogRead(CV_AUDIO_IN);
		if (r < 100 && reset)
			{
			reset = false;
			}
		else if (r > 200 && !reset)
			{
			reset = true;
			doReset();
			}

		// Are we clocked?
		uint16_t c = mozziAnalogRead(CV_IN3);
		if (c < 100 && clock)
			{
			clock = false;
			digitalWrite(CV_GATE_OUT, 0);
			}
		else if (c > 200 && !clock)
			{
			clock = true;
			go();
			countdown = COUNTDOWN;
			}
    }

int8_t out = 0;

void go()
	{
	uint8_t s = (style * 12) >> 10;		// 0...11
	uint8_t c = (chord >> 6);			// 0...15
	//uint8_t i = (inversion * numNotes[c]) >> 10;		// varies
	if (s != lastS || c != lastC)
		{
		lastS = s;
		lastC = c;
		doReset();
		}
	
	switch((style * 12) >> 10)
		{
		case UP:
		case UP2:
			{
			if (idx == -1 || chordMap[idx] < 0) idx = 0;
			out = chordMap[idx];
			idx++;
			break;
			}
		case DOWN:
			{
			if (idx == -1 || chordMap[idx] < 0) idx = numNotes[lastC] - 1;
			out = chordMap[idx];
			idx--;
			break;
			}
		case DOWN2:
			{
			if (idx == -1 || chordMap[idx] < 0) idx = numNotes[lastC] * 2 - 1;
			out = chordMap[idx];
			idx--;
			break;
			}
		case UPDOWN:
		case UPDOWN_PLUS:
			{
			if (direction == 0)		// up
				{
				if (idx == -1) idx = 0;
				out = chordMap[idx];
				idx++;
				if (chordMap[idx] < 0) { direction = 1; idx -= 2; }
				}
			else 					// down
				{
				if (chordMap[idx] < 0) idx = numNotes[lastC] - 1;
				out = chordMap[idx];
				idx--;
				if (idx == -1) { direction = 0; idx = 1; }
				}
			break;
			}
		case UPDOWN2:
		case UPDOWN_PLUS2:
			{
			if (direction == 0)		// up
				{
				if (idx == -1) idx = 0;
				out = chordMap[idx];
				idx++;
				if (chordMap[idx] < 0) { direction = 1; idx -= 2; }
				}
			else 					// down
				{
				if (chordMap[idx] < 0) idx = numNotes[lastC] * 2 - 1;
				out = chordMap[idx];
				idx--;
				if (idx == -1) { direction = 0; idx = 1; }
				}
			break;
			}
		case RANDOM:
		case RANDOM2:
			{
			while(1)
				{
				uint8_t val = random(chordMapLen);
				if (val != idx)
					{
					idx = val;
					out = chordMap[idx];
					break;
					}
				}
			break;
			}
		case RANDOM_WALK:
		case RANDOM2_WALK:
			{
			while(1)
				{
				uint8_t up = random(2);
				if (up && idx + 1 < chordMapLen)
					{
					idx++;
					out = chordMap[idx];
					break;
					}
				else if (!up && idx - 1 >= 0)
					{
					idx--;
					out = chordMap[idx];
					break;
					}
				}
			}
		}
	}
	
int updateAudio()    
    {
    int p = out + pitch;
    while (p >= numPositions) p -= 12;	// drop an octave
    return positions[p] - 244;
    }

void setup() 
    {
	initializePitch(CV_POT_IN1);
	pinMode(CV_IN3, INPUT);
	randomSeed(RANDOM_PIN);
    startMozzi();
    Serial.begin(9600);
    doReset();
    }

void loop() 
    {
    audioHook();
    }