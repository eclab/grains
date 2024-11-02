// Copyright 2024 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// TRAJECTORY
///
/// TRAJECTORY is a MIDI-driven 1-voice vector synthesizer similar to VS, but with different
/// design choices.  
/// TRAJECTORY is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.
/// TRAJECTORY only responds to MIDI, via the wonkystuff mb/1, IMDI MAIN module, or from DAVE. 
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.
///
/// TRAJECTORY was named by AE Modular user reductionist_earth_catalog, who also kindly provided
/// some hardware donations to the cause.
///
/// TRAJECTORY crossfades between four waveforms in a special way.  You can control the cross-fading
/// manually with two CVs, such as with a joystick; and you can also use the same to program
/// about 4 seconds of CV crossfading movement.
///
/// TRAJECTORY is different from VS in the following ways:
///
/// 1. TRAJECTORY has four fixed waves that you cannot change, whereas with VS you can pick your waves.
/// 2. VS is heavily aliased because it has only one copy of each wave.  But because TRAJECTORY has
///    the waves it does, it has copies of them at different frequencies and so is much less aliased.
/// 3. TRAJECTORY has half the vector envelope resolution of VS, because it needs extra space to fit its
///    wave information.
///
/// HOW VECTOR SYNTHESIS WORKS
///
/// Vector synthesis dynamically changes the cross-fade between four different sound waves.
/// To control the cross-fade mix you modify two CVs called X and Y.  Imagine that these CV
/// values each go from 0 to 1 inclusive, so together they define a square region from (0,0) to (1,1). 
/// The amplitude of the four waves is defined as:
///
/// AMPLITUDE OF WAVE 1 = (1-x) * (1-y)
/// AMPLITUDE OF WAVE 2 = x * y				[opposite corner from WAVE 1]
/// AMPLITUDE OF WAVE 3 = (1-x) * y
/// AMPLITUDE OF WAVE 4 = x * (1-y)			[opposite corner from WAVE 3]
///
/// In short: each wave has a corner in the square where it is the only sound.  Wave 1 is solo
/// in the corner (x=0, y=0), Wave 2 in the corner (x=1, y=1), wave 3 in (x=0, y=1), and wave 4
/// in (x=1, y=0).  All four waves are mixed together (25% each) in the center.  
///
/// You can either manually change these CVs to your heart's content, or you can record up to 4
/// seconds of CV-twiddling, and that will be played back each time a new note/chord is played.
///
///
/// SETTING THE WAVES
/// 
/// By default TRAJECTORY sets its four waves to be:
///
/// WAVE 1	Sine
/// WAVE 2  White Noise						[opposite corner from Sine]
/// Wave 3	Square
/// Wave 4  Sawtooth						[opposite corner from Square]
///
/// You have three options here:
///
/// OPTION 1: Play the four waves as-is
/// OPTION 2: Replace the SINE with SILENCE
/// OPTION 3: Replace the WHITE NOISE with SILENCE
/// 
/// You set options 2 and 3 by optionally removing the // in front of exactly ONE of the following #defines

// #define USE_NOISE			// Option 2: replace sine with silence, retaining noise
// #define USE_SINE			// Option 3: replace white noise with silence, retaining sine

/// Additionally, if you find the noise too bright, you can darken (and quiet) it a bit by uncommenting the
/// following #define, that is, remove the // in front of it:

// #define DARK_NOISE



/// PLAYING MODES
///
/// TRAJECTORY has four MODES, specified by POT 3.
///
///	FREE MODE: the wave mix is set by changing IN1/POT1 and IN2/POT2.
///
/// RECORD MODE: the wave mix is **recorded** by changing IN1/POT1 and IN2/POT2 for up to 4 seconds. 
/// When a note is played, the GATE OUT is raised, and when a note is released, the GATE OUT is lowered.  
/// Recording stops when (1) the time has elapsed or (2) the note is released or (3) the mode is 
/// changed back to PLAY or FREE.
///
/// When you have stopped recording, TRAJECTORY will save your internal recording to the EEPROM so that it's
/// available even after you reboot.
///
/// PLAY MODE: when a note is played, the recorded wave mix is played back.  When a note is played,
/// the GATE OUT is raised, and when a note stops being played, the GATE OUT is lowered.


