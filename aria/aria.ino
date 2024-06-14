// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// ARIA
///
/// Aria is a monophonic sequencer with song mode.  Aria is meant to run on the 
/// AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// In Aria you define one or more PATTERNS, which are essentially melodies, and then
/// you define one of more SONGS.  A PATTERN is a list of notes and their lengths.
/// Lengths can be 16th notes, 8th notes, quarter notes, half notes, or whole notes.
/// Notes can be regular, staccato, or legato unless you defined staccato (no new gate).  
/// You can also have RESTS for those lengths.  You can extend lengths further with TIES.
///
/// A SONG is a list of patterns.  For each pattern in a song you can say how
/// many times each pattern is played before going on to the next pattern.  You can
/// also stipulate that a pattern is played FOREVER until the musician advances to the
/// next pattern in the song.  Songs can also be set up to loop forever.
///
/// Songs are begun by choosing a song, then raising a GATE.  Then sixteenth notes are
/// counted each time Aria receives a CLOCK signal, and notes are outputted appropriately.
/// When the GATE is dropped again, the song ends if it has not already done so.  As long as the
/// gate is raised, you can also trigger an ADVANCE to force the song to go to the next pattern.
///
/// Songs are output as PITCH OUT and as GATE OUT, which can be used to drive an oscillator
/// and a VCA envelope.
///
/// At present the gate for a sixteenth note is only dropped just before a new note, so there's 
/// no release, unless you defined staccato.  This may change in the future.
///
/// GRAINS OUTPUT WARNING: this program outputs note CV information from GRAINS.  If you feed this into
/// (for example) a VCO, 2OSC/d, or 2OSC, it will not be proper 1V/oct because they pull too much amperage 
/// and this causes the GRAINS to output incorrect voltages.  Even feeding into another GRAINS will cause 
/// the pitch to drop a bit.  You can fix this by feeding into a buffered mult first.  The 555 does not have
/// this issue (its inputs are buffered).
///
///
/// DEFINING PATTERNS AND SONGS
/// 
/// PATTERNS are defined below where it says:	////////// PATTERNS
/// SONGS are defined below where it says: 		////////// SONGS
///
/// That's where you set them in the code.  At some point in the near future I might make
/// a utility to convert MIDI files into patterns, so the patterns and/or songs may be
/// moved out to a separate file.
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





/// CONFIGURATION
///
/// IN 1            Advance
/// IN 2            Transpose CV
/// IN 3            Clock
/// AUDIO IN (A)    Gate In
/// AUDIO OUT       Pitch Out
/// DIGITAL OUT (D) Gate Out
///
/// POT 1           [UNUSED]
///					[Set switch to IN1 and set pot to far right]
///
/// POT 2           Transpose 
///					[If using Transpose CV, set switch to IN 2 and set pot to 2'oclock]
///					[If NOT using Transpose CV, set switch to MAN]
///					[If NOT transposeing at all, set switch to MAN and set pot to far left]
///
/// POT 3           Select Song




#define CONTROL_RATE 256

#include <MozziGuts.h>
#include "EEPROM.h"


#define CV_POT_IN1    A2    // Advance
#define CV_POT_IN2    A1    // Transpose
#define CV_POT3       A0    // Song
#define CV_IN3        A3    // Clock
#define CV_AUDIO_IN   A4    // Gate In
#define CV_AUDIO_OUT  9     // Pitch Out
#define CV_GATE_OUT   8     // Gate Out
#define RANDOM_PIN    A5


#define END (0)
#define _N8 (-8)
#define _N4 (-4)
#define _N2 (-2)
#define _N1 (-1)
#define _R (-13)
#define _R8 (-12)
#define _R4 (-11)
#define _R2 (-10)
#define _R1 (-9)
#define LEG (-14)
#define _LEG (-14)
#define TIE (-15)
#define _TIE (-15)
#define STO (-16)
#define _STO (-16)

#define _C (1)
#define _Db (2)
#define _D (3)
#define _Eb (4)
#define _E (5)
#define _F (6)
#define _Gb (7)
#define _G (8)
#define _Ab (9)
#define _A (10)
#define _Bb (11)
#define _B (12)

#define _C1 (_C + 12)
#define _Db1 (_Db + 12)
#define _D1 (_D + 12)
#define _Eb1 (_Eb + 12)
#define _E1 (_E + 12)
#define _F1 (_F + 12)
#define _Gb1 (_Gb + 12)
#define _G1 (_G + 12)
#define _Ab1 (_Ab + 12)
#define _A1 (_A + 12)
#define _Bb1 (_Bb + 12)
#define _B1 (_B + 12)

