/// Copyright 2024 by Sean Luke
/// Open Source 
/// Licensed under the Apache 2.0 License

/// Version 0.4:        "Tweaked RPN/NRPN"


#include "emitmidi.h"

// MIDI Status Byte Types
#define NOTE_OFF 0x80
#define NOTE_ON 0x90
#define POLY_AT 0xA0
#define CC 0xB0
#define PC 0xC0
#define AT 0xD0
#define BEND 0xE0
#define SYSTEM_EXCLUSIVE 0xF0
#define MIDI_TIME_CODE_QUARTER_FRAME 0xF1
#define SONG_POSITION_POINTER 0xF2
#define SONG_SELECT 0xF3
#define UNDEFINED_F4 0xF4                               // Ignore this
#define UNDEFINED_F5 0xF5                               // Ignore this
#define TUNE_REQUEST 0xF6
#define END_OF_SYSEX 0xF7
#define CLOCK_PULSE 0xF8
#define UNDEFINED_F9 0xF9                               // Ignore this
#define CLOCK_START 0xFA
#define CLOCK_CONTINUE 0xFB
#define CLOCK_STOP 0xFC
#define UNDEFINED_FD 0xFD                               // Ignore this
#define ACTIVE_SENSING 0xFE                             // You probably want to ignore this
#define SYSTEM_RESET 0xFF                               // You probably want to ignore this

#define INVALID 255
#define INVALID_BIG 65535



void resetEmitter(struct midiEmitter* emitter)
    {
    emitter->lastStatus = INVALID;
    emitter->lastHighResParameter = INVALID;
    emitter->lastHighResMSB = INVALID;
    emitter->lastNRPNMSB = INVALID;
    emitter->lastNRPN = INVALID_BIG;
    }

void initializeEmitter(struct midiEmitter* emitter, unsigned char tag)
    {
    emitter->tag = tag;
    resetEmitter(emitter);
    }

// this is just called internally as a wrapper around emitMidi
void doEmit(struct midiEmitter* emitter, unsigned char byte)
    {
    if (emitter->lastStatus == byte)    // already emitted it as the last status byte, ignore for running status
        return;
    // should we save the status byte for running status?
    if (byte >= NOTE_OFF)                               // it's a status byte
        {
        if (byte < SYSTEM_EXCLUSIVE)    // voiced message
            {
            emitter->lastStatus = byte;
            }
        else if (byte < CLOCK_PULSE)    // >= CLOCK_PULSE is system real-time, it doesn't count
            {
            emitter->lastStatus = INVALID;
            }
        }
    emitMidi(emitter, byte);
    }

void emitNoteOn(struct midiEmitter* emitter, unsigned char pitch, unsigned char velocity, unsigned char channel)
    {
    doEmit(emitter, NOTE_ON | (channel & 15));
    doEmit(emitter, pitch & 127);
    doEmit(emitter, velocity & 127);
    }
        
void emitNoteOffVel(struct midiEmitter* emitter, unsigned char pitch, unsigned char velocity, unsigned char channel)
    {
    if (velocity == 0x40) emitNoteOn(emitter, pitch, 0, channel);
    else
        {
        doEmit(emitter, NOTE_OFF | (channel & 15));
        doEmit(emitter, pitch & 127);
        doEmit(emitter, velocity & 127);
        }
    }

void emitNoteOff(struct midiEmitter* emitter, unsigned char pitch, unsigned char channel)     
    {
    emitNoteOn(emitter, pitch, 0, channel);
    }

void emitClockStop(struct midiEmitter* emitter)
    {
    doEmit(emitter, CLOCK_STOP);
    }
        
void emitClockStart(struct midiEmitter* emitter)
    {
    doEmit(emitter, CLOCK_START);
    }

void emitClockContinue(struct midiEmitter* emitter)
    {
    doEmit(emitter, CLOCK_CONTINUE);
    }

void emitClockPulse(struct midiEmitter* emitter)
    {
    doEmit(emitter, CLOCK_PULSE);
    }

