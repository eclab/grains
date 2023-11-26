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


/// BOOKER
///
/// Booker is a Hammond Organ simulator, complete with Leslie.  It is meant to run on the 
/// AE Modular GRAINS, but it could be  adapted to any Arduino.  
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.
///
/// ALIASING: At extremely high frequencies, a few of Booker's drawbars will go over Nyquist and
/// you may hear some antialiasing effects.  I can fix this but it puts me over the
/// computational budget of the device, so it's not going to happen.
///
/// THE LESLIE.  Booker comes with a Leslie, which is on by default:

#define LESLIE_ON               // To turn this off, put a // before the #define, as in //#define

// Leslie changes both the amplitude and pitch at a certain rate.
// You can adjust them here

#define LESLIE_FREQUENCY 5.66                   // This is the 450 speed.  The classic slower speed is 0.66, but it's too slow
#define LESLIE_VOLUME 3                                 // Values are 0 (min), 1, 2, 3, 4, 5, 6, 7, 8, or 9 (max).
#define LESLIE_PITCH 45                                 // Values are 1.0 (lots) ... 60.0 (little).  Can be floating point.

/// CONFIGURATION
///
/// IN 1            [Unused]
/// IN 2            [Unused]
/// IN 3            Pitch CV    [This isn't IN2, like Pitch Tune, because Pitch Tune must be +/-]
/// AUDIO IN (A)    [Unused]
/// AUDIO OUT       Out
/// DIGITAL OUT (D) [Unused]
///
/// POT 1           Choice of Organ [Set the switch to MAN]
///
/// POT 2           Pitch Tune  [Set the switch to MAN]
///
/// POT 3           Volume

/// Lastly, here is our present drawbar selection.  Keep this to no more than 31 but feel free
/// to change the selections.  There is a big list of 99 selections below you could swap in.

#define NUM_DRAWBAR_SELECTIONS 16
int8_t drawbars[NUM_DRAWBAR_SELECTIONS][9] = 
    {
    { 8, 8, 8, 8, 8, 8, 8, 8, 8 },    // 888888888 Full Organ
    { 8, 8, 5, 3, 2, 4, 5, 8, 8 },    // 885324588 Blues
    { 8, 8, 8, 8, 0, 0, 0, 0, 0 },    // 888800000 Booker T. Jones 1
    { 8, 8, 8, 6, 3, 0, 0, 0, 0 },    // 888630000 Booker T. Jones 2
    { 8, 7, 8, 0, 0, 0, 4, 5, 6 },    // 878000456 Bright Comping
    { 8, 4, 3, 0, 0, 0, 0, 0, 0 },    // 843000000 Dark Comping
    { 8, 0, 8, 8, 0, 8, 0, 0, 8 },    // 808808008 Gospel 1
    { 8, 8, 8, 0, 0, 0, 0, 0, 8 },    // 888000008 Gospel 2
    { 8, 6, 8, 6, 6, 6, 5, 6, 8 },    // 868666568 Greg Allman 1
    { 8, 8, 8, 6, 0, 0, 0, 0, 0 },    // 888600000 Greg Allman 2
    { 8, 8, 6, 8, 0, 0, 3, 0, 0 },    // 886800300 Paul Shaffer 1
    { 8, 8, 8, 7, 6, 8, 8, 8, 8 },    // 888768888 Paul Shaffer 2
    { 8, 8, 8, 8, 7, 8, 6, 7, 8 },    // 888878678 Paul Shaffer 3
    { 8, 0, 8, 0, 0, 0, 0, 0, 8 },    // 808000008 Reggae
    { 0, 8, 0, 0, 0, 0, 0, 0, 0 },    // 080000000 Sine
    { 8, 7, 6, 5, 4, 3, 2, 1, 1 },    // 876543211 Strings
    };

