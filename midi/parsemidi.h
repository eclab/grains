/// Copyright 2024 by Sean Luke
/// Open Source 
/// Licensed under the Apache 2.0 License

/// Version 0.3:        "Passes Tests, Modular Stuff Added"


#ifndef PARSEMIDI_H
#define PARSEMIDI_H

#include "miditypes.h"


////// MIDI PARSER

/// This parser can parse MIDI note data of all kinds, including 14-bit and 7-bit CC, NRPN, and RPN,
/// as well as all kinds of non-note data, including combined PC/BankSelect messages, and also 
/// including system exclusive messages up to 254 bytes in length not including the F0 and F7.
///
/// Each parser is a struct containing state data.  A single parser is designed to parse data for
/// a specific channel, or all data (OMNI).  You can also set up a parser to listen to a channel in an
/// MPE Zone.  You can make as many parsers as you like, for different channels or different devices.
///
/// When you receive an incoming MIDI byte, you call parseMIDI(...) on any parser that you think would
/// be interested in that byte.
///
/// There is also a MIDI Emitter.  The MIDI Parser and MIDI Emitter can be used and compiled independently. 
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
/// Note that if you have turned on ALLOW_PC and ALLOW_CC, then Bank Select will be processed with pc(...)
/// and will NOT be sent to cc(...) or to highResCC(...).  If you do not turn on ALLOW_PC, but you turn 
/// on ALLOW_CC, then Bank Select will be treated like a regular CC message and will be sent to cc(...) 
/// or to highResCC(...) as appropriate.
///
///
/// To use the MIDI Parser you must set a few things.
///
/// 0. You may need to properly define SIGNED_16_BIT_INT and UNSIGNED_16_BIT_INT according to your
///    platform, see miditypes.h
///
/// 1. In THIS file (parsemidi.h), you must uncomment the FEATURES that you permit, or comment
///    out ones to disable them.
///
/// 2. Somewhere you have to implement CALLBACKS for each of the features you have
///    turned on.  For example, if one of your FEATURES is ALLOW_SYSTEM_EXCLUSIVE, then
///    you have to implement the function:
///
///            extern void sysex(unsigned char* buffer, unsigned char len, unsigned char status);
///
/// 3. If one of your FEATURES is ALLOW_SYSTEM_EXCLUSIVE, then in this file (parsemidi.h) you might
///    want to customize the SYSTEM_EXCLUSIVE_BUFFER_SIZE, up to 254.
///
/// 4. If one of your FEATURES is ALLOW_MPE, then you probably want to set the parser's MPE
///    Master Channel.  This causes the parser to respond to messages arriving on that channel
///    in addition to its regular channel.
///
/// A parser is INITIALIZED by calling initializeParser(...) on it.  You provide the channel it listens
/// to, or OMNI, plus a tag that you can use for whatever purpose, and also indicate whether it should
/// respond to non-voice messages (like clock or sysex).
///
/// Then whenever data bytes arrive, you call parseMidi(...) on the parser to process them.  Processing 
/// a data byte does not consume it: you can call parseMidi(...) on multiple parsers with the same data
/// byte.
///
/// If your device responds to multiple MIDI channels (perhaps it's multitimbral?  MPE?), you'd typically
/// make multiple parsers, one per channel.  Then when a data byte comes in, you pass it to parseMidi(...)
/// for each parser in turn.  You may wish to set only one of them to respond to non-voice data so that
/// you don't get those functions called multiple times (once per parser).
///
/// If your device responds to MPE, either high zone, low zone, or both, and you have turned on ALLOW_MPE,
/// you can set the master channel on certain parsers in those zones, which causes them to respond to
/// messages sent to their master channels as well.  Parsers outside the zones shouldn't have a master
/// channel.  Note that RPN messages, notably the MPE RPN message, will wind up being processed by every
/// parser that responds to that master channel: you'll have to watch for that. Depending on how your
/// MPE system works you may or may not wish to set up all the parsers to respond to non-voice messages
/// or only one of them.
///
///
/// WEAKNESSES OF THIS CODE
///
/// 1. If you turn on ALLOW_SYSTEM_EXCLUSIVE, every single parser will have its own separate system exclusive
/// buffer.  This is because the buffer is part of the parser structure: it's not separately allocated
/// (the parser does not use malloc or free at all).  One way around this is to use a very small sysex
/// buffer and collect the sysex in pieces yourself as they come in.
///
/// 2. OMNI is going to be a problem in combination with 14-bit CC or NRPN, because they cannot distinguish by
/// channel.  For example, if you get an NRPN MSB 5 on channel 2, then an NRPN LSB 2 on channel 4, the parser will
/// assume they're supposed to be together even though they're on separate channels.  For this reason, it is
/// recommended that you use OMNI only if you know that all the data coming in will be on the same channel.
///
/// 3. If you have multiple parsers, one per channel, when you receive a byte, you have to hand it to ALL the parsers
/// to parse individually.  It'd be nice to have a meta-parser which determines if the byte is part of a channel voice
/// message and hands it to only one parser as a result: and this would kinda work except for MPE, where some parsers
/// respond to more than one channel (such as the master channel).  For future work perhaps.  For now you have O(n)
/// parses per incoming byte.




