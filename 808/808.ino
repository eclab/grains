// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License
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


#define SAMPLE_1        "tr808/BD5000.h"             
#define SAMPLE_2        "tr808/CP.h"
#define SAMPLE_3        "tr808/RS.h"             
#define SAMPLE_4        "tr808/CH.h"
#define SAMPLE_5        "tr808/OH00.h"
#define SAMPLE_6        "tr808/SD1050.h"
#define SAMPLE_7        "tr808/HC00.h"          
#define SAMPLE_8        "tr808/CB.h"
#define SAMPLE_9        "tr808/MA.h"

/*
             #define SAMPLE_1        "blank1.h"
             #define SAMPLE_2        "blank2.h"
             #define SAMPLE_3        "blank3.h"
             #define SAMPLE_4        "blank4.h"
             #define SAMPLE_5        "blank5.h"
             #define SAMPLE_6        "blank6.h"
             #define SAMPLE_7        "blank7.h"
             #define SAMPLE_8        "blank8.h"
             #define SAMPLE_9        "blank9.h"
*/

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

#define FORMAT_0        0               // A single sample, with no options, aiming for minimal noise and accurate triggering
#define FORMAT_1        1               // A single sample, plus editable start/end points and pitch control
#define FORMAT_2        2               // Two samples, each with pitch control, plus total volume control
#define FORMAT_3        3               // Three samples, the first two with pitch control, plus total volume control
#define FORMAT_4        4               // Four samples, the first one with pitch control, plus total volume control
#define FORMAT_5        5               // Has 5 triggers: thus not for TRIQ164 or GRAINS BEATS
#define FORMAT_5A       6               // An alternative to FORMAT_5 for people with 4-trigger step sequencers like TRIQ164 or GRAINS BEATS 
#define FORMAT_6        7               // Six samples and four triggers: the last three samples share a trigger
#define FORMAT_7        8               // Seven samples and four triggers: the last four samples share a trigger
#define FORMAT_7A       9               // An alternative to FORMAT_7, with three triggers.  The last three samples share a trigger, and the next three share another
#define FORMAT_8        10               // Eight samples and three triggers.  The last four samples share a trigger, and the next three share another
#define FORMAT_9        11              // Eight samples and three triggers.  The last four samples share a trigger, and the next four share another
#define FORMAT_10       12              // One sample and one trigger, but the sample produces a chord

/// You set the format by changing the following value.  For example, to set to FORMAT_7,
/// you would change below to   #define FORMAT FORMAT_7

#define FORMAT FORMAT_0


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
#define CONTROL_RATE 200
#endif


/// THE FORMATS, and their associated CONFIGURATIONS, are described in detail below:

/// FORMAT_0
/// In this format there is a single sample, SAMPLE_1.  There are no options: the goal is
/// to reduce noise as much as possible.  Additionally, when Drum 1 is played, TRIGGER OUT
/// is output.  This allows you to line up other drums (like KICK) precisely, since Mozzi
/// has some degree of latency.
// POT 3                UNUSED
// POT 2                UNUSED
// POT 1                UNUSED
// GATE OUT             TRIGGER 1
// AUDIO IN             TRIGGER OUT
// IN 3                 UNUSED
// IN 2                 UNUSED
// IN 1                 UNUSED

/// FORMAT_1
/// In this format there is a single sample, SAMPLE_1.  You can change its pitch and
/// start and end points with the pots.
// POT 3                END 1           [If START > END, they are swapped]      [Set to MAN]
// POT 2                START 1         [If START > END, they are swapped]      [Set to MAN]
// POT 1                PITCH 1         [CENTER: Original Pitch]
// GATE OUT             TRIGGER 1
// AUDIO IN             UNUSED
// IN 3                 UNUSED
// IN 2                 UNUSED
// IN 1                 PITCH CV 1

/// FORMAT_2
/// There are two samples, SAMPLE_1 and SAMPLE_2.  You can change the pitch of each
/// one and the total volume with the pots  Set the volume such that when the two 
/// samples play simultaneously you don't have pops or clipping.  Additionally, when Drum 1 is 
/// played, TRIGGER OUT is output.  This allows you to line up other drums (like KICK) 
/// precisely, since 808 has some degree of latency.
// POT 3                VOLUME
// POT 2                PITCH 2         [CENTER: Original Pitch]        [Note GRAINS BUG ABOVE]
// POT 1                PITCH 1         [CENTER: Original Pitch]        [Note GRAINS BUG ABOVE]
// GATE OUT             TRIGGER 1
// AUDIO IN             TRIGGER 2
// IN 3                 UNUSED
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

/// FORMAT_10
/// In this format there is a single sample, SAMPLE_1, but it is played
/// as a chord.  You can change its pitch and the chord type.  Note that the samples in the
/// chord will be different lengths -- you'll need to make sure you envelope them.
/// When the longest (lowest) sample has finished playing, the OUTPUT TRIGGER will be set.
// POT 3                CHORD CHOICE
// POT 2                VOLUME
// POT 1                PITCH				[Unlike other Formats, this pitch is standard note pitch]
// GATE OUT             TRIGGER
// AUDIO IN             PITCH CV TUNE
// IN 3                 OUTPUT TRIGGER
// IN 2                 VOLUME CV
// IN 1                 PITCH CV
//
/// CHORDS
///
/// The chords are, in order:
///
/// No Chord
/// Intervals: minor 3, Major 3, 4, 5, minor 6, Major 6, minor 7, octave, octave + minor 3, octave + major 3, octave + 5
/// Triads: minor, minor first inversion, minor second inversion, Major, Major first inversion, Major second inversion
/// Sevenths: 7, minor 7, Major 7, diminished 7
/// Octave Triads: minor + octave, Major + octave  



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
int16_t previousChord = UNDEFINED;

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