#define _C2 (_C + 24)
#define _Db2 (_Db + 24)
#define _D2 (_D +  24)
#define _Eb2 (_Eb + 24)
#define _E2 (_E + 24)
#define _F2 (_F + 24)
#define _Gb2 (_Gb + 24)
#define _G2 (_G + 24)
#define _Ab2 (_Ab + 24)
#define _A2 (_A + 24)
#define _Bb2 (_Bb + 24)
#define _B2 (_B + 24)

#define _C3 (_C + 36)
#define _Db3 (_Db + 36)
#define _D3 (_D +  36)
#define _Eb3 (_Eb + 36)
#define _E3 (_E + 36)
#define _F3 (_F + 36)

/*
// These are not accessible
#define _Gb3 (_Gb + 36)
#define _G3 (_G + 36)
#define _Ab3 (_Ab + 36)
#define _A3 (_A + 36)
#define _Bb3 (_Bb + 36)
#define _B3 (_B + 36)

#define _C4 (_C + 48)
#define _Db4 (_Db + 48)
#define _D4 (_D +  48)
#define _Eb4 (_Eb + 48)
#define _E4 (_E + 48)
#define _F4 (_F + 48)
#define _Gb4 (_Gb + 48)
#define _G4 (_G + 48)
#define _Ab4 (_Ab + 48)
#define _A4 (_A + 48)
#define _Bb4 (_Bb + 48)
#define _B4 (_B + 48)

#define _C5 (_C + 60)
*/





////////// PATTERNS


//// RULES FOR PATTERNS
//// We start at the beginning of the pattern.
//// We advance until we find a NOTE or a REST or a TIE or an END
//// A NOTE is defined by its PITCH, one of _C, _Cb, _D, ..., _C1, _Cb1, _D, ... _F3 (the
////     highest possible note).  When defined by itself, without a length, a note's length is
////     assumed to be a SIXTEENTH NOTE.
//// If before the NOTE we find a LENGTH (_N8, for eighth, _N4 for quarter, etc.), this indicates
////     the LENGTH of the NOTE.
//// Rests have lengths built-in (_R is sixteenth, _R8 is eighth, _R4 is quarter, etc.)
//// We may also find a TIE.  A TIE indicates that the previous note or rest or tie continues.
//// The length of a tie is by default a sixteenth note, but it can be preceeded by a LENGTH too.
//// A LEG mark indicates that we do not gate when going to the next note (NOTE_LEGATO).  It only makes
////     sense in front of NOTES.
//// A STO mark indicates that the gate should be very short (NOTE_STACCATO).  It only makes sense
////     in front of NOTES. 
//// An END (0) mark indicates that the pattern is finished.  If a pattern doesn't have one, it ends
//// at the end.

#define MAX_NOTES 1024		// be careful modifying this (see below)
#define MAX_PATTERNS 126	// don't modify this, it must stay at 126, because LOOP = 126 and FOREVER = 127, see SONGS below

#define NUM_PATTERNS 8		// don't exceed MAX_PATTERNS (126)
const PROGMEM int8_t patterns[NUM_PATTERNS][MAX_NOTES + 1] = 
	{
	// By default we have a few arpeggios and two melodies, plus 16 empty additional patterns.
	// You can add more patterns, but you need to change NUM_PATTERNS to accommodate them.
	// [And you can of course change or delete these patterns]
	// Each pattern can have no more than 1024 elements (notes, rests, other marks) in them.
	// [You can increase the number of elements by increasing MAX_NOTES, but remember that your 
	// total memory is only about 25,000 bytes, and you are consuming (MAX_NOTES + 1) * NUM_PATTERNS
	// memory here, not including the memory for SONGS]
	
 	{ LEG, _C,  LEG, _E,  LEG, _G, },	// Major Triad SMOOTH
	{ _C,  _E,  _G,  _A, },				// Major 6th
	{ _C,  _D,  _E,  _G,  _A, },		// Major 6th + 2
	{ _C,  _Eb, _G, },					// Minor Triad
	{ _C,  _Eb, _G,  _A, },				// Minor 6th
	{ _C,  _D,  _Eb, _G,  _A,},		// Minor 6th + 2
	// Happy Birthday
	{ _G,  _G,  _N8,  _A,  _N8,  _G,  _N8,  _C1,  _N4,  _B,  				// Happy Birthday to you
	  _G,  _G,  _N8,  _A,  _N8,  _G,  _N8,  _D1,  _N4,  _C1,   				// Happy Birthday to you
	  _G,  _G,  _N8,  _G1,  _N8,  _E1,  _N8,  _C1,  _N8,  _B,  _N8,  _A,  	// Happy Birthday dear children
	  _F1,  _F1,  _N8,  _E1,  _N8,  _C1,  _N8,  _D1,  _N4,  _C2, }, 		// Happy Birthday to you
	// Shave and a Haircut
	{ _N8,  _C1,  STO, _G, STO,  _G,  _N8,  _A,  _N4,  STO, _G,  _N8,  _B,  _N4,  _C1, },
	};
	
	
	
	



