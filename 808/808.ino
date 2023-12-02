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
//
// Each of the Drum ".h" files is under Mozzi's license.


/// 808
///
/// 808 is a drum sampler with up to 9 voices and up to 5 triggers.  In some cases you
/// choose which voices sound for a given trigger.  It's comes with an assortment of low-rent,
/// noisy 8-bit drum samples.  drum sampler with low-rent, noisy 8-bit drum samples presently
/// taken from a TR-808.  808 is meant to run on the AE Modular GRAINS, but it could be 
/// adapted to any Arduino.
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.
///
/// To set up 808, you just have to do two things:
///
/// - State the sample files you're using and
///
/// - State which FORMAT you're using.  The FORMAT is the layout of the 808's interface.
///   There are 11 formats available, and they dictate both how many samples will be used
///   which will be grouped to the same trigger (you can only play one sample from a group
///   on a given trigger at a given time).
///
/// MEMORY ALLOCATION
/// 808 provides about 26000 bytes of memory for your samples.  The total byte length of
/// your samples cannot exceed this amount.  [I'll try to squeeze out some more bytes if I have time].
///
/// NOTE ABOUT DIGITAL OUT PIN
/// 808 uses the Digital Out pin as its first trigger input.  This pin is different from the
/// others.  If you leave a bare disconnected wire in this pin, it will cause GRAINS to
/// receive noise which it will interpret as a bunch of triggers, resulting in your kick 
/// drum (or whatever) going nuts.  So disconnect the wire entirely from the socket, or plug
/// the wire into *something* else at the other end.
/// 
///
/// SETTING THE SAMPLE FILES
/// Each sample file is a ".h" file which has been converted from a WAV file using Mozzi's
/// tools, and slightly tweaked so it can be loaded by 808 easily.  I provide a bunch of
/// examples.  I also provide 9 files called "blank1.h" through "blank9.h".  If you do not
/// have 9 samples to load, fill the remaining slots with one each of these "blank" files.
/// It doesn't matter which ones you use as long as each slot has a diffrent one.  
/// ALL 9 SLOTS must have unique files.
/// 
// The default, which you can change, has 6 drum samples and three blank samples.
// That takes up 89% of memory.  If you want more samples, they have to be SHORT.

#define SAMPLE_1	"kick808.h"
#define SAMPLE_2	"clap808.h"
#define SAMPLE_3	"rim808.h"
#define SAMPLE_4	"hat808.h"
#define SAMPLE_5	"claves808.h"
#define SAMPLE_6	"tomHigh808.h"
#define SAMPLE_7	"snare.h"		// not enough memory for "tomMid808.h"  We're doing FORMAT_6 anyway
#define SAMPLE_8	"shaker.h"		// not enough memory for "tomLow808.h"  We're doing FORMAT_6 anyway
#define SAMPLE_9	"click.h"

/// Here is an example with all empty samples, showing use of all the "blank" files.
/// You probably don't want this!  :-)

///		#define SAMPLE_1	"blank1.h"
///		#define SAMPLE_2	"blank2.h"
///		#define SAMPLE_3	"blank3.h"
///		#define SAMPLE_4	"blank4.h"
///		#define SAMPLE_5	"blank5.h"
///		#define SAMPLE_6	"blank6.h"
///		#define SAMPLE_7	"blank7.h"
///		#define SAMPLE_8	"blank8.h"
///		#define SAMPLE_9	"blank9.h"


/// U-LAW SAMPLES
///
/// NOTE: THE U-LAW CODE HAS NOT BEEN TESTED YET.  DON'T TRY IT UNTIL I HAVE IT VERIFIED.
/// Your samples can be ALL 8-bit raw linear, or they can ALL be 8-bit raw uLaw.  If they are
/// uLaw, you need to uncomment the following #define, that is, remove the //

// #define USE_ULAW

// Note that when using U-Law, 808 will make the samples sound louder, so you'll need
// to turn the volume pot down.  The U-Law code also very slightly reduces the available
// memory for your samples in GRAINS.


/// ADJUSTING INDIVIDUAL SAMPLE VOLUME
///
/// Is a sample too loud compared to the others?  
/// You can adjust the relative gain of each sample by reducing any of the following values.
/// The values go between 0 (fully muted) to 16 (standard volume) to 50 (maximum volume)

#define GAIN_0		16
#define GAIN_1		16
#define GAIN_2		16			// rim808.h is quiet: it would benefit from being boosted, though it'll clip in one spot
#define GAIN_3		16
#define GAIN_4		16
#define GAIN_5		16
#define GAIN_6		16
#define GAIN_7		16
#define GAIN_8		16
#define GAIN_9		16


/// SETTING THE FORMAT
/// There are 11 formats, and they dictate the number of samples used, how they're
/// grouped, and how the interface is presented.   Here are the format names, don't
/// play with this:

#define FORMAT_1	0		// A single sample, plus editable start/end points and pitch control
#define FORMAT_2	1		// Two samples, each with pitch control, plus total volume control
#define FORMAT_3	2		// Three samples, the first two with pitch control, plus total volume control
#define FORMAT_4	3		// Four samples, the first one with pitch control, plus total volume control
#define FORMAT_5	4		// Has 5 triggers: thus not for TRIQ164 or GRAINS BEATS
#define FORMAT_5A	5		// An alternative to FORMAT_5 for people with 4-trigger step sequencers like TRIQ164 or GRAINS BEATS 
#define FORMAT_6	6		// Six samples and four triggers: the last three samples share a trigger
#define FORMAT_7	7		// Seven samples and four triggers: the last four samples share a trigger
#define FORMAT_7A	8		// An alternative to FORMAT_7, with three triggers.  The last three samples share a trigger, and the next three share another
#define FORMAT_8	9		// Eight samples and three triggers.  The last four samples share a trigger, and the next three share another
#define FORMAT_9	10		// Eight samples and three triggers.  The last four samples share a trigger, and the next four share another

/// You set the format by changing the following value.  For example, to set to FORMAT_7,
/// you would change below to   #define FORMAT FORMAT_7

#define FORMAT FORMAT_9


/// THE FORMATS, and their associated CONFIGURATIONS, are described in detail below:

/// FORMAT_1
/// In this format there is a single sample, SAMPLE_1.  You can change its pitch and
/// start and end points with the pots.
	// POT 3		END 1			[If START > END, they are swapped]	[Set to MAN]
	// POT 2		START 1			[If START > END, they are swapped]	[Set to MAN]
	// POT 1		PITCH 1			[CENTER: Original Pitch]
	// GATE OUT		UNUSED
	// AUDIO IN		UNUSED
	// IN 3			TRIGGER 1
	// IN 2			UNUSED
	// IN 1			PITCH CV 1

/// FORMAT_2
/// There are two samples, SAMPLE_1 and SAMPLE_2.  You can change the pitch of each
/// one and the total volume with the pots  Set the volume such that when the two 
/// samples play simultaneously you don't have pops or clipping.
	// POT 3		VOLUME
	// POT 2		PITCH 2		[CENTER: Original Pitch]	[Note GRAINS BUG ABOVE]
	// POT 1		PITCH 1		[CENTER: Original Pitch]	[Note GRAINS BUG ABOVE]
	// GATE OUT		UNUSED
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 1
	// IN 2			PITCH CV 2
	// IN 1			PITCH CV 1 

/// FORMAT_3
/// There are three samples, SAMPLE_1 through SAMPLE_3.  You can change the pitch
/// of SAMPLE_1 and SAMPLE_2 with the pots, as well as the total volume.
/// Set the volume such that when the two samples play
/// simultaneously you don't have pops or clipping.
	// POT 3		VOLUME
	// POT 2		PITCH 2		[CENTER: Original Pitch]	[Note GRAINS BUG ABOVE]
	// POT 1		PITCH 1		[CENTER: Original Pitch]	[Note GRAINS BUG ABOVE]
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			PITCH CV 2
	// IN 1			PITCH CV 1 

/// FORMAT_4
/// There are four samples, SAMPLE_1 through SAMPLE_4.  You can change the pitch
/// of SAMPLE_1 and the total volume with the pots.
/// Set the volume such that when the two samples play
/// simultaneously you don't have pops or clipping.
	// POT 3		VOLUME
	// POT 2		UNUSED		[SET TO MAXIMUM, AND SET SWITCH TO IN2]
	// POT 1		PITCH 1		[CENTER: Original Pitch]	[Note GRAINS BUG ABOVE]
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			TRIGGER 4
	// IN 1			PITCH CV 1 

/// FORMAT_5
/// There are five samples, SAMPLE_1 through SAMPLE_5.  You can change the
/// total volume with a pot.
/// Set the volume such that when the two samples play
/// simultaneously you don't have pops or clipping.
	// POT 3		VOLUME
	// POT 2		UNUSED		[SET TO MAXIMUM, AND SET SWITCH TO IN2]
	// POT 1		UNUSED		[SET TO MAXIMUM, AND SET SWITCH TO IN1]
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			TRIGGER 4
	// IN 1			TRIGGER 5 

/// FORMAT_5A
/// There are five samples, SAMPLE_1 through SAMPLE_5.  You can change the
/// total volume with a pot.  Set the volume such that when the two samples play
/// simultaneously you don't have pops or clipping.
/// There are only FOUR triggers.  SAMPLE_4 and SAMPLE_5 share a trigger: a 
/// pot determines which sample plays.  The purpose of this format is to be
/// more convenient for people with 4-trigger sequencers such as TRIQ164 or
/// BEATS on the GRAINS.
	// POT 3		VOLUME
	// POT 2		UNUSED		[SET TO MAXIMUM, AND SET SWITCH TO IN2]
	// POT 1		SELECT SAMPLES FOR TRIGGER 4 -> { 4, 5 }	[SET TO MAN]
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			TRIGGER 4
	// IN 1			UNUSED 


/// FORMAT_6
/// There are six samples, SAMPLE_1 through SAMPLE_6.  You can change the
/// total volume with a pot.  Set the volume such that when the two samples play
/// simultaneously you don't have pops or clipping.
/// There are only FOUR triggers.  SAMPLE_4, SAMPLE_5, and SAMPLE_6 share a trigger: a 
/// pot determines which sample plays.  
	// POT 3		VOLUME
	// POT 2		UNUSED		[SET TO MAXIMUM, AND SET SWITCH TO IN2]
	// POT 1		SELECT SAMPLES FOR TRIGGER 4 -> { 4, 5, 6 }	[SET TO MAN]
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			TRIGGER 4
	// IN 1			UNUSED 

/// FORMAT_7
/// There are seven samples, SAMPLE_1 through SAMPLE_7.  You can change the
/// total volume with a pot.  Set the volume such that when the two samples play
/// simultaneously you don't have pops or clipping.
/// There are only FOUR triggers.  SAMPLE_4, SAMPLE_5, SAMPLE_6, and SAMPLE_7 
/// share a trigger: a pot determines which sample plays.  If you would like to
/// spread the sharing love over two triggers, try FORMAT_7A instead.
	// POT 3		VOLUME
	// POT 2		UNUSED		[SET TO MAXIMUM, AND SET SWITCH TO IN2]
	// POT 1		SELECT SAMPLES FOR TRIGGER 4 -> { 4, 5, 6, 7 }	[SET TO MAN]
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			TRIGGER 4
	// IN 1			UNUSED 

/// FORMAT_7A
/// There are seven samples, SAMPLE_1 through SAMPLE_7.  You can change the
/// total volume with a pot.  Set the volume such that when the two samples play
/// simultaneously you don't have pops or clipping.
/// There are only FOUR triggers.  SAMPLE_2, SAMPLE_3, SAMPLE_4 share a trigger,
/// and SAMPLE_5, SAMPLE_6, and SAMPLE_7 share a different trigger.  Two pots
/// determine which samples play on these triggers.  If you would like to
/// group more samples on one trigger and off the other, try FORMAT_7 instead.
	// POT 3		VOLUME
	// POT 2		SELECT SAMPLES FOR TRIGGER 2 -> { 2, 3, 4 }	[SET TO MAN]
	// POT 1		SELECT SAMPLES FOR TRIGGER 3 -> { 5, 6, 7 }	[SET TO MAN]
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			UNUSED
	// IN 1			UNUSED 