// Don't mess with these constants
#define NO_CHANNEL 16                               // Used when we do NOT have an associated channel
#define OMNI 16                                     // Used when we listen to all channels
#define STATUS_NORMAL   0                           // Plain value for NRPN/RPN/High-Res CC
#define STATUS_BARE_MSB 1                           // Value only contains the MSB (so far), LSB assumed to be 0
#define STATUS_BARE_LSB 2                           // Value only contains the LSB (so far), MSB assumed to be 0
#define STATUS_NRPN_INCREMENT 3                     // Value is amount to increment by [only for NRPN/RPN]
#define STATUS_NRPN_DECREMENT 4                     // Value is amount to decrement by [only for NRPN/RPN]
#define STATUS_SYSEX_COMPLETE 5                     // Sysex buffer contains a complete message
#define STATUS_SYSEX_START 6                        // Sysex buffer contains the start but not the end of a message
#define STATUS_SYSEX_END 7                          // Sysex buffer contains the end but not the start of a message
#define STATUS_SYSEX_INCOMPLETE 8                   // Sysex buffer contains a fragment of, but neither the start nor end of a message
#define ERROR_UNRECOGNIZED_CHANNEL (-1)             // Parser ignored a voiced message because it was the wrong channel
#define ERROR_IGNORE_UNVOICED (-2)                  // Parser ignored an unvoiced message because it was set up to do so 
#define ERROR_DISALLOWED (-3)                       // Parser ignored a message because you didn't allow (turn on) a feature
#define ERROR_NO_NRPN_PARAMETER (-4)                // Parser ignored an RPN/NRPN value message because no RPN/NRPN parameter had been set yet
#define ERROR_BARE_NRPN_VALUE_MSB (-5)              // Parser ignored an RPN/NRPN value LSB CC because no RPN/NRPN value MSB had been set yet
#define ERROR_BARE_HIGH_RES_MSB (-6)                // Parser ignored a 14-bit value LSB CC because no equivalent 14-bit value MSB had been set yet
#define ERROR_INVALID_DATA_BYTE (-7)                // Parser ignored a bad data byte
#define ERROR_RESERVED_STATUS_BYTE (-8)             // Parser ignored a reserved status byte (F4, F5, F9, or FD).
#define NOTE_INVALID_ATOMIC_MODULATION_CC (-9)      // Parser temporarily ignored a 14-bit value MSB CC on 26 or 27 because the prior CC was not the same thing, or an INVALID


