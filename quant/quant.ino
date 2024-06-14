// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// QUANT
///
/// Quant is a note quantizer.  Quant is meant to run on the AE Modular GRAINS, but it 
/// could be adapted to any Arduino.
///
/// Quant is pretty simple.  You provide a CV on IN 1, and specify the scale you would
/// like to quantize to. The pitch is quantized to AUDIO OUT.
///
/// GRAINS OUTPUT WARNING: this program outputs note CV information from GRAINS.  If you feed this into
/// (for example) a VCO, 2OSC/d, or 2OSC, it will not be proper 1V/oct because they pull too much amperage 
/// and this causes the GRAINS to output incorrect voltages.  Even feeding into another GRAINS will cause 
/// the pitch to drop a bit.  You can fix this by feeding into a buffered mult first.  The 555 does not have
/// this issue (its inputs are buffered).
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
/// One of the issues in using this quantizer is that GRAINS does not have a buffered:
/// the voltage its output will produce is strongly affected by the amperage being pulled
/// by the oscillator it's plugged into, and different AE oscillators pull different amounts.
/// VCO is particularly bad here -- it pulls a lot of voltage, thus scaling down Quant's output
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



/// SCALES
///
/// Scales come in three categories.  You provide the category via POT 1, and then the 
/// scale in the category via POT 2.  There are three categories and 10 scales per category.
///
/// CATEGORY 1: CHROMATIC AND 7-TONE SCALES
/// Major
/// Harmonic Minor
/// Melodic Minor
/// Dorian
/// Phrygian
/// Lydian
/// Mixolydian
/// Aeolian (Relative Minor)
/// Locrian
/// Chromatic
///
/// CATEGORY 2: OTHER SCALES
/// Blues Minor
/// Pentatonic
/// Minor Pentatonic
/// Japanese Pentatonic
/// Whole Tone
/// Hungarian Gypsy
/// Phrygian Dominant
/// Persian
/// Diminished (Octatonic)
/// Augmented (Hexatonic)
///
/// CATEGORY 2: CHORDS
/// Octave
/// 5th + Octave 
/// Major Triad
/// Minor Triad
/// Augmented Triad
/// 7
/// Major 7
/// Minor 7
/// Diminished 7
/// Minor-Major 7


/// ADJUSTING TUNING AND TRACKING
///
/// Not only does Quant need to adjust its OUTPUT PITCH to the appropriate oscillator due to lack
/// of buffering, but you also have to adjust its INPUT PITCH TRACKING as well. 
///
/// Grains's Inputs track 1.3V/octave, not 1V/octave: we'll need to scale them to track properly.  
/// To do this, you can adjust the Pitch CV Scaling on Pot 1.  This GRAINS program is set up to 
/// associate 0V with note 0, and 5V with note 60 (of course).  However you need to get the tracking
/// (scaling) properly trimmed or that won't happen.  To do this, the easiest thing to do is to set the
/// scale to OCTAVES (Pot 2 fully right, Pot 3 fully left), play notes, and adjust the tracking to get
/// the octaves to scale properly.
///
/// Note that as GRAINS resistors warm up, the scaling will change and you will need to adjust the 
/// tracking again, at least until they are fully warmed up.

/// CONFIGURATION
///
/// IN 1            Pitch In
/// IN 2            [UNUSED]
/// IN 3            [UNUSED]
/// AUDIO IN (A)    [UNUSED]
/// AUDIO OUT       Quantized Pitch Out
/// DIGITAL OUT (D) [UNUSED]
///
/// POT 1           Pitch In CV Tracking Adjust
///
///
/// POT 2           Scale Category
///					[Set switch to MAN]
///
/// POT 3           Scale



#define CONTROL_RATE 256

#include <MozziGuts.h>
#include "EEPROM.h"

