/// Copyright 2024 by Sean Luke
/// Open Source 
/// Licensed under the Apache 2.0 License

/// Version 0.1: 	"It Compiles, that's all"

#ifndef PARSEMIDI_H
#define PARSEMIDI_H

#include "miditypes.h"


////// MIDI PARSER

/// This parser can parse MIDI note data of all kinds, including 14-bit and 7-bit CC, NRPN, and RPN,
/// as well as all kinds of non-note data, including system exclusive messages up to 254 bytes in
/// length not including the F0 and F7.
///
/// Each parser is a struct containing state data.  A single parser is designed to parse data for
/// a specific channel, or all data (OMNI).  You can make as many parsers as you like, for different
/// channels or different devices.
///
/// The MIDI Parser and MIDI Emitter can be used and compiled independently. 
///
/// Parsing happens at two levels.  At the first level there is a simple state machine to handle multi-byte
/// messages, as well as system exclusive.  At the second level there is an additional state machine to
/// handle assembly of 14-bit CC, RPN, and NRPN messages.
///
/// The parser is complicated because it is designed for you to turn off FEATURES you don't want
/// to respond to.  This will in turn make the parser smaller in total memory and code size, as
/// well as slightly faster in some situations. 
///
/// Features you can turn OFF include different kinds of MIDI messages (such as PITCH BEND),
/// System Exclusive, RPN and NRPN, and 14-bit CC.
///
/// To use the MIDI Parser you must set a few things.
///
/// 0. You may need to properly define SIGNED_16_BIT_INT and UNSIGNED_16_BIT_INT according to your
///    platform, see miditypes.h
///
/// 1. In THIS file (parsemidi.h), you must uncomment the FEATURES that you permit, or comment
///    out ones to disable them.
///
/// 2. You have to define CALLBACKS for each of the features you have
///    turned on.  For example, if one of your FEATURES is ALLOW_SYSTEM_EXCLUSIVE, then
///    you have to implement the function   extern void sysex(unsigned char* buffer, unsigned char len);
///
/// 3. If one of your FEATURES is ALLOW_SYSTEM_EXCLUSIVE, then in this file you might
///    want to customize the SYSTEM_EXCLUSIVE_BUFFER_SIZE, up to 254.






// Don't mess with these constants
#define OMNI 16											// Used when we listen to all channels
#define STATUS_NORMAL	0								// Plain value for NRPN/RPN/High-Res CC
#define STATUS_BARE_MSB 1								// Value only contains the MSB (so far), LSB assumed to be 0
#define STATUS_NRPN_INCREMENT 3							// Value is amount to increment by [only for NRPN/RPN]
#define STATUS_NRPN_DECREMENT 4							// Value is amount to decrement by [only for NRPN/RPN]





// FEATURES TO PERMIT
// Comment these out to turn them off.  
// NOTE: in order to ALLOW_HIGH_RES_CC or ALLOW_RPN_NRPN, or both, you must also ALLOW_CC

#define ALLOW_NOTES
// 	#define ALLOW_POLY_AT
// 	#define ALLOW_CC
// 	#define ALLOW_HIGH_RES_CC				// You must ALSO call setHighResUsed(...) to state which ones are high-res
// 	#define ALLOW_RPN_NRPN
// 	#define ALLOW_PC
// 	#define ALLOW_AT
// 	#define ALLOW_BEND
// 	#define ALLOW_SYSTEM_EXCLUSIVE
// 	#define ALLOW_MIDI_TIME_CODE_QUARTER_FRAME
// 	#define ALLOW_SONG_POSITION_POINTER
// 	#define ALLOW_SONG_SELECT
// 	#define ALLOW_TUNE_REQUEST
// 	#define ALLOW_CLOCK
// 	#define ALLOW_ACTIVE_SENSING
// 	#define ALLOW_SYSTEM_RESET





// SYSTEM EXCLUSIVE BUFFER SIZE
// You need to tune this if you have turned on ALLOW_SYSTEM_EXCLUSIVE, otherwise ignore this.
// The data size does not include the F0 nor any F7.  These are not included in the buffer that is passed to you.
// You will not receive a callback until the message is complete -- that is, we do not do the Casio CZ broken sysex thing.
// If a system exclusive message exceeds SYSTEM_EXCLUSIVE_BUFFER_SIZE in length, then it is ignored.