// FEATURES TO PERMIT
// Comment these out to turn them off.  
// NOTE 1: in order to ALLOW_HIGH_RES_CC or ALLOW_RPN_NRPN, or both, you must also ALLOW_CC
// NOTE 2: if you have ALLOW_PC and ALLOW_CC both on, then Bank CC will be processed as part of the pc(...) method, not cc(...) or highResCC(...)
// NOTE 3: if you turn on ALLOW_BARE_LSB, then nrpn(...) and highResCC(...) will be called with STATUS_BARE_LSB when receiving a bare LSB (no preceding MSB).
//         Otherwise, when this happens the parser will NOT call them and will return ERROR_BARE_NRPN_VALUE_MSB or ERROR_BARE_HIGH_RES_MSB as appropriate.

#define ALLOW_NOTES
#define ALLOW_POLY_AT
#define ALLOW_CC
#define ALLOW_HIGH_RES_CC                           // You must ALSO call setHighResUsed(...) to state which CCs are high-res
#define ALLOW_RPN_NRPN
#define ALLOW_PC
#define ALLOW_AT
#define ALLOW_BEND
#define ALLOW_SYSTEM_EXCLUSIVE
#define ALLOW_MIDI_TIME_CODE_QUARTER_FRAME
#define ALLOW_SONG_POSITION_POINTER
#define ALLOW_SONG_SELECT
#define ALLOW_TUNE_REQUEST
#define ALLOW_CLOCK
#define ALLOW_ACTIVE_SENSING
#define ALLOW_SYSTEM_RESET
#define ALLOW_MPE                                   // If ON, then you can call setMPEMasterChannel(...)  
#define ALLOW_BARE_LSB								   

// EXTRA-MIDI FEATURES

// Used for Modular MIDI Spec
// #define ALLOW_ATOMIC_MODULATION_CC               // Only allow High-Res CC MSBs 26 and 27 to emit a cc(...) if the previous CC was the same (26 or 27) or INVALID



// SYSTEM EXCLUSIVE BUFFER SIZE
// You need to tune this if you have turned on ALLOW_SYSTEM_EXCLUSIVE, otherwise ignore this.
// The data size does not include the F0 nor any F7.  These are not included in the buffer that is passed to you.
// You will not receive a callback until the message is complete -- that is, we do not do the Casio CZ broken sysex thing.
// If a system exclusive message exceeds SYSTEM_EXCLUSIVE_BUFFER_SIZE in length, then it is ignored.

#define SYSTEM_EXCLUSIVE_BUFFER_SIZE 64             // can be no larger than MAX_SYSTEM_EXCLUSIVE_BUFFER_SIZE (254, defined in miditypes.h)





/// THE PARSER STATE MACHINE STRUCTURE
// Don't fool with this.  Treat it as a black box.  You can get the channel, current channel, and tag
// by calling getParserMIDIChannel(), getParserCurrentMIDIChannel, and getParserTag() -- see below
typedef struct midiParser
    {
    unsigned char tag;                              // The parser's tag.  This is a byte for you to use however you like.
    unsigned char channel;                          // The channel to which we are meant to listen, or OMNI 
    unsigned char currentChannel;                   // The current channel, which may vary if we are OMNI
    unsigned char status;                           // The status byte, stripped of channel
    unsigned char data1;                            // The first data byte, if any
    unsigned char unvoicedMessages;                 // Does this parser respond to unvoiced messages?

#ifdef ALLOW_PC
    unsigned char bankMSB;
    unsigned char bankLSB;
#endif   // ALLOW_PC

#ifdef ALLOW_HIGH_RES_CC
    unsigned char highResUsed[4];                   // 32-bit bitfield, bit indicates whether high-res is being used for that CC
    unsigned char highResMSB[32];                   // Low 7 bits is current value MSB.  High bit is whether the MSB has been set yet.
#endif   // ALLOW_HIGH_RES_CC

#ifdef ALLOW_RPN_NRPN
    unsigned char nrpnParamMSB;                     // Low 7 bits is current param MSB.  High bit is whether the MSB has been set yet.
    unsigned char nrpnParamLSB;                     // Low 7 bits is current param LSB.  High bit is whether the LSB has been set yet.
    unsigned char nrpnValueMSB;                     // Low 7 bits is current value MSB.  High bit is whether the MSB has been set yet.
    unsigned char rpn;                              // Is this RPN instead?
#endif   // ALLOW_RPN_NRPN

#ifdef ALLOW_SYSTEM_EXCLUSIVE
    unsigned char systemExclusiveBufferStarted;
    unsigned char systemExclusiveBufferLen;                                 // The length of our system exclusive buffer
    unsigned char systemExclusiveBuffer[SYSTEM_EXCLUSIVE_BUFFER_SIZE];      // System exclusive buffer, not including F0 or F7
#endif   // ALLOW_SYSTEM_EXCLUSIVE

#ifdef ALLOW_ATOMIC_MODULATION_CC
    unsigned char lastCC;                           // What was the previous CC parameter?
#endif   // ALLOW_ATOMIC_MODULATION_CC

#ifdef ALLOW_MPE
    unsigned char mpeMasterChannel;                 // Is this parser part of an MPE group, and if so, what is the mpeMasterChannel?
#endif   // ALLOW_MPE
    } midiParser;







