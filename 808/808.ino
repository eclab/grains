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
/// FORMATS differ in capability based on the number of samples involved so as to maximize 
/// GRAINS's limited interface.  FORMAT_1 has only one sample but lets you specify the start 
/// and end sample points, and the pitch.  FORMAT_2 has two samples but you can specify their 
/// pitch; similarly for FORMAT_3 and FORMAT_4.  FORMAT_5 has up to 5 triggers.  
/// FORMATS 6, 7, 7A, 8, and 9 have increasing numbers of samples, but they are crammed into 
/// selectable groups with smaller and smaller numbers of triggers.  
///
/// A note about pitch: Mozzi's sampler facility is very primitive, and changes in pitch 
/// will have a lot of aliasing and other artifacts.
///
/// MEMORY ALLOCATION
/// 808 provides about 26000 bytes of memory for your samples.  The total byte length of
/// your samples cannot exceed this amount.
///
///
/// COMPUTATIONAL POWER
/// As you increase the number of triggers (to say, 4 or 5), Mozzi will start to struggle
/// to keep up.  You might occasionally get a little click.  You can reduce the clicks by
/// reducing the CONTROL_RATE from 64 to something less, but the drum machine will start
/// to get sloppy in its response to triggers.  It's already really sloppy as it is: 64 means
/// that it checks for triggers only 64 times a second, or every 15 ms.
///
/// Small clicks also occur when the samples are too long and must be reset, especially on
/// top of one another.  I notice this with snare drums + bass drums together, running too
/// fast.
///
/// I have also noticed that occasionally one sample will start slightly before other
/// samples played at the same time.  I do not know if this is a Mozzi bug or a bug in the
/// TRIQ164 sequencer I'm using.  If most of your samples are played on different beats,
/// you will probably never even notice this.
/// 
///
/// A NOTE ABOUT THE DIGITAL OUT PIN
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


#define SAMPLE_1        "tr808/BD7500.h" // "tr808/BD7525.h"                
#define SAMPLE_2        "tr808/SD0050.h"
#define SAMPLE_3        "tr808/CP.h"	 	
#define SAMPLE_4        "tr808/CH.h"
#define SAMPLE_5        "tr808/OH00.h"
#define SAMPLE_6        "tr808/CB.h"
#define SAMPLE_7        "tr808/HC00.h"		
#define SAMPLE_8        "tr808/RS.h"
#define SAMPLE_9        "tr808/MA.h"

/// Here is an example with all empty samples, showing use of all the "blank" files.
/// You probably don't want this!  :-)

///             #define SAMPLE_1        "blank1.h"
///             #define SAMPLE_2        "blank2.h"
///             #define SAMPLE_3        "blank3.h"
///             #define SAMPLE_4        "blank4.h"
///             #define SAMPLE_5        "blank5.h"
///             #define SAMPLE_6        "blank6.h"
///             #define SAMPLE_7        "blank7.h"
///             #define SAMPLE_8        "blank8.h"
///             #define SAMPLE_9        "blank9.h"


/// U-LAW SAMPLES
///
/// Your samples can be ALL 8-bit linear, or they can ALL be 8-bit uLaw.  uLaw sounds a lot better.
/// The samples in the tr808 directory are a
/// If they are
/// uLaw, you need to uncomment the following #define, that is, remove the //

#define USE_ULAW

// Note that when using U-Law, 808 will make the samples sound louder, so you'll need
// to turn the volume pot down.  The U-Law code also very slightly reduces the available
// memory for your samples in GRAINS.


/// ADJUSTING INDIVIDUAL SAMPLE VOLUME
///
/// Is a sample too loud compared to the others?  
/// You can adjust the relative gain of each sample by reducing any of the following values.
/// The values go between 0 (fully muted) to 8 (standard volume).  You can go higher but risk clipping.
/// NOTE: this feature is turned off for FORMAT_5, because with 5 triggers, the computational
/// cost is just too high.
///
/// NOTE 2: this feature is particularly useful for FORMAT_1, which has a fixed volume.  For many sounds
/// you might get away with a slightly larger gain (perhaps 9? 10?) for GAIN_0.

#define GAIN_0          8
#define GAIN_1          8
#define GAIN_2          8
#define GAIN_3          8
#define GAIN_4          8
#define GAIN_5          8
#define GAIN_6          8
#define GAIN_7          8
#define GAIN_8          8
#define GAIN_9          8


/// SETTING THE FORMAT
/// There are 11 formats, and they dictate the number of samples used, how they're
/// grouped, and how the interface is presented.   Here are the format names, don't
/// play with this:

#define FORMAT_1        0               // A single sample, plus editable start/end points and pitch control
#define FORMAT_2        1               // Two samples, each with pitch control, plus total volume control
#define FORMAT_3        2               // Three samples, the first two with pitch control, plus total volume control
#define FORMAT_4        3               // Four samples, the first one with pitch control, plus total volume control
#define FORMAT_5        4               // Has 5 triggers: thus not for TRIQ164 or GRAINS BEATS
#define FORMAT_5A       5               // An alternative to FORMAT_5 for people with 4-trigger step sequencers like TRIQ164 or GRAINS BEATS 
#define FORMAT_6        6               // Six samples and four triggers: the last three samples share a trigger
#define FORMAT_7        7               // Seven samples and four triggers: the last four samples share a trigger
#define FORMAT_7A       8               // An alternative to FORMAT_7, with three triggers.  The last three samples share a trigger, and the next three share another
#define FORMAT_8        9               // Eight samples and three triggers.  The last four samples share a trigger, and the next three share another
#define FORMAT_9        10              // Eight samples and three triggers.  The last four samples share a trigger, and the next four share another

/// You set the format by changing the following value.  For example, to set to FORMAT_7,
/// you would change below to   #define FORMAT FORMAT_7

#define FORMAT FORMAT_4


/// The CONTROL_RATE value below indicates how often per second Mozzi checks for incoming
/// triggers and changes to knobs.  You want this as large as possible, so Mozzi checks for
/// triggers a lot and so its drumming is tight.  Low values are very sloppy.  But higher 
/// values also means Mozzi's audio engine will struggle to keep up.  This has two impacts:
///
/// 1. You'll hear a background hum related to output sound generator.  The pitch of the 
///    hum is directly related to the control rate, and is particularly high (well, mid-level)
///    especially for the formats with more samples (7, 8, 9, etc.)  It's fairly low and 
///    ina around 100 (but 100 is pretty sloppy).
///
/// 2. Higher values and you'll start getting pops and crackles here and there as Mozzi can't
///    keep the audio going.
///
/// A value of 256 seems to be pretty good except for formats 5A and 7, which can't handle that
/// speed.  I keep them down to 200.  But you might drop down into the sloppy range (64 to 128)
/// if it bothers you.

#if ((FORMAT == FORMAT_5A) || (FORMAT == FORMAT_7))
#define CONTROL_RATE 200
#else
#define CONTROL_RATE 256
#endif