// value goes -8192 ... +8191, channel goes 0 ... 15
void emitBend(struct midiEmitter* emitter, SIGNED_16_BIT_INT value, unsigned char channel)
    {
    doEmit(emitter, BEND | (channel & 15));
    UNSIGNED_16_BIT_INT val = (UNSIGNED_16_BIT_INT)(value + 8192);
    doEmit(emitter, (val >> 7) & 127);
    doEmit(emitter, val & 127);
    }

// This will only be called for non-high-res, non-NRPN, non-RPN CCs
// This will NOT be called for any CCs you have previously registered as high resolution via setHighResUsed(...)
// parameter goes 0 ... 127, value goes 0 ... 127, channel goes 0 ... 15
void emitCC(struct midiEmitter* emitter, unsigned char parameter, unsigned char value, unsigned char channel)
    {
    doEmit(emitter, CC | (channel & 15));
    doEmit(emitter, parameter & 127);
    doEmit(emitter, value & 127);
    }   
    

// program goes 0 ... 127, channel goes 0 ... 15
void emitPC(struct midiEmitter* emitter, unsigned char program, unsigned char channel)       
    {
    doEmit(emitter, PC | (channel & 15));
    doEmit(emitter, program & 127);
    }

// program goes 0 ... 127, channel goes 0 ... 15
void emitPCAndBank(struct midiEmitter* emitter, unsigned char program, unsigned bankMSB, unsigned bankLSB, unsigned char channel)       
    {
    emitCC(emitter, 0, bankMSB, channel);
    emitCC(emitter, 32, bankLSB, channel);
    doEmit(emitter, PC | (channel & 15));
    doEmit(emitter, program & 127);
    }

// parameter goes 0...16383, value goes 0...16383, rpn is true or false, channel goes 0...15
void emitNRPN(struct midiEmitter* emitter, UNSIGNED_16_BIT_INT parameter, UNSIGNED_16_BIT_INT value, unsigned char rpn, unsigned char channel)
    {
    if (rpn) parameter += 16384;
    // Do we resubmit the parameter?
    if (emitter->lastNRPN != parameter)
        {
        emitter->lastNRPN = parameter;                                                  // store high if rpn
        if (parameter >= 16384)
            {
            parameter -= 16384;
            emitCC(emitter, 101, (parameter >> 7) & 127, channel);             // RPN MSB
            emitCC(emitter, 100, parameter & 127, channel);                    // RPN LSB
            }
        else
            {
            emitCC(emitter, 99, (parameter >> 7) & 127, channel);              // NRPN MSB
            emitCC(emitter, 98, parameter & 127, channel);                     // NRPN LSB
            }
        emitter->lastNRPNMSB = INVALID;                                       // MUST resubmit MSB    
        }
                
    // Do we resubmit the MSB?
    unsigned char msb = (value >> 7) & 127;
    if (emitter->lastNRPNMSB != msb)
        {
        emitter->lastNRPNMSB = msb;
        emitCC(emitter, 6, msb, channel);
        }
                
    emitCC(emitter, 38, value & 127, channel);
    emitter->lastHighResMSB = INVALID;
    }


// parameter goes 0...16383, value goes 0...127, rpn is true or false, channel goes 0...15
void emitNRPNIncrement(struct midiEmitter* emitter, UNSIGNED_16_BIT_INT parameter, UNSIGNED_16_BIT_INT value, unsigned char rpn, unsigned char channel)
    {
    if (rpn) parameter += 16384;
    // Do we resubmit the parameter?
    if (emitter->lastNRPN != parameter)
        {
        emitter->lastNRPN = parameter;                                                  // store high if rpn
        if (parameter >= 16384)
            {
            parameter -= 16384;
            emitCC(emitter, 101, (parameter >> 7) & 127, channel);             // RPN MSB
            emitCC(emitter, 100, parameter & 127, channel);                    // RPN LSB
            }
        else
            {
            emitCC(emitter, 99, (parameter >> 7) & 127, channel);              // NRPN MSB
            emitCC(emitter, 98, parameter & 127, channel);                     // NRPN LSB
            }
        emitter->lastNRPNMSB = INVALID;                                                                 // MUST resubmit MSB    
        }

    emitCC(emitter, 96, value & 127, channel);
    emitter->lastNRPNMSB = INVALID;
    emitter->lastHighResMSB = INVALID;
    }

