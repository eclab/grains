/// Copyright 2024 by Sean Luke
/// Open Source 
/// Licensed under the Apache 2.0 License

/// Version 0.4:        "Tweaked RPN/NRPN"


/// EMITMODULAR.H
///
/// This package is intended to make it easier to parse features specific to the Modular MIDI conventions:
///
/// 0. ID Mapping for CCs
/// 1. ID Mapping for NRPN
/// 2. ID Mapping for Auxiliary Parameters
///
/// See https://github.com/eclab/grains/blob/main/info/ModularMIDIConventionsForDevelopers.md
///     for more information on the Modular MIDI conventions.
/// See also https://cs.gmu.edu/~sean/papers/modulelevelmidi.pdf

/// SENDING A 7-BIT OR 14-BIT CC BY ID OR MODULATION PARAM
///
/// Each module in Modular MIDI has an ID, possibly more than one ID.  An ID defines a set of CC parameters which
/// it owns and may do with as it wishes.  Within an ID, the parameters in the set are called parameters 'a' through 'i'.
/// the functions emit7BitCCParam(...) and emit14BitCCParam(...) will map an ID and a parameter to the proper CC parameter
/// and send it for you.
///
/// Additionally modules may respond to a special set of CC parameters called "modulation parameters".  Just like the ID
/// parameters, the modulation parameters are also named: 'a' through 'h'.  The function emitModulationCCParam(...) will  
/// map to the proper CC parameter and send a CC for you.

/// SENDING TO NRPN BY ID
///
/// A module's ID also carves out a region of 256 parameters in NRPN space for that module.  The function emitNRPNParam(...)
/// will take an ID and a parameter number and map to the proper NRPN parameter, then send that NRPN for you.


/// SENDING TO AUXILIARY PARAMETERS
///
/// A module's ID may carve out a region of 16 parameters in a special CC space called the "Auxiliary Parameter" space.
/// Auxiliary parameters are sent by with two special CCs.  To make sending auxiliary parameters easier, the function
/// emitAuxiliaryParam(...) will map an ID and an auxiliary parameter number to the proper CC combination, then send 
/// it for you.


/// SAVING AND REVERTING PROGRAMS
///
/// The function emitProgramSave(...) will instruct a module to save its current program to a given Bank MSB, LSB, and Program Number.
///
/// The function emitCurrentProgramSave(...) will instruct a module to save its current program the Bank MBS, LSB, and Program Number
/// currently associated with that program.
///
/// The function emitCurrentProgramRevert(...) will instruct a module to reload its current program from the Bank MBS, LSB, and Program Number
/// currently associated with that program.



#ifndef __EMIT_MODULAR_H
#define __EMIT_MODULAR_H
#include "emitmidi.h"

// Emits a 7-bit CC parameter for the given ID.  ID is 0...7 for IDs 1...8. 
// Parameter should be (0 ... 8 for 'a'...'i').  Value is 0...127
// If you are sending to 'a', 'b', 'h', or 'i', this assumes that you have split that 14-bit CC into two 7-bit CCs
extern void emit7BitCCParam(struct midiEmitter* emitter, unsigned char id, unsigned char param, unsigned char value, unsigned char channel);


// Emits a 14-bit CC parameter for the given ID.  ID is 0...7 for IDs 1...8. 
// Parameter should be (0 ... 1 for 'a'...'b').  Value is 0...16383
extern void emit14BitCCParam(struct midiEmitter* emitter, unsigned char id, unsigned char param, UNSIGNED_16_BIT_INT value, unsigned char channel);


// Emits a Modulation Parameter (0..7 for 'a'...'h').  Value should be 0...16383.
// For the 7-bit modulation parameters ('c'...'h'), the value should MSB-only, that is, multiplied by 128
// For example, to send value 7 to 7-bit modulation parameter 'c', we'd send emitModulationCCParam(emitter, 2, 7 * 128);
extern void emitModulationCCParam(struct midiEmitter* emitter, unsigned char param, UNSIGNED_16_BIT_INT value, unsigned char channel);


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