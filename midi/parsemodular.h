/// Copyright 2024 by Sean Luke
/// Open Source 
/// Licensed under the Apache 2.0 License

/// Version 0.4:        "Tweaked RPN/NRPN"


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
/// FIRST:
/// Turn ON at least ALLOW_CC, ALLOW_HIGH_RES_CC, ALLOW_RPN_NRPN, ALLOW_PC
/// Turn ON ALLOW_ATOMIC_MODULATION_CC
///
/// This means you'll have to include the functions: 
///
///		void cc(struct midiParser* parser, unsigned char parameter, unsigned char value)
///		void nrpn(struct midiParser* parser, UNSIGNED_16_BIT_INT parameter, UNSIGNED_16_BIT_INT value, unsigned char rpn, unsigned char status)
///		void highResCC(struct midiParser* parser, unsigned char parameter, UNSIGNED_16_BIT_INT value, unsigned char status)
///		void pc(struct midiParser* parser, unsigned char program, unsigned char bankMSB, unsigned char bankLSB)
///
/// ... even if their implementations are empty.
///
///
/// NEXT: 
/// You should set your BANK SELECT, MOD WHEEL, AUXILIARY, DATA ENTRY, and EXPRESSION CONTROLLER CCs to be 14-bit.
/// You should also set MODULATION A and MODULATION B to be 14-bit:
///		setHighResUsed(parser, 0, 1);				// Bank Select
///		setHighResUsed(parser, 1, 1);				// Mod Wheel
///		setHighResUsed(parser, 3, 1);				// Auxiliary
///		setHighResUsed(parser, 6, 1);				// Data Entry
///		setHighResUsed(parser, 11, 1);				// Expression Controller
///		setHighResUsed(parser, 58, 1);				// Modulation A
///		setHighResUsed(parser, 59, 1);				// Modulation B
///
/// The function setStandardHighResParameters(parser) will do this for you as a convenience.
///
///
/// NEXT:
/// Your ID has 9 parameters (CCs) called A, B, C, D, E, F, G, H, I.  
/// You have the option of treating parameters A and I as a combined
/// 14-bit "high res" parameter, which we will then call "A".  To do this, call 
///		setHighResUsed(parser, A's CC, true);			// Where "A's CC" is the CC for Parameter A for your ID
///
/// You further the option of treating parameters B and H as a combined 
/// 14-bit "high res" parameter, which we will then call "A".  To do this, call 
///		setHighResUsed(parser, B's CC, true);			// Where "B's CC" is the CC for Parameter B for your ID
///
/// The method setIDHighResParameters(parser, id, setA, setB) will do this for you as a convenience, including
/// looking up which CC corresponds to the A or B parameters for your ID.
///
/// Note that if you respond to multiple IDs, you can set the A and B parameters for each of them as you see fit.
///
///
/// FINALLY:
/// You have the option of setting certain open CC pairs to be 14-bit if you wished:
///		setHighResUsed(parser, 2, true);				// MSB 2, LSB 34
///		setHighResUsed(parser, 4, true);				// MSB 4, LSB 36
///		setHighResUsed(parser, 28, true);				// MSB 28, LSB 60
///		setHighResUsed(parser, 29, true);				// MSB 29, LSB 61
///		setHighResUsed(parser, 30, true);				// MSB 30, LSB 62
///		setHighResUsed(parser, 31, true);				// MSB 31, LSB 63



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

#define CC_TYPE_ID_1 0						// A CC Belonging to ID 1
#define CC_TYPE_ID_2 1						// A CC Belonging to ID 2
#define CC_TYPE_ID_3 2						// A CC Belonging to ID 3
#define CC_TYPE_ID_4 3						// A CC Belonging to ID 4
#define CC_TYPE_ID_5 4						// A CC Belonging to ID 5
#define CC_TYPE_ID_6 5						// A CC Belonging to ID 6
#define CC_TYPE_ID_7 6						// A CC Belonging to ID 6
#define CC_TYPE_ID_8 7						// A CC Belonging to ID 8