// parameter goes 0...16383, value goes 0...127, rpn is true or false, channel goes 0...15
void emitNRPNDecrement(struct midiEmitter* emitter, UNSIGNED_16_BIT_INT parameter, UNSIGNED_16_BIT_INT value, unsigned char rpn, unsigned char channel)
    {
    if (rpn) parameter += 16384;
    // Do we resubmit the parameter?
    if (emitter->lastNRPN != parameter)
        {
        emitter->lastNRPN = parameter;                                                  // store high if rpn
        if (parameter >= 16384)
            {
            parameter -= 16384;
            emitCC(emitter, 101, (parameter >> 7) & 127, channel);             // RPN MSB
            emitCC(emitter, 100, parameter & 127, channel);                    // RPN LSB
            }
        else
            {
            emitCC(emitter, 99, (parameter >> 7) & 127, channel);              // NRPN MSB
            emitCC(emitter, 98, parameter & 127, channel);                     // NRPN LSB
            }
        emitter->lastNRPNMSB = INVALID;                                                                 // MUST resubmit MSB    
        }

    emitCC(emitter, 97, value & 127, channel);
    emitter->lastNRPNMSB = INVALID;
    emitter->lastHighResMSB = INVALID;
    }


void emitHighResCC(struct midiEmitter* emitter, unsigned char parameter, UNSIGNED_16_BIT_INT value, unsigned char channel)   
    {
    unsigned char msb = (value >> 7) & 127;

    // Do we resubmit the MSB?
    if (emitter->lastHighResParameter != parameter ||
        emitter->lastHighResMSB != msb)
        {
        emitter->lastHighResParameter = parameter;
        emitter->lastHighResMSB = msb;
        emitCC(emitter, parameter, msb, channel);
        }
        
    emitCC(emitter, parameter + 32, value & 127, channel);          // lsb
    }


// buffer does not include 0xF0 at start nor 0xF7 at end
void emitSysex(struct midiEmitter* emitter, unsigned char* buffer, unsigned char len)
    {
    doEmit(emitter, 0xF0);
    for(unsigned char i = 0; i < len; i++)
        {
        doEmit(emitter, buffer[i] & 127);
        }
    doEmit(emitter, 0xF7);
    }

// value goes 0 ... 127, channel goes 0 ... 15
void emitAftertouch(struct midiEmitter* emitter, unsigned char value, unsigned char channel)
    {
    doEmit(emitter, AT | (channel & 15));
    doEmit(emitter, value & 127);
    }

// note goes 0 ... 127, value goes 0 ... 127, channel goes 0 ... 15
void emitPolyAftertouch(struct midiEmitter* emitter, unsigned char note, unsigned char value, unsigned char channel)
    {
    doEmit(emitter, POLY_AT | (channel & 15));
    doEmit(emitter, note & 127);
    doEmit(emitter, value & 127);
    }

// value goes 0 ... 16383, channel goes 0 ... 15
void emitSongPositionPointer(struct midiEmitter* emitter, UNSIGNED_16_BIT_INT value)
    {
    doEmit(emitter, SONG_POSITION_POINTER);
    doEmit(emitter, (value >> 7) & 127);
    doEmit(emitter, value & 127);
    }

// song goes 0 ... 127, channel goes 0 ... 15
void emitSongSelect(struct midiEmitter* emitter, unsigned char song)
    {
    doEmit(emitter, SONG_SELECT);
    doEmit(emitter, song & 127);
    }

// data goes 0...127, channel goes 0 ... 15
void emitMidiTimeCodeQuarterFrame(struct midiEmitter* emitter, unsigned char data)
    {
    doEmit(emitter, MIDI_TIME_CODE_QUARTER_FRAME);
    doEmit(emitter, data & 127);
    }

void emitActiveSensing(struct midiEmitter* emitter)
    {
    doEmit(emitter, ACTIVE_SENSING);
    }
        
void emitSystemReset(struct midiEmitter* emitter)
    {
    doEmit(emitter, SYSTEM_RESET);
    }
        
void emitTuneRequest(struct midiEmitter* emitter)
    {
    doEmit(emitter, TUNE_REQUEST);
    }

// Returns the parser's tag
static inline unsigned char getEmitterTag(struct midiEmitter* emitter)
    {
    return emitter->tag;
    }