/// FORMAT_8
/// There are eight samples, SAMPLE_1 through SAMPLE_8.  You can change the
/// total volume with a pot.  Set the volume such that when the two samples play
/// simultaneously you don't have pops or clipping.
/// There are only FOUR triggers.  SAMPLE_2, SAMPLE_3, SAMPLE_4 share a trigger,
/// and SAMPLE_5, SAMPLE_6, SAMPLE_7, and SAMPLE_8 share a different trigger.  
/// Two pots determine which samples play on these triggers.
	// POT 3		VOLUME
	// POT 2		SELECT SAMPLES FOR TRIGGER 2 -> { 2, 3, 4 }		[SET TO MAN]
	// POT 1		SELECT SAMPLES FOR TRIGGER 3 -> { 5, 6, 7, 8 }	[SET TO MAN]
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			UNUSED
	// IN 1			UNUSED 

/// FORMAT_9
/// There are nine samples, SAMPLE_1 through SAMPLE_9.  You can change the
/// total volume with a pot.  Set the volume such that when the two samples play
/// simultaneously you don't have pops or clipping.
/// There are only FOUR triggers.  SAMPLE_2, SAMPLE_3, SAMPLE_4, and SAMPLE_5 
/// share a trigger, and SAMPLE_6, SAMPLE_7, SAMPLE_8, and SAMPLE_9 share a 
/// different trigger.  Two pots determine which samples play on these triggers.
	// POT 3		VOLUME
	// POT 2		SELECT SAMPLES FOR TRIGGER 2 -> { 2, 3, 4, 5 }	[SET TO MAN]
	// POT 1		SELECT SAMPLES FOR TRIGGER 3 -> { 6, 7, 8, 9 }	[SET TO MAN]
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			UNUSED
	// IN 1			UNUSED 




/// .... AND WE'RE DONE!









#define CONTROL_RATE 64

#include <MozziGuts.h>
#include <Sample.h>

#include SAMPLE_1
Sample<DATA_LENGTH, 16384> sample0(DATA);
uint16_t lengths0 = DATA_LENGTH;
#undef DATA
#undef DATA_LENGTH

#include SAMPLE_2
Sample<DATA_LENGTH, 16384> sample1(DATA);
uint16_t lengths1 = DATA_LENGTH;
#undef DATA
#undef DATA_LENGTH

#include SAMPLE_3
Sample<DATA_LENGTH, 16384> sample2(DATA);
uint16_t lengths2 = DATA_LENGTH;
#undef DATA
#undef DATA_LENGTH

#include SAMPLE_4
Sample<DATA_LENGTH, 16384> sample3(DATA);
uint16_t lengths3 = DATA_LENGTH;
#undef DATA
#undef DATA_LENGTH

#include SAMPLE_5
Sample<DATA_LENGTH, 16384> sample4(DATA);
uint16_t lengths4 = DATA_LENGTH;
#undef DATA
#undef DATA_LENGTH

#include SAMPLE_6
Sample<DATA_LENGTH, 16384> sample5(DATA);
uint16_t lengths5 = DATA_LENGTH;
#undef DATA
#undef DATA_LENGTH

#include SAMPLE_7
Sample<DATA_LENGTH, 16384> sample6(DATA);
uint16_t lengths6 = DATA_LENGTH;
#undef DATA
#undef DATA_LENGTH

#include SAMPLE_8
Sample<DATA_LENGTH, 16384> sample7(DATA);
uint16_t lengths7 = DATA_LENGTH;
#undef DATA
#undef DATA_LENGTH

#include SAMPLE_9
Sample<DATA_LENGTH, 16384> sample8(DATA);
uint16_t lengths8 = DATA_LENGTH;
#undef DATA
#undef DATA_LENGTH



#define CV_POT_IN1    A2 
#define CV_POT_IN2    A1
#define CV_POT3       A0
#define CV_IN3        A3
#define CV_AUDIO_IN   A4
#define CV_AUDIO_OUT  9
#define CV_GATE_OUT   8
#define RANDOM_PIN    A5

float inverseLengths0;
float inverseLengths1;

uint16_t triggered[5] = { 0, 0, 0, 0, 0 };
uint16_t trigger[5];
uint16_t pot[3];
#define UNDEFINED (-1)
int16_t previousPitch1 = UNDEFINED;
int16_t previousPitch2 = UNDEFINED;
int16_t previousStart = UNDEFINED;
int16_t previousEnd = UNDEFINED;
int16_t previousVolume = UNDEFINED;


const int16_t ULAW[] = { -244, -234, -224, -215, -206, -197, -188, -180, -173, -165, -158,
-152, -145, -139, -133, -127, -122, -117, -112, -107, -103, -98, -94,
-90, -86, -82, -79, -76, -72, -69, -66, -63, -61, -58, -56, -53, -51,
-49, -47, -45, -43, -41, -39, -38, -36, -34, -33, -32, -30, -29, -28,
-26, -25, -24, -23, -22, -21, -20, -19, -19, -18, -17, -16, -16, -15,
-14, -14, -13, -12, -12, -11, -11, -10, -10, -9, -9, -9, -8, -8, -8,
-7, -7, -7, -6, -6, -6, -5, -5, -5, -5, -5, -4, -4, -4, -4, -4, -3,
-3, -3, -3, -3, -3, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1,
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7,
7, 7, 8, 8, 8, 9, 9, 10, 10, 11, 11, 12, 13, 13, 14, 15, 15, 16, 17,
18, 18, 19, 20, 21, 22, 23, 24, 25, 27, 28, 29, 31, 32, 33, 35, 37,
38, 40, 42, 44, 46, 48, 50, 52, 55, 57, 60, 62, 65, 68, 71, 75, 78,
81, 85, 89, 93, 97, 102, 106, 111, 116, 121, 126, 132, 138, 144, 151,
157, 164, 172, 179, 187, 196, 205, 214, 223, 233 };