/** Here is a collection of Drawbar Selections for you.
    You can find more online.
        
    { 0, 0, 7, 7, 4, 0, 0, 3, 4 },    // 007740034 Alone in the City
    { 8, 8, 7, 7, 2, 4, 1, 1, 0 },    // 887724110 America (Gospel) (U)
    { 0, 0, 6, 6, 0, 6, 0, 0, 0 },    // 006606000 America (Gospel) (L)
    { 8, 8, 5, 3, 2, 4, 5, 8, 8 },    // 885324588 Blues
    { 8, 8, 8, 8, 0, 0, 0, 0, 0 },    // 888800000 Booker T. Jones 1
    { 8, 8, 8, 6, 3, 0, 0, 0, 0 },    // 888630000 Booker T. Jones 2
    { 8, 8, 8, 8, 0, 8, 0, 0, 8 },    // 888808008 Born to B3 (Gospel) (U)
    { 0, 0, 7, 7, 2, 5, 4, 0, 0 },    // 007725400 Born to B3 (Gospel) (L)
    { 8, 8, 8, 1, 1, 0, 0, 0, 0 },    // 888110000 Brian Auger 1
    { 8, 8, 8, 8, 0, 5, 0, 0, 0 },    // 888805000 Brian Auger 2
    { 8, 7, 8, 0, 0, 0, 4, 5, 6 },    // 878000456 Bright Comping
    { 8, 0, 0, 0, 0, 0, 8, 8, 8 },    // 800000888 Brother Jack
    { 8, 4, 3, 0, 0, 0, 0, 0, 0 },    // 843000000 Dark Comping
    { 8, 8, 8, 8, 8, 8, 8, 8, 8 },    // 888888888 Dark Solo A (U)
    { 6, 6, 2, 0, 0, 0, 0, 0, 0 },    // 662000000 Dark Solo A (L)
    { 8, 2, 8, 2, 0, 0, 0, 0, 2 },    // 828200002 Dark Solo B (U)
    { 6, 0, 6, 0, 0, 0, 0, 0, 0 },    // 606000000 Dark Solo B (L)
    { 8, 8, 8, 0, 0, 0, 8, 8, 8 },    // 888000888 Fat
    { 0, 8, 0, 0, 8, 0, 8, 8, 3 },    // 080080883 Fifth Organ (Gospel) (U)
    { 0, 0, 8, 8, 0, 6, 0, 0, 0 },    // 008806000 Fifth Organ (Gospel) (L)
    { 0, 0, 6, 8, 0, 2, 0, 0, 0 },    // 006802000 Flutes
    { 8, 8, 8, 6, 6, 6, 8, 8, 8 },    // 888666888 Full and High
    { 8, 6, 8, 8, 6, 8, 0, 6, 8 },    // 868868068 Full and Sweet
    { 8, 8, 8, 8, 8, 8, 8, 8, 8 },    // 888888888 Full Organ
    { 6, 8, 8, 6, 0, 0, 0, 0, 4 },    // 688600004 Funky Comping
    { 8, 8, 8, 8, 0, 0, 0, 0, 0 },    // 888800000 Gimme Some Loving
    { 8, 0, 8, 8, 0, 8, 0, 0, 8 },    // 808808008 Gospel 1
    { 8, 8, 8, 0, 0, 0, 0, 0, 8 },    // 888000008 Gospel 2
    { 8, 6, 8, 6, 6, 6, 5, 6, 8 },    // 868666568 Greg Allman 1
    { 8, 8, 8, 6, 0, 0, 0, 0, 0 },    // 888600000 Greg Allman 2
    { 8, 8, 6, 0, 0, 0, 0, 4, 0 },    // 886000040 Greg Allman 3
    { 8, 8, 8, 8, 0, 0, 0, 8, 8 },    // 888800088 Greg Rolie 1
    { 8, 8, 6, 4, 0, 0, 0, 0, 0 },    // 886400000 Greg Rolie 2
    { 8, 8, 8, 8, 8, 6, 6, 6, 6 },    // 888886666 Greg Rolie 4
    { 8, 8, 8, 4, 2, 0, 0, 8, 0 },    // 888420080 Groove Holmes (Gospel) (U)
    { 0, 0, 0, 5, 0, 5, 0, 0, 0 },    // 000505000 Groove Holmes (Gospel) (L)
    { 8, 8, 0, 0, 0, 0, 0, 0, 0 },    // 880000000 House Bass (Gospel) (U)
    { 0, 0, 8, 0, 8, 0, 0, 0, 0 },    // 008080000 House Bass (Gospel) (L)
    { 8, 6, 8, 6, 0, 0, 0, 0, 6 },    // 868600006 Jimmy McGriff 1
    { 8, 8, 3, 2, 0, 0, 1, 2, 5 },    // 883200125 Jimmy McGriff 2 (Gospel) (U)
    { 4, 4, 8, 6, 5, 0, 0, 0, 0 },    // 448650000 Jimmy McGriff 2 (Gospel) (L)
    { 8, 8, 8, 8, 8, 8, 8, 8, 8 },    // 888888888 Jimmy Smith 1 (U)
    { 0, 0, 7, 5, 0, 0, 0, 0, 0 },    // 007500000 Jimmy Smith 1 (L)
    { 8, 8, 8, 0, 0, 0, 0, 0, 0 },    // 888000000 Jimmy Smith 2 (U)
    { 8, 3, 8, 0, 0, 0, 0, 0, 0 },    // 838000000 Jimmy Smith 2 (L)
    { 8, 8, 8, 0, 0, 0, 0, 0, 0 },    // 888000000 Jimmy Smith 3 (U)
    { 8, 0, 8, 0, 0, 0, 0, 0, 0 },    // 808000000 Jimmy Smith 3 (L)
    { 8, 8, 8, 4, 0, 0, 0, 8, 0 },    // 888400080 Joey DeFrancesco
    { 8, 8, 4, 4, 0, 0, 0, 0, 0 },    // 884400000 Jon Lord
    { 8, 8, 0, 0, 6, 0, 0, 0, 0 },    // 880060000 Latin (Gospel) (U)
    { 0, 0, 6, 6, 7, 6, 0, 0, 0 },    // 006676000 Latin (Gospel) (L)
    { 8, 0, 0, 8, 0, 8, 0, 0, 0 },    // 800808000 Matthew Fisher
    { 8, 6, 8, 8, 0, 0, 0, 0, 4 },    // 868800004 Melvin Crispel
    { 8, 0, 3, 6, 0, 0, 0, 0, 0 },    // 803600000 Mellow
    { 0, 0, 7, 8, 0, 0, 4, 5, 3 },    // 007800453 Meditation Time (Gospel) (U)
    { 0, 0, 6, 7, 0, 0, 5, 4, 0 },    // 006700540 Meditation Time (Gospel) (L)
    { 8, 8, 6, 8, 0, 0, 3, 0, 0 },    // 886800300 Paul Shaffer 1
    { 8, 8, 8, 7, 6, 8, 8, 8, 8 },    // 888768888 Paul Shaffer 2
    { 8, 8, 8, 8, 7, 8, 6, 7, 8 },    // 888878678 Paul Shaffer 3
    { 8, 5, 0, 0, 0, 5, 0, 0, 0 },    // 850005000 Pink Floyd
    { 8, 8, 8, 8, 0, 0, 0, 0, 0 },    // 888800000 Power Chords
    { 8, 8, 8, 8, 0, 0, 0, 0, 0 },    // 888800000 Progessive (Gospel) (U)
    { 0, 0, 8, 8, 8, 4, 0, 0, 0 },    // 008884000 Progessive (Gospel) (L)
    { 0, 0, 6, 8, 7, 6, 4, 0, 0 },    // 006876400 Ray Charles
    { 8, 0, 8, 0, 0, 0, 0, 0, 8 },    // 808000008 Reggae
    { 8, 8, 8, 8, 0, 0, 0, 0, 0 },    // 888800000 Rock, R&B (U)
    { 8, 4, 8, 0, 0, 0, 0, 0, 0 },    // 848000000 Rock, R&B (L)
    { 8, 0, 0, 3, 8, 8, 8, 8, 8 },    // 800388888 Screaming (Gospel) (U)
    { 0, 0, 7, 0, 3, 3, 3, 3, 3 },    // 007033333 Screaming (Gospel) (L)
    { 0, 0, 8, 8, 8, 8, 8, 0, 0 },    // 008888800 Shirley Scott
    { 8, 3, 0, 0, 0, 0, 3, 7, 8 },    // 830000378 Simmering
    { 0, 8, 0, 0, 0, 0, 0, 0, 0 },    // 080000000 Sine
    { 8, 7, 6, 5, 5, 6, 7, 8, 8 },    // 876556788 Shouting 1
    { 6, 6, 8, 8, 4, 8, 5, 8, 8 },    // 668848588 Shouting 2
    { 8, 7, 8, 6, 4, 5, 4, 6, 6 },    // 878645466 Shouting 3 (Gospel) (U)
    { 8, 8, 8, 8, 0, 0, 0, 0, 0 },    // 888800000 Shouting 3 (Gospel) (L)
    { 0, 0, 8, 4, 0, 0, 0, 0, 0 },    // 008400000 Slow Balllad
    { 0, 6, 8, 8, 4, 0, 0, 0, 3 },    // 068840003 Slowly
    { 8, 8, 8, 7, 0, 0, 0, 0, 0 },    // 888700000 Soft Backing (Gospel) (U)
    { 5, 5, 5, 4, 0, 0, 0, 0, 0 },    // 555400000 Soft Backing (Gospel) (L)
    { 8, 0, 8, 4, 0, 0, 0, 0, 8 },    // 808400008 Soft Chords
    { 6, 7, 8, 4, 0, 4, 2, 3, 1 },    // 678404231 Speaker Talking (Gospel) (U)
    { 0, 0, 6, 6, 0, 2, 0, 2, 4 },    // 006602024 Speaker Talking (Gospel) (L)
    { 8, 8, 8, 6, 4, 3, 2, 0, 0 },    // 888643200 Steppenwolf
    { 8, 8, 8, 8, 7, 6, 7, 8, 8 },    // 888876788 Steve Winwood
    { 8, 7, 6, 5, 4, 3, 2, 1, 1 },    // 876543211 Strings
    { 0, 0, 8, 0, 0, 0, 0, 0, 0 },    // 008000000 Sweet
    { 7, 8, 7, 7, 4, 6, 0, 4, 6 },    // 787746046 Testimony Service  (Gospel) (U)
    { 0, 0, 8, 8, 0, 0, 6, 7, 3 },    // 008800673 Testimony Service  (Gospel) (L)
    { 8, 7, 8, 6, 5, 6, 4, 6, 7 },    // 878656467 Theatre Organ (Gospel) (U)
    { 0, 0, 8, 8, 4, 4, 0, 0, 0 },    // 008844000 Theatre Organ (Gospel) (L)
    { 8, 8, 8, 8, 0, 0, 0, 0, 0 },    // 888800000 Tom Coster
    { 8, 0, 0, 0, 0, 0, 0, 0, 8 },    // 800000008 Whistle 1
    { 8, 8, 8, 0, 0, 0, 0, 0, 8 },    // 888000008 Whistle 2
    { 6, 8, 8, 6, 0, 0, 0, 0, 0 },    // 688600000 Whiter Shade Of Pale 1 (U)
    { 8, 8, 0, 0, 7, 0, 7, 7, 0 },    // 880070770 Whiter Shade Of Pale 1 (L)
    { 8, 8, 8, 8, 0, 8, 0, 0, 6 },    // 888808006 Whiter Shade Of Pale 2 (U)
    { 0, 0, 4, 4, 4, 0, 0, 0, 0 },    // 004440000 Whiter Shade Of Pale 2 (L)
    { 8, 6, 6, 8, 0, 0, 0, 0, 0 },    // 866800000 Wide Leslie
*/    



