// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the GNU Public License 2.0

/// DAVE
///
/// Dave is a MIDI router and utility which can do a variety of things.  Dave is meant to run 
/// on the AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// Set your GRAINS to GRAINS Mode.  Dave does not work in Mozzi mode.
///
/// DAVE REQUIRES that you install the NeoHWSerial library from the Arduino library manager.  It has
/// its own customized (and bug-fixed) copy of the NeoSWSerial library, so do not install
/// that one (and in fact you may need to uninstall it).
///
/// Dave runs in a variety of modes.  Many of these modes are meant to receive MIDI
/// over USB, or in one case from the WonkyStuff MB/1, and route it to WonkyStuff 
/// modules or to the MASTER I/O.  The GRAINS USB port cannot present itself as a 
/// MIDI Device, so Dave works with a Java program, also called Dave, which receives 
/// MIDI from your DAW or controller and sends it over the USB port to the GRAINS.  
/// This program is located in the "java" subdirectory.
///
/// The general hookup to a controller as follows: DAVE <--- Dave Java Program <---- Controller Device
/// The hookup to a typical DAW is:  DAVE <--- Dave Java Program <--- MIDI Loopback Device <--- DAW
///     (Though Logic on the Mac doesn't need a Loopback.  Ableton does)
/// See the Dave java program README for more information.
///
/// You may have noticed that neither the MB/1 nor MASTER I/O have a WonkyStuff-style MIDI In port.
/// They just have MIDI TRS-B jacks.  However it is possible to send MIDI to them from the GRAINS
/// with some wiring.   See the file "docs/TRS.md" in the "dave" directory for more information.
///
/// DAVE works in one or more MODES and has a few configuration options beyond that.
///
/// Dave also has a facility to convert .mid files to .h files that you can cut and paste into GRAINS Beats.
///
///
/// THE MODES
///
/// Here are the modes:
///
/// USB CLOCK MODE
/// This mode takes MIDI input over USB and outputs clock signals.
///
/// - DIGITAL OUT outputs a GATE: it goes high when receiving a START or CONTINUE,
///   and goes low when receiving a STOP
/// - AUDIO OUT outputs CLOCK in-between a MIDI START/CONTINUE and STOP.
/// - IN 3 outputs a trigger (RESET) when it receives START or CONTINUE
/// - AUDIO IN outputs a trigger when it receives a STOP
/// - The Pots should NOT be set to output anything
///
///
/// USB ROUTER MODE
/// This mode takes MIDI input over USB and outputs directly to DIGITAL OUT. This can be useful
/// to send MIDI data directly to a one Wonkystuff MCO/1, MCC/4, MTR/8, or MDIV.  With careful
/// wiring you could also feed into a MASTER I/O with some caveats and disclaimers.
///
/// - DIGITAL OUT outputs MIDI from USB
/// - AUDIO OUT outputs CLOCK in-between a MIDI START/CONTINUE and STOP.
/// - IN 3 outputs a trigger (RESET) when it receives START or CONTINUE
/// - AUDIO IN outputs a trigger when it receives a STOP
/// - The Pots *can* be set to output CCs of your choice.
/// - Optionally AUDIO IN receives CV for a CC if EXTRA_CC_4 is set
/// - Optionally IN 3 receives CV for a CC if EXTRA_CC_5 is set
///
///
/// USB DISTRIBUTOR MODE
/// This mode takes MIDI input over USB and outputs to DIGITAL OUT, but redistributes Channel 1.
/// to several other channels For example, the first note goes to channel 2, the second note goes
/// to channel 3, and so on.  The number of voices (up to 15) is set by the #define NUM_VOICES below.  
/// This is useful in combination when fed into with a WonkyStuff MB/1, with careful wiring:
/// it would allow you to have a polyphonic synthesizer controlled by a normal
/// MIDI Controller.
///
/// - DIGITAL OUT outputs MIDI from USB, with channels modified as described
/// - AUDIO OUT outputs CLOCK in-between a MIDI START/CONTINUE and STOP.
/// - IN 3 outputs a trigger (RESET) when it receives START or CONTINUE
/// - AUDIO IN outputs a trigger when it receives a STOP
/// - The Pots *can* be set to output CCs of your choice.
/// - Optionally AUDIO IN receives CV for a CC if EXTRA_CC_4 is set
/// - Optionally IN 3 receives CV for a CC if EXTRA_CC_5 is set
/// 
///
/// USB MPE MODE
/// This mode takes MIDI input over USB and and outputs directly to DIGITAL OUT.  It assumes that we
/// are doing MPE with a ZONE LOW, where Channel 1 is the MPE Master channel, and the individual
/// channels start at 2.  The number of voices (hence channels except the Master channel) is set by the
/// #define NUM_VOICES below.  This is useful in combination when fed into with a WonkyStuff MB/1, 
/// with careful wiring to create an MPE-controlled polysynth.
///
/// - DIGITAL OUT outputs MIDI from USB, with channels modified as described
/// - AUDIO OUT outputs CLOCK in-between a MIDI START/CONTINUE and STOP.
/// - IN 3 outputs a trigger (RESET) when it receives START or CONTINUE
/// - AUDIO IN outputs a trigger when it receives a STOP
/// - The Pots *can* be set to output CCs of your choice.
/// - Optionally AUDIO IN receives CV for a CC if EXTRA_CC_4 is set
/// - Optionally IN 3 receives CV for a CC if EXTRA_CC_5 is set
///
///
/// USB TRIGGERS MODE
/// This takes four different MIDI notes from USB and generates drum triggers, similar to a WonkyStuff
///  MTR/8.  The notes are Middle C (60), D (62), E (64), and F (65), but you can change
/// them if you needed to for some reason by modifying the "triggerNotes" variable below: it
/// holds the MIDI pitch values for each of the four notes.
/// 
/// - AUDIO OUT outputs trigger 1 (Middle C)
/// - IN 3 outputs trigger 2 (D)
/// - AUDIO IN outputs trigger 3 (E)
/// - DIGITAL OUT outputs trigger 4 (F)
/// - The Pots do not do anything (if you can think of something useful they could do, let me know).
/// 
///
/// INTERNAL TRIGGERS MODE
/// This is basically the same thing as USB TRIGGERS MODE except that the MIDI input is not
/// via USB but via a local MIDI socket, such as from a Wonkystuff MB/1.  There are only three
/// trigger notes: Middle C (60), D (62), and E (64), but you can change
/// them if you needed to for some reason by modifying the *first three* values in the 
/// "triggerNotes" variable below: it holds the MIDI pitch values for each of the three/four
/// notes used by the INTERNAL TRIGGERS and by USB TRIGGERS modes.
///
/// - AUDIO OUT outputs trigger 1 (Middle C)
/// - IN 3 outputs trigger 2 (D)
/// - AUDIO IN outputs trigger 3 (E)
/// - DIGITAL OUT takes MIDI IN
/// - The Pots do not do anything (if you can think of something useful they could do, let me know).
///
///
/// INTERNAL ROUTER MODE  --- DOES NOT PRESENTLY WORK, SOFTWARE SERIAL PORTS TOO SLOW --- 
/// This is the same as USB ROUTER MODE except that that the input doesn't come from USB but rather
/// from PORT 1, and the output is PORT 2.  What's the point?  CC injection is the point.  This
/// allows you, for example, to modify the CC values of a Wonkystuff MCO/1 while sending it notes.
///
/// - DIGITAL OUT receives MIDI
/// - AUDIO IN outputs MIDI from DIGITAL OUT
/// - AUDIO OUT outputs CLOCK in-between a MIDI START/CONTINUE and STOP.
/// - IN 3 outputs a trigger (RESET) when it receives START or CONTINUE
/// - The Pots *can* be set to output CCs of your choice.
/// - Optionally IN 3 receives CV for a CC if EXTRA_CC_5 is set
///
///
/// NOTE GENERATOR MODE.  This only produces MIDI: you could use it to trigger a WonkyStuff MCO/4
/// from a regular setup without USB.  It takes a pitch CV and a gate in and generates a note 
/// on CHANNEL 1 for the MCO/4, and it can also still produce the THREE output CCs, but one of 
/// them is now from a CV in.
/// 
/// - DIGITAL OUT outputs MIDI with the note data being generated
/// - IN 1 receives the pitch.  Set POT 1 to MAN, or else adjust POT 1 to scale things 
///   (probably around 2'oclock) to 1V/oct
/// - AUDIO IN receives the gate
/// - Pots 2 and 3 can be set to output CCs of your choice.  
/// - IN 3 can be set to output the CC normally reserved for POT 1.  It will not output any CC value
///   less than 8, as this likely indicates that IN 3 is disconnected.
///
///
/// USB HEX MODE.  This mode takes hexadecimal input over USB from your Arduino IDE Serial Monitor and outputs 
/// MIDI bytes over DIGITAL OUT.  It's only meant for debugging.  Don't bother with it otherwise.
///
/// - DIGITAL OUT outputs MIDI from USB originally as hexadecimal input
/// - Nothing else does anything
///
///
/// To set the MODE, you change the #define MODE below

