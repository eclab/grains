#include "parsemidi.c"
#include "emitmidi.c"
#include "utilities.c"


// ENSEMBLE
//
// Ensemble is a note distributor.  A note distributor takes notes coming in on one channel,
// or on OMNI, and distributes them to up to N channels, possibly to up to 10 separate output
// sockets.  This allows you to play chords or polyphony and have your AE Modular rack play
// all of the notes.
//
// For example, if you play C E G, then the C might get get routed to Channel or socket 0,
// the E is routed to channel or socket 1, and the G is routed to channel or socket 2.  To do
// this, the distributor must keep track of which notes are assigned to which channels so it
// can turn them OFF when you let go of keys.
//
// There are two basic ways you could set ensemble up.  First, you could use an Arduino 
// device which has multiple serial ports, such as the Arduino Nano Every.  Here, MIDI data
// would come in one serial port, then get distributed to N channels and each channel would
// be set up to go out a separate serial port and thus a separate socket.  You'd need a device
// such as an MB/1 (or an IMDI HEART) to get the MIDI data in the first place.
// Accompanying this file is a PDF file showing configurations of the Arduino Nano Every on
// a BRAEDBOARD which might be useful for you in setting this up.
//
// Alternatively you could use an Arduino device with a single serial port, but rig it up with
// the IMDI HEART to break out to separate sockets.  Here, MIDI data would come into the IMDI
// HEART and immediately routed to your Arduino's input serial port.  The Arduino would distribute
// the notes to separate channells and write them out its output serial port.  This would then
// get fed into the IMDI Input on the IMDI Heart, and the IMDI Heart would then break the channels
// out to separate sockets.  You have to do a minor menu setting on the IMDI Heart to pull this off.
//
// Ensemble uses the note distributor code found in utility.c; you may find this useful for
// other projects.
//
// Additionally, when it receives an ALL NOTES OFF or ALL SOUNDS OFF message, ENSEMBLE will
// both send this message on to all of its voices, and will also reset itself.
//
// Here are some configurations.  Note that I am have only tested #1.
//
// 1. GRAINS V2 PROTOTYPE OR NANO OR NANO EVERY IN COMBINATION WITH AN IMDI HEART.
//    I presently own a prototype GRAINS V2 from Tangible Waves, but
//    it is far from clear if this prototype will ever go into production.  The prototype
//    has buttons and LEDs on it and exposed RX and TX pins.  I can use it to distribute
//    incoming MIDI notes on a single channnel (or on OMNI) to up to 16 channels output.
//    I then route the output back into IMDI HEART and it breaks it up into the 16 sockets!
//    to do this, I set ICN=NONE on IMDI HEART (that's a bug on my unit, ICN=NONE is actually
//    ICN=IMDI and ICN=IMDI is ICN=NONE).  This causes IMDI HEART to route IMDI, not MIDI,
//    into the 16 breakout sockets.  Then I attach my MIDI device to IMDI HEART, and connect 
//    the socket labelled "MIDI IN -> IMDI" (very confusing) to RX on my GRAINS.  Then I
//    attach TX on my GRAINS to the top left "IMDI" socket on the IMDI HEART.  And it works!
//
//    You could do this same trick with a standard Arduino Nano, or with a Nano Every.  In
//    the second case, if you have set up the Nano Every as shown in the diagrams that accompany
//    ensemble.ino, and have modified your Arduino files to allow Serial1, Serial2, and Serial3,
//    then you will need to set HARD_SERIAL to Serial1.
//
//    Additionally, pressing the TOP BUTTON will send both an ALL NOTES OFF and an ALL SOUNDS OFF to all its voices.
//
// 2. ARDUINO NANO EVERY.  The NANO EVERY has four serial ports, three of which can be used
//    for our purposes.  This can be used without an IMDI HEART.  Each serial port will output
//    a separate voice.  This means that we can have up to *3* voices (not 16).  To do this,
//    first set up your Nano Every as shown in the diagrams that accompany ensemble.ino, 
//    and modify your Arduino files to allow Serial1, Serial2, and Serial3.  Next, uncomment
//    MULTI_OUTPUT.  Make sure that NUM_VOICES is <= 3.  For information on setting up the
//    Arduino Nano Every,
//    See https://forum.arduino.cc/t/arduino-nano-every-access-to-4-serial/614358/2
//    See https://emalliab.wordpress.com/2021/12/16/arduino-nano-every-serial-ports/

