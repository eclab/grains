/// Copyright 2024 by Sean Luke
/// Open Source 
/// Licensed under the Apache 2.0 License

/// Version 0.4:        "Tweaked RPN/NRPN"


#include "parsemodular.h"
#ifdef __AVR__
#include <avr/pgmspace.h>
#endif

#define CC_TYPE_ID_1 0
#define CC_TYPE_ID_2 1
#define CC_TYPE_ID_3 2
#define CC_TYPE_ID_4 3
#define CC_TYPE_ID_5 4
#define CC_TYPE_ID_6 5
#define CC_TYPE_ID_7 6
#define CC_TYPE_ID_8 7

#define S(id, param) ((param << 3) | id)                                // used internally

#ifdef __AVR__
const PROGMEM signed char _CC_DATA[128] = 
#else		// __AVR__
const signed char _CC_DATA[128] =
#endif		// __AVR__
    {
    CC_TYPE_BANK_SELECT,
    CC_TYPE_MOD_WHEEL,
    CC_TYPE_OPEN,
    CC_TYPE_AUXILIARY,
    CC_TYPE_OPEN,
    CC_TYPE_GLIDE,
    CC_TYPE_DATA_ENTRY,
    CC_TYPE_VOLUME,
    S(CC_TYPE_ID_1, 0),
    S(CC_TYPE_ID_1, 1),
    CC_TYPE_PAN,
    CC_TYPE_EXPRESSION,
    S(CC_TYPE_ID_6, 0),
    S(CC_TYPE_ID_6, 1),
    S(CC_TYPE_ID_2, 0),
    S(CC_TYPE_ID_2, 1),
    S(CC_TYPE_ID_3, 0),
    S(CC_TYPE_ID_3, 1),
    S(CC_TYPE_ID_4, 0),
    S(CC_TYPE_ID_4, 1),
    S(CC_TYPE_ID_5, 0),
    S(CC_TYPE_ID_5, 1),
    S(CC_TYPE_ID_7, 0),
    S(CC_TYPE_ID_7, 1),
    S(CC_TYPE_ID_8, 0),
    S(CC_TYPE_ID_8, 1),
    CC_TYPE_MODULATION_A,
    CC_TYPE_MODULATION_B,
    CC_TYPE_OPEN,
    CC_TYPE_OPEN,
    CC_TYPE_OPEN,
    CC_TYPE_OPEN,
        
    CC_TYPE_BANK_SELECT_LSB,
    CC_TYPE_MOD_WHEEL_LSB,
    CC_TYPE_OPEN,
    CC_TYPE_AUXILIARY_LSB,
    CC_TYPE_OPEN,
    CC_TYPE_OPEN,
    CC_TYPE_DATA_ENTRY_LSB,
    CC_TYPE_OPEN,
    S(CC_TYPE_ID_1, 8),
    S(CC_TYPE_ID_1, 7),
    CC_TYPE_OPEN,
    CC_TYPE_EXPRESSION_LSB,
    S(CC_TYPE_ID_6, 8),
    S(CC_TYPE_ID_6, 7),
    S(CC_TYPE_ID_2, 8),
    S(CC_TYPE_ID_2, 7),
    S(CC_TYPE_ID_3, 8),
    S(CC_TYPE_ID_3, 7),
    S(CC_TYPE_ID_4, 8),
    S(CC_TYPE_ID_4, 7),
    S(CC_TYPE_ID_5, 8),
    S(CC_TYPE_ID_5, 7),
    S(CC_TYPE_ID_7, 8),
    S(CC_TYPE_ID_7, 7),
    S(CC_TYPE_ID_8, 8),
    S(CC_TYPE_ID_8, 7),
    CC_TYPE_MODULATION_A_LSB,
    CC_TYPE_MODULATION_B_LSB,
    CC_TYPE_OPEN,
    CC_TYPE_OPEN,
    CC_TYPE_OPEN,
    CC_TYPE_OPEN,

    CC_TYPE_SUSTAIN,
    S(CC_TYPE_ID_1, 2),
    S(CC_TYPE_ID_1, 3),
    S(CC_TYPE_ID_1, 4),
    CC_TYPE_LEGATO,
    S(CC_TYPE_ID_1, 5),
    S(CC_TYPE_ID_2, 2),
    S(CC_TYPE_ID_2, 3),
    S(CC_TYPE_ID_2, 4),
    S(CC_TYPE_ID_2, 5),
    CC_TYPE_MPE_TIMBRE,
    S(CC_TYPE_ID_2, 6),
    S(CC_TYPE_ID_3, 2),
    S(CC_TYPE_ID_3, 3),
    S(CC_TYPE_ID_3, 4),
    S(CC_TYPE_ID_3, 5),
    S(CC_TYPE_ID_3, 6),
    S(CC_TYPE_ID_4, 2),
    S(CC_TYPE_ID_4, 3),
    S(CC_TYPE_ID_4, 4),
    S(CC_TYPE_ID_4, 5),
    S(CC_TYPE_ID_4, 6),
    S(CC_TYPE_ID_5, 2),
    S(CC_TYPE_ID_5, 3),
    S(CC_TYPE_ID_5, 4),
    S(CC_TYPE_ID_5, 5),
    S(CC_TYPE_ID_5, 6),
    S(CC_TYPE_ID_6, 2),
    S(CC_TYPE_ID_6, 3),
    S(CC_TYPE_ID_6, 4),
    S(CC_TYPE_ID_6, 5),
    S(CC_TYPE_ID_6, 6),
        
    CC_TYPE_INCREMENT,
    CC_TYPE_DECREMENT,
    CC_TYPE_NRPN_LSB,
    CC_TYPE_NRPN_MSB,
    CC_TYPE_RPN_LSB,
    CC_TYPE_RPN_MSB,
    S(CC_TYPE_ID_7, 2),
    S(CC_TYPE_ID_7, 3),
    S(CC_TYPE_ID_7, 4),
    S(CC_TYPE_ID_7, 5),
    S(CC_TYPE_ID_7, 6),
    S(CC_TYPE_ID_8, 2),
    S(CC_TYPE_ID_8, 3),
    S(CC_TYPE_ID_8, 4),
    S(CC_TYPE_ID_8, 5),
    S(CC_TYPE_ID_8, 6),
    S(CC_TYPE_ID_1, 6),
    CC_TYPE_MODULATION_C,
    CC_TYPE_MODULATION_D,
    CC_TYPE_MODULATION_E,
    CC_TYPE_MODULATION_F,
    CC_TYPE_MODULATION_G,
    CC_TYPE_MODULATION_H,
    CC_TYPE_OPEN,
    CC_TYPE_ALL_SOUND_OFF,
    CC_TYPE_RESET_ALL_CONTROLLERS,  
    CC_TYPE_RESERVED,
    CC_TYPE_ALL_NOTES_OFF,
    CC_TYPE_RESERVED_NOTES_OFF,
    CC_TYPE_RESERVED_NOTES_OFF,
    CC_TYPE_RESERVED_NOTES_OFF,
    CC_TYPE_RESERVED_NOTES_OFF,
    };

	