#if (FORMAT == FORMAT_10)
PROGMEM const float frequencies[1024+512] = {
    32.7, 32.81097, 32.922318, 33.034042, 33.146145, 33.25863, 33.37149, 33.48474, 33.59837, 33.71239, 33.826794, 33.94159, 34.056774, 34.172344, 34.28831, 34.40467, 
    34.521423, 34.638577, 34.756123, 34.87407, 34.992416, 35.111168, 35.23032, 35.349876, 35.469837, 35.590206, 35.710987, 35.832172, 35.95377, 36.075783, 36.198208, 36.32105, 
    36.444305, 36.56798, 36.692078, 36.816593, 36.941532, 37.066895, 37.192684, 37.3189, 37.445545, 37.572617, 37.700123, 37.82806, 37.956432, 38.08524, 38.214485, 38.34417, 
    38.47429, 38.60486, 38.735863, 38.867317, 38.999214, 39.131565, 39.26436, 39.397606, 39.531303, 39.665455, 39.80006, 39.935123, 40.070644, 40.20663, 40.34307, 40.47998, 
    40.617348, 40.755188, 40.893494, 41.03227, 41.171513, 41.31123, 41.451424, 41.59209, 41.733234, 41.874863, 42.016968, 42.159554, 42.302628, 42.446182, 42.59022, 42.73476, 
    42.87978, 43.025295, 43.171303, 43.317806, 43.46481, 43.612312, 43.760315, 43.908817, 44.057823, 44.207336, 44.357357, 44.507885, 44.658924, 44.81048, 44.962543, 45.115128, 
    45.26823, 45.421852, 45.575993, 45.73066, 45.885845, 46.04156, 46.197807, 46.354584, 46.511887, 46.66973, 46.828106, 46.987022, 47.146477, 47.30647, 47.467007, 47.62809, 
    47.78972, 47.951893, 48.11462, 48.277905, 48.44174, 48.60613, 48.771072, 48.93658, 49.102654, 49.269283, 49.43648, 49.604248, 49.772583, 49.94149, 50.11097, 50.281025, 
    50.451656, 50.622868, 50.79466, 50.967033, 51.13999, 51.31354, 51.487675, 51.6624, 51.83772, 52.013634, 52.190147, 52.36726, 52.544968, 52.723286, 52.902206, 53.08173, 
    53.261868, 53.44261, 53.623974, 53.80595, 53.988544, 54.171757, 54.355595, 54.540054, 54.725136, 54.91085, 55.097195, 55.28417, 55.47178, 55.660027, 55.848915, 56.038437, 
    56.228607, 56.419422, 56.61089, 56.803, 56.99576, 57.189182, 57.38326, 57.57799, 57.773384, 57.96944, 58.166164, 58.363556, 58.561615, 58.76035, 58.95976, 59.15984, 
    59.360603, 59.562046, 59.764175, 59.966988, 60.17049, 60.374683, 60.579563, 60.785145, 60.991425, 61.198402, 61.406086, 61.614468, 61.82356, 62.033363, 62.243877, 62.455105, 
    62.667053, 62.879715, 63.093098, 63.307213, 63.52205, 63.737614, 63.95391, 64.170944, 64.38871, 64.607216, 64.82647, 65.04646, 65.2672, 65.488686, 65.71092, 65.93392, 
    66.15767, 66.38218, 66.60745, 66.83349, 67.06029, 67.287865, 67.51621, 67.74534, 67.975235, 68.20591, 68.43737, 68.66962, 68.90265, 69.136475, 69.37109, 69.606514, 
    69.84273, 70.07974, 70.31756, 70.55618, 70.79562, 71.03587, 71.27694, 71.518814, 71.76152, 72.00505, 72.249405, 72.49458, 72.7406, 72.98745, 73.23514, 73.483665, 
    73.73304, 73.98325, 74.23433, 74.48624, 74.73901, 74.992645, 75.24714, 75.502495, 75.75872, 76.01581, 76.27377, 76.53261, 76.79233, 77.052925, 77.314415, 77.57678, 
    77.84005, 78.1042, 78.36925, 78.63521, 78.902054, 79.169815, 79.438484, 79.70806, 79.97856, 80.24997, 80.52231, 80.795555, 81.06975, 81.34486, 81.62091, 81.897896, 
    82.17582, 82.45468, 82.734505, 83.01527, 83.29699, 83.57966, 83.8633, 84.14788, 84.43345, 84.71997, 85.00748, 85.29596, 85.58542, 85.875854, 86.167274, 86.459694, 
    86.7531, 87.0475, 87.3429, 87.63931, 87.936714, 88.23513, 88.53457, 88.835014, 89.136475, 89.43897, 89.742485, 90.04703, 90.352615, 90.65923, 90.96689, 91.27559, 
    91.585335, 91.89614, 92.208, 92.52091, 92.83488, 93.14992, 93.466034, 93.78321, 94.10147, 94.420815, 94.741234, 95.062744, 95.38535, 95.709045, 96.03384, 96.35974, 
    96.68674, 97.014854, 97.34407, 97.674416, 98.00588, 98.33848, 98.672195, 99.007034, 99.343025, 99.68015, 100.018425, 100.35785, 100.69841, 101.04014, 101.383026, 101.72707, 
    102.072296, 102.418686, 102.76624, 103.11499, 103.46492, 103.81603, 104.168335, 104.521835, 104.87654, 105.232445, 105.58956, 105.94788, 106.30742, 106.66818, 107.03017, 107.39338, 
    107.75783, 108.12351, 108.49043, 108.858604, 109.22803, 109.598694, 109.97062, 110.34382, 110.71828, 111.094, 111.47101, 111.8493, 112.22886, 112.60971, 112.99186, 113.375305, 
    113.760056, 114.1461, 114.53347, 114.92215, 115.31214, 115.70346, 116.0961, 116.49008, 116.8854, 117.28206, 117.68006, 118.079414, 118.480125, 118.882195, 119.28563, 119.69043, 
    120.09661, 120.504166, 120.9131, 121.323425, 121.73515, 122.14826, 122.56278, 122.97871, 123.39604, 123.8148, 124.23497, 124.65657, 125.0796, 125.50407, 125.92997, 126.35732, 
    126.78612, 127.21638, 127.64809, 128.08127, 128.51593, 128.95204, 129.38965, 129.82875, 130.26933, 130.71141, 131.15498, 131.60007, 132.04666, 132.49478, 132.9444, 133.39555, 
    133.84824, 134.30246, 134.75822, 135.21555, 135.6744, 136.13481, 136.59679, 137.06035, 137.52547, 137.99217, 138.46046, 138.93033, 139.40181, 139.87486, 140.34953, 140.82582, 
    141.30373, 141.78326, 142.2644, 142.7472, 143.2316, 143.71767, 144.2054, 144.69475, 145.18579, 145.67848, 146.17285, 146.6689, 147.16663, 147.66605, 148.16716, 148.66997, 
    149.1745, 149.68073, 150.18869, 150.69836, 151.20976, 151.7229, 152.23778, 152.75441, 153.27278, 153.79292, 154.31483, 154.83852, 155.36397, 155.8912, 156.42023, 156.95105, 
    157.48367, 158.01811, 158.55435, 159.0924, 159.6323, 160.17403, 160.71759, 161.26299, 161.81024, 162.35936, 162.91034, 163.46318, 164.0179, 164.57451, 165.133, 165.69339, 
    166.25568, 166.81989, 167.38599, 167.95403, 168.52399, 169.09589, 169.66972, 170.24551, 170.82324, 171.40294, 171.9846, 172.56825, 173.15387, 173.74149, 174.33109, 174.92268, 
    175.5163, 176.11192, 176.70956, 177.30923, 177.91096, 178.51471, 179.1205, 179.72836, 180.33829, 180.95027, 181.56433, 182.18048, 182.79874, 183.41907, 184.0415, 184.66606, 
    185.29276, 185.92155, 186.55247, 187.18556, 187.82079, 188.45816, 189.09772, 189.73943, 190.38332, 191.0294, 191.67766, 192.32814, 192.9808, 193.6357, 194.29282, 194.95216, 
    195.61374, 196.27757, 196.94365, 197.61198, 198.2826, 198.95547, 199.63065, 200.3081, 200.98785, 201.66992, 202.35431, 203.04102, 203.73004, 204.42142, 205.11514, 205.8112, 
    206.50964, 207.21043, 207.91362, 208.6192, 209.32715, 210.0375, 210.75029, 211.46548, 212.1831, 212.90317, 213.62566, 214.35062, 215.07803, 215.8079, 216.54027, 217.2751, 
    218.01244, 218.75227, 219.49463, 220.2395, 220.9869, 221.73682, 222.4893, 223.24432, 224.00192, 224.7621, 225.52484, 226.29016, 227.0581, 227.82864, 228.60178, 229.37756, 
    230.15598, 230.93703, 231.72072, 232.50706, 233.2961, 234.08781, 234.88219, 235.67928, 236.47908, 237.28157, 238.0868, 238.89478, 239.70547, 240.51894, 241.33514, 242.15413, 
    242.97589, 243.80046, 244.62779, 245.45796, 246.29094, 247.12674, 247.96538, 248.80687, 249.65121, 250.49841, 251.3485, 252.20146, 253.05731, 253.91609, 254.77777, 255.64238, 
    256.50992, 257.3804, 258.2538, 259.13025, 260.0096, 260.89197, 261.7773, 262.66568, 263.55707, 264.45145, 265.34888, 266.24933, 267.1529, 268.05948, 268.96915, 269.88193, 
    270.7978, 271.71674, 272.63882, 273.56406, 274.4924, 275.42392, 276.3586, 277.29642, 278.23743, 279.18167, 280.1291, 281.0797, 282.03357, 282.9907, 283.95102, 284.91464, 
    285.8815, 286.85165, 287.82513, 288.80188, 289.78192, 290.76532, 291.75204, 292.74213, 293.7356, 294.7324, 295.7326, 296.73615, 297.74316, 298.75357, 299.76743, 300.78467, 
    301.8054, 302.82962, 303.8573, 304.88843, 305.9231, 306.96127, 308.00296, 309.0482, 310.097, 311.1493, 312.20517, 313.2647, 314.32776, 315.39444, 316.46475, 317.53873, 
    318.61627, 319.69754, 320.78244, 321.87106, 322.96335, 324.05936, 325.15906, 326.26248, 327.3697, 328.48065, 329.59537, 330.71384, 331.83618, 332.96228, 334.0922, 335.22595, 
    336.36356, 337.50504, 338.65036, 339.79962, 340.95273, 342.10977, 343.27072, 344.43567, 345.60455, 346.77734, 347.95416, 349.13498, 350.31976, 351.5086, 352.70148, 353.89838, 
    355.09937, 356.3044, 357.51355, 358.7268, 359.94415, 361.16565, 362.3913, 363.6211, 364.85507, 366.0932, 367.33557, 368.58215, 369.83298, 371.088, 372.34732, 373.6109, 
    374.87878, 376.15094, 377.42746, 378.70825, 379.99344, 381.28296, 382.57687, 383.87518, 385.17786, 386.485, 387.79654, 389.11258, 390.43304, 391.758, 393.08746, 394.42142, 
    395.75992, 397.10294, 398.45056, 399.8027, 401.1595, 402.52084, 403.8868, 405.25742, 406.6327, 408.01263, 409.39722, 410.78656, 412.1806, 413.57935, 414.98285, 416.3911, 
    417.80417, 419.22202, 420.64468, 422.07214, 423.5045, 424.94168, 426.38376, 427.83072, 429.28256, 430.73938, 432.20108, 433.66782, 435.13947, 436.61615, 438.09784, 439.58453, 
    441.07632, 442.57312, 444.075, 445.582, 447.09412, 448.61136, 450.13376, 451.66132, 453.19406, 454.73203, 456.27515, 457.82355, 459.3772, 460.93616, 462.50037, 464.0699, 
    465.6447, 467.2249, 468.8105, 470.4014, 471.99774, 473.5995, 475.2067, 476.81934, 478.43744, 480.06107, 481.6902, 483.3248, 484.96503, 486.61075, 488.26212, 489.91907, 
    491.58163, 493.24985, 494.9237, 496.60327, 498.28854, 499.9795, 501.6762, 503.37866, 505.0869, 506.80096, 508.5208, 510.24652, 511.97806, 513.7155, 515.4588, 517.20807, 
    518.96326, 520.72437, 522.49146, 524.2646, 526.0437, 527.82886, 529.6201, 531.4174, 533.2208, 535.0303, 536.84595, 538.6678, 540.4958, 542.32996, 544.1704, 546.01715, 
    547.87, 549.7293, 551.59485, 553.4667, 555.3449, 557.2295, 559.1205, 561.01794, 562.9218, 564.8321, 566.7489, 568.6721, 570.602, 572.5383, 574.4813, 576.43085, 
    578.387, 580.3498, 582.3193, 584.2954, 586.2782, 588.26776, 590.2641, 592.2672, 594.2771, 596.2938, 598.3174, 600.3478, 602.3851, 604.4293, 606.4805, 608.5387, 
    610.60376, 612.6759, 614.75507, 616.84125, 618.9346, 621.0349, 623.14246, 625.2571, 627.3789, 629.508, 631.6443, 633.7878, 635.93866, 638.09674, 640.26215, 642.4349, 
    644.61505, 646.8026, 648.99756, 651.19995, 653.40985, 655.62726, 657.8521, 660.0846, 662.32465, 664.57227, 666.8275, 669.09045, 671.361, 673.63934, 675.9254, 678.21924, 
    680.5208, 682.8302, 685.1474, 687.4725, 689.8054, 692.14636, 694.4952, 696.85205, 699.2168, 701.58966, 703.9705, 706.3595, 708.7566, 711.16174, 713.5752, 715.9967, 
    718.42645, 720.86456, 723.31085, 725.7654, 728.22833, 730.69965, 733.1793, 735.6674, 738.16394, 740.66895, 743.18243, 745.7045, 748.23505, 750.7743, 753.322, 755.87854, 
    758.44366, 761.01746, 763.6, 766.19135, 768.79144, 771.4004, 774.0182, 776.64484, 779.28046, 781.925, 784.5785, 787.241, 789.9126, 792.5932, 795.2829, 797.98175, 
    800.6897, 803.4069, 806.13336, 808.869, 811.61395, 814.3682, 817.13184, 819.90485, 822.68726, 825.47906, 828.28033, 831.0912, 833.9115, 836.74146, 839.581, 842.4302, 
    845.289, 848.15753, 851.0358, 853.9238, 856.8217, 859.7294, 862.647, 865.57434, 868.5118, 871.4591, 874.4165, 877.38385, 880.3613, 883.3488, 886.34656, 889.35443, 
    892.3725, 895.4008, 898.4394, 901.4883, 904.54755, 907.61725, 910.69727, 913.7878, 916.8888, 920.0003, 923.1224, 926.25507, 929.3983, 932.5523, 935.717, 938.8924, 
    942.0786, 945.27563, 948.48346, 951.7022, 954.9319, 958.1725, 961.42413, 964.68677, 967.96045, 971.2453, 974.54126, 977.84845, 981.1668, 984.4965, 987.8374, 991.1897, 
    994.55334, 997.92847, 1001.31494, 1004.713, 1008.12256, 1011.5437, 1014.97644, 1018.42084, 1021.8769, 1025.3447, 1028.8242, 1032.3157, 1035.8188, 1039.334, 1042.861, 1046.4, 
    1049.951, 1053.5142, 1057.0894, 1060.6766, 1064.2761, 1067.8877, 1071.5117, 1075.1478, 1078.7965, 1082.4574, 1086.1309, 1089.8168, 1093.515, 1097.226, 1100.9495, 1104.6855, 
    1108.4344, 1112.1959, 1115.9702, 1119.7573, 1123.5574, 1127.3702, 1131.196, 1135.0348, 1138.8866, 1142.7516, 1146.6295, 1150.5206, 1154.425, 1158.3427, 1162.2736, 1166.2178, 
    1170.1754, 1174.1465, 1178.131, 1182.129, 1186.1406, 1190.1659, 1194.2048, 1198.2574, 1202.3237, 1206.4039, 1210.4979, 1214.6058, 1218.7277, 1222.8635, 1227.0134, 1231.1772, 
    1235.3555, 1239.5476, 1243.7542, 1247.9749, 1252.2101, 1256.4595, 1260.7234, 1265.0017, 1269.2946, 1273.6019, 1277.924, 1282.2606, 1286.6122, 1290.9783, 1295.3594, 1299.7551, 
    1304.166, 1308.5918, 1313.0326, 1317.4884, 1321.9594, 1326.4456, 1330.9469, 1335.4635, 1339.9956, 1344.543, 1349.1057, 1353.6841, 1358.2778, 1362.8871, 1367.5123, 1372.153, 
    1376.8094, 1381.4817, 1386.1698, 1390.8739, 1395.594, 1400.3301, 1405.0822, 1409.8503, 1414.6348, 1419.4354, 1424.2523, 1429.0856, 1433.9353, 1438.8014, 1443.6841, 1448.5834, 
    1453.4993, 1458.4318, 1463.3811, 1468.347, 1473.33, 1478.3298, 1483.3467, 1488.3804, 1493.4314, 1498.4994, 1503.5847, 1508.6873, 1513.807, 1518.9442, 1524.0989, 1529.271, 
    1534.4606, 1539.6678, 1544.893, 1550.1356, 1555.3961, 1560.6743, 1565.9706, 1571.2849, 1576.6171, 1581.9674, 1587.3359, 1592.7227, 1598.1277, 1603.551, 1608.9928, 1614.453, 
    1619.9318, 1625.4291, 1630.9451, 1636.4797, 1642.0333, 1647.6056, 1653.1968, 1658.807, 1664.4363, 1670.0847, 1675.7523, 1681.439, 1687.1451, 1692.8706, 1698.6154, 1704.3798, 
    1710.1636, 1715.9672, 1721.7904, 1727.6334, 1733.4962, 1739.379, 1745.2817, 1751.2043, 1757.1472, 1763.1102, 1769.0934, 1775.0969, 1781.1208, 1787.1653, 1793.23, 1799.3154, 
    1805.4215, 1811.5485, 1817.696, 1823.8644, 1830.0538, 1836.2643, 1842.4957, 1848.7483, 1855.0221, 1861.3173, 1867.6338, 1873.9717, 1880.3312, 1886.7123, 1893.1149, 1899.5393, 
    1905.9855, 1912.4536, 1918.9436, 1925.4557, 1931.9899, 1938.546, 1945.1246, 1951.7256, 1958.3489, 1964.9948, 1971.663, 1978.3539, 1985.0676, 1991.8041, 1998.5634, 2005.3457, 
    2012.1509, 2018.9791, 2025.8308, 2032.7056, 2039.6036, 2046.5251, 2053.4702, 2060.4387, 2067.431, 2074.447, 2081.4868, 2088.5503, 2095.638, 2102.7495, 2109.8855, 2117.0454, 
    2124.2297, 2131.4385, 2138.6716, 2145.9292, 2153.2117, 2160.5188, 2167.8508, 2175.2075, 2182.589, 2189.9958, 2197.4277, 2204.8848, 2212.3672, 2219.875, 2227.4084, 2234.9673, 
    2242.5518, 2250.1619, 2257.7979, 2265.46, 2273.148, 2280.862, 2288.602, 2296.3687, 2304.1616, 2311.981, 2319.8267, 2327.6992, 2335.5984, 2343.5244, 2351.4773, 2359.4573, 
    2367.464, 2375.4985, 2383.5596, 2391.6484, 2399.7646, 2407.9084, 2416.0798, 2424.279, 2432.5059, 2440.7607, 2449.0435, 2457.3545, 2465.6936, 2474.0613, 2482.457, 2490.8816, 
    2499.3345, 2507.816, 2516.3267, 2524.8657, 2533.434, 2542.0315, 2550.658, 2559.314, 2567.999, 2576.7139, 2585.4578, 2594.232, 2603.0354, 2611.8691, 2620.7327, 2629.6262, 
    2638.5498, 2647.5042, 2656.4885, 2665.5037, 2674.549, 2683.6255, 2692.7322, 2701.8704, 2711.039, 2720.2393, 2729.4707, 2738.7334, 2748.0273, 2757.3528, 2766.7102, 2776.099, 
    2785.52, 2794.973, 2804.458, 2813.9749, 2823.5242, 2833.1062, 2842.7205, 2852.3672, 2862.047, 2871.7595, 2881.505, 2891.2837, 2901.0955, 2910.9404, 2920.8188, 2930.7307, 
    2940.6765, 2950.656, 2960.6692, 2970.716, 2980.7974, 2990.913, 3001.0627, 3011.247, 3021.466, 3031.7195, 3042.0078, 3052.3313, 3062.6895, 3073.0828, 3083.5117, 3093.9756, 
    3104.4753, 3115.0103, 3125.5813, 3136.1882, 3146.8313, 3157.5103, 3168.225, 3178.9768, 3189.765, 3200.5896, 3211.4512, 3222.349, 3233.2844, 3244.2568, 3255.2664, 3266.3135, 
    3277.398, 3288.5198, 3299.6797, 3310.8774, 3322.113, 3333.3867, 3344.6987, 3356.0493, 3367.4382, 3378.866, 3390.3323, 3401.8374, 3413.3818, 3424.9653, 3436.5881, 3448.2505, 
    3459.9524, 3471.6938, 3483.4753, 3495.2969, 3507.1582, 3519.0598, 3531.0022, 3542.9849, 3555.008, 3567.0723, 3579.1775, 3591.3235, 3603.5107, 3615.7395, 3628.0098, 3640.3218, 
    3652.6753, 3665.071, 3677.5088, 3689.9885, 3702.5107, 3715.0752, 3727.6826, 3740.3328, 3753.026, 3765.762, 3778.5413, 3791.364, 3804.2302, 3817.1401, 3830.0938, 3843.0916, 
    3856.1333, 3869.2192, 3882.3496, 3895.525, 3908.7444, 3922.009, 3935.3186, 3948.6733, 3962.0735, 3975.519, 3989.0103, 4002.547, 4016.1301, 4029.759, 4043.4343, 4057.1558, 
    4070.924, 4084.7388, 4098.6006, 4112.51, 4126.4653, 4140.4688, 4154.52, 4168.6187, 4182.765, 4196.9595, 4211.202, 4225.493, 4239.833, 4254.2207, 4268.6577, 4283.1436, 
    4297.6787, 4312.263, 4326.8975, 4341.5806, 4356.314, 4371.097, 4385.931, 4400.815, 4415.7495, 4430.735, 4445.7705, 4460.858, 4475.9956, 4491.185, 4506.4263, 4521.719, 
    4537.0645, 4552.461, 4567.91, 4583.411, 4598.9653, 4614.5728, 4630.232, 4645.9453, 4661.7114, 4677.5313, 4693.405, 4709.332, 4725.3135, 4741.349, 4757.439, 4773.584, 
    4789.783, 4806.038, 4822.3477, 4838.7124, 4855.133, 4871.609, 4888.141, 4904.729, 4921.3735, 4938.0747, 4954.8325, 4971.647, 4988.5186, 5005.4473, 5022.4336, 5039.4775, 
    5056.5796, 5073.7393, 5090.957, 5108.2334, 5125.569, 5142.963, 5160.4155, 5177.9277, 5195.4995, 5213.131, 5230.822, 5248.5728, 5266.3843, 5284.256, 5302.1885, 5320.1816, 
    5338.2363, 5356.3516, 5374.529, 5392.7676, 5411.0684, 5429.431, 5447.8564, 5466.3438, 5484.894, 5503.5073, 5522.184, 5540.924, 5559.7275, 5578.5947, 5597.526, 5616.5215, 
    5635.5815, 5654.706, 5673.8955, 5693.151, 5712.4707, 5731.856, 5751.3076, 5770.825, 5790.4087, 5810.0586, 5829.7754, 5849.5596, 5869.41, 5889.328, 5909.314, 5929.368, 
    };

