// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License

/// DAVE
///
/// Dave is a MIDI router and utility which can do a variety of things.  Dave is meant to run 
/// on the AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// DAVE REQUIRES that you install the NeoHWSerial library from the Arduino library manager.  It has
/// its own customized (and bug-fixed) copy of the NeoSWSerial library, so do not install
/// that one (and in fact you may need to uninstall it).
///
/// Dave runs in a variety of modes.  Many of these modes are meant to receive MIDI
/// over USB and route it to WonkyStuff modules or to the MASTER I/O.  The GRAINS USB 
/// port cannot present itself as a MIDI Device, so Dave works with a Java program, 
/// also called Dave, which receives MIDI from your DAW or controller and sends it over 
/// the USB port to the GRAINS.  This program is located in the "java" subdirectory.
///
/// You may have noticed that neither the MB/1 nor MASTER I/O have a WonkyStuff-style MIDI In port.
/// They just have MIDI TRS-B jacks.  However it is possible to send MIDI to them from the GRAINS
/// with some wiring.   See the file "docs/TRS.md" in the dave directory for more information.
///
/// DAVE works in one or more MODES and has a few configuration options beyond that.
///
///
///
/// THE MODES
///
/// Here are the basic modes:
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
///
///
/// There are a few other modes which have not been tested yet or are not working properly due
/// to speed problems with GRAINS's serial ports:
///
/// - USB DISTRIBUTOR BREAKOUT MODE.  This is like USB Distributor Mode but routes Channel 2
///   out DIGITAL OUT, routes Channel 3 out AUDIO IN, and Routes Channel 4 out IN 3.
///   Unfortunately IN 3 is not reliable as a MIDI transmitter, and two output serial ports
///   (DIGITAL OUT, AUDIO IN) are too slow, sometimes resulting in stuck notes.
///
/// - USB MPE BREAKOUT MODE.  This is like USB MPE Mode but routes Channel 2
///   out DIGITAL OUT, routes Channel 3 out AUDIO IN, and Routes Channel 4 out IN 3.
///   Unfortunately IN 3 is not reliable as a MIDI transmitter, and two output serial ports
///   (DIGITAL OUT, AUDIO IN) are too slow, sometimes resulting in stuck notes.
///
/// - INTERNAL CLOCK MODE.  This is similar to USB Clock Mode but takes MIDI Input from DIGITAL IN
///   rather than USB.  It's not been tested yet.
///
///
/// To set the MODE, you change the #define MODE below

// don't touch these, they're constants
#define USB_CLOCK 1								// Inadequately tested
#define USB_ROUTER 2
#define USB_DISTRIBUTOR 3
#define USB_MPE 4
#define USB_DISTRIBUTOR_BREAKOUT 5			 	// I'd not do this
#define USB_MPE_BREAKOUT 6						// I'd not do this
#define INTERNAL_CLOCK 7						// Untested

/// SET THE MODE HERE
#define MODE	 USB_DISTRIBUTOR				// Change this to one of the MODES above

/// SET THE NUMBER OF VOICES (for MPE and DISTRIBUTOR MODES) HERE
#define NUM_VOICES 3			// Can be 0 or 1 [Both Off], or any value 2-16 



/// CUSTOMIZING THE POT CC OUTPUTS
///
/// In modes other than the CLOCK modes, you can change up to three CC parameters with the pots.
/// This is not available in the USB_CLOCK or INTERNAL_CLOCK modes.
///
/// WARNING: if you play or release notes immediately after or during changing the pots, DAVE
/// may not see these notes, possibly resulting in stuck or oddly behaving note states.  This is
/// because a stream of pots overwhelms the MIDI output, and so DAVE may not be able to return to
/// the incoming notes in time.  So don't do that.  Play/release notes, or change the pots, but not
/// both at the same time.
///
/// The CC PARAMETER for each Pot is defined with the following #defines:

