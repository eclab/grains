// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License

/// DAVE
///
/// Dave is a MIDI router and utility which can do many things.  Dave is meant to run 
/// on the AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// Dave runs in a variety of modes.  Many of these modes are meant to receive MIDI
/// over USB.  The GRAINS USB port cannot present itself as a MIDI Device, so Dave
/// works with a Java program, also called Dave, which receives MIDI from your DAW
/// or controller and sends it over the USB port to the GRAINS.  This program is
/// located in the "java" subdirectory.
///
/// DAVE has a lot of configuration options.  However it also has six basic MODES, and
/// you can just choose a mode and most of the configurations will be automatically
/// set for you to make things simple.
///
/// Let's start with the basics:
///
/// 1. DAVE has FOUR MIDI ports, called USB, PORT1, PORT2, and PORT3
///
///    USB
///    PORT1	(DIGITAL OUT)
///    PORT2	(IN 3)
///    PORT3	(AUDIO IN)
///
/// These are in fact the ONLY ports that GRAINS can send or receive MIDI on!  
///
/// 2. DAVE receives MIDI either over USB or via PORT1.  It's impossible to receive
///    MIDI over PORT2 or PORT3.
///
/// 3. DAVE sends MIDI on any combination of PORT1, PORT2, and PORT3.  You can set
///    the channel that each of these ports listens in on.
///
/// 4. DAVE always sends CLOCK signals on AUDIO OUT.  It only sends them between
///    a MIDI START / CONTINUE and a MIDI STOP.
///
/// 5. Depending on how it's set up, DAVE can also send RESET (START), STOP,
///    or START/STOP GATE signals on PORT1, PORT2, and/or PORT3.
///
/// 6. You can set each of the three pots to output a CC.  You can choose the CC
///    parameter and the CC channel, and when you change a pot, it will inject a CC
///    into the stream for PORT1, PORT2, and/or PORT3.  This allows you to change
///    the MIDI parameters of the WonkyStuff MCO/1 for example.
///
/// 7. DAVE is capable of various filtering facilities:
/// 
///    - DAVE can do MPE (Low Zone).  It will treat Channel 1 as the Master Channel,
///      and any data received on that channel will be copied to the other channels
///      in the MPE Zone.
///
///    - Dave can distribute notes from Channel 1 to other channels.  If you had 4
///      other channels, Dave could redistribute notes from channel 1 to them round-robin,
///      which would give you a 4-voice polysynth on Channel 1.
///
///    - Dave can "De-Legato" notes: each time it receives a NOTE ON, it first sends
///      a NOTE OFF to eliminate the previous note before sending the NOTE ON.  This
///      counters an issue in the MCO/1, which only plays legato.
///
/// 8. DAVE can send Debug messages back to its Java program via USB
///
///
///
/// MODES
/// Here are the basic modes:
///
/// USB CLOCK MODE
/// This mode takes MIDI input over USB and outputs clock signals. Specifically:
/// - AUDIO OUT outputs gated clock as usual
/// - DIGITAL OUT (PORT 1) outputs a GATE: it goes high when receiving a START or CONTINUE,
///   and goes low when receiving a STOP
/// - IN 3 (PORT 2) outputs a trigger (RESET) when it receives START or CONTINUE
/// - AUDIO IN (PORT 3) outputs a trigger when it receives a STOP
/// - The Pots should NOT be set to output anything
///
///
/// USB ROUTER MODE
/// This mode takes MIDI input over USB and outputs directly to PORT 1. This can be useful
/// to send MIDI to the Wonkystuff MB/1, or as a simple way to send MIDI data directly to
/// a Wonkystuff MCO/1, MCC/4, MTR/8, or MDIV.  You could also use this to route MIDI data to
/// the MASTER I/O.  Additionally:
/// - AUDIO OUT outputs gated clock as usual
/// - IN 3 (PORT 2) outputs a trigger (RESET) when it receives START or CONTINUE
/// - AUDIO IN (PORT 3) outputs a trigger when it receives a STOP
/// - The Pots *can* be set to output CCs of your choice.
///
///
/// USB DISTRIBUTOR MODE
/// This mode takes MIDI input over USB and redistributes Channel 1 to several other channels.
/// It only sends MIDI out PORT 1.
/// It's particularly useful to use in combination with a WonkyStuff MB/1, which would then
/// break out those channels to make a polysynth. Additionally:
/// - AUDIO OUT outputs gated clock as usual
/// - IN 3 (PORT 2) outputs a trigger (RESET) when it receives START or CONTINUE
/// - AUDIO IN (PORT 3) outputs a trigger when it receives a STOP
/// - The Pots *can* be set to output CCs of your choice.
/// - If there aren't any available channels for voices, voice stealing just steals channel 2
/// 
/// To use USB DISTRIBUTOR MODE you have to set highest CHANNEL NUMBER.  Let's say that number
/// is N.  Then DAVE will redistribute MIDI on channel 1 to channels 2 ... N inclusive.  Thus 
/// if you wanted to use the MB/1 to set up a 4-voice polysynth, you'd set N = 5, and DAVE would
/// redistribute channel 1 to channels 2, 3, 4, and 5.  To set this highest channel number, you
/// change the following define.  By default it's 0.  0 or 1 mean "off":