//
// 3. ORIGINAL GRAINS IN COMBINATION WITH IMDI HEART.  In this case you do not have exposed
//    hardware ports and have to use software serial.  Software serial turns off interrupts
//    for too long when writing bytes, and as a result it may miss a read byte.  So expect 
//    note drops and general flakiness. I would only use this if your notes are very SLOW 
//    and SEPARATE from one another, or only for testing purposes.  Here, uncomment SOFT_SERIAL.  
//    Set things up as in #1 above.  Your RX socket will be AUDIO IN ("A") and your TX socket
//    will be DIGITAL OUT ("B").  No buttons or LEDs for you.

// You could probably get other Arduinos working, such as an Arduino Uno in the case of #1,
// or an Arduino Mega in the case of #1 or #2.

// LATENCY
//
// To do this magic, ENSEMBLE must parse entire MIDI messages and then emit them along multiple channels.  Thus there
// is a SMALL amount of latency until ENSEMBLE has received an entire MIDI message before it can send it out.  There also
// will be a SIGNIFICANT amount of latency if ENSEMBLE has to repeat this message multiple times to multiple voices.
// For example, if you have 10 voices, and ENSEMBLE receives a CC message, it will have to emit 15 separate CC messages, 
// one for each channel.








// MULTI_OUTPUT
//
// If this is commented, then we assume we are outputting to a SINGLE serial port output,
// SERIAL, and the different notes are being sent out on that output to three different
// channels, and other MIDI data is being copied to that output.  In this case, NUM_VOICES
// specifies how many channels we'll be distributing to.
//
// If this is uncommented, then we assume we are outputting to up to THREE separate
// serial port outputs, SERIAL1, SERIAL2, and SERIAL3, and sending copies of MIDI data
// to each one, with different notes distributed to different outputs on different channels
// (each output has its own channel.  In this case, NUM_VOICES dictates how many outputs
// (and thus channels) we have.

#define MULTI_OUTPUT


// HARD_SERIAL
//
// This dicates the serial device used when MULTI_OUTPUT is OFF.  Normally this is Serial,
// But if you have a Nano Every and you were using NOT in MULTI_OUTPUT mode, you might wish 
// to set this to Serial1 (don't set to Serial2 or Serial3, it conflicts with the button/LED code).

#define HARD_SERIAL Serial


// GRAINS_V2
//
// This indicates that my GRAINS V2 prototype is being used.  Since I'm presently the only person
// who ones a GRAINS V2, you probably comment this out!

// #define GRAINS_V2


// SOFT_SERIAL
//
// The purpose of this is to allow ensemble.ino to be tested (not used for real) on a standard GRAINS.
// If this is uncommented, then we will use the NeoSWSerial package instead of hardware serial.
// This only works if MULTI_OUTPUT is **OFF** and is very flakey -- you'll get stuck notes because
// soft serial is too slow.  The issue is that writing a byte in soft serial requires temporarily
// disabling the interrupts, and this can in turn causes incoming bytes to be missed and not read.
//
// If we are doing soft serial, then the RX (MIDI INPUT) socket is AUDIO IN ("A"), and the
// TX (MIDI OUTPUT) socket is DIGITAL OUT ("D").

// #define SOFT_SERIAL


// NUM_VOICES
//
// This dictates how large the polyphony is.  You can have up to 16 voices.  If you're doing
// MULTI_OUTPUT, you can only have 3 voices because the Arduino Nano Every only has three available
// serial ports.

#define NUM_VOICES 3				// should be no more than 3 if you're chosing MULTI_OUTPUT


// INCOMING_CHANNEL
//
// This indicates what the incoming MIDI channel is.  It can be any value 0...15, which represent
// MIDI channels 1 through 16, or it can be OMNI, which means any channel.

#define INCOMING_CHANNEL 0


// MIN_OUTGOING_CHANNEL
//
// This indicates what the outgoing MIDI channels will be.  MIN_OUTGOING_CHANNEL is the
// lowest such channel, and is a value 0...15, representing MIDI channels 1 through 16.
// The set of outgoing channels will be MIN_OUTGOING_CHANNEL through 
// MIN_OUTGOING_CHANNEL + NUM_VOICES.  No channel may be a value
// higher than 15 (representing channel 16).  For example, if you have 3 voices, your
// MIN_OUTGOING_CHANNEL must be <= 13.