#define POT_1_CC	5		// Wonkystuff MCO/1 Sub.  Can be any value 1...128 OR NONE
#define POT_2_CC	6		// Wonkystuff MCO/1 Ramp.  Can be any value 1...128 OR NONE
#define POT_3_CC	7		// Wonkystuff MCO/1 Square.  Can be any value 1...128 OR NONE

/// You can customize which channels will receive CCs from the pots by
/// modifying the "1" values below to "0":

const boolean INJECT_POTS_TO_CHANNEL[16] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };


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


/// CLOCK DIVISION
///
/// You can change the MIDI clock divisor.  By default the divisor is 1, meaning 24 PPQ.
/// If you made this 6, for example, you'd get 4PPQ, otherwise known as one pulse every 16th note.
/// If you changed this to 24, you'd get one pulse every quarter note.  The FIRST pulse is at the
/// very first clock pulse after a START or a CONTINUE.  Change the value below.

#define CLOCK_DIVISOR 1					// This must be >= 1.  6 will pulse 16th notes.  24 will pulse quarter notes.









/// (Don't play with these, they're constants)
#define NONE 0
#define ALL 17
#define USE_MPE 1
#define DISTRIBUTE 2

#define USB_TO_NONE 0
#define USB_TO_PORT_1 1
#define USB_TO_BOTH 2
#define PORT_1_TO_PORT_2 3
#define PORT_1_TO_NONE 4
#define CONFIGURATION	USB_TO_PORT_1

#define OUTPUT_1_CHANNEL NONE	// can be NONE, 1...16, or ALL
#define OUTPUT_2_CHANNEL NONE	// can be NONE, 1...16, or ALL
// How Input is filterChanneled to Output
#define FILTER_CHANNEL_1 	NONE		// can be NONE, USE_MPE, or DISTRIBUTE
// Are we debugging to the USB port?
#define DEBUG 1
// How many times a second do we update the pots?  If this is too fast, it will overwhelm MIDI
// and create a lot of lag, but we'd like it to be fast enough to sound realistic. Higher values
// are SLOWER.  You can't go much faster than 256.
#define TIMER_SPEED 128








#include "NeoSWSerial.h"   // you have to install this via the library manager
#include <NeoHWSerial.h>    // you have to install this via the library manager
//#include <AltSoftSerial.h>  // you have to install this via the library manager
#include <FlexiTimer2.h>    // you have to install this via the library manager, but I include a zip just in case

////////// PINOUTS

#define CV_POT_IN1    A2    // CC 1 Value
#define CV_POT_IN2    A1    // CC 2 Value
#define CV_POT3       A0    // CC 3 Value
#define CV_IN3        A3    // Reset
#define CV_AUDIO_IN   A4    // PORT 2
#define CV_AUDIO_OUT  11    // MIDI Clock (only when between start/continue and stop)
#define CV_GATE_OUT   8     // PORT 1
#define BLANK_RX	  5		// Blank Serial Pin




// MODE DEFINITIONS
#define PIN_UNUSED 255

#if (MODE == USB_CLOCK)
#define CONFIGURATION USB_TO_NONE
#define OUTPUT_1_CHANNEL NONE
#define OUTPUT_2_CHANNEL NONE
#define POT_1_CC NONE
#define POT_2_CC NONE
#define POT_3_CC NONE
#define FILTER_CHANNEL_1 	NONE

#elif (MODE == USB_ROUTER)
#define CONFIGURATION USB_TO_PORT_1
#define OUTPUT_1_CHANNEL ALL
#define OUTPUT_2_CHANNEL NONE
#define FILTER_CHANNEL_1 	NONE
NeoSWSerial softSerial(BLANK_RX, CV_GATE_OUT, PIN_UNUSED);

#elif (MODE == USB_DISTRIBUTOR)
#define CONFIGURATION USB_TO_PORT_1
#define OUTPUT_1_CHANNEL ALL
#define OUTPUT_2_CHANNEL NONE
#define FILTER_CHANNEL_1 	DISTRIBUTE
NeoSWSerial softSerial(BLANK_RX, CV_GATE_OUT, PIN_UNUSED);