/// LOOPING OPTION
///
/// If you uncomment the #define below, you can get TRAJECTORY to loop its vector envelope while you play it.
/// This will only work if the loop was recorded to its maximum length: if you recorded to something
/// shorter, it won't loop.

//  #define LOOP


/// USING TRAJECTORY WITH THE AE MODULAR JOYSTICK
///
/// TRAJECTORY works perfectly with JOYSTICK.  Just attach JOYSTICK's CV X to GRAINS IN 1, and CV Y to IN 2.
/// Make sure that POT 1 is set to IN 1 and POT 2 is set to IN 2.  Then you'll need to trim the pots
/// to get the range right.  I find that setting POT 1 (and 2) to a little before the 3 o'clock position
/// is the right position.  You can test by setting TRAJECTORY to FREE mode and see if each of the JOYSTICK's
/// corners correspond to a pure sound.


/// CHANNEL
///
/// You can set TRAJECTORY's channel to 0...15 (for channels 1...16) or to OMNI (to listen to any of them).
/// This is set here:

// Set this to 0...15, or to OMNI
#define CHANNEL OMNI


/// MIDI RESPONSE
///
/// MIDI NOTES 		Note On/Off
///						Pitch: All notes C0 through G10, but realistically C0 through B8
///						Velocity: Velocity 0...127 for Note On.  Velocity 0 is a Note Off
///					
/// CC				All Notes Off		CC 123		[Resets all notes, lowers Gate]
///					All Sounds Off		CC 120		[Resets all notes, lowers Gate]


/// MIDI CONFIGURATION
///
/// IN 1            X
/// IN 2            Y
/// IN 3            Gate Out
/// AUDIO IN (A)    Gate In                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
/// AUDIO OUT       Out
/// DIGITAL OUT (D) MIDI In
///
/// POT 1           X
///
/// POT 2           Y
///
/// POT 3           Mode: FREE (Left), PLAY (Center), RECORD (Right)




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
#define CONTROL_RATE 128			// must be 128, so we can play wave for 4 secs

#define CV_POT_IN1    A2    // X
#define CV_POT_IN2    A1    // Y
#define CV_POT3       A0    // Mode
#define CV_IN3        A3    // Gate Out
#define CV_AUDIO_IN   A4    // [Unused]
#define CV_AUDIO_OUT  9     // Out
#define CV_GATE_OUT   8     // MIDI In
#define RANDOM_PIN    A5


#include <EEPROM.h>
#include <MozziGuts.h>
#include <Oscil.h>
#include <MetaOscil.h>
//#include <ResonantFilter.h>
//#include "mozzi_rand.h"
#include "rnd.c"

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

#include <tables/sin512_int8.h>

#include "NeoSWSerial.h"   // you have to install this via the library manager
#include "parsemidi.c"

#define MIDI_RATE 31250
#define BLANK_SERIAL	  5		// Blank Serial Pin
#define PIN_UNUSED 255
NeoSWSerial softSerial(CV_GATE_OUT, BLANK_SERIAL, PIN_UNUSED);
midiParser parse;

Oscil<SIN512_NUM_CELLS, AUDIO_RATE> _sine(SIN512_DATA);

/// OSCILLATORS

// Square Wave Oscillator Bank 1
//Oscil <SQUARE_MAX_72_AT_16384_512_NUM_CELLS, AUDIO_RATE> square72(SQUARE_MAX_72_AT_16384_512_DATA);
//Oscil <SQUARE_MAX_76_AT_16384_512_NUM_CELLS, AUDIO_RATE> square76(SQUARE_MAX_76_AT_16384_512_DATA);
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

// If we increase this to 19 (including square76), we drop to about 60 bytes remaining which is not enough for the stack, and it starts getting unstable
MetaOscil<SQUARE_MAX_68_AT_16384_512_NUM_CELLS, AUDIO_RATE, 18> _square {/* &square72, &square76, */ &square81, &square86, &square92, &square98, &square106, &square115, &square126, &square138, &square154, &square174, &square199, &square234, &square282, &square356, &square481, &square744, &square1638, &square2730 };