#define FILTER_MAX_CHANNEL 0			// Can be 0 or 1 [Both Off], or any value 2-16 

/// USB MPE MODE
/// This mode takes MIDI input over USB and and outputs directly to PORT 1.  It assumes that we
/// are doing MPE with a ZONE LOW, where Channel 1 is the MPE Master channel, and the individual
/// channels are again channels 2...FILTER_MAX_CHANNEL.  This would be useful to connect to the MB/1
/// to create an MPE-controlled polysynth.  Additionally:
/// - AUDIO OUT outputs gated clock as usual
/// - IN 3 (PORT 2) outputs a trigger (RESET) when it receives START or CONTINUE
/// - AUDIO IN (PORT 3) outputs a trigger when it receives a STOP
/// - The Pots *can* be set to output CCs of your choice.
///
///
/// USB MPE MODE
/// This mode takes MIDI input over USB and and outputs directly to PORT 1.  It assumes that we
/// are doing MPE with a ZONE LOW, where Channel 1 is the MPE Master channel, and the individual
/// channels are again channels 2...FILTER_MAX_CHANNEL.  This would be useful to connect to the MB/1
/// to create an MPE-controlled polysynth.  Additionally:
/// - AUDIO OUT outputs gated clock as usual
/// - IN 3 (PORT 2) outputs a trigger (RESET) when it receives START or CONTINUE
/// - AUDIO IN (PORT 3) outputs a trigger when it receives a STOP
/// - The Pots *can* be set to output CCs of your choice.
///
///
/// USB DISTRIBUTOR BREAKOUT MODE
/// This mode is just like USB DISTRIBUTOR MODE except that it outputs channel 2 to PORT1,
/// channel 3 to PORT2, and channel 4 to PORT3.  This would be useful if you DIDN'T have
/// a Wonkystuff MB/1, and still wanted to have at least a 3-voice single-channel polysynth.
/// - AUDIO OUT outputs gated clock as usual
/// - The Pots *can* be set to output CCs of your choice.
/// - If there aren't any available channels for voices, voice stealing just steals channel 2
///
///
/// USB MPE BREAKOUT MODE
/// This mode is just like USB MPE MODE except that it outputs channel 2 to PORT1,
/// channel 3 to PORT2, and channel 4 to PORT3.  This would be useful if you DIDN'T have
/// a Wonkystuff MB/1, and still wanted to have at least a 3-voice MPE-based polysynth.
/// - AUDIO OUT outputs gated clock as usual
/// - The Pots *can* be set to output CCs of your choice.
///
///
/// INTERNAL DISTRIBUTOR BREAKOUT MODE
/// This mode is just like USB DISTRIBUTOR BREAKOUT MODE except that it takes INPUT from PORT1,
/// notionally from the Wonkystuff MB/1 or another Wonkystuff module, and distributes channel 1 to
/// channels 2 and 3, then outputs them as PORT2 and PORT3.  This not a common case, but would be 
/// useful to build a 2-voice polysynth.
/// - AUDIO OUT outputs gated clock as usual
/// - The Pots *can* be set to output CCs of your choice.
/// - If there aren't any available channels for voices, voice stealing just steals channel 2
///
///
/// INTERNAL CLOCK MODE
/// This mode is just like USB CLOCK MODE except that it takes INPUT from PORT1,
/// and outputs reset and gate on PORT2 and PORT3. 
/// - AUDIO OUT outputs gated clock as usual
/// - IN 3 (PORT 2) outputs a trigger (RESET) when it receives START or CONTINUE
/// - AUDIO IN (PORT 3) outputs a GATE
/// - The Pots should NOT be set to output anything
///
///
/// NO MODE 	(NONE)
/// You will need to customize everything yourself.
///
///
/// To set the MODE, you change the #define MODE below

// don't touch these, they're constants
#define NONE 0
#define USB_CLOCK 1
#define USB_ROUTER 2
#define USB_DISTRIBUTOR 3
#define USB_MPE 4
#define USB_DISTRIBUTOR_BREAKOUT 5
#define USB_MPE_BREAKOUT 6
#define INTERNAL_CLOCK 7
#define INTERNAL_DISTRIBUTOR_BREAKOUT 8

#define MODE	 USB_ROUTER				// Change this to one of the MODES above


/// CUSTOMIZING THE POT CC OUTPUTS
///
/// Remember, you can't set the Pot CCs if you're doing USB_CLOCK or INTERNAL_CLOCK modes.
///
/// The CC PARAMETER for each Pot is defined with the following #defines:

#define POT_1_CC	5		// Wonkystuff MCO/1 Sub.  Can be any value 1...128 OR NONE
#define POT_2_CC	6		// Wonkystuff MCO/1 Ramp.  Can be any value 1...128 OR NONE
#define POT_3_CC	7		// Wonkystuff MCO/1 Square.  Can be any value 1...128 OR NONE

