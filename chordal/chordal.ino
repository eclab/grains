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


/// CHORDAL
///
/// Chordal is a mixed square/saw/triangle- and sine-wave chording oscillator.
/// Chordal is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// Chordal can mix between square and sine, saw and sine, or triangle and sine.  You select
/// which one you want in the code.  The default is square and sine.
///
/// You also set the pitch CV and tune, the amount of square/saw/tringle vs sine wave, 
/// and your chord choice.
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.
///
/// NOTE: Probably due to the voltage divider on all of its analog inputs, GRAINS is limited
/// to about a 45 note range.  


/// CONFIGURATION
///
/// IN 1            Mix CV              [Low: all Sine, High: all Square/Saw/Tri]
/// IN 2            [Unused]
/// IN 3            Pitch CV    [This isn't IN2, like Pitch Tune, because Pitch Tune must be +/-]
/// AUDIO IN (A)    [Unused]
/// AUDIO OUT       Out
/// DIGITAL OUT (D) [Unused]
///
/// POT 1           Mix                 [Low: all Sine, High: all Square/Saw/Tri]
///                                     [If you're not using Mix CV, set the switch to MAN]
///
/// POT 2           Pitch Tune  [Set the switch to MAN]
///
/// POT 3           Chord


/// CHOOSING SAW VS SQUARE VS TRIANGLE
///
/// The choice of wave is deterined by the #define used below.  Notice that two are commented out
/// as // #define ...   To select a wave, remove the // but make sure that the other two waves
/// have been commented out (they have // in front of them).  For example, to use sawtooth, you'd
/// change things to look like this:
///
///    // #define USE_SQUARE
///    #define USE_SAW
///    // #define USE_TRI
///
/// Set it right here:


#define USE_SQUARE
// #define USE_SAW
// #define USE_TRI


/// CHORDS
///
/// The chords are, in order:
///
/// No Chord
/// Intervals: minor 3, Major 3, 4, 5, minor 6, Major 6, minor 7, octave, octave + minor 3, octave + major 3, octave + 5
/// Triads: minor, minor first inversion, minor second inversion, Major, Major first inversion, Major second inversion
/// Sevenths: 7, minor 7, Major 7, diminished 7
/// Octave Triads: minor + octave, Major + octave  


// ADJUSTING TRACKING

// GRAINS inputs are not v/oct: they're approximately 1.3v/oct, and the exact amount
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
// 6. Test.  How close does my code on the GRAINS get to correct tracking?  You might use a tuning 
//    phone app, tuned to C two octaves below middle C, and then and working your way up to see how
//    badly it gets off.
// 7. If it's too stretched, reduce the number.  If it's too compressed, increase.
// 8. Go to step 2 and repeat until the tracking looks good.
//
// Once you have the right tracking #define, you should be able to use it in all of my mozzi projects,
// and with any luck, if you let your GRAINS warm up, you won't need to change it further, fingers crossed.

#define TRACKING_47_3



#define FREQUENCY(pitch) pgm_read_float_near(&frequencies[pitch])
#define CONTROL_RATE 128

#include "tracking.h"
#include <MozziGuts.h>
#include <Oscil.h>
#include <MetaOscil.h>

#include <tables/BandLimited_SQUARE/512/square_max_90_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_101_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_122_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_138_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_154_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_174_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_210_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_264_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_327_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_431_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_546_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_744_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_910_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_1170_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_1638_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_2730_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_8192_at_16384_512_int8.h>

#include <tables/BandLimited_SAW/512/saw_max_136_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_138_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_154_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_174_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_210_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_264_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_327_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_431_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_546_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_744_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_910_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_1170_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_1638_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_2048_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_2730_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_4096_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_8192_at_16384_512_int8.h>

#include <tables/BandLimited_TRI/512/tri_max_103_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_106_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_115_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_130_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_154_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_174_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_210_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_264_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_327_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_431_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_546_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_744_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_910_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_1170_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_1638_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_2730_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_8192_at_16384_512_int8.h>

#include <tables/sin512_int8.h>

/// OSCILLATORS

