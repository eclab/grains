/// Copyright 2024 by Sean Luke
/// Open Source 
/// Licensed under the Apache 2.0 License

/// Version 0.3:        "Passes Tests, Modular Stuff Added"


/// NOTE ON MIDI INJECTION
/// MIDI Injection is inserting MIDI directives into an existing stream.  It's a lightweight form of MIDI Merge.
/// This has to be done in such a way as not to conflict with running status or system real-time commands.
/// The simplest way to do injection is to take data from the incoming stream, parse it into messages,
/// and then emit the messages.  To do injection, you'd just emit additional messages when you like.
/// This works fine except for latency: messages like note on or poly AT must wait until the entire
/// message is received before it is emitted.  This will incur a roughly 3-byte or 1 millisecond latency.
/// The worst latency will be for sysex messages, but those have the least concern generally.  NRPN and 14-bit
/// CC can cause problems latency-wise as well if you're trying to interweave them with other messages.

/// NOTE ON MIDI MERGE
/// MIDI Merge can be done the same way as MIDI Injection -- parse two incoming streams with two existing
/// parsers, and as the messages come out, emit them to an outgoing stream.  It has the same latency
/// concerns as injection does.  You may wish to only permit clock or active sensing requests to come from
/// one of the two parsers; or just set one parser to reject all non-voiced messages.




#ifndef __EMIT_MODULAR_H
#define __EMIT_MODULAR_H
#include "emitmidi.h"

// Emits a Modulation Parameter (0..7 for 'a'...'h').  Value should be 0...16383.
// For the 7-bit modulation parameters ('c'...'h'), the value should MSB-only, that is, multiplied by 128
// For example, to send value 7 to 7-bit modulation parameter 'c', we'd send emitModulationCCParam(emitter, 2, 7 * 128);
extern void emitModulationCCParam(midiEmitter* emitter, unsigned char param, UNSIGNED_16_BIT_INT value, unsigned char channel);


// Emits a 7-bit CC parameter for the given ID.  ID is 0...7 for IDs 1...8. 
// Parameter should be (0 ... 8 for 'a'...'i').  Value is 0...127
// If you are sending to 'a', 'b', 'h', or 'i', this assumes that you have split that 14-bit CC into two 7-bit CCs
extern void emit7BitCCParam(midiEmitter* emitter, unsigned char id, unsigned char param, unsigned char value, unsigned char channel);


// Emits a 14-bit CC parameter for the given ID.  ID is 0...7 for IDs 1...8. 
// Parameter should be (0 ... 1 for 'a'...'b').  Value is 0...16383
extern void emit14BitCCParam(midiEmitter* emitter, unsigned char id, unsigned char param, UNSIGNED_16_BIT_INT value, unsigned char channel);


// Emits a 7-bit Auxiliary parameter for the given ID.  ID is 8...14 for IDs 9...15 
// Parameter should be (0 ... 15 for 'a'...'p').  Value is 0...127
extern void emitAuxiliaryParam(midiEmitter* emitter, unsigned char id, unsigned char param, unsigned char value, unsigned char channel);


// Emits a 14-bit NRPN parameter for the given ID.  ID is 0...14 for IDs 1...15 
// Parameter should be (0 ... 255 for 'a'... ).  Value is 0...16383
extern void emitNRPNParam(midiEmitter* emitter, unsigned char id, unsigned char param, UNSIGNED_16_BIT_INT value, unsigned char channel);


// Sends a Program Save to the given Bank MSB, Bank LSB, and Program, all 0...127
extern void emitProgramSave(midiEmitter* emitter, unsigned char bankMSB, unsigned char bankLSB, unsigned char program, unsigned char channel);


// Sends a Current Program Save
extern void emitCurrentProgramSave(midiEmitter* emitter, unsigned char channel);


// Sends a Current Program Revert
extern void emitCurrentProgramRevert(midiEmitter* emitter, unsigned char channel);


#endif 			// __EMIT_MODULAR_H