#define SYSTEM_EXCLUSIVE_BUFFER_SIZE 64		// can be no larger than MAX_SYSTEM_EXCLUSIVE_BUFFER_SIZE (254, defined in miditypes.h)





/// THE PARSER STATE MACHINE STRUCTURE
// Don't fool with this.  Treat it as a black box.  You can get the channel, current channel, and tag
// by calling getMIDIChannel(), getCurrentMIDIChannel, and getTag() -- see below
typedef struct midiParser
	{
	unsigned char tag;								// The parser's tag.  This is a byte for you to use however you like.
	unsigned char channel;							// The channel to which we are meant to listen, or OMNI 
	unsigned char currentChannel;                  // The current channel, which may vary if we are OMNI
	unsigned char status;                           // The status byte, stripped of channel
	unsigned char data1;                            // The first data byte, if any
	unsigned char unvoicedMessages;				// Does this parser respond to unvoiced messages?

#ifdef ALLOW_HIGH_RES_CC
	unsigned char highResUsed[4];		// 32-bit bitfield, bit indicates whether high-res is being used for that CC
	unsigned char highResMSB[32];		// Low 7 bits is current value MSB.  High bit is whether the MSB has been set yet.
#endif

#ifdef ALLOW_RPN_NRPN
	unsigned char nrpnParamMSB;			// Low 7 bits is current param MSB.  High bit is whether the MSB has been set yet.
	unsigned char nrpnParamLSB;			// Low 7 bits is current param LSB.	 High bit is whether the LSB has been set yet.
	unsigned char nrpnValueMSB;			// Low 7 bits is current value MSB.  High bit is whether the MSB has been set yet.
	unsigned char rpn;					// Is this RPN instead?
#endif

#ifdef ALLOW_SYSTEM_EXCLUSIVE
	unsigned char systemExclusiveBufferLen;			// The length of our system exclusive buffer
	unsigned char systemExclusiveBuffer[SYSTEM_EXCLUSIVE_BUFFER_SIZE];	// System exclusive buffer, not including F0 or F7
#endif
	} midiParser;







// CALLBACKS TO IMPLEMENT
// You must provide callbacks below for all features you permitted.  


#ifdef ALLOW_NOTES
// note goes 0 ... 127, velocity goes 0 ... 127, channel goes 0...15
// IMPORTANT: noteOn will never have a velocity of 0
extern void noteOn(midiParser* parser, unsigned char note, unsigned char velocity);
extern void noteOff(midiParser* parser, unsigned char note, unsigned char velocity);
#endif

#ifdef ALLOW_CLOCK
extern void clockStop(midiParser* parser);
extern void clockStart(midiParser* parser);
extern void clockContinue(midiParser* parser);
extern void clockPulse(midiParser* parser);
#endif

#ifdef ALLOW_BEND
// value goes -8192 ... +8191, channel goes 0 ... 15
extern void bend(midiParser* parser, SIGNED_16_BIT_INT value);
#endif

#ifdef ALLOW_PC
// program goes 0 ... 127, channel goes 0 ... 15
extern void pc(midiParser* parser, unsigned char program);
#endif

#ifdef ALLOW_CC
// This will only be called for non-high-res, non-NRPN, non-RPN CCs
// This will NOT be called for any CCs you have previously registered as high resolution via setHighResUsed(...)
// parameter goes 0 ... 127, value goes 0 ... 127, channel goes 0 ... 15
extern void cc(midiParser* parser, unsigned char parameter, unsigned char value);
#endif

#ifdef ALLOW_RPN_NRPN
// parameter goes 0...16383, value goes 0...16383, rpn is true or false, channel goes 0...15
// status is STATUS_NORMAL, STATUS_BARE_MSB, STATUS_NRPN_INCREMENT, or STATUS_NRPN_DECREMENT 
// STATUS_NORMAL: 'value' holds the value to set.
// STATUS_BARE_MSB: 'value' is determined only by the MSB.  No LSB has arrived yet.  Thus 'value' = MSB * 128 + 0.
// STATUS_NRPN_INCREMENT: 'value' holds the amount to increment the current value by.  Usually it is 1.
// STATUS_NRPN_DECREMENT: 'value' holds the amount to decrement the current value by.  Usually it is 1.
extern void nrpn(midiParser* parser, UNSIGNED_16_BIT_INT parameter, UNSIGNED_16_BIT_INT value, unsigned char rpn, unsigned char status);
#endif