#define MIN_OUTGOING_CHANNEL 0











/// LEFT SOCKETS IN ORDER
#define CV_POT_IN1    A2
#define CV_POT_IN2    A1
#define CV_IN3        A3
#define CV_AUDIO_IN   A4			// Used for soft serial in
#define CV_IN5	      A5

/// ADDITIONAL INPUTS
#define CV_POT3       A0
#define CV_SW1		  3				// Has no pull down resistor.  Use as INPUT_PULLUP.  *NEVER* use as OUTPUT, will destroy it.
#define CV_SW2		  4				// Has no pull down resistor.  Use as INPUT_PULLUP.  *NEVER* use as OUTPUT, will destroy it.
#define CV_SW3		  7				// Has no pull down resistor.  Use as INPUT_PULLUP.  *NEVER* use as OUTPUT, will destroy it.

/// RIGHT SOCKETS IN ORDER
#define CV_AUDIO_OUT  9				// or 11 if GRAINS
#define CV_D10		  10			// Second from top
#define CV_GATE_OUT   8				// "D", used for soft serial out
#define CV_RX		  0				// UART
#define CV_TX		  1				// UART
#define CV_LED_1	  2				// "D2" output, third from bottom
#define CV_LED_2	  5				// "D5" output, second from bottom.  Appears to go through a filter
#define CV_LED_3	  6				// "D6" output, bottom.  Appears to go through a filter

/// ADDITIONAL PINOUTS
#define RANDOM_1      A6
#define RANDOM_2      A7			// Used for blank Serial Pin




#ifdef MULTI_OUTPUT

struct noteDistributor distributor;
struct midiParser parser;
struct midiEmitter emitter1;
struct midiEmitter emitter2;
struct midiEmitter emitter3;
struct midiEmitter* emitters[3];

extern void noteOn(struct midiParser* parser, unsigned char note, unsigned char velocity)
	{
  Serial.println(note);
	uint8_t channel = allocateChannel(&distributor, note);
	if (channel == DISTRIBUTOR_NO_CHANNEL)
		{
		channel = oldestChannel(&distributor, true);
		emitNoteOffVel(emitters[channel], stealChannel(&distributor, note), velocity, channel);
		}
	emitNoteOn(emitters[channel], note, velocity, channel);
	}
	
extern void noteOff(struct midiParser* parser, unsigned char note, unsigned char velocity)
	{
	uint8_t channel = deallocateChannel(&distributor, note);
	if (channel != DISTRIBUTOR_NO_CHANNEL)
		{
		emitNoteOffVel(emitters[channel], note, velocity, channel);
		}
	}

extern void polyAftertouch(struct midiParser* parser, unsigned char note, unsigned char value)	
	{
	uint8_t channel = getChannel(&distributor, note);
	if (channel != DISTRIBUTOR_NO_CHANNEL)
		{
		emitAftertouch(emitters[channel], value, channel);
		}
	}
	
extern void pc(struct midiParser* parser, unsigned char program, unsigned char bankMSB, unsigned char bankLSB)
	{
	for(uint8_t i = 0; i < NUM_VOICES; i++)
		{
		emitBankAndPC(emitters[i], program, bankMSB, bankLSB, i);
		}
	}
	
void resetDistributor()
	{
		for(uint8_t i = distributor.minChannel; i <= distributor.maxChannel; i++)
			{
			uint8_t note = getNote(&distributor, i);
			if (note != DISTRIBUTOR_NO_NOTE)
				{
				emitNoteOff(emitters[i - 1], note, i);				// FIXME is this safety precaution necessary
				}
			}
		initializeDistributor(&distributor, distributor.minChannel, distributor.maxChannel);
	}
	
extern void cc(struct midiParser* parser, unsigned char parameter, unsigned char value)
	{
	if (parameter == 120 ||		// All Sound Off
		parameter == 123)		// All Notes Off
		{
		resetDistributor();
		}
		
	for(uint8_t i = 0; i < NUM_VOICES; i++)
		{
		emitCC(emitters[i], parameter, value, i);
		}
	}

extern void bend(struct midiParser* parser, SIGNED_16_BIT_INT value)
	{
	for(uint8_t i = 0; i < NUM_VOICES; i++)
		{
		emitBend(emitters[i], value, i);
		}
	}

extern void aftertouch(struct midiParser* parser, unsigned char value)
	{
	for(uint8_t i = 0; i < NUM_VOICES; i++)
		{
		emitAftertouch(emitters[i], value, i);
		}
	}

