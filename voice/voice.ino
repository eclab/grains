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


/// VOICE
///
/// Voice is a complete synthesizer voice, with an oscillator, a filter, and an amplifier.
/// Voice is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// Voice has an saw, square, or triangle oscillator, four filter options, and an amplifier.  
/// You can control the pitch, amplitude, filter cutoff, and resonance with CV values.  
/// You can also control the amplitude and filter cutoff with the pots, and you can tune
/// the pitch (+/-) with a pot.  You cannot control resonance from the pot (not enough pots!)
///
/// You can choose from square, triangle, and sawtooth waves, and from low-pass, high-pass, 
/// band-pass, and notch filter types.  There is no pulse width or PWM.
/// The filter is a simple two-pole filter consisting of two one-pole filters plus resonance.
/// It's what's provided by the Mozzi distribution but it's not great, particularly at high
/// frequencies and high resonance.  I might replace it with my own filter later.
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
/// IN 1            Filter Cutoff CV
/// IN 2            Amplitude CV
/// IN 3            Pitch CV
/// AUDIO IN (A)    Resonance CV 
/// AUDIO OUT       Out
/// DIGITAL OUT (D) [Unused]
///
/// POT 1           Fiter Cutoff
///
/// POT 2           Amplitude
///
/// POT 3           Pitch Tune


/// CHOOSING SAW VS SQUARE VS TRIANGLE
///
/// The choice of wave is determined by the #define used below.  Notice that two are commented out
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
//#define USE_SAW
//#define USE_TRI



/// CHOOSING THE FILTER TYPE
///
/// The choice of filter type is determined by the #define used below.  Notice that three are 
/// commented out as // #define ...   To select a fitler type, remove the // but make sure 
/// that the other two filter types have been commented out (they have // in front of them).  
/// For example, to use high pass, you'd change things to look like this:
///
///    // #define RESONANT_FILTER_TYPE 	LOWPASS
///    #define RESONANT_FILTER_TYPE 	HIGHPASS
///    // #define RESONANT_FILTER_TYPE 	BANDPASS
///    // #define RESONANT_FILTER_TYPE 	NOTCH
///
/// Set it right here:


#define RESONANT_FILTER_TYPE 	LOWPASS
// #define RESONANT_FILTER_TYPE 	HIGHPASS
// #define RESONANT_FILTER_TYPE 	BANDPASS
// #define RESONANT_FILTER_TYPE 	NOTCH



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
#define CONTROL_RATE 256

#include "tracking.h"
#include <MozziGuts.h>
#include <Oscil.h>
#include <MetaOscil.h>
#include <ResonantFilter.h>

#include <tables/BandLimited_SQUARE/512/square_max_101_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_103_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_106_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_109_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_112_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_115_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_1170_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_118_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_122_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_126_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_130_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_134_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_138_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_143_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_148_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_154_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_160_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_1638_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_167_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_174_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_182_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_190_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_199_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_210_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_221_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_234_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_248_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_264_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_2730_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_282_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_303_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_327_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_356_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_390_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_431_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_481_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_546_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_630_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_68_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_70_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_71_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_72_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_73_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_744_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_75_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_76_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_78_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_79_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_8192_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_81_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_82_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_84_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_86_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_88_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_90_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_910_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_92_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_94_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_96_at_16384_512_int8.h>
#include <tables/BandLimited_SQUARE/512/square_max_98_at_16384_512_int8.h>



#include <tables/BandLimited_SAW/512/saw_max_1024_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_1170_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_1365_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_136_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_138_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_141_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_143_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_146_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_148_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_151_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_154_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_157_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_160_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_1638_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_163_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_167_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_170_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_174_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_178_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_182_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_186_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_190_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_195_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_199_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_2048_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_204_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_210_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_215_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_221_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_227_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_234_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_240_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_248_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_256_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_264_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_2730_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_273_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_282_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_292_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_303_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_315_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_327_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_341_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_356_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_372_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_390_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_4096_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_409_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_431_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_455_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_481_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_512_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_546_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_585_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_630_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_682_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_744_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_8192_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_819_at_16384_512_int8.h>
#include <tables/BandLimited_SAW/512/saw_max_910_at_16384_512_int8.h>


