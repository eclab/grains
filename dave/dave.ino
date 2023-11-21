// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// DAVE
///
/// Dave is a USB MIDI bridge.  It is meant to run on the AE Modular GRAINS, but it could be 
/// adapted to any Arduino.  Dave takes serial data received over USB and routes it
/// to TTL MIDI (such as proposed by WonkyStuff).  Dave comes with a small Java program
/// which works on your laptop: it takes data from a MIDI Device (typically a MIDI Loopback)
/// and sends it over MIDI.
///
/// FIXME: set up Dave to divide MIDI up and send it out different ports, at least D, Audio In, and In3.
/// Can we send it out Audio Out?  Dunno, it has a filter attached to it...
///
/// SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)
///
/// CONFIGURATION
///
/// IN 1            [Unused]
/// IN 2            [Unused]
/// IN 3            Clock
/// AUDIO IN (A)    Reset
/// AUDIO OUT       Clock Division 1
/// DIGITAL OUT (D) MIDI Out
///
/// POT 1           Choice of Clock Division 1   [Set the switch to MAN]
///                 [changing this value triggers a reset]
///
/// POT 2           Choice of Clock Division 2  [Set the switch to MAN]
///                 [changing this value triggers a reset]
///
/// POT 3           Pulse Width (rounded down to nearest clock.  If at 0, a trigger pulse is sent)
///
/// CLOCK DIVISION CHOICES
///
/// At present the options are (left to right):
///
/// 96 clocks per pulse		          (MIDI Clock bars)
/// 64 clocks per pulse
/// 32 clocks per pulse
/// 24 clocks per pulse		          (MIDI Clock quarter notes)
/// 16 clocks per pulse
/// 8 clocks per pulse
/// 6 clocks per pulse		          (MIDI Clock 16th notes)
/// 4 clocks per pulse offset by 2    (Binary 4 digit)
/// 4 clocks per pulse
/// 3 clocks per pulse
/// 2 clocks per pulse offset by 1    (Binary 2 digit)  (1 digit is the main clock pulse)
/// 2 clocks per pulse
///
/// You could change these any way you liked right here:



#include "SoftwareSerialTX.h"
#include "MIDI.h"



////////// PINOUTS

#define CV_POT_IN1    A2    // Choice of Clock Division 1
#define CV_POT_IN2    A1    // Choice of Clock Division 1
#define CV_POT3       A0    // Pulsewidth
#define CV_IN3        A3    // Clock
#define CV_AUDIO_IN   A4    // Reset
#define CV_AUDIO_OUT  11    // Division 1
#define CV_GATE_OUT   8     // Division 2

// Create and bind the MIDI interface to the default hardware Serial port
MIDI_CREATE_DEFAULT_INSTANCE();

SoftwareSerialTX out1(CV_GATE_OUT);
SoftwareSerialTX out2(CV_IN3);

uint8_t noteOut[3];
void handleNoteOn(byte channel, byte note, byte velocity)
	{
	noteOut[0] = (0x90 + channel);
	noteOut[1] = note;
	noteOut[2] = velocity;
	out1.write(noteOut);
	}

void handleNoteOff(byte channel, byte note, byte velocity)
	{
	noteOut[0] = (0x80 + channel);
	noteOut[1] = note;
	noteOut[2] = velocity;
	out1.write(noteOut);
	}

void setup()
    {
    pinMode(CV_GATE_OUT, OUTPUT);
    pinMode(CV_IN3, OUTPUT);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.begin(MIDI_CHANNEL_OMNI);  // Listen to all incoming messages
	out1.begin(31250);		// MIDI Rates
	Serial.begin(115200);
	}
	
void loop()
	{
    MIDI.read();
    }