#define FREQUENCY(pitch) pgm_read_float_near(&frequencies[pitch])


// These are the semitone values for the chords.  
// The first number is the number of additional notes beyond the first
const uint8_t chords[24][4] = 
    { 
    {0, 0, 0, 0},                           // None
    {1, 3, 0, 0},                           // m3
    {1, 4, 0, 0},                           // M3
    {1, 5, 0, 0},                           // 4
    {1, 7, 0, 0},                           // 5
    {1, 8, 0, 0},                           // m6
    {1, 9, 0, 0},                           // M6
    {1, 10, 0, 0},                  // m7
    {1, 12, 0, 0},                  // Octave
    {1, 15, 0, 0},                  // Octave + m3
    {1, 16, 0, 0},                  // Octave + M3
    {1, 19, 0, 0},                  // Octave + 5
    {2, 3, 7, 0},                   // min
    {2, 4, 9, 0},                   // min-1
    {2, 5, 8, 0},                   // min-2
    {2, 4, 7, 0},                   // Maj
    {2, 3, 8, 0},                   // Maj-1
    {2, 5, 9, 0},                   // Maj-2
    {3, 4, 7, 10},          // 7
    {3, 3, 7, 10},          // min7
    {3, 4, 7, 11},          // Maj7
    {3, 3, 6, 9},           // dim7
    {3, 3, 7, 12},          // min+Oct
    {3, 4, 7, 12},          // Maj+Oct
    };
                

