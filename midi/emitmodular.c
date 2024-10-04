/// Copyright 2024 by Sean Luke
/// Open Source 
/// Licensed under the Apache 2.0 License

/// Version 0.3:        "Passes Tests, Modular Stuff Added"



#include "emitmodular.h"

#define AUXILIARY_CC 3

void emitModulationCCParam(struct midiEmitter* emitter, unsigned char param, UNSIGNED_16_BIT_INT value, unsigned char channel)
	{
	if (param < 2)
		{
		emitHighResCC(emitter, 26 + param, value, channel);
		}
	else
		{
		emitCC(emitter, 112 + param - 2, value >> 7, channel);
		}
	}
	
#ifdef __AVR__
const PROGMEM unsigned char _PARAM_CC_DATA[8][9] =
#else		// __AVR__
const unsigned char _PARAM_CC_DATA[8][9] =
#endif		// __AVR__
    {
    // This table is due to Mathias Brussel
    {  4,   5,  65,  66,  67,  68,  69,  37,  36 }, 
    { 14,  15,  70,  71,  72,  73,  75,  47,  46 },
    { 16,  17,  76,  77,  78,  79,  80,  49,  48 },
    { 18,  19,  81,  82,  83,  84,  85,  51,  50 },
    { 20,  21,  86,  87,  88,  89,  90,  53,  52 },
    { 12,  13,  91,  92,  93,  94,  95,  45,  44 },
    { 22,  23, 102, 103, 104, 105, 106,  55,  54 },
    { 24,  25, 107, 108, 109, 110, 111,  57,  56 }
    };

void emit7BitCCParam(struct midiEmitter* emitter, unsigned char id, unsigned char param, unsigned char value, unsigned char channel)
	{
#ifdef __AVR__
	unsigned char pos = pgm_read_byte_near(&_PARAM_CC_DATA[id][param]);
#else		// __AVR__
	unsigned char pos = _PARAM_CC_DATA[id][param];
#endif		// __AVR__
	emitCC(emitter, pos, value, channel);
	}
	
void emit14BitCCParam(struct midiEmitter* emitter, unsigned char id, unsigned char param, UNSIGNED_16_BIT_INT value, unsigned char channel)	
	{
#ifdef __AVR__
	unsigned char pos = pgm_read_byte_near(&_PARAM_CC_DATA[id][param]);
#else		// __AVR__
	unsigned char pos = _PARAM_CC_DATA[id][param];
#endif		// __AVR__
	emitHighResCC(emitter, pos, value, channel);
	}

void emitAuxiliaryParam(struct midiEmitter* emitter, unsigned char id, unsigned char param, unsigned char value, unsigned char channel)
	{
	unsigned char msb = (id - 8) * 16 + 16 + param;
	unsigned char lsb = value;
	emitHighResCC(emitter, AUXILIARY_CC, param * 128 + value, channel);
	}
	
void emitNRPNParam(struct midiEmitter* emitter, unsigned char id, unsigned char param, UNSIGNED_16_BIT_INT value, unsigned char channel)
	{
	emitNRPN(emitter, id + 256 + param, value, 0, channel);
	}
	
void emitProgramSave(struct midiEmitter* emitter, unsigned char bankMSB, unsigned char bankLSB, unsigned char program, unsigned char channel)
	{
	emitCC(emitter, 0, bankMSB, channel);
	emitCC(emitter, 32, bankLSB, channel);
	emitHighResCC(emitter, AUXILIARY_CC, 0 * 128 + program, channel);
	}
	
void emitCurrentProgramSave(struct midiEmitter* emitter, unsigned char channel)
	{
	emitHighResCC(emitter, AUXILIARY_CC, 1 * 128 + 0, channel);
	}
	
void emitCurrentProgramRevert(struct midiEmitter* emitter, unsigned char channel)
	{
	emitHighResCC(emitter, AUXILIARY_CC, 1 * 128 + 1, channel);
	}
	