#ifdef USE_ULAW
int16_t C(int16_t value, uint8_t volume) { return (ULAW[(uint8_t)(value + 128)] * volume) >> 4; }
//#define C(value, volume) ((ULAW[(uint8_t)(value + 128)] * volume) >> 4)
#else
int16_t C(int16_t value, uint8_t volume) { return (value * volume) >> 4; }
//#define C(value, volume) ((value * volume) >> 4)
#endif


// I want a lookup table from 0...1024 which maps to 1/2 ... 2 exponentially.
// I don't have the memory for it in floats though, nor 1024 elements.  So
// we take 0...1024 and divide by 4 to get 0...255.  Then we look it up in FREQUENCIES.
// Finally we divide by 128.
const uint8_t FREQUENCIES[] = {64, 64, 64, 65, 65, 65, 66, 66, 66, 67, 67, 67, 68, 68, 69, 69, 69,
						70, 70, 70, 71, 71, 72, 72, 72, 73, 73, 74, 74, 74, 75, 75, 76, 76,
						76, 77, 77, 78, 78, 79, 79, 79, 80, 80, 81, 81, 82, 82, 82, 83, 83,
						84, 84, 85, 85, 86, 86, 87, 87, 88, 88, 89, 89, 90, 90, 91, 91, 91,
						92, 92, 93, 94, 94, 95, 95, 96, 96, 97, 97, 98, 98, 99, 99, 100, 100,
						101, 101, 102, 103, 103, 104, 104, 105, 105, 106, 107, 107, 108, 108,
						109, 109, 110, 111, 111, 112, 113, 113, 114, 114, 115, 116, 116, 117,
						118, 118, 119, 119, 120, 121, 121, 122, 123, 123, 124, 125, 125, 126,
						127, 128, 128, 129, 130, 130, 131, 132, 132, 133, 134, 135, 135, 136,
						137, 138, 138, 139, 140, 141, 141, 142, 143, 144, 144, 145, 146, 147,
						148, 148, 149, 150, 151, 152, 153, 153, 154, 155, 156, 157, 158, 158,
						159, 160, 161, 162, 163, 164, 165, 165, 166, 167, 168, 169, 170, 171,
						172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 182, 183, 184,
						185, 186, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
						200, 201, 202, 203, 205, 206, 207, 208, 209, 210, 211, 212, 214, 215,
						216, 217, 218, 219, 221, 222, 223, 224, 226, 227, 228, 229, 230, 232,
						233, 234, 236, 237, 238, 239, 241, 242, 243, 245, 246, 247, 249, 250,
						251, 253, 254};

float computeFrequency(uint16_t pitch, float inverseLength)
	{
	float freq = 1.0;		// dead center
	if (pitch >= 512 + 128)
		{
		 freq = FREQUENCIES[(pitch - 128) >> 2] * (1.0 / 128.0);
   		}
	else // if (pitch >= 512 + 128)
		{
		freq = FREQUENCIES[(pitch + 128) >> 2] * (1.0 / 128.0);
   		}
   	return 16384 * inverseLength * freq;
	}

void updateControl1()
	{
	// POT 3		END 1
	// POT 2		START 1
	// POT 1		PITCH 1
	// GATE OUT		TRIGGER 1
	// AUDIO IN		UNUSED
	// IN 3			UNUSED
	// IN 2			UNUSED
	// IN 1			PITCH CV 1
	
	if (previousPitch1 == UNDEFINED) previousPitch1 = pot[0] = mozziAnalogRead(CV_POT3);
	else pot[0] = previousPitch1 = (previousPitch1 * 3 + mozziAnalogRead(CV_POT3)) >> 2;
	if (previousStart == UNDEFINED) previousStart = pot[1] = mozziAnalogRead(CV_POT_IN2);
	else pot[1] = previousStart = (previousStart * 3 + mozziAnalogRead(CV_POT_IN2)) >> 2;
	if (previousEnd == UNDEFINED) previousEnd = pot[2] = mozziAnalogRead(CV_POT_IN1);
	else pot[2] = previousEnd = (previousEnd * 3 + mozziAnalogRead(CV_POT_IN1)) >> 2;

	uint8_t val = digitalRead(CV_GATE_OUT);
	if (!triggered[0] && val) 
		{ 
		// set sample lengths and pitch only when we start
		sample0.setFreq(computeFrequency(pot[0], inverseLengths0));
		if (pot[2] < pot[1]) // start is after end, need to swap
			{
			uint8_t p = pot[2];
			pot[2] = pot[1];
			pot[1] = p;
			}
			
		sample0.setStart((pot[1] * (uint32_t)lengths0) >> 10);
		sample0.setEnd((pot[2] * (uint32_t)lengths0) >> 10);
		triggered[0] = 1;
		sample0.start(); 
		}
	triggered[0] = val;
	}
	
void updateControl2()
	{
	// POT 3		VOLUME
	// POT 2		PITCH 2
	// POT 1		PITCH 1
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			UNUSED
	// IN 2			PITCH CV 2
	// IN 1			PITCH CV 1 

	if (previousPitch1 == UNDEFINED) previousPitch1 = pot[0] = mozziAnalogRead(CV_POT_IN1);
	else pot[0] = previousPitch1 = (previousPitch1 * 3 + mozziAnalogRead(CV_POT_IN1)) >> 2;
	if (previousPitch2 == UNDEFINED) previousPitch2 = pot[1] = mozziAnalogRead(CV_POT_IN2);
	else pot[1] = previousPitch2 = (previousPitch2 * 3 + mozziAnalogRead(CV_POT_IN2)) >> 2;
	if (previousVolume == UNDEFINED) previousVolume = pot[2] = mozziAnalogRead(CV_POT3);
	else pot[2] = previousVolume = (previousVolume * 3 + mozziAnalogRead(CV_POT3)) >> 2;
	
	uint8_t val = digitalRead(CV_GATE_OUT);
	if (!triggered[0] && val) 
		{ 
		// set pitch only when we start
		sample0.setFreq(computeFrequency(pot[0], inverseLengths0));
		triggered[0] = 1;
		sample0.start(); 
		}
	triggered[0] = val;
	
	uint16_t val1 = mozziAnalogRead(CV_AUDIO_IN);
	if (!triggered[1] && val1 > 800) 
		{ 
		// set pitch only when we start
		sample1.setFreq(computeFrequency(pot[1], inverseLengths1));
		triggered[1] = 1; 
		sample1.start(); 
		}
	else if (triggered[1] && val1 < 400) { triggered[1] = 0; }
	}