// 4 Sine Wave Oscillators
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> sine1(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> sine2(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> sine3(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> sine4(SIN512_DATA);

#ifdef USE_SQUARE
// Square Wave Oscillator Bank 1
Oscil <SQUARE_MAX_90_AT_16384_512_NUM_CELLS, AUDIO_RATE> square90(SQUARE_MAX_90_AT_16384_512_DATA);
Oscil <SQUARE_MAX_101_AT_16384_512_NUM_CELLS, AUDIO_RATE> square101(SQUARE_MAX_101_AT_16384_512_DATA);
Oscil <SQUARE_MAX_122_AT_16384_512_NUM_CELLS, AUDIO_RATE> square122(SQUARE_MAX_122_AT_16384_512_DATA);
Oscil <SQUARE_MAX_138_AT_16384_512_NUM_CELLS, AUDIO_RATE> square138(SQUARE_MAX_138_AT_16384_512_DATA);
Oscil <SQUARE_MAX_154_AT_16384_512_NUM_CELLS, AUDIO_RATE> square154(SQUARE_MAX_154_AT_16384_512_DATA);
Oscil <SQUARE_MAX_174_AT_16384_512_NUM_CELLS, AUDIO_RATE> square174(SQUARE_MAX_174_AT_16384_512_DATA);
Oscil <SQUARE_MAX_210_AT_16384_512_NUM_CELLS, AUDIO_RATE> square210(SQUARE_MAX_210_AT_16384_512_DATA);
Oscil <SQUARE_MAX_264_AT_16384_512_NUM_CELLS, AUDIO_RATE> square264(SQUARE_MAX_264_AT_16384_512_DATA);
Oscil <SQUARE_MAX_327_AT_16384_512_NUM_CELLS, AUDIO_RATE> square327(SQUARE_MAX_327_AT_16384_512_DATA);
Oscil <SQUARE_MAX_431_AT_16384_512_NUM_CELLS, AUDIO_RATE> square431(SQUARE_MAX_431_AT_16384_512_DATA);
Oscil <SQUARE_MAX_546_AT_16384_512_NUM_CELLS, AUDIO_RATE> square546(SQUARE_MAX_546_AT_16384_512_DATA);
Oscil <SQUARE_MAX_744_AT_16384_512_NUM_CELLS, AUDIO_RATE> square744(SQUARE_MAX_744_AT_16384_512_DATA);
Oscil <SQUARE_MAX_910_AT_16384_512_NUM_CELLS, AUDIO_RATE> square910(SQUARE_MAX_910_AT_16384_512_DATA);
Oscil <SQUARE_MAX_1170_AT_16384_512_NUM_CELLS, AUDIO_RATE> square1170(SQUARE_MAX_1170_AT_16384_512_DATA);
Oscil <SQUARE_MAX_1638_AT_16384_512_NUM_CELLS, AUDIO_RATE> square1638(SQUARE_MAX_1638_AT_16384_512_DATA);
Oscil <SQUARE_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> square2730(SQUARE_MAX_2730_AT_16384_512_DATA);
Oscil <SQUARE_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> square8192(SQUARE_MAX_8192_AT_16384_512_DATA);

// Square Wave Oscillator Bank 2
Oscil <SQUARE_MAX_90_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare90(SQUARE_MAX_90_AT_16384_512_DATA);
Oscil <SQUARE_MAX_101_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare101(SQUARE_MAX_101_AT_16384_512_DATA);
Oscil <SQUARE_MAX_122_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare122(SQUARE_MAX_122_AT_16384_512_DATA);
Oscil <SQUARE_MAX_138_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare138(SQUARE_MAX_138_AT_16384_512_DATA);
Oscil <SQUARE_MAX_154_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare154(SQUARE_MAX_154_AT_16384_512_DATA);
Oscil <SQUARE_MAX_174_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare174(SQUARE_MAX_174_AT_16384_512_DATA);
Oscil <SQUARE_MAX_210_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare210(SQUARE_MAX_210_AT_16384_512_DATA);
Oscil <SQUARE_MAX_264_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare264(SQUARE_MAX_264_AT_16384_512_DATA);
Oscil <SQUARE_MAX_327_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare327(SQUARE_MAX_327_AT_16384_512_DATA);
Oscil <SQUARE_MAX_431_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare431(SQUARE_MAX_431_AT_16384_512_DATA);
Oscil <SQUARE_MAX_546_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare546(SQUARE_MAX_546_AT_16384_512_DATA);
Oscil <SQUARE_MAX_744_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare744(SQUARE_MAX_744_AT_16384_512_DATA);
Oscil <SQUARE_MAX_910_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare910(SQUARE_MAX_910_AT_16384_512_DATA);
Oscil <SQUARE_MAX_1170_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare1170(SQUARE_MAX_1170_AT_16384_512_DATA);
Oscil <SQUARE_MAX_1638_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare1638(SQUARE_MAX_1638_AT_16384_512_DATA);
Oscil <SQUARE_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare2730(SQUARE_MAX_2730_AT_16384_512_DATA);
Oscil <SQUARE_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare8192(SQUARE_MAX_8192_AT_16384_512_DATA);

// Square Wave Oscillator Bank 3
Oscil <SQUARE_MAX_90_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare90(SQUARE_MAX_90_AT_16384_512_DATA);
Oscil <SQUARE_MAX_101_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare101(SQUARE_MAX_101_AT_16384_512_DATA);
Oscil <SQUARE_MAX_122_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare122(SQUARE_MAX_122_AT_16384_512_DATA);
Oscil <SQUARE_MAX_138_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare138(SQUARE_MAX_138_AT_16384_512_DATA);
Oscil <SQUARE_MAX_154_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare154(SQUARE_MAX_154_AT_16384_512_DATA);
Oscil <SQUARE_MAX_174_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare174(SQUARE_MAX_174_AT_16384_512_DATA);
Oscil <SQUARE_MAX_210_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare210(SQUARE_MAX_210_AT_16384_512_DATA);
Oscil <SQUARE_MAX_264_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare264(SQUARE_MAX_264_AT_16384_512_DATA);
Oscil <SQUARE_MAX_327_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare327(SQUARE_MAX_327_AT_16384_512_DATA);
Oscil <SQUARE_MAX_431_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare431(SQUARE_MAX_431_AT_16384_512_DATA);
Oscil <SQUARE_MAX_546_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare546(SQUARE_MAX_546_AT_16384_512_DATA);
Oscil <SQUARE_MAX_744_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare744(SQUARE_MAX_744_AT_16384_512_DATA);
Oscil <SQUARE_MAX_910_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare910(SQUARE_MAX_910_AT_16384_512_DATA);
Oscil <SQUARE_MAX_1170_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare1170(SQUARE_MAX_1170_AT_16384_512_DATA);
Oscil <SQUARE_MAX_1638_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare1638(SQUARE_MAX_1638_AT_16384_512_DATA);
Oscil <SQUARE_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare2730(SQUARE_MAX_2730_AT_16384_512_DATA);
Oscil <SQUARE_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare8192(SQUARE_MAX_8192_AT_16384_512_DATA);

// Square Wave Oscillator Bank 4
Oscil <SQUARE_MAX_90_AT_16384_512_NUM_CELLS, AUDIO_RATE> csquare90(SQUARE_MAX_90_AT_16384_512_DATA);
Oscil <SQUARE_MAX_101_AT_16384_512_NUM_CELLS, AUDIO_RATE> csquare101(SQUARE_MAX_101_AT_16384_512_DATA);
Oscil <SQUARE_MAX_122_AT_16384_512_NUM_CELLS, AUDIO_RATE> csquare122(SQUARE_MAX_122_AT_16384_512_DATA);
Oscil <SQUARE_MAX_138_AT_16384_512_NUM_CELLS, AUDIO_RATE> csquare138(SQUARE_MAX_138_AT_16384_512_DATA);
Oscil <SQUARE_MAX_154_AT_16384_512_NUM_CELLS, AUDIO_RATE> csquare154(SQUARE_MAX_154_AT_16384_512_DATA);
Oscil <SQUARE_MAX_174_AT_16384_512_NUM_CELLS, AUDIO_RATE> csquare174(SQUARE_MAX_174_AT_16384_512_DATA);
Oscil <SQUARE_MAX_210_AT_16384_512_NUM_CELLS, AUDIO_RATE> csquare210(SQUARE_MAX_210_AT_16384_512_DATA);
Oscil <SQUARE_MAX_264_AT_16384_512_NUM_CELLS, AUDIO_RATE> csquare264(SQUARE_MAX_264_AT_16384_512_DATA);
Oscil <SQUARE_MAX_327_AT_16384_512_NUM_CELLS, AUDIO_RATE> csquare327(SQUARE_MAX_327_AT_16384_512_DATA);
Oscil <SQUARE_MAX_431_AT_16384_512_NUM_CELLS, AUDIO_RATE> csquare431(SQUARE_MAX_431_AT_16384_512_DATA);
Oscil <SQUARE_MAX_546_AT_16384_512_NUM_CELLS, AUDIO_RATE> csquare546(SQUARE_MAX_546_AT_16384_512_DATA);
Oscil <SQUARE_MAX_744_AT_16384_512_NUM_CELLS, AUDIO_RATE> csquare744(SQUARE_MAX_744_AT_16384_512_DATA);
Oscil <SQUARE_MAX_910_AT_16384_512_NUM_CELLS, AUDIO_RATE> csquare910(SQUARE_MAX_910_AT_16384_512_DATA);
Oscil <SQUARE_MAX_1170_AT_16384_512_NUM_CELLS, AUDIO_RATE> csquare1170(SQUARE_MAX_1170_AT_16384_512_DATA);
Oscil <SQUARE_MAX_1638_AT_16384_512_NUM_CELLS, AUDIO_RATE> csquare1638(SQUARE_MAX_1638_AT_16384_512_DATA);
Oscil <SQUARE_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> csquare2730(SQUARE_MAX_2730_AT_16384_512_DATA);
Oscil <SQUARE_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> csquare8192(SQUARE_MAX_8192_AT_16384_512_DATA);

// Meta-Oscillators for Square Waves
MetaOscil<SQUARE_MAX_90_AT_16384_512_NUM_CELLS, AUDIO_RATE, 17> meta1 {&square90, &square101, &square122, &square138, &square154, &square174, &square210, &square264, &square327, &square431, &square546, &square744, &square1170, &square910, &square1638, &square2730, &square8192};
MetaOscil<SQUARE_MAX_90_AT_16384_512_NUM_CELLS, AUDIO_RATE, 17> meta2 {&asquare90, &asquare101, &asquare122, &asquare138, &asquare154, &asquare174, &asquare210, &asquare264, &asquare327, &asquare431, &asquare546, &asquare744, &asquare910, &asquare1170, &asquare1638, &asquare2730, &asquare8192};
MetaOscil<SQUARE_MAX_90_AT_16384_512_NUM_CELLS, AUDIO_RATE, 17> meta3 {&bsquare90, &bsquare101, &bsquare122, &bsquare138, &bsquare154, &bsquare174, &bsquare210, &bsquare264, &bsquare327, &bsquare431, &bsquare546, &bsquare744, &bsquare910, &bsquare1170, &bsquare1638, &bsquare2730, &bsquare8192};
MetaOscil<SQUARE_MAX_90_AT_16384_512_NUM_CELLS, AUDIO_RATE, 17> meta4 {&csquare90, &csquare101, &csquare122, &csquare138, &csquare154, &csquare174, &csquare210, &csquare264, &csquare327, &csquare431, &csquare546, &csquare744, &csquare910, &csquare1170, &csquare1638, &csquare2730, &csquare8192};
#endif

#ifdef USE_SAW
// Sawtooth Wave Oscillator Bank 1
Oscil <SAW_MAX_136_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw136(SAW_MAX_136_AT_16384_512_DATA);
Oscil <SAW_MAX_138_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw138(SAW_MAX_138_AT_16384_512_DATA);
Oscil <SAW_MAX_154_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw154(SAW_MAX_154_AT_16384_512_DATA);
Oscil <SAW_MAX_174_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw174(SAW_MAX_174_AT_16384_512_DATA);
Oscil <SAW_MAX_210_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw210(SAW_MAX_210_AT_16384_512_DATA);
Oscil <SAW_MAX_264_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw264(SAW_MAX_264_AT_16384_512_DATA);
Oscil <SAW_MAX_327_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw327(SAW_MAX_327_AT_16384_512_DATA);
Oscil <SAW_MAX_431_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw431(SAW_MAX_431_AT_16384_512_DATA);
Oscil <SAW_MAX_546_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw546(SAW_MAX_546_AT_16384_512_DATA);
Oscil <SAW_MAX_744_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw744(SAW_MAX_744_AT_16384_512_DATA);
Oscil <SAW_MAX_910_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw910(SAW_MAX_910_AT_16384_512_DATA);
Oscil <SAW_MAX_1170_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw1170(SAW_MAX_1170_AT_16384_512_DATA);
Oscil <SAW_MAX_1638_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw1638(SAW_MAX_1638_AT_16384_512_DATA);
Oscil <SAW_MAX_2048_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw2048(SAW_MAX_2048_AT_16384_512_DATA);
Oscil <SAW_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw2730(SAW_MAX_2730_AT_16384_512_DATA);
Oscil <SAW_MAX_4096_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw4096(SAW_MAX_4096_AT_16384_512_DATA);
Oscil <SAW_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw8192(SAW_MAX_8192_AT_16384_512_DATA);

// Sawtooth Wave Oscillator Bank 2
Oscil <SAW_MAX_136_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw136(SAW_MAX_136_AT_16384_512_DATA);
Oscil <SAW_MAX_138_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw138(SAW_MAX_138_AT_16384_512_DATA);
Oscil <SAW_MAX_154_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw154(SAW_MAX_154_AT_16384_512_DATA);
Oscil <SAW_MAX_174_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw174(SAW_MAX_174_AT_16384_512_DATA);
Oscil <SAW_MAX_210_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw210(SAW_MAX_210_AT_16384_512_DATA);
Oscil <SAW_MAX_264_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw264(SAW_MAX_264_AT_16384_512_DATA);
Oscil <SAW_MAX_327_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw327(SAW_MAX_327_AT_16384_512_DATA);
Oscil <SAW_MAX_431_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw431(SAW_MAX_431_AT_16384_512_DATA);
Oscil <SAW_MAX_546_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw546(SAW_MAX_546_AT_16384_512_DATA);
Oscil <SAW_MAX_744_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw744(SAW_MAX_744_AT_16384_512_DATA);
Oscil <SAW_MAX_910_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw910(SAW_MAX_910_AT_16384_512_DATA);
Oscil <SAW_MAX_1170_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw1170(SAW_MAX_1170_AT_16384_512_DATA);
Oscil <SAW_MAX_1638_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw1638(SAW_MAX_1638_AT_16384_512_DATA);
Oscil <SAW_MAX_2048_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw2048(SAW_MAX_2048_AT_16384_512_DATA);
Oscil <SAW_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw2730(SAW_MAX_2730_AT_16384_512_DATA);
Oscil <SAW_MAX_4096_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw4096(SAW_MAX_4096_AT_16384_512_DATA);
Oscil <SAW_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw8192(SAW_MAX_8192_AT_16384_512_DATA);

// Sawtooth Wave Oscillator Bank 3
Oscil <SAW_MAX_136_AT_16384_512_NUM_CELLS, AUDIO_RATE> csaw136(SAW_MAX_136_AT_16384_512_DATA);
Oscil <SAW_MAX_138_AT_16384_512_NUM_CELLS, AUDIO_RATE> csaw138(SAW_MAX_138_AT_16384_512_DATA);
Oscil <SAW_MAX_154_AT_16384_512_NUM_CELLS, AUDIO_RATE> csaw154(SAW_MAX_154_AT_16384_512_DATA);
Oscil <SAW_MAX_174_AT_16384_512_NUM_CELLS, AUDIO_RATE> csaw174(SAW_MAX_174_AT_16384_512_DATA);
Oscil <SAW_MAX_210_AT_16384_512_NUM_CELLS, AUDIO_RATE> csaw210(SAW_MAX_210_AT_16384_512_DATA);
Oscil <SAW_MAX_264_AT_16384_512_NUM_CELLS, AUDIO_RATE> csaw264(SAW_MAX_264_AT_16384_512_DATA);
Oscil <SAW_MAX_327_AT_16384_512_NUM_CELLS, AUDIO_RATE> csaw327(SAW_MAX_327_AT_16384_512_DATA);
Oscil <SAW_MAX_431_AT_16384_512_NUM_CELLS, AUDIO_RATE> csaw431(SAW_MAX_431_AT_16384_512_DATA);
Oscil <SAW_MAX_546_AT_16384_512_NUM_CELLS, AUDIO_RATE> csaw546(SAW_MAX_546_AT_16384_512_DATA);
Oscil <SAW_MAX_744_AT_16384_512_NUM_CELLS, AUDIO_RATE> csaw744(SAW_MAX_744_AT_16384_512_DATA);
Oscil <SAW_MAX_910_AT_16384_512_NUM_CELLS, AUDIO_RATE> csaw910(SAW_MAX_910_AT_16384_512_DATA);
Oscil <SAW_MAX_1170_AT_16384_512_NUM_CELLS, AUDIO_RATE> csaw1170(SAW_MAX_1170_AT_16384_512_DATA);
Oscil <SAW_MAX_1638_AT_16384_512_NUM_CELLS, AUDIO_RATE> csaw1638(SAW_MAX_1638_AT_16384_512_DATA);
Oscil <SAW_MAX_2048_AT_16384_512_NUM_CELLS, AUDIO_RATE> csaw2048(SAW_MAX_2048_AT_16384_512_DATA);
Oscil <SAW_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> csaw2730(SAW_MAX_2730_AT_16384_512_DATA);
Oscil <SAW_MAX_4096_AT_16384_512_NUM_CELLS, AUDIO_RATE> csaw4096(SAW_MAX_4096_AT_16384_512_DATA);
Oscil <SAW_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> csaw8192(SAW_MAX_8192_AT_16384_512_DATA);

// Sawtooth Wave Oscillator Bank 4
Oscil <SAW_MAX_136_AT_16384_512_NUM_CELLS, AUDIO_RATE> dsaw136(SAW_MAX_136_AT_16384_512_DATA);
Oscil <SAW_MAX_138_AT_16384_512_NUM_CELLS, AUDIO_RATE> dsaw138(SAW_MAX_138_AT_16384_512_DATA);
Oscil <SAW_MAX_154_AT_16384_512_NUM_CELLS, AUDIO_RATE> dsaw154(SAW_MAX_154_AT_16384_512_DATA);
Oscil <SAW_MAX_174_AT_16384_512_NUM_CELLS, AUDIO_RATE> dsaw174(SAW_MAX_174_AT_16384_512_DATA);
Oscil <SAW_MAX_210_AT_16384_512_NUM_CELLS, AUDIO_RATE> dsaw210(SAW_MAX_210_AT_16384_512_DATA);
Oscil <SAW_MAX_264_AT_16384_512_NUM_CELLS, AUDIO_RATE> dsaw264(SAW_MAX_264_AT_16384_512_DATA);
Oscil <SAW_MAX_327_AT_16384_512_NUM_CELLS, AUDIO_RATE> dsaw327(SAW_MAX_327_AT_16384_512_DATA);
Oscil <SAW_MAX_431_AT_16384_512_NUM_CELLS, AUDIO_RATE> dsaw431(SAW_MAX_431_AT_16384_512_DATA);
Oscil <SAW_MAX_546_AT_16384_512_NUM_CELLS, AUDIO_RATE> dsaw546(SAW_MAX_546_AT_16384_512_DATA);
Oscil <SAW_MAX_744_AT_16384_512_NUM_CELLS, AUDIO_RATE> dsaw744(SAW_MAX_744_AT_16384_512_DATA);
Oscil <SAW_MAX_910_AT_16384_512_NUM_CELLS, AUDIO_RATE> dsaw910(SAW_MAX_910_AT_16384_512_DATA);
Oscil <SAW_MAX_1170_AT_16384_512_NUM_CELLS, AUDIO_RATE> dsaw1170(SAW_MAX_1170_AT_16384_512_DATA);
Oscil <SAW_MAX_1638_AT_16384_512_NUM_CELLS, AUDIO_RATE> dsaw1638(SAW_MAX_1638_AT_16384_512_DATA);
Oscil <SAW_MAX_2048_AT_16384_512_NUM_CELLS, AUDIO_RATE> dsaw2048(SAW_MAX_2048_AT_16384_512_DATA);
Oscil <SAW_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> dsaw2730(SAW_MAX_2730_AT_16384_512_DATA);
Oscil <SAW_MAX_4096_AT_16384_512_NUM_CELLS, AUDIO_RATE> dsaw4096(SAW_MAX_4096_AT_16384_512_DATA);
Oscil <SAW_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> dsaw8192(SAW_MAX_8192_AT_16384_512_DATA);

// Meta-Oscillators for Sawtooth Waves
MetaOscil<SAW_MAX_136_AT_16384_512_NUM_CELLS, AUDIO_RATE, 17> meta1 {&asaw136, &asaw138, &asaw154, &asaw174, &asaw210, &asaw264, &asaw327, &asaw431, &asaw546, &asaw744, &asaw910, &asaw1170, &asaw1638, &asaw2048, &asaw2730, &asaw4096, &asaw8192};
MetaOscil<SAW_MAX_136_AT_16384_512_NUM_CELLS, AUDIO_RATE, 17> meta2 {&bsaw136, &bsaw138, &bsaw154, &bsaw174, &bsaw210, &bsaw264, &bsaw327, &bsaw431, &bsaw546, &bsaw744, &bsaw910, &bsaw1170, &bsaw1638, &bsaw2048, &bsaw2730, &bsaw4096, &bsaw8192};
MetaOscil<SAW_MAX_136_AT_16384_512_NUM_CELLS, AUDIO_RATE, 17> meta3 {&csaw136, &csaw138, &csaw154, &csaw174, &csaw210, &csaw264, &csaw327, &csaw431, &csaw546, &csaw744, &csaw910, &csaw1170, &csaw1638, &csaw2048, &csaw2730, &csaw4096, &csaw8192};
MetaOscil<SAW_MAX_136_AT_16384_512_NUM_CELLS, AUDIO_RATE, 17> meta4 {&dsaw136, &dsaw138, &dsaw154, &dsaw174, &dsaw210, &dsaw264, &dsaw327, &dsaw431, &dsaw546, &dsaw744, &dsaw910, &dsaw1170, &dsaw1638, &dsaw2048, &dsaw2730, &dsaw4096, &dsaw8192};
#endif

#ifdef USE_TRI
// Triangle Wave Oscillator Bank 1
Oscil <TRI_MAX_103_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri103(TRI_MAX_103_AT_16384_512_DATA);
Oscil <TRI_MAX_106_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri106(TRI_MAX_106_AT_16384_512_DATA);
Oscil <TRI_MAX_115_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri115(TRI_MAX_115_AT_16384_512_DATA);
Oscil <TRI_MAX_130_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri130(TRI_MAX_130_AT_16384_512_DATA);
Oscil <TRI_MAX_154_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri154(TRI_MAX_154_AT_16384_512_DATA);
Oscil <TRI_MAX_174_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri174(TRI_MAX_174_AT_16384_512_DATA);
Oscil <TRI_MAX_210_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri210(TRI_MAX_210_AT_16384_512_DATA);
Oscil <TRI_MAX_264_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri264(TRI_MAX_264_AT_16384_512_DATA);
Oscil <TRI_MAX_327_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri327(TRI_MAX_327_AT_16384_512_DATA);
Oscil <TRI_MAX_431_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri431(TRI_MAX_431_AT_16384_512_DATA);
Oscil <TRI_MAX_546_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri546(TRI_MAX_546_AT_16384_512_DATA);
Oscil <TRI_MAX_744_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri744(TRI_MAX_744_AT_16384_512_DATA);
Oscil <TRI_MAX_910_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri910(TRI_MAX_910_AT_16384_512_DATA);
Oscil <TRI_MAX_1170_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri1170(TRI_MAX_1170_AT_16384_512_DATA);
Oscil <TRI_MAX_1638_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri1638(TRI_MAX_1638_AT_16384_512_DATA);
Oscil <TRI_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri2730(TRI_MAX_2730_AT_16384_512_DATA);
Oscil <TRI_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri8192(TRI_MAX_8192_AT_16384_512_DATA);

// Triangle Wave Oscillator Bank 2
Oscil <TRI_MAX_103_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri103(TRI_MAX_103_AT_16384_512_DATA);
Oscil <TRI_MAX_106_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri106(TRI_MAX_106_AT_16384_512_DATA);
Oscil <TRI_MAX_115_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri115(TRI_MAX_115_AT_16384_512_DATA);
Oscil <TRI_MAX_130_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri130(TRI_MAX_130_AT_16384_512_DATA);
Oscil <TRI_MAX_154_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri154(TRI_MAX_154_AT_16384_512_DATA);
Oscil <TRI_MAX_174_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri174(TRI_MAX_174_AT_16384_512_DATA);
Oscil <TRI_MAX_210_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri210(TRI_MAX_210_AT_16384_512_DATA);
Oscil <TRI_MAX_264_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri264(TRI_MAX_264_AT_16384_512_DATA);
Oscil <TRI_MAX_327_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri327(TRI_MAX_327_AT_16384_512_DATA);
Oscil <TRI_MAX_431_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri431(TRI_MAX_431_AT_16384_512_DATA);
Oscil <TRI_MAX_546_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri546(TRI_MAX_546_AT_16384_512_DATA);
Oscil <TRI_MAX_744_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri744(TRI_MAX_744_AT_16384_512_DATA);
Oscil <TRI_MAX_910_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri910(TRI_MAX_910_AT_16384_512_DATA);
Oscil <TRI_MAX_1170_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri1170(TRI_MAX_1170_AT_16384_512_DATA);
Oscil <TRI_MAX_1638_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri1638(TRI_MAX_1638_AT_16384_512_DATA);
Oscil <TRI_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri2730(TRI_MAX_2730_AT_16384_512_DATA);
Oscil <TRI_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri8192(TRI_MAX_8192_AT_16384_512_DATA);

// Triangle Wave Oscillator Bank 3
Oscil <TRI_MAX_103_AT_16384_512_NUM_CELLS, AUDIO_RATE> ctri103(TRI_MAX_103_AT_16384_512_DATA);
Oscil <TRI_MAX_106_AT_16384_512_NUM_CELLS, AUDIO_RATE> ctri106(TRI_MAX_106_AT_16384_512_DATA);
Oscil <TRI_MAX_115_AT_16384_512_NUM_CELLS, AUDIO_RATE> ctri115(TRI_MAX_115_AT_16384_512_DATA);
Oscil <TRI_MAX_130_AT_16384_512_NUM_CELLS, AUDIO_RATE> ctri130(TRI_MAX_130_AT_16384_512_DATA);
Oscil <TRI_MAX_154_AT_16384_512_NUM_CELLS, AUDIO_RATE> ctri154(TRI_MAX_154_AT_16384_512_DATA);
Oscil <TRI_MAX_174_AT_16384_512_NUM_CELLS, AUDIO_RATE> ctri174(TRI_MAX_174_AT_16384_512_DATA);
Oscil <TRI_MAX_210_AT_16384_512_NUM_CELLS, AUDIO_RATE> ctri210(TRI_MAX_210_AT_16384_512_DATA);
Oscil <TRI_MAX_264_AT_16384_512_NUM_CELLS, AUDIO_RATE> ctri264(TRI_MAX_264_AT_16384_512_DATA);
Oscil <TRI_MAX_327_AT_16384_512_NUM_CELLS, AUDIO_RATE> ctri327(TRI_MAX_327_AT_16384_512_DATA);
Oscil <TRI_MAX_431_AT_16384_512_NUM_CELLS, AUDIO_RATE> ctri431(TRI_MAX_431_AT_16384_512_DATA);
Oscil <TRI_MAX_546_AT_16384_512_NUM_CELLS, AUDIO_RATE> ctri546(TRI_MAX_546_AT_16384_512_DATA);
Oscil <TRI_MAX_744_AT_16384_512_NUM_CELLS, AUDIO_RATE> ctri744(TRI_MAX_744_AT_16384_512_DATA);
Oscil <TRI_MAX_910_AT_16384_512_NUM_CELLS, AUDIO_RATE> ctri910(TRI_MAX_910_AT_16384_512_DATA);
Oscil <TRI_MAX_1170_AT_16384_512_NUM_CELLS, AUDIO_RATE> ctri1170(TRI_MAX_1170_AT_16384_512_DATA);
Oscil <TRI_MAX_1638_AT_16384_512_NUM_CELLS, AUDIO_RATE> ctri1638(TRI_MAX_1638_AT_16384_512_DATA);
Oscil <TRI_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> ctri2730(TRI_MAX_2730_AT_16384_512_DATA);
Oscil <TRI_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> ctri8192(TRI_MAX_8192_AT_16384_512_DATA);

// Triangle Wave Oscillator Bank 4
Oscil <TRI_MAX_103_AT_16384_512_NUM_CELLS, AUDIO_RATE> dtri103(TRI_MAX_103_AT_16384_512_DATA);
Oscil <TRI_MAX_106_AT_16384_512_NUM_CELLS, AUDIO_RATE> dtri106(TRI_MAX_106_AT_16384_512_DATA);
Oscil <TRI_MAX_115_AT_16384_512_NUM_CELLS, AUDIO_RATE> dtri115(TRI_MAX_115_AT_16384_512_DATA);
Oscil <TRI_MAX_130_AT_16384_512_NUM_CELLS, AUDIO_RATE> dtri130(TRI_MAX_130_AT_16384_512_DATA);
Oscil <TRI_MAX_154_AT_16384_512_NUM_CELLS, AUDIO_RATE> dtri154(TRI_MAX_154_AT_16384_512_DATA);
Oscil <TRI_MAX_174_AT_16384_512_NUM_CELLS, AUDIO_RATE> dtri174(TRI_MAX_174_AT_16384_512_DATA);
Oscil <TRI_MAX_210_AT_16384_512_NUM_CELLS, AUDIO_RATE> dtri210(TRI_MAX_210_AT_16384_512_DATA);
Oscil <TRI_MAX_264_AT_16384_512_NUM_CELLS, AUDIO_RATE> dtri264(TRI_MAX_264_AT_16384_512_DATA);
Oscil <TRI_MAX_327_AT_16384_512_NUM_CELLS, AUDIO_RATE> dtri327(TRI_MAX_327_AT_16384_512_DATA);
Oscil <TRI_MAX_431_AT_16384_512_NUM_CELLS, AUDIO_RATE> dtri431(TRI_MAX_431_AT_16384_512_DATA);
Oscil <TRI_MAX_546_AT_16384_512_NUM_CELLS, AUDIO_RATE> dtri546(TRI_MAX_546_AT_16384_512_DATA);
Oscil <TRI_MAX_744_AT_16384_512_NUM_CELLS, AUDIO_RATE> dtri744(TRI_MAX_744_AT_16384_512_DATA);
Oscil <TRI_MAX_910_AT_16384_512_NUM_CELLS, AUDIO_RATE> dtri910(TRI_MAX_910_AT_16384_512_DATA);
Oscil <TRI_MAX_1170_AT_16384_512_NUM_CELLS, AUDIO_RATE> dtri1170(TRI_MAX_1170_AT_16384_512_DATA);
Oscil <TRI_MAX_1638_AT_16384_512_NUM_CELLS, AUDIO_RATE> dtri1638(TRI_MAX_1638_AT_16384_512_DATA);
Oscil <TRI_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> dtri2730(TRI_MAX_2730_AT_16384_512_DATA);
Oscil <TRI_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> dtri8192(TRI_MAX_8192_AT_16384_512_DATA);

// Meta-Oscillators for Triangle Waves
MetaOscil<TRI_MAX_103_AT_16384_512_NUM_CELLS, AUDIO_RATE, 17> meta1 {&atri103, &atri106, &atri115, &atri130, &atri154, &atri174, &atri210, &atri264, &atri327, &atri431, &atri546, &atri744, &atri910, &atri1170, &atri1638, &atri2730, &atri8192};
MetaOscil<TRI_MAX_103_AT_16384_512_NUM_CELLS, AUDIO_RATE, 17> meta2 {&btri103, &btri106, &btri115, &btri130, &btri154, &btri174, &btri210, &btri264, &btri327, &btri431, &btri546, &btri744, &btri910, &btri1170, &btri1638, &btri2730, &btri8192};
MetaOscil<TRI_MAX_103_AT_16384_512_NUM_CELLS, AUDIO_RATE, 17> meta3 {&ctri103, &ctri106, &ctri115, &ctri130, &ctri154, &ctri174, &ctri210, &ctri264, &ctri327, &ctri431, &ctri546, &ctri744, &ctri910, &ctri1170, &ctri1638, &ctri2730, &ctri8192};
MetaOscil<TRI_MAX_103_AT_16384_512_NUM_CELLS, AUDIO_RATE, 17> meta4 {&dtri103, &dtri106, &dtri115, &dtri130, &dtri154, &dtri174, &dtri210, &dtri264, &dtri327, &dtri431, &dtri546, &dtri744, &dtri910, &dtri1170, &dtri1638, &dtri2730, &dtri8192};
#endif



// These are the semitone values for the chords.  
// The first number is the number of additional notes beyond the first
uint8_t chords[24][4] = 
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
float semitoneFrequencyRatios[] = 
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



#define CV_POT_IN1    A2    // Mix
#define CV_POT_IN2    A1    // Pitch Tune
#define CV_POT3       A0    // Chord
#define CV_IN3        A3    // Pitch
#define CV_AUDIO_IN   A4    // [Unused]
#define CV_AUDIO_OUT  9     // Out
#define CV_GATE_OUT   8     // [Unused]
#define RANDOM_PIN    A5



void setup() 
    {
#ifdef USE_SQUARE
    meta1.setCutoffFreqs(90, 101, 122, 138, 154, 174, 210, 264, 327, 431, 546, 744, 910, 1170, 1638, 2730, 8192);
    meta2.setCutoffFreqs(90, 101, 122, 138, 154, 174, 210, 264, 327, 431, 546, 744, 910, 1170, 1638, 2730, 8192);
    meta3.setCutoffFreqs(90, 101, 122, 138, 154, 174, 210, 264, 327, 431, 546, 744, 910, 1170, 1638, 2730, 8192);
    meta4.setCutoffFreqs(90, 101, 122, 138, 154, 174, 210, 264, 327, 431, 546, 744, 910, 1170, 1638, 2730, 8192);
#endif
#ifdef USE_SAW
    meta1.setCutoffFreqs(136, 138, 154, 174, 210, 264, 327, 431, 546, 744, 910, 1170, 1638, 2048, 2730, 4096, 8192);
    meta2.setCutoffFreqs(136, 138, 154, 174, 210, 264, 327, 431, 546, 744, 910, 1170, 1638, 2048, 2730, 4096, 8192);
    meta3.setCutoffFreqs(136, 138, 154, 174, 210, 264, 327, 431, 546, 744, 910, 1170, 1638, 2048, 2730, 4096, 8192);
    meta4.setCutoffFreqs(136, 138, 154, 174, 210, 264, 327, 431, 546, 744, 910, 1170, 1638, 2048, 2730, 4096, 8192);
#endif
#ifdef USE_TRI
    meta1.setCutoffFreqs(103, 106, 115, 130, 154, 174, 210, 264, 327, 431, 546, 744, 910, 1170, 1638, 2730, 8192);
    meta2.setCutoffFreqs(103, 106, 115, 130, 154, 174, 210, 264, 327, 431, 546, 744, 910, 1170, 1638, 2730, 8192);
    meta3.setCutoffFreqs(103, 106, 115, 130, 154, 174, 210, 264, 327, 431, 546, 744, 910, 1170, 1638, 2730, 8192);
    meta4.setCutoffFreqs(103, 106, 115, 130, 154, 174, 210, 264, 327, 431, 546, 744, 910, 1170, 1638, 2730, 8192);
#endif
    startMozzi();
    }


// Median-Of-Three Filter for Pitch, dunno if we need it
#define MEDIAN_OF_THREE(a,b,c) (((a) <= (b)) ? (((b) <= (c)) ? (b) : (((a) < (c)) ? (c) : (a))) : (((a) <= (c)) ? (a) : (((b) < (c)) ? (c) : (b))))
uint16_t inA = 0;
uint16_t inB = 0;
uint16_t pitchCV;
uint16_t tuneCV;
uint16_t pitch;
uint8_t chord = 0;
int8_t chordCounter = -1;
uint8_t distortion = 0;
int8_t distortionCounter = -1;
int16_t alpha = 0;

void updateControl() 
    {
    uint8_t d = (mozziAnalogRead(CV_POT3) * 24) >> 10;
    if (d != chord)
        {
        if (chordCounter == -1) chordCounter = 10;
        else if (chordCounter == 0) chord = d;
        chordCounter--;
        }
    else chordCounter = -1;

    alpha = mozziAnalogRead(CV_POT_IN1) >> 2;               // should we filter this?

    uint16_t in = mozziAnalogRead(CV_IN3);
    // let's try a median of three filter to reduce some of the jumpiness
    uint16_t pitchCV = MEDIAN_OF_THREE(in, inA, inB);
    inA = inB;
    inB = in;
    tuneCV = (tuneCV * 7 + mozziAnalogRead(CV_POT_IN2)) >> 3;
    pitch = (pitch * 3 + pitchCV + (tuneCV >> 1)) >> 2;
        
    float frequency = FREQUENCY(pitch);
        
    meta1.setFreq(frequency);
    sine1.setFreq(frequency);
    uint8_t* c = chords[chord];
    if (c[0] >= 1)
        {
        meta2.setFreq(frequency * semitoneFrequencyRatios[c[1]]);
        sine2.setFreq(frequency * semitoneFrequencyRatios[c[1]]);
        if (c[0] >= 2)
            {
            meta3.setFreq(frequency * semitoneFrequencyRatios[c[2]]);
            sine3.setFreq(frequency * semitoneFrequencyRatios[c[2]]);
            if (c[0] >= 3)
                {
                meta4.setFreq(frequency * semitoneFrequencyRatios[c[3]]);
                sine4.setFreq(frequency * semitoneFrequencyRatios[c[3]]);
                }
            }
        }
                

    }

int updateAudio()    
    {
    int16_t sq1 = meta1.next();
    int16_t sq2 = meta2.next();
    int16_t sq3 = meta3.next();
    int16_t sq4 = meta4.next();
    int16_t s1 = sine1.next();
    int16_t s2 = sine2.next();
    int16_t s3 = sine3.next();
    int16_t s4 = sine4.next();
        
    switch (chords[chord][0])
        {
        case 0:
            return (sq1 * alpha + (255 - alpha) * s1) >> 9;
            break;
        case 1:
            return ((int8_t)(((sq1 + sq2) * (alpha >> 1) + ((255 - alpha) >> 1)*(s1 + s2)) >> 8));
            break;
        case 2:
            sq4 = sq3 >> 1;
            sq3 = sq4;
            // FALL THRU
        case 3:
            return ((int8_t)(((sq1 + sq2 + sq3 + sq4) * (alpha >> 2) + ((255 - alpha) >> 2) * (s1 + s2 + s3 + s4)) >> 8));
            break;
        }
    }


void loop() 
    {
    audioHook();
    }