extern void songPositionPointer(struct midiParser* parser, UNSIGNED_16_BIT_INT value)
	{
	for(uint8_t i = 0; i < NUM_VOICES; i++)
		{
		emitSongPositionPointer(emitters[i], value);
		}
	}
	
extern void songSelect(struct midiParser* parser, unsigned char song)
	{
	for(uint8_t i = 0; i < NUM_VOICES; i++)
		{
		emitSongSelect(emitters[i], song);
		}
	}

extern void clockStop(struct midiParser* parser)
	{
	for(uint8_t i = 0; i < NUM_VOICES; i++)
		{
		emitClockStop(emitters[i]);
		}
	}
	
extern void clockStart(struct midiParser* parser)
	{
	for(uint8_t i = 0; i < NUM_VOICES; i++)
		{
		emitClockStart(emitters[i]);
		}
	}
	
extern void clockContinue(struct midiParser* parser)
	{
	for(uint8_t i = 0; i < NUM_VOICES; i++)
		{
		emitClockContinue(emitters[i]);
		}
	}
	
extern void clockPulse(struct midiParser* parser)
	{
	for(uint8_t i = 0; i < NUM_VOICES; i++)
		{
		emitClockPulse(emitters[i]);
		}
	}
	
extern void sysex(struct midiParser* parser, unsigned char* buffer, unsigned char len, unsigned char status)
	{
	uint8_t F7 = 0xF7;
	uint8_t F0 = 0xF0;
	for(uint8_t i = 0; i < NUM_VOICES; i++)
		{
		if (status == STATUS_SYSEX_COMPLETE)
			{
			emitSysex(emitters[i], &F0, 1);
			if (len != 0) emitSysex(emitters[i], buffer, len);
			emitSysex(emitters[i], &F7, 1);
			}
		else if (status == STATUS_SYSEX_START)
			{
			emitSysex(emitters[i], &F0, 1);
			if (len != 0) emitSysex(emitters[i], buffer, len);
			}
		else if (status == STATUS_SYSEX_END)
			{
			if (len != 0) emitSysex(emitters[i], buffer, len);
			emitSysex(emitters[i], &F7, 1);
			}
		else if (status == STATUS_SYSEX_INCOMPLETE)
			{
			if (len != 0) emitSysex(emitters[i], buffer, len);
			}
		}
	}
	
void setup()
	{
    Serial.begin(115200);
	Serial1.begin(31250);
	Serial2.begin(31250);
	Serial3.begin(31250);
	initializeParser(&parser, INCOMING_CHANNEL, 0, true);
	initializeEmitter(&emitter1, 0);
	initializeEmitter(&emitter2, 1);
	initializeEmitter(&emitter3, 2);
	initializeDistributor(&distributor, MIN_OUTGOING_CHANNEL, MIN_OUTGOING_CHANNEL + NUM_VOICES - 1);
	emitters[0] = &emitter1;
	emitters[1] = &emitter2;
	emitters[2] = &emitter3;
	}

void loop()
	{
	if (Serial1.available())
		{
		parseMidi(&parser, Serial1.read());
		}
	}
	
extern void emitMidi(struct midiEmitter* emitter, unsigned char byte)
	{
	int tag = getEmitterTag(emitter);
	if (tag == 0)
		{
		Serial1.write(byte);
		}
	else if (tag == 1)
		{
		Serial2.write(byte);
		}
	else if (tag == 2)
		{
		Serial3.write(byte);
		}
	}


#else


#ifdef SOFT_SERIAL
#include "NeoSWSerial.h"   			// you have to install this via the library manager
#define BLANK_SERIAL	  5			// Blank Serial Pin
#define PIN_UNUSED 255
#define CV_GATE_OUT   8     		// Digital Out
NeoSWSerial softSerial(CV_AUDIO_IN, CV_GATE_OUT, PIN_UNUSED);
#endif

struct noteDistributor distributor;
struct midiParser parser;
struct midiEmitter emitter;
#ifdef GRAINS_V2
uint8_t resetButton = 0;
uint32_t resetTime = 0;
#define RESET_INTERVAL 250
uint8_t leds[3] = { CV_LED_1, CV_LED_2, CV_LED_3 };
#endif

