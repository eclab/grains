// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// MONO-M
///
/// Mono-M is a MIDI-driven Monophonic synthesizer with a mixed square/saw/triangle and sine waves.
/// Mono-M is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// Mono-M can mix between square and sine, saw and sine, or triangle and sine.  You select
/// which one you want in the code.  The default is square and sine.
///
/// You can set the amount of square/saw/tringle vs sine wave.
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.

/// Mono-M also sends a Gate via IN 3 whenever a note has been played, and drops it when the note
/// has been released.  If you play a new note prior to the old one being released, it will also 
/// drop the gate and wait tiny bit before raising it for the new note to prevent legato.

/// CHANNEL
///
/// You can set Para-M's channel to 0...15 (for channels 1...16) or to OMNI (to listen to any of them).
/// This is set here:

// Set this to 0...15, or to OMNI
#define CHANNEL OMNI



/// MONO-M VS PARA-M
///
/// Para-M has three voices.  Mono-M has only one.
/// However Mono-M has twice as many waves in its memory, so it will sound less aliased for many notes.

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


// #define USE_SQUARE
#define USE_SAW
//  #define USE_TRI


/// GATE MODE
///
/// By default Mono-M does not stop playing notes when it receives a NOTE OFF -- 
/// that is the responsibility of your envelope and VCA (it will send them a gate off).
/// However you can change this behavior by uncommenting the following #define:

/// #define GATE    


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
/// IN 1            Saw/Square/Tri Vs Sine Ratio CV
/// IN 2            [Unused]
/// IN 3            Gate Out
/// AUDIO IN (A)    [Unused]  
/// AUDIO OUT       Out
/// DIGITAL OUT (D) MIDI In
///
/// POT 1           Saw/Square/Tri Vs Sine Ratio
///					[If not using CV, set switch to MAN, else set switch to IN1 and set pot to about 2'oclock] 
///
/// POT 2           [Unused]
///
/// POT 3           [Unused]




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

// Sine Wave Oscillator
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> sine(SIN512_DATA);