/// THE FORMATS, and their associated CONFIGURATIONS, are described in detail below:

/// FORMAT_1
/// In this format there is a single sample, SAMPLE_1.  You can change its pitch and
/// start and end points with the pots.
// POT 3                END 1           [If START > END, they are swapped]      [Set to MAN]
// POT 2                START 1         [If START > END, they are swapped]      [Set to MAN]
// POT 1                PITCH 1         [CENTER: Original Pitch]
// GATE OUT             UNUSED
// AUDIO IN             UNUSED
// IN 3                 TRIGGER 1
// IN 2                 UNUSED
// IN 1                 PITCH CV 1

/// FORMAT_2
/// There are two samples, SAMPLE_1 and SAMPLE_2.  You can change the pitch of each
/// one and the total volume with the pots  Set the volume such that when the two 
/// samples play simultaneously you don't have pops or clipping.
// POT 3                VOLUME
// POT 2                PITCH 2         [CENTER: Original Pitch]        [Note GRAINS BUG ABOVE]
// POT 1                PITCH 1         [CENTER: Original Pitch]        [Note GRAINS BUG ABOVE]
// GATE OUT             UNUSED
// AUDIO IN             TRIGGER 2
// IN 3                 TRIGGER 1
// IN 2                 PITCH CV 2
// IN 1                 PITCH CV 1 

/// FORMAT_3
/// There are three samples, SAMPLE_1 through SAMPLE_3.  You can change the pitch
/// of SAMPLE_1 and SAMPLE_2 with the pots, as well as the total volume.
/// Set the volume such that when the two samples play
/// simultaneously you don't have pops or clipping.
// POT 3                VOLUME
// POT 2                PITCH 2         [CENTER: Original Pitch]        [Note GRAINS BUG ABOVE]
// POT 1                PITCH 1         [CENTER: Original Pitch]        [Note GRAINS BUG ABOVE]
// GATE OUT             TRIGGER 1
// AUDIO IN             TRIGGER 2
// IN 3                 TRIGGER 3
// IN 2                 PITCH CV 2
// IN 1                 PITCH CV 1 

/// FORMAT_4
/// There are four samples, SAMPLE_1 through SAMPLE_4.  You can change the pitch
/// of SAMPLE_1 and the total volume with the pots.
/// Set the volume such that when the two samples play
/// simultaneously you don't have pops or clipping.
// POT 3                VOLUME
// POT 2                UNUSED          [SET TO MAXIMUM, AND SET SWITCH TO IN2]
// POT 1                PITCH 1         [CENTER: Original Pitch]        [Note GRAINS BUG ABOVE]
// GATE OUT             TRIGGER 1
// AUDIO IN             TRIGGER 2
// IN 3                 TRIGGER 3
// IN 2                 TRIGGER 4
// IN 1                 PITCH CV 1 

/// FORMAT_5
/// There are five samples, SAMPLE_1 through SAMPLE_5.  You can change the
/// total volume with a pot.
/// Set the volume such that when the two samples play
/// simultaneously you don't have pops or clipping.
/// Note that in FORMAT_5 the per-sound gain #define feature is turned OFF.
// POT 3                VOLUME
// POT 2                UNUSED          [SET TO MAXIMUM, AND SET SWITCH TO IN2]
// POT 1                UNUSED          [SET TO MAXIMUM, AND SET SWITCH TO IN1]
// GATE OUT             TRIGGER 1
// AUDIO IN             TRIGGER 2
// IN 3                 TRIGGER 3
// IN 2                 TRIGGER 4
// IN 1                 TRIGGER 5 

/// FORMAT_5A
/// There are five samples, SAMPLE_1 through SAMPLE_5.  You can change the
/// total volume with a pot.  Set the volume such that when the two samples play
/// simultaneously you don't have pops or clipping.
/// There are only FOUR triggers.  SAMPLE_4 and SAMPLE_5 share a trigger: a 
/// pot determines which sample plays.  The purpose of this format is to be
/// more convenient for people with 4-trigger sequencers such as TRIQ164 or
/// BEATS on the GRAINS.
// POT 3                VOLUME
// POT 2                UNUSED          [SET TO MAXIMUM, AND SET SWITCH TO IN2]
// POT 1                SELECT SAMPLES FOR TRIGGER 4 -> { 4, 5 }        [SET TO MAN]
// GATE OUT             TRIGGER 1
// AUDIO IN             TRIGGER 2
// IN 3                 TRIGGER 3
// IN 2                 TRIGGER 4
// IN 1                 UNUSED 


/// FORMAT_6
/// There are six samples, SAMPLE_1 through SAMPLE_6.  You can change the
/// total volume with a pot.  Set the volume such that when the two samples play
/// simultaneously you don't have pops or clipping.
/// There are only FOUR triggers.  SAMPLE_4, SAMPLE_5, and SAMPLE_6 share a trigger: a 
/// pot determines which sample plays.  
// POT 3                VOLUME
// POT 2                UNUSED          [SET TO MAXIMUM, AND SET SWITCH TO IN2]
// POT 1                SELECT SAMPLES FOR TRIGGER 4 -> { 4, 5, 6 }     [SET TO MAN]
// GATE OUT             TRIGGER 1
// AUDIO IN             TRIGGER 2
// IN 3                 TRIGGER 3
// IN 1                 UNUSED 

/// FORMAT_7
/// There are seven samples, SAMPLE_1 through SAMPLE_7.  You can change the
/// total volume with a pot.  Set the volume such that when the two samples play
/// simultaneously you don't have pops or clipping.
/// There are only FOUR triggers.  SAMPLE_4, SAMPLE_5, SAMPLE_6, and SAMPLE_7 
/// share a trigger: a pot determines which sample plays.  If you would like to
/// spread the sharing love over two triggers, try FORMAT_7A instead.
// POT 3                VOLUME
// POT 2                UNUSED          [SET TO MAXIMUM, AND SET SWITCH TO IN2]
// POT 1                SELECT SAMPLES FOR TRIGGER 4 -> { 4, 5, 6, 7 }  [SET TO MAN]
// GATE OUT             TRIGGER 1
// AUDIO IN             TRIGGER 2
// IN 3                 TRIGGER 3
// IN 2                 TRIGGER 4
// IN 1                 UNUSED 

/// FORMAT_7A
/// There are seven samples, SAMPLE_1 through SAMPLE_7.  You can change the
/// total volume with a pot.  Set the volume such that when the two samples play
/// simultaneously you don't have pops or clipping.
/// There are only THREE triggers.  SAMPLE_2, SAMPLE_3, SAMPLE_4 share a trigger,
/// and SAMPLE_5, SAMPLE_6, and SAMPLE_7 share a different trigger.  Two pots
/// determine which samples play on these triggers.  If you would like to
/// group more samples on one trigger and off the other, try FORMAT_7 instead.
// POT 3                VOLUME
// POT 2                SELECT SAMPLES FOR TRIGGER 2 -> { 2, 3, 4 }     [SET TO MAN]
// POT 1                SELECT SAMPLES FOR TRIGGER 3 -> { 5, 6, 7 }     [SET TO MAN]
// GATE OUT             TRIGGER 1
// AUDIO IN             TRIGGER 2
// IN 3                 TRIGGER 3
// IN 2                 UNUSED
// IN 1                 UNUSED 