#include <tables/BandLimited_TRI/512/tri_max_103_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_106_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_109_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_112_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_115_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_1170_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_118_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_122_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_126_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_130_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_134_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_138_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_143_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_148_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_154_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_160_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_1638_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_167_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_174_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_182_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_190_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_199_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_210_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_221_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_234_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_248_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_264_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_2730_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_282_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_303_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_327_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_356_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_390_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_431_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_481_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_546_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_630_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_744_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_8192_at_16384_512_int8.h>
#include <tables/BandLimited_TRI/512/tri_max_910_at_16384_512_int8.h>


/// OSCILLATORS

/// We can cram in 35 oscillators before we run out of memory.  I've tried to do reasonable selections

#ifdef USE_SQUARE
Oscil <SQUARE_MAX_70_AT_16384_512_NUM_CELLS, AUDIO_RATE> square70(SQUARE_MAX_70_AT_16384_512_DATA);
Oscil <SQUARE_MAX_71_AT_16384_512_NUM_CELLS, AUDIO_RATE> square71(SQUARE_MAX_71_AT_16384_512_DATA);
Oscil <SQUARE_MAX_73_AT_16384_512_NUM_CELLS, AUDIO_RATE> square73(SQUARE_MAX_73_AT_16384_512_DATA);
Oscil <SQUARE_MAX_75_AT_16384_512_NUM_CELLS, AUDIO_RATE> square75(SQUARE_MAX_75_AT_16384_512_DATA);
Oscil <SQUARE_MAX_79_AT_16384_512_NUM_CELLS, AUDIO_RATE> square79(SQUARE_MAX_79_AT_16384_512_DATA);
Oscil <SQUARE_MAX_82_AT_16384_512_NUM_CELLS, AUDIO_RATE> square82(SQUARE_MAX_82_AT_16384_512_DATA);
Oscil <SQUARE_MAX_84_AT_16384_512_NUM_CELLS, AUDIO_RATE> square84(SQUARE_MAX_84_AT_16384_512_DATA);
Oscil <SQUARE_MAX_90_AT_16384_512_NUM_CELLS, AUDIO_RATE> square90(SQUARE_MAX_90_AT_16384_512_DATA);
Oscil <SQUARE_MAX_94_AT_16384_512_NUM_CELLS, AUDIO_RATE> square94(SQUARE_MAX_94_AT_16384_512_DATA);
Oscil <SQUARE_MAX_101_AT_16384_512_NUM_CELLS, AUDIO_RATE> square101(SQUARE_MAX_101_AT_16384_512_DATA);
Oscil <SQUARE_MAX_103_AT_16384_512_NUM_CELLS, AUDIO_RATE> square103(SQUARE_MAX_103_AT_16384_512_DATA);
Oscil <SQUARE_MAX_112_AT_16384_512_NUM_CELLS, AUDIO_RATE> square112(SQUARE_MAX_112_AT_16384_512_DATA);
Oscil <SQUARE_MAX_118_AT_16384_512_NUM_CELLS, AUDIO_RATE> square118(SQUARE_MAX_118_AT_16384_512_DATA);
Oscil <SQUARE_MAX_122_AT_16384_512_NUM_CELLS, AUDIO_RATE> square122(SQUARE_MAX_122_AT_16384_512_DATA);
Oscil <SQUARE_MAX_130_AT_16384_512_NUM_CELLS, AUDIO_RATE> square130(SQUARE_MAX_130_AT_16384_512_DATA);
Oscil <SQUARE_MAX_134_AT_16384_512_NUM_CELLS, AUDIO_RATE> square134(SQUARE_MAX_134_AT_16384_512_DATA);
Oscil <SQUARE_MAX_143_AT_16384_512_NUM_CELLS, AUDIO_RATE> square143(SQUARE_MAX_143_AT_16384_512_DATA);
Oscil <SQUARE_MAX_148_AT_16384_512_NUM_CELLS, AUDIO_RATE> square148(SQUARE_MAX_148_AT_16384_512_DATA);
Oscil <SQUARE_MAX_160_AT_16384_512_NUM_CELLS, AUDIO_RATE> square160(SQUARE_MAX_160_AT_16384_512_DATA);
Oscil <SQUARE_MAX_167_AT_16384_512_NUM_CELLS, AUDIO_RATE> square167(SQUARE_MAX_167_AT_16384_512_DATA);
Oscil <SQUARE_MAX_182_AT_16384_512_NUM_CELLS, AUDIO_RATE> square182(SQUARE_MAX_182_AT_16384_512_DATA);
Oscil <SQUARE_MAX_190_AT_16384_512_NUM_CELLS, AUDIO_RATE> square190(SQUARE_MAX_190_AT_16384_512_DATA);
Oscil <SQUARE_MAX_210_AT_16384_512_NUM_CELLS, AUDIO_RATE> square210(SQUARE_MAX_210_AT_16384_512_DATA);
Oscil <SQUARE_MAX_221_AT_16384_512_NUM_CELLS, AUDIO_RATE> square221(SQUARE_MAX_221_AT_16384_512_DATA);
Oscil <SQUARE_MAX_248_AT_16384_512_NUM_CELLS, AUDIO_RATE> square248(SQUARE_MAX_248_AT_16384_512_DATA);
Oscil <SQUARE_MAX_264_AT_16384_512_NUM_CELLS, AUDIO_RATE> square264(SQUARE_MAX_264_AT_16384_512_DATA);
Oscil <SQUARE_MAX_303_AT_16384_512_NUM_CELLS, AUDIO_RATE> square303(SQUARE_MAX_303_AT_16384_512_DATA);
Oscil <SQUARE_MAX_327_AT_16384_512_NUM_CELLS, AUDIO_RATE> square327(SQUARE_MAX_327_AT_16384_512_DATA);
Oscil <SQUARE_MAX_390_AT_16384_512_NUM_CELLS, AUDIO_RATE> square390(SQUARE_MAX_390_AT_16384_512_DATA);
Oscil <SQUARE_MAX_431_AT_16384_512_NUM_CELLS, AUDIO_RATE> square431(SQUARE_MAX_431_AT_16384_512_DATA);
Oscil <SQUARE_MAX_546_AT_16384_512_NUM_CELLS, AUDIO_RATE> square546(SQUARE_MAX_546_AT_16384_512_DATA);
Oscil <SQUARE_MAX_630_AT_16384_512_NUM_CELLS, AUDIO_RATE> square630(SQUARE_MAX_630_AT_16384_512_DATA);
Oscil <SQUARE_MAX_1170_AT_16384_512_NUM_CELLS, AUDIO_RATE> square1170(SQUARE_MAX_1170_AT_16384_512_DATA);
Oscil <SQUARE_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> square2730(SQUARE_MAX_2730_AT_16384_512_DATA);
Oscil <SQUARE_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> square8192(SQUARE_MAX_8192_AT_16384_512_DATA);
MetaOscil<SQUARE_MAX_90_AT_16384_512_NUM_CELLS, AUDIO_RATE, 35> meta {&square70, &square71, &square73, &square75, &square79, &square82, &square84, &square90, &square94, &square101, &square103, &square112, &square118, &square122, &square130, &square134, &square143, &square148, &square160, &square167, &square182, &square190, &square210, &square221, &square248, &square264, &square303, &square327, &square390, &square431, &square546, &square630, &square1170, &square2730, &square8192};
#endif