// Relative frequency ratios for each semitone
const float semitoneFrequencyRatios[] = 
    {
    1.0,
    1.0594630943592953,
    1.1224620483093730,
    1.1892071150027210,
    1.2599210498948732,
    1.3348398541700344,
    1.4142135623730951,
    1.4983070768766815,
    1.5874010519681994,
    1.6817928305074290,
    1.7817974362806785,
    1.8877486253633868,
    2.0,
    2.1189261887185906,
    2.2449240966187460,
    2.3784142300054420,
    2.5198420997897464,
    2.6696797083400687,
    2.8284271247461903,
    2.9966141537533630,
    3.1748021039363990,
    3.3635856610148580,
    3.5635948725613570,
    3.7754972507267740,
    4.0
    };
    
#define MEDIAN_OF_THREE(a,b,c) (((a) <= (b)) ? (((b) <= (c)) ? (b) : (((a) < (c)) ? (c) : (a))) : (((a) <= (c)) ? (a) : (((b) < (c)) ? (c) : (b))))
uint16_t pitchCV;
uint16_t tuneCV;
uint16_t pA;
uint16_t pB;
void initializeFrequency(uint8_t pitch, uint8_t tune)
    {
    pitchCV = mozziAnalogRead(pitch);
    tuneCV = mozziAnalogRead(tune);
    }
        
