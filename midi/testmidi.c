/// Copyright 2024 by Sean Luke
/// Open Source 
/// Licensed under the Apache 2.0 License

/// Version 0.4:        "Tweaked RPN/NRPN"


#include "emitmidi.c"
#include "parsemidi.c"
#include <stdio.h>


struct midiEmitter emit;
struct midiParser parse;

void emitMidi(struct midiEmitter* emitter, unsigned char byte)
    {
    printf("\t%d (%02x)\n", byte, byte);
    parseMidi(&parse, byte);
    }

unsigned char expectedNote;
unsigned char expectedVelocity;
signed int expectedValue;
unsigned int expectedParameter;
unsigned char expectedProgram;
unsigned char expectedBankLSB;
unsigned char expectedBankMSB;
unsigned char testBuffer[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
unsigned char testBuffer2[23] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22 };
unsigned char *expectedBuffer;
unsigned char expectedLength;
unsigned char expectedData;
unsigned char expectedSong;
unsigned char expectedRPN;
unsigned char expectedParam;
unsigned char expectedStatus;

void noteOn(struct midiParser* parser, unsigned char note, unsigned char velocity)
    {
    printf("NOTE ON %d/%d %d/%d %s\n", note, expectedNote, velocity, expectedVelocity,
        note == expectedNote && velocity == expectedVelocity ? "PASS" : "FAIL");
    }

void noteOff(struct midiParser* parser, unsigned char note, unsigned char velocity)
    {
    printf("NOTE OFF %d/%d %d/%d %s\n", note, expectedNote, velocity, expectedVelocity,
        note == expectedNote && velocity == expectedVelocity ? "PASS" : "FAIL");
    }

extern void aftertouch(struct midiParser* parser, unsigned char value)
    {
    printf("AT %d/%d %s\n", value, expectedValue, value == expectedValue ? "PASS" : "FAIL");
    }
        
void polyAftertouch(struct midiParser* parser, unsigned char note, unsigned char value)
    {
    printf("POLY AT %d/%d %d/%d %s\n", note, expectedNote, value, expectedValue,
        note == expectedNote && value == expectedValue ? "PASS" : "FAIL");
    }
        
void cc(struct midiParser* parser, unsigned char parameter, unsigned char value)
    {
    printf("CC %d/%d %d/%d %s\n", parameter, expectedParameter, value, expectedValue,
        parameter == expectedParameter && value == expectedValue ? "PASS" : "FAIL");
    }

void pc(struct midiParser* parser, unsigned char program, unsigned char bankMSB, unsigned char bankLSB)
    {
    printf("PC %d/%d %d/%d %d/%d %s\n", program, expectedProgram, bankMSB, expectedBankMSB, bankLSB, expectedBankLSB,
        program == expectedProgram && 
        bankMSB == expectedBankMSB &&
        bankLSB == expectedBankLSB ? "PASS" : "FAIL");
    }

void bend(struct midiParser* parser, SIGNED_16_BIT_INT value)
    {
    printf("BEND %d/%d %s\n", value, expectedValue,
        value == expectedValue ? "PASS" : "FAIL");
    }

void sysex(struct midiParser* parser, unsigned char* buffer, unsigned char len, unsigned char status)
    {
    printf("SYSEX ");
    if (status == STATUS_SYSEX_COMPLETE) printf("(COMPLETE) ");
    else if (status == STATUS_SYSEX_START) printf("(START) ");
    else if (status == STATUS_SYSEX_END) printf("(END) ");
    else // if (status == STATUS_SYSEX_INCOMPLETE) 
        printf("(INCOMPLETE) ");
    int pass = 1;
    if (len != expectedLength) { printf("UNEQUAL LENGTHS %d %d FAIL\n", len, expectedLength); return; }
    for(int i = 0; i < len; i++) 
        {
        printf("%d/%d ", buffer[i], expectedBuffer[i]);
        pass = pass & (buffer[i] == expectedBuffer[i]);
        }
    printf("%s\n", pass ? "PASS" : "FAIL");
    }

