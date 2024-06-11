/// Copyright 2024 by Sean Luke
/// Open Source 
/// Licensed under the Apache 2.0 License

/// parsemidi.c is the implementation of the MIDI parser -- see parsemidi.h for instructions for use



#include "parsemidi.h"
#include <stdio.h>

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

// MIDI Status Byte Categories
#define STATUS NOTE_OFF                                 // All status bytes are >= STATUS
#define REALTIME 0xF8                                   // All realtime system bytes are >= REALTIME

// State Machine Variable Settings
#define INVALID 255                                     // Indicates that a variable cannot be used
#define INITIALIZED 128                                 // Indicates that a variable is ready to be set but has no value






///// HIGH RESOLUTION CC, RPN, AND NRPN PARSER




#ifdef ALLOW_HIGH_RES_CC
// Returns whether a given CC parameter (0...31) is being used as a 14-bit CC

static inline unsigned char isHighResUsed(midiParser* parser, unsigned char parameter)
    {
    unsigned char pos = parameter >> 3;             // which byte
    unsigned char slot = parameter & 8;             // where in the byte
    return parser->highResUsed[pos] >> slot;
    }


// Sets whether a given CC parameter (0...31) is being used as a 14-bit CC

void setHighResUsed(midiParser* parser, unsigned char parameter, unsigned char on)
    {
    unsigned char pos = parameter >> 3;             // which byte
    unsigned char slot = parameter & 8;             // where in the byte
    if (on)
        {
        parser->highResUsed[pos] = parser->highResUsed[pos] | (1 << slot);
        }
    else
        {
        parser->highResUsed[pos] = parser->highResUsed[pos] & ~(1 << slot);
        }
    }


// Returns whether the 14-bit MSB was defined at least once so far
// Note: returns 0 if false, and 128 if true [on purpose]

static inline unsigned char isHighResMSBSet(midiParser* parser, unsigned char parameter)
    {
    return parser -> highResMSB[parameter] & 0x80;
    }


// Sets the 14-bit MSB.  Don't call this.

static inline void setHighResMSB(midiParser* parser, unsigned char parameter, unsigned char val)
    {
    parser -> highResMSB[parameter] = 0x80 | val;
    }


// Returns the 14-bit MSB.  If not defined yet, it will be 0 by default.

static inline unsigned char getHighResMSB(midiParser* parser, unsigned char parameter)
    {
    return parser -> highResMSB[parameter] & 0x7F;
    }
#endif
        


#ifdef ALLOW_RPN_NRPN
// Returns whether the NRPN / RPN Parameter MSB was defined at least once so far
// Note: returns 0 if false, and 128 if true [on purpose]

static inline unsigned char isNRPNParamMSBSet(midiParser* parser)
    {
    return parser -> nrpnParamMSB & 0x80;
    }


// Sets the NRPN / RPN Parameter MSB.  Don't call this.

static inline void setNRPNParamMSB(midiParser* parser, unsigned char param)
    {
    parser -> nrpnParamMSB = 0x80 | param;
    }


// Returns the NRPN / RPN Parameter MSB.  If not defined yet, it will be 0 by default.

static inline unsigned char getNRPNParamMSB(midiParser* parser)
    {
    return parser -> nrpnParamMSB & 0x7F;
    }


// Returns whether the NRPN / RPN Parameter LSB was defined at least once so far
// Note: returns 0 if false, and 128 if true [on purpose]

static inline unsigned char isNRPNParamLSBSet(midiParser* parser)
    {
    return parser -> nrpnParamLSB & 0x80;
    }


// Sets the NRPN / RPN Parameter LSB.  Don't call this.

static inline void setNRPNParamLSB(midiParser* parser, unsigned char param)
    {
    parser -> nrpnParamLSB = 0x80 | param;
    }


// Returns the NRPN / RPN Parameter LSB.  If not defined yet, it will be 0 by default.

static inline unsigned char getNRPNParamLSB(midiParser* parser)
    {
    return parser -> nrpnParamLSB & 0x7F;
    }


// Returns whether the NRPN / RPN Value MSB was defined at least once so far
// Note: returns 0 if false, and 128 if true [on purpose]

static inline unsigned char isNRPNValueMSBSet(midiParser* parser)
    {
    return parser -> nrpnValueMSB & 0x80;
    }


// Sets the NRPN / RPN Value MSB.  Don't call this.

static inline void setNRPNValueMSB(midiParser* parser, unsigned char val)
    {
    parser -> nrpnValueMSB = 0x80 | val;
    }