/// You can customize which ports (PORT1, PORT2, PORT3) will receive CCs from the pots by
/// modifying the "true" values below to "false" (or 0):

const boolean INJECT_POTS_TO_OUTPUT[3] = { true, true, true };


/// DE-LEGATO
///
/// This sends a NOTE OFF before any NOTE ON, to guarantee that we do a gate prior to changing the pitch.
/// This is specified on a per-channel basis.  By default, it's ON (1) for ALL OUTGOING CHANNELS.  But you
/// can change it here:

/// This adds a NOTE_OFF before every NOTE_ON LEAVING on the following channels, if 1, else 0
const uint8_t DELEGATO_CHANNELS[16] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
#define DELEGATO 1			// If you set this to 0, DE-LEGATO is turned off entirely



/// FULL CUSTOMIZING
///
/// If you set MODE to NONE, you have the option of completely customizing everything.  Here are your
/// options:

/// (Don't play with these, they're constants)
#define USB 1
#define PORT1 2
#define PORT2 3 
#define PORT3 4
#define NO_CV 255
#define ALL 17
#define MPE 1
#define DISTRIBUTE 2

/////// You can customize these if MODE is NONE

// INPUT MIDI port
#define INPUT 	NONE		// can be NONE, USB, or PORT1

// OUTPUT MIDI ports
#define OUTPUT1 NONE		// can be NONE, PORT1, PORT2, or PORT3
#define OUTPUT2 NONE		// can be NONE, PORT1, PORT2, or PORT3
#define OUTPUT3 NONE		// can be NONE, PORT1, PORT2, or PORT3

// How Input is filtered to Output
#define FILTER_CHANNEL_1 	NONE		// can be NONE, MPE, or DISTRIBUTE

// MIDI channels for output ports
#define OUTPUT_1_CHANNEL	// can be NONE, 1...16, or ALL
#define OUTPUT_2_CHANNEL	// can be NONE, 1...16, or ALL
#define OUTPUT_3_CHANNEL	// can be NONE, 1...16, or ALL

// Reset (Start) Stop, and Gate signals
#define GATE NONE			// can be PORT1, PORT2 or PORT3
#define RESET NONE			// can be PORT1, PORT2 or PORT3
#define STOP NONE			// can be PORT1, PORT2 or PORT3

// Are we debugging to the USB port?
#define DEBUG	0













// MODE DEFINITIONS

#if (MODE == USB_CLOCK)
#define INPUT USB
#define OUTPUT1 NONE
#define OUTPUT2 NONE
#define OUTPUT3 NONE
#define OUTPUT_1_CHANNEL NONE
#define OUTPUT_2_CHANNEL NONE
#define OUTPUT_3_CHANNEL NONE
#define STOP PORT3
#define RESET PORT2
#define GATE PORT1
#define POT_1_CC NONE
#define POT_2_CC NONE
#define POT_3_CC NONE
#define FILTER_CHANNEL_1 	NONE

#elif (MODE == USB_ROUTER)
#define INPUT USB
#define OUTPUT1 PORT1
#define OUTPUT2 NONE
#define OUTPUT3 NONE
#define OUTPUT_1_CHANNEL ALL
#define OUTPUT_2_CHANNEL NONE
#define OUTPUT_3_CHANNEL NONE
#define STOP PORT3
#define RESET PORT2
#define GATE NONE
#define FILTER_CHANNEL_1 	NONE

#elif (MODE == USB_DISTRIBUTOR)
#define INPUT USB
#define OUTPUT1 PORT1
#define OUTPUT2 NONE
#define OUTPUT3 NONE
#define OUTPUT_1_CHANNEL ALL
#define OUTPUT_2_CHANNEL NONE
#define OUTPUT_3_CHANNEL NONE
#define STOP PORT3
#define RESET PORT2
#define GATE NONE
#define FILTER_CHANNEL_1 	DISTRIBUTE

#elif (MODE == USB_MPE)
#define INPUT USB
#define OUTPUT1 PORT1
#define OUTPUT2 NONE
#define OUTPUT3 NONE
#define OUTPUT_1_CHANNEL ALL
#define OUTPUT_2_CHANNEL NONE
#define OUTPUT_3_CHANNEL NONE
#define STOP PORT3
#define RESET PORT2
#define GATE NONE
#define FILTER_CHANNEL_1 	MPE

#elif (MODE == USB_DISTRIBUTOR_BREAKOUT)
#define INPUT USB
#define OUTPUT1 PORT1
#define OUTPUT2 PORT2
#define OUTPUT3 PORT3
#define OUTPUT_1_CHANNEL 2
#define OUTPUT_2_CHANNEL 3
#define OUTPUT_3_CHANNEL 4
#define STOP NONE
#define RESET NONE
#define GATE NONE
#define FILTER_CHANNEL_1 	DISTRIBUTE

#elif (MODE == USB_MPE_BREAKOUT)
#define INPUT USB
#define OUTPUT1 PORT1
#define OUTPUT2 PORT2
#define OUTPUT3 PORT3
#define OUTPUT_1_CHANNEL 2
#define OUTPUT_2_CHANNEL 3
#define OUTPUT_3_CHANNEL 4
#define STOP NONE
#define RESET NONE
#define GATE NONE
#define FILTER_CHANNEL_1 	MPE

