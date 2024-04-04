/// Copyright 2024 by Sean Luke
/// Open Source 
/// Licensed under the Apache 2.0 License

/// Version 0.3:        "Passes Tests, Modular Stuff Added"


#ifndef __PARSE_MODULAR_H
#define __PARSE_MODULAR_H
#include "parsemidi.h"


/// PARSEMODULAR.H
///
/// This package is intended to make it easier to parse features specific to the Modular MIDI conventions:
///
/// 0. Utilities for setting things up
/// 1. ID Mapping for CCs
/// 2. ID Mapping for NRPN
/// 3. ID Mapping for Auxiliary Parameters
/// 4. Default Type Definitions


/// SETTING THINGS UP
///
/// You will want to set up your parser as follows:
///
/// Turn ON at least ALLOW_CC, ALLOW_HIGH_RES_CC, ALLOW_RPN_NRPN, ALLOW_PC
/// Turn ON ALLOW_ATOMIC_MODULATION_CC
/// 
/// Set up your first 32 CCs according to how you use the two first Parameters in your ID.
///
/// 	- If they are both 14-bit CC, then call setBothHighResCCsUsed(...)
///		- If only the first one is 14-bit, then call setFirstHighResCCUsed(...)
///		- If neither is 14-bit, then call setNoHighResCCsUsed(...)
///


/// PROCESSING A 7-BIT OR 14-BIT CC
///
/// When you receive a 7-bit or 14-bit CC via cc(...) or highResCC(...), you can use certain methods to
/// tell you what ID or TYPE it is and what PARAM it is within that ID.  Call getCCType(..) to determine
/// the ID or type of the CC, and if it's an ID region or a Modulation CC, call getCCParam(...) to determine
/// the PARAM of the CC.
///
/// TYPES
///
/// CC message types include:
///
// A CC for a given parameter for a given ID.  This will be a value 0...7 for types 1 ... 8.
// A Modulation CC
// An Auxiliary CC.  See PROCESSING AN AUXILIARY CC below.
// A System CC, such as Bank Select, Mod Wheel, Breath, Data Entry, Expression, Sustain, MPE Timbre, 
// Increment, Decrement, NRPN LSB, NRPN LSB, RPN LSB, RPN MSB, All Sound Off, or Reset All Controllers
// A Reserved CC.  Ignore these, they should have not been sent.
// A value < 0.  This is likely an ERROR -- it's an LSB of a 14-bit CC that you should NOT have received.
//      Did you turn on the features required above?
//
// Keep in mind that you may receive MSB-ONLY CC values for your 14-bit CCs.  Sometimes these will what you want;
// in other cases you MIGHT be getting the first half of a message (the LSB hasn't arrived yet).  



/// PROCESSING AN AUXILIARY CC
///
/// If getCCType(...) returned CC_TYPE_AUXILIARY, you'll want to dig further to determine the TYPE and the
/// PARAM of the Auxiliary CC.  You can get this information with getAuxiliaryType(...) and getAuxiliaryParam(...)
///
/// TYPES
///
/// Auxiliary message types include:
///
// An Auxiliary message for a given ID.  This will be a value 8...14 for types 9 ... 15
// Program Save
// Current Program Save
// Current Program Revert
// A Reserved message.  Ignore these, they should have not been sent.



/// PROCESSING NRPN
///
/// If you got an NRPN message (not an RPN message), you can extract the ID and the PARAM from it with the
/// getNRPNType(...) and getNRPNParam(...) methods.
///
/// TYPES
///
/// NRPN message types include:
///
// An NRPN message for a given ID.  This will be a value 0...14 for types 1 ... 15
// A Reserved message.  Ignore these, they should have not been sent.





/// ID Defaults.  Maybe this might be useful to you?

#define ID_DEFAULT_OSCILLATOR	0	// Oscillators, Samplers, Note to Gate/Trigger Modules
#define ID_DEFAULT_ENVELOPE		1	// Envelopes
#define ID_DEFAULT_CYCLIC		2	// LFOs, Sequencers, Clock to Gate/Trigger Modules
#define ID_DEFAULT_FILTER		3	// Filters
#define ID_DEFAULT_AMPLIFIER	4	// VCAs, Attenuverters, Amplifiers, Mixers
#define ID_DEFAULT_EFFECT		5	// Effects, Audio Processors
#define ID_DEFAULT_CV			6	// CC to CV Generators
#define ID_DEFAULT_MISC			7	// Miscellaneous


/// CC Type and Param Constants