////////// SONGS
	
	
	
//// RULES FOR SONGS
//// We start at the beginning of the song.
//// We advance until we find a PATTERN.  A pattern is a number >= 1 and < NUM_PATTERNS <= MAX_PATTERNS
//// Along the way, if we find a negative value, this indicates the number of REPETITIONS of the pattern.
//// Also if we find FOREVER (127), this means that the pattern is repeated forever.
//// Also if we find LOOP (126), this means to loop back to the beginning and restart there.
//// Also if we find END (0), this means to end the song immediately.  Otherwise the song ends at its end.
//// If we find a value > NUM_PATTERNS and < LOOP, it is ignored.
////
//// There are NUM_SONGS total.  You can increase/decrease this but don't go over 16 as it'll be hard
//// to choose them on the pot dial.

#define NUM_SONGS 11			// You CAN but oughtn't exceed 16
#define FOREVER 127
#define LOOP 126
const PROGMEM int8_t songs[NUM_SONGS][MAX_PATTERNS + 1] = 
	{
	// You can change/delete these songs or add to them, but do not exceed 16 songs.  
	// You need to set NUM_SONGS to the number of songs.
	// [Well technically you can increase to more, but it'll be hard to choose them on the dial]
	// You must have at least one pattern!  For exampe, do not have a { LOOP } all by itself, 
	// or without a pattern before it, or you will get caught in an infinite loop
	
	{ FOREVER, 1, },		// Play the first arpeggio forever until ADVANCE is sent
	{ FOREVER, 2, },		// Play the second arpeggio forever until ADVANCE is sent
	{ FOREVER, 3, },		// Play the third arpeggio forever until ADVANCE is sent
	{ FOREVER, 4, },		// Play the fourth arpeggio forever until ADVANCE is sent
	{ FOREVER, 5, },		// Play the fifth arpeggio forever until ADVANCE is sent
	{ FOREVER, 6, },		// Play the sixth arpeggio forever until ADVANCE is sent
	{ 7, },				// Play Happy Birthday ONCE
	{ 8, },				// Play Shave and a Haircut ONCE
	{ FOREVER, 1, FOREVER, 2, FOREVER, 3, FOREVER, 4, FOREVER, 5, FOREVER, 6, LOOP, }, 	// Play each arpeggio forever until ADVANCE, then go to the next one, looping around
	{ -4, 1, -2, 3, -4, 4, -2, 6, },		// play Major 4 times, then Maj6+2 2 times, then Minor 4 times, then Min6+2 2 times, then stop
	{ -4, 2, -4, 5, LOOP, }					// play Maj6 4 times, then Min6 4 times, then loop
	};
	
	
	
	
	
	
	
	
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




	
#define GET_SONG_VAL(song,pos) pgm_read_byte_near(&songs[song][pos])
#define STATUS_START 0
#define STATUS_PLAYING 1
#define STATUS_END 2
uint8_t status = STATUS_START;		// are we playing, just starting, or done?

#define NO_SONG 255
uint8_t oldSong = NO_SONG;		// what was the previously chosen song?
uint8_t songPos = 0;			// where are we in the song's pattern array?
uint8_t pattern = 0;			// which pattern are we playing?
#define PATTERN_REPEAT_FOREVER 255
uint8_t patternRepeat = 0;		// how often do we repeat the pattern?  This counts down 
uint8_t patternPos = 0;			// where we in the pattern's notes array?
uint8_t atPatternEnd = false;

void advanceSong()
	{
	patternRepeat = 0;
	patternPos = 0;
	atPatternEnd = true;
	}

