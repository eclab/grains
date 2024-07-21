// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// VS
///
/// VS is a MIDI-driven 1-voice vector synthesizer.
/// VS is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.
/// VS only responds to MIDI, via the wonkystuff mb/1, IMDI MAIN module, or from DAVE. 
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.
///
/// VS crossfades between four waveforms in a special way.  You can control the cross-fading
/// manually with two CVs, such as with a joystick; and you can also use the same to program
/// about 4 seconds of CV crossfading movement.
///
///
/// HOW VECTOR SYNTHESIS WORKS
///
/// Vector synthesis dynamically changes the cross-fade between four different sound waves.
/// To control the cross-fade mix you modify two CVs called X and Y.  Imagine that these CV
/// values each go from 0 to 1 inclusive, so together they define a square region from (0,0) to (1,1). 
/// The amplitude of the four waves is defined as:
///
/// AMPLITUDE OF WAVE 1 = (1-x) * (1-y)
/// AMPLITUDE OF WAVE 2 = x * y
/// AMPLITUDE OF WAVE 3 = (1-x) * y
/// AMPLITUDE OF WAVE 4 = x * (1-y)
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
/// VS allows you set the waves you'd like to use.  You have to provide waveform files for all
/// four waves, and they must all be different.  You can use any of the files in the "vs" 
/// directory, and you can also use any of the AKWF waveform files from the AKWF project 
/// (just copy them over to the "vs" directory and use them from there)

#define VS_1        "vs/vs_100.h"              
#define VS_2        "vs/vs_42.h"
#define VS_3        "vs/vs_76.h"               
#define VS_4        "vs/vs_53.h"


/// PLAYING MODES
///
/// VS has three MODES, specified by POT 3.
///
///	FREE MODE: the wave mix is set by changing IN1/POT1 and IN2/POT2.
///
/// RECORD MODE: the wave mix is **recorded** by changing IN1/POT1 and IN2/POT2 for up to 4 seconds. 
/// When a note is played, the GATE OUT is raised, and when a note is released, the GATE OUT is lowered.  
/// Recording stops when (1) the time has elapsed or (2) the note is released or (3) the mode is 
/// changed back to PLAY or FREE.


///
/// When you have stopped recording, VS will save your internal recording to the EEPROM so that it's
/// available even after you reboot.
///
/// PLAY MODE: when a note is played, the recorded wave mix is played back.  When a note is played,
/// the GATE OUT is raised, and when a note stops being played, the GATE OUT is lowered.


/// CHANNEL
///
/// You can set VS's channel to 0...15 (for channels 1...16) or to OMNI (to listen to any of them).
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
/// AUDIO IN (A)    [Unused]                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
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

#include VS_1
Oscil<DATA_LENGTH, 16384> osc0(DATA);
uint16_t lengths0 = DATA_LENGTH;
#undef DATA
#undef DATA_LENGTH

#include VS_2
Oscil<DATA_LENGTH, 16384> osc1(DATA);
uint16_t lengths1 = DATA_LENGTH;
#undef DATA
#undef DATA_LENGTH

#include VS_3
Oscil<DATA_LENGTH, 16384> osc2(DATA);
uint16_t lengths2 = DATA_LENGTH;
#undef DATA
#undef DATA_LENGTH

#include VS_4
Oscil<DATA_LENGTH, 16384> osc3(DATA);
uint16_t lengths3 = DATA_LENGTH;
#undef DATA
#undef DATA_LENGTH




#include "NeoSWSerial.h"   // you have to install this via the library manager
#include "parsemidi.c"

#define MIDI_RATE 31250
#define BLANK_SERIAL	  5		// Blank Serial Pin
#define PIN_UNUSED 255
NeoSWSerial softSerial(CV_GATE_OUT, BLANK_SERIAL, PIN_UNUSED);
midiParser parse;


void setup() 
    {
    startMozzi();

	pinMode(CV_IN3, OUTPUT);
	pinMode(CV_GATE_OUT, INPUT);
	/// Setup MIDI
	initializeParser(&parse, CHANNEL, 0, 1);
	softSerial.begin(MIDI_RATE);
	osc0.setFreq(FREQUENCY(60));
	osc1.setFreq(FREQUENCY(60));
	osc2.setFreq(FREQUENCY(60));
	osc3.setFreq(FREQUENCY(60));
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
	
	osc0.setFreq(FREQUENCY(note));
	osc1.setFreq(FREQUENCY(note));
	osc2.setFreq(FREQUENCY(note));
	osc3.setFreq(FREQUENCY(note));

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

uint16_t len = 0;
uint8_t data[1024];			// this will push us to the edge...

// Write wave sequence.  I want to be careful here to avoid burning out EEPROMS!.  
// I have two levels of protection.
// 1. I am using update(), not write()
// 2. This function is only called if 'recording' is turned on, and the immediately
//    afterwards, 'recording' is turned off.  'recording' is only turned on when 
//    we enter recording the first time.
void store() 
	{ 
	// stretch first
	if (len >= 2 && len != 1024)
		{
		for(uint16_t i = len; i < 1024; i++)
			{
			data[i] = data[i - 2];
			data[i + 1] = data[i + 1 - 2];
			}
		}
	len = 1024;
	for(uint16_t i = 0; i < 1024; i++)
		EEPROM.update(i, data[i]); 
	}

// load wave sequence
void load() 
	{ 
	for(uint16_t i = 0; i < 1024; i++)
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

#define MODE_FREE 	0
#define MODE_PLAY	1
#define MODE_RECORD 2

uint8_t lastMode = 255;
uint8_t recording;

void updateControl() 
    {
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
    		if (count > 1022) count = 1022;
    		updateWeights(data[count], data[count+1]);
    		}
    	else	// gate up
    		{
    		}
    	}
    else		// mode == MODE_RECORD
    	{
    	if (gate)
    		{
    		if (gate && (lastGate == 0 || lastGate == GATE_INITIALIZED))		// just raised gate
    			{
    			count = 0;
    			len = 0;
    			recording = true;			// start recording
    			}
    			
    		if (len < 1024)
    			{
				uint8_t x = mozziAnalogRead(CV_POT_IN1) >> 3;
    			uint8_t y = mozziAnalogRead(CV_POT_IN2) >> 3;
    	
    		    data[count] = x;
	    		data[count+1] = y;
    			updateWeights(data[count], data[count+1]);
    			count += 2;
    			len += 2;
    			}
    		else	// len >= 1024, we're done
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
    
    lastGate = gate;
     
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

int16_t outputs[4];

int updateAudio()    
    {
    outputs[0] = osc0.next();
    outputs[1] = osc1.next();
    outputs[2] = osc2.next();
    outputs[3] = osc3.next();
    	 
	return (((outputs[0] * weights[0]) +
    	 (outputs[1] * weights[1]) +
    	 (outputs[2] * weights[2]) +
    	 (outputs[3] * weights[3])) * (int32_t)velocity) >> 12;
    }

void loop() 
    {
    audioHook();
    }