void midiTimeCodeQuarterFrame(struct midiParser* parser, unsigned char data)
    {
    printf("MIDI TIME CODE %d/%d %s\n", data, expectedData,
        data == expectedData ? "PASS" : "FAIL");
    }

void songPositionPointer(struct midiParser* parser, UNSIGNED_16_BIT_INT value)
    {
    printf("SONG POSITION %d/%d %s\n", value, expectedValue,
        value == expectedValue ? "PASS" : "FAIL");
    }

void songSelect(struct midiParser* parser, unsigned char song)
    {
    printf("SONG SELECT %d/%d %s\n", song, expectedSong,
        song == expectedSong ? "PASS" : "FAIL");
    }

void tuneRequest(struct midiParser* parser)
    {
    printf("TUNE REQUEST\n");
    }

void clockStart(struct midiParser* parser)
    {
    printf("CLOCK START\n");
    }

void clockStop(struct midiParser* parser)
    {
    printf("CLOCK STOP\n");
    }

void clockPulse(struct midiParser* parser)
    {
    printf("CLOCK PULSE\n");
    }

void clockContinue(struct midiParser* parser)
    {
    printf("CLOCK CONTINUE\n");
    }

void activeSensing(struct midiParser* parser)
    {
    printf("ACTIVE SENSING\n");
    }

void systemReset(struct midiParser* parser)
    {
    printf("SYSTEM RESET\n");
    }

void nrpn(struct midiParser* parser, UNSIGNED_16_BIT_INT parameter, UNSIGNED_16_BIT_INT value, unsigned char rpn, unsigned char status)
    {
    if (status == STATUS_BARE_MSB)
        {
        printf("%s %d/%d %d/%d %d/%d %d/%d %s\n", 
            rpn ? "RPN" : "NRPN",
            parameter , expectedParameter,
            value , expectedValue,
            rpn , expectedRPN,
            status , expectedStatus, "BARE MSB");
        }
    else
        {
        printf("%s %d/%d %d/%d %d/%d %d/%d %s\n", 
            rpn ? "RPN" : "NRPN",
            parameter , expectedParameter,
            value , expectedValue,
            rpn , expectedRPN,
            status , expectedStatus,
            parameter == expectedParameter &&
            value == expectedValue &&
            rpn == expectedRPN &&
            status == expectedStatus ? "PASS" : "FAIL");
        }
    }
        

void highResCC(struct midiParser* parser, unsigned char parameter, UNSIGNED_16_BIT_INT value, unsigned char status)
    {
    if (status == STATUS_BARE_MSB)
        {
        printf("14-BIT %d/%d %d/%d %d/%d %s\n", 
            parameter , expectedParameter,
            value , expectedValue,
            status , expectedStatus, "BARE MSB");
        }
    else if (status == STATUS_BARE_LSB)
    	{
        printf("14-BIT %d/%d %d/%d %d/%d %s\n", 
            parameter , expectedParameter,
            value , expectedValue,
            status , expectedStatus, "BARE LSB");
    	}
    else
        {
        printf("14-BIT %d/%d %d/%d %d/%d %s\n", 
            parameter , expectedParameter + 32,
            value , expectedValue,
            status , expectedStatus,
            parameter == expectedParameter + 32 &&
            value == expectedValue &&
            status == expectedStatus ? "PASS" : "FAIL");
        }
    }

