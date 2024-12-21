// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// PARA
///
/// Para is a MIDI-driven 3-voice Paraphonic synthesizer with a mixed square/saw/triangle and sine waves.
/// Para is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// Para can select between square and sine, saw and sine, or triangle and sine.  You select
/// which one you want in the code.  The default is saw and sine.
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.

/// Para also sends a Gate via IN 3 whenever all of its notes have been released and one
/// new one has been started.  This means it won't send a gate if you release only one note,
/// then play a new one, while other notes are held down.  Various grooveboxes (such as the
/// Polyend Tracker) behave like this when playing all new notes suddenly, so you need to put
/// a small timestep between the previous chord's NOTE OFF and the new notes ON.

/// GATE MODE
///
/// By default Para does not stop playing notes when it receives a NOTE OFF -- 
/// that is the responsibility of your envelope and VCA (it will send them a gate off).
/// However you can change this behavior by uncommenting the following #define:

/// #define GATE    


/// CHANNEL
///
/// You can set Para's channel to 0...15 (for channels 1...16) or to OMNI (to listen to any of them).
/// This is set here:

// Set this to 0...15, or to OMNI
#define CHANNEL OMNI


/// MULTI MODE
/// 
/// Normally Para plays chords using the given channel.  But instead you can set it to play one note
/// each from a different channel, like MPE.  The channels will be CHANNEL, CHANNEL+1, and CHANNEL+2.
/// Thus you should only set CHANNEL to values 0...13.  Do not set CHANNEL to OMNI.  MULTI mode is turned
/// on by uncommenting the following #define:

// #define MULTI


/// MONO VS PARA
///
/// Para has three voices.  Mono has only one.
/// However Mono has twice as many waves in its memory, so it will sound less aliased for many notes.

/// CHOOSING SAW VS SQUARE VS TRIANGLE
///
/// The choice of wave is determined by the #define used below.  Notice that two are commented out
/// as // #define ...   To select a wave, remove the // but make sure that the other two waves
/// have been commented out (they have // in front of them).  For example, to use square, you'd
/// change things to look like this:
///
///    #define USE_SQUARE
///    // #define USE_SAW
///    // #define USE_TRI
///
/// Set it right here:


//#define USE_SQUARE
 #define USE_SAW
// #define USE_TRI


/// MIDI RESPONSE
///
/// MIDI NOTES 		Note On/Off
///						Pitch: All notes C0 through G10, but realistically C0 through B8
///						Velocity: Velocity 0...127 for Note On.  Velocity 0 is a Note Off
///					
/// CC				All Notes Off		CC 123		[Resets all notes, lowers Gate]
///					All Sounds Off		CC 120		[Resets all notes, lowers Gate]


/// CONFIGURATION
///
/// IN 1            Saw/Square/Tri Vs Sine CV
/// IN 2            [Unused]
/// IN 3            Gate Out
/// AUDIO IN (A)    [Unused]  
/// AUDIO OUT       Out
/// DIGITAL OUT (D) MIDI In
///
/// POT 1           Saw/Square/Tri Vs Sine
///					[If not using CV, set switch to MAN, else set switch to IN1 and set pot to about 2'oclock] 
///
/// POT 2           [Unused]
///
/// POT 3           Gain


PROGMEM const float frequencies[128] =
{
16.352, 17.3243, 18.3545, 19.4459, 20.6022, 21.8273, 23.1252,
24.5003, 25.9572, 27.5007, 29.136, 30.8685, 32.704, 34.6487, 36.709,
38.8918, 41.2045, 43.6546, 46.2504, 49.0006, 51.9144, 55.0014,
58.2719, 61.7369, 65.408, 69.2974, 73.418, 77.7837, 82.4089, 87.3092,
92.5009, 98.0013, 103.829, 110.003, 116.544, 123.474, 130.816,
138.595, 146.836, 155.567, 164.818, 174.618, 185.002, 196.003,
207.657, 220.005, 233.088, 246.948, 261.632, 277.189, 293.672,
311.135, 329.636, 349.237, 370.004, 392.005, 415.315, 440.011,
466.175, 493.895, 523.264, 554.379, 587.344, 622.269, 659.271,
698.474, 740.007, 784.01, 830.63, 880.022, 932.35, 987.791, 1046.53,
1108.76, 1174.69, 1244.54, 1318.54, 1396.95, 1480.01, 1568.02,
1661.26, 1760.04, 1864.7, 1975.58, 2093.06, 2217.52, 2349.38,
2489.08, 2637.09, 2793.89, 2960.03, 3136.04, 3322.52, 3520.09,
3729.4, 3951.16, 4186.11, 4435.03, 4698.75, 4978.15, 5274.17,
5587.79, 5920.06, 6272.08, 6645.04, 7040.17, 7458.8, 7902.33,
// Obviously beyond this it's moot
8372.22, 8870.06, 9397.5, 9956.31, 10548.3, 11175.6, 11840.1,
12544.2, 13290.1, 14080.3, 14917.6, 15804.7, 16744.4, 17740.1,
18795., 19912.6, 21096.7, 22351.2, 23680.2, 25088.3 
};



#define FREQUENCY(pitch) pgm_read_float_near(&frequencies[pitch])
#define CONTROL_RATE 256

#define CV_POT_IN1    A2    // Note In, Pitch Scaling                           // Mix
#define CV_POT_IN2    A1    // Mix                                                                      // Pitch Tune
#define CV_POT3       A0    // Chord
#define CV_IN3        A3    // [Unused]
#define CV_AUDIO_IN   A4    // Pitch Tune                                                       // [Unused]
#define CV_AUDIO_OUT  9     // Out
#define CV_GATE_OUT   8     // [Unused]
#define RANDOM_PIN    A5