#ifdef USE_SQUARE
Oscil <SQUARE_MAX_68_AT_16384_512_NUM_CELLS, AUDIO_RATE> square68(SQUARE_MAX_68_AT_16384_512_DATA);
Oscil <SQUARE_MAX_70_AT_16384_512_NUM_CELLS, AUDIO_RATE> square70(SQUARE_MAX_70_AT_16384_512_DATA);
Oscil <SQUARE_MAX_72_AT_16384_512_NUM_CELLS, AUDIO_RATE> square72(SQUARE_MAX_72_AT_16384_512_DATA);
Oscil <SQUARE_MAX_73_AT_16384_512_NUM_CELLS, AUDIO_RATE> square73(SQUARE_MAX_73_AT_16384_512_DATA);
Oscil <SQUARE_MAX_76_AT_16384_512_NUM_CELLS, AUDIO_RATE> square76(SQUARE_MAX_76_AT_16384_512_DATA);
Oscil <SQUARE_MAX_78_AT_16384_512_NUM_CELLS, AUDIO_RATE> square78(SQUARE_MAX_78_AT_16384_512_DATA);
Oscil <SQUARE_MAX_81_AT_16384_512_NUM_CELLS, AUDIO_RATE> square81(SQUARE_MAX_81_AT_16384_512_DATA);
Oscil <SQUARE_MAX_82_AT_16384_512_NUM_CELLS, AUDIO_RATE> square82(SQUARE_MAX_82_AT_16384_512_DATA);
Oscil <SQUARE_MAX_86_AT_16384_512_NUM_CELLS, AUDIO_RATE> square86(SQUARE_MAX_86_AT_16384_512_DATA);
Oscil <SQUARE_MAX_88_AT_16384_512_NUM_CELLS, AUDIO_RATE> square88(SQUARE_MAX_88_AT_16384_512_DATA);
Oscil <SQUARE_MAX_92_AT_16384_512_NUM_CELLS, AUDIO_RATE> square92(SQUARE_MAX_92_AT_16384_512_DATA);
Oscil <SQUARE_MAX_94_AT_16384_512_NUM_CELLS, AUDIO_RATE> square94(SQUARE_MAX_94_AT_16384_512_DATA);
Oscil <SQUARE_MAX_98_AT_16384_512_NUM_CELLS, AUDIO_RATE> square98(SQUARE_MAX_98_AT_16384_512_DATA);
Oscil <SQUARE_MAX_101_AT_16384_512_NUM_CELLS, AUDIO_RATE> square101(SQUARE_MAX_101_AT_16384_512_DATA);
Oscil <SQUARE_MAX_106_AT_16384_512_NUM_CELLS, AUDIO_RATE> square106(SQUARE_MAX_106_AT_16384_512_DATA);
Oscil <SQUARE_MAX_109_AT_16384_512_NUM_CELLS, AUDIO_RATE> square109(SQUARE_MAX_109_AT_16384_512_DATA);
Oscil <SQUARE_MAX_115_AT_16384_512_NUM_CELLS, AUDIO_RATE> square115(SQUARE_MAX_115_AT_16384_512_DATA);
Oscil <SQUARE_MAX_118_AT_16384_512_NUM_CELLS, AUDIO_RATE> square118(SQUARE_MAX_118_AT_16384_512_DATA);
Oscil <SQUARE_MAX_126_AT_16384_512_NUM_CELLS, AUDIO_RATE> square126(SQUARE_MAX_126_AT_16384_512_DATA);
Oscil <SQUARE_MAX_130_AT_16384_512_NUM_CELLS, AUDIO_RATE> square130(SQUARE_MAX_130_AT_16384_512_DATA);
Oscil <SQUARE_MAX_138_AT_16384_512_NUM_CELLS, AUDIO_RATE> square138(SQUARE_MAX_138_AT_16384_512_DATA);
Oscil <SQUARE_MAX_143_AT_16384_512_NUM_CELLS, AUDIO_RATE> square143(SQUARE_MAX_143_AT_16384_512_DATA);
Oscil <SQUARE_MAX_154_AT_16384_512_NUM_CELLS, AUDIO_RATE> square154(SQUARE_MAX_154_AT_16384_512_DATA);
Oscil <SQUARE_MAX_160_AT_16384_512_NUM_CELLS, AUDIO_RATE> square160(SQUARE_MAX_160_AT_16384_512_DATA);
Oscil <SQUARE_MAX_174_AT_16384_512_NUM_CELLS, AUDIO_RATE> square174(SQUARE_MAX_174_AT_16384_512_DATA);
Oscil <SQUARE_MAX_182_AT_16384_512_NUM_CELLS, AUDIO_RATE> square182(SQUARE_MAX_182_AT_16384_512_DATA);
Oscil <SQUARE_MAX_199_AT_16384_512_NUM_CELLS, AUDIO_RATE> square199(SQUARE_MAX_199_AT_16384_512_DATA);
Oscil <SQUARE_MAX_210_AT_16384_512_NUM_CELLS, AUDIO_RATE> square210(SQUARE_MAX_210_AT_16384_512_DATA);
Oscil <SQUARE_MAX_234_AT_16384_512_NUM_CELLS, AUDIO_RATE> square234(SQUARE_MAX_234_AT_16384_512_DATA);
Oscil <SQUARE_MAX_248_AT_16384_512_NUM_CELLS, AUDIO_RATE> square248(SQUARE_MAX_248_AT_16384_512_DATA);
Oscil <SQUARE_MAX_282_AT_16384_512_NUM_CELLS, AUDIO_RATE> square282(SQUARE_MAX_282_AT_16384_512_DATA);
Oscil <SQUARE_MAX_303_AT_16384_512_NUM_CELLS, AUDIO_RATE> square303(SQUARE_MAX_303_AT_16384_512_DATA);
Oscil <SQUARE_MAX_356_AT_16384_512_NUM_CELLS, AUDIO_RATE> square356(SQUARE_MAX_356_AT_16384_512_DATA);
Oscil <SQUARE_MAX_390_AT_16384_512_NUM_CELLS, AUDIO_RATE> square390(SQUARE_MAX_390_AT_16384_512_DATA);
Oscil <SQUARE_MAX_481_AT_16384_512_NUM_CELLS, AUDIO_RATE> square481(SQUARE_MAX_481_AT_16384_512_DATA);
Oscil <SQUARE_MAX_546_AT_16384_512_NUM_CELLS, AUDIO_RATE> square546(SQUARE_MAX_546_AT_16384_512_DATA);
Oscil <SQUARE_MAX_744_AT_16384_512_NUM_CELLS, AUDIO_RATE> square744(SQUARE_MAX_744_AT_16384_512_DATA);
Oscil <SQUARE_MAX_910_AT_16384_512_NUM_CELLS, AUDIO_RATE> square910(SQUARE_MAX_910_AT_16384_512_DATA);
Oscil <SQUARE_MAX_1638_AT_16384_512_NUM_CELLS, AUDIO_RATE> square1638(SQUARE_MAX_1638_AT_16384_512_DATA);
Oscil <SQUARE_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> square2730(SQUARE_MAX_2730_AT_16384_512_DATA);
Oscil <SQUARE_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> square8192(SQUARE_MAX_8192_AT_16384_512_DATA);