// Sawtooth Wave Oscillator Bank 1
//Oscil <SAW_MAX_143_AT_16384_512_NUM_CELLS, AUDIO_RATE> saw143(SAW_MAX_143_AT_16384_512_DATA);
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


// Meta-Oscillators for Sawtooth Waves
MetaOscil<SAW_MAX_136_AT_16384_512_NUM_CELLS, AUDIO_RATE, 19> _saw {/* &saw143, */ &saw151, &saw160, &saw170, &saw182, &saw195, &saw210, &saw227, &saw248, &saw273, &saw303, &saw341, &saw390, &saw455, &saw546, &saw682, &saw910, &saw1365, &saw2730, &saw4096 };




void setup() 
    {
    _square.setCutoffFreqs(/*72, 76, */ 81, 86, 92, 98, 106, 115, 126, 138, 154, 174, 199, 234, 282, 356, 481, 744, 1638, 2730);
    _saw.setCutoffFreqs(/* 143, */ 151, 160, 170, 182, 195, 210, 227, 248, 273, 303, 341, 390, 455, 546, 682, 910, 1365, 2730, 4096);
    _square.setFreq(FREQUENCY(60));
    _saw.setFreq(FREQUENCY(60));
    _sine.setFreq(FREQUENCY(60));

    startMozzi();

    pinMode(CV_IN3, OUTPUT);
    pinMode(CV_AUDIO_IN, INPUT);
    pinMode(CV_GATE_OUT, INPUT);
    /// Setup MIDI
    initializeParser(&parse, CHANNEL, 0, 1);
    softSerial.begin(MIDI_RATE);
    //Serial.begin(115200);
    load();
    }
    
uint8_t weights[4];			

uint8_t gate = 0;
uint8_t timer = 0;
uint8_t velocity = 63;
uint8_t pitch;

void cc(midiParser* parser, unsigned char parameter, unsigned char value)
    {
    if (parameter == 120 || parameter == 123)	// All Sound Off or All Notes Off
	{
	// everyone is off, lower gate
	digitalWrite(CV_IN3, 0);
	gate = 0;
	}
    }
	
void noteOn(midiParser* parser, unsigned char note, unsigned char _velocity)
    {
    pitch = note;
	
    _square.setFreq(FREQUENCY(note));
    _saw.setFreq(FREQUENCY(note));
    _sine.setFreq(FREQUENCY(note));

    velocity = _velocity;
	
    // I have to delay the gate to give the oscillators time.  Originally
    // I was delaying the gate for new NOTE ON in legato, but now I do it always	
    timer = 2;
    digitalWrite(CV_IN3, 0);
    gate = 0;	
    }

void noteOff(midiParser* parser, unsigned char note, unsigned char velocity)
    {
    if (note == pitch)
	{
	timer = 0;
	gate = 0;
	digitalWrite(CV_IN3, 0);
	}
    }

#define MAX_LEN 512
uint16_t len = 0;
uint8_t data[MAX_LEN];			// this will push us to the edge...

// Write wave sequence.  I want to be careful here to avoid burning out EEPROMS!.  
// I have two levels of protection.
// 1. I am using update(), not write()
// 2. This function is only called if 'recording' is turned on, and the immediately
//    afterwards, 'recording' is turned off.  'recording' is only turned on when 
//    we enter recording the first time.
void store() 
    { 
    // stretch first
    if (len >= 2 && len < MAX_LEN)
	{
	for(uint16_t i = len; i < MAX_LEN; i++)
	    {
	    data[i] = data[i - 2];
	    data[i + 1] = data[i + 1 - 2];
	    }
	}
    len = MAX_LEN;
    for(uint16_t i = 0; i < MAX_LEN; i++)
	EEPROM.update(i, data[i]); 
    }

// load wave sequence
void load() 
    { 
    for(uint16_t i = 0; i < MAX_LEN; i++)
	data[i] = EEPROM.read(i); 
    }
	


