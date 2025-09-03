#include "parsemidi.c"
#include "emitmidi.c"
#include "utilities.c"


// MASTER
//
// Master handles and redistributes MIDI Mode 4 or MPE Master Zone messages in combination with an IMDI HEART.
// Master can only be used in scenarios involving a SINGLE MPE Master Zone.  This normally should be the LOWER ZONE,
// but it is possible to configure it for an Upper Zone.
// 
// Explanation.  MIDI Polyphonic Expression (MPE) takes up multiple channels.  These channels are divided into
// a single MASTER CHANNEL, and then some N ZONE CHANNELS.  When notes arrive in MPE they show up in the ZONE CHANNELS.
// Other messages, such as CC or Pitch Bend, also arrive individually on the zone channels, but they can also show
// up on the MASTER CHANNEL, in which they are interpreted as affecting all the zone channels together.  But Modular MIDI
// on the AE system doesn't think about a Master Channel, so it has no way to route the Master's messages to the zone channels.
//
// The most common scenario is to route CC or PC to all your polyphonic oscillators or other voice modules.
//
// If you need to route the Master's messages to zones, that's what Master is for.
//
// CONFIGURATION
//
// You will need an IMDI HEART.  Master can be set up to run on one of two configurations:
//
// 1. Some kind of Arduino, such as an Arduino Nano or Arduino Nano Every.  You will connect to its hardware UART,
//    that its RX and TX ports.  You will also need to set HARD_SERIAL to the appropriate device.  On most Arduinos,
//    this device is     Serial     But on the Nano Every this device is      Serial1      
//    Accompanying this file is a PDF file showing configurations of the Arduino Nano Every on
//    a BRAEDBOARD which might be useful for you in setting up the hardware.
//
// 2. GRAINS V2.  This doesn't exist yet: I have a prototype but it may never go into production.
//    You will connect to the RX and TX ports.   Uncomment GRAINS_V2.  And set HARD_SERIAL to    Serial
//
// The arrangement is as follows.  We will modify the IMDI HEART so that it routes to its 16 outputs data coming
// from the IMDI input instead of from MIDI or USB.  To do this, we set ICN=NONE.  [It should be ICN=IMDI, but there
// is a bug in IMDI HEART which accidentally swapped IMDI and NONE in the ICN menu!]
//
// Next, we connect the socket labelled "MIDI IN -> IMDI" (very confusing) to RX on our GRAINS V2 or Arduino.
// Finally, we attach TX on the GRAINS V2 or Arduino to the top left "IMDI" socket on the IMDI HEART.
//
// What will happen is this.  MIDI comes into the MIDI socket or USB on the IMDI HEART.  It is then routed directly to
// the Grains or Arduino.  The Grains or Arduino then modifies the MIDI and sends it back to the IMDI HEART in the IMDI
// socket.  This is then redirected to the 16 sockets.
//
// Pitch Bend is the current problem child.  The MPE spec specifies that Bend messages must be receivable both on
// the Master Channel and also on the per-note channels, and furthermore that these may have different bend ranges,
// and a synthesizer must be able to add them together in a meaningful way.  We cannot easily do that, and it is costly
// to compute.  So at present, DISTRIBUTE_BEND is turned off, and if you turn it on, MASTER simply redistributes
// the Master Channel bend message to the children.


// DISTRIBUTION RULES
//
// Not all MIDI data is routed: some is ignored because it is inappropriate to route.  Here are the rules.
//
//
// NOTE ON		Master messages are ignored.  Channel messages are passed through.
// NOTE OFF	Master messages are ignored.  Channel messages are passed through.
// POLY AFTERTOUCH		Master messages are ignored.  Channel messages (discouraged) are passed through.
// CHANNEL AFTERTOUCH	Master messages are distributed to all channels.  Channel messages (discouraged) are passed through.
// BANK / PROGRAM CHANGE (PC)		Master messages are distributed to all channels.  Channel messages are passed through.
// CONTROL CHANGE (CC)	Master messages are distributed to all channels.  Channel messages are passed through.
// PITCH BEND		Master messages are distributed to all channels only if DISTRIBUTE_BEND.  Channel messages are passed through.
// SONG POSITION POINTER, SONG SELECT	All messages are passed through.
// START, STOP, CONTINUE, CLOCK PULSE	All messages are passed through.
// SYSTEM EXCLUSIVE	All messages are passed through.
// ACTIVE SENSING, SYSTEM RESET, TUNE REQUEST	All messages are ignored.
// MIDI TIME CODE QUARTER FRAME	 All messages are ignored.

