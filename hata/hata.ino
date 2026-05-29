// Copyright 2026 Sean Luke 
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


//
/// HATA
///
/// Hata is meant to run on the AE Modular GRAINS, but it could be adapted
/// to any Arduino.  Hata was written by Sean Luke [sean@cs.gmu.edu]
///
/// SET GRAINS TO GRAINS MODE.
///
/// Hata converts four trigger inputs into MIDI notes for the Wonkystuff Playa.
/// This allows you to drive Playa from a TRIQ164 or other trigger emitter.
/// This performs essentially the same task as the Wonkystuff CVMx, only for
/// just four triggers, not eight.
///
/// You can also set POT 3 to do any ONE of the following:
///
/// - Reduce the Bit Depth on the Playa
/// - Reduce the Sample Rate on the Playa
/// - Change the Pitch on the Playa
/// - Set the note velocity (volume) on the Playa (the default is maximum)
/// - Select which four drum notes on the Playa are being played from the
///   Hata, from 8 combo presets that you have created.  This is the default
///   POT 3 parameter.
///
/// To set POT 3 to change the bit depth, uncomment the following #define:

//	#define BIT_DEPTH

/// To set POT 3 to change the sample rate, uncomment the following #define:

//  #define SAMPLE_RATE

/// To set POT 3 to change the pitch, uncomment the following #define:

//  #define PITCH

/// To set POT 3 to change the velocity, uncomment the following #define:

//  #define VELOCITY

/// If you have not uncommented any of these, then Hata will use POT 3 to select
/// the four MIDI notes corresponding to the four TRIG164 triggers.  The Playa 
/// has drums assigned to MIDI notes 60 through 67.  Below is the array which
/// describes 8 combinations of four notes each.  If you HAVE uncommented any
/// of the #defines above, then Hata will use the first combination in the array.
/// With care you can change these values:

uint8_t combos[8][4] = 
	{
	{ 60, 61, 62, 63 },
	{ 64, 65, 66, 67 },
	{ 60, 61, 66, 67 },
	{ 62, 63, 64, 65 },
	{ 60, 62, 64, 66 },
	{ 61, 63, 65, 67 },
	{ 60, 63, 64, 67 },
	{ 61, 62, 65, 66 },
	};


/// We presume you are using a GRAINS, not a GRAINS V2, of which at present I'm the only
/// owner in the world outside of Tangible Waves.  If you ARE on a GRAINS V2, you will want
/// to use hardware serial output, not soft serial, which isn't very reliable.  To do that
/// you would comment out the following #define (normally don't do this):

#define USE_SOFT_SERIAL


/// NOTE
///
/// Hata assumes that your playa has default learned MIDI settings for its notes and
/// CC parameters.  If you have changed them, you will need to change them back.  To do
/// this, you can long-press the Playa button, then send it a MIDI CC 32 (any value),
/// and also play a MIDDLE C (MIDI 60).




/// CONFIGURATION
///
/// IN 1            Trigger Input 1
/// IN 2            Trigger Input 2
/// IN 3            Trigger Input 3
/// AUDIO IN (A)    Trigger Input 4
/// AUDIO OUT       [UNUSED]
/// DIGITAL OUT (D) MIDI Out to Playa (on Grains)
///
/// POT 1           [UNUSED] Set to maximum value and turn on IN1
/// POT 2           [UNUSED] Set to maximum value and turn on IN2
/// POT 3           Sets parameter as described earlier.
///
/// Serial RX		MIDI Out to Playa (on Grains V2)        



#define DEFAULT_SELECTION (0)
#define DEFAULT_VELOCITY (127)
#define BIT_DEPTH_CC (32)
#define SAMPLE_RATE_CC (33)
#define PITCH_CC (34)


////////// PINOUTS