#ifdef ALLOW_HIGH_RES_CC
// This will be called for CC 6 and 38 (Data Entry) only if you have NOT turned on RPN/NRPN
// This will only be called for CCs you have previously registered as high resolution via setHighResUsed(...)
// parameter goes 0...31, value goes 0...16383, channel goes 0...15
// status is STATUS_NORMAL or STATUS_BARE_MSB 
// STATUS_NORMAL: 'value' holds the value to set.
// STATUS_BARE_MSB: 'value' is determined only by the MSB.  No LSB has arrived yet.  Thus 'value' = MSB * 128 + 0.
extern void highResCC(midiParser* parser, unsigned char parameter, UNSIGNED_16_BIT_INT value, unsigned char status);
#endif

#ifdef ALLOW_SYSTEM_EXCLUSIVE
// buffer does not include 0xF0 at start nor 0xF7 at end.  len will not exceed SYSTEM_EXCLUSIVE_BUFFER_SIZE (254)
extern void sysex(midiParser* parser, unsigned char* buffer, unsigned char len);
#endif

#ifdef ALLOW_AT
// value goes 0 ... 127, channel goes 0 ... 15
extern void aftertouch(midiParser* parser, unsigned char value);
#endif

#ifdef ALLOW_POLY_AT
// note goes 0 ... 127, value goes 0 ... 127, channel goes 0 ... 15
extern void polyAftertouch(midiParser* parser, unsigned char note, unsigned char value);
#endif

#ifdef ALLOW_SONG_POSITION_POINTER
// value goes 0 ... 16383
extern void songPositionPointer(midiParser* parser, UNSIGNED_16_BIT_INT value);
#endif

#ifdef ALLOW_SONG_SELECT
// song goes 0 ... 127
extern void songSelect(midiParser* parser, unsigned char song);
#endif

#ifdef ALLOW_MIDI_TIME_CODE_QUARTER_FRAME
// data goes 0...127
extern void midiTimeCodeQuarterFrame(midiParser* parser, unsigned char data);
#endif

#ifdef ALLOW_ACTIVE_SENSING
extern void activeSensing(midiParser* parser);
#endif

#ifdef ALLOW_SYSTEM_RESET
extern void systemReset(midiParser* parser);
#endif

#ifdef ALLOW_TUNE_REQUEST
extern void tuneRequest(midiParser* parser);
#endif





/// BUILDING AND CALLING THE PARSER
/// You can make multiple parsers: they are independent of one another.  However 

// Initializes a MIDI Parser.  You provide the parser and the channel it is meant to listen to to.
// The channel can be any of 0...15 or OMNI
// You can use the tag for anything you like: for example, if you have two MIDI devices, the tag might indicate the device
void initializeParser(midiParser* parser, unsigned char channel, unsigned char tag, unsigned char unvoicedMessages);

// Resets the MIDI parser.  You don't need to call this initially but you might
// call it if you need to reset the stream for some reason (very rare).
void resetParser(midiParser* parser);

// Updates the MIDI parser to reflect a new incoming byte.
void parseMidi(midiParser* parser, unsigned char c);

// Returns the parser's assigned MIDI channel
// Will be 0...15 or OMNI
unsigned char getParserMIDIChannel(midiParser* parser);

// Returns the parser's current MIDI channel: the channel of the latest incoming voice data
// This will be 0...15 or INVALID is there has been no valid voice data yet.
unsigned char getParserCurrentMIDIChannel(midiParser* parser);

// Returns the parser's tag
unsigned char getParserTag(midiParser* parser);

#if defined(ALLOW_HIGH_RES_CC)
// Sets whether High Resolution CC is used for the given parameter.  For example, if you call
// setHighResUsed(state, 3, 1), then param 3 will define the MSB and 35 will define the LSB of CC 3.
// Only the first 32 parameters (0...31) may be turned on or off.
// Note that CC 6 and 38 (Data Entry) will be ignored here if you have turned on RPN/NRPN, which use them instead.
void setHighResUsed(midiParser* state, unsigned char parameter, unsigned char on);
#endif



#endif