// ADJUSTING TRACKING

// GRAINS inputs are unfortunately not v/oct: they're approximately 1.3v/oct, and the exact amount
// varies depending on how warmed up the GRAINS resistors are, and also based on how close
// to 5V your MASTER I/O is (you can tune that), how much total load is on the system, etc.
// As a result, your GRAINS tracking will be out of whack.  You'll need to adjust it.  Hopefully
// you won't have to adjust it every time, if you let your GRAINS warm up first.  :-) 
//
// Below you will find a #define TRACKING_47_3.  This sets the tracking value to 47.3, which is
// the value appropriate for my own personal GRAINS.  If Grains was a perfect v/oct, this would 
// have been 60 (60 semitones = 5 octaves).  Your GRAINS value will be somewhat different from 47.3.
// You can change it to a different value, say, 42.9, which would be:     #define TRACKING_42_9
// It's always three digits in the format  #define TRACKING_XX_Y
//
// I have provided TRACKING values from 40.0 to 50.9, that is, TRACKING_40_0 to TRACKING_50_9
// If you need values outside of this, send me mail.
//
// To tune the tracking, do this:
//
// 0. Let the GRAINS warm up.  A lot.
// 1. Start with your best guess of tracking (let's say 47.3; or with another MASTER I/O I had 45)
// 2. Set the #define below to that value.
// 3. Plug in the USB cable.  
// 4. Compile and upload the code
// 5. UNPLUG the USB cable.  This is important: USB boosts GRAINS voltage, which messes things up 
// 6. Test.  How close does my code on the GRAINS get to correct tracking? 
// 7. If it's too stretched, reduce the number.  If it's too compressed, increase.
// 8. Go to step 2 and repeat until the tracking looks good.
//
// Once you have the right tracking #define, you should be able to use it in all of my mozzi projects,
// and with any luck, if you let your GRAINS warm up, you won't need to change it further, fingers crossed.