// Meta-Oscillators for Square Waves
MetaOscil<SQUARE_MAX_68_AT_16384_512_NUM_CELLS, AUDIO_RATE, 41> meta {&square68, &square70, &square72, &square73, &square76, &square78, &square81, &square82, &square86, &square88, &square92, &square94, &square98, &square101, &square106, &square109, &square115, &square118, &square126, &square130, &square138, &square143, &square154, &square160, &square174, &square182, &square199, &square210, &square234, &square248, &square282, &square303, &square356, &square390, &square481, &square546, &square744, &square910, &square1638, &square2730, &square8192 };
#endif

#ifdef USE_SAW
Oscil <SAW_MAX_136_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw136(SAW_MAX_136_AT_16384_512_DATA);
Oscil <SAW_MAX_138_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw138(SAW_MAX_138_AT_16384_512_DATA);
Oscil <SAW_MAX_143_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw143(SAW_MAX_143_AT_16384_512_DATA);
Oscil <SAW_MAX_146_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw146(SAW_MAX_146_AT_16384_512_DATA);
Oscil <SAW_MAX_151_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw151(SAW_MAX_151_AT_16384_512_DATA);
Oscil <SAW_MAX_154_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw154(SAW_MAX_154_AT_16384_512_DATA);
Oscil <SAW_MAX_160_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw160(SAW_MAX_160_AT_16384_512_DATA);
Oscil <SAW_MAX_163_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw163(SAW_MAX_163_AT_16384_512_DATA);
Oscil <SAW_MAX_170_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw170(SAW_MAX_170_AT_16384_512_DATA);
Oscil <SAW_MAX_174_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw174(SAW_MAX_174_AT_16384_512_DATA);
Oscil <SAW_MAX_182_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw182(SAW_MAX_182_AT_16384_512_DATA);
Oscil <SAW_MAX_186_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw186(SAW_MAX_186_AT_16384_512_DATA);
Oscil <SAW_MAX_195_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw195(SAW_MAX_195_AT_16384_512_DATA);
Oscil <SAW_MAX_199_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw199(SAW_MAX_199_AT_16384_512_DATA);
Oscil <SAW_MAX_210_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw210(SAW_MAX_210_AT_16384_512_DATA);
Oscil <SAW_MAX_215_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw215(SAW_MAX_215_AT_16384_512_DATA);
Oscil <SAW_MAX_227_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw227(SAW_MAX_227_AT_16384_512_DATA);
Oscil <SAW_MAX_234_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw234(SAW_MAX_234_AT_16384_512_DATA);
Oscil <SAW_MAX_248_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw248(SAW_MAX_248_AT_16384_512_DATA);
Oscil <SAW_MAX_256_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw256(SAW_MAX_256_AT_16384_512_DATA);
Oscil <SAW_MAX_273_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw273(SAW_MAX_273_AT_16384_512_DATA);
Oscil <SAW_MAX_282_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw282(SAW_MAX_282_AT_16384_512_DATA);
Oscil <SAW_MAX_303_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw303(SAW_MAX_303_AT_16384_512_DATA);
Oscil <SAW_MAX_315_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw315(SAW_MAX_315_AT_16384_512_DATA);
Oscil <SAW_MAX_341_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw341(SAW_MAX_341_AT_16384_512_DATA);
Oscil <SAW_MAX_356_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw356(SAW_MAX_356_AT_16384_512_DATA);
Oscil <SAW_MAX_390_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw390(SAW_MAX_390_AT_16384_512_DATA);
Oscil <SAW_MAX_409_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw409(SAW_MAX_409_AT_16384_512_DATA);
Oscil <SAW_MAX_455_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw455(SAW_MAX_455_AT_16384_512_DATA);
Oscil <SAW_MAX_481_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw481(SAW_MAX_481_AT_16384_512_DATA);
Oscil <SAW_MAX_546_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw546(SAW_MAX_546_AT_16384_512_DATA);
Oscil <SAW_MAX_585_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw585(SAW_MAX_585_AT_16384_512_DATA);
Oscil <SAW_MAX_682_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw682(SAW_MAX_682_AT_16384_512_DATA);
Oscil <SAW_MAX_744_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw744(SAW_MAX_744_AT_16384_512_DATA);
Oscil <SAW_MAX_910_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw910(SAW_MAX_910_AT_16384_512_DATA);
Oscil <SAW_MAX_1024_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw1024(SAW_MAX_1024_AT_16384_512_DATA);
Oscil <SAW_MAX_1365_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw1365(SAW_MAX_1365_AT_16384_512_DATA);
Oscil <SAW_MAX_1638_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw1638(SAW_MAX_1638_AT_16384_512_DATA);
Oscil <SAW_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw2730(SAW_MAX_2730_AT_16384_512_DATA);
Oscil <SAW_MAX_4096_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw4096(SAW_MAX_4096_AT_16384_512_DATA);
Oscil <SAW_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw8192(SAW_MAX_8192_AT_16384_512_DATA);



