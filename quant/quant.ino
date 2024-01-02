// Copyright 2023 Sean Luke
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


/// QUANT
///
/// Quant is a note quantizer.  Quant is meant to run on the AE Modular GRAINS, but it 
/// could be adapted to any Arduino.
///
/// Quant is pretty simple.  You provide a CV on IN 1, and specify the scale you would
/// like to quantize to. The pitch is quantized to AUDIO OUT.
///
/// RANGE AND RESOLUTION
/// 
/// Mozzi's output is capable of a range of 42 notes. That's about 3.5 octaves.  This 
/// is also the quantizer's range: values above that will just get quantized to the 
/// top note.
///
/// The range is a bit nonlinear.  I have a lookup table in QUANT which gets as close 
/// as I can to my own GRAINS but I do not yet know: 
///
/// - Does it match other people's GRAINS? 
/// 
/// - Does it change as the GRAINS warms up?  This is hard to nail down because I am 
/// driving VCOs, which also warm up... 
///
/// I could use some help here: if I can't get it to match, it'll be hard to output 
/// notes from GRAINS for some other projects I have in mind.  Send me mail and let 
/// me know how it does on your unit.
///
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
/// Grains's Inputs track 1.3V/octave, not 1V/octave: we'll need to scale them to track properly.  
/// To do this, you can adjust the Pitch CV Scaling on Pot 1.  This GRAINS program is set up to 
/// associate 0V with note 0, and 5V with note 60 (of course).  However you need to get the tracking
/// (scaling) properly trimmed or that won't happen.  To do this, the easiest thing to do is to set the
/// scale to OCTAVES (Pot 2 fully right, Pot 3 fully left), play notes, and adjust the tracking to get
/// the octaves to scale properly.
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

// GRAINS's PWM output is not quite uniform.  This table is tweaked
// specifically for my unit.  I do not know if it is consistent for
// other units, but I hope it is!  The range is just under 3.5 octaves,
// and starts at slightly above 0V.
uint16_t positions[42] = 
{  
   0,   13,  23,  32,  41,  50,  59,  68,  78,  87,  96,  106, 	// by 9.3, except first which jumps by 13
   116, 125, 135, 144, 154, 163, 173, 182, 192, 202, 212, 222,	// by 9.666
   232, 242, 251, 261, 271, 281, 291, 300, 310, 320, 330, 339, 	// by 9.75
   349, 358, 368, 377, 386, 397		// by 9.25, except last which jumps by 11.  One more pitch is exists but is highly nonlinear	
};

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
	if (pitch > 41) pitch = 41;		// highest position
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