// LATENCY
//
// To do this magic, MASTER must parse entire MIDI messages and then emit them along multiple channels.  Thus there
// is a SMALL amount of latency until MASTER has received an entire MIDI message before it can send it out.  There also
// will be a SIGNIFICANT amount of latency if MASTER has to repeat this message multiple times to multiple zone channels.
// For example, if your MPE Zone is 15 channels, and MASTER receives a CC message on the Master Channel, it will have to
// emit 15 separate CC messages, one for each channel.




// HARD_SERIAL
//
// This dicates the serial device being used.  Normally this is Serial,
// But if you have a Nano Every, you might wish to set this to Serial1 (don't set to Serial2 or Serial3).

#define HARD_SERIAL Serial



// GRAINS_V2
//
// This indicates that my GRAINS V2 prototype is being used.  Since I'm presently the only person
// who ones a GRAINS V2, you probably comment this out!

#define GRAINS_V2




// NUM_ZONE_CHANNELS
//
// This dictates the size of your MPE Zone, not including the Master Channel.  This can be
// any value up to and including 14.

#define NUM_ZONE_CHANNELS 3				// should be no more than 3 if you're chosing MULTI_OUTPUT



// MASTER_CHANNEL
//
// This indicates what the MPE Zone Master Channel is.  It will be either 0 or 15.
//
// When using the MPE LOWER ZONE as your sole MPE Zone, as is usually the case,
// set MASTER_CHANNEL to 0 (that is, MIDI Channel 1), and set ZONE_START_CHANNEL to 1
// (that is, MIDI Channel 2).
//
// If using the MPE UPPER ZONE as a sole MPE Zone, which is much less common,
// set MASTER_CHANNEL to 15 (that is, MIDI Channel 16), and set ZONE_START_CHANNEL to 
// (MASTER_CHANNEL - NUM_ZONE_CHANNELS).  For example, if you have 6 zone channels, then
// your starting channel would be 15 - 6 = 9 (that is, MIDI channel 10).

#define MASTER_CHANNEL 0



// ZONE_START_CHANNEL
//
// This indicates what the MPE Zone Channels will be.  ZONE_START_CHANNEL is the
// lowest such channel, and is a value 1...14, representing MIDI channels 2 through 15.
//
// When using the MPE LOWER ZONE as your sole MPE Zone, as is usually the case,
// set MASTER_CHANNEL to 0 (that is, MIDI Channel 1), and set ZONE_START_CHANNEL to 1
// (that is, MIDI Channel 2).
//
// If using the MPE UPPER ZONE as a sole MPE Zone, which is much less common,
// set MASTER_CHANNEL to 15 (that is, MIDI Channel 16), and set ZONE_START_CHANNEL to 
// (MASTER_CHANNEL - NUM_ZONE_CHANNELS).  For example, if you have 6 zone channels, then
// your starting channel would be 15 - 6 = 9 (that is, MIDI channel 10).

#define ZONE_START_CHANNEL 1



// DISTRIBUTE_BEND
//
// This indicates that if a BEND message appears on the zone master channel, it should
// be redistributed to the other channels, simply replacing existing per-channel bend signals. 
// In most cases you don't want this.
// Maybe later we'll set it up so that it gets added to per-channel bend in an appropriate way.
// But it's suprisingly complex to do.

// #define DISTRIBUTE_BEND