// Returns the NRPN / RPN Value MSB.  If not defined yet, it will be 0 by default.

static inline unsigned char getNRPNValueMSB(midiParser* parser)
    {
    return parser -> nrpnValueMSB & 0x7F;
    }
#endif



// Call this whenever you receive a new MIDI CC message.  Processes the CC message and updates
// the cc state, calling callbacks as appropriate.

void processCC(midiParser* parser, unsigned char param, unsigned char val)
    {
#ifdef ALLOW_CC
#ifdef ALLOW_RPN_NRPN
    if (param >= 96 && param <= 101) // NRPN Parameter Change
        {
        if (param == 98)        // NRPN LSB
            {
            // At preset we do not reset the MSB even if we've emitted some NRPN values.  Is this wise?
            setNRPNParamLSB(parser, val);
            parser -> nrpnValueMSB = 0;
            parser -> rpn = 0;
            }
        else if (param == 99)   // NRPN MSB
            {
            // At preset we do not reset the LSB even if we've emitted some NRPN values.  Is this wise?
            setNRPNParamMSB(parser, val);
            parser -> nrpnValueMSB = 0;
            parser -> rpn = 0;
            }
        else if (param == 100)  // RPN LSB
            {
            // At preset we do not reset the MSB even if we've emitted some NRPN values.  Is this wise?
            setNRPNParamLSB(parser, val);
            parser -> nrpnValueMSB = 0;
            parser -> rpn = 1;
            }
        else if (param == 101)  // RPN MSB
            {
            // At preset we do not reset the LSB even if we've emitted some NRPN values.  Is this wise?
            setNRPNParamMSB(parser, val);
            parser -> nrpnValueMSB = 0;
            parser -> rpn = 1;
            }
        else if (param == 96)   // Increment
            {
            if (isNRPNParamMSBSet(parser) && isNRPNParamLSBSet(parser))
                {
	            parser -> nrpnValueMSB = 0;		// invalidate later LSBs
                nrpn(parser, getNRPNParamMSB(parser) * 128 + getNRPNParamLSB(parser), val, parser->rpn, STATUS_NRPN_INCREMENT);
                }
            }
        else if (param == 97)
            {
            if (isNRPNParamMSBSet(parser) && isNRPNParamLSBSet(parser))
                {
	            parser -> nrpnValueMSB = 0;		// invalidate later LSBs
                nrpn(parser, getNRPNParamMSB(parser) * 128 + getNRPNParamLSB(parser), val, parser->rpn, STATUS_NRPN_DECREMENT);
                }
            }
        return;
        }
    else if (param == 6)  // NRPN Value Change MSB
        {
        if (isNRPNParamMSBSet(parser) && isNRPNParamLSBSet(parser))
            {
            setNRPNValueMSB(parser, val);           
            nrpn(parser, getNRPNParamMSB(parser) * 128 + getNRPNParamLSB(parser), val * 128, parser->rpn, STATUS_BARE_MSB);
            }
        return;
        }
    else if (param == 38)  // NRPN Value Change LSB
        {
        if (isNRPNParamMSBSet(parser) && isNRPNParamLSBSet(parser) && isNRPNValueMSBSet(parser))
            {
            unsigned char msb = getNRPNValueMSB(parser);
            nrpn(parser, getNRPNParamMSB(parser) * 128 + getNRPNParamLSB(parser), msb * 128 + val, parser->rpn, STATUS_NORMAL);
            }
        return;
        }
#endif
                
#ifdef ALLOW_HIGH_RES_CC                
    if (param < 32 && isHighResUsed(parser, param))         // High Res CC MSB
        {
        setHighResMSB(parser, param, val);
        highResCC(parser, param, val * 128, STATUS_BARE_MSB);
        return;
        }
    else if (param < 64 && isHighResUsed(parser, param - 32) && isHighResMSBSet(parser, param - 32))        // High Res CC LSB
        {
        unsigned char msb = getHighResMSB(parser, param - 32);
        highResCC(parser, param, msb * 128 + val, STATUS_NORMAL);
        return;
        }
#endif

    // Low Res CC
    cc(parser, param, val);
#endif
    }









///// TOP-LEVEL MIDI PARSER