#define CC_TYPE_BANK_SELECT 72				// Bank Select
#define CC_TYPE_MOD_WHEEL 73				// Modulation Wheel
#define CC_TYPE_GLIDE 74					// Glide
#define CC_TYPE_DATA_ENTRY 75				// Data Entry
#define CC_TYPE_VOLUME 76					// Volume
#define CC_TYPE_PAN 77						// Pan
#define CC_TYPE_EXPRESSION 78				// Expression Controller
#define CC_TYPE_SUSTAIN 79					// Sustain Pedal
#define CC_TYPE_LEGATO 80					// Legato Switch
#define CC_TYPE_MPE_TIMBRE 81				// MPE Timbre Controller
#define CC_TYPE_INCREMENT 82				// Increment
#define CC_TYPE_DECREMENT 83				// Decrement
#define CC_TYPE_NRPN_LSB 84					// NRPN LSB
#define CC_TYPE_NRPN_MSB 85					// NRPN MSB
#define CC_TYPE_RPN_LSB 86					// RPN LSB
#define CC_TYPE_RPN_MSB 87					// RPN MSB
#define CC_TYPE_ALL_SOUND_OFF 88			// All Sounds Off
#define CC_TYPE_RESET_ALL_CONTROLLERS 89	// Reset All Controllers
#define CC_TYPE_ALL_NOTES_OFF 90			// All Notes Off

#define CC_TYPE_MODULATION_A 116			// Modulation CC
#define CC_TYPE_MODULATION_B 117			// Modulation CC
#define CC_TYPE_MODULATION_C 118			// Modulation CC
#define CC_TYPE_MODULATION_D 119			// Modulation CC
#define CC_TYPE_MODULATION_E 120			// Modulation CC
#define CC_TYPE_MODULATION_F 121			// Modulation CC
#define CC_TYPE_MODULATION_G 122			// Modulation CC
#define CC_TYPE_MODULATION_H 123			// Modulation CC

#define CC_TYPE_AUXILIARY 124				// Auxiliary CC
#define CC_TYPE_OPEN 125					// Free For Use
#define CC_TYPE_RESERVED 126				// Do Not Use
#define CC_TYPE_RESERVED_NOTES_OFF 127		// Do Not Use, but interpret as an All Notes Off

#define CC_TYPE_BANK_SELECT_LSB (-1)		// The LSB for a Bank Select, with no MSB.  This means you didn't set bank select (CC 0) to be 16-bit via setHighResUsed()
#define CC_TYPE_MOD_WHEEL_LSB (-2)			// The LSB for Mod Wheel, with no MSB.  This means you didn't set mod wheel (CC 1) to be 16-bit via setHighResUsed()
#define CC_TYPE_AUXILIARY_LSB (-3)			// The LSB for Auxiliary, with no MSB.  This means you didn't set auxiliary (CC 3) to be 16-bit via setHighResUsed()
#define CC_TYPE_DATA_ENTRY_LSB (-4)			// The LSB for Data Entry, with no MSB.  This means you didn't set data entry (CC 6) to be 16-bit via setHighResUsed()
#define CC_TYPE_EXPRESSION_LSB (-5)			// The LSB for Expression Controller, with no MSB.  This means you didn't set expression controller (CC 11) to be 16-bit via setHighResUsed()
#define CC_TYPE_MODULATION_A_LSB (-6)		// The LSB for Modulation Parameter A, with no MSB.  This means you didn't set Modulation Parameter A (CC 26) to be 16-bit via setHighResUsed()
#define CC_TYPE_MODULATION_B_LSB (-7)		// The LSB for Modulation Parameter B, with no MSB.  This means you didn't set Modulation Parameter B (CC 27) to be 16-bit via setHighResUsed()

#define CC_PARAM_NONE 255					// Returned when the CC Parameter is requested for a CC that is not associated with an ID


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

#ifdef ALLOW_HIGH_RES_CC
// Sets standard modular parameters to 14-bit.  This includes
// Bank Select, Mod Wheel, Auxiliar, Data Entry, Expression Controller,
// Modulation A, and Modulation B.  This is a convenience method for setting up a parser.
extern void setStandardHighResParameters(struct midiParser* parser);

// Sets select parameters A and/or B to 14-bit for a given ID.  
// This is a convenience method for setting up a parser.
extern void setIDHighResParameters(struct midiParser* parser, unsigned char id, unsigned char setA, unsigned char setB);
#endif

#endif		// __PARSE_MODULAR_H