#elif (MODE == INTERNAL_DISTRIBUTOR_BREAKOUT)
#define INPUT PORT1
#define OUTPUT1 PORT2
#define OUTPUT2 PORT3
#define OUTPUT3 NONE
#define OUTPUT_1_CHANNEL 2
#define OUTPUT_2_CHANNEL 3
#define OUTPUT_3_CHANNEL NONE
#define STOP NONE
#define RESET NONE
#define GATE NONE
#define FILTER_CHANNEL_1 	DISTRIBUTE
#endif

#if (MODE == INTERNAL_CLOCK)
#define INPUT PORT1
#define OUTPUT1 NONE
#define OUTPUT2 NONE
#define OUTPUT3 NONE
#define OUTPUT_1_CHANNEL NONE
#define OUTPUT_2_CHANNEL NONE
#define OUTPUT_3_CHANNEL NONE
#define STOP NONE
#define RESET PORT2
#define GATE PORT3
#define POT_1_CC NONE
#define POT_2_CC NONE
#define POT_3_CC NONE
#define FILTER_CHANNEL_1 	NONE
#endif

#if (MODE == NONE)
// we do nothing
#endif



// Error checking
#define ERROR(val) static_assert(false, val);
#if (INPUT == NONE && OUTPUT1 == NONE && OUTPUT2 == NONE && OUTPUT3 == NONE)
ERROR("All Inputs and Outputs are set to NONE, won't be very interesting.")
#endif
#if (OUTPUT1 == OUTPUT2 && OUTPUT1 != NONE)
ERROR("OUTPUT 1 and OUTPUT 2 are set to the same value, which is not permitted.")
#endif
#if (OUTPUT2 == OUTPUT3 && OUTPUT2 != NONE)
ERROR("OUTPUT 2 and OUTPUT 3 are set to the same value, which is not permitted.")
#endif
#if (OUTPUT3 == OUTPUT1 && OUTPUT3 != NONE)
ERROR("OUTPUT 1 and OUTPUT 3 are set to the same value, which is not permitted.")
#endif
#if (INPUT != NONE && INPUT != USB && INPUT != PORT1)
ERROR("INPUT must be NONE, USB, or PORT1")
#endif
#if (INPUT != NONE && INPUT == OUTPUT1)
ERROR("INPUT cannot be the same as OUTPUT 2")
#endif
#if (OUTPUT1 != NONE && OUTPUT1 != PORT1 && OUTPUT1 != PORT2 && OUTPUT1 != PORT3)
ERROR("OUTPUT1 must be NONE, PORT1, PORT2, or PORT3")
#endif
#if (OUTPUT2 != NONE && OUTPUT2 != PORT1 && OUTPUT2 != PORT2 && OUTPUT2 != PORT3)
ERROR("OUTPUT2 must be NONE, PORT1, PORT2, or PORT3")
#endif
#if (OUTPUT3 != NONE && OUTPUT3 != PORT1 && OUTPUT3 != PORT2 && OUTPUT3 != PORT3)
ERROR("OUTPUT3 must be NONE, PORT1, PORT2, or PORT3")
#endif
#if (RESET != NONE && RESET != PORT1 && RESET != PORT2 && RESET != PORT3)
ERROR("RESET must be NONE, PORT1, PORT2, or PORT3")
#endif
#if (GATE != NONE && GATE != PORT1 && GATE != PORT2 && GATE != PORT3)
ERROR("GATE must be NONE, PORT1, PORT2, or PORT3")
#endif
#if (STOP != NONE && STOP != PORT1 && STOP != PORT2 && STOP != PORT3)
ERROR("STOP must be NONE, PORT1, PORT2, or PORT3")
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
#if (RESET != NONE && RESET == OUTPUT3)
ERROR("RESET may not be the same as OUTPUT3")
#endif
#if (STOP != NONE && STOP == OUTPUT1)
ERROR("STOP may not be the same as OUTPUT1")
#endif
#if (STOP != NONE && STOP == OUTPUT2)
ERROR("STOP may not be the same as OUTPUT2")
#endif
#if (STOP != NONE && STOP == OUTPUT3)
ERROR("STOP may not be the same as OUTPUT3")
#endif
#if (GATE != NONE && GATE == OUTPUT1)
ERROR("GATE may not be the same as OUTPUT1")
#endif
#if (GATE != NONE && GATE == OUTPUT2)
ERROR("GATE may not be the same as OUTPUT2")
#endif
#if (GATE != NONE && GATE == OUTPUT3)
ERROR("GATE may not be the same as OUTPUT3")
#endif
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









#include <NeoSWSerial.h>    // you have to install this via the library manager
#include <NeoHWSerial.h>    // you have to install this via the library manager
#include <AltSoftSerial.h>  // you have to install this via the library manager
#include <FlexiTimer2.h>    // you have to install this via the library manager, but I include a zip just in case

////////// PINOUTS