#define TRACKING_47_3



#define FREQUENCY(pitch) pgm_read_float_near(&frequencies[pitch])
#define CONTROL_RATE 128                        // More than this and we're gonna get clicks probably

#include "tracking.h"
#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin256_int8.h>

/// OSCILLATORS

Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel1(SIN256_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel2(SIN256_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel3(SIN256_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel4(SIN256_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel5(SIN256_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel6(SIN256_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel7(SIN256_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel8(SIN256_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel9(SIN256_DATA);

Oscil<SIN256_NUM_CELLS, AUDIO_RATE> oscils[9] = 
    { 
    tonewheel1, tonewheel2, tonewheel3,
    tonewheel4, tonewheel5, tonewheel6, 
    tonewheel7, tonewheel8, tonewheel9 
    };

#ifdef LESLIE_ON
Oscil<SIN256_NUM_CELLS, CONTROL_RATE> leslie(SIN256_DATA);              // Leslie LFO
#endif
        
float drawbarFrequencies[9] = { 0.5, 1.0, 1.5, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0 };

// These are the amplitudes of each of the drawbar stop positions (0...8).
// Each additional stop increases by 3db.  I think I have this right?  Not sure.
// We are int8_t, not uin8_t, so we can multiply faster against oscils, which is
// also int8_t.
uint8_t drawBarAmplitudes[9] = 
    {
    0, 7, 18, 32, 48, 65, 85, 105, 127
    };

void setup()
    {
//Serial.begin(9600);
    startMozzi();
    // Fire up the leslie
#ifdef LESLIE_ON  
    leslie.setFreq((float)LESLIE_FREQUENCY);
#endif

// precompute the drawbars so we don't have to compute them during updateAudio()
    for(uint8_t i = 0; i < NUM_DRAWBAR_SELECTIONS; i++)
        {
        for(uint8_t j = 0; j < 9; j++)
            {
            drawbars[i][j] = drawBarAmplitudes[drawbars[i][j]];
            }
        }
    }

void loop()
    {
    audioHook();
    }


#define CV_POT_IN1    A2    // Organ Selection
#define CV_POT_IN2    A1    // Pitch Tune
#define CV_POT3       A0    // Gain
#define CV_IN3        A3    // Pitch
#define CV_AUDIO_IN   A4    // [Unused]
#define CV_AUDIO_OUT  9     // Out
#define CV_GATE_OUT   8     // [Unused]

// Median-Of-Three Filter for Pitch, dunno if we need it
#define MEDIAN_OF_THREE(a,b,c) (((a) <= (b)) ? (((b) <= (c)) ? (b) : (((a) < (c)) ? (c) : (a))) : (((a) <= (c)) ? (a) : (((b) < (c)) ? (c) : (b))))
uint16_t inA = 0;
uint16_t inB = 0;
uint16_t pitchCV;
uint16_t tuneCV;
uint16_t pitch;
uint8_t organ = 0;      // Current index into the drawbars array
uint16_t gain;          // our current gain

void updateControl()                          
    {
    uint16_t in = mozziAnalogRead(CV_IN3);
    // let's try a median of three filter to reduce some of the jumpiness
    uint16_t pitchCV = MEDIAN_OF_THREE(in, inA, inB);
    inA = inB;
    inB = in;
    tuneCV = (tuneCV * 3 + mozziAnalogRead(CV_POT_IN2)) >> 2;
    pitch = (pitch + pitchCV + (tuneCV >> 1)) >> 1;

    float frequency = FREQUENCY(pitch);
        

    // leslie!
#ifdef LESLIE_ON
    int8_t les = leslie.next();
    frequency += les * (1.0 / LESLIE_PITCH);
#endif

    // set the drawbars
    for(uint8_t i = 0; i < 9; i++)
        {
        oscils[i].setFreq((frequency * drawbarFrequencies[i]));
        }

    // determine the gain
    gain = 1 + (mozziAnalogRead(CV_POT3) >> 2)
#ifdef LESLIE_ON
#if LESLIE_VOLUME==0
#else
        + ((les + 128) >> (9 - LESLIE_VOLUME));                              // note: >> 0 still goes between -1 and 0 depending on whether it's positive or negative initially
#endif
#endif
    ;

    // what organ are we playing?
    organ = (organ + (mozziAnalogRead(CV_POT_IN1) * NUM_DRAWBAR_SELECTIONS)) >> 10;
    }                                             


int updateAudio()                             
    { 
    uint8_t* d = drawbars[organ];
    int32_t val = 0;
    for(uint8_t i = 0; i < 9; i++)
        {
        val += (oscils[i].next() * d[i]);
        }
    return ((val * gain) >> 16) ;
    }