// We only call this when we have reached the END of the pattern (in next()).
void stepSong(uint8_t song, uint8_t start)
	{
	// First, is our pattern countdown at 0 yet?  If not 
	if (patternRepeat > 0) 
		{
		if (patternRepeat != PATTERN_REPEAT_FOREVER)
			patternRepeat--; 
		patternPos = 0;
		atPatternEnd = false; 
		}
	else
		{
		while(true)
			{
			int8_t val = GET_SONG_VAL(song, songPos);
			if (val == END) { status = STATUS_END; return; }
			else if (val == LOOP) { patternRepeat = 0; songPos = 0; patternPos = 0; }				// WARNING: Infinite LOOPS
			else if (val == FOREVER) { patternRepeat = PATTERN_REPEAT_FOREVER; songPos++; patternPos = 0;  }
			else if (val < 0) { patternRepeat = (0 - val) - 1; songPos++; patternPos = 0; }
			else { pattern = val; songPos++; atPatternEnd = false; break; }
			}
		}
	status = STATUS_PLAYING;
	}


#define GET_PATTERN_VAL(pattern,pos) pgm_read_byte_near(&songs[pattern][pos])
const uint8_t noteLengths[] = { 16, 8, 4, 2, 1 };	// Corresponding to _R1, _R2, _R4, _R8, _R16
// 0...60 are normal notes
// 70 ... 130 are NOTE_STACCATO notes
// 140 ... 200 are NOTE_LEGATO notes
// 201 is a NOTE_TIE
// 202 is a NOTE_REST
#define NORMAL 0
#define NOTE_STACCATO 70
#define NOTE_LEGATO 140
#define NOTE_TIE 201
#define NOTE_REST 202
uint8_t note = 0;					// Which note/rest/tie are we playing and what style?
uint8_t noteLength = 0;				// How long is the note/rest/tie playing?  This counts down.
uint8_t releaseLength = 0;			// How long before we do a release?  This counts down.

#define GATE_OUT_UP_COUNTDOWN 2
uint8_t gateOutUpCounter = 0;					// countdown for re-raising the gate after we have lowered it to start a new note
#define GATE_OUT_DOWN_COUNTDOWN 32
uint8_t gateOutDownCounter = 0;					// countdown for dropping the gate after we have finished a NOTE_STACCATO note


uint8_t stepPattern()
	{
	// First reduce the release length and do a release if need be
	if (releaseLength > 0)
		{
		releaseLength--;
		digitalWrite(CV_GATE_OUT, 0);
		gateOutUpCounter = 0;
		gateOutDownCounter = 0;
		}
		
	// First reduce the note length, and look for a new note if need be
	if (noteLength > 0)
		{
		noteLength--;
		note = NOTE_TIE;	// nothing new
		}
	else
		{
		uint8_t staccato = false;			// is the next note staccato?
		uint8_t legato = false;				// is the next note legato?
		releaseLength = 0;
		while(true)
			{
			int8_t val = GET_PATTERN_VAL(pattern, patternPos);
			if (val == END) { return false; }
			else if (val == LEG) { legato = true; patternPos++; }
			else if (val == STO) { staccato = true; patternPos++; }
			else if (val == TIE) { patternPos++; note = NOTE_TIE; return true; }  // do nothing
			else if (val >= _R1 && val <= _R) // rests
				{ 
				noteLength = noteLengths[0 - (val - _R1)]; 
				releaseLength = (noteLength > 1 ? noteLengths[0 - (val - _R1)] / 2: 1); 
				note = NOTE_REST; 
				patternPos++; 
				return true; 
				}
			else if (val >= _N8 && val <= _N1) 
				{
				noteLength = noteLengths[0 - (val - _N1)]; 
				releaseLength = (noteLength > 1 ? noteLengths[0 - (val - _N1)] / 2 : 1); 
				patternPos++; 
				}
			else if (val >= 0 && val <= 60) { note = val; if (staccato) note += NOTE_STACCATO; else if (legato) note += NOTE_LEGATO; patternPos++; return true; } 
			else { patternPos++; } // uh....
			}
		}
	}



uint8_t transpose = 0;							// How much to transpose the pitch in
uint16_t pitchOut = 0;							// Final pitch played by Mozzi