void resetParser(midiParser* parser)
    {
    parser->currentChannel = INVALID;
    parser->status = INVALID;
    parser->data1 = INVALID;
#ifdef ALLOW_SYSTEM_EXCLUSIVE
    parser->systemExclusiveBufferLen = 0;
#endif

#ifdef ALLOW_HIGH_RES_CC
    for(unsigned char i = 0; i < 32; i++)
        {
        parser->highResMSB[i] = 0;
        }
#endif

#ifdef ALLOW_RPN_NRPN           
    parser->nrpnValueMSB = 0;
    parser->nrpnParamMSB = 0;
    parser->nrpnParamLSB = 0;
    parser->rpn = 0;
#endif
    }

// channel can be 0...15 or OMNI
void initializeParser(midiParser* parser, unsigned char channel, unsigned char tag, unsigned char unvoicedMessages)
    {
    parser->channel = channel;
    parser->tag = tag;
    parser->unvoicedMessages = unvoicedMessages;
        
#ifdef ALLOW_HIGH_RES_CC
    for(unsigned char i = 0; i < 4; i++)
        {
        parser->highResUsed[i] = 0;
        }
#endif

    resetParser(parser);
    }




// This is just to remove redundancy in parseDataByte.
// It sets the channel to the voice message's channel,
// and sets up whether we should listen to this message based on the channel
static inline void processVoiceMessage(midiParser* parser, unsigned char c, unsigned char status)
    {
    unsigned char channel = c - status;
    
    if (parser->channel == OMNI ||
        parser->channel == channel)
        {
        parser->currentChannel = channel;
        parser->status = status;
        parser->data1 = INITIALIZED;
        }
    else
        {
        parser->currentChannel = INVALID;
        parser->status = INVALID;
        parser->data1 = INVALID;
        }
    }
        

// This handles the first data byte
static inline void processFirstDataByte(midiParser* parser, unsigned char c)          // yes, inline
    {
    switch(parser->status)
        {
#ifdef ALLOW_PC
        case PC:
            {
            pc(parser, c);
            return;             // data1 stays initialized for running status
            }
        break;
#endif
#ifdef ALLOW_AT
        case AT:
            {
            aftertouch(parser, c);
            return;             // data1 stays initialized for running status
            }
        break;
#endif
#ifdef ALLOW_SYSTEM_EXCLUSIVE
        case SYSTEM_EXCLUSIVE:
            {
            if (!parser->unvoicedMessages) 
                {
                parser->systemExclusiveBufferLen = INVALID;
                }
            else if (parser->systemExclusiveBufferLen < SYSTEM_EXCLUSIVE_BUFFER_SIZE)
                {
                parser->systemExclusiveBuffer[parser->systemExclusiveBufferLen++] = c;
                }
            else
                {
                parser->systemExclusiveBufferLen = INVALID;
                }
            return;             // data1 stays initialized for continuing system exclusive
            }
        break;
#endif
#ifdef ALLOW_MIDI_TIME_CODE_QUARTER_FRAME
        case MIDI_TIME_CODE_QUARTER_FRAME:
            {
            if (parser->unvoicedMessages) midiTimeCodeQuarterFrame(parser, c);
            return;             // data1 stays initialized for running status
            }
        break;
#endif
#ifdef ALLOW_SONG_SELECT
        case SONG_SELECT:
            {
            if (parser->unvoicedMessages) songSelect(parser, c);
            return;             // data1 stays initialized for running status
            }
        break;
#endif
        default:
            parser->data1 = c;
            break;
        }
    }


// This handles the second data byte
static inline void processSecondDataByte(midiParser* parser, unsigned char c)          // yes, inline
    {
    switch(parser->status)
        {
#ifdef ALLOW_NOTES
        case NOTE_OFF:
            noteOff(parser, parser->data1, c);
            break;
        case NOTE_ON:
            if (c == 0)
                {
                noteOff(parser, parser->data1, 0x40);
                }
            else
                {
                noteOn(parser, parser->data1, c);
                }
            break;
#endif
#ifdef ALLOW_POLY_AT
        case POLY_AT:
            polyAftertouch(parser, parser->data1, c);
            break;
#endif
#ifdef ALLOW_CC
        case CC:
#if defined(ALLOW_HIGH_RES_CC) || defined(ALLOW_RPN_NRPN)
            processCC(parser, parser->data1, c);
#else
            cc(parser, parser->data1, c);
#endif
            break;
#endif
#ifdef ALLOW_BEND
        case BEND:
            bend(parser, ((SIGNED_16_BIT_INT)parser->data1) * 128 + c - 8192);
            break;
#endif
#ifdef ALLOW_SONG_POSITION_POINTER
        case SONG_POSITION_POINTER:
            if (parser->unvoicedMessages) songPositionPointer(parser, ((UNSIGNED_16_BIT_INT)parser->data1) * 128 + c);
            break;
#endif

        }
    parser->data1 = INITIALIZED;            // running status!
    }
        
    