#define TRANSPOSE_BITS 0
#define TRANSPOSE_SEMITONES 0
#define TRANSPOSE_OCTAVES 0
#define LARGE_JUMP 32
#define SEMITONE 17
#define FREQ_COUNTER_MAX 4
uint8_t freqCounter = 0;
inline float getFrequency(uint8_t pitch, uint8_t tune)
    {
    tuneCV = (tuneCV * 15 + mozziAnalogRead(tune)) >> 4;
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
    int16_t finalPitch = pitchCV + (tuneCV >> 1) + TRANSPOSE_SEMITONES * 17 + TRANSPOSE_OCTAVES * 205 + TRANSPOSE_BITS;
    return FREQUENCY(finalPitch < 0 ? 0 : (finalPitch > 1535 ? 1535 : finalPitch));
    }

#endif


#include SAMPLE_1
Sample<DATA_LENGTH, 16384> sample0(DATA);
uint16_t lengths0 = DATA_LENGTH;
#if (FORMAT == FORMAT_10)
Sample<DATA_LENGTH, 16384> sample0a(DATA);
Sample<DATA_LENGTH, 16384> sample0b(DATA);
Sample<DATA_LENGTH, 16384> sample0c(DATA);
#endif
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

#define TRIGGER_OUT_COUNTDOWN 2
uint8_t triggerOutCountdown = 0;


