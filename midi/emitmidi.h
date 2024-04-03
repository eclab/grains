/// Copyright 2024 by Sean Luke
/// Open Source 
/// Licensed under the Apache 2.0 License

/// Version 0.2:        "Compiles and Passes Basic Tests"


#ifndef __EMIT_MIDI_H
#define __EMIT_MIDI_H

#include "miditypes.h"


////// MIDI EMITTER

/// This emitter will emit MIDI and properly handle running status, as well as avoiding additional MSB 
/// messages for 14-bit CC, NRPN, and RPN, as well as avoiding parameter declarations for NRPN and RPN.
///
/// Each emitter is a struct containing state data.  A single emitter is designed to emit data to a particular
/// device, regardless of channel.  You can make multiple emitters, but you should only have one emitter per device. 
///
/// The MIDI Parser and MIDI Emitter can be used and compiled independently. 
///
/// To use the MIDI Emitter you must do a few things.
///
/// 0. You may need to properly define SIGNED_16_BIT_INT and UNSIGNED_16_BIT_INT according to your
///    platform, see miditypes.h
///
/// 1. You must implement the function          void emitMidi(midiEmitter* emitter, unsigned char byte);
///    This method should emit the byte to the device you have associated with this emitter.
///
/// Then you can make a midiEmitter and pass a pointer of it to initializeEmitter to set it up.
/// After that, you can start emitting with it.





/// THE EMITTER STRUCTURE

typedef struct midiEmitter
    {
    UNSIGNED_16_BIT_INT lastNRPN;                   // Last NRPN/RPN Parameter sent.  RPN parameters are + 16384
    unsigned char lastNRPNMSB;                              // Last NRPN/RPN MSB value sent.
    unsigned char lastStatus;                               // The last status byte the emitter emitted
    unsigned char lastHighResParameter;             // Last High-Res (14-bit) CC Parameter sent.  Though we could maintain ALL parameters, we'll only maintain the last one for sanity
    unsigned char lastHighResMSB;                   // Last High-Res (14-bit) CC MSB value sent.  Though we could maintain ALL parameters, we'll only maintain the last one for sanity
    unsigned char tag;                                              // The emitter's tag.  Use this however you like.
    } midiEmitter;


// You must implement this to emit data
extern void emitMidi(midiEmitter* emitter, unsigned char byte);





//// MIDI EMITTER UTILITIES

// Initializes a MIDI emitter.  You provide the emitter.  The tag can be anything you want.
void initializeEmitter(midiEmitter* emitter, unsigned char tag);

// Resets the MIDI emitter.  You don't need to call this initially but you might
// call it if you need to reset the stream for some reason (very rare).
void resetEmitter(midiEmitter* emitter);

// Returns the parser's tag
static inline unsigned char getEmitterTag(midiEmitter* emitter);



//// EMITTING MIDI DATA

// pitch goes 0 ... 127, velocity goes 0 ... 127
// IMPORTANT: note off of 0 velocity will be sent as a note on.
void emitNoteOn(midiEmitter* emitter, unsigned char pitch, unsigned char velocity, unsigned char channel);
void emitNoteOffVel(midiEmitter* emitter, unsigned char pitch, unsigned char velocity, unsigned char channel);
void emitNoteOff(midiEmitter* emitter, unsigned char pitch, unsigned char channel);     // velocity is assumed to be 64 (0x40)

void emitClockStop(midiEmitter* emitter);
void emitClockStart(midiEmitter* emitter);
void emitClockContinue(midiEmitter* emitter);
void emitClockPulse(midiEmitter* emitter);

// value goes -8192 ... +8191
void emitBend(midiEmitter* emitter, SIGNED_16_BIT_INT value, unsigned char channel);

// program goes 0 ... 127
void emitPC(midiEmitter* emitter, unsigned char program, unsigned char channel);

// program goes 0 ... 127, bankMSB and bankLSB both go 0 ... 127
void emitPCAndBank(midiEmitter* emitter, unsigned char program, unsigned bankMSB, unsigned bankLSB, unsigned char channel);       

// If you support High-Res (14-bit) CC, you should not use this functinon to send to the MSB or LSB of
// high-res CCs.  Instead, use emitHighResCC(...).
// If you support NRPN or RPN, you should not use this function to send to parameters 6, 38, 96, 97, 98, 99, 100, or 101
// Instead you should use emitNRPN, emitNRPNIncrement, or emitNRPNDecrement
// parameter goes 0...127, value goes 0..127
void emitCC(midiEmitter* emitter, unsigned char parameter, unsigned char value, unsigned char channel);

// If you support NRPN or NRPN, you should not use this function to send to parameter 6.
// parameter goes 0...32, value goes 0...16383
void emitHighResCC(midiEmitter* emitter, unsigned char parameter, UNSIGNED_16_BIT_INT value, unsigned char channel);

// parameter goes 0...16383, value goes 0...16383, rpn is true or false
void emitNRPN(midiEmitter* emitter, UNSIGNED_16_BIT_INT parameter, UNSIGNED_16_BIT_INT value, unsigned char rpn, unsigned char channel);

// parameter goes 0...16383, value goes 0...16383, rpn is true or false
void emitNRPNIncrement(midiEmitter* emitter, UNSIGNED_16_BIT_INT parameter, UNSIGNED_16_BIT_INT value, unsigned char rpn, unsigned char channel);

// parameter goes 0...16383, value goes 0...16383, rpn is true or false
void emitNRPNDecrement(midiEmitter* emitter, UNSIGNED_16_BIT_INT parameter, UNSIGNED_16_BIT_INT value, unsigned char rpn, unsigned char channel);

// buffer does not include 0xF0 at start nor 0xF7 at end.  Len may not exceed SYSTEM_EXCLUSIVE_BUFFER_SIZE (254, defined in miditypes.h)
void emitSysex(midiEmitter* emitter, unsigned char* buffer, unsigned char len);

// value goes 0 ... 127
void emitAftertouch(midiEmitter* emitter, unsigned char value, unsigned char channel);

// note goes 0 ... 127, value goes 0 ... 127
void emitPolyAftertouch(midiEmitter* emitter, unsigned char note, unsigned char value, unsigned char channel);

// value goes 0 ... 16383
void emitSongPositionPointer(midiEmitter* emitter, UNSIGNED_16_BIT_INT value);

// song goes 0 ... 127
void emitSongSelect(midiEmitter* emitter, unsigned char song);

// data goes 0...127
void emitMidiTimeCodeQuarterFrame(midiEmitter* emitter, unsigned char data);

void emitActiveSensing(midiEmitter* emitter);
void emitSystemReset(midiEmitter* emitter);
void emitTuneRequest(midiEmitter* emitter);



#endif