void updateControl3()
	{
	// POT 3		VOLUME
	// POT 2		PITCH 2
	// POT 1		PITCH 1
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			PITCH CV 2
	// IN 1			PITCH CV 1 

	if (previousPitch1 == UNDEFINED) previousPitch1 = pot[0] = mozziAnalogRead(CV_POT_IN1);
	else pot[0] = previousPitch1 = (previousPitch1 * 3 + mozziAnalogRead(CV_POT_IN1)) >> 2;
	if (previousPitch2 == UNDEFINED) previousPitch2 = pot[1] = mozziAnalogRead(CV_POT_IN2);
	else pot[1] = previousPitch2 = (previousPitch2 * 3 + mozziAnalogRead(CV_POT_IN2)) >> 2;
	if (previousVolume == UNDEFINED) previousVolume = pot[2] = mozziAnalogRead(CV_POT3);
	else pot[2] = previousVolume = (previousVolume * 3 + mozziAnalogRead(CV_POT3)) >> 2;
	
	uint8_t val = digitalRead(CV_GATE_OUT);
	if (!triggered[0] && val) 
		{ 
		// set pitch only when we start
		sample0.setFreq(computeFrequency(pot[0], inverseLengths0));
		triggered[0] = 1;
		sample0.start(); 
		}
	triggered[0] = val;
	
	uint16_t val1 = mozziAnalogRead(CV_AUDIO_IN);
	if (!triggered[1] && val1 > 800) 
		{ 
		// set pitch only when we start
		sample1.setFreq(computeFrequency(pot[1], inverseLengths1));
		triggered[1] = 1;
		sample1.start(); 
		}
	else if (triggered[1] && val1 < 400) { triggered[1] = 0; }
	
	uint16_t val2 = mozziAnalogRead(CV_IN3);
	if (!triggered[2] && val2 > 800) { triggered[2] = 1; sample2.start(); }
	else if (triggered[2] && val2 < 400) { triggered[2] = 0; }
	}

void updateControl4()
	{
	// POT 3		VOLUME
	// POT 2		UNUSED
	// POT 1		PITCH 1
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			TRIGGER 4
	// IN 1			PITCH CV 1 
	
	if (previousPitch1 == UNDEFINED) previousPitch1 = pot[0] = mozziAnalogRead(CV_POT_IN1);
	else pot[0] = previousPitch1 = (previousPitch1 * 3 + mozziAnalogRead(CV_POT_IN1)) >> 2;
	/// POT[1] USED FOR TRIGGER
	if (previousVolume == UNDEFINED) previousVolume = pot[2] = mozziAnalogRead(CV_POT3);
	else pot[2] = previousVolume = (previousVolume * 3 + mozziAnalogRead(CV_POT3)) >> 2;
	
	uint8_t val = digitalRead(CV_GATE_OUT);
	if (!triggered[0] && val) 
		{ 
		// set pitch only when we start
		sample0.setFreq(computeFrequency(pot[0], inverseLengths0));
		triggered[0] = 1;
		sample0.start(); 
		}
	triggered[0] = val;
	
	uint16_t val1 = mozziAnalogRead(CV_AUDIO_IN);
	if (!triggered[1] && val1 > 800) { triggered[1] = 1; sample1.start(); }
	else if (triggered[1] && val1 < 400) { triggered[1] = 0; }

	uint16_t val2 = mozziAnalogRead(CV_IN3);
	if (!triggered[2] && val2 > 800) { triggered[2] = 1; sample2.start(); }
	else if (triggered[2] && val2 < 400) { triggered[2] = 0; }

	uint16_t val3 = mozziAnalogRead(CV_POT_IN2);
	if (!triggered[3] && val3 > 800) { triggered[3] = 1; sample3.start(); }
	else if (triggered[3] && val3 < 400) { triggered[3] = 0; }
	
	}

void updateControl5()
	{
	// POT 3		VOLUME
	// POT 2		UNUSED
	// POT 1		UNUSED
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			TRIGGER 4
	// IN 1			TRIGGER 5 

	/// POT[0] USED FOR TRIGGER
	/// POT[1] USED FOR TRIGGER
	if (previousVolume == UNDEFINED) previousVolume = pot[2] = mozziAnalogRead(CV_POT3);
	else pot[2] = previousVolume = (previousVolume * 3 + mozziAnalogRead(CV_POT3)) >> 2;
	
	uint8_t val = digitalRead(CV_GATE_OUT);
	if (!triggered[0] && val) { sample0.start(); }
	triggered[0] = val;
	
	uint16_t val1 = mozziAnalogRead(CV_AUDIO_IN);
	if (!triggered[1] && val1 > 800) { triggered[1] = 1; sample1.start(); }
	else if (triggered[1] && val1 < 400) { triggered[1] = 0; }

	uint16_t val2 = mozziAnalogRead(CV_IN3);
	if (!triggered[2] && val2 > 800) { triggered[2] = 1; sample2.start(); }
	else if (triggered[2] && val2 < 400) { triggered[2] = 0; }

	uint16_t val3 = mozziAnalogRead(CV_POT_IN2);
	if (!triggered[3] && val3 > 800) { triggered[3] = 1; sample3.start(); }
	else if (triggered[3] && val3 < 400) { triggered[3] = 0; }

	uint16_t val4 = mozziAnalogRead(CV_POT_IN1);
	if (!triggered[4] && val4 > 800) { triggered[4] = 1; sample4.start(); }
	else if (triggered[4] && val4 < 400) { triggered[4] = 0; }
	}