// don't touch these, they're constants
#define USB_CLOCK 1
#define USB_ROUTER 2
#define USB_DISTRIBUTOR 3
#define USB_MPE 4
#define USB_TRIGGERS 5
#define INTERNAL_TRIGGERS 6
//#define INTERNAL_ROUTER 7
#define NOTE_GENERATOR 8
#define USB_HEX 9

/// SET THE MODE HERE
#define MODE	 USB_ROUTER				// Change this to one of the MODES above

/// SET THE NUMBER OF VOICES (for USB_MPE and USB_DISTRIBUTOR MODES) HERE
#define NUM_VOICES 3			// Can be 0 or 1 [Both Off], or any value 2-16 

/// SET THE CHANNEL THAT NOTE_GENERATOR PRODUCES HERE
#define NOTE_GENERATOR_CHANNEL 1	

/// SET THE NOTES THAT THE USB_TRIGGERS AND INTERNAL_TRIGGERS MODES MODES RESPOND TO (INTERNAL_TRIGGERS USES JUST THE FIRST THREE)
const uint8_t triggerNotes[4] = { 60, 62, 64, 65 };		// MIDDLE C, D, E, and F




/// CUSTOMIZING THE POT CC OUTPUTS
///
/// In modes other than the CLOCK modes, you can change up to three CC parameters with the pots.
/// This is not available in the USB_CLOCK mode.
///
/// WARNING: if you play or release notes immediately after or during changing the pots, DAVE
/// may not see these notes, possibly resulting in stuck or oddly behaving note states.  This is
/// because a stream of pots overwhelms the MIDI output, and so DAVE may not be able to return to
/// the incoming notes in time.  So don't do that.  Play/release notes, or change the pots, but not
/// both at the same time.
///
/// The CC PARAMETER for each Pot is defined with the following #defines:

#define POT_1_CC	5		// Wonkystuff MCO/1 Sub.  Can be any value 1...128 OR NONE.  ALSO PROVIDED ON IN 3 IN NOTE GENERATOR MODE
#define POT_2_CC	6		// Wonkystuff MCO/1 Ramp.  Can be any value 1...128 OR NONE
#define POT_3_CC	7		// Wonkystuff MCO/1 Square.  Can be any value 1...128 OR NONE
#define POT_4_CC	2		// Wonkystuff MCO/1 Noise.  Can be any value 1...128 OR NONE.    Only if EXTRA_CC_4 is defined.
#define POT_5_CC 	8		// Wonkystuff MCO/1 Square PW.  Can be any value 1...128 OR NONE.   Only if EXTRA_CC_5 is defined.

// Some modes allow additional CCs (POT 4 and POT 5) instead of clock signals.  Uncomment these to turn them on:

// #define EXTRA_CC_4
// #define EXTRA_CC_5

/// You can customize which channels will receive CCs from the pots by
/// modifying the "1" values below to "0".  Regardless of the settings here,
/// the only ones which will receive injection will be those channels from 0...NUM_VOICES-1
/// which have INJECT_POTS_TO_CHANNEL[...] set to 1.


/// DELAY

const boolean INJECT_POTS_TO_CHANNEL[16] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