// LEFT SOCKETS IN ORDER
#define CV_POT_IN1    A2
#define CV_POT_IN2    A1
#define CV_IN3        A3
#define CV_AUDIO_IN   A4			// Used for soft serial in
#define CV_IN5	      A5

// ADDITIONAL INPUTS
#define CV_POT3       A0
#define CV_SW1		  3				// Has no pull down resistor.  Use as INPUT_PULLUP.  *NEVER* use as OUTPUT, will destroy it.
#define CV_SW2		  4				// Has no pull down resistor.  Use as INPUT_PULLUP.  *NEVER* use as OUTPUT, will destroy it.
#define CV_SW3		  7				// Has no pull down resistor.  Use as INPUT_PULLUP.  *NEVER* use as OUTPUT, will destroy it.

// RIGHT SOCKETS IN ORDER
#define CV_AUDIO_OUT  9				// or 11 if GRAINS
#define CV_D10		  10			// Second from top
#define CV_GATE_OUT   8				// "D", used for soft serial out
#define CV_RX		  0				// UART
#define CV_TX		  1				// UART
#define CV_LED_1	  2				// "D2" output, third from bottom
#define CV_LED_2	  5				// "D5" output, second from bottom.  Appears to go through a filter
#define CV_LED_3	  6				// "D6" output, bottom.  Appears to go through a filter

// ADDITIONAL PINOUTS
#define RANDOM_1      A6
#define RANDOM_2      A7			// Used for blank Serial Pin




struct noteDistributor distributor;
struct midiParser parser;
struct midiEmitter emitter;
#ifdef GRAINS_V2
uint8_t resetButton = 0;
uint32_t resetTime = 0;
#define RESET_INTERVAL 250
#endif

extern void noteOn(struct midiParser* parser, unsigned char note, unsigned char velocity)
	{
	uint8_t channel = getParserMIDIChannel(parser);
	if (channel == MASTER_CHANNEL)
		{
		// do nothing
		}
	else
		{
		emitNoteOn(&emitter, note, velocity, channel);
		}
  	}
	
extern void noteOff(struct midiParser* parser, unsigned char note, unsigned char velocity)
	{
	uint8_t channel = getParserMIDIChannel(parser);
	if (channel == MASTER_CHANNEL)
		{
		// do nothing
		}
	else
		{
		emitNoteOffVel(&emitter, note, velocity, channel);
		}
	}

extern void polyAftertouch(struct midiParser* parser, unsigned char note, unsigned char value)	
	{
	uint8_t channel = getParserMIDIChannel(parser);
	if (channel == MASTER_CHANNEL)
		{
		// do nothing
		}
	else
		{
		emitPolyAftertouch(&emitter, note, value, channel);
		}
	}
	
extern void aftertouch(struct midiParser* parser, unsigned char value)
	{
	uint8_t channel = getParserMIDIChannel(parser);
	if (channel == MASTER_CHANNEL)
		{
		for(uint8_t i = ZONE_START_CHANNEL; i < ZONE_START_CHANNEL + NUM_ZONE_CHANNELS; i++)
			{
			emitAftertouch(&emitter, value, i);
			}
		}
	else
		{
		emitAftertouch(&emitter, value, channel);
		}
	}

extern void pc(struct midiParser* parser, unsigned char program, unsigned char bankMSB, unsigned char bankLSB)
	{
	uint8_t channel = getParserMIDIChannel(parser);
	if (channel == MASTER_CHANNEL)
		{
		for(uint8_t i = ZONE_START_CHANNEL; i < ZONE_START_CHANNEL + NUM_ZONE_CHANNELS; i++)
			{
			emitBankAndPC(&emitter, program, bankMSB, bankLSB, i);
			}
		}
	else
		{
		emitBankAndPC(&emitter, program, bankMSB, bankLSB, channel);
		}
	}