// CALLBACKS TO IMPLEMENT
// You must provide callbacks below for all features you permitted.  


#ifdef ALLOW_NOTES
// note goes 0 ... 127, velocity goes 0 ... 127
// IMPORTANT: noteOn will never have a velocity of 0
extern void noteOn(midiParser* parser, unsigned char note, unsigned char velocity);
extern void noteOff(midiParser* parser, unsigned char note, unsigned char velocity);
#endif   // ALLOW_NOTES

#ifdef ALLOW_CLOCK
extern void clockStop(midiParser* parser);
extern void clockStart(midiParser* parser);
extern void clockContinue(midiParser* parser);
extern void clockPulse(midiParser* parser);
#endif   // ALLOW_CLOCK

#ifdef ALLOW_BEND
// value goes -8192 ... +8191
extern void bend(midiParser* parser, SIGNED_16_BIT_INT value);
#endif   // ALLOW_BEND

#ifdef ALLOW_PC
// program goes 0 ... 127, bankMSB goes 0 ... 127 (0 by default), and bankLSB goes 0 ... 127 (0 by default)
extern void pc(midiParser* parser, unsigned char program, unsigned char bankMSB, unsigned char bankLSB);
#endif   // ALLOW_PC

#ifdef ALLOW_CC
// This will only be called for non-high-res, non-NRPN, non-RPN CCs
// This will NOT be called for any CCs you have previously registered as high resolution via setHighResUsed(...)
// parameter goes 0 ... 127, value goes 0 ... 127
extern void cc(midiParser* parser, unsigned char parameter, unsigned char value);
#endif   // ALLOW_CC

#ifdef ALLOW_RPN_NRPN
// parameter goes 0...16383, value goes 0...16383, rpn is true or false, channel goes 0...15
// status is STATUS_NORMAL, STATUS_BARE_MSB, STATUS_NRPN_INCREMENT, or STATUS_NRPN_DECREMENT 
// STATUS_NORMAL: 'value' holds the value to set.
// STATUS_BARE_MSB: 'value' is determined only by the MSB.  No LSB has arrived yet.  Thus 'value' = MSB * 128 + 0.
// STATUS_BARE_LSB: 'value' is determined only by the LSB.  No MSB has arrived yet.  Thus 'value' = LSB.  This is normally an error.  Only returned if ALLOW_BARE_LSB.
// STATUS_NRPN_INCREMENT: 'value' holds the amount to increment the current value by.  Usually it is 1.
// STATUS_NRPN_DECREMENT: 'value' holds the amount to decrement the current value by.  Usually it is 1.
extern void nrpn(midiParser* parser, UNSIGNED_16_BIT_INT parameter, UNSIGNED_16_BIT_INT value, unsigned char rpn, unsigned char status);
#endif