inline void updateControl0()
    {
      /*
      if (triggerOutCountdown > 0)
        {
        if (triggerOutCountdown == 1)
          {
            digitalWrite(CV_AUDIO_IN, 0);
          }
        triggerOutCountdown--;
        }
       */
       
    // POT 3            UNUSED
    // POT 2            UNUSED
    // POT 1            UNUSED
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER OUT
    // IN 3             UNUSED
    // IN 2             UNUSED
    // IN 1             UNUSED
       
    uint8_t val = digitalRead(CV_GATE_OUT);
    if (!triggered[0] && val) 
        { 
        // don't bother with the countdown in this situation, only one drum
        sample0.start(); 
        //digitalWrite(CV_AUDIO_IN, 1);
        //triggerOutCountdown = TRIGGER_OUT_COUNTDOWN;
        }
    triggered[0] = val;
    }
    
inline void updateControl1()
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
 

inline void updateControl2()
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



inline void updateControl3()
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

inline void updateControl4()
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

inline void updateControl5()
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

inline void updateControl5A()
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

inline void updateControl6()
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

inline void updateControl7()
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

inline void updateControl7A()
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

inline void updateControl8()
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

inline void updateControl9()
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


#if (FORMAT == FORMAT_10)
uint8_t finalGain = 0;
uint8_t lastChord = 255;
uint8_t completed = false;
inline void updateControl10()
    {       
    // POT 3            CHORD CHOICE
    // POT 2            VOLUME
    // POT 1            PITCH 1
    // GATE OUT         TRIGGER 1
    // AUDIO IN         PITCH CV TUNE
    // IN 3             OUTPUT TRIGGER
    // IN 2             VOLUME CV
    // IN 1             PITCH CV

    float frequency = getFrequency(CV_POT_IN1, CV_AUDIO_IN);

    if (previousPitch1 == UNDEFINED) previousPitch1 = pot[0] = mozziAnalogRead(CV_POT_IN1);
    else pot[0] = previousPitch1 = (previousPitch1 * 7 + mozziAnalogRead(CV_POT_IN1)) >> 3;
    if (previousVolume == UNDEFINED) previousVolume = pot[1] = mozziAnalogRead(CV_POT_IN2);
    else pot[1] = previousVolume = (previousVolume * 7 + mozziAnalogRead(CV_POT_IN2)) >> 3;
    pot[3] = (mozziAnalogRead(CV_POT3) * 6) >> 8;		// 0...23
    finalGain = pot[1] >>  4;			// 0...63
    
    uint8_t val = digitalRead(CV_GATE_OUT);
    if (!triggered[0] && val) 
        { 
        if (counter == 0) counter = COUNTDOWN;
        }
    triggered[0] = val;

    // Compute Chord
    if (pot[3] != lastChord)
    	{
    	lastChord = pot[3];
    	uint8_t* c = chords[pot[3]];
		sample0.setFreq(frequency);
		if (c[0] > 0)
			{
			sample0a.setFreq(frequency * semitoneFrequencyRatios[c[1]]);
			if (c[0] > 1)
				{
				sample0b.setFreq(frequency * semitoneFrequencyRatios[c[2]]);
				if (c[0] > 2)
					{
					sample0c.setFreq(frequency * semitoneFrequencyRatios[c[3]]);
					}
				}
			}
		}

    if (counter == 1)
        {
        sample0.start(); 
        sample0a.start(); 
        sample0b.start(); 
        sample0c.start();
        completed = false;
        triggerOutCountdown = 0;
       }
    if (counter > 0) counter--;
    
    // at present we trigger if the LONGEST sound is done playing.  Maybe that's not a good choice?
    if (!completed && !sample0.isPlaying())
    	{
    	completed = true;
    	triggerOutCountdown = TRIGGER_OUT_COUNTDOWN;
        digitalWrite(CV_IN3, 1);
    	}
    else if (triggerOutCountdown > 0)
        {
        if (triggerOutCountdown == 1)
          {
            digitalWrite(CV_IN3, 0);
          }
        triggerOutCountdown--;
        }
        
    }
