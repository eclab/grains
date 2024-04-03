#include "parsemodular.h"

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
    CC_TYPE_BREATH,
    CC_TYPE_AUXILIARY,
    S(CC_TYPE_ID_1, 0),
    S(CC_TYPE_ID_1, 1),
    CC_TYPE_DATA_ENTRY,
    CC_TYPE_RESERVED,
    CC_TYPE_RESERVED,
    CC_TYPE_RESERVED,
    CC_TYPE_RESERVED,
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
    CC_TYPE_MODULATION + 0,
    CC_TYPE_MODULATION + 1,
    CC_TYPE_RESERVED,
    CC_TYPE_RESERVED,
    CC_TYPE_RESERVED,
    CC_TYPE_RESERVED,
        
    CC_TYPE_BANK_SELECT_LSB,
    CC_TYPE_MOD_WHEEL_LSB,
    CC_TYPE_BREATH_LSB,
    CC_TYPE_AUXILIARY_LSB,
    S(CC_TYPE_ID_1, 8),
    S(CC_TYPE_ID_1, 7),
    CC_TYPE_DATA_ENTRY_LSB,
    CC_TYPE_RESERVED,
    CC_TYPE_RESERVED,
    CC_TYPE_RESERVED,
    CC_TYPE_RESERVED,
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
    CC_TYPE_RESERVED,
    CC_TYPE_RESERVED,
    CC_TYPE_RESERVED,
    CC_TYPE_RESERVED,
        
    CC_TYPE_SUSTAIN,
    S(CC_TYPE_ID_1, 2),
    S(CC_TYPE_ID_1, 3),
    S(CC_TYPE_ID_1, 4),
    S(CC_TYPE_ID_1, 5),
    S(CC_TYPE_ID_1, 6),
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
    CC_TYPE_MODULATION + 2,
    CC_TYPE_MODULATION + 3,
    CC_TYPE_MODULATION + 4,
    CC_TYPE_MODULATION + 5,
    CC_TYPE_MODULATION + 6,
    CC_TYPE_MODULATION + 7,
    CC_TYPE_RESERVED,
    CC_TYPE_RESERVED,
    CC_TYPE_ALL_SOUND_OFF,
    CC_TYPE_RESET_ALL_CONTROLLERS,  
    CC_TYPE_RESERVED,
    CC_TYPE_ALL_NOTES_OFF,
    CC_TYPE_RESERVED,
    CC_TYPE_RESERVED,
    CC_TYPE_RESERVED,
    CC_TYPE_RESERVED,
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
#define CC_TYPE(data) (data >= CC_TYPE_MODULATION ? data : (data & 7))

signed char getCCType(unsigned char cc)
	{
	unsigned char data = getCCData(cc);
	return CC_TYPE(data);
	}
	
// Returns the CC PARAMETER for the given CC Data, or CC_PARAM_NONE if there is no parameter.  This is an UNSIGNED char.
#define CC_PARAM(data) (data >= CC_TYPE_AUXILIARY ? CC_PARAM_NONE : (data >= CC_TYPE_MODULATION ? data - CC_TYPE_MODULATION : (data >> 3)))

unsigned char getCCParam(unsigned char cc)
	{
	unsigned char data = getCCData(cc);
	return CC_PARAM(data);
	}
	
signed char getNRPNType(UNSIGNED_16_BIT_INT nrpnParam)
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

void setBothHighResCCsUsed(midiParser* parser)
    {
    for(unsigned char cc = 0; cc < 32; cc++)
        setHighResUsed(parser, cc, 1);
    }
        
void setFirstHighResCCUsed(midiParser* parser)
    {
    for(unsigned char cc = 0; cc < 32; cc++)
        if (CC_TYPE(getCCData(cc)) <= CC_TYPE_ID_8)
            setHighResUsed(parser, cc, (cc & 1) == 0);        // it's even
        else
            setHighResUsed(parser, cc, 1);
    }
        
void setNoHighResCCsUsed(midiParser* parser)
    {
    for(unsigned char cc = 0; cc < 32; cc++)
        if (CC_TYPE(getCCData(cc)) <= CC_TYPE_ID_8)
            setHighResUsed(parser, cc, 0);
        else
            setHighResUsed(parser, cc, 1);
    }
