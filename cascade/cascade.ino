// Copyright 2025 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License

/// CASCADE
///
/// Cascade is a MIDI player which can play a song of up to about 6000 notes on GRAINS,
/// and more on certain other Arduino configurations.  Cascade is meant to run on the
/// AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// Cascade can store several thousand notes or other MIDI events and can replay them over
/// Modular MIDI.  You could attach it to a single MIDI module, such as the mco/1 or mcc/4,
/// or you can feed it into the IMDI HEART and break the MIDI into multiple channels for
/// multiple downstream devices.
///
/// BUG IN IMDI HEART
///
/// Cascade is particularly useful in combination with IMDI HEART: you feed Cascade's 
/// Modular MIDI into IMDI HEART's "IMDI" input. To do this, you must change ICN, IRT, 
/// and ISY so that IMDI HEART receives its MIDI from its IMDI input. However the menu
/// options for these three settings is mixed up. To set these to IMDI, you must instead 
/// set them to NONE. (NONE and IMDI are mixed up in the menu).
/// 
/// THE SONG.H FILE
///
/// To load MIDI into Cascade, you create a file called "song.h" and place it right next
/// to the file "cascade.ino".  This file should look like this:
///
///    #define BPM ...						// This is your speed in beat per minute.  It should be an integer > 0, such as 120.
///    #define TIMELENGTH ...				// This is the length the timestep array below.
///    #define LONGTYPE						// This line might not exist.  If it exists, then TIMETYPE is 32-bit.  If not, 16-bit.
///
///    // This little macro sets our integer type for the timestep array.
///    // It must be here.
///    #ifdef LONGTYPE
///    #define TIMETYPE uint32_t
///    #else
///    #define TIMETYPE uint16_t
///    #endif
///
///    // This is the array of timestamps for each MIDI message.  They're either 16-bit or 32-bit integers.
///    PROGMEM const TIMETYPE timestep[TIMELENGTH] = { ..., ..., ... }; 
///    
///    #define DATALENGTH ...				// This is the length of the data array below.  
///
///    // This array holds the bytestream of MIDI Messages.  There are exactlyt TIMELENGTH messages in total,
///    // but they can be various bytes in length each.
///    PROGMEM const uint8_t data[DATALENGTH] = { ..., ..., ... };
///
/// The [data] array is the bytes for all the MIDI Messages in order.  You can have any messages you like,
/// but you are not permitted to insert the single-byte System Realtime messages (such as Clock Pulse)
/// in the middle of some message, which is normally permitted by MIDI but which is too complex for
/// Cascade's trivial parser.  Most DAWs don't do it, but it might happen.
///
/// Cascade comes with a "song.h" file which is my submission to AETHERWAVES vol 7.  It's too big to fit
/// on GRAINS, but works well on the Arduino Nano Every.
/// 
///
/// USING DAVE AND SEQ
/// 
/// The Dave Java program, part of the Dave GRAINS firmware in my firmware collection, can be used to convert
/// a standard MIDI file into a "song.h" file.  You can download the Dave Java program here:
///
///		https://github.com/eclab/grains/tree/main/dave/java
///
/// See Dave's menu option "File -> Convert .mid file to Cascade .txt"
///
/// If you need a sequencer which can output to MIDI files, try Seq, my sequencer.
/// 
///    https://github.com/eclab/seq
///
///
/// SOME NOTES ON TIMING AND MUSIC SPEED
///
/// The [timestep] array are timestamps for when the MIDI Messages should appear.  The timestamps are in 
/// TICKS, where there are a certain number of ticks per beat.  At present this value is is 192, which
/// is common; but your DAW may output something else.  In the "song.h" file you also define BPM, 
/// which is the beats permit (the speed) of your song.  Commonly BPM might be 120.  Cascade converts 
/// time (in ms) into ticks with the following equation:
///
///    	return (time * BPM) / (60000 / TICKS_PER_BEAT);
/// 
/// If your DAW has a TICKS PER BEAT of, say, 360, then you may need to change the #define for
/// TICKS_PER_BEAT.  By default it is 192.
///
/// Note that if BPM=120, the maximum
/// possible value Cascade can compute for the time is 2^32 / BPM = 2^32 / 120 = 4294967295 / 120.
/// That value is 35791394 milliseconds, or about 35791 seconds, or 596 minutes, or about 10 hours
/// before Cascade overflows in its time computation and things go haywire.  If you increase your BPM 
/// significantly, the number of hours will drop correspondingly.
///
/// Also remember that MIDI runs at 3125 bytes per second.  A typical MIDI message is 3 byes long,
/// so you can at most have 1000 messages per second.  A note needs a note-on and a note-off message,
/// so this means you can play about 500 notes per second.  If you increase your BPM dramatically,
/// or have very heavy MIDI traffic, you may force cascade to output more notes than it is capable.
///
///
/// SOME NOTES ON MEMORY
///
/// Cascade can represent timestamps with either 16-bit integers or 32-bit integers.  With 16-bit 
/// integers, a typical MIDI message drops from (roughly) 8 bytes to 6 bytes, giving you more memory
/// in which to store your song.  But this means that your maximum timstamp must be no larger than 
/// 65535 ticks.  If it exceeds this, you have to move to 32-bit integer timestamps.
///
/// Other arduinos (such as the Arduino Nano Every, or the Arduino Mega) have more memory available
/// than GRAINS (which is the same memory as a Nano or an Uno).
///
///
/// CASCADE ON THE GRAINS VS THE NANO EVERY
///
/// By default Cascade is set up to run on the Nano Every.  You can easily set up a Nano Every to
/// run on your BRAEDBOARD, and it's cheap to buy.  For instructions on setup, see here:  
/// https://github.com/eclab/grains/tree/main/v2/info
///
/// The Nano Every uses Serial1 as it MIDI output.
///
/// Alternatively you can use Cascade on a GRAINS, where DIGITAL OUT (D) is the MIDI Output. 
/// To do this you need to change two things:
///
///		- Unzip the files "NeoSWSerial.h" and "NeoSWSerial.cpp" located in this directory
/// 	- Uncomment the following #define:

// #define USE_SOFT_SERIAL 

/// This sets up Cascade to use Software Serial.  Be warned that this may drop a byte here
/// or there but it should work okay.  
///
/// BTW if you have a Grains V2 (and presently I am the only person in the world outside of
/// Tangible Waves, so that's unlikely) you could use it as well.  Serial1 is the MIDI output.
/// In this case, you'd instead uncomment the following #define:

// #define GRAINS_V2 

///
/// 
/// CONTROLLING CASCADE
///
/// Cascade has a trigger which starts the sequence running or resets it while it is running.
/// Cascade also has a trigger which stops a sequence.  When a sequence is stopped, Cascade
/// will send ALL NOTES OFF to all channels, and a NOTE OFF (actually a NOTE ON with velocity 0) 
/// to every single pitch on channel 0.
///
///
/// CONFIGURATION
///
/// IN 1            
/// IN 2            
/// IN 3            Stop
/// AUDIO IN (A)    Start / Reset
/// AUDIO OUT       
/// DIGITAL OUT (D) MIDI Output (on GRAINS)
///
/// POT 1           
///
/// POT 2           
///
/// POT 3   
///
/// Serial 1		MIDI Output (on Nano Every)
/// Serial RX		MIDI Output (on Grains V2)        




#include "emitmidi.c"
#ifdef USE_SOFT_SERIAL
#include "NeoSWSerial.h"
#endif

#define CV_POT_IN1    A2    
#define CV_POT_IN2    A1    
#define CV_POT3       A0    
#define CV_IN3        A3    // Stop
#define CV_AUDIO_IN   A4    // Start / Reset
#define CV_AUDIO_OUT  9     
#define CV_GATE_OUT   8     // MIDI Input (on GRAINS)
#define RANDOM_PIN    A5

#define BLANK_SERIAL  5		// Blank Serial Pin
#define PIN_UNUSED 	  255

// This is our timestamp resolution.  The time is in BEATS, not in SECONDS.
#define TICKS_PER_BEAT (192)

// MIDI Serial baud rate
#define MIDI_RATE 31250

#ifdef USE_SOFT_SERIAL
NeoSWSerial softSerial(BLANK_SERIAL, CV_GATE_OUT, PIN_UNUSED);
#endif

// Where are we in the TIMESTEP ARRAY
uint16_t timePos = 0;
// Where are we in the DATA ARRAY
uint16_t dataPos = 0;

#include "song.h"

// At this point thanks to song.h, TIMETYPE and LONGTYPE have been defined.