void updateWeights(uint8_t x, uint8_t y)
    {
/// AMPLITUDE OF WAVE 1 = (1-x) * (1-y)
/// AMPLITUDE OF WAVE 2 = x * y
/// AMPLITUDE OF WAVE 3 = (1-x) * y
/// AMPLITUDE OF WAVE 4 = x * (1-y)

    weights[0] = ((127 - x) * (127 - y)) >> 8;
    weights[1] = (x * y) >> 8;
    weights[2] = ((127 - x) * y) >> 8;
    weights[3] = (x * (127 - y)) >> 8;
    }

uint16_t count = 0;
#define GATE_INITIALIZED 255
uint16_t lastGate = GATE_INITIALIZED;
uint8_t controlCounter = 0;

#define MODE_FREE 	0
#define MODE_PLAY	1
#define MODE_RECORD 2

uint8_t lastMode = 255;
uint8_t recording;

void updateControl() 
    {
    controlCounter++;
    	
    uint8_t mode = (mozziAnalogRead(CV_POT3) * 3) >> 10;
    
    if (mode == MODE_FREE)
    	{
    	if (recording) store();
    	recording = false;			// turn off recording so we don't store multiple times!
    	
    	uint8_t x = mozziAnalogRead(CV_POT_IN1) >> 3;
    	uint8_t y = mozziAnalogRead(CV_POT_IN2) >> 3;
    	updateWeights(x, y);
    	}
    else if (mode == MODE_PLAY)
    	{
	if (controlCounter & 1)
	    {
	    if (recording) store();
	    recording = false;			// turn off recording so we don't store multiple times!
		
	    if (gate && (lastGate == 0 || lastGate == GATE_INITIALIZED))
    		{
    		count = 0;
    		updateWeights(data[count], data[count+1]);
    		}
	    else if (gate)
    		{
    		count += 2;
#ifdef LOOP    		
    		if (count > (MAX_LEN - 2)) count = 0;
#else
    		if (count > (MAX_LEN - 2)) count = (MAX_LEN - 2);
#endif
    		updateWeights(data[count], data[count+1]);
    		}
	    else	// gate up
    		{
    		}
	    }
    	}
    else		// mode == MODE_RECORD
    	{
	if (controlCounter & 1)
	    {
	    if (gate)
    		{
    		if (gate && (lastGate == 0 || lastGate == GATE_INITIALIZED))		// just raised gate
		    {
		    count = 0;
		    len = 0;
		    recording = true;			// start recording
		    }
    			
    		if (len < MAX_LEN)
		    {
		    uint8_t x = mozziAnalogRead(CV_POT_IN1) >> 3;
		    uint8_t y = mozziAnalogRead(CV_POT_IN2) >> 3;
    	
    		data[count] = x;
		    data[count+1] = y;
		    updateWeights(data[count], data[count+1]);
		    count += 2;
		    len += 2;
		    }
    		else	// len >= MAX_LEN, we're done
		    {
		    if (recording) store();
		    recording = false;			// turn off recording so we don't store multiple times!
		    }
    		}
	    else
    		{
    		if (recording) store();
    		recording = false;			// turn off recording so we don't store multiple times!
    		}
	    }
    	}
    lastGate = gate;    

    if (timer > 0)
    	{
    	if (timer == 1)
			{
			digitalWrite(CV_IN3, 1);
			//Serial.println("Go");
			gate = 1;
			controlCounter = 0;	// so next time it'll be 1
			}
    	timer--;
    	}
    	
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
    int16_t outputs[4];

#if defined(USE_NOISE)
	outputs[0] = 0;
#else
    outputs[0] = _sine.next();
#endif

#if defined(USE_SINE)
	outputs[1] = 0;
#else
#if defined (DARK_NOISE)
    outputs[1] = (outputs[1] + (int8_t)(rnd4294967294())) >> 1;	// noise.  This RNG is faster than xorshift, which is 32 bits
#else
    outputs[1] = (int8_t)(rnd4294967294());	// noise.    This RNG is faster than xorshift, which is 32 bits
#endif
#endif

    outputs[2] = _square.next();
    outputs[3] = _saw.next();
 
    return ((((outputs[0] * weights[0]) +
	    (outputs[1] * weights[1]) +
	    (outputs[2] * weights[2]) +
	    (outputs[3] * weights[3])) >> 5) * velocity) >> 8;
    }

void loop() 
    {
    audioHook();
    }