#elif (MODE == USB_MPE)
#define CONFIGURATION USB_TO_PORT_1
#define OUTPUT_1_CHANNEL ALL
#define OUTPUT_2_CHANNEL NONE
#define FILTER_CHANNEL_1 	USE_MPE
NeoSWSerial softSerial(BLANK_RX, CV_GATE_OUT, PIN_UNUSED);

#elif (MODE == USB_DISTRIBUTOR_BREAKOUT)
#define CONFIGURATION USB_TO_BOTH
#define OUTPUT_1_CHANNEL 2
#define OUTPUT_2_CHANNEL 3
#define FILTER_CHANNEL_1 	DISTRIBUTE
#define NUM_VOICES 2
NeoSWSerial softSerial(BLANK_RX, CV_GATE_OUT, CV_AUDIO_IN);

#elif (MODE == USB_MPE_BREAKOUT)
#define CONFIGURATION USB_TO_BOTH
#define OUTPUT_1_CHANNEL 2
#define OUTPUT_2_CHANNEL 3
#define FILTER_CHANNEL_1 	USE_MPE
#define NUM_VOICES 2
NeoSWSerial softSerial(BLANK_RX, CV_GATE_OUT, CV_AUDIO_IN);

#elif (MODE == INTERNAL_CLOCK)
#define CONFIGURATION PORT_1_TO_NONE
#define POT_1_CC NONE
#define POT_2_CC NONE
#define POT_3_CC NONE
#define FILTER_CHANNEL_1 	NONE
NeoSWSerial softSerial(CV_GATE_OUT, CV_AUDIO_IN, PIN_UNUSED);
#endif



// Error checking
/*
#define ERROR(val) static_assert(false, val);
#if (INPUT == NONE && OUTPUT1 == NONE && OUTPUT2 == NONE)
ERROR("All Inputs and Outputs are set to NONE, which won't be very interesting.")
#endif
#if (OUTPUT1 == OUTPUT2 && OUTPUT1 != NONE)
ERROR("OUTPUT 1 and OUTPUT 2 are set to the same value, which is not permitted.")
#endif
#if (INPUT != NONE && INPUT != USB && INPUT != PORT1)
ERROR("INPUT must be NONE, USB, or PORT1")
#endif
#if (INPUT != NONE && INPUT == OUTPUT1)
ERROR("INPUT cannot be the same as OUTPUT 1")
#endif
#if (INPUT != NONE && INPUT == OUTPUT2)
ERROR("INPUT cannot be the same as OUTPUT 2")
#endif
#if (INPUT != NONE && INPUT == GATE)
ERROR("INPUT cannot be the same as GATE")
#endif
#if (INPUT != NONE && INPUT == STOP)
ERROR("INPUT cannot be the same as STOP")
#endif
#if (OUTPUT1 != NONE && OUTPUT1 != PORT1 && OUTPUT1 != PORT2)
ERROR("OUTPUT1 must be NONE, PORT1, PORT2")
#endif
#if (OUTPUT2 != NONE && OUTPUT2 != PORT1 && OUTPUT2 != PORT2)
ERROR("OUTPUT2 must be NONE, PORT1, PORT2")
#endif
#if (GATE != NONE && GATE != PORT1 && GATE != PORT2)
ERROR("GATE must be NONE, PORT1, PORT2")
#endif
#if (STOP != NONE && STOP != PORT1 && STOP != PORT2)
ERROR("STOP must be NONE, PORT1")
#endif
#if (RESET != NONE && RESET == GATE)
ERROR("RESET may not be the same as GATE")
#endif
#if (RESET != NONE && RESET == STOP)
ERROR("RESET may not be the same as STOP")
#endif
#if (STOP != NONE && STOP == GATE)
ERROR("STOP may not be the same as GATE")
#endif
#if (RESET != NONE && RESET == OUTPUT1)
ERROR("RESET may not be the same as OUTPUT1")
#endif
#if (RESET != NONE && RESET == OUTPUT2)
ERROR("RESET may not be the same as OUTPUT2")
#endif
#if (STOP != NONE && STOP == OUTPUT1)
ERROR("STOP may not be the same as OUTPUT1")
#endif
#if (STOP != NONE && STOP == OUTPUT2)
ERROR("STOP may not be the same as OUTPUT2")
#endif
#if (GATE != NONE && GATE == OUTPUT1)
ERROR("GATE may not be the same as OUTPUT1")
#endif
#if (GATE != NONE && GATE == OUTPUT2)
ERROR("GATE may not be the same as OUTPUT2")
#endif
*/
#if (POT_1_CC > 128)
ERROR("POT_1_CC must be in the range 1...128 or NONE")
#endif
#if (POT_2_CC > 128)
ERROR("POT_2_CC must be in the range 1...128 or NONE")
#endif
#if (POT_3_CC > 128)
ERROR("POT_3_CC must be in the range 1...128 or NONE")
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










