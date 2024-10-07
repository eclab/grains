/// Copyright 2024 by Sean Luke
/// Open Source 
/// Licensed under the Apache 2.0 License

/// Version 0.4:        "Tweaked RPN/NRPN"


#ifndef __EMIT_MODULAR_H
#define __EMIT_MODULAR_H
#include "emitmidi.h"

// Emits a Modulation Parameter (0..7 for 'a'...'h').  Value should be 0...16383.
// For the 7-bit modulation parameters ('c'...'h'), the value should MSB-only, that is, multiplied by 128
// For example, to send value 7 to 7-bit modulation parameter 'c', we'd send emitModulationCCParam(emitter, 2, 7 * 128);
extern void emitModulationCCParam(struct midiEmitter* emitter, unsigned char param, UNSIGNED_16_BIT_INT value, unsigned char channel);


// Emits a 7-bit CC parameter for the given ID.  ID is 0...7 for IDs 1...8. 
// Parameter should be (0 ... 8 for 'a'...'i').  Value is 0...127
// If you are sending to 'a', 'b', 'h', or 'i', this assumes that you have split that 14-bit CC into two 7-bit CCs
extern void emit7BitCCParam(struct midiEmitter* emitter, unsigned char id, unsigned char param, unsigned char value, unsigned char channel);


// Emits a 14-bit CC parameter for the given ID.  ID is 0...7 for IDs 1...8. 
// Parameter should be (0 ... 1 for 'a'...'b').  Value is 0...16383
extern void emit14BitCCParam(struct midiEmitter* emitter, unsigned char id, unsigned char param, UNSIGNED_16_BIT_INT value, unsigned char channel);


// Emits a 7-bit Auxiliary parameter for the given ID.  ID is 8...14 for IDs 9...15 
// Parameter should be (0 ... 15 for 'a'...'p').  Value is 0...127
extern void emitAuxiliaryParam(struct midiEmitter* emitter, unsigned char id, unsigned char param, unsigned char value, unsigned char channel);


// Emits a 14-bit NRPN parameter for the given ID.  ID is 0...14 for IDs 1...15 
// Parameter should be (0 ... 255 for 'a'... ).  Value is 0...16383
extern void emitNRPNParam(struct midiEmitter* emitter, unsigned char id, unsigned char param, UNSIGNED_16_BIT_INT value, unsigned char channel);


// Sends a Program Save to the given Bank MSB, Bank LSB, and Program, all 0...127
extern void emitProgramSave(struct midiEmitter* emitter, unsigned char bankMSB, unsigned char bankLSB, unsigned char program, unsigned char channel);


// Sends a Current Program Save
extern void emitCurrentProgramSave(struct midiEmitter* emitter, unsigned char channel);


// Sends a Current Program Revert
extern void emitCurrentProgramRevert(struct midiEmitter* emitter, unsigned char channel);


#endif 			// __EMIT_MODULAR_H