void next(uint8_t song)
	{
	// we're at the end of the song, make sure we're done
	if (status == STATUS_END)
		{
		digitalWrite(CV_GATE_OUT, 0);
		gateOutUpCounter = 0;
		gateOutDownCounter = 0;
		return;
		}
	
	uint8_t songStart = false;	// for Legato, can't use status == STATUS_START
	// If we have a new song, reinitialize
	if (oldSong != song)
		{
		oldSong = song;
		songPos = 0;
		pattern = 0;
		patternPos = 0;
		patternRepeat = 0;
		status = STATUS_START;
		atPatternEnd = false;
		}
	
	//  What's the next note?  And quit if we're at END, just like above
	while(true)
		{
		// First step to the next pattern if we're at the start of the song or at the end of the last pattern
		if (atPatternEnd || status == STATUS_START)
			stepSong(song, status == STATUS_START);				// update what pattern we're using
		// we're at the end of the song, make sure we're done
		if (status == STATUS_END)
			{
			digitalWrite(CV_GATE_OUT, 0);
			gateOutUpCounter = 0;
			gateOutDownCounter = 0;
			return;
			}
		// Finally step within the pattern and load the note
		if (atPatternEnd = stepPattern()) break;						// found a note
		}
	
	// Now we have a note.  What do we do with it?
	if (note == NOTE_TIE)
		{
 		// A TIE.  Do nothing!  We're already doing what we need
 		}
 	else if (note == NOTE_REST)
 		{
 		// A REST. Drop the gate
		digitalWrite(CV_GATE_OUT, 0);
		gateOutUpCounter = 0;
		gateOutDownCounter = 0;
 		}
 	else if (note < NOTE_STACCATO)
 		{
 		// NORMAL note, temporarily drop the gate to reset the envelope
 		pitchOut = positions[note] + transpose;
 		if (pitchOut > 41) pitchOut = 41;
		digitalWrite(CV_GATE_OUT, 0);
		gateOutUpCounter = GATE_OUT_UP_COUNTDOWN;
 		}
 	else if (note < NOTE_LEGATO)
 		{
 		// STACCATO note, temporarily drop the gate and then drop it hard
 		pitchOut = positions[note - NOTE_STACCATO] + transpose;
 		if (pitchOut > 41) pitchOut = 41;
		digitalWrite(CV_GATE_OUT, 0);
		gateOutUpCounter = GATE_OUT_UP_COUNTDOWN;
		gateOutDownCounter = GATE_OUT_DOWN_COUNTDOWN;
 		}
 	else
 		{
  		if (songStart)
 			{
 			// Normal note because we're the first one
	  		pitchOut = positions[note] + transpose;
	 		if (pitchOut > 41) pitchOut = 41;
			digitalWrite(CV_GATE_OUT, 0);
			gateOutUpCounter = GATE_OUT_UP_COUNTDOWN;
			}
		else
			{
			// LEGATO note.  Don't touch the gate
	 		pitchOut = positions[note - NOTE_LEGATO] + transpose;
	 		if (pitchOut > 41) pitchOut = 41;
			}	
 		}
	}
	
	
	

uint8_t gate = false;								// has the gate been raised already?
uint8_t clock = false;								// is the clock on already?
uint8_t advance = false;							// is advance triggered already?

void updateControl() 
    {
	uint16_t g = mozziAnalogRead(CV_AUDIO_IN);		// get gate value
	if (g < 100)
		{
		// Not gated -- we're not playing, make sure everything relevant is reset
		gateOutUpCounter = 0;
		gateOutDownCounter = 0;
		oldSong = NO_SONG;
		digitalWrite(CV_GATE_OUT, 0);
		}
	else if (g > 500)
		{
		gate = true;

		// Okay we're gated.  Do we need to raise the gate out?
		if (gateOutUpCounter > 0)
			{
			if (gateOutUpCounter == 1) digitalWrite(CV_GATE_OUT, 1);
			gateOutUpCounter--;
			}

		// Okay our note is NOTE_STACCATO.  Do we need to LOWER the gate out prematurely?
		if (gateOutDownCounter > 0)					// note NOT else if
			{
			if (gateOutDownCounter == 1) digitalWrite(CV_GATE_OUT, 0);
			gateOutDownCounter--;
			}
		
		// Advance
		uint16_t a = mozziAnalogRead(CV_POT_IN1);
		if (a < 100)
			{
			advance = false;
			}
		else if (a > 500 && !advance)
			{
			// we're advancing
			advance = true;
			advanceSong();
			}

		// Are we clocked?
		uint16_t c = mozziAnalogRead(CV_IN3);
		if (c < 100)
			{
			clock = false;
			}
		else if (c > 500 && !clock)
			{
			// okay, we clocked.  Figure the note
			clock = true;
			transpose = getPitchCV(CV_POT_IN2);
			uint8_t song = (mozziAnalogRead(CV_POT3) * NUM_SONGS) >> 10;
			next(song);
			}
		}
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
    }

void loop() 
    {
    audioHook();
    }