#define CV_POT_IN1    A2    // CC 1 Value
#define CV_POT_IN2    A1    // CC 2 Value
#define CV_POT3       A0    // CC 3 Value
#define CV_IN3        A3    // PORT 2
#define CV_AUDIO_IN   A4    // PORT 3
#define CV_AUDIO_OUT  11    // MIDI Clock (only when between start/continue and stop)
#define CV_GATE_OUT   8     // PORT 1
#define SERIAL_1_RX	  5		// Blank Serial Pin
#define SERIAL_2_RX	  6		// Blank Serial Pin
#define SERIAL_3_RX	  7		// Blank Serial Pin



// Define the ports, except for HW Serial

/////// SERIAL PORT DEFINITIONS
/////// Serial ports are then setup in setup()

NeoSWSerial* outputs[3] = { NULL, NULL, NULL };		// pointers to all three output serial ports

// The author of NeoSWSerial suggested you could use an invalid
// pin, such as 255, for the RX if you didn't have an RX, but this
// appears to be wrong -- you have to have a valid pin or it doesn't
// output, or outputs in some corrupted or incorrect rate fashion.
// so I'm using SERIAL_1_RX etc.
// #define INVALID_PIN 255

#if (OUTPUT1 == PORT1)
NeoSWSerial softSerial1(SERIAL_1_RX, CV_GATE_OUT);
#elif (OUTPUT1 == PORT2)
NeoSWSerial softSerial1(SERIAL_1_RX, CV_IN3);
#elif (OUTPUT1 == PORT3)
NeoSWSerial softSerial1(SERIAL_1_RX, CV_AUDIO_IN);
#endif

#if (OUTPUT2 == PORT1)
NeoSWSerial softSerial2(SERIAL_2_RX, CV_GATE_OUT);
#elif (OUTPUT2 == PORT2)
NeoSWSerial softSerial2(SERIAL_2_RX, CV_IN3);
#elif (OUTPUT2 == PORT3)
NeoSWSerial softSerial2(SERIAL_2_RX, CV_AUDIO_IN);
#endif

#if (OUTPUT3 == PORT1)
NeoSWSerial softSerial3(SERIAL_3_RX, CV_GATE_OUT);
#elif (OUTPUT3 == PORT2)
NeoSWSerial softSerial3(SERIAL_3_RX, CV_IN3);
#elif (OUTPUT3 == PORT3)
NeoSWSerial softSerial3(SERIAL_3_RX, CV_AUDIO_IN);
#endif

#if (INPUT == PORT1)
AltSoftSerial altsoftserial;		// RX on CV_GATE_OUT (pin 8) (TX on pin 9, can't use)
#endif


////// MIDI DEFINES
#define MIDI_RATE 31250
#define MIDI_CLOCK 0xF8
#define MIDI_START 0xFA
#define MIDI_CONTINUE 0xFB
#define MIDI_STOP 0xFC
#define MIDI_CC 0xB0
#define MIDI_NOTE_ON 0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_POLY_AT 0xA0
#define MIDI_SYSTEM 0xF0
#define MIDI_SYSTEM_REALTIME MIDI_CLOCK		// MIDI_CLOCK is the first one



const uint8_t OUTPUT_CHANNELS[3] = { OUTPUT_1_CHANNEL, OUTPUT_2_CHANNEL, OUTPUT_3_CHANNEL };	// For each of the 3 outputs, either NONE, 1...16, or ALL




void setup()
    {
// Pin Modes
#if (INPUT == PORT1)
	pinMode(CV_GATE_OUT, INPUT);
#else
	pinMode(CV_GATE_OUT, OUTPUT);
#endif
	pinMode(CV_AUDIO_IN, OUTPUT);
	pinMode(CV_IN3, OUTPUT);
	pinMode(CV_AUDIO_OUT, OUTPUT);

// load ports
#if (OUTPUT1 == PORT1)
outputs[0] = &softSerial1;
#elif (OUTPUT1 == PORT2)
outputs[0] = &softSerial1;
#elif (OUTPUT1 == PORT3)
outputs[0] = &softSerial1;
#endif

#if (OUTPUT2 == PORT1)
outputs[1] = &softSerial2;
#elif (OUTPUT2 == PORT2)
outputs[1] = &softSerial2;
#elif (OUTPUT2 == PORT3)
outputs[1] = &softSerial2;
#endif

#if (OUTPUT3 == PORT1)
outputs[2] = &softSerial3;
#elif (OUTPUT3 == PORT2)
outputs[2] = &softSerial3;
#elif (OUTPUT3 == PORT3)
outputs[2] = &softSerial3;
#endif


// Start Input Serial Port
#if (INPUT == USB || DEBUG == 1)
	NeoSerial.begin(MIDI_RATE);
#endif
#if (INPUT == PORT1)
	altsoftserial.begin(MIDI_RATE);
#endif

// Start Output Serial Ports
#if (OUTPUT1 == PORT1 || OUTPUT1 == PORT2 || OUTPUT1 == PORT3)
	softSerial1.begin(MIDI_RATE);
#endif
#if (OUTPUT2 == PORT3 || OUTPUT2 == PORT2 || OUTPUT2 == PORT3)
	softSerial2.begin(MIDI_RATE);
#endif
#if (OUTPUT3 == PORT3 || OUTPUT3 == PORT2 || OUTPUT3 == PORT3)
	softSerial3.begin(MIDI_RATE);
#endif

	if (INJECT_POTS_TO_OUTPUT[0] == 1 || INJECT_POTS_TO_OUTPUT[1] == 1 || INJECT_POTS_TO_OUTPUT[2] == 1)
		{
		// set up timer
			FlexiTimer2::set(1, 1.0 / 256, setInject);
			FlexiTimer2::start();
		}
    }