// When negative, represents the last note off
// When positive, represents a delayed note on
int8_t delayPitches[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
uint8_t delayAmounts[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
uint8_t delayVels[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

#define MIN_DELAY 5

void delayOrPlayNoteOn(uint8_t pitch, uint8_t vel, uint8_t channel)
	{
	// If the last note off was too recent
	if (delayPitches[channel] < 0 &&			// Note OFF
		delayAmounts[channel] != 0 &&			// Was Released
		delayAmounts[channel] < MIN_DELAY)		// Too recently
		{
		// Play the NOTE OFF
		write(MIDI_NOTE_OFF + channel);
		write(0 - delayPitches[channel]);
		write(delayVels[channel]);
		
		// Delay the NOTE ON
		delayPitches[channel] = pitch;
		delayAmounts[channel] = MIN_DELAY - delayAmounts[channel];
		delayVels[channel] = vel;
		}
	// If we have a delayed note on, uh oh, just play it, this probably shouldn't have happened
	else if (delayPitches[channel] >= 0 &&		// Note ON
		delayAmounts[channel] != 0)			// Is Delayed
		{
		// Play and remove the DELAYED NOTE ON
		write(MIDI_NOTE_ON + channel);
		write(delayPitches[channel]);
		write(delayVels[channel]);
		delayAmounts[channel] = 0;

		// Play the NOTE ON
		write(MIDI_NOTE_ON + channel);
		write(pitch);
		write(vel);
		}
	// No delayed NOTE ON, no too-recently-delayed NOTE OFF
	else
		{
		// Play the NOTE ON
		write(MIDI_NOTE_ON + channel);
		write(pitch);
		write(vel);
		}
	}

void delayOrPlayNoteOff(uint8_t pitch, uint8_t vel, uint8_t channel)
	{
	// If we have a delayed note on, uh oh, just play it, this probably shouldn't have happened
	if (delayPitches[channel] >= 0 &&		// Note ON
		delayAmounts[channel] != 0)			// Is Delayed
		{
		// Play and remove the DELAYED NOTE ON
		write(MIDI_NOTE_ON + channel);
		write(delayPitches[channel]);
		write(delayVels[channel]);
		delayAmounts[channel] = 0;
		}
		
	// Play the NOTE OFF
	write(MIDI_NOTE_OFF + channel);
	write(pitch);
	write(vel);

	// Register the NOTE OFF
	delayPitches[channel] = pitch;
	delayVels[channel] = vel;
	delayAmounts[channel] = 1;
	}

void incrementDelay()
	{
	for(uint8_t channel = 0; channel < 16; channel++)
		{
		// If we're delayed
		if (delayAmounts[16] >= 1)
			{
			// If the delay is a NOTE OFF
			if (delayPitches[16] < 0)
				{
				delayAmounts[16]++;		// 255 will increment to 0
				}
			// If the delay is a NOTE ON
			else
				{
				// If we are ready to play the NOTE ON
				if (delayAmounts[16] == 1)
					{
					write(MIDI_NOTE_ON + channel);
					write(0 - delayPitches[channel]);
					write(delayVels[channel]);
					}
				delayAmounts[16]--;		// 1 will decrement to 0
				}
			}
		}
	}


/// DE-LEGATO
///
/// This sends a NOTE OFF before any NOTE ON, to guarantee that we do a gate prior to changing the pitch.
/// The purpose of De-Legato is to work around the fact that WonkyStuff's MCO/1 module is
/// legato-only.  HOWEVER De-Legato will trigger a MIDI bug in the MASTER I/O: if it receives two note-offs
/// in a row for the same note, its gate will hang permanently at HIGH.  So if you're sending signals to
/// the MASTER I/O, you need to turn off De-Legato.
///
/// De-Legato is specified on a per-channel basis.  By default, it's ON (1) for ALL OUTGOING CHANNELS.  
/// But you can change it here:

/// This adds a NOTE_OFF before every NOTE_ON LEAVING on the following channels, if 1, else 0
const uint8_t DELEGATO_CHANNELS[16] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
#define DELEGATO 	1			// If you set this to 0, DE-LEGATO is turned off entirely



/** NOT COMPLETED

/// SUSTAIN
/// If a SUSTAIN is received (CC 64) ignores a NOTE OFF until the next NOTE ON or until the next SUSTAIN 
/// is released.  
#define SUSTAIN 	1
*/


/// CLOCK DIVISION
///
/// You can change the MIDI clock divisor.  By default the divisor is 1, meaning 24 PPQ.
/// If you made this 6, for example, you'd get 4PPQ, otherwise known as one pulse every 16th note.
/// If you changed this to 24, you'd get one pulse every quarter note.  The FIRST pulse is at the
/// very first clock pulse after a START or a CONTINUE.  Change the value below.

#define CLOCK_DIVISOR 1					// This must be >= 1.  6 will pulse 16th notes.  24 will pulse quarter notes.









//// And we're done!




/// (Don't play with these, they're constants)
#define NONE 0
#define ALL 17
#define USE_MPE 1
#define DISTRIBUTE 2

// Configurations
#define USB_TO_NONE 0
#define USB_TO_PORT_1 1
#define USB_TO_BOTH 2
#define PORT_1_TO_PORT_2 3
#define PORT_1_TO_NONE 4
#define NONE_TO_PORT_1 5
#define USB_TO_USB 6
#define CONFIGURATION	USB_TO_PORT_1

// What channel is sent to output 1?
#define OUTPUT_CHANNEL NONE	// can be NONE, 1...16, or ALL
// How is Input is filtered to Output
#define FILTER_STYLE 	NONE		// can be NONE, USE_MPE, or DISTRIBUTE
// Are we debugging to the USB port?
#define DEBUG 1
// How many milliseconds do we wait to update the pots?  If this is too fast, it will overwhelm MIDI
// and create a lot of lag, but we'd like it to be fast enough to sound realistic.  Higher values
// are SLOWER. 
#define TIMER_SPEED 25








#include "NeoSWSerial.h"   // you have to install this via the library manager
#include <NeoHWSerial.h>    // you have to install this via the library manager
#include <FlexiTimer2.h>    // you have to install this via the library manager, but I include a zip just in case
#include "parsemidi.h"

////////// PINOUTS

#define CV_POT_IN1    A2    // CC 1 Value
#define CV_POT_IN2    A1    // CC 2 Value
#define CV_POT3       A0    // CC 3 Value
#define CV_IN3        A3    // Reset
#define CV_AUDIO_IN   A4    // PORT 2
#define CV_AUDIO_OUT  11    // MIDI Clock (only when between start/continue and stop)
#define CV_GATE_OUT   8     // PORT 1
#define BLANK_SERIAL	  5		// Blank Serial Pin




// MODE DEFINITIONS
#define PIN_UNUSED 255

#if (MODE == USB_CLOCK)
#define CONFIGURATION USB_TO_NONE
#define OUTPUT_CHANNEL NONE
#define POT_1_CC NONE
#define POT_2_CC NONE
#define POT_3_CC NONE
#define POT_4_CC NONE
#define POT_5_CC NONE
#define FILTER_STYLE 	NONE
#define GATE PORT1
#define STOP PORT2
#define CLOCK
#define RESET

#elif (MODE == USB_ROUTER)
#define CONFIGURATION	USB_TO_USB			//  USB_TO_PORT_1
#define OUTPUT_CHANNEL ALL
#define FILTER_STYLE 	NONE
#define CLOCK
#ifndef EXTRA_CC_4
#define POT_4_CC NONE
#define STOP PORT2
#endif
#ifndef EXTRA_CC_5
#define POT_5_CC NONE
#define RESET
#endif
//NeoSWSerial softSerial(BLANK_SERIAL, CV_GATE_OUT, PIN_UNUSED);

/*
#elif (MODE == INTERNAL_ROUTER)
#define CONFIGURATION PORT_1_TO_PORT_2
#define OUTPUT_CHANNEL ALL
#define FILTER_STYLE 	NONE
#define CLOCK
#define STOP NONE
#define POT_4_CC NONE
#ifndef EXTRA_CC_5
#define POT_5_CC NONE
#define RESET
#endif
NeoSWSerial softSerial(CV_GATE_OUT, CV_AUDIO_IN, PIN_UNUSED);
*/

#elif (MODE == USB_DISTRIBUTOR)
#define CONFIGURATION USB_TO_PORT_1
#define OUTPUT_CHANNEL ALL
#define FILTER_STYLE 	DISTRIBUTE
#ifndef EXTRA_CC_4
#define POT_4_CC NONE
#define STOP PORT2
#endif
#ifndef EXTRA_CC_5
#define POT_5_CC NONE
#define RESET
#endif
NeoSWSerial softSerial(BLANK_SERIAL, CV_GATE_OUT, PIN_UNUSED);

#elif (MODE == USB_MPE)
#define CONFIGURATION USB_TO_PORT_1
#define OUTPUT_CHANNEL ALL
#define FILTER_STYLE 	USE_MPE
#ifndef EXTRA_CC_4
#define POT_4_CC NONE
#define STOP PORT2
#endif
#ifndef EXTRA_CC_5
#define POT_5_CC NONE
#define RESET
#endif
NeoSWSerial softSerial(BLANK_SERIAL, CV_GATE_OUT, PIN_UNUSED);

#elif (MODE == NOTE_GENERATOR)
#define CONFIGURATION NONE_TO_PORT_1
#define OUTPUT_CHANNEL NOTE_GENERATOR_CHANNEL
#define FILTER_STYLE 	NONE
#define POT_4_CC NONE
#define POT_5_CC NONE
NeoSWSerial softSerial(BLANK_SERIAL, CV_GATE_OUT, PIN_UNUSED);

#elif (MODE == USB_TRIGGERS)
#define CONFIGURATION USB_TO_NONE
#define OUTPUT_CHANNEL NONE
#define POT_1_CC NONE
#define POT_2_CC NONE
#define POT_3_CC NONE
#define POT_4_CC NONE
#define POT_5_CC NONE
#define FILTER_STYLE 	NONE

#elif (MODE == INTERNAL_TRIGGERS)
#define CONFIGURATION PORT_1_TO_NONE
#define OUTPUT_CHANNEL NONE
#define POT_1_CC NONE
#define POT_2_CC NONE
#define POT_3_CC NONE
#define POT_4_CC NONE
#define POT_5_CC NONE
#define FILTER_STYLE 	NONE
NeoSWSerial softSerial(CV_GATE_OUT, BLANK_SERIAL, PIN_UNUSED);

#elif (MODE == USB_HEX)
#define CONFIGURATION USB_TO_PORT_1
#define OUTPUT_CHANNEL ALL
#define POT_1_CC NONE
#define POT_2_CC NONE
#define POT_3_CC NONE
#define POT_4_CC NONE
#define POT_5_CC NONE
#define FILTER_STYLE 	NONE
NeoSWSerial softSerial(BLANK_SERIAL, CV_GATE_OUT, PIN_UNUSED);

#endif

// Error checking
#if (POT_1_CC > 128)
ERROR("POT_1_CC must be in the range 1...128 or NONE")
#endif
#if (POT_2_CC > 128)
ERROR("POT_2_CC must be in the range 1...128 or NONE")
#endif
#if (POT_3_CC > 128)
ERROR("POT_3_CC must be in the range 1...128 or NONE")
#endif
#if (POT_4_CC > 128)
ERROR("POT_4_CC must be in the range 1...128 or NONE")
#endif
#if (POT_5_CC > 128)
ERROR("POT_5_CC must be in the range 1...128 or NONE")
#endif
#if (POT_1_CC != NONE && POT_1_CC == POT_2_CC)
ERROR("POT_1_CC should not be the same as POT_2_CC")
#endif
#if (POT_2_CC != NONE && POT_2_CC == POT_3_CC)
ERROR("POT_2_CC should not be the same as POT_3_CC")
#endif
#if (POT_1_CC != NONE && POT_1_CC == POT_3_CC)
ERROR("POT_1_CC should not be the same as POT_3_CC")
#endif
#if (POT_4_CC != NONE && POT_4_CC == POT_1_CC)
ERROR("POT_4_CC should not be the same as POT_1_CC")
#endif
#if (POT_4_CC != NONE && POT_4_CC == POT_2_CC)
ERROR("POT_4_CC should not be the same as POT_2_CC")
#endif
#if (POT_4_CC != NONE && POT_4_CC == POT_3_CC)
ERROR("POT_4_CC should not be the same as POT_3_CC")
#endif
#if (POT_5_CC != NONE && POT_5_CC == POT_1_CC)
ERROR("POT_5_CC should not be the same as POT_1_CC")
#endif
#if (POT_5_CC != NONE && POT_5_CC == POT_2_CC)
ERROR("POT_5_CC should not be the same as POT_2_CC")
#endif
#if (POT_5_CC != NONE && POT_5_CC == POT_3_CC)
ERROR("POT_5_CC should not be the same as POT_3_CC")
#endif
#if (POT_5_CC != NONE && POT_5_CC == POT_4_CC)
ERROR("POT_5_CC should not be the same as POT_4_CC")
#endif










// Define the ports, except for HW Serial

/////// SERIAL PORT DEFINITIONS
/////// Serial ports are then setup in setup()

////// MIDI DEFINES
#define MIDI_RATE 31250
#define MIDI_NOTE_OFF 0x80
#define MIDI_STATUS_BYTE MIDI_NOTE_OFF		// everything below this is a data byte
#define MIDI_NOTE_ON 0x90
#define MIDI_POLY_AT 0xA0
#define MIDI_CC 0xB0
#define MIDI_PROGRAM_CHANGE 0xC0 
#define MIDI_CHANNEL_AT 0xD0 
#define MIDI_PITCH_BEND 0xE0
#define MIDI_SYSTEM_EXCLUSIVE 0xF0
#define MIDI_UNVOICED MIDI_SYSTEM_EXCLUSIVE		// everything below this is voiced
#define MIDI_TIME_CODE 0xF1
#define MIDI_SONG_POSITION_POINTER 0xF2 
#define MIDI_SONG_SELECT 0xF3
#define MIDI_UNDEFINED_F4 0xF4
#define MIDI_UNDEFINED_F5 0xF5
#define MIDI_TUNE_REQUEST 0xF6
#define MIDI_END_OF_SYSTEM_EXCLUSIVE 0xF7
#define MIDI_CLOCK 0xF8
#define MIDI_UNDEFINED_F9 0xF9
#define MIDI_START 0xFA
#define MIDI_CONTINUE 0xFB
#define MIDI_STOP 0xFC
#define MIDI_UNDEFINED_FD 0xFD
#define MIDI_ACTIVE_SENSING 0xFE
#define MIDI_SYSTEM_RESET 0xFF
#define MIDI_SYSTEM_REALTIME MIDI_CLOCK		// MIDI_CLOCK is the first one






void setup()
    {
#if (MODE == USB_TRIGGERS)
	pinMode(CV_AUDIO_IN, OUTPUT);
	pinMode(CV_IN3, OUTPUT);
	pinMode(CV_GATE_OUT, OUTPUT);
	pinMode(CV_AUDIO_OUT, OUTPUT);
	NeoSerial.begin(MIDI_RATE);
#elif (MODE == INTERNAL_TRIGGERS)
	pinMode(CV_AUDIO_IN, OUTPUT);
	pinMode(CV_IN3, OUTPUT);
	pinMode(CV_GATE_OUT, INPUT);
	pinMode(CV_AUDIO_OUT, OUTPUT);
	softSerial.begin(MIDI_RATE);
	NeoSerial.begin(MIDI_RATE);		// for debugging
#elif (MODE == NOTE_GENERATOR)
	pinMode(CV_AUDIO_IN, INPUT);
	pinMode(CV_IN3, INPUT);
	pinMode(CV_GATE_OUT, OUTPUT);
	softSerial.begin(MIDI_RATE);
	NeoSerial.begin(MIDI_RATE);
	initializePitch(CV_POT_IN1);
#else
	pinMode(CV_AUDIO_IN, OUTPUT);
	pinMode(CV_IN3, OUTPUT);
	pinMode(CV_AUDIO_OUT, OUTPUT);
	
	
#if (CONFIGURATION == PORT_1_TO_PORT_2)
	pinMode(CV_GATE_OUT, INPUT);
	pinMode(CV_AUDIO_IN, OUTPUT);
	softSerial.begin(MIDI_RATE);
#elif (CONFIGURATION == USB_TO_NONE || CONFIGURATION == USB_TO_PORT_1 || CONFIGURATION == USB_TO_BOTH)
	NeoSerial.begin(MIDI_RATE);
	pinMode(CV_GATE_OUT, OUTPUT);
#elif (DEBUG == 1)
	NeoSerial.begin(MIDI_RATE);
#endif

#if (CONFIGURATION == USB_TO_PORT_1 || CONFIGURATION == USB_TO_BOTH || CONFIGURATION == PORT_1_TO_PORT_2 || CONFIGURATION == NONE_TO_PORT_1)
	softSerial.begin(MIDI_RATE);
#endif

#if (CONFIGURATION == USB_TO_PORT_1 || CONFIGURATION == USB_TO_BOTH || CONFIGURATION == PORT_1_TO_PORT_2 || CONFIGURATION == USB_TO_USB)
		// set up timer
			FlexiTimer2::set(TIMER_SPEED, 1.0 / 1000, setInject);
			FlexiTimer2::start();
#endif
#endif
    }





/// FILTERING




/// parseMessage.  This facility parses the latest message and holds onto it.

uint8_t sustain;
uint8_t lastNoteOff = 0;

#define MAX_MESSAGE_LENGTH 32
#define INVALID_MESSAGE_LENGTH (MAX_MESSAGE_LENGTH + 1)
uint8_t systemRealTime = 0;
uint8_t message[MAX_MESSAGE_LENGTH];
uint8_t messageLength = 0;
uint8_t messageExpectedLength = MAX_MESSAGE_LENGTH;
#define MESSAGE_COMPLETE() (messageLength == 0 || (messageLength >= messageExpectedLength && messageLength > 0))
#define VOICE_MESSAGE(m) (m[0] >= MIDI_STATUS_BYTE && m[0] < MIDI_UNVOICED) 
#define CHANNEL(m) (m[0] & 0x0F)
#define CHANNEL_1_MESSAGE(m) (VOICE_MESSAGE(m) && CHANNEL(m) == 0)
#define PITCH(m) (m[1])
#define VELOCITY(m) (m[2])
#define NOTE_OFF(m) ((m[0] & 0xF0) == MIDI_NOTE_OFF || ((m[0] & 0xF0) == MIDI_NOTE_ON && VELOCITY(m) == 0))
#define NOTE_ON(m) ((m[0] & 0xF0) == MIDI_NOTE_ON)
#define POLY_AT(m) ((m[0] & 0xF0) == MIDI_POLY_AT)

// This gathers bytes and builds messages. As they are being built,
// most messages are put into message[].  A message is completed when
// MESSAGE_COMPLETE() is true, and you can see the message as
// message[0...messageLength].  The maximum message length is
// MAX_MESSAGE_LENGTH: sysex messages Note that sysex messages are *never* 
// complete.  The largest possible message is MAX_MESSAGE_LENGTH 
// long, after which further data bytes are ignored.  System real time
// messages, such as STOP or CLOCK, are stored in systemRealTime when they
// show up: it stores 0 otherwise.  This is because you can receive a
// system realtime message in the middle of another incomplete message.

// FIXME: We are assuming that all system exclusive messages end in F7

inline uint8_t parseMessage(uint8_t c)
	{
	systemRealTime = 0;
	if (c == MIDI_END_OF_SYSTEM_EXCLUSIVE)	// handled specially
		{
		if (messageLength > 0 && message[0] == MIDI_SYSTEM_EXCLUSIVE)
			{
			if (messageLength != INVALID_MESSAGE_LENGTH)		// we're good
				messageExpectedLength = messageLength;
			else
				messageLength = 0;								// dump it
			}
		}
	else if (c >= MIDI_STATUS_BYTE)
		{
		if ( c >= MIDI_SYSTEM_REALTIME)
			{
			systemRealTime = c;
			}
		else
			{
			messageLength = 1;
			message[0] = c;
			if (c < MIDI_PROGRAM_CHANGE)	// note off, note on, poly at, cc
				{
				messageExpectedLength = 3;
				}
			else if (c < MIDI_PITCH_BEND)		// PC, AT
				{
				messageExpectedLength = 1;
				}
			else if (c < MIDI_SYSTEM_EXCLUSIVE)		// Bend
				{
				messageExpectedLength = 3;
				}
			else if (c == MIDI_SYSTEM_EXCLUSIVE)		// Sysex
				{
				messageExpectedLength = MAX_MESSAGE_LENGTH + 2;		// Max for us
				}
			else if (c == MIDI_SONG_POSITION_POINTER)		// Song Position Pointer
				{
				messageExpectedLength = 3;
				}
			else if (c == MIDI_SONG_SELECT)		// Song Select
				{
				messageExpectedLength = 2;
				}
			else if (c < MIDI_SYSTEM_REALTIME)		// 0xF1 Midi Time Code Quarter Frame (ignored), tune request, undefined
				{
				messageExpectedLength = 1;
				}
			}
		}
	else if (messageLength < messageExpectedLength &&
			messageLength < MAX_MESSAGE_LENGTH)
		{
		message[messageLength++] = c;
		}
	else if (messageLength >= MAX_MESSAGE_LENGTH)
		{
		// we're still trying to grow, illegal
		messageLength = INVALID_MESSAGE_LENGTH;		// we never reach the max expected sysex length in this case
		}
	return (systemRealTime != 0 || messageLength == messageExpectedLength);
	}
	
	

/// emitWithDifferentChannel.  This changes the channel of a voice message and re-emits it.

void emitWithDifferentChannel(uint8_t* message, uint8_t len, uint8_t channel)
	{
	uint8_t c = message[0];
	// change channel
	uint8_t mess = (c & 0xF0);
	c = mess + channel;
	emit(c);
	// push rest of message
	for(uint8_t i = 1; i < len; i++)
		{
		emit(message[i]);
		}
	}



/// distribute.  This distributes notes to NUM_VOICES different channels if they are available

// For each note currently being played, what channel was it sent to?
uint8_t noteChannels[128];						// NONE means not used, 1..16 means channel, we don't allow 1
// For each channel, what note has been allocated to it?
uint8_t channelNotes[17];					// For channels 1...16, 0 ignored. NONE means available, 1...128 means already allocated to 0..127 note pitch

uint8_t lowestChannelAvailable()				// returns the lowest channel available
	{
	uint8_t min = 2;
	for(uint8_t i = 3; i <= NUM_VOICES + 1; i++)
		{
		if (channelNotes[i] < channelNotes[min]) 
			{
			min = i;
			}
		}
	return min;
	}
	
inline void distribute()
	{
	if (VOICE_MESSAGE(message))
		{
		if (NOTE_OFF(message))
			{
			uint8_t pitch = PITCH(message);
			// release the voice
			uint8_t channel = noteChannels[pitch];
			if (channel != NONE && channel != 1)
				{
				channelNotes[channel] = NONE;	// make available
				}
			noteChannels[pitch] = NONE;		// make available
			emitWithDifferentChannel(message, messageLength, channel - 1);
			}
		else if (NOTE_ON(message))
			{
			// find and allocate a voice -- this isn't quite right, we really need a NOTE OFF ring buffer too
			uint8_t pitch = PITCH(message);
			uint8_t channel = NONE;
			for(uint8_t i = 2; i <= NUM_VOICES + 1; i++)			// Note <=.  we skip channel 1
				{
				if (channelNotes[i] == NONE)	// available
					{
					channelNotes[i] = pitch + 1;
					noteChannels[pitch] = i;
					channel = i;
					break;
					}
				}
				
			if (channel == NONE)	// everything allocated, identify who to remove via the ring buffer
				{
				uint8_t die = lowestChannelAvailable();
				
				// send a NOTE OFF to oldest
				emit(MIDI_NOTE_OFF + die);
				emit(channelNotes[die] - 1);
				emit(0x40);

				// reallocate					
				channelNotes[die] = pitch;
				noteChannels[pitch] = die;
				channel = die;
				}
						
			// now we have voice allocated 
			emitWithDifferentChannel(message, messageLength, channel - 1);
			}
		else if (POLY_AT(message))		// do we really need this?
			{
			// What's the correct channel?
			uint8_t channel = noteChannels[PITCH(message)];
			if (channel != NONE) // uh...
				{
				emitWithDifferentChannel(message, messageLength, channel - 1);
				}
			}
		else	// AT, Pitch Bend, Program Change, CC
			{
			// Send to everybody
			for(uint8_t i = 0; i < NUM_VOICES; i++)
				{
				emitWithDifferentChannel(message, messageLength, i + 1);
				}
			}
		}
	else 
		{
		for(uint8_t i = 0; i < messageLength; i++)
			emit(message[i]);
		}
	}



// Routes messages to a different channel (if they are voice messages of course)

void addToChannel(uint8_t* message, uint8_t len, uint8_t channel)
	{
	if (len > 0)
		{
		if (VOICE_MESSAGE(message))
			{
			emitWithDifferentChannel(message, len, channel);
			}
		else
			{
			for(uint8_t i = 0; i < len; i++)
				{
				emit(message);
				}
			}
		}
	}
	
	
	
	
	
/// filterChannel.  Takes an incoming byte and processes it.  First, the byte is parsed.
/// Then depending on the task, either triggers are issued, or the byte is rerouted to
/// a new channel, or we distribute the message per-note.  Or just re-emit it.

void filterChannel(uint8_t c)
	{
	parseMessage(c);		// also needed by inject CC
#if (MODE == USB_TRIGGERS || MODE == INTERNAL_TRIGGERS)
	if (MESSAGE_COMPLETE() && !systemRealTime && CHANNEL_1_MESSAGE(message))
		{
		doTriggers();
		}
#elif (FILTER_STYLE == USE_MPE)
	if (MESSAGE_COMPLETE() && !systemRealTime && CHANNEL_1_MESSAGE(message))
		{
		// add to other channels
		for(uint8_t i = 0; i < NUM_VOICES; i++)
			addToChannel(message, messageLength, i + 1);
		}
	emit(c);
#elif (FILTER_STYLE == DISTRIBUTE)
	if (MESSAGE_COMPLETE() && (!VOICE_MESSAGE(message) || CHANNEL_1_MESSAGE(message)))
		{
		if (systemRealTime) emit(systemRealTime);
		else distribute();
		}
#else
	emit(c);
#endif
	}





/// writing to serial port.  This is broken out so we can insert a debug() in it when appropriate

inline void write(uint8_t c)
	{
//debug(c);
#if (CONFIGURATION != USB_TO_NONE && CONFIGURATION != USB_TO_USB)
 	softSerial.write(c);
#endif
#if (CONFIGURATION == USB_TO_USB)
	NeoSerial.write(c);
#endif
	}




// DeLegato injects a NOTE OFF in front of every NOTE ON to make sure we do a gate

#define NO_PITCH 255
uint8_t noteOnPitch[16] = 				// per-channel, what was the last note played?
	{ NO_PITCH, NO_PITCH, NO_PITCH, NO_PITCH, NO_PITCH, NO_PITCH, NO_PITCH, NO_PITCH, NO_PITCH, NO_PITCH, NO_PITCH, NO_PITCH, NO_PITCH, NO_PITCH, NO_PITCH, NO_PITCH, };
#define NO_CHANNEL 255
uint8_t lastNoteOnChannel = NO_CHANNEL;	// what was the last channel played?
inline void deLegato(uint8_t c)
	{
	if (c >= MIDI_NOTE_ON && c < MIDI_NOTE_ON + 16)
		{
		// Do we need to send a NOTE OFF?
		uint8_t channel = c - MIDI_NOTE_ON;
		if (DELEGATO_CHANNELS[channel] & noteOnPitch[channel] != NO_PITCH)
			{
			if (OUTPUT_CHANNEL == channel || OUTPUT_CHANNEL == ALL)
				{
				write(MIDI_NOTE_OFF + channel);
				write(noteOnPitch[channel]);
				write(0x40);					// Note off
				}
			noteOnPitch[channel] = NO_PITCH;
			}

		// What's the new note?  Prepare the channel for the next byte
		lastNoteOnChannel =  channel;
		}
	else if (lastNoteOnChannel < 16 && c < MIDI_NOTE_ON)				// It's not a status byte (remember, clock etc. can happen right now) 
		{
		// It's the next byte of NOTE ON, which holds the pitch so we can store it.
		noteOnPitch[lastNoteOnChannel] = c;
		lastNoteOnChannel = NO_CHANNEL;
		}
	}



// emitClock emits clock, start, and start/stop gate out various unused digital outs
#define MAX_COUNTDOWN 255
uint8_t clockCountdown;
uint8_t resetCountdown;
uint8_t stopCountdown;
uint8_t clockCounter = 0;
inline uint8_t emitClock(uint8_t c)
	{
	if (c == MIDI_CLOCK) 
		{
		if (clockCounter == 0)
			{
			clockCountdown = MAX_COUNTDOWN;
			digitalWrite(CV_AUDIO_OUT, 1);
			clockCounter = CLOCK_DIVISOR - 1;
			}
		else clockCounter--;
		}
	else if (c == MIDI_STOP) 
		{ 
		resetCountdown = 0; 
		stopCountdown = MAX_COUNTDOWN;
#if (GATE == PORT1)
		digitalWrite(CV_GATE_OUT, 0); 
#elif (GATE == PORT2)
		digitalWrite(CV_AUDIO_IN, 0); 
#endif
#if (STOP == PORT1)
		digitalWrite(CV_GATE_OUT, 1); 
#elif (STOP == PORT2)
		digitalWrite(CV_AUDIO_IN, 1); 
#endif
		}
	else if (c == MIDI_START) 
		{
		clockCounter = 0;
		stopCountdown = 0;
		resetCountdown = MAX_COUNTDOWN; 
#ifdef RESET
		digitalWrite(CV_IN3, 1); 			// Reset
#endif
#if (GATE == PORT1)
		digitalWrite(CV_GATE_OUT, 1); 
#elif (GATE == PORT2)
		digitalWrite(CV_AUDIO_IN, 1); 
#endif
#if (STOP == PORT1)
		digitalWrite(CV_GATE_OUT, 0); 
#elif (STOP == PORT2)
		digitalWrite(CV_AUDIO_IN, 0); 
#endif
		}
	else if (c == MIDI_CONTINUE) 
		{
		resetCountdown = 0;
		clockCounter = 0;
		stopCountdown = 0;				// No Reset
#if (GATE == PORT1)
		digitalWrite(CV_GATE_OUT, 1); 
#elif (GATE == PORT2)
		digitalWrite(CV_AUDIO_IN, 1); 
#endif
#if (STOP == PORT1)
		digitalWrite(CV_GATE_OUT, 0); 
#elif (STOP == PORT2)
		digitalWrite(CV_AUDIO_IN, 0); 
#endif
		}
	}



/// emitting.  This first does delegato when appropriate.  It then writes the byte if it's the right
/// channel.  This used to write to different serial ports, but that's too costly right now, so we've
/// eliminated that.  As a result this method is now a bit vestigial.

uint8_t emitChannel = ALL;
inline uint8_t emit(uint8_t c)
	{
#if (DELEGATO == 1)
	deLegato(c);
#endif

	if (c >= MIDI_STATUS_BYTE) 
		{
		if (c >= MIDI_SYSTEM_REALTIME)
			{
			emitChannel = ALL;
			}
		else
			{
			emitChannel = (c & 0x0F) + 1;
			}
		}

	if (OUTPUT_CHANNEL == emitChannel || OUTPUT_CHANNEL== ALL || (emitChannel == ALL && OUTPUT_CHANNEL != NONE))
		{
		write(c);
		}

#ifdef CLOCK
	emitClock(c);
#endif
	}
	

/// handling trigger outputs.  There are 3 or 4 trigger output notes depending on how many outputs we have
/// available.


#define MAX_TRIGGER_COUNTDOWN 512
uint8_t triggerPins[4] = { CV_AUDIO_OUT, CV_IN3, CV_AUDIO_IN, CV_GATE_OUT };
uint16_t triggerCountdowns[4] = { 0, 0, 0, 0 };
void doTriggers()
	{
#if (MODE == USB_TRIGGERS)
for(uint8_t i = 0; i < 4; i++)
#else
for(uint8_t i = 0; i < 3; i++)
#endif
		{
		if (NOTE_ON(message) && message[1] == triggerNotes[i])
			{
			digitalWrite(triggerPins[i], 1);
			triggerCountdowns[i] = MAX_TRIGGER_COUNTDOWN;
			break;
			}
		}
	}


/// DEBUGGING
/// WRITING TO A SERIAL OUTPUT PORT

inline void debugString(const char* string)
	{
#if (DEBUG == 1)
		NeoSerial.print("\t");
		NeoSerial.println(string);
#endif
	}

inline void debug(uint8_t c)
	{
#if (DEBUG == 1)
		NeoSerial.print("\t");
		NeoSerial.println(String(c, HEX));
#endif
	}
	
	
	
	
/// INJECTION.  The setInject() method grabs the latest pot values and tries to inject their CCS, but if the current MIDI 
/// message is not complete, it holds the injection until after the message is done and injects immediately afterwards.

uint8_t injectWaiting = false;
#define HIGH 500
#define LOW 100
uint8_t pitchIn = 60;
uint8_t gateIn = false;
uint8_t oldCC1Val = 255;
uint8_t oldCC2Val = 255;
uint8_t oldCC3Val = 255;
uint8_t oldCC4Val = 255;
uint8_t oldCC5Val = 255;
uint8_t cc1Val = 255;
uint8_t cc2Val = 255;
uint8_t cc3Val = 255;
uint8_t cc4Val = 255;
uint8_t cc5Val = 255;

// Called by the timer to set up injection every once in a while.  We read the pots, then set the CC values and try to inject.
// If we fail (because a message is incomplete), we'll try to inject repeatedly after the message is done until we are successful.
void setInject() 
	{ 
#if (MODE == NOTE_GENERATOR)
	 uint8_t c1 = analogRead(CV_IN3) >> 3;
	 cc1Val = (cc1Val == 255 ? c1 : (c1 + cc1Val * 3) >> 2);
	 uint8_t c2 = analogRead(CV_POT_IN2) >> 3;
	 cc2Val = (cc2Val == 255 ? c2 : (c2 + cc2Val * 3) >> 2);
	 uint8_t c3 = analogRead(CV_POT3) >> 3;
	 cc3Val = (cc3Val == 255 ? c3 : (c3 + cc3Val * 3) >> 2);
	 uint8_t g = digitalRead(CV_AUDIO_IN);		// Gate In
	 if (g && !gateIn) // Note on
	 	{
	 	pitchIn = getPitch(CV_POT_IN1);
		write(MIDI_NOTE_ON + OUTPUT_CHANNEL);
		write(pitchIn + 24);
		write(0x40);
		gateIn = true;
	 	}
	 else if (!g && gateIn)	// Note off
	 	{
	 	// use the previous pitchIn
		write(MIDI_NOTE_OFF + OUTPUT_CHANNEL);
		write(pitchIn + 24);
		write(0x40);
		gateIn = false;
	 	}
#else
	 uint8_t c1 = analogRead(CV_POT_IN1) >> 3;
	 cc1Val = (cc1Val == 255 ? c1 : (c1 + cc1Val * 3) >> 2);
	 uint8_t c2 = analogRead(CV_POT_IN2) >> 3;
	 cc2Val = (cc2Val == 255 ? c2 : (c2 + cc2Val * 3) >> 2);
	 uint8_t c3 = analogRead(CV_POT3) >> 3;
	 cc3Val = (cc3Val == 255 ? c3 : (c3 + cc3Val * 3) >> 2);
#if POT_4_CC != NONE
	 uint8_t c4 = analogRead(CV_AUDIO_IN) >> 3;
	 cc4Val = (cc4Val == 255 ? c4 : (c4 + cc4Val * 3) >> 2);
#endif
#if POT_5_CC != NONE
	 uint8_t c5 = analogRead(CV_IN3) >> 3;
	 cc5Val = (cc5Val == 255 ? c5 : (c5 + cc5Val * 3) >> 2);
#endif

#endif
	 injectWaiting = true;
	 // try now
	 inject();
	}


// Injects a single CC message into the stream.
void injectCC(uint8_t param, uint8_t value)
	{
			write(MIDI_CC); 
			write(param); 
			write(value);

	for(uint8_t i = 0; i <= NUM_VOICES; i++)
		{
		if (INJECT_POTS_TO_CHANNEL[i])
			{
			write(MIDI_CC + i); 
			write(param); 
			write(value);
			}
		}
	}

// Injects up to three CC messages into the stream, only when a message is completely written.
void inject()
	{
	if (injectWaiting && MESSAGE_COMPLETE())
		{
#if (MODE == NOTE_GENERATOR)
 		if (cc1Val >= 8 && oldCC1Val != cc1Val) injectCC(POT_1_CC, oldCC1Val = cc1Val);		// don't inject a low value, probably disconnected
#else
 		if (oldCC1Val != cc1Val) injectCC(POT_1_CC, oldCC1Val = cc1Val);
#endif
		if (oldCC2Val != cc2Val) injectCC(POT_2_CC, oldCC2Val = cc2Val); 
		if (oldCC3Val != cc3Val) injectCC(POT_3_CC, oldCC3Val = cc3Val);
#if POT_4_CC != NONE
		if (oldCC4Val != cc4Val) injectCC(POT_4_CC, oldCC4Val = cc4Val); 
#endif
#if POT_5_CC != NONE
		if (oldCC5Val != cc5Val) injectCC(POT_5_CC, oldCC5Val = cc5Val); 
#endif
		injectWaiting = false;
		}
	}


// Hex digit parsing
uint8_t hexFirstDigit = 128;		// 128 means "no first digit yet"
void processHex(uint8_t val)		// val is a number 0...15, or if > 15, resets
	{
	if (val >= 16) 					// reset value
		{
		hexFirstDigit = 128;
		}
	else if (hexFirstDigit == 128)	// first digit
		{
		hexFirstDigit = val;
		}
	else							// process with second digit and reset
		{
		write(hexFirstDigit * 16 + val);
		hexFirstDigit = 128;
		}
	}


// Handle all manner of inputs and do the right thing with them.  This is called by either
// loop() or by the interrupt
void handleRead(uint8_t c)
{
#if (MODE == USB_HEX)
	if (c >= 'a' && c <= 'f') c = c - 'a' + 10;
	else if (c >= 'A' && c <= 'F') c = c - 'A' + 10;
	else if (c >= '0' && c <= '9') c = c - '0';
	else c = 16;
	processHex(c);
#else	
	filterChannel(c);
#if (MODE != USB_TRIGGERS && MODE != INTERNAL_TRIGGERS && MODE != USB_HEX)
	inject();
#endif
#endif
}



void loop()
    {
#if (MODE == NOTE_GENERATOR)
	setInject();
	// do nothing
#elif (MODE == USB_TRIGGERS)
	for(uint8_t i = 0; i < 4; i++)
		{
		if (triggerCountdowns[i] >= 1)
			{
			triggerCountdowns[i]--;
			if (triggerCountdowns[i] == 0)
				{
	  			digitalWrite(triggerPins[i], 0);
				}
			}
		}
#elif (MODE == INTERNAL_TRIGGERS)
	for(uint8_t i = 0; i < 3; i++)			// 3 vs 4, the only difference
		{
		if (triggerCountdowns[i] >= 1)
			{
			triggerCountdowns[i]--;
			if (triggerCountdowns[i] == 0)
				{
	  			digitalWrite(triggerPins[i], 0);
				}
			}
		}
#else

 	if (clockCountdown >= 1) 
 		{ 
 		clockCountdown--; 
 		if (clockCountdown == 0) 
 			{
 			digitalWrite(CV_AUDIO_OUT, 0);
			}
 		}
 		

 	if (stopCountdown >= 1) 
 		{ 
 		stopCountdown--; 
 		if (stopCountdown == 0) 
 			{
			#if (STOP == PORT1)
				digitalWrite(CV_GATE_OUT, 0); 
			#elif (STOP == PORT2)
				digitalWrite(CV_AUDIO_IN, 0); 
			#endif
 			}
 		}

 	if (resetCountdown >= 1) 
 		{ 
 		resetCountdown--; 
 		if (resetCountdown == 0) 
 			{
			digitalWrite(CV_IN3, 0); 
 			}
 		}

#endif
    
    #if (MODE == INTERNAL_TRIGGERS) 	//  || MODE == INTERNAL_ROUTER)
    	uint8_t val = softSerial.available();
		for(uint8_t i = 0; i < val; i++)
			{
			handleRead(softSerial.read());
			}
	#else
    	uint8_t val = NeoSerial.available();
		for(uint8_t i = 0; i < val; i++)
			{
			uint8_t g = NeoSerial.read();
			handleRead(g);
			}
	#endif
    }
    
    
    
    
    
    
    
///// BORROWED FROM QUANT


#define MEDIAN_OF_THREE(a,b,c) (((a) <= (b)) ? (((b) <= (c)) ? (b) : (((a) < (c)) ? (c) : (a))) : (((a) <= (c)) ? (a) : (((b) < (c)) ? (c) : (b))))
uint16_t pitchCV;
uint16_t pA;
uint16_t pB;

// This is a modified version of my standard initializeFrequency(...) function
void initializePitch(uint8_t pitch)
    {
    pitchCV = analogRead(pitch);
    }

#define LARGE_JUMP 32
#define SEMITONE 17
#define FREQ_COUNTER_MAX 4
uint8_t freqCounter = 0;
// This is a modified version of my standard getFrequency(...) function
// which returns a PITCH from 0...59 or so.
inline uint8_t getPitch(uint8_t pitch)
    {
    uint16_t p = analogRead(pitch);

    uint16_t diff = (p > pitchCV ? p - pitchCV : pitchCV - p);
    if (diff >= LARGE_JUMP)
        {
        pitchCV = p;            // jump right there
        freqCounter = FREQ_COUNTER_MAX;
        }
    else if (freqCounter > 0)
        {
        freqCounter--;
        pitchCV = (pitchCV + p) >> 1;
        pB = pA;
        pA = pitchCV;
        }
    else
        {
        uint16_t p1 = MEDIAN_OF_THREE(p, pA, pB);
        pB = pA;
        pA = p;
        pitchCV = (pitchCV * 7 + p1) >> 3;
        }
    return (uint8_t) ((pitchCV * 60) >> 10);		// now should range 0...60 or maybe 0...59
    }


uint16_t pitchCV2;
uint16_t pA2;
uint16_t pB2;

// This is a modified version of my standard initializeFrequency(...) function
void initializePitch2(uint8_t pitch)
    {
    pitchCV2 = analogRead(pitch);
    }
        
uint8_t freqCounter2 = 0;
// This is a modified version of my standard getFrequency(...) function
// which returns a PITCH from 0...59 or so.
inline uint8_t getPitch2(uint8_t pitch)
    {
    uint16_t p = analogRead(pitch);

    uint16_t diff = (p > pitchCV2 ? p - pitchCV2 : pitchCV2 - p);
    if (diff >= LARGE_JUMP)
        {
        pitchCV2 = p;            // jump right there
        freqCounter2 = FREQ_COUNTER_MAX;
        }
    else if (freqCounter2 > 0)
        {
        freqCounter2--;
        pitchCV2 = (pitchCV2 + p) >> 1;
        pB2 = pA2;
        pA2 = pitchCV2;
        }
    else
        {
        uint16_t p1 = MEDIAN_OF_THREE(p, pA2, pB2);
        pB2 = pA2;
        pA2 = p;
        pitchCV2 = (pitchCV2 * 7 + p1) >> 3;
        }
    return (uint8_t) ((pitchCV2 * 60) >> 10);		// now should range 0...60 or maybe 0...59
    }