// Define the ports, except for HW Serial

/////// SERIAL PORT DEFINITIONS
/////// Serial ports are then setup in setup()

const uint8_t outputPins[2] = { CV_GATE_OUT, CV_AUDIO_IN };
const uint8_t OUTPUT_CHANNELS[2] = { OUTPUT_1_CHANNEL, OUTPUT_2_CHANNEL };	// For each of the 2 outputs, either NONE, 1...16, or ALL

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
	pinMode(CV_AUDIO_IN, OUTPUT);
	pinMode(CV_IN3, OUTPUT);
	pinMode(CV_AUDIO_OUT, OUTPUT);


#if (CONFIGURATION == USB_TO_NONE || CONFIGURATION == USB_TO_PORT_1 || CONFIGURATION == USB_TO_BOTH)
	NeoSerial.begin(MIDI_RATE);
	pinMode(CV_GATE_OUT, OUTPUT);
#endif

	softSerial.begin(MIDI_RATE);

#if (CONFIGURATION == USB_TO_PORT_1 || CONFIGURATION == USB_TO_BOTH || CONFIGURATION == PORT_1_TO_PORT_2 )
		// set up timer
			FlexiTimer2::set(1, 1.0 / TIMER_SPEED, setInject);
			FlexiTimer2::start();
#endif
    }





/// FILTERING




/// parseMessage.  This facility parses the latest message and holds onto it.