/// preprocessING




/// parseMessage.  This facility parses the latest message and holds onto it.

#define MAX_MESSAGE_LENGTH 32
#define INVALID_MESSAGE_LENGTH (MAX_MESSAGE_LENGTH + 1)
uint8_t systemRealTime = 0;
uint8_t message[MAX_MESSAGE_LENGTH];
uint8_t messageLength = 0;
uint8_t messageExpectedLength = MAX_MESSAGE_LENGTH;
#define MESSAGE_COMPLETE() (messageLength >= messageExpectedLength && messageLength > 0)
#define VOICE_MESSAGE(m) (m[0] >= MIDI_NOTE_OFF && m[0] < 0xF) 
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
	if (c == 0xF7)	// handled specially
		{
		if (messageLength > 0 && message[0] == 0xF0)
			{
			if (messageLength != INVALID_MESSAGE_LENGTH)		// we're good
				messageExpectedLength = messageLength;
			else
				messageLength = 0;								// dump it
			}
		}
	else if (c >= MIDI_NOTE_OFF)
		{
		messageLength = 1;
		message[0] = c;
		if (c < 0xC0)	// note off, note on, poly at, cc
			{
			messageExpectedLength = 3;
			}
		else if (c < 0xE0)		// PC, AT
			{
			messageExpectedLength = 1;
			}
		else if (c < 0xF0)		// Bend
			{
			messageExpectedLength = 3;
			}
		else if (c == 0xF0)		// Sysex
			{
			messageExpectedLength = MAX_MESSAGE_LENGTH + 2;		// Max for us
			}
		else if (c == 0xF2)		// Song Position Pointer
			{
			messageExpectedLength = 3;
			}
		else if (c == 0xF3)		// Song Select
			{
			messageExpectedLength = 2;
			}
		else if (c < 0xF8)		// 0xF1 Midi Time Code Quarter Frame (ignored), tune request, undefined
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
	
	

void postProcessWithChannel(uint8_t* message, uint8_t len, uint8_t channel)
	{
	uint8_t c = message[0];
	// change channel
	uint8_t mess = (c & 0xF0);
	c = mess + channel;
	postprocess(c);
	// push rest of message
	for(uint8_t i = 1; i < len; i++)
		{
		postprocess(message[i]);
		}
	}


uint8_t noteChannels[128];						// NONE means not used, 1...128 means 0..127 note pitch
uint8_t availableChannels[FILTER_MAX_CHANNEL];	// NONE means available, 1...128 means 0..127 note pitch

inline void distribute()
	{
	if (VOICE_MESSAGE(message))
		{
		if (NOTE_OFF(message))
			{
			// release the voice
			uint8_t channel = noteChannels[PITCH(message)];
			if (channel != NONE && channel != 1) 
				{
				availableChannels[channel] = NONE;
				}
			noteChannels[PITCH(message)] = NONE;
			postProcessWithChannel(message, messageLength, channel);
			}
		else if (NOTE_ON(message))
			{
			// allocate a voice
			uint8_t channel = 0;
			for(uint8_t i = 1; i < FILTER_MAX_CHANNEL; i++)
				{
				if (availableChannels[i] == 0)	// available
					{
					availableChannels[i] = PITCH(message) + 1;
					noteChannels[PITCH(message)] = i;
					channel = i;
					break;
					}
				}
			if (channel == 0)	// everything allocated, we can't afford a heap, steal channel 2
				{
					// send a NOTE OFF to channel 2
					postprocess(MIDI_NOTE_OFF + 1);
					postprocess(availableChannels[1] - 1);
					postprocess(0x40);

					// allocate channel 2					
					availableChannels[1] = PITCH(message);
					noteChannels[PITCH(message)] = 1;
					channel = 1;
				}
			
			// now we have voice allocated 
			postProcessWithChannel(message, messageLength, channel);
			}
		else if (POLY_AT(message))		// do we really need this?
			{
			// What's the correct channel?
			uint8_t channel = noteChannels[PITCH(message)];
			if (channel != NONE) // uh...
				{
				postProcessWithChannel(message, messageLength, channel);
				}
			}
		else	// AT, Pitch Bend, Program Change, CC
			{
			// Send to everybody
			for(uint8_t i = 1; i < FILTER_MAX_CHANNEL; i++)
				{
				postProcessWithChannel(message, messageLength, i);
				}
			}
		}
	else 
		{
		for(uint8_t i = 0; i < messageLength; i++)
			postprocess(message[i]);
		}
	}



void addToChannel(uint8_t* message, uint8_t len, uint8_t channel)
	{
	if (len > 0)
		{
		if (VOICE_MESSAGE(message))
			{
			postProcessWithChannel(message, len, channel);
			}
		else
			{
			for(uint8_t i = 0; i < len; i++)
				{
				postprocess(message);
				}
			}
		}
	}

void preprocess(uint8_t c)
	{
#if (FILTER_CHANNEL_1 == MPE)
	parseMessage(c);
	if (MESSAGE_COMPLETE() && !systemRealTime && CHANNEL_1_MESSAGE(message))
		{
		// add to other channels
		for(uint8_t i = 2; i <= FILTER_MAX_CHANNEL; i++)
			addToChannel(message, messageLength, i);
		}
	postprocess(c);
#elif (FILTER_CHANNEL_1 == DISTRIBUTE && FILTER_MAX_CHANNEL > 1)
	parseMessage(c);
	if (MESSAGE_COMPLETE() && (!VOICE_MESSAGE(message) || CHANNEL_1_MESSAGE(message)))
		{
		if (systemRealtime) postprocess(systemRealTime);
		else distribute();
		}
#else
	postprocess(c);
#endif
	}





/// postprocessing


// DeLegato injects a NOTE OFF in front of every NOTE ON to make sure we do a gate

uint8_t noteOnPitch[16];				// per-channel, what was the last note played?
#define NO_CHANNEL 255
uint8_t lastNoteOnChannel = NO_CHANNEL;	// what was the last channel played?
inline void deLegato(uint8_t c)
	{
	if (c >= MIDI_NOTE_ON && c < MIDI_NOTE_ON + 16)
		{
		// Do we need to send a NOTE OFF?
		uint8_t channel = c - MIDI_NOTE_ON;
		if (DELEGATO_CHANNELS[channel])
			{
			for(uint8_t i = 0; i < 3; i++)
				{
				if (outputs[i] != NULL && (OUTPUT_CHANNELS[i] == channel || OUTPUT_CHANNELS[i] == ALL))
					{
					write(outputs[i], MIDI_NOTE_OFF + channel);
					write(outputs[i], noteOnPitch[channel]);
					write(outputs[i], 0x00);					// Note off
					}
				}
			}

		// What's the new note?  Prepare the channel for the next byte
		lastNoteOnChannel =  channel;
		}
	else if (lastNoteOnChannel < 16 &&
			 c < MIDI_NOTE_ON)				// It's not a status byte (remember, clock etc. can happen right now) 
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
inline uint8_t emitClock(uint8_t c)
	{
	if (c == MIDI_CLOCK) 
		{ 
		clockCountdown = MAX_COUNTDOWN; 
		digitalWrite(CV_AUDIO_OUT, 1);
		}
	else if (c == MIDI_STOP) 
		{ 
		resetCountdown = 0; 
		stopCountdown = MAX_COUNTDOWN; 
#if (GATE == PORT1 || RESET == PORT1)
		digitalWrite(CV_GATE_OUT, 0); 
#elif (GATE == PORT2 || RESET == PORT1)
		digitalWrite(CV_IN3, 0); 
#elif (GATE == PORT3 || RESET == PORT1)
		digitalWrite(CV_AUDIO_IN, 0); 
#endif
#if (STOP == PORT1)
		digitalWrite(CV_GATE_OUT, 1); 
#elif (STOP == PORT2)
		digitalWrite(CV_IN3, 1); 
#elif (STOP == PORT3)
		digitalWrite(CV_AUDIO_IN, 1); 
#endif
		}
	else if (c == MIDI_START || c == MIDI_CONTINUE) 
		{ 
		stopCountdown = 0;
		resetCountdown = MAX_COUNTDOWN; 
#if (GATE == PORT1 || RESET == PORT1)
		digitalWrite(CV_GATE_OUT, 1); 
#elif (GATE == PORT2 || RESET == PORT2)
		digitalWrite(CV_IN3, 1); 
#elif (GATE == PORT3 || RESET == PORT3)
		digitalWrite(CV_AUDIO_IN, 1); 
#endif
#if (STOP == PORT1)
		digitalWrite(CV_GATE_OUT, 0); 
#elif (STOP == PORT2)
		digitalWrite(CV_IN3, 0); 
#elif (STOP == PORT3)
		digitalWrite(CV_AUDIO_IN, 0); 
#endif
		}
	}


uint8_t postprocessChannel = ALL;
inline uint8_t postprocess(uint8_t c)
	{
#if (DELEGATO == 1)
	deLegato(c);
#endif

	if (c >= 0x80) 
		{
		if (c >= 0xF0)
			{
			postprocessChannel = ALL;
			}
		else
			{
			postprocessChannel = (c & 0x0F);
			}
		}

	for(uint8_t i = 0; i < 3; i++)
		{
		if (outputs[i] != NULL && (OUTPUT_CHANNELS[i] == postprocessChannel || OUTPUT_CHANNELS[i] == ALL || postprocessChannel == ALL))
			{
			write(outputs[i], c);
			}
		}

	emitClock(c);
	}
	


/// DEBUGGING

#ifdef DEBUG
String serialIdentifier(NeoSWSerial* softSerial)
	{
#if (MODE == USB_ROUTER)
	if (softSerial == &softSerial1) return "1: ";
	return ("?: ");
#elif (MODE == USB_DISTRIBUTOR)
	if (softSerial == &softSerial1) return "1: ";
	return ("?: ");
#elif (MODE == USB_MPE_BREAKOUT)
	if (softSerial == &softSerial1) return "1: ";
	if (softSerial == &softSerial2) return "2: ";
	if (softSerial == &softSerial3) return "3: ";
	return ("?: ");
#elif (MODE == USB_DISTRIBUTOR_BREAKOUT)
	if (softSerial == &softSerial1) return "1: ";
	if (softSerial == &softSerial2) return "2: ";
	if (softSerial == &softSerial3) return "3: ";
	return ("?: ");
#elif (MODE == INTERNAL_DISTRIBUTOR_BREAKOUT)
	if (softSerial == &softSerial1) return "1: ";
	if (softSerial == &softSerial1) return "2: ";
	return ("?: ");
#endif
	}
#endif


/// WRITING TO A SERIAL OUTPUT PORT

inline void debug(NeoSWSerial* softSerial, uint8_t c)
	{
#if (DEBUG == 1)
		NeoSerial.print("\t");
		NeoSerial.print(serialIdentifier(softSerial));
		NeoSerial.println(String(c, HEX));
#endif
	}

inline void debugString(const char* string)
	{
#if (DEBUG == 1)
		NeoSerial.print("\t");
		NeoSerial.println(string);
#endif
	}

inline void debugVal(uint8_t c)
	{
#if (DEBUG == 1)
		NeoSerial.print("\t");
		NeoSerial.println(String(c, HEX));
#endif
	}
	
inline void write(NeoSWSerial* softSerial, uint8_t c)
	{
	debug(softSerial, c);
	softSerial->write(c);
	}


uint8_t injectWaiting = false;
uint8_t oldCC1Val = 255;
uint8_t oldCC2Val = 255;
uint8_t oldCC3Val = 255;
uint8_t cc1Val = 0;
uint8_t cc2Val = 0;
uint8_t cc3Val = 0;

// Called by the timer to set up injection every once in a while.  We read the pots, then set the CC values and try to inject.
// If we fail (because a message is incomplete), we'll try to inject repeatedly after the message is done until we are successful.
void setInject() 
	{ 
	 cc1Val = analogRead(CV_POT_IN1) >> 3;
	 cc2Val = analogRead(CV_POT_IN2) >> 3;
	 cc3Val = analogRead(CV_POT3) >> 3;
	 injectWaiting = true;
	 // try now
	 inject();
	}

// Injects CC messages into the stream, only when a message is completely written.
void injectCC(uint8_t param, uint8_t value)
	{
	for(uint8_t i = 0; i < 3; i++)
		{
		if (INJECT_POTS_TO_OUTPUT[i] && outputs[i] != NULL)
			{
			write(outputs[i], MIDI_CC + OUTPUT_CHANNELS[i]); 
			write(outputs[i], param); 
			write(outputs[i], value);
			}
		}
	}

// Injects CC messages into the stream, only when a message is completely written.
void inject()
	{
	if (injectWaiting && MESSAGE_COMPLETE())
		{
		injectCC(POT_1_CC - 1, cc1Val);
		injectCC(POT_2_CC - 1, cc2Val);
		injectCC(POT_3_CC - 1, cc3Val);
		injectWaiting = false;
		}
	oldCC1Val = cc1Val;
	oldCC2Val = cc2Val;
	oldCC3Val = cc3Val;
	}



// Handle all manner of inputs and do the right thing with them.  This is called by either
// loop() or by the interrupt
void handleRead(uint8_t c)
{
	preprocess(c);
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
				digitalWrite(CV_IN3, 0); 
			#elif (STOP == PORT3)
				digitalWrite(CV_AUDIO_IN, 0); 
			#endif
 			}
 		}

 	if (resetCountdown >= 1) 
 		{ 
 		resetCountdown--; 
 		if (resetCountdown == 0) 
 			{
			#if (RESET == PORT1)
				digitalWrite(CV_GATE_OUT, 0); 
			#elif (RESET == PORT2)
				digitalWrite(CV_IN3, 0); 
			#elif (RESET == PORT3)
				digitalWrite(CV_AUDIO_IN, 0); 
			#endif
 			}
 		}
    
    while(true)
    	{
    #if (INPUT == PORT1)
    	uint8_t val = altsoftserial.available();
		for(uint8_t i = 0; i < val; i++)
			{
			handleRead(altsoftserial.read());
			}
	#elif (INPUT == USB)	
    	uint8_t val = NeoSerial.available();
		for(uint8_t i = 0; i < val; i++)
			{
			handleRead(NeoSerial.read());
			}
	#endif
    	}
    }
