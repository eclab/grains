/// Copyright 2024 by Sean Luke
/// Open Source 
/// Licensed under the Apache 2.0 License

/// Version 0.4:        "Tweaked RPN/NRPN"


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
/// 1. You must implement the function          void emitMidi(struct midiEmitter* emitter, unsigned char byte);
///    This method should emit the byte to the device you have associated with this emitter.
///
/// Then you can make a midiEmitter and pass a pointer of it to initializeEmitter to set it up.
/// After that, you can start emitting with it.



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


/// USING THE EMITTER IN GRAINS
///
/// Once you have a parser set up, you implement emitMidi(...) to emit bytes out your serial port.  
/// But what serial port?  GRAINS has two ways of sending MIDI over serial ports:
/// 
///   - From RX over a software serial library.  At present this is your more viable option.  There are several
///     serial port libraries but from my testing it is clear that NeoSoftSerial (https://github.com/SlashDevin/NeoSWSerial) 
///     is the best choice.  You can also get this library through the Arduino library manager.  And several
///     of my GRAINS projects (such as para-m) include the two files NeoSWSerial.h and NeoSWSerial.c, though they have
///     slightly modified them to add an additional parameter which, as it turns out, I don't use anyway.  So there
///     you go.
///
///     To use NeoSWSerial you'd say:
///
///         NeoSWSerial softSerial(receivePin, transmitPin);
///					// If you're using my modified implementation, there are three arguments:
///					// NeoSWSerial softSerial(receivePin, transmitPin, secondTransmitPin);
///					// Ignore this and pass in 255 for secondTransmitPin.
///			softSerial.begin(31250);
///
///     Then you'd implement emitMidi something like this:
///    
///    		void emitMidi(struct midiEmitter* emitter, unsigned char byte) {
///    			softSerial.write(byte);
///    		    }
///
///     The most reliable pin for emitting is CV_GATE_OUT (8).  I strongly suggest that.  If you are not also receiving
///     MIDI, set receivePin to 5 (an unused but valid pin).  Remember to change the pin mode of CV_GATE_OUT to OUTPUT.
///
///     If you are attempting to receive AND send -- to create a MIDI with Soft Thru or whatnot -- it'll be hard.  This is
///     because software serial writing turns off the interrupts for a while, and during that time incoming MIDI will be lost.
///     I have not found it to be reliable.  You might have more luck reading from hardware RX and emitting to software serial.
///
///     You can in theory also write to CV_IN3 (A3) and to CV_AUDIO_IN (A4).  Remember to set their pin modes to OUTPUT.
///
///   - From Hardware TX.  At present this is tied to the USB port, so you can only send MIDI data out over USB.
///     I suggest using this in conjunction with the Dave.java program and a MIDI loopback to route data GRAINS to
///     Dave.java, then to the MIDI loopback, then to a DAW (say).  (In future versions
///     of GRAINS, Hardware TX will be exposed as a separate pin so you could access it directly.  But not now.)
///
///     To do this, you'd set up serial as:
///
///        Serial.begin(31250);
///    
///     Then you'd implement emitMidi something like this:
///    
///    		void emitMidi(struct midiEmitter* emitter, unsigned char byte) {
///    			Serial.write(byte);
///    		    }





/// THE EMITTER STRUCTURE

struct midiEmitter									// we can't make this a typedef because it breaks Arduino's compilation
    {
    UNSIGNED_16_BIT_INT lastNRPN;                   // Last NRPN/RPN Parameter sent.  RPN parameters are + 16384
    unsigned char lastNRPNMSB;                      // Last NRPN/RPN MSB value sent.
    unsigned char lastHighResParameter;             // Last High-Res (14-bit) CC Parameter sent.  Though we could maintain ALL parameters, we'll only maintain the last one for sanity
    unsigned char lastHighResMSB;                   // Last High-Res (14-bit) CC MSB value sent.  Though we could maintain ALL parameters, we'll only maintain the last one for sanity
    unsigned char lastStatus;                       // The last status byte the emitter emitted
    unsigned char tag;                              // The emitter's tag.  Use this however you like.
    };


// You must implement this to emit data
extern void emitMidi(struct midiEmitter* emitter, unsigned char byte);





//// MIDI EMITTER UTILITIES

// Initializes a MIDI emitter.  You provide the emitter.  The tag can be anything you want.
void initializeEmitter(struct midiEmitter* emitter, unsigned char tag);

// Resets the MIDI emitter.  You don't need to call this initially but you might
// call it if you need to reset the stream for some reason (very rare).
void resetEmitter(struct midiEmitter* emitter);