#endif







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
#if (FORMAT == FORMAT_5 || FORMAT == FORMAT_10)
inline int16_t C(int8_t value, uint8_t volume) { return (__ULAW2[value + 128]); }                      // too expensive
#else
inline int16_t C(int8_t value, uint8_t volume) { return ((__ULAW2[value + 128]) * volume) >> 3; } 
#endif
#else
#if (FORMAT == FORMAT_5 || FORMAT == FORMAT_10)
inline int16_t C(int8_t value, uint8_t volume) { return ((value) * 32); }
#else
inline int16_t C(int8_t value, uint8_t volume) { return ((value * volume) * 32) >> 3; }
#endif
#endif
#define CC(sample, volume) (sample.isPlaying() * C(sample.next(), volume) )

uint16_t sum;



/** Maps -128 ... +127 to -168 ... +167 */ 
inline int16_t scaleAudioSmall(int16_t val)
	{
	return (val * 21) >> 4;
	}
	
/** Maps -128 ... +127 to -244 ... +170 */ 
inline int16_t scaleAudioSmallBiased(int16_t val)
	{
	return ((val * 13) >> 3) - 36;
	}

/** Maps -32768 ... +32767 to -168 ... +167 */ 
inline int16_t scaleAudio(int16_t val)
	{
	return ((val >> 5) * 21) >> 7;
	}
	
/** Maps -32768 ... +32767 to -244 ... +171 */ 
inline int16_t scaleAudioBiased(int16_t val)
	{
	return (((val >> 4) * 13) >> 7) - 36;
	}



inline int updateAudio0()
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
   sum += CC(sample1, GAIN_1);             // sum is to outwit the compiler
   sum += CC(sample2, GAIN_2);
   sum += CC(sample3, GAIN_3);
   
    return scaleAudio(CC(sample0, GAIN_0) * 8);
    }

inline int updateAudio1()
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
   sum += CC(sample1, GAIN_1);             // sum is to outwit the compiler
   sum += CC(sample2, GAIN_2);
   sum += CC(sample3, GAIN_3);
   
    return scaleAudio(CC(sample0, GAIN_0) * 8);
    }
        
inline int updateAudio2()
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
    sum += CC(sample2, GAIN_2);             // sum is to outwit the compiler
    return scaleAudio(((CC(sample0, GAIN_0) + CC(sample1, GAIN_1)) * (uint32_t) pot[2]) >> 6); 
    }

inline int updateAudio3()
    {
    // POT 3            VOLUME
    // POT 2            PITCH 2
    // POT 1            PITCH 1
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             PITCH CV 2
    // IN 1             PITCH CV 1 

    return scaleAudio(((CC(sample0, GAIN_0) + CC(sample1, GAIN_1) + CC(sample2, GAIN_2)) * (uint32_t) pot[2]) >> 6);
    }

inline int updateAudio4()
    {
    // POT 3            VOLUME
    // POT 2            UNUSED
    // POT 1            PITCH 1
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             TRIGGER 4
    // IN 1             PITCH CV 1 

    return scaleAudio(((CC(sample0, GAIN_0) + CC(sample1, GAIN_1) + CC(sample2, GAIN_2) + CC(sample3, GAIN_3)) * (uint32_t) pot[2]) >> 6);
    }

inline int updateAudio5()
    {
    // POT 3            VOLUME
    // POT 2            UNUSED
    // POT 1            UNUSED
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             TRIGGER 4
    // IN 1             TRIGGER 5 

    return scaleAudio((((CC(sample0, GAIN_0) + CC(sample1, GAIN_1) + CC(sample2, GAIN_2) + CC(sample3, GAIN_3) + CC(sample4, GAIN_4))) * (uint32_t) pot[2]) >> 6);
    }

inline int updateAudio5A()
    {
    // POT 3            VOLUME
    // POT 2            UNUSED
    // POT 1            SAMPLES 4 -> { 4, 5 }
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             TRIGGER 4
    // IN 1             UNUSED 

    return scaleAudio(((CC(sample0, GAIN_0) + CC(sample1, GAIN_1) + CC(sample2, GAIN_2) + (pot[0] < 512 ? CC(sample3, GAIN_3) : CC(sample4, GAIN_4))) * (uint32_t) pot[2]) >> 6);
    }