#ifdef USE_SAW
//Oscil <SAW_MAX_138_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw138(SAW_MAX_138_AT_16384_512_DATA);
//Oscil <SAW_MAX_141_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw141(SAW_MAX_141_AT_16384_512_DATA);
Oscil <SAW_MAX_146_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw146(SAW_MAX_146_AT_16384_512_DATA);
Oscil <SAW_MAX_151_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw151(SAW_MAX_151_AT_16384_512_DATA);
Oscil <SAW_MAX_154_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw154(SAW_MAX_154_AT_16384_512_DATA);
Oscil <SAW_MAX_157_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw157(SAW_MAX_157_AT_16384_512_DATA);
Oscil <SAW_MAX_163_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw163(SAW_MAX_163_AT_16384_512_DATA);
Oscil <SAW_MAX_167_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw167(SAW_MAX_167_AT_16384_512_DATA);
Oscil <SAW_MAX_174_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw174(SAW_MAX_174_AT_16384_512_DATA);
Oscil <SAW_MAX_178_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw178(SAW_MAX_178_AT_16384_512_DATA);
Oscil <SAW_MAX_186_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw186(SAW_MAX_186_AT_16384_512_DATA);
Oscil <SAW_MAX_190_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw190(SAW_MAX_190_AT_16384_512_DATA);
Oscil <SAW_MAX_199_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw199(SAW_MAX_199_AT_16384_512_DATA);
Oscil <SAW_MAX_204_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw204(SAW_MAX_204_AT_16384_512_DATA);
Oscil <SAW_MAX_215_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw215(SAW_MAX_215_AT_16384_512_DATA);
Oscil <SAW_MAX_221_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw221(SAW_MAX_221_AT_16384_512_DATA);
Oscil <SAW_MAX_234_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw234(SAW_MAX_234_AT_16384_512_DATA);
Oscil <SAW_MAX_240_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw240(SAW_MAX_240_AT_16384_512_DATA);
Oscil <SAW_MAX_256_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw256(SAW_MAX_256_AT_16384_512_DATA);
Oscil <SAW_MAX_264_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw264(SAW_MAX_264_AT_16384_512_DATA);
Oscil <SAW_MAX_282_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw282(SAW_MAX_282_AT_16384_512_DATA);
Oscil <SAW_MAX_292_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw292(SAW_MAX_292_AT_16384_512_DATA);
Oscil <SAW_MAX_315_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw315(SAW_MAX_315_AT_16384_512_DATA);
Oscil <SAW_MAX_327_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw327(SAW_MAX_327_AT_16384_512_DATA);
Oscil <SAW_MAX_356_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw356(SAW_MAX_356_AT_16384_512_DATA);
Oscil <SAW_MAX_372_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw372(SAW_MAX_372_AT_16384_512_DATA);
Oscil <SAW_MAX_409_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw409(SAW_MAX_409_AT_16384_512_DATA);
Oscil <SAW_MAX_431_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw431(SAW_MAX_431_AT_16384_512_DATA);
Oscil <SAW_MAX_481_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw481(SAW_MAX_481_AT_16384_512_DATA);
Oscil <SAW_MAX_512_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw512(SAW_MAX_512_AT_16384_512_DATA);
Oscil <SAW_MAX_546_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw546(SAW_MAX_546_AT_16384_512_DATA);
Oscil <SAW_MAX_585_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw585(SAW_MAX_585_AT_16384_512_DATA);
Oscil <SAW_MAX_682_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw682(SAW_MAX_682_AT_16384_512_DATA);
Oscil <SAW_MAX_910_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw910(SAW_MAX_910_AT_16384_512_DATA);  
Oscil <SAW_MAX_1638_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw1638(SAW_MAX_1638_AT_16384_512_DATA); 
Oscil <SAW_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw2730(SAW_MAX_2730_AT_16384_512_DATA); 
Oscil <SAW_MAX_4096_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw4096(SAW_MAX_4096_AT_16384_512_DATA);
Oscil <SAW_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw8192(SAW_MAX_8192_AT_16384_512_DATA);
MetaOscil<SAW_MAX_136_AT_16384_512_NUM_CELLS, AUDIO_RATE, 35> meta {&saw146, &saw151, &saw157, &saw163, &saw167, &saw174, &saw178, &saw186, &saw190, &saw199, &saw204, &saw215, &saw221, &saw234, &saw240, &saw256, &saw264, &saw282, &saw292, &saw315, &saw327, &saw356, &saw372, &saw409, &saw431, &saw481, &saw512, &saw546, &saw585, &saw682, &saw910, &saw1638, &saw2730, &saw4096, &saw8192};
#endif