void updateControl5A()
	{
	// POT 3		VOLUME
	// POT 2		UNUSED
	// POT 1		SAMPLES 4 -> { 4, 5 }
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			TRIGGER 4
	// IN 1			UNUSED 
	
	pot[0] = mozziAnalogRead(CV_POT_IN1);
	/// POT[1] USED FOR TRIGGER
	if (previousVolume == UNDEFINED) previousVolume = pot[2] = mozziAnalogRead(CV_POT3);
	else pot[2] = previousVolume = (previousVolume * 3 + mozziAnalogRead(CV_POT3)) >> 2;
	
	uint8_t val = digitalRead(CV_GATE_OUT);
	if (!triggered[0] && val) { sample0.start(); }
	triggered[0] = val;
	
	uint16_t val1 = mozziAnalogRead(CV_AUDIO_IN);
	if (!triggered[1] && val1 > 800) { triggered[1] = 1; sample1.start(); }
	else if (triggered[1] && val1 < 400) { triggered[1] = 0; }

	uint16_t val2 = mozziAnalogRead(CV_IN3);
	if (!triggered[2] && val2 > 800) { triggered[2] = 1; sample2.start(); }
	else if (triggered[2] && val2 < 400) { triggered[2] = 0; }

	uint16_t val3 = mozziAnalogRead(CV_POT_IN2);
	if (!triggered[3] && val3 > 800) { triggered[3] = 1; if (pot[0] < 512) sample3.start(); else sample4.start(); }
	else if (triggered[3] && val3 < 400) { triggered[3] = 0; }
	}

void updateControl6()
	{
	// POT 3		VOLUME
	// POT 2		UNUSED
	// POT 1		SAMPLES 4 -> { 4, 5, 6 }
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			TRIGGER 4
	// IN 1			UNUSED 

	pot[0] = mozziAnalogRead(CV_POT_IN1);
	/// POT[1] USED FOR TRIGGER
	if (previousVolume == UNDEFINED) previousVolume = pot[2] = mozziAnalogRead(CV_POT3);
	else pot[2] = previousVolume = (previousVolume * 3 + mozziAnalogRead(CV_POT3)) >> 2;
	
	uint8_t val = digitalRead(CV_GATE_OUT);
	if (!triggered[0] && val) { sample0.start(); }
	triggered[0] = val;
	
	uint16_t val1 = mozziAnalogRead(CV_AUDIO_IN);
	if (!triggered[1] && val1 > 800) { triggered[1] = 1; sample1.start(); }
	else if (triggered[1] && val1 < 400) { triggered[1] = 0; }

	uint16_t val2 = mozziAnalogRead(CV_IN3);
	if (!triggered[2] && val2 > 800) { triggered[2] = 1; sample2.start(); }
	else if (triggered[2] && val2 < 400) { triggered[2] = 0; }

	uint16_t val3 = mozziAnalogRead(CV_POT_IN2);
	if (!triggered[3] && val3 > 800) { triggered[3] = 1; if (pot[0] < 341) sample3.start(); else if (pot[0] < 682) sample4.start(); else sample5.start(); }
	else if (triggered[3] && val3 < 400) { triggered[3] = 0; }
	}

void updateControl7()
	{
	// POT 3		VOLUME
	// POT 2		UNUSED
	// POT 1		SAMPLES 4 -> { 4, 5, 6, 7 }
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			TRIGGER 4
	// IN 1			UNUSED 

	pot[0] = mozziAnalogRead(CV_POT_IN1);
	/// POT[1] USED FOR TRIGGER
	if (previousVolume == UNDEFINED) previousVolume = pot[2] = mozziAnalogRead(CV_POT3);
	else pot[2] = previousVolume = (previousVolume * 3 + mozziAnalogRead(CV_POT3)) >> 2;
	
	uint8_t val = digitalRead(CV_GATE_OUT);
	if (!triggered[0] && val) { sample0.start(); }
	triggered[0] = val;
	
	uint16_t val1 = mozziAnalogRead(CV_AUDIO_IN);
	if (!triggered[1] && val1 > 800) { triggered[1] = 1; sample1.start(); }
	else if (triggered[1] && val1 < 400) { triggered[1] = 0; }

	uint16_t val2 = mozziAnalogRead(CV_IN3);
	if (!triggered[2] && val2 > 800) { triggered[2] = 1; sample2.start(); }
	else if (triggered[2] && val2 < 400) { triggered[2] = 0; }

	uint16_t val3 = mozziAnalogRead(CV_POT_IN2);
	if (!triggered[3] && val3 > 800) { triggered[3] = 1; if (pot[0] < 256) sample3.start(); else if (pot[0] < 512) sample4.start(); else if (pot[0] < 768) sample5.start(); else sample6.start(); }
	else if (triggered[3] && val3 < 400) { triggered[3] = 0; }
	}

void updateControl7A()
	{
	// POT 3		VOLUME
	// POT 2		SAMPLES 2 -> { 2, 3, 4 }
	// POT 1		SAMPLES 3 -> { 5, 6, 7 }
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			UNUSED
	// IN 1			UNUSED 

	pot[0] = mozziAnalogRead(CV_POT_IN1);
	pot[1] = mozziAnalogRead(CV_POT_IN2);
	if (previousVolume == UNDEFINED) previousVolume = pot[2] = mozziAnalogRead(CV_POT3);
	else pot[2] = previousVolume = (previousVolume * 3 + mozziAnalogRead(CV_POT3)) >> 2;

	Serial.println(pot[0]);	
	uint8_t val = digitalRead(CV_GATE_OUT);
	if (!triggered[0] && val) { sample0.start(); }
	triggered[0] = val;
	
	uint16_t val1 = mozziAnalogRead(CV_AUDIO_IN);
	if (!triggered[1] && val1 > 800) { triggered[1] = 1; if (pot[1] < 341) sample1.start(); else if (pot[1] < 682) sample2.start(); else sample3.start(); }
	else if (triggered[1] && val1 < 400) { triggered[1] = 0; }

	uint16_t val2 = mozziAnalogRead(CV_IN3);
	if (!triggered[2] && val2 > 800) { triggered[2] = 1; if (pot[0] < 341) sample4.start(); else if (pot[0] < 682) sample5.start(); else sample6.start(); }
	else if (triggered[2] && val2 < 400) { triggered[2] = 0; }
	}