inline int updateAudio6()
    {
    // POT 3            VOLUME
    // POT 2            UNUSED
    // POT 1            SAMPLES 4 -> { 4, 5, 6 }
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             TRIGGER 4
    // IN 1             UNUSED 

    return scaleAudio(((CC(sample0, GAIN_0) + CC(sample1, GAIN_1) + CC(sample2, GAIN_2) + (pot[0] < 341 ? CC(sample3, GAIN_3) : (pot[0] < 682 ? CC(sample4, GAIN_4) : CC(sample5, GAIN_5)))) * (uint32_t) pot[2]) >> 6);
    }

inline int updateAudio7()
    {
    // POT 3            VOLUME
    // POT 2            UNUSED
    // POT 1            SAMPLES 4 -> { 4, 5, 6, 7}
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             TRIGGER 4
    // IN 1             UNUSED 

    return scaleAudio(((CC(sample0, GAIN_0) + CC(sample1, GAIN_1) + CC(sample2, GAIN_2) + 
                            (pot[0] < 512 ? 
                            (pot[0] < 256 ? CC(sample3, GAIN_3) : CC(sample4, GAIN_4)) :
                            (pot[0] < 768 ? CC(sample5, GAIN_5) : CC(sample6, GAIN_6)))) * (uint32_t) pot[2]) >> 6);
    }

inline int updateAudio7A()
    {
    // POT 3            VOLUME
    // POT 2            SAMPLES 2 -> { 2, 3, 4 }
    // POT 1            SAMPLES 3 -> { 5, 6, 7 }
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             UNUSED
    // IN 1             UNUSED 

    return scaleAudio(((CC(sample0, GAIN_0) + (pot[1] < 341 ? CC(sample1, GAIN_1) : (pot[1] < 682 ? CC(sample2, GAIN_2) : CC(sample3, GAIN_3))) + (pot[0] < 341 ? CC(sample4, GAIN_4) : (pot[0] < 682 ? CC(sample5, GAIN_5) : CC(sample6, GAIN_6)))) * (uint32_t) pot[2]) >> 6);
    }

inline int updateAudio8()
    {
    // POT 3            VOLUME
    // POT 2            SAMPLES 2 -> { 2, 3, 4 }
    // POT 1            SAMPLES 3 -> { 5, 6, 7, 8 }
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             UNUSED
    // IN 1             UNUSED 
        
    return scaleAudio(((CC(sample0, GAIN_0) + 
                (pot[1] < 341 ? CC(sample1, GAIN_1) : (pot[1] < 682 ? CC(sample2, GAIN_2) : CC(sample3, GAIN_3))) + 
                            (pot[0] < 512 ?
                            (pot[0] < 256 ? CC(sample4, GAIN_4) : CC(sample5, GAIN_5)) :
                            (pot[0] < 768 ? CC(sample6, GAIN_6) : CC(sample7, GAIN_7)))) * (uint32_t) pot[2]) >> 6);
    }

inline int updateAudio9()
    {
    // POT 3            VOLUME
    // POT 2            SAMPLES 2 -> { 2, 3, 4, 5 }
    // POT 1            SAMPLES 3 -> { 6, 7, 8, 9 }
    // GATE OUT         TRIGGER 1
    // AUDIO IN         TRIGGER 2
    // IN 3             TRIGGER 3
    // IN 2             UNUSED
    // IN 1             UNUSED 
        
    return scaleAudio(((CC(sample0, GAIN_0) + 
                            (pot[1] < 512 ?
                            (pot[1] < 256 ? CC(sample1, GAIN_1) : CC(sample2, GAIN_2)) :
                            (pot[1] < 768 ? CC(sample3, GAIN_3) : CC(sample4, GAIN_4))) + 
                            (pot[0] < 512 ? 
                            (pot[0] < 256 ? CC(sample5, GAIN_5) : CC(sample6, GAIN_6)) : 
                            (pot[0] < 768 ? CC(sample7, GAIN_7) : CC(sample8, GAIN_8)))) * (uint32_t) pot[2]) >> 6);
    }

#if (FORMAT == FORMAT_10)
inline int updateAudio10()
    {
    // POT 3            CHORD CHOICE
    // POT 2            VOLUME
    // POT 1            PITCH 1
    // GATE OUT         TRIGGER 1
    // AUDIO IN         PITCH CV TUNE
    // IN 3             OUTPUT TRIGGER
    // IN 2             VOLUME CV
    // IN 1             PITCH CV
    
    uint8_t numNotes = chords[previousChord][0];
    
    int16_t out = CC(sample0, GAIN_0);
    if (numNotes > 0) 
    	{
    	out += CC(sample0a, GAIN_0);
	    if (numNotes > 1) 
	    	{
	    	out += CC(sample0b, GAIN_0);
	    	if (numNotes > 2)
	    		{
	    		out += CC(sample0c, GAIN_0);
	    		}
	    	}
	    }
    
    return scaleAudio(out * finalGain);
    }
#endif



void updateControl()
    {
#if (FORMAT == FORMAT_0)
    updateControl0();
#elif (FORMAT == FORMAT_1)
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
#elif (FORMAT == FORMAT_10)
    updateControl10();
#endif
    }


int updateAudio()
    {
#if (FORMAT == FORMAT_0)
    return updateAudio0();
#elif (FORMAT == FORMAT_1)
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
#elif (FORMAT == FORMAT_10)
    return updateAudio9();
#endif
    }
        
void setup()
    {
#if (FORMAT == FORMAT_10)
    initializeFrequency(CV_POT_IN1, CV_AUDIO_IN);
#endif 
   
    pinMode(CV_GATE_OUT, INPUT);
    if (FORMAT == FORMAT_0)
      pinMode(CV_AUDIO_IN, OUTPUT);

    inverseLengths0 = 1.0 / lengths0;
    inverseLengths1 = 1.0 / lengths1;

    sample0.setFreq(16384 / (float) lengths0);
#if (FORMAT == FORMAT_10)
    sample0a.setFreq(16384 / (float) lengths0);
    sample0b.setFreq(16384 / (float) lengths0);
    sample0c.setFreq(16384 / (float) lengths0);
    pinMode(CV_IN3, OUTPUT);
#endif
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