/// FORMAT_8
/// There are eight samples, SAMPLE_1 through SAMPLE_8.  You can change the
/// total volume with a pot.  Set the volume such that when the two samples play
/// simultaneously you don't have pops or clipping.
/// There are only THREE triggers.  SAMPLE_2, SAMPLE_3, SAMPLE_4 share a trigger,
/// and SAMPLE_5, SAMPLE_6, SAMPLE_7, and SAMPLE_8 share a different trigger.  
/// Two pots determine which samples play on these triggers.
// POT 3                VOLUME
// POT 2                SELECT SAMPLES FOR TRIGGER 2 -> { 2, 3, 4 }             [SET TO MAN]
// POT 1                SELECT SAMPLES FOR TRIGGER 3 -> { 5, 6, 7, 8 }  [SET TO MAN]
// GATE OUT             TRIGGER 1
// AUDIO IN             TRIGGER 2
// IN 3                 TRIGGER 3
// IN 2                 UNUSED
// IN 1                 UNUSED 

/// FORMAT_9
/// There are nine samples, SAMPLE_1 through SAMPLE_9.  You can change the
/// total volume with a pot.  Set the volume such that when the two samples play
/// simultaneously you don't have pops or clipping.
/// There are only THREE triggers.  SAMPLE_2, SAMPLE_3, SAMPLE_4, and SAMPLE_5 
/// share a trigger, and SAMPLE_6, SAMPLE_7, SAMPLE_8, and SAMPLE_9 share a 
/// different trigger.  Two pots determine which samples play on these triggers.
// POT 3                VOLUME
// POT 2                SELECT SAMPLES FOR TRIGGER 2 -> { 2, 3, 4, 5 }  [SET TO MAN]
// POT 1                SELECT SAMPLES FOR TRIGGER 3 -> { 6, 7, 8, 9 }  [SET TO MAN]
// GATE OUT             TRIGGER 1
// AUDIO IN             TRIGGER 2
// IN 3                 TRIGGER 3
// IN 2                 UNUSED
// IN 1                 UNUSED 




/// .... AND WE'RE DONE!




#define CV_POT_IN1    A2 
#define CV_POT_IN2    A1
#define CV_POT3       A0
#define CV_IN3        A3
#define CV_AUDIO_IN   A4
#define CV_AUDIO_OUT  9
#define CV_GATE_OUT   8
#define RANDOM_PIN    A5

// Mozzi's setFreq function (which we use for setting sample pitch)
// requires dividing by the total length of the sample as a float.
// We precompute the inverse lengths of Sample 0 and Sample 1 so we
// don't have to divide, but can multiply.
float inverseLengths0;
float inverseLengths1;

// When a pin goes high, and we are triggered, we need to indicate this
// so next time we check we don't think we're triggered a second time just
// because it's stayed high.  We then set this back to 0 when the pin has
// gone low.
uint16_t triggered[5] = { 0, 0, 0, 0, 0 };

// The current values of our three pots.
uint16_t pot[3];

// The smoothed estimates of our pitches for Samples 0 and 1,
// Start and End for Sample 0, and Volume for everyone else
#define UNDEFINED (-1)
int16_t previousPitch1 = UNDEFINED;
int16_t previousPitch2 = UNDEFINED;
int16_t previousStart = UNDEFINED;
int16_t previousEnd = UNDEFINED;
int16_t previousVolume = UNDEFINED;

#define HIGH_TRIGGER_BOUNDARY 200
#define LOW_TRIGGER_BOUNDARY 100