#define CV_POT_IN1    A2    // Pitch In
#define CV_POT_IN2    A1    // Scale Category
#define CV_POT3       A0    // Scale
#define CV_IN3        A3    // Unused
#define CV_AUDIO_IN   A4    // Unused
#define CV_AUDIO_OUT  9     // Pitch Out
#define CV_GATE_OUT   8     // Unused
#define RANDOM_PIN    A5

    

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
// which returns a PITCH from 0...59 or so.
inline uint8_t getPitch(uint8_t pitch)
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
    return (uint8_t) ((pitchCV * 60) >> 10);		// now should range 0...60 or maybe 0...59
    }

#define NUM_SCALES 30
#define NUM_NOTES 12
const uint8_t scales[NUM_SCALES][NUM_NOTES] = 
    {
	/// COMMON SCALES
//    C  Db D  Eb E  F  Gb G  Ab A  Bb B
	{ 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1 },		// Major
	{ 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1 },		// Harmonic Minor
	{ 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1 },		// Melodic Minor
	{ 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0 },		// Dorian
	{ 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0 },		// Phrygian
	{ 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1 },		// Lydian
	{ 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0 },		// Mixolydian
	{ 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0 },		// Aeolian (Relative Minor)
	{ 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0 },		// Locrian
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },		// Chromatic

	/// OTHER SCALES
//    C  Db D  Eb E  F  Gb G  Ab A  Bb B
	{ 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0 },		// Blues Minor
	{ 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0 },		// Pentatonic
	{ 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0 },		// Minor Pentatonic
	{ 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0 },		// Japanese Pentatonic
	{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 },		// Whole Tone
	{ 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0 },		// Hungarian Gypsy
	{ 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0 },		// Phrygian Dominant
	{ 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1 },		// Persian
	{ 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1 },		// Diminished (Octatonic)
	{ 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1 },		// Augmented (Hexatonic)

	// CHORDS
//    C  Db D  Eb E  F  Gb G  Ab A  Bb B
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },		// Octave
	{ 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 },		// 5th + Octave 
	{ 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0 },		// Major Triad
	{ 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0 },		// Minor Triad
	{ 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 },		// Augmented Triad
	{ 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 },		// 7
	{ 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1 },		// Major 7
	{ 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0 },		// Minor 7
	{ 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0 },		// Diminished 7
	{ 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1 },		// Minor-Major 7
	};

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
   314, 323, 331, 340, 348, 356, 365, 373, 382, 390, 403, 403	
};
#endif

uint8_t quantizeToScale(uint8_t pitch, uint8_t scale)
	{
	if (pitch >= 59) pitch = 59;
	// avoid a % ...
	uint8_t octave = (pitch < 36 ? 
		( pitch < 24 ? (pitch < 12 ? 0 : 1 ) : 2 ) :
		( pitch < 48 ? 3 : 4)) * 12;
	uint8_t note = pitch - octave;
	
	while(true)
		{
		// note: all scales have C = 1, so this is safe
		if (scales[scale][note]) break;
		note--;
		}
	
	pitch = octave + note; 
	if (pitch > 47) pitch = 47;		// highest position
	return pitch;
	}

int16_t pitchOut;
void updateControl() 
    {
    // Determine Scale 
    uint16_t category = (mozziAnalogRead(CV_POT_IN2) * 3) >> 10;
    uint16_t scale = (mozziAnalogRead(CV_POT3) * 10) >> 10;

    // we assume quantize goes 0...60 inclusive.
    // We convert this to 0...488 by multiplying by 8 and call it a day.
	pitchOut = positions[quantizeToScale(getPitch(CV_POT_IN1), category * 10 + scale)];
    }

int updateAudio()    
    {
    return pitchOut - 244;
    }

void setup() 
    {
	initializePitch(CV_POT_IN1);
    startMozzi();
    //Serial.begin(9600);
    //digitalWrite(CV_GATE_OUT, 1);
    }

void loop() 
    {
    audioHook();
    }