#ifdef ALLOW_HIGH_RES_CC
// This will be called for CC 6 and 38 (Data Entry) only if you have NOT turned on RPN/NRPN
// This will only be called for CCs you have previously registered as high resolution via setHighResUsed(...)
// parameter goes 0...31, value goes 0...16383
// status is STATUS_NORMAL or STATUS_BARE_MSB 
// STATUS_NORMAL: 'value' holds the value to set.
// STATUS_BARE_MSB: 'value' is determined only by the MSB.  No LSB has arrived yet.  Thus 'value' = MSB * 128 + 0.
// STATUS_BARE_LSB: 'value' is determined only by the LSB.  No MSB has arrived yet.  Thus 'value' = LSB.  This is normally an error.  Only returned if ALLOW_BARE_LSB.
extern void highResCC(midiParser* parser, unsigned char parameter, UNSIGNED_16_BIT_INT value, unsigned char status);
#endif   // ALLOW_HIGH_RES_CC

#ifdef ALLOW_SYSTEM_EXCLUSIVE
// buffer does not include 0xF0 at start nor 0xF7 at end.  len will not exceed SYSTEM_EXCLUSIVE_BUFFER_SIZE (254)
// The status will be one of the following values:
// STATUS_SYSEX_COMPLETE        The buffer contains a complete sysex message from start to end (not including F0 nor F7)
// STATUS_SYSEX_START           The buffer contains just the start of a possibly incomplete sysex message (not including F0)
// STATUS_SYSEX_END             The buffer contains just the end of a sysex message (not including F7)
// STATUS_SYSEX_INCOMPLETE      The buffer contains part of a sysex message in the middle, with neither the start nor end
extern void sysex(midiParser* parser, unsigned char* buffer, unsigned char len, unsigned char status);
#endif   // ALLOW_SYSTEM_EXCLUSIVE

#ifdef ALLOW_AT
// value goes 0 ... 127
extern void aftertouch(midiParser* parser, unsigned char value);
#endif   // ALLOW_AT

#ifdef ALLOW_POLY_AT
// note goes 0 ... 127, value goes 0 ... 127
extern void polyAftertouch(midiParser* parser, unsigned char note, unsigned char value);
#endif   // ALLOW_POLY_AT

#ifdef ALLOW_SONG_POSITION_POINTER
// value goes 0 ... 16383
extern void songPositionPointer(midiParser* parser, UNSIGNED_16_BIT_INT value);
#endif   // ALLOW_SONG_POSITION_POINTER

#ifdef ALLOW_SONG_SELECT
// song goes 0 ... 127
extern void songSelect(midiParser* parser, unsigned char song);
#endif   // ALLOW_SONG_SELECT

#ifdef ALLOW_MIDI_TIME_CODE_QUARTER_FRAME
// data goes 0...127
extern void midiTimeCodeQuarterFrame(midiParser* parser, unsigned char data);
#endif   // ALLOW_MIDI_TIME_CODE_QUARTER_FRAME

#ifdef ALLOW_ACTIVE_SENSING
extern void activeSensing(midiParser* parser);
#endif   // ALLOW_ACTIVE_SENSING

#ifdef ALLOW_SYSTEM_RESET
extern void systemReset(midiParser* parser);
#endif   // ALLOW_SYSTEM_RESET

#ifdef ALLOW_TUNE_REQUEST
extern void tuneRequest(midiParser* parser);
#endif   // ALLOW_TUNE_REQUEST





/// BUILDING AND CALLING THE PARSER
/// You can make multiple parsers: they are independent of one another.
 

// Initializes a MIDI Parser.  You provide the parser and the channel it is meant to listen to to.
// The channel can be any of 0...15 or OMNI
// You can use the tag for anything you like: for example, if you have two MIDI devices, the tag might indicate the device
// unvoicedMessages indicates whether this parser responds to any unvoiced messages (as opposed to ignoring them)
void initializeParser(midiParser* parser, unsigned char channel, unsigned char tag, unsigned char unvoicedMessages);