void updateControl8()
	{
	// POT 3		VOLUME
	// POT 2		SAMPLES 2 -> { 2, 3, 4 }
	// POT 1		SAMPLES 3 -> { 5, 6, 7, 8 }
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			UNUSED
	// IN 1			UNUSED 

	pot[0] = mozziAnalogRead(CV_POT_IN1);
	pot[1] = mozziAnalogRead(CV_POT_IN2);
	if (previousVolume == UNDEFINED) previousVolume = pot[2] = mozziAnalogRead(CV_POT3);
	else pot[2] = previousVolume = (previousVolume * 3 + mozziAnalogRead(CV_POT3)) >> 2;
	
	uint8_t val = digitalRead(CV_GATE_OUT);
	if (!triggered[0] && val) { sample0.start(); }
	triggered[0] = val;
	
	uint16_t val1 = mozziAnalogRead(CV_AUDIO_IN);
	if (!triggered[1] && val1 > 800) { triggered[1] = 1; if (pot[1] < 341) sample1.start(); else if (pot[1] < 682) sample2.start(); else sample3.start(); }
	else if (triggered[1] && val1 < 400) { triggered[1] = 0; }

	uint16_t val2 = mozziAnalogRead(CV_IN3);
	if (!triggered[2] && val2 > 800) { triggered[2] = 1; if (pot[0] < 256) sample4.start(); else if (pot[0] < 512) sample5.start(); else if (pot[0] < 768) sample6.start(); else sample7.start(); }
	else if (triggered[2] && val2 < 400) { triggered[2] = 0; }
	}

void updateControl9()
	{
	// POT 3		VOLUME
	// POT 2		SAMPLES 2 -> { 2, 3, 4, 5 }
	// POT 1		SAMPLES 3 -> { 6, 7, 8, 9 }
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			UNUSED
	// IN 1			UNUSED 

	pot[0] = mozziAnalogRead(CV_POT_IN1);
	pot[1] = mozziAnalogRead(CV_POT_IN2);
	if (previousVolume == UNDEFINED) previousVolume = pot[2] = mozziAnalogRead(CV_POT3);
	else pot[2] = previousVolume = (previousVolume * 3 + mozziAnalogRead(CV_POT3)) >> 2;
	
	uint8_t val = digitalRead(CV_GATE_OUT);
	if (!triggered[0] && val) { sample0.start(); }
	triggered[0] = val;
	
	uint16_t val1 = mozziAnalogRead(CV_AUDIO_IN);
	if (!triggered[1] && val1 > 800) { triggered[1] = 1; if (pot[1] < 256) sample1.start(); else if (pot[1] < 512) sample2.start(); else if (pot[1] < 768) sample3.start(); else sample4.start(); }
	else if (triggered[1] && val1 < 400) { triggered[1] = 0; }

	uint16_t val2 = mozziAnalogRead(CV_IN3);
	if (!triggered[2] && val2 > 800) { triggered[2] = 1; if (pot[0] < 256) sample5.start(); else if (pot[0] < 512) sample6.start(); else if (pot[0] < 768) sample7.start(); else sample8.start(); }
	else if (triggered[2] && val2 < 400) { triggered[2] = 0; }
	}





int updateAudio1()
	{
	// POT 3		END 1
	// POT 2		START 1
	// POT 1		PITCH 1
	// GATE OUT		TRIGGER 1
	// AUDIO IN		UNUSED
	// IN 3			UNUSED
	// IN 2			UNUSED
	// IN 1			PITCH CV 1
	
#ifdef USE_ULAW
	return ULAW[(uint8_t)(sample0.next() + 128)];
#else
	return (sample0.next() * 244) >> 7;		// stretch to 488
#endif
	}
	
int updateAudio2()
	{
	// POT 3		VOLUME
	// POT 2		PITCH 2
	// POT 1		PITCH 1
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			UNUSED
	// IN 2			PITCH CV 2
	// IN 1			PITCH CV 1 

	return (int16_t)(((C(sample0.next(), GAIN_0) + C(sample1.next(), GAIN_1)) * (int32_t)pot[2]) >> 9);
	}

int updateAudio3()
	{
	// POT 3		VOLUME
	// POT 2		PITCH 2
	// POT 1		PITCH 1
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			PITCH CV 2
	// IN 1			PITCH CV 1 

	return (int16_t)(((C(sample0.next(), GAIN_0) + C(sample1.next(), GAIN_1) + C(sample2.next(), GAIN_2)) * (int32_t)pot[2]) >> 9);
	}

int updateAudio4()
	{
	// POT 3		VOLUME
	// POT 2		UNUSED
	// POT 1		PITCH 1
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			TRIGGER 4
	// IN 1			PITCH CV 1 

	return (int16_t)(((C(sample0.next(), GAIN_0) + C(sample1.next(), GAIN_1) + C(sample2.next(), GAIN_2) + C(sample3.next(), GAIN_3)) * (int32_t)pot[2]) >> 9);
	}

int updateAudio5()
	{
	// POT 3		VOLUME
	// POT 2		UNUSED
	// POT 1		UNUSED
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			TRIGGER 4
	// IN 1			TRIGGER 5 

	return (int16_t)(((C(sample0.next(), GAIN_0) + C(sample1.next(), GAIN_1) + C(sample2.next(), GAIN_2) + C(sample3.next(), GAIN_3) + C(sample4.next(), GAIN_4)) * (int32_t)pot[2]) >> 9);
	}

int updateAudio5A()
	{
	// POT 3		VOLUME
	// POT 2		UNUSED
	// POT 1		SAMPLES 4 -> { 4, 5 }
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			TRIGGER 4
	// IN 1			UNUSED 

	return (int16_t)(((C(sample0.next(), GAIN_0) + C(sample1.next(), GAIN_1) + C(sample2.next(), GAIN_2) + (pot[0] < 512 ? C(sample3.next(), GAIN_3) : C(sample4.next(), GAIN_4))) * (int32_t)pot[2]) >> 9);
	}

int updateAudio6()
	{
	// POT 3		VOLUME
	// POT 2		UNUSED
	// POT 1		SAMPLES 4 -> { 4, 5, 6 }
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			TRIGGER 4
	// IN 1			UNUSED 

	return (int16_t)(((C(sample0.next(), GAIN_0) + C(sample1.next(), GAIN_1) + C(sample2.next(), GAIN_2) + (pot[0] < 341 ? C(sample3.next(), GAIN_3) : (pot[0] < 682 ? C(sample4.next(), GAIN_4) : C(sample5.next(), GAIN_5)))) * (int32_t)pot[2]) >> 9);
	}