// Insertions   
#define NOTE_ON 0x90
#define TUNE_REQUEST 0xF6
#define CLOCK_PULSE 0xF8
#define UNDEFINED_F9 0xF9                               // Ignore this
#define CLOCK_START 0xFA
#define CLOCK_CONTINUE 0xFB
#define CLOCK_STOP 0xFC
#define UNDEFINED_FD 0xFD                               // Ignore this
#define ACTIVE_SENSING 0xFE                             // You probably want to ignore this
#define SYSTEM_RESET 0xFF                               // You probably want to ignore this
static unsigned char SYSEX_INSERTION[16] = { 0, 1, CLOCK_PULSE, 2, 3, 4, CLOCK_START, 5, 6, 7, CLOCK_STOP, 8, ACTIVE_SENSING, SYSTEM_RESET, CLOCK_CONTINUE, 9 };
static unsigned char SYSEX_INSERTION2[17] = { 0, 1, CLOCK_PULSE, 2, 3, 4, CLOCK_START, 5, 6, 7, CLOCK_STOP, 8, ACTIVE_SENSING, SYSTEM_RESET, CLOCK_CONTINUE, 9, TUNE_REQUEST };
static unsigned char SYSEX_INSERTION_EXPECTED[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
static unsigned char NOTE_ON_INSERTION[7] = { 0x91, CLOCK_PULSE, CLOCK_START, 0, CLOCK_CONTINUE, CLOCK_STOP, 1 };
static unsigned char INVALID_DATA[16] = { 0x91, CLOCK_PULSE, 0, 0x91, CLOCK_PULSE,  0x91, 0xF7, 0, 1, 2, 3, 4, 5, 0x91, 0, 0xF4 };

int main(int argc, char* argv[])
    {
    initializeEmitter(&emit, 0);
    initializeParser(&parse, 1, 0, 1);
        
    //// NOTES
        
    printf("VALID MIDI TESTS START\n");

    // Test Note On
    expectedNote = 23;
    expectedVelocity = 4;
    emitNoteOn(&emit, 23, 4, 1);

    // Test Note On -> Note Off
    expectedNote = 23;
    expectedVelocity = 64;          // Note on vel 0 is note off vel 64
    emitNoteOn(&emit, 23, 0, 1);

    // Test Note Off with Velocity 4
    expectedNote = 23;
    expectedVelocity = 4;
    emitNoteOffVel(&emit, 23, 4, 1);

    // Test Note Off with no Velocity (sent as Note On  with vel 0, which is Note off with Vel 64)
    expectedNote = 23;
    expectedVelocity = 64;
    emitNoteOff(&emit, 23, 1);
        
        
    //// AFTERTOUCH AND POLY AT
        
    expectedValue = 64;
    emitAftertouch(&emit, 64, 1);

    expectedValue = 65;
    emitAftertouch(&emit, 65, 1);
        
    expectedNote = 23;
    expectedValue = 64;
    emitPolyAftertouch(&emit, 23, 64, 1);

    expectedNote = 24;
    expectedValue = 65;
    emitPolyAftertouch(&emit, 24, 65, 1);
        
    //// CC
        
    expectedParameter = 23;
    expectedValue = 64;
    emitCC(&emit, 23, 64, 1);

    expectedParameter = 24;
    expectedValue = 65;
    emitCC(&emit, 24, 65, 1);
        
        
    //// PC
    
    expectedBankMSB = 0;
    expectedBankLSB = 0;
    expectedProgram = 23;
    emitPC(&emit, 23, 1);

    expectedBankMSB = 0;
    expectedBankLSB = 0;
    expectedProgram = 24;
    emitPC(&emit, 24, 1);
        
    expectedBankMSB = 41;
    expectedBankLSB = 72;
    expectedProgram = 24;
    emitBankAndPC(&emit, 24, 41, 72, 1);
        
    expectedBankMSB = 41;
    expectedBankLSB = 72;
    expectedProgram = 26;
    emitPC(&emit, 26, 1);
        
    //// BEND
        
    expectedValue = -1234;
    emitBend(&emit, -1234, 1);

    expectedValue = 1234;
    emitBend(&emit, 1234, 1);
        
    expectedValue = 0;
    emitBend(&emit, 0, 1);

    //// SYSEX
        
    expectedBuffer = testBuffer;
    expectedLength = 8;
    emitSysex(&emit, expectedBuffer, expectedLength);

    expectedBuffer = testBuffer2;
    expectedLength = 8;
    emitSysex(&emit, expectedBuffer, expectedLength);
        
    // MIDI TIME CODE

    expectedData = 23;
    emitMidiTimeCodeQuarterFrame(&emit, 23);

    // SONG POSITION POINTER

    expectedValue = 1234;
    emitSongPositionPointer(&emit, 1234);

    // SONG SELECT

    expectedSong = 23;
    emitSongSelect(&emit, 23);

    // TUNE REQUEST

    emitTuneRequest(&emit);

    // CLOCK START

    emitClockStart(&emit);

    // CLOCK STOP

    emitClockStop(&emit);

    // CLOCK PULSE

    emitClockPulse(&emit);

    // CLOCK CONTINUE

    emitClockContinue(&emit);

    // ACTIVE SENSING

    emitActiveSensing(&emit);

    // SYSTEM RESET

    emitSystemReset(&emit);

    // RPN/NRPN

	// Testing a single LSB.  The Novation A-station does this, grrr
    expectedParameter = 15;
    expectedValue = 1290;
    expectedRPN = 0;
    expectedStatus = STATUS_NORMAL;
    emitCC(&emit, 98, 15, 1);
    emitCC(&emit, 6, 10, 1);
    emitCC(&emit, 38, 10, 1);

	// Testing a single MSB.  Switching to RPN to force reset.
    expectedParameter = 2560;
    expectedValue = 1419;
    expectedRPN = 1;
    expectedStatus = STATUS_NORMAL;
    emitCC(&emit, 101, 20, 1);
    emitCC(&emit, 6, 11, 1);
    emitCC(&emit, 38, 11, 1);

	// Regular testing
    expectedParameter = 1234;
    expectedValue = 5678;
    expectedRPN = 0;
    expectedStatus = STATUS_NORMAL;
    emitNRPN(&emit, 1234, 5678, 0, 1);

    expectedParameter = 1234;
    expectedValue = 5679;
    expectedRPN = 0;
    expectedStatus = STATUS_NORMAL;
    emitNRPN(&emit, 1234, 5679, 0, 1);

    expectedParameter = 1234;
    expectedValue = 5680;
    expectedRPN = 0;
    expectedStatus = STATUS_NORMAL;
    emitNRPN(&emit, 1234, 5680, 0, 1);
        
    expectedParameter = 1234;
    expectedValue = 5678;
    expectedRPN = 1;
    expectedStatus = STATUS_NORMAL;
    emitNRPN(&emit, 1234, 5678, 1, 1);

    expectedParameter = 1234;
    expectedValue = 5679;
    expectedRPN = 1;
    expectedStatus = STATUS_NORMAL;
    emitNRPN(&emit, 1234, 5679, 1, 1);

    expectedParameter = 1234;
    expectedValue = 62;
    expectedRPN = 0;
    expectedStatus = STATUS_NRPN_INCREMENT;
    emitNRPNIncrementBy(&emit, 1234, 62, 0, 1);

    expectedParameter = 1234;
    expectedValue = 63;
    expectedRPN = 0;
    expectedStatus = STATUS_NRPN_INCREMENT;
    emitNRPNIncrementBy(&emit, 1234, 63, 0, 1);

    expectedParameter = 1234;
    expectedValue = 64;
    expectedRPN = 0;
    expectedStatus = STATUS_NRPN_DECREMENT;
    emitNRPNDecrementBy(&emit, 1234, 64, 0, 1);

    expectedParameter = 1234;
    expectedValue = 65;
    expectedRPN = 0;
    expectedStatus = STATUS_NRPN_DECREMENT;
    emitNRPNDecrementBy(&emit, 1234, 65, 0, 1);

    expectedParameter = 1234;
    expectedValue = 5678;
    expectedRPN = 1;
    expectedStatus = STATUS_NORMAL;
    emitNRPN(&emit, 1234, 5678, 1, 1);


    // HIGH-RES CC (14-BIT)
        
  	setHighResUsed(&parse, 1, 1);
    setHighResUsed(&parse, 2, 1);
	setHighResUsed(&parse, 3, 1);
        
    expectedParameter = 1;
    expectedValue = 1;
    expectedStatus = STATUS_BARE_LSB;
 	emitCC(&emit, 33, 1, 1);

    expectedParameter = 1;
    expectedValue = 5678;
    expectedStatus = STATUS_NORMAL;
    emitHighResCC(&emit, 1, 5678, 1);

    expectedParameter = 1;
    expectedValue = 5679;
    expectedStatus = STATUS_NORMAL;
    emitHighResCC(&emit, 1, 5679, 1);

    expectedParameter = 2;
    expectedValue = 5678;
    expectedStatus = STATUS_NORMAL;
    emitHighResCC(&emit, 2, 5678, 1);

    expectedParameter = 2;
    expectedValue = 5679;
    expectedStatus = STATUS_NORMAL;
    emitHighResCC(&emit, 2, 5679, 1);

    expectedParameter = 3;
    expectedValue = 5678;
    expectedStatus = STATUS_NORMAL;
    emitHighResCC(&emit, 3, 5678, 1);

    expectedParameter = 3;
    expectedValue = 5679;
    expectedStatus = STATUS_NORMAL;
    emitHighResCC(&emit, 3, 5679, 1);

    printf("VALID MIDI TESTS END\n\n\n");


    // SYSTEM REALTIME INSERTIONS
    printf("SYSTEM REALTIME INSERTIONS START\n");
        
    expectedBuffer = SYSEX_INSERTION_EXPECTED;
    expectedLength = 10;
    emitMidi(&emit, 0xF0);
    for(int i = 0; i < 16; i++)
        {
        emitMidi(&emit, SYSEX_INSERTION[i]);
        }
    emitMidi(&emit, 0xF7);

    expectedBuffer = SYSEX_INSERTION_EXPECTED;
    expectedLength = 10;
    emitMidi(&emit, 0xF0);
    for(int i = 0; i < 17; i++)
        {
        emitMidi(&emit, SYSEX_INSERTION2[i]);
        }
        
    expectedNote = 0;
    expectedVelocity = 1;
    for(int i = 0; i < 7; i++)
        {
        emitMidi(&emit, NOTE_ON_INSERTION[i]);
        }
        
    printf("SYSTEM REALTIME INSERTIONS END\n\n\n");


    // INVALID CHANNELS
        
    printf("INVALID CHANNELS START\nTo PASS, expect to see nothing here except data bytes\n");
    // Test Note On
    expectedNote = 23;
    expectedVelocity = 4;
    emitNoteOn(&emit, 23, 4, 2);

    // Test Note Off with Velocity 4
    expectedNote = 23;
    expectedVelocity = 4;
    emitNoteOffVel(&emit, 23, 4, 3);

    expectedValue = 64;
    emitAftertouch(&emit, 64, 4);

    expectedNote = 23;
    expectedValue = 64;
    emitPolyAftertouch(&emit, 23, 64, 5);

    expectedParameter = 23;
    expectedValue = 64;
    emitCC(&emit, 23, 64, 6);

    expectedProgram = 23;
    emitPC(&emit, 23, 7);

    expectedValue = -1234;
    emitBend(&emit, -1234, 8);

    printf("INVALID CHANNELS END\n\n\n");


    // OMNI CHANNELS
        
    printf("OMNI CHANNELS START\n");
        
    parse.channel = OMNI;
        
    // Test Note On
    expectedNote = 23;
    expectedVelocity = 4;
    emitNoteOn(&emit, 23, 4, 2);

    // Test Note Off with Velocity 4
    expectedNote = 23;
    expectedVelocity = 4;
    emitNoteOffVel(&emit, 23, 4, 3);

    expectedValue = 64;
    emitAftertouch(&emit, 64, 4);

    expectedNote = 23;
    expectedValue = 64;
    emitPolyAftertouch(&emit, 23, 64, 5);

    expectedParameter = 23;
    expectedValue = 64;
    emitCC(&emit, 23, 64, 6);

    expectedBankMSB = 42;
    expectedBankLSB = 65;
    expectedProgram = 23;
    emitBankAndPC(&emit, 23, 42, 65, 7);

    expectedValue = -1234;
    emitBend(&emit, -1234, 8);

    parse.channel = 1;
        
    printf("OMNI CHANNELS END\n\n\n");

    printf("INVALID DATA START\nTo PASS, expect to see nothing here except data bytes and CLOCK PULSE\n");

    for(int i = 0; i < 16; i++)
        {
        emitMidi(&emit, INVALID_DATA[i]);
        }

    printf("INVALID DATA END\n\n\n");

    printf("ALL DONE\n\n");
        
    }