// Resets the MIDI parser.  You don't need to call this initially but you might
// call it if you need to reset the stream for some reason (very rare).
void resetParser(midiParser* parser);

// Updates the MIDI parser to reflect a new incoming byte.
// Returns one of:
// > 0                                  Completed parse of a MIDI message
// = 0                                  Incomplete parse
// ERROR_UNRECOGNIZED_CHANNEL           Voice message constructed, but its channel is not responded to by this parser
// ERROR_IGNORE_UNVOICED                Unvoiced message constructed but the parser has been instructed to ignore unvoiced messages
// ERROR_DISALLOWED                     Message constructed but ignored because a feature was not allowed (turned on)
// ERROR_NO_NRPN_PARAMETER              An NRPN/RPN value message was constructed, but ignored because no complete NRPN/RPN parameter message had been found yet
// ERROR_BARE_NRPN_VALUE_MSB            An NRPN/RPN *LSB* value message was constructed, but ignored because no NRPN/RPN *MSB* value message had been found yet. Not generated when ALLOW_BARE_LSB.
// ERROR_BARE_HIGH_RES_MSB              A 14-bit *LSB* CC message was constructed, but ignored because no equivalent 14-bit *MSB* CC message had been found yet. Not generated when ALLOW_BARE_LSB.
// ERROR_INVALID_DATA_BYTE              Bad or out of sync data byte, or data byte following a status byte with ERROR_UNRECOGNIZED_CHANNEL
// ERROR_RESERVED_STATUS_BYTE           Illegal reserved status byte found (F4, F5, F9, or FD).
// NOTE_INVALID_ATOMIC_MODULATION_CC    A CC 26 or 27 was constructed but ignored (for the moment) because the previous CC was not the same or invalid.
//                                      This isn't an error per se, but a declaration that the CC was not processed yet to guarantee atomicity.
//                                      This feature is only turned on if we ALLOW_ATOMIC_MODULATION_CC.
signed char parseMidi(midiParser* parser, unsigned char c);

// Returns the parser's assigned MIDI channel
// Will be 0...15 or OMNI
unsigned char getParserMIDIChannel(midiParser* parser);

// Returns the parser's current MIDI channel: the channel of the latest incoming voice data
// This will be 0...15 or INVALID is there has been no valid voice data yet.
unsigned char getParserCurrentMIDIChannel(midiParser* parser);

// Returns the parser's tag
unsigned char getParserTag(midiParser* parser);

#if defined(ALLOW_MPE)
// Sets the MPE Master Channel for this parser.  If voiced messages are assigned to the MPE Master Channel,
// they will be responded to by this parser (along with voiced messages on its regular channel).  The
// MPE Master Channel should be either 0, 15, or NO_CHANNEL.
void setMPEMasterChannel(midiParser* parser, unsigned char mpeChannel);

// Returns the MPE Master Channel for this parser.  If voiced messages are assigned to the MPE Master Channel,
// they will be responded to by this parser (along with voiced messages on its regular channel).  The
// MPE Master Channel should be either 0, 15, or NO_CHANNEL.
unsigned char getMPEMasterChannel(midiParser* parser);
#endif	// defined(ALLOW_MPE)

#if defined(ALLOW_HIGH_RES_CC)
// Sets whether High Resolution CC is used for the given parameter.  For example, if you call
// setHighResUsed(state, 3, 1), then param 3 will define the MSB and 35 will define the LSB of CC 3.
// Only the first 32 parameters (0...31) may be turned on or off.
// Note that CC 6 and 38 (Data Entry) will be ignored here if you have turned on RPN/NRPN, which use them instead.
void setHighResUsed(midiParser* parser, unsigned char parameter, unsigned char on);
#endif	// defined(ALLOW_HIGH_RES_CC)

#endif