inline unsigned char getCCData(unsigned char cc) 
	{ 
#ifdef __AVR__
	return pgm_read_byte_near(&_CC_DATA[cc]); 
#else		//  __AVR__
	return _CC_DATA[cc];
#endif		// __AVR__
	}

// Returns the CC TYPE for the given CC Data.  This is a SIGNED CHAR.
#define CC_TYPE(data) (data >= CC_TYPE_BANK_SELECT ? data : (data & 7))

signed char getCCType(unsigned char cc)
	{
	unsigned char data = getCCData(cc);
	return CC_TYPE(data);
	}
	
// Returns the CC PARAMETER for the given CC Data, or CC_PARAM_NONE if there is no parameter.  This is an UNSIGNED char.
#define CC_PARAM(data) (data >= CC_TYPE_BANK_SELECT ? (data >= CC_TYPE_MODULATION_A && data <= CC_TYPE_MODULATION_H ? data - CC_TYPE_MODULATION_A : CC_PARAM_NONE) : (data >> 3))

unsigned char getCCParam(unsigned char cc)
	{
	unsigned char data = getCCData(cc);
	return CC_PARAM(data);
	}
	
SIGNED_16_BIT_INT getNRPNType(UNSIGNED_16_BIT_INT nrpnParam)
	{
	if (nrpnParam < 256 || nrpnParam >= 4098)
		{
		return NRPN_TYPE_RESERVED;
		}
	else return (nrpnParam >> 8) - 1;
	}

UNSIGNED_16_BIT_INT getNRPNParam(UNSIGNED_16_BIT_INT nrpnParam)
	{
	if (nrpnParam < 256 || nrpnParam >= 4098)
		{
		return NRPN_PARAM_NONE;
		}
	return nrpnParam - ((nrpnParam >> 8) << 8);
	}

SIGNED_16_BIT_INT getAuxiliaryType(UNSIGNED_16_BIT_INT value)
	{
	if (value < 128)
		{
		return AUXILIARY_TYPE_PROGRAM_SAVE;
		}
	else if (value == 128)
		{
		return AUXILIARY_TYPE_CURRENT_PROGRAM_SAVE;
		}
	else if (value == 129)
		{
		return AUXILIARY_TYPE_CURRENT_PROGRAM_REVERT;
		}
	else if (value < 2048)
		{
		return AUXILIARY_TYPE_RESERVED;
		}
	else return ((value - 2048) >> 7) + 8;
	}

unsigned char getAuxiliaryParam(UNSIGNED_16_BIT_INT value)
	{
	if (value < 128)
		{
		return value;
		}
	else if (value < 2048)
		{
		return AUXILIARY_PARAM_NONE;
		}
	else return value - ((value >> 7) << 7);
	}

#ifdef ALLOW_HIGH_RES_CC			// defined in parsemidi.h
void setStandardHighResParameters(struct midiParser* parser)
	{
	setHighResUsed(parser, 0, 1);				// Bank Select
	setHighResUsed(parser, 1, 1);				// Mod Wheel
	setHighResUsed(parser, 3, 1);				// Auxiliary
	setHighResUsed(parser, 6, 1);				// Data Entry
	setHighResUsed(parser, 11, 1);				// Expression Controller
	setHighResUsed(parser, 58, 1);				// Modulation A
	setHighResUsed(parser, 59, 1);				// Modulation B
	}

void setIDHighResParameters(struct midiParser* parser, uint8_t id, uint8_t setA, uint8_t setB)
	{
	uint8_t a[8] = { 8, 14, 16, 18, 20, 12, 22, 24 };
	if (setA)
		{
		setHighResUsed(parser, a[id], 1);
		}
	if (setB)
		{
		setHighResUsed(parser, a[id] + 1, 1);
		}
	}
#endif