#define MAX_MESSAGE_LENGTH 32
#define INVALID_MESSAGE_LENGTH (MAX_MESSAGE_LENGTH + 1)
uint8_t systemRealTime = 0;
uint8_t message[MAX_MESSAGE_LENGTH];
uint8_t messageLength = 0;
uint8_t messageExpectedLength = MAX_MESSAGE_LENGTH;
#define MESSAGE_COMPLETE() (messageLength >= messageExpectedLength && messageLength > 0)
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
		else			// all system real-time
			{
			systemRealTime = c;
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


uint8_t noteChannels[128];						// NONE means not used, 1..16 means channel, we don't allow 1
uint8_t channelAvailable[17];					// For channels 1...16, 0 ignored. NONE means available, 1...128 means already allocated to 0..127 note pitch

uint8_t lowest()
	{
	uint8_t min = 2;
	for(uint8_t i = 3; i <= NUM_VOICES + 1; i++)
		{
		if (channelAvailable[i] < channelAvailable[min]) 
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
				channelAvailable[channel] = NONE;	// make available
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
				if (channelAvailable[i] == NONE)	// available
					{
					channelAvailable[i] = pitch + 1;
					noteChannels[pitch] = i;
					channel = i;
					break;
					}
				}
				
			if (channel == NONE)	// everything allocated, identify who to remove via the ring buffer
				{
				uint8_t die = lowest();
				
				// send a NOTE OFF to oldest
				emit(MIDI_NOTE_OFF + die);
				emit(channelAvailable[die] - 1);
				emit(0x40);

				// reallocate					
				channelAvailable[die] = pitch;
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

void filterChannel(uint8_t c)
	{
	parseMessage(c);		// also needed by inject CC
#if (FILTER_CHANNEL_1 == USE_MPE)
	if (MESSAGE_COMPLETE() && !systemRealTime && CHANNEL_1_MESSAGE(message))
		{
		// add to other channels
		for(uint8_t i = 0; i < NUM_VOICES; i++)
			addToChannel(message, messageLength, i + 1);
		}
	emit(c);
#elif (FILTER_CHANNEL_1 == DISTRIBUTE)
	if (MESSAGE_COMPLETE() && (!VOICE_MESSAGE(message) || CHANNEL_1_MESSAGE(message)))
		{
		if (systemRealTime) emit(systemRealTime);
		else distribute();
		}
#else
	emit(c);
#endif
	}





/// emiting


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
			for(uint8_t i = 0; i < 2; i++)
				{
				if (OUTPUT_CHANNELS[i] == channel || OUTPUT_CHANNELS[i] == ALL)
					{
//  					softSerial.changeTxPin(outputPins[i]);
					softSerial.write(MIDI_NOTE_OFF + channel);
					softSerial.write(noteOnPitch[channel]);
					softSerial.write(0x40);					// Note off
					}
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
uint8_t clockCounter;
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
		digitalWrite(CV_IN3, 1); 			// Reset
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

	for(uint8_t i = 0; i < 2; i++)
		{
		if (OUTPUT_CHANNELS[i] == emitChannel || OUTPUT_CHANNELS[i] == ALL || (emitChannel == ALL && OUTPUT_CHANNELS != NONE))
			{
//  			softSerial.changeTxPin(outputPins[i]);
			softSerial.write(c);
			}
		}

#if (GATE != NONE && RESET != NONE && STOP != NONE)
	emitClock(c);
#endif
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
	
uint8_t injectWaiting = false;
uint8_t oldCC1Val = 255;
uint8_t oldCC2Val = 255;
uint8_t oldCC3Val = 255;
uint8_t cc1Val = 255;
uint8_t cc2Val = 255;
uint8_t cc3Val = 255;

// Called by the timer to set up injection every once in a while.  We read the pots, then set the CC values and try to inject.
// If we fail (because a message is incomplete), we'll try to inject repeatedly after the message is done until we are successful.
void setInject() 
	{ 
	 uint8_t c1 = analogRead(CV_POT_IN1) >> 3;
	 cc1Val = (cc1Val == 255 ? c1 : (c1 + cc1Val * 3) >> 2);
	 uint8_t c2 = analogRead(CV_POT_IN2) >> 3;
	 cc2Val = (cc2Val == 255 ? c2 : (c2 + cc2Val * 3) >> 2);
	 uint8_t c3 = analogRead(CV_POT3) >> 3;
	 cc3Val = (cc3Val == 255 ? c3 : (c3 + cc3Val * 3) >> 2);
	 injectWaiting = true;
	 // try now
	 inject();
	}

// Injects CC messages into the stream, only when a message is completely written.
void injectCC(uint8_t param, uint8_t value)
	{
	for(uint8_t i = 0; i <= NUM_VOICES; i++)
		{
		if (INJECT_POTS_TO_CHANNEL[i])
			{
// 			softSerial.changeTxPin(outputPins[i]);
			softSerial.write(MIDI_CC + i); 
			softSerial.write(param); 
			softSerial.write(value);
			}
		}
	}

// Injects CC messages into the stream, only when a message is completely written.
void inject()
	{	
	if (injectWaiting && MESSAGE_COMPLETE())
		{
		if (oldCC1Val != cc1Val) injectCC(POT_1_CC, oldCC1Val = cc1Val);
		if (oldCC2Val != cc2Val) injectCC(POT_2_CC, oldCC2Val = cc2Val); 
		if (oldCC3Val != cc3Val) injectCC(POT_3_CC, oldCC3Val = cc3Val);
		injectWaiting = false;
		}
	}



// Handle all manner of inputs and do the right thing with them.  This is called by either
// loop() or by the interrupt
void handleRead(uint8_t c)
{
	filterChannel(c);
	inject();
}



void loop()
    {
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
    
    while(true)
    	{
    #if (CONFIGURATION == INTERNAL_CLOCK)
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
    }