void ledOn(uint8_t channel)
	{
#ifdef GRAINS_V2
	if (channel - MIN_OUTGOING_CHANNEL < 3)
		{
		digitalWrite(leds[channel - MIN_OUTGOING_CHANNEL], 1);
		}
#endif
	}

void ledOff(uint8_t channel)
	{
#ifdef GRAINS_V2
	if (channel - MIN_OUTGOING_CHANNEL < 3)
		{
		digitalWrite(leds[channel - MIN_OUTGOING_CHANNEL], 0);
		}
#endif
	}

extern void noteOn(struct midiParser* parser, unsigned char note, unsigned char velocity)
	{
	uint8_t channel = allocateChannel(&distributor, note);
	if (channel == DISTRIBUTOR_NO_CHANNEL)
		{
		channel = oldestChannel(&distributor, true);
		emitNoteOffVel(&emitter, stealChannel(&distributor, note), velocity, channel);
		ledOff(channel);
		}
	emitNoteOn(&emitter, note, velocity, channel);
	ledOn(channel);
  	}
	
extern void noteOff(struct midiParser* parser, unsigned char note, unsigned char velocity)
	{
	uint8_t channel = deallocateChannel(&distributor, note);
	if (channel != DISTRIBUTOR_NO_CHANNEL)
		{
		emitNoteOffVel(&emitter, note, velocity, channel);
		ledOff(channel);
		}
	else
		{
		//digitalWrite(leds[2], 1);
		}
	}

extern void polyAftertouch(struct midiParser* parser, unsigned char note, unsigned char value)	
	{
	uint8_t channel = getChannel(&distributor, note);
	if (channel != DISTRIBUTOR_NO_CHANNEL)
		{
		emitAftertouch(&emitter, value, channel);
		}
	}
	
extern void pc(struct midiParser* parser, unsigned char program, unsigned char bankMSB, unsigned char bankLSB)
	{
	for(uint8_t i = 0; i < NUM_VOICES; i++)
		{
		emitBankAndPC(&emitter, program, bankMSB, bankLSB, i);
		}
	}

void resetDistributor()
	{
		for(uint8_t i = distributor.minChannel; i <= distributor.maxChannel; i++)
			{
			uint8_t note = getNote(&distributor, i);
			if (note != DISTRIBUTOR_NO_NOTE)
				{
				emitNoteOff(&emitter, note, i);				// FIXME is this safety precaution necessary
				}
			}
		initializeDistributor(&distributor, distributor.minChannel, distributor.maxChannel);
	}
	
extern void cc(struct midiParser* parser, unsigned char parameter, unsigned char value)
	{
	if (parameter == 120 ||		// All Sound Off
		parameter == 123)		// All Notes Off
		{
		resetDistributor();
		}
		
	for(uint8_t i = 0; i < NUM_VOICES; i++)
		{
		emitCC(&emitter, parameter, value, i);
		}
	}

extern void bend(struct midiParser* parser, SIGNED_16_BIT_INT value)
	{
	for(uint8_t i = 0; i < NUM_VOICES; i++)
		{
		emitBend(&emitter, value, i);
		}
	}

extern void aftertouch(struct midiParser* parser, unsigned char value)
	{
	for(uint8_t i = 0; i < NUM_VOICES; i++)
		{
		emitAftertouch(&emitter, value, i);
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
#ifdef SOFT_SERIAL
	softSerial.begin(31250);
#else
	HARD_SERIAL.begin(31250);
#endif
	initializeParser(&parser, INCOMING_CHANNEL, 0, true);
	initializeEmitter(&emitter, 0);
	initializeDistributor(&distributor, MIN_OUTGOING_CHANNEL, MIN_OUTGOING_CHANNEL + NUM_VOICES - 1);
#ifdef GRAINS_V2
	pinMode(CV_LED_1, OUTPUT);
	pinMode(CV_LED_2, OUTPUT);
	pinMode(CV_LED_3, OUTPUT);
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
		resetDistributor();
		}
#endif
		
#ifdef SOFT_SERIAL
	if (softSerial.available())
		{
		parseMidi(&parser, softSerial.read());
		}
#else
	if (HARD_SERIAL.available())
		{
		parseMidi(&parser, HARD_SERIAL.read());
		}
#endif
	}
	
extern void emitMidi(struct midiEmitter* emitter, unsigned char byte)
	{
#ifdef SOFT_SERIAL
	softSerial.write(byte);
#else
	HARD_SERIAL.write(byte);
#endif
	}
	

#endif