int updateAudio7()
	{
	// POT 3		VOLUME
	// POT 2		UNUSED
	// POT 1		SAMPLES 4 -> { 4, 5, 6, 7}
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			TRIGGER 4
	// IN 1			UNUSED 

	return (int16_t)(((C(sample0.next(), GAIN_0) + C(sample1.next(), GAIN_1) + C(sample2.next(), GAIN_2) + (pot[0] < 256 ? C(sample3.next(), GAIN_3) : (pot[0] < 512 ? C(sample4.next(), GAIN_4) : (pot[0] < 768 ? C(sample5.next(), GAIN_5) : C(sample6.next(), GAIN_6))))) * (int32_t)pot[2]) >> 9);
	}

int updateAudio7A()
	{
	// POT 3		VOLUME
	// POT 2		SAMPLES 2 -> { 2, 3, 4 }
	// POT 1		SAMPLES 3 -> { 5, 6, 7 }
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			UNUSED
	// IN 1			UNUSED 

	return (int16_t)(((C(sample0.next(), GAIN_0) + (pot[1] < 341 ? C(sample1.next(), GAIN_1) : (pot[1] < 682 ? C(sample2.next(), GAIN_2) : C(sample3.next(), GAIN_3))) + (pot[0] < 341 ? C(sample4.next(), GAIN_4) : (pot[0] < 682 ? C(sample5.next(), GAIN_5) : C(sample6.next(), GAIN_6)))) * (int32_t)pot[2]) >> 9);
	}

int updateAudio8()
	{
	// POT 3		VOLUME
	// POT 2		SAMPLES 2 -> { 2, 3, 4 }
	// POT 1		SAMPLES 3 -> { 5, 6, 7, 8 }
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			UNUSED
	// IN 1			UNUSED 
	
	return (int16_t)(((C(sample0.next(), GAIN_0) + (pot[1] < 341 ? C(sample1.next(), GAIN_1) : (pot[1] < 682 ? C(sample2.next(), GAIN_2) : C(sample3.next(), GAIN_3))) + (pot[0] < 256 ? C(sample4.next(), GAIN_4) : (pot[0] < 512 ? C(sample5.next(), GAIN_5) : (pot[0] < 768 ? C(sample6.next(), GAIN_6) : C(sample7.next(), GAIN_7))))) * (int32_t)pot[2]) >> 9);
	}

int updateAudio9()
	{
	// POT 3		VOLUME
	// POT 2		SAMPLES 2 -> { 2, 3, 4, 5 }
	// POT 1		SAMPLES 3 -> { 6, 7, 8, 9 }
	// GATE OUT		TRIGGER 1
	// AUDIO IN		TRIGGER 2
	// IN 3			TRIGGER 3
	// IN 2			UNUSED
	// IN 1			UNUSED 
	
	return (int16_t)(((C(sample0.next(), GAIN_0) + (pot[1] < 256 ? C(sample1.next(), GAIN_1) : (pot[1] < 512 ? C(sample2.next(), GAIN_2) : (pot[1] < 768 ? C(sample3.next(), GAIN_3) : C(sample4.next(), GAIN_4)))) + (pot[0] < 256 ? C(sample5.next(), GAIN_5) : (pot[0] < 512 ? C(sample6.next(), GAIN_6) : (pot[0] < 768 ? C(sample7.next(), 7) : C(sample8.next(), 8))))) * (int32_t)pot[2]) >> 9);
	}





void updateControl()
	{
#if (FORMAT == FORMAT_1)
	updateControl1();
#elif (FORMAT == FORMAT_2)
	updateControl2();
#elif (FORMAT == FORMAT_3)
	updateControl3();
#elif (FORMAT == FORMAT_4)
	updateControl4();
#elif (FORMAT == FORMAT_5)
	updateControl5();
#elif (FORMAT == FORMAT_5A)
	updateControl5A();
#elif (FORMAT == FORMAT_6)
	updateControl6();
#elif (FORMAT == FORMAT_7)
	updateControl7();
#elif (FORMAT == FORMAT_7A)
	updateControl7A();
#elif (FORMAT == FORMAT_8)
	updateControl8();
#elif (FORMAT == FORMAT_9)
	updateControl9();
#endif
	}

int updateAudio()
	{
#if (FORMAT == FORMAT_1)
	return updateAudio1();
#elif (FORMAT == FORMAT_2)
	return updateAudio2();
#elif (FORMAT == FORMAT_3)
	return updateAudio3();
#elif (FORMAT == FORMAT_4)
	return updateAudio4();
#elif (FORMAT == FORMAT_5)
	return updateAudio5();
#elif (FORMAT == FORMAT_5A)
	return updateAudio5A();
#elif (FORMAT == FORMAT_6)
	return updateAudio6();
#elif (FORMAT == FORMAT_7)
	return updateAudio7();
#elif (FORMAT == FORMAT_7A)
	return updateAudio7A();
#elif (FORMAT == FORMAT_8)
	return updateAudio8();
#elif (FORMAT == FORMAT_9)
	return updateAudio9();
#endif
	}
	
void setup()
	{
	pinMode(CV_GATE_OUT, INPUT);

	inverseLengths0 = 1.0 / lengths0;
	inverseLengths1 = 1.0 / lengths1;

	sample0.setFreq(16384 / (float) lengths0);
	sample1.setFreq(16384 / (float) lengths1);
	sample2.setFreq(16384 / (float) lengths2);
	sample3.setFreq(16384 / (float) lengths3);
	sample4.setFreq(16384 / (float) lengths4);
	sample5.setFreq(16384 / (float) lengths5);
	sample6.setFreq(16384 / (float) lengths6);
	sample7.setFreq(16384 / (float) lengths7);
	sample8.setFreq(16384 / (float) lengths8);
	startMozzi();

	Serial.begin(9600);
	}

void loop()
	{
    audioHook();
	}