#ifdef USE_TRI
Oscil <TRI_MAX_106_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri103(TRI_MAX_103_AT_16384_512_DATA);
Oscil <TRI_MAX_106_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri106(TRI_MAX_106_AT_16384_512_DATA);
Oscil <TRI_MAX_109_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri109(TRI_MAX_109_AT_16384_512_DATA);
Oscil <TRI_MAX_112_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri112(TRI_MAX_112_AT_16384_512_DATA);
Oscil <TRI_MAX_115_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri115(TRI_MAX_115_AT_16384_512_DATA);
Oscil <TRI_MAX_118_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri118(TRI_MAX_118_AT_16384_512_DATA);
Oscil <TRI_MAX_122_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri122(TRI_MAX_122_AT_16384_512_DATA);
Oscil <TRI_MAX_126_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri126(TRI_MAX_126_AT_16384_512_DATA);
Oscil <TRI_MAX_130_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri130(TRI_MAX_130_AT_16384_512_DATA);
Oscil <TRI_MAX_134_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri134(TRI_MAX_134_AT_16384_512_DATA);
Oscil <TRI_MAX_138_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri138(TRI_MAX_138_AT_16384_512_DATA);
Oscil <TRI_MAX_143_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri143(TRI_MAX_143_AT_16384_512_DATA);
Oscil <TRI_MAX_148_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri148(TRI_MAX_148_AT_16384_512_DATA);
Oscil <TRI_MAX_154_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri154(TRI_MAX_154_AT_16384_512_DATA);
Oscil <TRI_MAX_160_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri160(TRI_MAX_160_AT_16384_512_DATA);
Oscil <TRI_MAX_167_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri167(TRI_MAX_167_AT_16384_512_DATA);
Oscil <TRI_MAX_174_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri174(TRI_MAX_174_AT_16384_512_DATA);
Oscil <TRI_MAX_182_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri182(TRI_MAX_182_AT_16384_512_DATA);
Oscil <TRI_MAX_190_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri190(TRI_MAX_190_AT_16384_512_DATA);
Oscil <TRI_MAX_199_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri199(TRI_MAX_199_AT_16384_512_DATA);
Oscil <TRI_MAX_210_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri210(TRI_MAX_210_AT_16384_512_DATA);
Oscil <TRI_MAX_221_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri221(TRI_MAX_221_AT_16384_512_DATA);
Oscil <TRI_MAX_234_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri234(TRI_MAX_234_AT_16384_512_DATA);
Oscil <TRI_MAX_248_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri248(TRI_MAX_248_AT_16384_512_DATA);
Oscil <TRI_MAX_264_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri264(TRI_MAX_264_AT_16384_512_DATA);
Oscil <TRI_MAX_282_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri282(TRI_MAX_282_AT_16384_512_DATA);
Oscil <TRI_MAX_303_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri303(TRI_MAX_303_AT_16384_512_DATA);
Oscil <TRI_MAX_327_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri327(TRI_MAX_327_AT_16384_512_DATA);
Oscil <TRI_MAX_356_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri356(TRI_MAX_356_AT_16384_512_DATA);
Oscil <TRI_MAX_390_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri390(TRI_MAX_390_AT_16384_512_DATA);
Oscil <TRI_MAX_431_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri431(TRI_MAX_431_AT_16384_512_DATA);
Oscil <TRI_MAX_546_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri546(TRI_MAX_546_AT_16384_512_DATA);
Oscil <TRI_MAX_910_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri910(TRI_MAX_910_AT_16384_512_DATA);
Oscil <TRI_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri2730(TRI_MAX_2730_AT_16384_512_DATA);
Oscil <TRI_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri8192(TRI_MAX_8192_AT_16384_512_DATA);
MetaOscil<TRI_MAX_103_AT_16384_512_NUM_CELLS, AUDIO_RATE, 35> meta {&tri103, &tri106, &tri109, &tri112, &tri115, &tri118, &tri122, &tri126, &tri130, &tri134, &tri138, &tri143, &tri148, &tri154, &tri160, &tri167, &tri174, &tri182, &tri190, &tri199, &tri210, &tri221, &tri234, &tri248, &tri264, &tri282, &tri303, &tri327, &tri356, &tri390, &tri431, &tri546, &tri910, &tri2730, &tri8192};
#endif