// Meta-Oscillators for Sawtooth Waves
MetaOscil<SAW_MAX_136_AT_16384_512_NUM_CELLS, AUDIO_RATE, 41> meta {&saw136, &saw138, &saw143, &saw146, &saw151, &saw154, &saw160, &saw163, &saw170, &saw174, &saw182, &saw186, &saw195, &saw199, &saw210, &saw215, &saw227, &saw234, &saw248, &saw256, &saw273, &saw282, &saw303, &saw315, &saw341, &saw356, &saw390, &saw409, &saw455, &saw481, &saw546, &saw585, &saw682, &saw744, &saw910, &saw1024, &saw1365, &saw1638, &saw2730, &saw4096, &saw8192 };
#endif

#ifdef USE_TRI
Oscil <TRI_MAX_103_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri103(TRI_MAX_103_AT_16384_512_DATA);
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
Oscil <TRI_MAX_481_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri481(TRI_MAX_481_AT_16384_512_DATA);
Oscil <TRI_MAX_546_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri546(TRI_MAX_546_AT_16384_512_DATA);
Oscil <TRI_MAX_630_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri630(TRI_MAX_630_AT_16384_512_DATA);
Oscil <TRI_MAX_744_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri744(TRI_MAX_744_AT_16384_512_DATA);
Oscil <TRI_MAX_910_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri910(TRI_MAX_910_AT_16384_512_DATA);
Oscil <TRI_MAX_1170_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri1170(TRI_MAX_1170_AT_16384_512_DATA);
Oscil <TRI_MAX_1638_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri1638(TRI_MAX_1638_AT_16384_512_DATA);
Oscil <TRI_MAX_2730_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri2730(TRI_MAX_2730_AT_16384_512_DATA);
Oscil <TRI_MAX_8192_AT_16384_512_NUM_CELLS, AUDIO_RATE> tri8192(TRI_MAX_8192_AT_16384_512_DATA);


// Meta-Oscillators for Triangle Waves
MetaOscil<TRI_MAX_103_AT_16384_512_NUM_CELLS, AUDIO_RATE, 40> meta {&tri103, &tri106, &tri109, &tri112, &tri115, &tri118, &tri122, &tri126, &tri130, &tri134, &tri138, &tri143, &tri148, &tri154, &tri160, &tri167, &tri174, &tri182, &tri190, &tri199, &tri210, &tri221, &tri234, &tri248, &tri264, &tri282, &tri303, &tri327, &tri356, &tri390, &tri431, &tri481, &tri546, &tri630, &tri744, &tri910, &tri1170, &tri1638, &tri2730, &tri8192};
#endif