// Main MIDI Parser
void parseMidi(midiParser* parser, unsigned char c)
    {
#ifdef ALLOW_SYSTEM_EXCLUSIVE
    if (c >= STATUS && c < REALTIME)
        {
        // non-realtime status bytes close system exclusive so we need to handle that first
        if (!parser->unvoicedMessages) 
            {
            parser->systemExclusiveBufferLen = INVALID;
            }
        else if (parser->systemExclusiveBufferLen > 0)
            {
            if (parser->systemExclusiveBufferLen != INVALID)
                {
                sysex(parser, parser->systemExclusiveBuffer, parser->systemExclusiveBufferLen);
                }
            parser->systemExclusiveBufferLen = 0;
            }
        }
#endif        
        
    switch(c)
        {
        case NOTE_OFF:
        case NOTE_OFF + 1:
        case NOTE_OFF + 2:
        case NOTE_OFF + 3:
        case NOTE_OFF + 4:
        case NOTE_OFF + 5:
        case NOTE_OFF + 6:
        case NOTE_OFF + 7:
        case NOTE_OFF + 8:
        case NOTE_OFF + 9:
        case NOTE_OFF + 10:
        case NOTE_OFF + 11:
        case NOTE_OFF + 12:
        case NOTE_OFF + 13:
        case NOTE_OFF + 14:
        case NOTE_OFF + 15:
#ifdef ALLOW_NOTES
            processVoiceMessage(parser, c, NOTE_OFF);
#else
            parser->status = INVALID;
            parser->data1 = INVALID;
#endif
            break;
        case NOTE_ON:
        case NOTE_ON + 1:
        case NOTE_ON + 2:
        case NOTE_ON + 3:
        case NOTE_ON + 4:
        case NOTE_ON + 5:
        case NOTE_ON + 6:
        case NOTE_ON + 7:
        case NOTE_ON + 8:
        case NOTE_ON + 9:
        case NOTE_ON + 10:
        case NOTE_ON + 11:
        case NOTE_ON + 12:
        case NOTE_ON + 13:
        case NOTE_ON + 14:
        case NOTE_ON + 15:
#ifdef ALLOW_NOTES
            processVoiceMessage(parser, c, NOTE_ON);
#else
            parser->status = INVALID;
            parser->data1 = INVALID;
#endif
            break;
        case POLY_AT:
        case POLY_AT + 1:
        case POLY_AT + 2:
        case POLY_AT + 3:
        case POLY_AT + 4:
        case POLY_AT + 5:
        case POLY_AT + 6:
        case POLY_AT + 7:
        case POLY_AT + 8:
        case POLY_AT + 9:
        case POLY_AT + 10:
        case POLY_AT + 11:
        case POLY_AT + 12:
        case POLY_AT + 13:
        case POLY_AT + 14:
        case POLY_AT + 15:
#ifdef ALLOW_POLY_AT
            processVoiceMessage(parser, c, POLY_AT);
#else
            parser->status = INVALID;
            parser->data1 = INVALID;
#endif
            break;
        case CC:
        case CC + 1:
        case CC + 2:
        case CC + 3:
        case CC + 4:
        case CC + 5:
        case CC + 6:
        case CC + 7:
        case CC + 8:
        case CC + 9:
        case CC + 10:
        case CC + 11:
        case CC + 12:
        case CC + 13:
        case CC + 14:
        case CC + 15:
#ifdef ALLOW_CC
            processVoiceMessage(parser, c, CC);
#else
            parser->status = INVALID;
            parser->data1 = INVALID;
#endif
            break;
        case PC:
        case PC + 1:
        case PC + 2:
        case PC + 3:
        case PC + 4:
        case PC + 5:
        case PC + 6:
        case PC + 7:
        case PC + 8:
        case PC + 9:
        case PC + 10:
        case PC + 11:
        case PC + 12:
        case PC + 13:
        case PC + 14:
        case PC + 15:
#ifdef ALLOW_PC
            processVoiceMessage(parser, c, PC);
#else
            parser->status = INVALID;
            parser->data1 = INVALID;
#endif
            break;
        case AT:
        case AT + 1:
        case AT + 2:
        case AT + 3:
        case AT + 4:
        case AT + 5:
        case AT + 6:
        case AT + 7:
        case AT + 8:
        case AT + 9:
        case AT + 10:
        case AT + 11:
        case AT + 12:
        case AT + 13:
        case AT + 14:
        case AT + 15:
#ifdef ALLOW_AT
            processVoiceMessage(parser, c, AT);
#else
            parser->status = INVALID;
            parser->data1 = INVALID;
#endif
            break;
        case BEND:
        case BEND + 1:
        case BEND + 2:
        case BEND + 3:
        case BEND + 4:
        case BEND + 5:
        case BEND + 6:
        case BEND + 7:
        case BEND + 8:
        case BEND + 9:
        case BEND + 10:
        case BEND + 11:
        case BEND + 12:
        case BEND + 13:
        case BEND + 14:
        case BEND + 15:
#ifdef ALLOW_BEND                                               
            processVoiceMessage(parser, c, BEND);
#else
            parser->status = INVALID;
            parser->data1 = INVALID;
#endif
            break;
        case SYSTEM_EXCLUSIVE:
#ifdef ALLOW_SYSTEM_EXCLUSIVE                                   
            parser->status = SYSTEM_EXCLUSIVE;
            parser->data1 = INITIALIZED;
#else
            parser->status = INVALID;
            parser->data1 = INVALID;
#endif
            break;
        case MIDI_TIME_CODE_QUARTER_FRAME:
#ifdef ALLOW_MIDI_TIME_CODE_QUARTER_FRAME
            parser->status = MIDI_TIME_CODE_QUARTER_FRAME;
            parser->data1 = INITIALIZED;
#else
            parser->status = INVALID;
            parser->data1 = INVALID;
#endif
            break;
        case SONG_POSITION_POINTER:
#ifdef ALLOW_SONG_POSITION_POINTER
            parser->status = SONG_POSITION_POINTER;
            parser->data1 = INITIALIZED;
#else
            parser->status = INVALID;
            parser->data1 = INVALID;
#endif
            break;
        case SONG_SELECT:
#ifdef ALLOW_SONG_SELECT
            parser->status = SONG_SELECT;
            parser->data1 = INITIALIZED;
#else
            parser->status = INVALID;
            parser->data1 = INVALID;
#endif
            break;
        case UNDEFINED_F4:
        case UNDEFINED_F5:
            // Do Nothing
            break;
        case TUNE_REQUEST:
#ifdef ALLOW_TUNE_REQUEST
            if (parser->unvoicedMessages) tuneRequest(parser);
#endif
            break;
        case END_OF_SYSEX:
            // Do Nothing
            break;
        case CLOCK_PULSE:
#ifdef ALLOW_CLOCK
            if (parser->unvoicedMessages) clockPulse(parser);
#endif
            break;
        case UNDEFINED_F9:
            // Do Nothing
            break;
        case CLOCK_START:
#ifdef ALLOW_CLOCK
            if (parser->unvoicedMessages) clockStart(parser);
#endif
            break;
        case CLOCK_CONTINUE:
#ifdef ALLOW_CLOCK
            if (parser->unvoicedMessages) clockContinue(parser);
#endif
            break;
        case CLOCK_STOP:
#ifdef ALLOW_CLOCK
            if (parser->unvoicedMessages) clockStop(parser);
#endif
            break;
        case UNDEFINED_FD:
            // Do Nothing
            break;
        case ACTIVE_SENSING:
#ifdef ALLOW_ACTIVE_SENSING
            if (parser->unvoicedMessages) activeSensing(parser);
#endif
            break;
        case SYSTEM_RESET:
#ifdef ALLOW_SYSTEM_RESET
            if (parser->unvoicedMessages) systemReset(parser);
#endif
            break;
        default:
            // It's a data byte
            if (parser->data1 == INITIALIZED)       // first data byte
                {
                processFirstDataByte(parser, c);
                }
            else if (parser->data1 != INVALID)      // second data byte
                {
                processSecondDataByte(parser, c);
                }
            break;
        }
    }

// Returns the parser's MIDI channel
unsigned char getParserMIDIChannel(midiParser* parser)
    {
    return parser->channel;
    }

// Returns the parser's MIDI channel
unsigned char getParserCurrentMIDIChannel(midiParser* parser)
    {
    return parser->currentChannel;
    }

// Returns the parser's tag
unsigned char getParserTag(midiParser* parser)
    {
    return parser->tag;
    }

