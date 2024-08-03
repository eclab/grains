// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// BOOKER-M
///
/// Booker is a MIDI-CONTROLLED Hammond Organ simulator, complete with Leslie.  It's the MIDI version
/// of BOOKER.  It is meant to run on the AE Modular GRAINS, but it could be  adapted to any Arduino.  
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.
///
/// ALIASING: At extremely high frequencies, a few of Booker-M's drawbars will go over Nyquist and
/// you may hear some antialiasing effects.  I can fix this but it puts me over the
/// computational budget of the device, so it's not going to happen.
///
/// THE LESLIE.  Booker-M comes with a Leslie, which is on by default:

#define LESLIE_ON               // To turn this off, put a // before the #define, as in //#define

// Leslie changes both the amplitude and pitch at a certain rate.
// You can adjust them here

#define LESLIE_FREQUENCY 5.66                           // This is the 450 speed.  The classic slower speed is 0.66, but it's too slow
#define LESLIE_PITCH 256                                 // Values are 1.0 (lots) ... 128.0 or more (little).  Can be floating point.

/// CHANNEL
///
/// You can set Booker-M's channel to 0...15 (for channels 1...16) or to OMNI (to listen to any of them).
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


/// CONFIGURATION
///
/// IN 1            [Unused]
/// IN 2            Unused
/// IN 3            Gate Out
/// AUDIO IN (A)    Pitch Tune
/// AUDIO OUT       Out
/// DIGITAL OUT (D)	MIDI In
///
/// POT 1           [Unused]
///
/// POT 2           Volume
///
/// POT 3           Choice of Organ [Set the switch to MAN]



/// Lastly, here is our present drawbar selection.  Keep this to no more than 31 but feel free
/// to change the selections.  There is a big list of 99 selections below you could swap in.


#define NUM_DRAWBAR_SELECTIONS 16
uint8_t drawbars[NUM_DRAWBAR_SELECTIONS][9] = 
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
#define CONTROL_RATE 128

#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin256_int8.h>
#include <tables/sin1024_int8.h>
#include "NeoSWSerial.h"   // you have to install this via the library manager
#include "parsemidi.c"

/// OSCILLATORS

Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> tonewheel1(SIN1024_DATA);
Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> tonewheel2(SIN1024_DATA);
Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> tonewheel3(SIN1024_DATA);
Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> tonewheel4(SIN1024_DATA);
Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> tonewheel5(SIN1024_DATA);
Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> tonewheel6(SIN1024_DATA);
Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> tonewheel7(SIN1024_DATA);
Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> tonewheel8(SIN1024_DATA);
Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> tonewheel9(SIN1024_DATA);

Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> oscils[9] = 
    { 
    tonewheel1, tonewheel2, tonewheel3,
    tonewheel4, tonewheel5, tonewheel6, 
    tonewheel7, tonewheel8, tonewheel9 
    };

#ifdef LESLIE_ON
Oscil<SIN256_NUM_CELLS, CONTROL_RATE> leslie(SIN256_DATA);              // Leslie LFO
#endif
        
//float drawbarFrequencies[9] = { 0.5, 1.0, 1.5, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0 };
uint8_t drawbarFrequencies[9] = { 1, 2, 3, 4, 6, 8, 10, 12, 16 };

// These are the amplitudes of each of the drawbar stop positions (0...8).
// Each additional stop increases by 3db.  I think I have this right?  Not sure.
// We are int8_t, not uin8_t, so we can multiply faster against oscils, which is
// also int8_t.
int8_t drawBarAmplitudes[9] = 
    {
    0, 7, 18, 32, 48, 65, 85, 105, 127
    };


#define CV_POT_IN1    A2    // Overall Volume
#define CV_POT_IN2    A1    // Leslie Volume
#define CV_POT3       A0    // Organ Selection
#define CV_IN3        A3    // Gate Out
#define CV_AUDIO_IN   A4    // [Unused]
#define CV_AUDIO_OUT  9     // Out
#define CV_GATE_OUT   8     // MIDI In


#define MIDI_RATE 31250
#define BLANK_SERIAL	  5		// Blank Serial Pin
#define PIN_UNUSED 255
NeoSWSerial softSerial(CV_GATE_OUT, BLANK_SERIAL, PIN_UNUSED);
midiParser parse;


uint8_t on = 0;
uint8_t gate = 0;
uint8_t timer = 0;
uint8_t pitch;


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
	
float noteFrequency = 65.408;		// middle c?

void noteOn(midiParser* parser, unsigned char note, unsigned char velocity)
	{
	pitch = note;

    // set the drawbars
    noteFrequency = FREQUENCY(note);
        
    // ignore velocity
		
	on = 1;
		
	/* if (gate)
		{
		Serial.println("GATE OFF");
		timer = 2;
		digitalWrite(CV_IN3, 0);
		gate = 0;
		}
	else */
		{
		//digitalWrite(CV_IN3, 1);
		gate = 1;		
		timer = 2;
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


void setup()
    {
    //Serial.begin(9600);
    pinMode(CV_IN3, OUTPUT);
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
        
	initializeParser(&parse, CHANNEL, 0, 1);
	softSerial.begin(MIDI_RATE);
    }

void loop()
    {
    audioHook();
    }
    

uint8_t organ = 0;      // Current index into the drawbars array
uint16_t gain;          // our current gain

#define COUNTER_MAX 4
uint16_t counter =0;
void updateControl()                          
    {
    if (timer > 0)
    	{
    	if (timer == 1)
    		{
    		if (gate) digitalWrite(CV_IN3, 1);
    		}
    	timer--;
    	}
    	
    uint8_t val = softSerial.available();
	for(uint8_t i = 0; i < val; i++)
		{
		parseMidi(&parse, softSerial.read());
		}

#ifdef LESLIE_ON
    uint8_t leslieVolume = (mozziAnalogRead(CV_POT_IN2) * 7) >> 10;		// 0...6 max	
#endif
    
	float frequency = noteFrequency;

    // leslie!
#ifdef LESLIE_ON
    int8_t les = leslie.next();
    frequency += les * (1.0 / LESLIE_PITCH);
#endif

    for(uint8_t i = 0; i < 9; i++)
        {
        oscils[i].setFreq((noteFrequency * drawbarFrequencies[i]));
        }

    // determine the gain
    gain = 1 + (mozziAnalogRead(CV_POT_IN1) >> 2)                       // Gain (Volume)
#ifdef LESLIE_ON
        + ((les + 128) >> (8 - (leslieVolume - 1)));         // note: >> 8 still goes between -1 and 0 depending on whether it's positive or negative initially
#endif
    ;

    // what organ are we playing?
    if (++counter == COUNTER_MAX)
        {
        counter = 0;
        organ = (organ + (mozziAnalogRead(CV_POT3) * NUM_DRAWBAR_SELECTIONS)) >> 10;  // Organ Selection
        }
        
    //Serial.println(frequency);
    }                                             

int updateAudio()                             
    { 
    uint8_t* d = drawbars[organ];
    int32_t val = oscils[0].next() * d[0];		// hoisting this out gives me just enough computational space to avoid clicks, ugh
    for(uint8_t i = 1; i < 9; i++)
        {
        val += (oscils[i].next() * d[i]);
        }
    return ((val >> 8) * gain) >> 8;
    }