void setup() 
    {
#ifdef USE_SQUARE
    meta.setCutoffFreqs(68, 70, 72, 73, 76, 78, 81, 82, 86, 88, 92, 94, 98, 101, 106, 109, 115, 118, 126, 130, 138, 143, 154, 160, 174, 182, 199, 210, 234, 248, 282, 303, 356, 390, 481, 546, 744, 910, 1638, 2730, 8192);
#endif
#ifdef USE_SAW
    meta.setCutoffFreqs(136, 138, 143, 146, 151, 154, 160, 163, 170, 174, 182, 186, 195, 199, 210, 215, 227, 234, 248, 256, 273, 282, 303, 315, 341, 356, 390, 409, 455, 481, 546, 585, 682, 744, 910, 1024, 1365, 1638, 2730, 4096, 8192);
#endif
#ifdef USE_TRI
    meta.setCutoffFreqs(103, 106, 109, 112, 115, 118, 122, 126, 130, 134, 138, 143, 148, 154, 160, 167, 174, 182, 190, 199, 210, 221, 234, 248, 264, 282, 303, 327, 356, 390, 431, 481, 546, 630, 744, 910, 1170, 1638, 2730, 8192);
#endif
    startMozzi();

	pinMode(CV_IN3, OUTPUT);
	/// Setup MIDI
	initializeParser(&parse, CHANNEL, 0, 1);
	softSerial.begin(MIDI_RATE);
	//Serial.begin(115200);
    }

uint8_t on = 0;
uint8_t gate = 0;
uint8_t timer = 0;
uint8_t _velocity = 0;
uint8_t vel = 0;
uint8_t sineVel = 0;
uint8_t pitch;
int16_t alpha = 0; 

void cc(midiParser* parser, unsigned char parameter, unsigned char value)
	{
	if (parameter == 120 || parameter == 123)	// All Sound Off or All Notes Off
		{
		on = 0;		
		// everyone is off, lower gate
		digitalWrite(CV_IN3, 0);
		gate = 0;
		}
	}
	
void noteOn(midiParser* parser, unsigned char note, unsigned char velocity)
	{
	pitch = note;
	meta.setFreq(FREQUENCY(note));
	sine.setFreq(FREQUENCY(note));
	_velocity = velocity;
		
	on = 1;
		
	if (gate)
		{
		timer = 2;
		digitalWrite(CV_IN3, 0);
		gate = 0;
		}
	else
		{
		digitalWrite(CV_IN3, 1);
		gate = 1;		
		timer = 0;
		}
	
	}

void noteOff(midiParser* parser, unsigned char note, unsigned char velocity)
	{
	if (note == pitch)
		{
		on = 0;
		timer = 0;
		gate = 0;
		digitalWrite(CV_IN3, 0);
		}
	}


uint8_t gain = 0;
void updateControl() 
    {
    alpha = mozziAnalogRead(CV_POT_IN1) >> 4;               // should we filter this?
    gain = mozziAnalogRead(CV_POT3) >> 3;
        
    if (timer > 0)
    	{
    	if (timer == 1)
    		{
    		digitalWrite(CV_IN3, 1);
    		gate = 1;
    		}
    	timer--;
    	}
    	
    uint8_t val = softSerial.available();
	for(uint8_t i = 0; i < val; i++)
		{
		parseMidi(&parse, softSerial.read());
		}

	vel = (_velocity * alpha) >> 8;
	sineVel = (_velocity * (255 - alpha)) >> 8;
    }


int updateAudio()    
    {
    int16_t sum = 0;
    
    // I'm hoisting these, may be a teeny bit faster in some cases...

#ifdef GATE    
		if (on)
			{
			sum += meta.next() * vel;
			sum += sine.next() * sineVel;
			}
#else
		sum += meta.next() * vel;
		sum += sine.next() * sineVel;
#endif

    return (((sum * 2) >> 8) * gain) >> 6;
    }

void loop() 
    {
    audioHook();
    }