#define CV_POT_IN1    A2    // Trigger Input 1
#define CV_POT_IN2    A1    // Trigger Input 2
#define CV_POT3       A0    // Parameter
#define CV_IN3        A3    // Trigger Input 3
#define CV_AUDIO_IN   A4    // Trigger Input 4
#define CV_AUDIO_OUT  11    // Unused
#define CV_GATE_OUT   8     // MIDI Out

#define BLANK_SERIAL  5		// Blank Serial Pin
#define PIN_UNUSED 	  255
#define MIDI_RATE 	  31250

#include "emitmidi.c"
#ifdef USE_SOFT_SERIAL
#include "NeoSWSerial.h"
NeoSWSerial softSerial(BLANK_SERIAL, CV_GATE_OUT, PIN_UNUSED);
#endif

midiEmitter midiEmitter;
uint8_t noteOn[4] = { 0, 0, 0, 0 };
uint8_t trigger[4] = { 0, 0, 0, 0 };
uint8_t oldTrigger[4] = { 0, 0, 0, 0 };

void emitMidi(struct midiEmitter* emitter, unsigned char byte)
	{
#ifdef USE_SOFT_SERIAL
	softSerial.write(byte);
    		Serial.println(byte);
#else
	Serial.write(byte);
#endif
	}


void setup()
    {
#ifdef USE_SOFT_SERIAL
    softSerial.begin(MIDI_RATE);
#else GRAINS_V2
	Serial.begin(MIDI_RATE);
#endif
    initializeEmitter(&midiEmitter, 0);
    Serial.begin(115200);
    }
    
uint8_t lastParam = 255;

void loop()
    {
    // Turn off previous triggers -- hopefully this isn't too fast
	for(uint8_t i = 0; i < 4; i++)
    	{
    	if (noteOn[i]) 
    		{
    		emitNoteOff(&midiEmitter, noteOn[i], 0);
    		}
    	noteOn[i] = 0;
    	}

    // Read new triggers and parameters
    
	uint8_t param = (uint8_t)(analogRead(CV_POT3) >> 3);
    trigger[0] = digitalRead(CV_POT_IN1);
    trigger[1] = digitalRead(CV_POT_IN2);
    trigger[2] = digitalRead(CV_IN3);
    analogRead(CV_AUDIO_IN);		// throwaway  -- necessary?
    trigger[3] = digitalRead(CV_AUDIO_IN);
    
    // Do I have a NEW trigger?
    for(uint8_t i = 0; i < 4; i++)
    	{
    	uint8_t tr = trigger[i];
		if (trigger[i] && oldTrigger[i])
			{
			trigger[i] = 0;		// get rid of it
			}
    	oldTrigger[i] = tr;
    	}
    
    // Emit new triggers and parameters
    if (trigger[0] || trigger[1] || trigger[2] || trigger[3])
    	{
    	if (lastParam != param)
    		{
    		lastParam = param; 
			#if defined(BIT_DEPTH)
				emitCC(&midiEmitter, BIT_DEPTH_CC, param, 0);
			#elif defined(SAMPLE_RATE)
				emitCC(&midiEmitter, SAMPLE_RATE_CC , param, 0);
			#elif defined(PITCH)
				emitCC(&midiEmitter, PITCH_CC, param, 0);
			#endif
			}
    	}
    
    // determine selection or velocity based on POT 3
    uint8_t selection = DEFAULT_SELECTION;
    uint8_t velocity = DEFAULT_VELOCITY;
    #if defined(BIT_DEPTH)
    #elif defined(SAMPLE_RATE)
    #elif defined(PITCH)
    #elif defined(VELOCITY)
    	velocity = param;
    #else 
    	selection = (param >> 4);
    #endif
    
    // Send MIDI notes to channel 0
    for(uint8_t i = 0; i < 4; i++)
    	{
   		if (trigger[i]) 
   			{
   			emitNoteOn(&midiEmitter, noteOn[i] = combos[selection][i], velocity, 0);
   			}
   		}
    }