// I want a lookup table from 0...1024 which maps to 1/2 ... 2 exponentially.
// I don't have the memory for it in floats though, nor 1024 elements.  So
// we take 0...1024 and divide by 4 to get 0...255.  Then we look it up in FREQUENCIES.
// Finally we divide by 128.
const uint8_t FREQUENCIES[] = {
    64, 64, 64, 65, 65, 65, 66, 66, 66, 67, 67, 67, 68, 68, 69, 69, 69,
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

// Computes the frequency for a given pitch.  We set it up so that "Normal Pitch"
// dominates the center region with a big sweet spot, and you have to get out of
// that sweet spot to start bending the pitch up or down.
float computeFrequency(uint16_t pitch, float inverseLength)
    {
    float freq = 1.0;   // dead center
    if (pitch >= 512 + 128)
        {
        freq = FREQUENCIES[(pitch - 128) >> 2] * (1.0 / 128.0);
        }
    else if (pitch <= 512 - 128)
        {
        freq = FREQUENCIES[(pitch + 128) >> 2] * (1.0 / 128.0);
        }
    return 16384 * inverseLength * freq;
    }




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


//// DIGITAL OUT registers a trigger faster than the analog reads, particularly Analog In,
//// partly because the capacitors on them I think and partly because mozziAnalogRead
//// does a delayed read.  At any rate, to get Digital Out to line up with the others
//// in its triggering, we have to delay it three ticks.  We do that with a little countdown
//// timer here.
#define COUNTDOWN 4
uint8_t counter = 1;

void updateControl1()
    {
    // POT 3            END 1
    // POT 2            START 1
    // POT 1            PITCH 1
    // GATE OUT         TRIGGER 1
    // AUDIO IN         UNUSED
    // IN 3             UNUSED
    // IN 2             UNUSED
    // IN 1             PITCH CV 1
        
    if (previousPitch1 == UNDEFINED) previousPitch1 = pot[0] = mozziAnalogRead(CV_POT3);
    else pot[0] = previousPitch1 = (previousPitch1 * 7 + mozziAnalogRead(CV_POT3)) >> 3;
    if (previousStart == UNDEFINED) previousStart = pot[1] = mozziAnalogRead(CV_POT_IN2);
    else pot[1] = previousStart = (previousStart * 7 + mozziAnalogRead(CV_POT_IN2)) >> 3;
    if (previousEnd == UNDEFINED) previousEnd = pot[2] = mozziAnalogRead(CV_POT_IN1);
    else pot[2] = previousEnd = (previousEnd * 7 + mozziAnalogRead(CV_POT_IN1)) >> 3;

    uint8_t val = digitalRead(CV_GATE_OUT);
    if (!triggered[0] && val) 
        { 
        // don't bother with the countdown in this situation, only one drum
        
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
		sample0.start(); 
        }
    triggered[0] = val;
    }
        
void updateControl2()
    {
    // POT 3            VOLUME
    // POT 2            PITCH 2
    // POT 1            PITCH 1
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             UNUSED
    // IN 2             PITCH CV 2
    // IN 1             PITCH CV 1 

    if (previousPitch1 == UNDEFINED) previousPitch1 = pot[0] = mozziAnalogRead(CV_POT_IN1);
    else pot[0] = previousPitch1 = (previousPitch1 * 7 + mozziAnalogRead(CV_POT_IN1)) >> 3;
    if (previousPitch2 == UNDEFINED) previousPitch2 = pot[1] = mozziAnalogRead(CV_POT_IN2);
    else pot[1] = previousPitch2 = (previousPitch2 * 7 + mozziAnalogRead(CV_POT_IN2)) >> 3;
    if (previousVolume == UNDEFINED) previousVolume = pot[2] = mozziAnalogRead(CV_POT3);
    else pot[2] = previousVolume = (previousVolume * 3 + mozziAnalogRead(CV_POT3)) >> 2;
        
    uint8_t val = digitalRead(CV_GATE_OUT);
    if (!triggered[0] && val) 
        { 
        if (counter == 0) counter = COUNTDOWN;
        }
    triggered[0] = val;
        
    if (counter == 1)
        {
        // set pitch only when we start
        sample0.setFreq(computeFrequency(pot[0], inverseLengths0));
        sample0.start(); 
        }
    if (counter > 0) counter--;
        
    uint16_t val1 = mozziAnalogRead(CV_AUDIO_IN);
    if (!triggered[1] && val1 > HIGH_TRIGGER_BOUNDARY) 
        { 
        // set pitch only when we start
        sample1.setFreq(computeFrequency(pot[1], inverseLengths1));
        triggered[1] = 1; 
        sample1.start(); 
        }
    else if (triggered[1] && val1 < LOW_TRIGGER_BOUNDARY) { triggered[1] = 0; }
    }

void updateControl3()
    {
    // POT 3            VOLUME
    // POT 2            PITCH 2
    // POT 1            PITCH 1
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             PITCH CV 2
    // IN 1             PITCH CV 1 

    if (previousPitch1 == UNDEFINED) previousPitch1 = pot[0] = mozziAnalogRead(CV_POT_IN1);
    else pot[0] = previousPitch1 = (previousPitch1 * 7 + mozziAnalogRead(CV_POT_IN1)) >> 3;
    if (previousPitch2 == UNDEFINED) previousPitch2 = pot[1] = mozziAnalogRead(CV_POT_IN2);
    else pot[1] = previousPitch2 = (previousPitch2 * 7 + mozziAnalogRead(CV_POT_IN2)) >> 3;
    if (previousVolume == UNDEFINED) previousVolume = pot[2] = mozziAnalogRead(CV_POT3);
    else pot[2] = previousVolume = (previousVolume * 3 + mozziAnalogRead(CV_POT3)) >> 2;
        
    uint8_t val = digitalRead(CV_GATE_OUT);
    if (!triggered[0] && val) 
        {
        if (counter == 0) counter = COUNTDOWN;
        }
    triggered[0] = val;

    if (counter == 1)
        {
        // set pitch only when we start
        sample0.setFreq(computeFrequency(pot[0], inverseLengths0));
        sample0.start(); 
        }
    if (counter > 0) counter--;
        
    uint16_t val1 = mozziAnalogRead(CV_AUDIO_IN);
    if (!triggered[1] && val1 > HIGH_TRIGGER_BOUNDARY) 
        { 
        // set pitch only when we start
        sample1.setFreq(computeFrequency(pot[1], inverseLengths1));
        triggered[1] = 1;
        sample1.start(); 
        }
    else if (triggered[1] && val1 < LOW_TRIGGER_BOUNDARY) { triggered[1] = 0; }
        
    uint16_t val2 = mozziAnalogRead(CV_IN3);
    if (!triggered[2] && val2 > HIGH_TRIGGER_BOUNDARY) { triggered[2] = 1; sample2.start(); }
    else if (triggered[2] && val2 < LOW_TRIGGER_BOUNDARY) { triggered[2] = 0; }
    }

void updateControl4()
    {
    // POT 3            VOLUME
    // POT 2            UNUSED
    // POT 1            PITCH 1
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             TRIGGER 4
    // IN 1             PITCH CV 1 
        
    if (previousPitch1 == UNDEFINED) previousPitch1 = pot[0] = mozziAnalogRead(CV_POT_IN1);
    else pot[0] = previousPitch1 = (previousPitch1 * 7 + mozziAnalogRead(CV_POT_IN1)) >> 3;
    /// POT[1] USED FOR TRIGGER
    if (previousVolume == UNDEFINED) previousVolume = pot[2] = mozziAnalogRead(CV_POT3);
    else pot[2] = previousVolume = (previousVolume * 3 + mozziAnalogRead(CV_POT3)) >> 2;
        
    uint8_t val = digitalRead(CV_GATE_OUT);
    if (!triggered[0] && val) 
        { 
        if (counter == 0) counter = COUNTDOWN;
        }
    triggered[0] = val;

    if (counter == 1)
        {
        // set pitch only when we start
        sample0.setFreq(computeFrequency(pot[0], inverseLengths0));
        sample0.start(); 
        }
    if (counter > 0) counter--;
        
    uint16_t val1 = mozziAnalogRead(CV_AUDIO_IN);
    if (!triggered[1] && val1 > HIGH_TRIGGER_BOUNDARY) { triggered[1] = 1; sample1.start(); }
    else if (triggered[1] && val1 < LOW_TRIGGER_BOUNDARY) { triggered[1] = 0; }

    uint16_t val2 = mozziAnalogRead(CV_IN3);
    if (!triggered[2] && val2 > HIGH_TRIGGER_BOUNDARY) { triggered[2] = 1; sample2.start(); }
    else if (triggered[2] && val2 < LOW_TRIGGER_BOUNDARY) { triggered[2] = 0; }

    uint16_t val3 = mozziAnalogRead(CV_POT_IN2);
    if (!triggered[3] && val3 > HIGH_TRIGGER_BOUNDARY) { triggered[3] = 1; sample3.start(); }
    else if (triggered[3] && val3 < LOW_TRIGGER_BOUNDARY) { triggered[3] = 0; }
        
    }

void updateControl5()
    {
    // POT 3            VOLUME
    // POT 2            UNUSED
    // POT 1            UNUSED
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             TRIGGER 4
    // IN 1             TRIGGER 5 

    /// POT[0] USED FOR TRIGGER
    /// POT[1] USED FOR TRIGGER
    if (previousVolume == UNDEFINED) previousVolume = pot[2] = mozziAnalogRead(CV_POT3);
    else pot[2] = previousVolume = (previousVolume * 3 + mozziAnalogRead(CV_POT3)) >> 2;
        
    uint8_t val = digitalRead(CV_GATE_OUT);
    if (!triggered[0] && val) 
        { 
        if (counter == 0) counter = COUNTDOWN;
        }
    triggered[0] = val;

    if (counter == 1)
        {
        sample0.start(); 
        }
    if (counter > 0) counter--;
        
    uint16_t val1 = mozziAnalogRead(CV_AUDIO_IN);
    if (!triggered[1] && val1 > HIGH_TRIGGER_BOUNDARY) { triggered[1] = 1; sample1.start(); }
    else if (triggered[1] && val1 < LOW_TRIGGER_BOUNDARY) { triggered[1] = 0; }

    uint16_t val2 = mozziAnalogRead(CV_IN3);
    if (!triggered[2] && val2 > HIGH_TRIGGER_BOUNDARY) { triggered[2] = 1; sample2.start(); }
    else if (triggered[2] && val2 < LOW_TRIGGER_BOUNDARY) { triggered[2] = 0; }

    uint16_t val3 = mozziAnalogRead(CV_POT_IN2);
    if (!triggered[3] && val3 > HIGH_TRIGGER_BOUNDARY) { triggered[3] = 1; sample3.start(); }
    else if (triggered[3] && val3 < LOW_TRIGGER_BOUNDARY) { triggered[3] = 0; }

    uint16_t val4 = mozziAnalogRead(CV_POT_IN1);
    if (!triggered[4] && val4 > HIGH_TRIGGER_BOUNDARY) { triggered[4] = 1; sample4.start(); }
    else if (triggered[4] && val4 < LOW_TRIGGER_BOUNDARY) { triggered[4] = 0; }
    }

void updateControl5A()
    {
    // POT 3            VOLUME
    // POT 2            UNUSED
    // POT 1            SAMPLES 4 -> { 4, 5 }
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             TRIGGER 4
    // IN 1             UNUSED 
        
    pot[0] = mozziAnalogRead(CV_POT_IN1);
    /// POT[1] USED FOR TRIGGER
    if (previousVolume == UNDEFINED) previousVolume = pot[2] = mozziAnalogRead(CV_POT3);
    else pot[2] = previousVolume = (previousVolume * 3 + mozziAnalogRead(CV_POT3)) >> 2;
        
    uint8_t val = digitalRead(CV_GATE_OUT);
    if (!triggered[0] && val) 
        { 
        if (counter == 0) counter = COUNTDOWN;
        }
    triggered[0] = val;

    if (counter == 1)
        {
        sample0.start(); 
        }
    if (counter > 0) counter--;
        
    uint16_t val1 = mozziAnalogRead(CV_AUDIO_IN);
    if (!triggered[1] && val1 > HIGH_TRIGGER_BOUNDARY) { triggered[1] = 1; sample1.start(); }
    else if (triggered[1] && val1 < LOW_TRIGGER_BOUNDARY) { triggered[1] = 0; }

    uint16_t val2 = mozziAnalogRead(CV_IN3);
    if (!triggered[2] && val2 > HIGH_TRIGGER_BOUNDARY) { triggered[2] = 1; sample2.start(); }
    else if (triggered[2] && val2 < LOW_TRIGGER_BOUNDARY) { triggered[2] = 0; }

    uint16_t val3 = mozziAnalogRead(CV_POT_IN2);
    if (!triggered[3] && val3 > HIGH_TRIGGER_BOUNDARY) { triggered[3] = 1; if (pot[0] < 512) sample3.start(); else sample4.start(); }
    else if (triggered[3] && val3 < LOW_TRIGGER_BOUNDARY) { triggered[3] = 0; }
    }

void updateControl6()
    {
    // POT 3            VOLUME
    // POT 2            UNUSED
    // POT 1            SAMPLES 4 -> { 4, 5, 6 }
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             TRIGGER 4
    // IN 1             UNUSED 

    pot[0] = mozziAnalogRead(CV_POT_IN1);
    /// POT[1] USED FOR TRIGGER
    if (previousVolume == UNDEFINED) previousVolume = pot[2] = mozziAnalogRead(CV_POT3);
    else pot[2] = previousVolume = (previousVolume * 3 + mozziAnalogRead(CV_POT3)) >> 2;
        
    uint8_t val = digitalRead(CV_GATE_OUT);
    if (!triggered[0] && val) 
        { 
        if (counter == 0) counter = COUNTDOWN;
        }
    triggered[0] = val;
        
    if (counter == 1)
        {
        sample0.start(); 
        }
    if (counter > 0) counter--;

    uint16_t val1 = mozziAnalogRead(CV_AUDIO_IN);
    if (!triggered[1] && val1 > HIGH_TRIGGER_BOUNDARY) { triggered[1] = 1; sample1.start(); }
    else if (triggered[1] && val1 < LOW_TRIGGER_BOUNDARY) { triggered[1] = 0; }

    uint16_t val2 = mozziAnalogRead(CV_IN3);
    if (!triggered[2] && val2 > HIGH_TRIGGER_BOUNDARY) { triggered[2] = 1; sample2.start(); }
    else if (triggered[2] && val2 < LOW_TRIGGER_BOUNDARY) { triggered[2] = 0; }

    uint16_t val3 = mozziAnalogRead(CV_POT_IN2);
    if (!triggered[3] && val3 > HIGH_TRIGGER_BOUNDARY) { triggered[3] = 1; if (pot[0] < 341) sample3.start(); else if (pot[0] < 682) sample4.start(); else sample5.start(); }
    else if (triggered[3] && val3 < LOW_TRIGGER_BOUNDARY) { triggered[3] = 0; }
    }

void updateControl7()
    {
    // POT 3            VOLUME
    // POT 2            UNUSED
    // POT 1            SAMPLES 4 -> { 4, 5, 6, 7 }
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             TRIGGER 4
    // IN 1             UNUSED 

    pot[0] = mozziAnalogRead(CV_POT_IN1);
    /// POT[1] USED FOR TRIGGER
    if (previousVolume == UNDEFINED) previousVolume = pot[2] = mozziAnalogRead(CV_POT3);
    else pot[2] = previousVolume = (previousVolume * 3 + mozziAnalogRead(CV_POT3)) >> 2;
        
    uint8_t val = digitalRead(CV_GATE_OUT);
    if (!triggered[0] && val) 
        { 
        if (counter == 0) counter = COUNTDOWN;
        }
    triggered[0] = val;

    if (counter == 1)
        {
        sample0.start(); 
        }
    if (counter > 0) counter--;
        
    uint16_t val1 = mozziAnalogRead(CV_AUDIO_IN);
    if (!triggered[1] && val1 > HIGH_TRIGGER_BOUNDARY) { triggered[1] = 1; sample1.start(); }
    else if (triggered[1] && val1 < LOW_TRIGGER_BOUNDARY) { triggered[1] = 0; }

    uint16_t val2 = mozziAnalogRead(CV_IN3);
    if (!triggered[2] && val2 > HIGH_TRIGGER_BOUNDARY) { triggered[2] = 1; sample2.start(); }
    else if (triggered[2] && val2 < LOW_TRIGGER_BOUNDARY) { triggered[2] = 0; }

    uint16_t val3 = mozziAnalogRead(CV_POT_IN2);
    if (!triggered[3] && val3 > HIGH_TRIGGER_BOUNDARY) { triggered[3] = 1; if (pot[0] < 256) sample3.start(); else if (pot[0] < 512) sample4.start(); else if (pot[0] < 768) sample5.start(); else sample6.start(); }
    else if (triggered[3] && val3 < LOW_TRIGGER_BOUNDARY) { triggered[3] = 0; }
    }

void updateControl7A()
    {
    // POT 3            VOLUME
    // POT 2            SAMPLES 2 -> { 2, 3, 4 }
    // POT 1            SAMPLES 3 -> { 5, 6, 7 }
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             UNUSED
    // IN 1             UNUSED 

    pot[0] = mozziAnalogRead(CV_POT_IN1);
    pot[1] = mozziAnalogRead(CV_POT_IN2);
    if (previousVolume == UNDEFINED) previousVolume = pot[2] = mozziAnalogRead(CV_POT3);
    else pot[2] = previousVolume = (previousVolume * 3 + mozziAnalogRead(CV_POT3)) >> 2;

    uint8_t val = digitalRead(CV_GATE_OUT);
    if (!triggered[0] && val) 
        { 
        if (counter == 0) counter = COUNTDOWN;
        }
    triggered[0] = val;

    if (counter == 1)
        {
        sample0.start(); 
        }
    if (counter > 0) counter--;
        
    uint16_t val1 = mozziAnalogRead(CV_AUDIO_IN);
    if (!triggered[1] && val1 > HIGH_TRIGGER_BOUNDARY) { triggered[1] = 1; if (pot[1] < 341) sample1.start(); else if (pot[1] < 682) sample2.start(); else sample3.start(); }
    else if (triggered[1] && val1 < LOW_TRIGGER_BOUNDARY) { triggered[1] = 0; }

    uint16_t val2 = mozziAnalogRead(CV_IN3);
    if (!triggered[2] && val2 > HIGH_TRIGGER_BOUNDARY) { triggered[2] = 1; if (pot[0] < 341) sample4.start(); else if (pot[0] < 682) sample5.start(); else sample6.start(); }
    else if (triggered[2] && val2 < LOW_TRIGGER_BOUNDARY) { triggered[2] = 0; }
    }

void updateControl8()
    {
    // POT 3            VOLUME
    // POT 2            SAMPLES 2 -> { 2, 3, 4 }
    // POT 1            SAMPLES 3 -> { 5, 6, 7, 8 }
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             UNUSED
    // IN 1             UNUSED 

    pot[0] = mozziAnalogRead(CV_POT_IN1);
    pot[1] = mozziAnalogRead(CV_POT_IN2);
    if (previousVolume == UNDEFINED) previousVolume = pot[2] = mozziAnalogRead(CV_POT3);
    else pot[2] = previousVolume = (previousVolume * 3 + mozziAnalogRead(CV_POT3)) >> 2;
        
    uint8_t val = digitalRead(CV_GATE_OUT);
    if (!triggered[0] && val) 
        { 
        if (counter == 0) counter = COUNTDOWN;
        }
    triggered[0] = val;
        
    if (counter == 1)
        {
        sample0.start(); 
        }
    if (counter > 0) counter--;
        
    uint16_t val1 = mozziAnalogRead(CV_AUDIO_IN);
    if (!triggered[1] && val1 > HIGH_TRIGGER_BOUNDARY) { triggered[1] = 1; if (pot[1] < 341) sample1.start(); else if (pot[1] < 682) sample2.start(); else sample3.start(); }
    else if (triggered[1] && val1 < LOW_TRIGGER_BOUNDARY) { triggered[1] = 0; }

    uint16_t val2 = mozziAnalogRead(CV_IN3);
    if (!triggered[2] && val2 > HIGH_TRIGGER_BOUNDARY) { triggered[2] = 1; if (pot[0] < 256) sample4.start(); else if (pot[0] < 512) sample5.start(); else if (pot[0] < 768) sample6.start(); else sample7.start(); }
    else if (triggered[2] && val2 < LOW_TRIGGER_BOUNDARY) { triggered[2] = 0; }
    }

void updateControl9()
    {
    // POT 3            VOLUME
    // POT 2            SAMPLES 2 -> { 2, 3, 4, 5 }
    // POT 1            SAMPLES 3 -> { 6, 7, 8, 9 }
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             UNUSED
    // IN 1             UNUSED 

    pot[0] = mozziAnalogRead(CV_POT_IN1);
    pot[1] = mozziAnalogRead(CV_POT_IN2);
    if (previousVolume == UNDEFINED) previousVolume = pot[2] = mozziAnalogRead(CV_POT3);
    else pot[2] = previousVolume = (previousVolume * 3 + mozziAnalogRead(CV_POT3)) >> 2;
                
    uint8_t val = digitalRead(CV_GATE_OUT);
    if (!triggered[0] && val) 
        { 
        if (counter == 0) counter = COUNTDOWN;
        }
    triggered[0] = val;

    if (counter == 1)
        {
        sample0.start(); 
        }
    if (counter > 0) counter--;
        
    uint16_t val1 = mozziAnalogRead(CV_AUDIO_IN);
    if (!triggered[1] && val1 > HIGH_TRIGGER_BOUNDARY) 
    	{ 
    	triggered[1] = 1; 
    	if (pot[1] < 512) 
    		{
    		if (pot[1] < 256) sample1.start(); else sample2.start();
    		}
    	else
    		{
    		if (pot[1] < 768) sample3.start(); else sample4.start();
    		}
    	}
    else if (triggered[1] && val1 < LOW_TRIGGER_BOUNDARY) { triggered[1] = 0; }

    uint16_t val2 = mozziAnalogRead(CV_IN3);
    if (!triggered[2] && val2 > HIGH_TRIGGER_BOUNDARY) 
    	{ 
    	triggered[2] = 1; 
    	if (pot[0] < 512) 
    		{
    		if (pot[0] < 256) sample5.start(); else sample6.start(); 
    		}
    	else 
    		{
    		if (pot[0] < 768) sample7.start(); else sample8.start(); 
    		}
    	}
    else if (triggered[2] && val2 < LOW_TRIGGER_BOUNDARY) { triggered[2] = 0; }
    }









/*
** This routine converts from ulaw to 16 bit linear.
**
** Craig Reese: IDA/Supercomputing Research Center
** 29 September 1989
**
** References:
** 1) CCITT Recommendation G.711  (very difficult to follow)
** 2) MIL-STD-188-113,"Interoperability and Performance Standards
**     for Analog-to_Digital Conversion Techniques,"
**     17 February 1987
**
** Input: 8 bit ulaw sample
** Output: signed 16 bit linear sample
*/

/*
  #include <stdio.h>

  typedef unsigned char uint8_t;
  typedef signed short int16_t;

  const int16_t exp_lut[8] = {0,132,396,924,1980,4092,8316,16764};
  int16_t ulaw2linear(uint8_t ulawbyte)
  {
  ulawbyte = ~ulawbyte;
  uint8_t sign = (ulawbyte & 0x80);
  uint8_t exponent = (ulawbyte >> 4) & 0x07;
  uint8_t mantissa = ulawbyte & 0x0F;
  int16_t sample = exp_lut[exponent] + (mantissa << (exponent + 3));
  if (sign != 0) sample = -sample;
  return sample;
  }
  int main( int argc, char *argv[] )
  {
  for(int i = 0; i < 256; i++)
  {
  printf("%d, ", (ulaw2linear(i) * 244) >> 15);
  if (i % 16 == 15) printf("\n");
  }
*/




//// NOTE about ULAW.  ULAW is stupidly encoded: 0 encoded does not map to 0 output.  As a result
//// we cannot just use the lookup table below and call it a day, because when the sound is not
//// playing, Mozzi (of course) outputs a 0, which when we push it through the ULAW decoder
//// comes out as 32124.  That's a big DC offset, making a huge click when you start the sound.
//// Hence the CC(...) #define, which does the decoding but ALSO multiplies it against isPlaying()
//// to force it to zero when there's no sound.  C(...) is the actual decoder, plus tweaks for volume.
//// Note that I keep the result between -4K and 4K so we can add all five of the triggers together
//// in 16 bit.

#ifdef USE_ULAW
/*
const int16_t __ULAW[] = {
    -32124, -31100, -30076, -29052, -28028, -27004, -25980, -24956, -23932, -22908, -21884, -20860, -19836, -18812, -17788, -16764, 
    -15996, -15484, -14972, -14460, -13948, -13436, -12924, -12412, -11900, -11388, -10876, -10364, -9852, -9340, -8828, -8316, 
    -7932, -7676, -7420, -7164, -6908, -6652, -6396, -6140, -5884, -5628, -5372, -5116, -4860, -4604, -4348, -4092, 
    -3900, -3772, -3644, -3516, -3388, -3260, -3132, -3004, -2876, -2748, -2620, -2492, -2364, -2236, -2108, -1980, 
    -1884, -1820, -1756, -1692, -1628, -1564, -1500, -1436, -1372, -1308, -1244, -1180, -1116, -1052, -988, -924, 
    -876, -844, -812, -780, -748, -716, -684, -652, -620, -588, -556, -524, -492, -460, -428, -396, 
    -372, -356, -340, -324, -308, -292, -276, -260, -244, -228, -212, -196, -180, -164, -148, -132, 
    -120, -112, -104, -96, -88, -80, -72, -64, -56, -48, -40, -32, -24, -16, -8, 0, 
    32124, 31100, 30076, 29052, 28028, 27004, 25980, 24956, 23932, 22908, 21884, 20860, 19836, 18812, 17788, 16764, 
    15996, 15484, 14972, 14460, 13948, 13436, 12924, 12412, 11900, 11388, 10876, 10364, 9852, 9340, 8828, 8316, 
    7932, 7676, 7420, 7164, 6908, 6652, 6396, 6140, 5884, 5628, 5372, 5116, 4860, 4604, 4348, 4092, 
    3900, 3772, 3644, 3516, 3388, 3260, 3132, 3004, 2876, 2748, 2620, 2492, 2364, 2236, 2108, 1980, 
    1884, 1820, 1756, 1692, 1628, 1564, 1500, 1436, 1372, 1308, 1244, 1180, 1116, 1052, 988, 924, 
    876, 844, 812, 780, 748, 716, 684, 652, 620, 588, 556, 524, 492, 460, 428, 396, 
    372, 356, 340, 324, 308, 292, 276, 260, 244, 228, 212, 196, 180, 164, 148, 132, 
    120, 112, 104, 96, 88, 80, 72, 64, 56, 48, 40, 32, 24, 16, 8, 0, 
    };
*/
// this version is 1/8 size 
const int16_t __ULAW2[] = {
-4015, -3887, -3759, -3631, -3503, -3375, -3247, -3119, -2991, -2863, -2735, -2607, -2479, -2351, -2223, -2095,
-1999, -1935, -1871, -1807, -1743, -1679, -1615, -1551, -1487, -1423, -1359, -1295, -1231, -1167, -1103, -1039, 
-991, -959, -927, -895, -863, -831, -799, -767, -735, -703, -671, -639, -607, -575, -543, -511, 
-487, -471, -455, -439, -423, -407, -391, -375, -359, -343, -327, -311, -295, -279, -263, -247, 
-235, -227, -219, -211, -203, -195, -187, -179, -171, -163, -155, -147, -139, -131, -123, -115, 
-109, -105, -101, -97, -93, -89, -85, -81, -77, -73, -69, -65, -61, -57, -53, -49, 
-46, -44, -42, -40, -38, -36, -34, -32, -30, -28, -26, -24, -22, -20, -18, -16,
 -15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 
 4015, 3887, 3759, 3631, 3503, 3375, 3247, 3119, 2991, 2863, 2735, 2607, 2479, 2351, 2223, 2095, 
 1999, 1935, 1871, 1807, 1743, 1679, 1615, 1551, 1487, 1423, 1359, 1295, 1231, 1167, 1103, 1039, 
 991, 959, 927, 895, 863, 831, 799, 767, 735, 703, 671, 639, 607, 575, 543, 511, 
 487, 471, 455, 439, 423, 407, 391, 375, 359, 343, 327, 311, 295, 279, 263, 247, 
 235, 227, 219, 211, 203, 195, 187, 179, 171, 163, 155, 147, 139, 131, 123, 115, 
 109, 105, 101, 97, 93, 89, 85, 81, 77, 73, 69, 65, 61, 57, 53, 49, 
 46, 44, 42, 40, 38, 36, 34, 32, 30, 28, 26, 24, 22, 20, 18, 16, 
 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 
 	};
// Gain is maximal at 8
// ranges (-4K .. 4K)...
#if (FORMAT == FORMAT_5)
int16_t C(int8_t value, uint8_t volume) { return (__ULAW2[value + 128]); }			// too expensive
#else
int16_t C(int8_t value, uint8_t volume) { return ((__ULAW2[value + 128]) * volume) >> 3; } 
#endif
#else
int16_t C(int8_t value, uint8_t volume) { return ((value * volume) * 32) >> 3; }
#endif
#define CC(sample, volume) (sample.isPlaying() * C(sample.next(), volume))

uint16_t sum;

int updateAudio1()
    {
    // POT 3            END 1
    // POT 2            START 1
    // POT 1            PITCH 1
    // GATE OUT         TRIGGER 1
    // AUDIO IN         UNUSED
    // IN 3             UNUSED
    // IN 2             UNUSED
    // IN 1             PITCH CV 1

// believe it or not, the background hum is lower when more samples are computing.
// so we add some dummy samples here.
	sum += CC(sample1, GAIN_1);		// sum is to outwit the compiler
	sum += CC(sample2, GAIN_2);
	sum += CC(sample3, GAIN_3);
    return MonoOutput::from16Bit(CC(sample0, GAIN_0) * 8);
    }
        
int updateAudio2()
    {
    // POT 3            VOLUME
    // POT 2            PITCH 2
    // POT 1            PITCH 1
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             UNUSED
    // IN 2             PITCH CV 2
    // IN 1             PITCH CV 1 

// believe it or not, the background hum is lower when more samples are computing.
// so we add some dummy samples here.
	sum += CC(sample2, GAIN_2);		// sum is to outwit the compiler
    return MonoOutput::from16Bit(((CC(sample0, GAIN_0) + CC(sample1, GAIN_1)) * (uint32_t) pot[2]) >> 6); 
    }

int updateAudio3()
    {
    // POT 3            VOLUME
    // POT 2            PITCH 2
    // POT 1            PITCH 1
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             PITCH CV 2
    // IN 1             PITCH CV 1 

    return MonoOutput::from16Bit(((CC(sample0, GAIN_0) + CC(sample1, GAIN_1) + CC(sample2, GAIN_2)) * (uint32_t) pot[2]) >> 6);
    }

int updateAudio4()
    {
    // POT 3            VOLUME
    // POT 2            UNUSED
    // POT 1            PITCH 1
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             TRIGGER 4
    // IN 1             PITCH CV 1 

    return MonoOutput::from16Bit(((CC(sample0, GAIN_0) + CC(sample1, GAIN_1) + CC(sample2, GAIN_2) + CC(sample3, GAIN_3)) * (uint32_t) pot[2]) >> 6);
    }

int updateAudio5()
    {
    // POT 3            VOLUME
    // POT 2            UNUSED
    // POT 1            UNUSED
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             TRIGGER 4
    // IN 1             TRIGGER 5 

    return MonoOutput::from16Bit((((CC(sample0, GAIN_0) + CC(sample1, GAIN_1) + CC(sample2, GAIN_2) + CC(sample3, GAIN_3) + CC(sample4, GAIN_4))) * (uint32_t) pot[2]) >> 6);
    }

int updateAudio5A()
    {
    // POT 3            VOLUME
    // POT 2            UNUSED
    // POT 1            SAMPLES 4 -> { 4, 5 }
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             TRIGGER 4
    // IN 1             UNUSED 

    return MonoOutput::from16Bit(((CC(sample0, GAIN_0) + CC(sample1, GAIN_1) + CC(sample2, GAIN_2) + (pot[0] < 512 ? CC(sample3, GAIN_3) : CC(sample4, GAIN_4))) * (uint32_t) pot[2]) >> 6);
    }

int updateAudio6()
    {
    // POT 3            VOLUME
    // POT 2            UNUSED
    // POT 1            SAMPLES 4 -> { 4, 5, 6 }
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             TRIGGER 4
    // IN 1             UNUSED 

    return MonoOutput::from16Bit(((CC(sample0, GAIN_0) + CC(sample1, GAIN_1) + CC(sample2, GAIN_2) + (pot[0] < 341 ? CC(sample3, GAIN_3) : (pot[0] < 682 ? CC(sample4, GAIN_4) : CC(sample5, GAIN_5)))) * (uint32_t) pot[2]) >> 6);
    }

int updateAudio7()
    {
    // POT 3            VOLUME
    // POT 2            UNUSED
    // POT 1            SAMPLES 4 -> { 4, 5, 6, 7}
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             TRIGGER 4
    // IN 1             UNUSED 

    return MonoOutput::from16Bit(((CC(sample0, GAIN_0) + CC(sample1, GAIN_1) + CC(sample2, GAIN_2) + 
    	(pot[0] < 512 ? 
    		(pot[0] < 256 ? CC(sample3, GAIN_3) : CC(sample4, GAIN_4)) :
    		(pot[0] < 768 ? CC(sample5, GAIN_5) : CC(sample6, GAIN_6)))) * (uint32_t) pot[2]) >> 6);
    }

int updateAudio7A()
    {
    // POT 3            VOLUME
    // POT 2            SAMPLES 2 -> { 2, 3, 4 }
    // POT 1            SAMPLES 3 -> { 5, 6, 7 }
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             UNUSED
    // IN 1             UNUSED 

    return MonoOutput::from16Bit(((CC(sample0, GAIN_0) + (pot[1] < 341 ? CC(sample1, GAIN_1) : (pot[1] < 682 ? CC(sample2, GAIN_2) : CC(sample3, GAIN_3))) + (pot[0] < 341 ? CC(sample4, GAIN_4) : (pot[0] < 682 ? CC(sample5, GAIN_5) : CC(sample6, GAIN_6)))) * (uint32_t) pot[2]) >> 6);
    }

int updateAudio8()
    {
    // POT 3            VOLUME
    // POT 2            SAMPLES 2 -> { 2, 3, 4 }
    // POT 1            SAMPLES 3 -> { 5, 6, 7, 8 }
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             UNUSED
    // IN 1             UNUSED 
        
    return MonoOutput::from16Bit(((CC(sample0, GAIN_0) + 
    	(pot[1] < 341 ? CC(sample1, GAIN_1) : (pot[1] < 682 ? CC(sample2, GAIN_2) : CC(sample3, GAIN_3))) + 
    	(pot[0] < 512 ?
    		(pot[0] < 256 ? CC(sample4, GAIN_4) : CC(sample5, GAIN_5)) :
    		(pot[0] < 768 ? CC(sample6, GAIN_6) : CC(sample7, GAIN_7)))) * (uint32_t) pot[2]) >> 6);
    }

int updateAudio9()
    {
    // POT 3            VOLUME
    // POT 2            SAMPLES 2 -> { 2, 3, 4, 5 }
    // POT 1            SAMPLES 3 -> { 6, 7, 8, 9 }
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             UNUSED
    // IN 1             UNUSED 
        
    return MonoOutput::from16Bit(((CC(sample0, GAIN_0) + 
    	(pot[1] < 512 ?
    		(pot[1] < 256 ? CC(sample1, GAIN_1) : CC(sample2, GAIN_2)) :
    		(pot[1] < 768 ? CC(sample3, GAIN_3) : CC(sample4, GAIN_4))) + 
    	(pot[0] < 512 ? 
    		(pot[0] < 256 ? CC(sample5, GAIN_5) : CC(sample6, GAIN_6)) : 
    		(pot[0] < 768 ? CC(sample7, GAIN_7) : CC(sample8, GAIN_8)))) * (uint32_t) pot[2]) >> 6);
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

    //Serial.begin(9600);
    }

void loop()
    {
    audioHook();
    }