#include <MozziGuts.h>
#include <Oscil.h>
#include <MetaOscil.h>

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

#include <tables/sin512_int8.h>

#include "NeoSWSerial.h"   // you have to install this via the library manager
#include "parsemidi.c"

#define MIDI_RATE 31250
#define BLANK_SERIAL	  5		// Blank Serial Pin
#define PIN_UNUSED 255
NeoSWSerial softSerial(CV_GATE_OUT, BLANK_SERIAL, PIN_UNUSED);
midiParser parse;

/// OSCILLATORS

// 4 Sine Wave Oscillators
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> sine1(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> sine2(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> sine3(SIN512_DATA);

#ifdef USE_SQUARE
// Square Wave Oscillator Bank 1
Oscil <SQUARE_MAX_68_AT_16384_512_NUM_CELLS, AUDIO_RATE> square68(SQUARE_MAX_68_AT_16384_512_DATA);
Oscil <SQUARE_MAX_72_AT_16384_512_NUM_CELLS, AUDIO_RATE> square72(SQUARE_MAX_72_AT_16384_512_DATA);
Oscil <SQUARE_MAX_76_AT_16384_512_NUM_CELLS, AUDIO_RATE> square76(SQUARE_MAX_76_AT_16384_512_DATA);
Oscil <SQUARE_MAX_81_AT_16384_512_NUM_CELLS, AUDIO_RATE> square81(SQUARE_MAX_81_AT_16384_512_DATA);
Oscil <SQUARE_MAX_86_AT_16384_512_NUM_CELLS, AUDIO_RATE> square86(SQUARE_MAX_86_AT_16384_512_DATA);
Oscil <SQUARE_MAX_92_AT_16384_512_NUM_CELLS, AUDIO_RATE> square92(SQUARE_MAX_92_AT_16384_512_DATA);
Oscil <SQUARE_MAX_98_AT_16384_512_NUM_CELLS, AUDIO_RATE> square98(SQUARE_MAX_98_AT_16384_512_DATA);
Oscil <SQUARE_MAX_106_AT_16384_512_NUM_CELLS, AUDIO_RATE> square106(SQUARE_MAX_106_AT_16384_512_DATA);
Oscil <SQUARE_MAX_115_AT_16384_512_NUM_CELLS, AUDIO_RATE> square115(SQUARE_MAX_115_AT_16384_512_DATA);
Oscil <SQUARE_MAX_126_AT_16384_512_NUM_CELLS, AUDIO_RATE> square126(SQUARE_MAX_126_AT_16384_512_DATA);
Oscil <SQUARE_MAX_138_AT_16384_512_NUM_CELLS, AUDIO_RATE> square138(SQUARE_MAX_138_AT_16384_512_DATA);
Oscil <SQUARE_MAX_154_AT_16384_512_NUM_CELLS, AUDIO_RATE> square154(SQUARE_MAX_154_AT_16384_512_DATA);
Oscil <SQUARE_MAX_174_AT_16384_512_NUM_CELLS, AUDIO_RATE> square174(SQUARE_MAX_174_AT_16384_512_DATA);
Oscil <SQUARE_MAX_199_AT_16384_512_NUM_CELLS, AUDIO_RATE> square199(SQUARE_MAX_199_AT_16384_512_DATA);
Oscil <SQUARE_MAX_234_AT_16384_512_NUM_CELLS, AUDIO_RATE> square234(SQUARE_MAX_234_AT_16384_512_DATA);
Oscil <SQUARE_MAX_282_AT_16384_512_NUM_CELLS, AUDIO_RATE> square282(SQUARE_MAX_282_AT_16384_512_DATA);
Oscil <SQUARE_MAX_356_AT_16384_512_NUM_CELLS, AUDIO_RATE> square356(SQUARE_MAX_356_AT_16384_512_DATA);
Oscil <SQUARE_MAX_481_AT_16384_512_NUM_CELLS, AUDIO_RATE> square481(SQUARE_MAX_481_AT_16384_512_DATA);
Oscil <SQUARE_MAX_744_AT_16384_512_NUM_CELLS, AUDIO_RATE> square744(SQUARE_MAX_744_AT_16384_512_DATA);
Oscil <SQUARE_MAX_1638_AT_16384_512_NUM_CELLS, AUDIO_RATE> square1638(SQUARE_MAX_1638_AT_16384_512_DATA);
Oscil <SQUARE_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> square2730(SQUARE_MAX_2730_AT_16384_512_DATA);
Oscil <SQUARE_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> square8192(SQUARE_MAX_8192_AT_16384_512_DATA);


// Square Wave Oscillator Bank 2
Oscil <SQUARE_MAX_68_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare68(SQUARE_MAX_68_AT_16384_512_DATA);
Oscil <SQUARE_MAX_72_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare72(SQUARE_MAX_72_AT_16384_512_DATA);
Oscil <SQUARE_MAX_76_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare76(SQUARE_MAX_76_AT_16384_512_DATA);
Oscil <SQUARE_MAX_81_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare81(SQUARE_MAX_81_AT_16384_512_DATA);
Oscil <SQUARE_MAX_86_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare86(SQUARE_MAX_86_AT_16384_512_DATA);
Oscil <SQUARE_MAX_92_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare92(SQUARE_MAX_92_AT_16384_512_DATA);
Oscil <SQUARE_MAX_98_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare98(SQUARE_MAX_98_AT_16384_512_DATA);
Oscil <SQUARE_MAX_106_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare106(SQUARE_MAX_106_AT_16384_512_DATA);
Oscil <SQUARE_MAX_115_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare115(SQUARE_MAX_115_AT_16384_512_DATA);
Oscil <SQUARE_MAX_126_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare126(SQUARE_MAX_126_AT_16384_512_DATA);
Oscil <SQUARE_MAX_138_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare138(SQUARE_MAX_138_AT_16384_512_DATA);
Oscil <SQUARE_MAX_154_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare154(SQUARE_MAX_154_AT_16384_512_DATA);
Oscil <SQUARE_MAX_174_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare174(SQUARE_MAX_174_AT_16384_512_DATA);
Oscil <SQUARE_MAX_199_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare199(SQUARE_MAX_199_AT_16384_512_DATA);
Oscil <SQUARE_MAX_234_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare234(SQUARE_MAX_234_AT_16384_512_DATA);
Oscil <SQUARE_MAX_282_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare282(SQUARE_MAX_282_AT_16384_512_DATA);
Oscil <SQUARE_MAX_356_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare356(SQUARE_MAX_356_AT_16384_512_DATA);
Oscil <SQUARE_MAX_481_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare481(SQUARE_MAX_481_AT_16384_512_DATA);
Oscil <SQUARE_MAX_744_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare744(SQUARE_MAX_744_AT_16384_512_DATA);
Oscil <SQUARE_MAX_1638_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare1638(SQUARE_MAX_1638_AT_16384_512_DATA);
Oscil <SQUARE_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare2730(SQUARE_MAX_2730_AT_16384_512_DATA);
Oscil <SQUARE_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> asquare8192(SQUARE_MAX_8192_AT_16384_512_DATA);

// Square Wave Oscillator Bank 3
Oscil <SQUARE_MAX_68_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare68(SQUARE_MAX_68_AT_16384_512_DATA);
Oscil <SQUARE_MAX_72_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare72(SQUARE_MAX_72_AT_16384_512_DATA);
Oscil <SQUARE_MAX_76_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare76(SQUARE_MAX_76_AT_16384_512_DATA);
Oscil <SQUARE_MAX_81_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare81(SQUARE_MAX_81_AT_16384_512_DATA);
Oscil <SQUARE_MAX_86_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare86(SQUARE_MAX_86_AT_16384_512_DATA);
Oscil <SQUARE_MAX_92_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare92(SQUARE_MAX_92_AT_16384_512_DATA);
Oscil <SQUARE_MAX_98_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare98(SQUARE_MAX_98_AT_16384_512_DATA);
Oscil <SQUARE_MAX_106_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare106(SQUARE_MAX_106_AT_16384_512_DATA);
Oscil <SQUARE_MAX_115_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare115(SQUARE_MAX_115_AT_16384_512_DATA);
Oscil <SQUARE_MAX_126_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare126(SQUARE_MAX_126_AT_16384_512_DATA);
Oscil <SQUARE_MAX_138_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare138(SQUARE_MAX_138_AT_16384_512_DATA);
Oscil <SQUARE_MAX_154_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare154(SQUARE_MAX_154_AT_16384_512_DATA);
Oscil <SQUARE_MAX_174_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare174(SQUARE_MAX_174_AT_16384_512_DATA);
Oscil <SQUARE_MAX_199_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare199(SQUARE_MAX_199_AT_16384_512_DATA);
Oscil <SQUARE_MAX_234_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare234(SQUARE_MAX_234_AT_16384_512_DATA);
Oscil <SQUARE_MAX_282_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare282(SQUARE_MAX_282_AT_16384_512_DATA);
Oscil <SQUARE_MAX_356_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare356(SQUARE_MAX_356_AT_16384_512_DATA);
Oscil <SQUARE_MAX_481_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare481(SQUARE_MAX_481_AT_16384_512_DATA);
Oscil <SQUARE_MAX_744_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare744(SQUARE_MAX_744_AT_16384_512_DATA);
Oscil <SQUARE_MAX_1638_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare1638(SQUARE_MAX_1638_AT_16384_512_DATA);
Oscil <SQUARE_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare2730(SQUARE_MAX_2730_AT_16384_512_DATA);
Oscil <SQUARE_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsquare8192(SQUARE_MAX_8192_AT_16384_512_DATA);

MetaOscil<SQUARE_MAX_68_AT_16384_512_NUM_CELLS, AUDIO_RATE, 22> meta1 {&square68, &square72, &square76, &square81, &square86, &square92, &square98, &square106, &square115, &square126, &square138, &square154, &square174, &square199, &square234, &square282, &square356, &square481, &square744, &square1638, &square2730, &square8192};
MetaOscil<SQUARE_MAX_68_AT_16384_512_NUM_CELLS, AUDIO_RATE, 22> meta2 {&asquare68, &asquare72, &asquare76, &asquare81, &asquare86, &asquare92, &asquare98, &asquare106, &asquare115, &asquare126, &asquare138, &asquare154, &asquare174, &asquare199, &asquare234, &asquare282, &asquare356, &asquare481, &asquare744, &asquare1638, &asquare2730, &asquare8192};
MetaOscil<SQUARE_MAX_68_AT_16384_512_NUM_CELLS, AUDIO_RATE, 22> meta3 {&bsquare68, &bsquare72, &bsquare76, &bsquare81, &bsquare86, &bsquare92, &bsquare98, &bsquare106, &bsquare115, &bsquare126, &bsquare138, &bsquare154, &bsquare174, &bsquare199, &bsquare234, &bsquare282, &bsquare356, &bsquare481, &bsquare744, &bsquare1638, &bsquare2730, &bsquare8192};
#endif

#ifdef USE_SAW
// Sawtooth Wave Oscillator Bank 1
Oscil <SAW_MAX_136_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw136(SAW_MAX_136_AT_16384_512_DATA);
Oscil <SAW_MAX_143_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw143(SAW_MAX_143_AT_16384_512_DATA);
Oscil <SAW_MAX_151_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw151(SAW_MAX_151_AT_16384_512_DATA);
Oscil <SAW_MAX_160_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw160(SAW_MAX_160_AT_16384_512_DATA);
Oscil <SAW_MAX_170_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw170(SAW_MAX_170_AT_16384_512_DATA);
Oscil <SAW_MAX_182_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw182(SAW_MAX_182_AT_16384_512_DATA);
Oscil <SAW_MAX_195_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw195(SAW_MAX_195_AT_16384_512_DATA);
Oscil <SAW_MAX_210_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw210(SAW_MAX_210_AT_16384_512_DATA);
Oscil <SAW_MAX_227_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw227(SAW_MAX_227_AT_16384_512_DATA);
Oscil <SAW_MAX_248_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw248(SAW_MAX_248_AT_16384_512_DATA);
Oscil <SAW_MAX_273_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw273(SAW_MAX_273_AT_16384_512_DATA);
Oscil <SAW_MAX_303_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw303(SAW_MAX_303_AT_16384_512_DATA);
Oscil <SAW_MAX_341_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw341(SAW_MAX_341_AT_16384_512_DATA);
Oscil <SAW_MAX_390_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw390(SAW_MAX_390_AT_16384_512_DATA);
Oscil <SAW_MAX_455_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw455(SAW_MAX_455_AT_16384_512_DATA);
Oscil <SAW_MAX_546_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw546(SAW_MAX_546_AT_16384_512_DATA);
Oscil <SAW_MAX_682_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw682(SAW_MAX_682_AT_16384_512_DATA);
Oscil <SAW_MAX_910_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw910(SAW_MAX_910_AT_16384_512_DATA);
Oscil <SAW_MAX_1365_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw1365(SAW_MAX_1365_AT_16384_512_DATA);
Oscil <SAW_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw2730(SAW_MAX_2730_AT_16384_512_DATA);
Oscil <SAW_MAX_4096_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw4096(SAW_MAX_4096_AT_16384_512_DATA);
Oscil <SAW_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw8192(SAW_MAX_8192_AT_16384_512_DATA);

// Sawtooth Wave Oscillator Bank 2
Oscil <SAW_MAX_136_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw136(SAW_MAX_136_AT_16384_512_DATA);
Oscil <SAW_MAX_143_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw143(SAW_MAX_143_AT_16384_512_DATA);
Oscil <SAW_MAX_151_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw151(SAW_MAX_151_AT_16384_512_DATA);
Oscil <SAW_MAX_160_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw160(SAW_MAX_160_AT_16384_512_DATA);
Oscil <SAW_MAX_170_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw170(SAW_MAX_170_AT_16384_512_DATA);
Oscil <SAW_MAX_182_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw182(SAW_MAX_182_AT_16384_512_DATA);
Oscil <SAW_MAX_195_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw195(SAW_MAX_195_AT_16384_512_DATA);
Oscil <SAW_MAX_210_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw210(SAW_MAX_210_AT_16384_512_DATA);
Oscil <SAW_MAX_227_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw227(SAW_MAX_227_AT_16384_512_DATA);
Oscil <SAW_MAX_248_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw248(SAW_MAX_248_AT_16384_512_DATA);
Oscil <SAW_MAX_273_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw273(SAW_MAX_273_AT_16384_512_DATA);
Oscil <SAW_MAX_303_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw303(SAW_MAX_303_AT_16384_512_DATA);
Oscil <SAW_MAX_341_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw341(SAW_MAX_341_AT_16384_512_DATA);
Oscil <SAW_MAX_390_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw390(SAW_MAX_390_AT_16384_512_DATA);
Oscil <SAW_MAX_455_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw455(SAW_MAX_455_AT_16384_512_DATA);
Oscil <SAW_MAX_546_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw546(SAW_MAX_546_AT_16384_512_DATA);
Oscil <SAW_MAX_682_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw682(SAW_MAX_682_AT_16384_512_DATA);
Oscil <SAW_MAX_910_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw910(SAW_MAX_910_AT_16384_512_DATA);
Oscil <SAW_MAX_1365_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw1365(SAW_MAX_1365_AT_16384_512_DATA);
Oscil <SAW_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw2730(SAW_MAX_2730_AT_16384_512_DATA);
Oscil <SAW_MAX_4096_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw4096(SAW_MAX_4096_AT_16384_512_DATA);
Oscil <SAW_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> asaw8192(SAW_MAX_8192_AT_16384_512_DATA);

// Sawtooth Wave Oscillator Bank 3
Oscil <SAW_MAX_136_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw136(SAW_MAX_136_AT_16384_512_DATA);
Oscil <SAW_MAX_143_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw143(SAW_MAX_143_AT_16384_512_DATA);
Oscil <SAW_MAX_151_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw151(SAW_MAX_151_AT_16384_512_DATA);
Oscil <SAW_MAX_160_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw160(SAW_MAX_160_AT_16384_512_DATA);
Oscil <SAW_MAX_170_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw170(SAW_MAX_170_AT_16384_512_DATA);
Oscil <SAW_MAX_182_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw182(SAW_MAX_182_AT_16384_512_DATA);
Oscil <SAW_MAX_195_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw195(SAW_MAX_195_AT_16384_512_DATA);
Oscil <SAW_MAX_210_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw210(SAW_MAX_210_AT_16384_512_DATA);
Oscil <SAW_MAX_227_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw227(SAW_MAX_227_AT_16384_512_DATA);
Oscil <SAW_MAX_248_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw248(SAW_MAX_248_AT_16384_512_DATA);
Oscil <SAW_MAX_273_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw273(SAW_MAX_273_AT_16384_512_DATA);
Oscil <SAW_MAX_303_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw303(SAW_MAX_303_AT_16384_512_DATA);
Oscil <SAW_MAX_341_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw341(SAW_MAX_341_AT_16384_512_DATA);
Oscil <SAW_MAX_390_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw390(SAW_MAX_390_AT_16384_512_DATA);
Oscil <SAW_MAX_455_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw455(SAW_MAX_455_AT_16384_512_DATA);
Oscil <SAW_MAX_546_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw546(SAW_MAX_546_AT_16384_512_DATA);
Oscil <SAW_MAX_682_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw682(SAW_MAX_682_AT_16384_512_DATA);
Oscil <SAW_MAX_910_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw910(SAW_MAX_910_AT_16384_512_DATA);
Oscil <SAW_MAX_1365_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw1365(SAW_MAX_1365_AT_16384_512_DATA);
Oscil <SAW_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw2730(SAW_MAX_2730_AT_16384_512_DATA);
Oscil <SAW_MAX_4096_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw4096(SAW_MAX_4096_AT_16384_512_DATA);
Oscil <SAW_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> bsaw8192(SAW_MAX_8192_AT_16384_512_DATA);

// Meta-Oscillators for Sawtooth Waves
MetaOscil<SAW_MAX_136_AT_16384_512_NUM_CELLS, AUDIO_RATE, 22> meta1 {&saw136, &saw143, &saw151, &saw160, &saw170, &saw182, &saw195, &saw210, &saw227, &saw248, &saw273, &saw303, &saw341, &saw390, &saw455, &saw546, &saw682, &saw910, &saw1365, &saw2730, &saw4096, &saw8192};
MetaOscil<SAW_MAX_136_AT_16384_512_NUM_CELLS, AUDIO_RATE, 22> meta2 {&asaw136, &asaw143, &asaw151, &asaw160, &asaw170, &asaw182, &asaw195, &asaw210, &asaw227, &asaw248, &asaw273, &asaw303, &asaw341, &asaw390, &asaw455, &asaw546, &asaw682, &asaw910, &asaw1365, &asaw2730, &asaw4096, &asaw8192};
MetaOscil<SAW_MAX_136_AT_16384_512_NUM_CELLS, AUDIO_RATE, 22> meta3 {&bsaw136, &bsaw143, &bsaw151, &bsaw160, &bsaw170, &bsaw182, &bsaw195, &bsaw210, &bsaw227, &bsaw248, &bsaw273, &bsaw303, &bsaw341, &bsaw390, &bsaw455, &bsaw546, &bsaw682, &bsaw910, &bsaw1365, &bsaw2730, &bsaw4096, &bsaw8192};
#endif

#ifdef USE_TRI
// Triangle Wave Oscillator Bank 1
Oscil <TRI_MAX_103_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri103(TRI_MAX_103_AT_16384_512_DATA);
Oscil <TRI_MAX_109_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri109(TRI_MAX_109_AT_16384_512_DATA);
Oscil <TRI_MAX_115_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri115(TRI_MAX_115_AT_16384_512_DATA);
Oscil <TRI_MAX_122_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri122(TRI_MAX_122_AT_16384_512_DATA);
Oscil <TRI_MAX_130_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri130(TRI_MAX_130_AT_16384_512_DATA);
Oscil <TRI_MAX_138_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri138(TRI_MAX_138_AT_16384_512_DATA);
Oscil <TRI_MAX_148_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri148(TRI_MAX_148_AT_16384_512_DATA);
Oscil <TRI_MAX_160_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri160(TRI_MAX_160_AT_16384_512_DATA);
Oscil <TRI_MAX_174_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri174(TRI_MAX_174_AT_16384_512_DATA);
Oscil <TRI_MAX_190_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri190(TRI_MAX_190_AT_16384_512_DATA);
Oscil <TRI_MAX_210_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri210(TRI_MAX_210_AT_16384_512_DATA);
Oscil <TRI_MAX_234_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri234(TRI_MAX_234_AT_16384_512_DATA);
Oscil <TRI_MAX_264_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri264(TRI_MAX_264_AT_16384_512_DATA);
Oscil <TRI_MAX_303_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri303(TRI_MAX_303_AT_16384_512_DATA);
Oscil <TRI_MAX_356_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri356(TRI_MAX_356_AT_16384_512_DATA);
Oscil <TRI_MAX_431_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri431(TRI_MAX_431_AT_16384_512_DATA);
Oscil <TRI_MAX_546_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri546(TRI_MAX_546_AT_16384_512_DATA);
Oscil <TRI_MAX_744_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri744(TRI_MAX_744_AT_16384_512_DATA);
Oscil <TRI_MAX_1170_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri1170(TRI_MAX_1170_AT_16384_512_DATA);
Oscil <TRI_MAX_1638_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri1638(TRI_MAX_1638_AT_16384_512_DATA);
Oscil <TRI_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri2730(TRI_MAX_2730_AT_16384_512_DATA);
Oscil <TRI_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri8192(TRI_MAX_8192_AT_16384_512_DATA);

// Triangle Wave Oscillator Bank 2
Oscil <TRI_MAX_103_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri103(TRI_MAX_103_AT_16384_512_DATA);
Oscil <TRI_MAX_109_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri109(TRI_MAX_109_AT_16384_512_DATA);
Oscil <TRI_MAX_115_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri115(TRI_MAX_115_AT_16384_512_DATA);
Oscil <TRI_MAX_122_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri122(TRI_MAX_122_AT_16384_512_DATA);
Oscil <TRI_MAX_130_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri130(TRI_MAX_130_AT_16384_512_DATA);
Oscil <TRI_MAX_138_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri138(TRI_MAX_138_AT_16384_512_DATA);
Oscil <TRI_MAX_148_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri148(TRI_MAX_148_AT_16384_512_DATA);
Oscil <TRI_MAX_160_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri160(TRI_MAX_160_AT_16384_512_DATA);
Oscil <TRI_MAX_174_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri174(TRI_MAX_174_AT_16384_512_DATA);
Oscil <TRI_MAX_190_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri190(TRI_MAX_190_AT_16384_512_DATA);
Oscil <TRI_MAX_210_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri210(TRI_MAX_210_AT_16384_512_DATA);
Oscil <TRI_MAX_234_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri234(TRI_MAX_234_AT_16384_512_DATA);
Oscil <TRI_MAX_264_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri264(TRI_MAX_264_AT_16384_512_DATA);
Oscil <TRI_MAX_303_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri303(TRI_MAX_303_AT_16384_512_DATA);
Oscil <TRI_MAX_356_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri356(TRI_MAX_356_AT_16384_512_DATA);
Oscil <TRI_MAX_431_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri431(TRI_MAX_431_AT_16384_512_DATA);
Oscil <TRI_MAX_546_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri546(TRI_MAX_546_AT_16384_512_DATA);
Oscil <TRI_MAX_744_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri744(TRI_MAX_744_AT_16384_512_DATA);
Oscil <TRI_MAX_1170_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri1170(TRI_MAX_1170_AT_16384_512_DATA);
Oscil <TRI_MAX_1638_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri1638(TRI_MAX_1638_AT_16384_512_DATA);
Oscil <TRI_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri2730(TRI_MAX_2730_AT_16384_512_DATA);
Oscil <TRI_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> atri8192(TRI_MAX_8192_AT_16384_512_DATA);

// Triangle Wave Oscillator Bank 3
Oscil <TRI_MAX_103_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri103(TRI_MAX_103_AT_16384_512_DATA);
Oscil <TRI_MAX_109_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri109(TRI_MAX_109_AT_16384_512_DATA);
Oscil <TRI_MAX_115_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri115(TRI_MAX_115_AT_16384_512_DATA);
Oscil <TRI_MAX_122_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri122(TRI_MAX_122_AT_16384_512_DATA);
Oscil <TRI_MAX_130_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri130(TRI_MAX_130_AT_16384_512_DATA);
Oscil <TRI_MAX_138_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri138(TRI_MAX_138_AT_16384_512_DATA);
Oscil <TRI_MAX_148_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri148(TRI_MAX_148_AT_16384_512_DATA);
Oscil <TRI_MAX_160_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri160(TRI_MAX_160_AT_16384_512_DATA);
Oscil <TRI_MAX_174_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri174(TRI_MAX_174_AT_16384_512_DATA);
Oscil <TRI_MAX_190_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri190(TRI_MAX_190_AT_16384_512_DATA);
Oscil <TRI_MAX_210_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri210(TRI_MAX_210_AT_16384_512_DATA);
Oscil <TRI_MAX_234_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri234(TRI_MAX_234_AT_16384_512_DATA);
Oscil <TRI_MAX_264_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri264(TRI_MAX_264_AT_16384_512_DATA);
Oscil <TRI_MAX_303_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri303(TRI_MAX_303_AT_16384_512_DATA);
Oscil <TRI_MAX_356_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri356(TRI_MAX_356_AT_16384_512_DATA);
Oscil <TRI_MAX_431_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri431(TRI_MAX_431_AT_16384_512_DATA);
Oscil <TRI_MAX_546_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri546(TRI_MAX_546_AT_16384_512_DATA);
Oscil <TRI_MAX_744_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri744(TRI_MAX_744_AT_16384_512_DATA);
Oscil <TRI_MAX_1170_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri1170(TRI_MAX_1170_AT_16384_512_DATA);
Oscil <TRI_MAX_1638_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri1638(TRI_MAX_1638_AT_16384_512_DATA);
Oscil <TRI_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri2730(TRI_MAX_2730_AT_16384_512_DATA);
Oscil <TRI_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> btri8192(TRI_MAX_8192_AT_16384_512_DATA);


// Meta-Oscillators for Triangle Waves
MetaOscil<TRI_MAX_103_AT_16384_512_NUM_CELLS, AUDIO_RATE, 22> meta1 {&tri103, &tri109, &tri115, &tri122, &tri130, &tri138, &tri148, &tri160, &tri174, &tri190, &tri210, &tri234, &tri264, &tri303, &tri356, &tri431, &tri546, &tri744, &tri1170, &tri1638, &tri2730, &tri8192};
MetaOscil<TRI_MAX_103_AT_16384_512_NUM_CELLS, AUDIO_RATE, 22> meta2 {&atri103, &atri109, &atri115, &atri122, &atri130, &atri138, &atri148, &atri160, &atri174, &atri190, &atri210, &atri234, &atri264, &atri303, &atri356, &atri431, &atri546, &atri744, &atri1170, &atri1638, &atri2730, &atri8192};
MetaOscil<TRI_MAX_103_AT_16384_512_NUM_CELLS, AUDIO_RATE, 22> meta3 {&btri103, &btri109, &btri115, &btri122, &btri130, &btri138, &btri148, &btri160, &btri174, &btri190, &btri210, &btri234, &btri264, &btri303, &btri356, &btri431, &btri546, &btri744, &btri1170, &btri1638, &btri2730, &btri8192};
#endif



void setup() 
    {
#ifdef USE_SQUARE
    meta1.setCutoffFreqs(68, 72, 76, 81, 86, 92, 98, 106, 115, 126, 138, 154, 174, 199, 234, 282, 356, 481, 744, 1638, 2730, 8192);
    meta2.setCutoffFreqs(68, 72, 76, 81, 86, 92, 98, 106, 115, 126, 138, 154, 174, 199, 234, 282, 356, 481, 744, 1638, 2730, 8192);
    meta3.setCutoffFreqs(68, 72, 76, 81, 86, 92, 98, 106, 115, 126, 138, 154, 174, 199, 234, 282, 356, 481, 744, 1638, 2730, 8192);
#endif
#ifdef USE_SAW
    meta1.setCutoffFreqs(136, 143, 151, 160, 170, 182, 195, 210, 227, 248, 273, 303, 341, 390, 455, 546, 682, 910, 1365, 2730, 4096, 8192);
    meta2.setCutoffFreqs(136, 143, 151, 160, 170, 182, 195, 210, 227, 248, 273, 303, 341, 390, 455, 546, 682, 910, 1365, 2730, 4096, 8192);
    meta3.setCutoffFreqs(136, 143, 151, 160, 170, 182, 195, 210, 227, 248, 273, 303, 341, 390, 455, 546, 682, 910, 1365, 2730, 4096, 8192);
#endif
#ifdef USE_TRI
    meta1.setCutoffFreqs(103, 109, 115, 122, 130, 138, 148, 160, 174, 190, 210, 234, 264, 303, 356, 431, 546, 744, 1170, 1638, 2730, 8192);
    meta2.setCutoffFreqs(103, 109, 115, 122, 130, 138, 148, 160, 174, 190, 210, 234, 264, 303, 356, 431, 546, 744, 1170, 1638, 2730, 8192);
    meta3.setCutoffFreqs(103, 109, 115, 122, 130, 138, 148, 160, 174, 190, 210, 234, 264, 303, 356, 431, 546, 744, 1170, 1638, 2730, 8192);
#endif
    startMozzi();
    //Serial.begin(115200);

	pinMode(CV_IN3, OUTPUT);

	/// Setup MIDI
#ifdef MULTI	
	initializeParser(&parse, OMNI, 0, 1);
#else
	initializeParser(&parse, CHANNEL, 0, 1);
#endif
	softSerial.begin(MIDI_RATE);
    }

uint8_t notes[3] = { 255, 255, 255 };
uint8_t on[3] = {0, 0, 0};
uint8_t vels[3] = {0, 0, 0 };
uint8_t sinevels[3] = {0, 0, 0 };
uint8_t times[3] = { 0, 0, 0 };
uint8_t gate = 0;
uint8_t play[3] = {0, 0, 0};

#ifdef MULTI

void cc(midiParser* parser, unsigned char parameter, unsigned char value)
	{
	uint8_t voice = CHANNEL - parser->channel;
	if (voice > 2) 	// invalid
		return;
	
	if (parameter == 120 || parameter == 123)	// All Sound Off or All Notes Off
		{
		for(uint8_t i = 0; i < 3; i++)
			{
			on[i] = 0;
			}
		
		// everyone is off, lower gate
		digitalWrite(CV_IN3, 0);
		gate = 0;
		}
	}


void noteOn(midiParser* parser, unsigned char note, unsigned char velocity)
	{
	uint8_t voice = CHANNEL - parser->channel;
	if (voice > 2) 	// invalid
		return;
		
	if (!on[0] && !on[1] && !on[2])
		{ play[0] = 0; play[1] = 0; play[2] = 0; }

	if (!gate)
		{
		digitalWrite(CV_IN3, 1);
		gate = 1;
		}
	
	if (!on[voice])
		{
		play[voice] = 1;
		on[voice] = 1;
		}
		
	// Again more hoisting because C++ templates are poop
	if (voice == 0)
		{
		notes[0] = note;
		vels[0] = velocity >> 1;		// will go 0...64
		meta1.setFreq(FREQUENCY(note));
		sine1.setFreq(FREQUENCY(note));
		}
	else if (voice == 1)
		{
		notes[1] = note;
		vels[1] = velocity >> 1;
		meta2.setFreq(FREQUENCY(note));
		sine2.setFreq(FREQUENCY(note));
		}
	else if (voice == 2)
		{
		notes[2] = note;
		vels[2] = velocity >> 1;
		meta3.setFreq(FREQUENCY(note));
		sine3.setFreq(FREQUENCY(note));
		}
	}

void noteOff(midiParser* parser, unsigned char note, unsigned char velocity)
	{
	uint8_t voice = CHANNEL - parser->channel;
	if (voice > 2) 	// invalid
		return;
		
	for(uint8_t i = 0; i < 3; i++)
		{
		if (notes[i] == note && on[i])
			{
			on[i] = 0;
			break;
			}
		}
		
	// is everyone off?
	for(uint8_t i = 0; i < 3; i++)
		{
		if (on[i])
			{
			return;
			}
		}
	
	// everyone is off, lower gate
	digitalWrite(CV_IN3, 0);
	gate = 0;
	}


#else

void cc(midiParser* parser, unsigned char parameter, unsigned char value)
	{
	if (parameter == 120 || parameter == 123)	// All Sound Off or All Notes Off
		{
		for(uint8_t i = 0; i < 3; i++)
			{
			on[i] = 0;
			}
		
		// everyone is off, lower gate
		digitalWrite(CV_IN3, 0);
		gate = 0;
		}
	}


void noteOn(midiParser* parser, unsigned char note, unsigned char velocity)
	{
	if (!on[0] && !on[1] && !on[2])
		{ play[0] = 0; play[1] = 0; play[2] = 0; }

	if (!gate)
		{
		digitalWrite(CV_IN3, 1);
		gate = 1;
		}
	
	// this part is ugly because C++ templating is garbage and doesn't permit arrays
		if (!on[0])
			{
			notes[0] = note;
			vels[0] = velocity >> 1;
			meta1.setFreq(FREQUENCY(note));
			sine1.setFreq(FREQUENCY(note));
			play[0] = 1;
      		on[0] = 1;
      		
			return;
			}
		if (!on[1])
			{
			notes[1] = note;
			vels[1] = velocity >> 1;
			meta2.setFreq(FREQUENCY(note));
			sine2.setFreq(FREQUENCY(note));
				play[1] = 1;
      		on[1] = 1;
			return;
			}
		if (!on[2])
			{
			notes[2] = note;
			vels[2] = velocity >> 1;
			meta3.setFreq(FREQUENCY(note));
			sine3.setFreq(FREQUENCY(note));
				play[2] = 1;
			on[2] = 1;
			return;
			}
		
	// everyone filled, we need to steal
	uint8_t oldest = 0;
	uint8_t oldestTime = times[0];
	for(uint8_t i = 1; i < 2; i++)
		{
		if (times[i] < oldestTime)
			{
			oldestTime = times[i];
			oldest = i;
			}
		}
	
	// Again more hoisting because C++ templates are poop
	if (oldest == 0)
		{
		notes[0] = note;
		vels[0] = velocity >> 1;
		meta1.setFreq(FREQUENCY(note));
		sine1.setFreq(FREQUENCY(note));
		}
	else if (oldest == 1)
		{
		notes[1] = note;
		vels[1] = velocity >> 1;
		meta2.setFreq(FREQUENCY(note));
		sine2.setFreq(FREQUENCY(note));
		}
	else if (oldest == 2)
		{
		notes[2] = note;
		vels[2] = velocity >> 1;
		meta3.setFreq(FREQUENCY(note));
		sine3.setFreq(FREQUENCY(note));
		}
	}

void noteOff(midiParser* parser, unsigned char note, unsigned char velocity)
	{
	for(uint8_t i = 0; i < 3; i++)
		{
		if (notes[i] == note && on[i])
			{
			on[i] = 0;
			break;
			}
		}
		
	// is everyone off?
	for(uint8_t i = 0; i < 3; i++)
		{
		if (on[i])
			{
			return;
			}
		}
	
	// everyone is off, lower gate
	digitalWrite(CV_IN3, 0);
	gate = 0;
	}

#endif

uint8_t state = 0;
uint8_t alpha;
uint8_t gain;
void updateControl() 
    {
    state++;
    // gain ranges 0...15
    if (state & 0) gain = mozziAnalogRead(CV_POT3) >> 6;
    else alpha = mozziAnalogRead(CV_POT_IN1) >> 2;
        
    uint8_t val = softSerial.available();
	for(uint8_t i = 0; i < val; i++)
		{
		parseMidi(&parse, softSerial.read());
		}
    }

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



int updateAudio()    
    {
    int16_t sum = 0;
    
    // I'm hoisting these, may be a teeny bit faster in some cases...

#ifdef GATE
		if (on[0])
			{
			uint16_t combined = (meta1.next() * alpha) + (sine1.next() * (255 - alpha)) >> 8;
			sum += combined * (gain == 0 ? 63 : vels[0]);
			}
		
		if (on[1])
			{
			uint16_t combined = (meta2.next() * alpha) + (sine2.next() * (255 - alpha)) >> 8;
			sum += combined * (gain == 0 ? 63 : vels[1]);
			}

		if (on[2])
			{
			uint16_t combined = (meta3.next() * alpha) + (sine3.next() * (255 - alpha)) >> 8;
			sum += combined * (gain == 0 ? 63 : vels[2]);
			}
		// Since vels goes 0...63 this should sum to no more than 
		// 127 * 63 * 3 = 24003
#else
		if (play[0])
			{
			uint16_t combined = (meta1.next() * alpha) + (sine1.next() * (255 - alpha)) >> 8;
			sum += combined * (gain == 0 ? 63 : vels[0]);
			}
		
		if (play[1])
			{
			uint16_t combined = (meta2.next() * alpha) + (sine2.next() * (255 - alpha)) >> 8;
			sum += combined * (gain == 0 ? 63 : vels[1]);
			}

		if (play[2])
			{
			uint16_t combined = (meta3.next() * alpha) + (sine3.next() * (255 - alpha)) >> 8;
			sum += combined * (gain == 0 ? 63 : vels[2]);
			}
		// Since vels goes 0...63 this should sum to no more than 
		// 127 * 63 * 3 = 24003
#endif
	if (gain == 0) return ((sum >> 4) * 15) >> 7;		// this maps 24003  -> 176
	else return ((sum >> 3) * gain) >> 7;				// this is similar if gain = 7.5, but gain can go up to 15

//    return scaleAudioSmall(((sum >> 6) * 15 ) >> 5);
    }


void loop() 
    {
    audioHook();
    }