#define CV_POT_IN1    A2    // Filter Cutoff
#define CV_POT_IN2    A1    // Amplitude
#define CV_POT3       A0    // Pitch CV
#define CV_IN3        A3    // Pitch
#define CV_AUDIO_IN   A4    // Resonance CV
#define CV_AUDIO_OUT  9     // Out
#define CV_GATE_OUT   8     // [Unused]
#define RANDOM_PIN    A5


ResonantFilter<RESONANT_FILTER_TYPE,uint16_t> filter;


// Median-Of-Three Filter for Pitch, dunno if we need it
#define MEDIAN_OF_THREE(a,b,c) (((a) <= (b)) ? (((b) <= (c)) ? (b) : (((a) < (c)) ? (c) : (a))) : (((a) <= (c)) ? (a) : (((b) < (c)) ? (c) : (b))))
uint16_t inA = 0;
uint16_t inB = 0;
uint16_t pitchCV;
uint16_t tuneCV;
uint16_t pitch;
int32_t amplitude;


void setup() 
    {
#ifdef USE_SQUARE
    meta.setCutoffFreqs(70, 71, 73, 75, 79, 82, 84, 90, 94, 101, 103, 112, 118, 122, 130, 134, 143, 148, 160, 167, 182, 190, 210, 221, 248, 264, 303, 327, 390, 431, 546, 630, 1170, 2730, 8192);
#endif
#ifdef USE_SAW
    meta.setCutoffFreqs(146, 151, 157, 163, 167, 174, 178, 186, 190, 199, 204, 215, 221, 234, 240, 256, 264, 282, 292, 315, 327, 356, 372, 409, 431, 481, 512, 546, 585, 682, 910, 1638, 2730, 4096, 8192);
#endif
#ifdef USE_TRI
    meta.setCutoffFreqs(103, 106, 109, 112, 115, 118, 122, 126, 130, 134, 138, 143, 148, 154, 160, 167, 174, 182, 190, 199, 210, 221, 234, 248, 264, 282, 303, 327, 356, 390, 431, 546, 910, 2730, 8192);
#endif
    // some reasonable initial defaults
    filter.setCutoffFreqAndResonance(0, 65535);
    amplitude = 1024;

    startMozzi();
    }


void updateControl() 
    {
    uint16_t in = mozziAnalogRead(CV_IN3);
    // let's try a median of three filter to reduce some of the jumpiness
    uint16_t pitchCV = MEDIAN_OF_THREE(in, inA, inB);
    inA = inB;
    inB = in;
    tuneCV = (tuneCV * 7 + mozziAnalogRead(CV_POT3)) >> 3;
    pitch = (pitch * 3 + pitchCV + (tuneCV >> 1)) >> 2;
    meta.setFreq(FREQUENCY(pitch));

    // Not sure we should bother filtering these, probably not
    amplitude = mozziAnalogRead(CV_POT_IN2);
    uint16_t filterCutoff = mozziAnalogRead(CV_POT_IN1) << 6;
    uint16_t resonance = mozziAnalogRead(CV_AUDIO_IN)  << 6;
    filter.setCutoffFreqAndResonance(filterCutoff, resonance); 
    }

int updateAudio()    
    {
    return (int16_t)(((filter.next(meta.next())) * amplitude) >> 10);
    }


void loop() 
    {
    audioHook();
    }