// Return the timestamp of the message at position POS in the TIMESTEP ARRAY
TIMETYPE getTime(uint16_t pos)
	{
#ifdef LONGTYPE
	return pgm_read_dword_near(&timestep[pos]);
#else
	return pgm_read_word_near(&timestep[pos]);
#endif
	}

// Return the data byte at position POS in the DATA ARRAY
uint8_t getData(uint16_t pos)
	{
	return pgm_read_byte_near(&data[pos]);
	}


// Return the timestamp, in MIDI Ticks, corresponding to the current TIME
// We provide the START TIME, which was the millis() when we began emitting MIDI
uint32_t getMIDITicks(uint32_t startTime)
	{
	uint32_t time = millis() - startTime;
	
	// ticks = time in ms * [1 sec / 1000 ms] * [1 min / 60 sec] * [BPM beats / min] * [TICKS_PER_BEAT ticks / beat]
	// ticks = time in ms * BPM beats * TICKS_PER_BEAT ticks / 1000 ms / 60 sec
	// ticks = time in ms * BPM * TICKS_PER_BEAT / 60000
	// If TICKS_PER_BEAT is 192, then 60000 / 192 is approximately 312,
	// So we say:
	
	return (time * BPM) / (60000 / TICKS_PER_BEAT);
	}

// Write a byte using one of several methods depending on the Arduino type used
void writeByte(uint8_t byte)
	{
//	Serial.println(byte);
#ifdef USE_SOFT_SERIAL
	softSerial.write(byte);
#elifdef GRAINS_V2
	Serial.write(byte);
#else
	Serial1.write(byte);
#endif
	}

// Turn off all notes, typically needed when we have prematurely quit.
// This algorithm is suboptimal but probably sufficient in most cases.
void allNotesOff()
	{
	// Let's write ALL NOTES OFF to all channels in the hopes that poly modules respond to it
	for(uint8_t i = 0; i < 16; i++)
		{
		writeByte(0xB0 + i);
		writeByte(123);
		writeByte(0);
		}

	// Not many modules implement ALL NOTES OFF.  So we have to do it manually for every note, which is quite slow.  :-(
	// But we'll assume that these modules are OMNI so we can just write to channel 1
	for(uint8_t i = 0; i < 128; i++)
		{
		writeByte(0x90);
		writeByte(i);
		writeByte(0);
		}
	}

// Emits the message STARTING at position p.
// Returns the revised position p of the next message.
uint16_t emit(uint16_t p)
	{
//	Serial.println("emit");
	uint8_t started = false;		// have we emitted the first byte of the message yet?
	uint16_t i;						// what is the next byte to emit?
	
	for(i = p; i < DATALENGTH; i++)
		{
		uint8_t s = getData(i);

		if (!started && s < 0x80) 	// uh oh, not a status byte, we have bad data initially
			continue;				// THIS IS AN ERROR

		if (started && s >= 0x80)  	// we're at the next status byte, so we're done
			return i;
			
		started = true;
		writeByte(s);
		}
	return i;
	}


// Indicates whether we have started running 
uint8_t started = false;
// Indicates the millis() at which we started running
uint32_t startTime = 0;

#define TRIGGER_HIGH (512)

void loop()
    {
    uint16_t trigger = analogRead(CV_AUDIO_IN);
    
    if (trigger >= TRIGGER_HIGH && !started)		// gate was just raised, reset
    	{
    	started = true;
    	dataPos = 0;
    	timePos = 0;
    	startTime = millis();
    	}
    else if (dataPos >= DATALENGTH || timePos >= TIMELENGTH) // || analogRead(CV_IN3) >= TRIGGER_HIGH)		// We quit, or ran out of data or timestamps
    	{
    	if (started)
    		{
    		allNotesOff();
    		}
    	started = false;
    	return;
    	}
    else if (!started)								// We haven't started yet
    	{
    	return;
    	}
    else
    	{
    	// we're running
    	}
    	
	if (getMIDITicks(startTime) >= getTime(timePos)) 	// Do we need to emit a message?
		{
		timePos++;	
		dataPos = emit(dataPos);
		}
   }
    
    
void setup()
    {
//    Serial.begin(9600);
#ifdef USE_SOFT_SERIAL
    softSerial.begin(MIDI_RATE);
#elifdef GRAINS_V2
	Serial.begin(MIDI_RATE);
#else
	Serial1.begin(MIDI_RATE);
#endif
	pinMode(CV_GATE_OUT, OUTPUT);		// just in case?
    }