extern void cc(struct midiParser* parser, unsigned char parameter, unsigned char value)
	{
	uint8_t channel = getParserMIDIChannel(parser);
	if (channel == MASTER_CHANNEL)
		{
		for(uint8_t i = ZONE_START_CHANNEL; i < ZONE_START_CHANNEL + NUM_ZONE_CHANNELS; i++)
			{
			emitCC(&emitter, parameter, value, i);
			}
		}
	else
		{
		emitCC(&emitter, parameter, value, channel);
		}
	}

extern void bend(struct midiParser* parser, SIGNED_16_BIT_INT value)
	{
	uint8_t channel = getParserMIDIChannel(parser);
	if (channel == MASTER_CHANNEL)
		{
#ifdef DISTRIBUTE_BEND		
		for(uint8_t i = ZONE_START_CHANNEL; i < ZONE_START_CHANNEL + NUM_ZONE_CHANNELS; i++)
			{
			emitBend(&emitter, value, i);
			}
#endif
		}
	else
		{
		emitBend(&emitter, value, channel);
		}
	}

extern void songPositionPointer(struct midiParser* parser, UNSIGNED_16_BIT_INT value)
	{
		emitSongPositionPointer(&emitter, value);
	}
	
extern void songSelect(struct midiParser* parser, unsigned char song)
	{
		emitSongSelect(&emitter, song);
	}

extern void clockStop(struct midiParser* parser)
	{
		emitClockStop(&emitter);
	}
	
extern void clockStart(struct midiParser* parser)
	{
		emitClockStart(&emitter);
	}
	
extern void clockContinue(struct midiParser* parser)
	{
		emitClockContinue(&emitter);
	}
	
extern void clockPulse(struct midiParser* parser)
	{
		emitClockPulse(&emitter);
	}
	
extern void sysex(struct midiParser* parser, unsigned char* buffer, unsigned char len, unsigned char status)
	{
	uint8_t F7 = 0xF7;
	uint8_t F0 = 0xF0;
		if (status == STATUS_SYSEX_COMPLETE)
			{
			emitSysex(&emitter, &F0, 1);
			if (len != 0) emitSysex(&emitter, buffer, len);
			emitSysex(&emitter, &F7, 1);
			}
		else if (status == STATUS_SYSEX_START)
			{
			emitSysex(&emitter, &F0, 1);
			if (len != 0) emitSysex(&emitter, buffer, len);
			}
		else if (status == STATUS_SYSEX_END)
			{
			if (len != 0) emitSysex(&emitter, buffer, len);
			emitSysex(&emitter, &F7, 1);
			}
		else if (status == STATUS_SYSEX_INCOMPLETE)
			{
			if (len != 0) emitSysex(&emitter, buffer, len);
			}
	}
	
void setup()
	{
	HARD_SERIAL.begin(31250);
	initializeParser(&parser, MASTER_CHANNEL, 0, true);
	initializeEmitter(&emitter, 0);
#ifdef GRAINS_V2
	pinMode(CV_SW1, INPUT_PULLUP);			// This must NEVER BE OUTPUT or it will damage the Grains
#endif
	}




void loop()
	{
#ifdef GRAINS_V2
	if (digitalRead(CV_SW1))
		{
		resetButton = 1;
		resetTime = millis();
		}
	else if (resetButton == 1 && resetTime != 0 && millis() > resetTime + RESET_INTERVAL)
		{
		resetButton = 0;
		resetTime = 0;

		for(uint8_t i = 0; i < NUM_ZONE_CHANNELS; i++)
			{
			emitCC(&emitter, 120, 0, i);		// All Sound Off
			}

		for(uint8_t i = 0; i < NUM_ZONE_CHANNELS; i++)
			{
			emitCC(&emitter, 123, 0, i);		// All Notes Off
			}
		}
#endif
		
	if (HARD_SERIAL.available())
		{
		parseMidi(&parser, HARD_SERIAL.read());
		}
	}
	
extern void emitMidi(struct midiEmitter* emitter, unsigned char byte)
	{
	HARD_SERIAL.write(byte);
	}
	