#define CC_TYPE_MODULATION 72
#define CC_TYPE_AUXILIARY 80
#define CC_TYPE_BANK_SELECT 81
#define CC_TYPE_MOD_WHEEL 82
#define CC_TYPE_BREATH 83
#define CC_TYPE_DATA_ENTRY 84
#define CC_TYPE_EXPRESSION 85
#define CC_TYPE_SUSTAIN 86
#define CC_TYPE_MPE_TIMBRE 87
#define CC_TYPE_INCREMENT 88
#define CC_TYPE_DECREMENT 89
#define CC_TYPE_NRPN_LSB 90
#define CC_TYPE_NRPN_MSB 91
#define CC_TYPE_RPN_LSB 92
#define CC_TYPE_RPN_MSB 93
#define CC_TYPE_ALL_SOUND_OFF 94
#define CC_TYPE_RESET_ALL_CONTROLLERS 95
#define CC_TYPE_ALL_NOTES_OFF 96
#define CC_TYPE_RESERVED 127
#define CC_TYPE_BANK_SELECT_LSB (-1)
#define CC_TYPE_MOD_WHEEL_LSB (-2)
#define CC_TYPE_BREATH_LSB (-3)
#define CC_TYPE_DATA_ENTRY_LSB (-4)
#define CC_TYPE_EXPRESSION_LSB (-5)
#define CC_TYPE_AUXILIARY_LSB (-6)
#define CC_TYPE_MODULATION_A_LSB (-7)
#define CC_TYPE_MODULATION_B_LSB (-8)
#define CC_PARAM_NONE 255


/// NRPN Type and Param Constants

#define NRPN_TYPE_RESERVED (-32768)
#define NRPN_PARAM_NONE 65535


/// Auxiliary Type and Param Constants

#define AUXILIARY_TYPE_PROGRAM_SAVE 0
#define AUXILIARY_TYPE_CURRENT_PROGRAM_SAVE 128
#define AUXILIARY_TYPE_CURRENT_PROGRAM_REVERT 129
#define AUXILIARY_TYPE_RESERVED (-32768)
#define AUXILIARY_PARAM_NONE 255




// Returns the CC TYPE of the given CC parameter. This is one of:
// An ID.  This will be a value 0...7 for IDs 1 ... 8.
// Any of the CC_TYPE values above.
// Note that the negative CC_TYPE values are likely errors and should be discarded,
// Assuming you've got your MIDI set up right.
extern signed char getCCType(unsigned char cc);

// Returns the CC PARAM of the given CC parameter. This is one of:
// A parameter 0...8 representing parameters "a" through "i" for a given ID
// A parameter 0...7 representing the 8 modulation parameters
// CC_PARAM_NONE for various other kinds of CC Types, which have no PARAMs
// You should call getCCType(...) first to determine the type.
extern unsigned char getCCParam(unsigned char cc);

// Returns the NRPN TYPE of the given NRPN parameter. This is one of:
// An ID.  This will be a value 0...14 for IDs 1...15
// NRPN_TYPE_RESERVED otherwise
extern SIGNED_16_BIT_INT getNRPNType(UNSIGNED_16_BIT_INT nrpnParam);

// Returns the NRPN PARAM of the given NRPN parameter.  This is one of:
// 0...255 for Parameters "a", "b", etc. for a given ID
// NRPN_PARAM_NONE for reserved NRPN parameters
extern UNSIGNED_16_BIT_INT getNRPNParam(UNSIGNED_16_BIT_INT nrpnParam);

// Returns the Auxiliary TYPE of the given Auxiliary parameter. This is one of:
// An ID.  This will be a value 8...14 for IDs 9...15
// AUXILIARY_TYPE_PROGRAM_SAVE
// AUXILIARY_TYPE_CURRENT_PROGRAM_SAVE
// AUXILIARY_TYPE_CURRENT_PROGRAM_REVERT
// AUXILIARY_TYPE_RESERVED
extern SIGNED_16_BIT_INT getAuxiliaryType(UNSIGNED_16_BIT_INT value);

// Returns the Auxiliary PARAM of the given Auxiliary parameter. This is one of:
// 0...15 for Parameters "a", "b", etc. for a given ID
// 0...255 indicating the program if the type is AUXILIARY_TYPE_PROGRAM_SAVE
// AUXILIARY_PARAM_NONE otherwise
extern unsigned char getAuxiliaryParam(UNSIGNED_16_BIT_INT value);

// Sets ALL 32 CCs to be high-resolution (14-bit), including the first and second parameters (a, b) of each ID
extern void setBothHighResCCsUsed(midiParser* parser);

// Sets ALL 32 CCs to be high-resolution (14-bit) except the second parameter (b) of each ID
extern void setFirstHighResCCUsed(midiParser* parser);

// Sets ALL 32 CCs to be high-resolution (14-bit) except the first and second parameters (a, b) of each ID
extern void setNoHighResCCsUsed(midiParser* parser);



#endif		// __PARSE_MODULAR_H