// Returns the parser's tag
static inline unsigned char getEmitterTag(struct midiEmitter* emitter);



//// EMITTING MIDI DATA

// pitch goes 0 ... 127, velocity goes 0 ... 127, channel goes 0...15 for 1...16
// IMPORTANT: note off of velocity 64, or with no velocity, will be sent as a note on of 0 velocity.
void emitNoteOn(struct midiEmitter* emitter, unsigned char pitch, unsigned char velocity, unsigned char channel);
void emitNoteOffVel(struct midiEmitter* emitter, unsigned char pitch, unsigned char velocity, unsigned char channel);
void emitNoteOff(struct midiEmitter* emitter, unsigned char pitch, unsigned char channel);     // velocity is assumed to be 64 (0x40)

void emitClockStop(struct midiEmitter* emitter);
void emitClockStart(struct midiEmitter* emitter);
void emitClockContinue(struct midiEmitter* emitter);
void emitClockPulse(struct midiEmitter* emitter);

// value goes -8192 ... +8191, channel goes 0...15 for 1...16
void emitBend(struct midiEmitter* emitter, SIGNED_16_BIT_INT value, unsigned char channel);

// program goes 0 ... 127, channel goes 0...15 for 1...16
void emitPC(struct midiEmitter* emitter, unsigned char program, unsigned char channel);

// program goes 0 ... 127, bankMSB and bankLSB both go 0 ... 127, channel goes 0...15 for 1...16
void emitPCAndBank(struct midiEmitter* emitter, unsigned char program, unsigned bankMSB, unsigned bankLSB, unsigned char channel);       

// If you support High-Res (14-bit) CC, you should not use this function to send to the MSB or LSB of
// high-res CCs.  Instead, use emitHighResCC(...).
// If you support NRPN or RPN, you should not use this function to send to parameters 6, 38, 96, 97, 98, 99, 100, or 101
// Instead you should use emitNRPN, emitNRPNIncrement, or emitNRPNDecrement
// parameter goes 0...127, value goes 0..127, channel goes 0...15 for 1...16
void emitCC(struct midiEmitter* emitter, unsigned char parameter, unsigned char value, unsigned char channel);

// If you support NRPN or RPN, you should not use this function to send to parameter 6.
// parameter goes 0...32, value goes 0...16383, channel goes 0...15 for 1...16
void emitHighResCC(struct midiEmitter* emitter, unsigned char parameter, UNSIGNED_16_BIT_INT value, unsigned char channel);

// Works for either NRPN or RPN
// parameter goes 0...16383, value goes 0...16383, rpn is true (!0) or false (0), channel goes 0...15 for 1...16
void emitNRPN(struct midiEmitter* emitter, UNSIGNED_16_BIT_INT parameter, UNSIGNED_16_BIT_INT value, unsigned char rpn, unsigned char channel);

// Works for either NRPN or RPN
// parameter goes 0...16383, value goes 0...16383, rpn is true (!0) or false (0), channel goes 0...15 for 1...16
void emitNRPNIncrement(struct midiEmitter* emitter, UNSIGNED_16_BIT_INT parameter, UNSIGNED_16_BIT_INT value, unsigned char rpn, unsigned char channel);

// Works for either NRPN or RPN
// parameter goes 0...16383, value goes 0...16383, rpn is true (!0) or false (0), channel goes 0...15 for 1...16
void emitNRPNDecrement(struct midiEmitter* emitter, UNSIGNED_16_BIT_INT parameter, UNSIGNED_16_BIT_INT value, unsigned char rpn, unsigned char channel);

// buffer does not include 0xF0 at start nor 0xF7 at end
void emitSysex(struct midiEmitter* emitter, unsigned char* buffer, unsigned char len);

// value goes 0 ... 127, channel goes 0...15 for 1...16
void emitAftertouch(struct midiEmitter* emitter, unsigned char value, unsigned char channel);

// note goes 0 ... 127, value goes 0 ... 127, channel goes 0...15 for 1...16
void emitPolyAftertouch(struct midiEmitter* emitter, unsigned char note, unsigned char value, unsigned char channel);

// value goes 0 ... 16383
void emitSongPositionPointer(struct midiEmitter* emitter, UNSIGNED_16_BIT_INT value);

// song goes 0 ... 127
void emitSongSelect(struct midiEmitter* emitter, unsigned char song);

// data goes 0...127
void emitMidiTimeCodeQuarterFrame(struct midiEmitter* emitter, unsigned char data);

void emitActiveSensing(struct midiEmitter* emitter);
void emitSystemReset(struct midiEmitter* emitter);
void emitTuneRequest(struct midiEmitter* emitter);



#endif
