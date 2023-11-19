WONT_COMPILE_ON_PURPOSE_PLEASE_SEE_TEXT_FOR_WARNING

/// WARNING
///
/// This program uses IN3 as an OUTPUT.  This works fine normally.
/// However on the Arduino, a pin used as an output must have a resistor
/// placed in series, because if you short it to ground, or to 5V, you
/// will destroy the pin otherwise.  You don't need this for pins used as INPUTS.
/// It is not yet clear from TangibleWaves if IN3 has a resistor.
/// This program works great if you connect IN3 to trigger
/// inputs, but I am concerned people will damage their GRAINS by attaching 
/// to outputs (probably okay), ground, or 5V.



// Copyright 2023 Sean Luke 
// (sean@cs.gmu.edu)
//
// Some portions, as noted, written by or adapted from Emilie Gillet 
// (emilie.o.gillet@gmail.com)
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.



/// LATTICE
///
/// Lattice is a "Grids Light" (or in AE-speak, a "TOPOGRAPH Light") which
/// runs on the Tangible Waves AE-Modular GRAINS module, though you could
/// modify it to run on most any arduino.  Lattice was written by Sean Luke 
/// [sean@cs.gmu.edu]
///
/// SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)
///
/// It implements much of Mutable Instruments's Grids algorithm except
/// for swing, accents, and internal tempo.  It is not nearly as convenient as
/// TOPOGRAF because Grains does not have as many knobs and sockets exposed,
/// but it can do quite a lot and has a variety of configuration options.
/// 
/// AE's Grains module has a limited number of inputs and outputs, so we must
/// be clever about how to use them.  There are three configurations:
/// 
/// - A SIMPLE configuration
/// - A SIMPLE "chaos" configuration
/// - A COMPLEX configuration.  
///
/// Let's handle the SIMPLE configuration first:
///
/// SIMPLE CONFIGURATION
///
/// IN 1            Clock In, plus X
/// IN 2            Y
/// IN 3            Drum 3 Trigger Out    [yes, it's changed to an OUT]
/// AUDIO IN (A)    Reset In
/// AUDIO OUT       Drum 1 Trigger Out
/// DIGITAL OUT (D) Drum 2 Trigger Out
///
/// POT 1           X
///                   [Set to "In 1"]
///                   If you turn it to zero, clock is paused (ignored)
///                   Otherwise, the range is from a little past zero (roughly 9 o'clock)
///                   to about 2 o'clock [due to GRAINS bug, see discussion below]
///                   
/// POT 2           Y
///                   [Set to "In 2" if using In 2, else "Man"]
///                   The range is from a little past zero (roughly 9 o'clock)
///                   to about 2 o'clock if set to "In 2" [due to GRAINS bug, see discussion below],
///                   or to fully right if set to "Man"
/// POT 3           Complexity for ALL THREE DRUMS
///
///
/// If you set POT 1 to less than 1/8, it will just output zero.  If you turn POT 1 fully left,
/// it will also turn the clock off.
///
/// IN 2 is an alternative CV in for POT 2, and if you set Pot 2 to "In 2", Pot 2 will 
/// temper the CV, else if you set it to "Man", it will just use Pot 2 (no CV).
///
/// IN 1 is ***BOTH*** a CV for POT 1 ***AND*** a CLOCK source.  You should ALWAYS set 
/// Pot 1 to "In 2", *not* to "Man".  If you don't care about the CV, just provide a clock 
/// signal and you're done.  If you care about the CV, the CV is simply the voltage of the clock.
/// You can do this by putting a 5V CLOCK signal into the INPUT of a 2VCA module set to **DC**.
/// Put the CV signal into the CV of the 2VCA module.  The output of the 2VCA is the two multiplied,
/// so take that and feed it into IN 1 on your GRAINS.
///
/// USING DRUMKIT010
///
/// Be sure to set DrumKit1010's TRIG MODE switch to "D" (Digital), so it properly 
/// responds to triggers.
///
/// GRAINS BUG.  If you set Pot P1 or P2 to "Man" then these pot have full range.  But if you set
/// them to "In1 1" or "In 2", then the maximum possible value is no longer at full right, but rather
/// is at about the 2 o'clock position: the range is cut down by 1/3.  I think it is an error in the 
/// GRAINS pot circuitry.  This is a particular problem for us because P1 **must** be set to "In 1"
/// due to the incoming clock.  Additionally, we take out about 1/8 at the other end to handle the
/// clock not being on, so the total range of these pots, at "In 1" or "In 2", is about 1/2.  :-( 
///
/// The Chaos is fixed to 0 (no randomness), but you can change it by changing the following #define.
/// You can set it to any value from 0 to 255 (maximum chaos).  Do not exceed those values.

#define CHAOS 0


///
///
///
/// SIMPLE "CHAOS" CONFIGURATION
///
/// This is turned on by uncommenting (removing the two slashes in the front) from the following line:

// #define CHAOTIC

/// In this configuration, POT 3 does not set complexity: instead it sets the degree of chaos.  
/// The complexity for all three drums is set to 127, but you can change them by changing
/// the following #defines.  The values you can set are between 0 and 255 -- do not go outside these
/// ranges.

#define DRUM1_COMPLEXITY  127
#define DRUM2_COMPLEXITY  127
#define DRUM3_COMPLEXITY  127



///
///
/// 
/// COMPLEX CONFIGURATION
///
/// This is turned on by uncommenting (removing the two slashes in the front) from the following line:

// #define COMPLEX

/// (The COMPLEX define takes precedence over the CHAOS define)
///
/// The Complex configuration changes POT 3 into an OPTION pot for changing the meaning of
/// IN 1 and IN 2.  
///
/// The OPTION potentiometer can be set to three values: WEST, NORTH, and EAST.
/// When turned WEST, then Pots/Inputs 1 and 2 control the X and Y dimensions of the
/// grid as usual.  When turned NORTH, the Pots/Inputs 1 and 2 control the Drum 1 and Drum 2
/// complexity.  Finally, when turned EAST, the Pots/Inputs 1 and 2 control the Drum 3
/// complexity and Chaos.
/// 
/// This gives you full access to customizing the complexity and chaos of the three drums.
/// However, as you move the option dial from option to option, you'll have to readjust 
/// the potentiometers.  For example, if the dial is at WEST and you have set up X and Y
/// parameters, then you turn the dial to NORTH, the Drum 1 Complexity and Drum 2 Complexity
/// parameters will suddenly get set to those values!  Okay so you change them back to the
/// values you want.  Then you turn the dial to EAST, an the Drum 3 Complexity and Chaos
/// parameters will suddenly get set to the new values again!  This will get tiring quickly.
///
/// ABOUT THE EEPROM
///
/// In complex mode, Lattice saves the settings you had made to EEPROM.  These are stored in
/// the first five bytes of the EEPROM.  Initially the EEPROM has 255 for these values,
/// the first time you'll initially get crazy results until you set them manually.  You'll also
/// get crazy results if you use any other program which saves to EEPROM, possibly overwriting
/// your values.  Just so you knew.



#include <avr/pgmspace.h>
#include <EEPROM.h>


////////// PINOUTS

#define CV_POT_IN1    A2    // X
#define CV_POT_IN2    A1    // Y, Clock
#define CV_POT3       A0    // Complexity or Perturbation
#define CV_IN3        A3    // Drum 3
#define CV_AUDIO_IN   A4    // Reset
#define CV_AUDIO_OUT  11    // Drum 1
#define CV_GATE_OUT   8     // Drum 2

#define RANDOM_PIN              A5

#define RESET_IN CV_AUDIO_IN
#define DRUM_1 CV_GATE_OUT
#define DRUM_2 CV_AUDIO_OUT
#define DRUM_3 CV_IN3




////////// SEQUENCER STRUCTURE

#define NUM_DRUMS 3
#define NUM_STEPS_PER_PATTERN 32

/// These are the global values for the x, y, complexity, and chaos parameters
uint8_t x = 0;
uint8_t y = 0;
uint8_t complexity[NUM_DRUMS] = { 0, 0, 0};
uint8_t chaos = 0;



////////// EEPROM HANDLING

/// These are the address locations for each of these six bytes
#define X_EEPROM 0
#define Y_EEPROM 1
#define COMPLEXITY1_EEPROM 2
#define COMPLEXITY2_EEPROM 3
#define COMPLEXITY3_EEPROM 4
#define CHAOS_EEPROM 5

// Loads parameters from EEPROM
void loadEEPROM()
    {
    x = EEPROM.read(X_EEPROM);
    y = EEPROM.read(Y_EEPROM);
    complexity[0] = EEPROM.read(COMPLEXITY1_EEPROM);
    complexity[1] = EEPROM.read(COMPLEXITY2_EEPROM);
    complexity[2] = EEPROM.read(COMPLEXITY3_EEPROM);
    chaos = EEPROM.read(CHAOS_EEPROM);
    }

// Saves parameters to EEPROM.  Don't do this more than 100,000 times.
void saveEEPROM()
    {
    EEPROM.update(X_EEPROM, x);
    EEPROM.update(Y_EEPROM, y);
    EEPROM.update(COMPLEXITY1_EEPROM, complexity[0]);
    EEPROM.update(COMPLEXITY2_EEPROM, complexity[1]);
    EEPROM.update(COMPLEXITY3_EEPROM, complexity[2]);
    EEPROM.update(CHAOS_EEPROM, chaos);
    }




////////// DRUM MAP

//// This drum data is from Mutable Instruments Grids's code (resources.cc)
const uint8_t node_0[] PROGMEM = {
    255,      0,      0,      0,      0,      0,    145,      0,
    0,      0,      0,      0,    218,      0,      0,      0,
    72,      0,     36,      0,    182,      0,      0,      0,
    109,      0,      0,      0,     72,      0,      0,      0,
    36,      0,    109,      0,      0,      0,      8,      0,
    255,      0,      0,      0,      0,      0,     72,      0,
    0,      0,    182,      0,      0,      0,     36,      0,
    218,      0,      0,      0,    145,      0,      0,      0,
    170,      0,    113,      0,    255,      0,     56,      0,
    170,      0,    141,      0,    198,      0,     56,      0,
    170,      0,    113,      0,    226,      0,     28,      0,
    170,      0,    113,      0,    198,      0,     85,      0,
    };
const uint8_t node_1[] PROGMEM = {
    229,      0,     25,      0,    102,      0,     25,      0,
    204,      0,     25,      0,     76,      0,      8,      0,
    255,      0,      8,      0,     51,      0,     25,      0,
    178,      0,     25,      0,    153,      0,    127,      0,
    28,      0,    198,      0,     56,      0,     56,      0,
    226,      0,     28,      0,    141,      0,     28,      0,
    28,      0,    170,      0,     28,      0,     28,      0,
    255,      0,    113,      0,     85,      0,     85,      0,
    159,      0,    159,      0,    255,      0,     63,      0,
    159,      0,    159,      0,    191,      0,     31,      0,
    159,      0,    127,      0,    255,      0,     31,      0,
    159,      0,    127,      0,    223,      0,     95,      0,
    };
const uint8_t node_2[] PROGMEM = {
    255,      0,      0,      0,    127,      0,      0,      0,
    0,      0,    102,      0,      0,      0,    229,      0,
    0,      0,    178,      0,    204,      0,      0,      0,
    76,      0,     51,      0,    153,      0,     25,      0,
    0,      0,    127,      0,      0,      0,      0,      0,
    255,      0,    191,      0,     31,      0,     63,      0,
    0,      0,     95,      0,      0,      0,      0,      0,
    223,      0,      0,      0,     31,      0,    159,      0,
    255,      0,     85,      0,    148,      0,     85,      0,
    127,      0,     85,      0,    106,      0,     63,      0,
    212,      0,    170,      0,    191,      0,    170,      0,
    85,      0,     42,      0,    233,      0,     21,      0,
    };
const uint8_t node_3[] PROGMEM = {
    255,      0,    212,      0,     63,      0,      0,      0,
    106,      0,    148,      0,     85,      0,    127,      0,
    191,      0,     21,      0,    233,      0,      0,      0,
    21,      0,    170,      0,      0,      0,     42,      0,
    0,      0,      0,      0,    141,      0,    113,      0,
    255,      0,    198,      0,      0,      0,     56,      0,
    0,      0,     85,      0,     56,      0,     28,      0,
    226,      0,     28,      0,    170,      0,     56,      0,
    255,      0,    231,      0,    255,      0,    208,      0,
    139,      0,     92,      0,    115,      0,     92,      0,
    185,      0,     69,      0,     46,      0,     46,      0,
    162,      0,     23,      0,    208,      0,     46,      0,
    };
const uint8_t node_4[] PROGMEM = {
    255,      0,     31,      0,     63,      0,     63,      0,
    127,      0,     95,      0,    191,      0,     63,      0,
    223,      0,     31,      0,    159,      0,     63,      0,
    31,      0,     63,      0,     95,      0,     31,      0,
    8,      0,      0,      0,     95,      0,     63,      0,
    255,      0,      0,      0,    127,      0,      0,      0,
    8,      0,      0,      0,    159,      0,     63,      0,
    255,      0,    223,      0,    191,      0,     31,      0,
    76,      0,     25,      0,    255,      0,    127,      0,
    153,      0,     51,      0,    204,      0,    102,      0,
    76,      0,     51,      0,    229,      0,    127,      0,
    153,      0,     51,      0,    178,      0,    102,      0,
    };
const uint8_t node_5[] PROGMEM = {
    255,      0,     51,      0,     25,      0,     76,      0,
    0,      0,      0,      0,    102,      0,      0,      0,
    204,      0,    229,      0,      0,      0,    178,      0,
    0,      0,    153,      0,    127,      0,      8,      0,
    178,      0,    127,      0,    153,      0,    204,      0,
    255,      0,      0,      0,     25,      0,     76,      0,
    102,      0,     51,      0,      0,      0,      0,      0,
    229,      0,     25,      0,     25,      0,    204,      0,
    178,      0,    102,      0,    255,      0,     76,      0,
    127,      0,     76,      0,    229,      0,     76,      0,
    153,      0,    102,      0,    255,      0,     25,      0,
    127,      0,     51,      0,    204,      0,     51,      0,
    };
const uint8_t node_6[] PROGMEM = {
    255,      0,      0,      0,    223,      0,      0,      0,
    31,      0,      8,      0,    127,      0,      0,      0,
    95,      0,      0,      0,    159,      0,      0,      0,
    95,      0,     63,      0,    191,      0,      0,      0,
    51,      0,    204,      0,      0,      0,    102,      0,
    255,      0,    127,      0,      8,      0,    178,      0,
    25,      0,    229,      0,      0,      0,     76,      0,
    204,      0,    153,      0,     51,      0,     25,      0,
    255,      0,    226,      0,    255,      0,    255,      0,
    198,      0,     28,      0,    141,      0,     56,      0,
    170,      0,     56,      0,     85,      0,     28,      0,
    170,      0,     28,      0,    113,      0,     56,      0,
    };
const uint8_t node_7[] PROGMEM = {
    223,      0,      0,      0,     63,      0,      0,      0,
    95,      0,      0,      0,    223,      0,     31,      0,
    255,      0,      0,      0,    159,      0,      0,      0,
    127,      0,     31,      0,    191,      0,     31,      0,
    0,      0,      0,      0,    109,      0,      0,      0,
    218,      0,      0,      0,    182,      0,     72,      0,
    8,      0,     36,      0,    145,      0,     36,      0,
    255,      0,      8,      0,    182,      0,     72,      0,
    255,      0,     72,      0,    218,      0,     36,      0,
    218,      0,      0,      0,    145,      0,      0,      0,
    255,      0,     36,      0,    182,      0,     36,      0,
    182,      0,      0,      0,    109,      0,      0,      0,
    };
const uint8_t node_8[] PROGMEM = {
    255,      0,      0,      0,    218,      0,      0,      0,
    36,      0,      0,      0,    218,      0,      0,      0,
    182,      0,    109,      0,    255,      0,      0,      0,
    0,      0,      0,      0,    145,      0,     72,      0,
    159,      0,      0,      0,     31,      0,    127,      0,
    255,      0,     31,      0,      0,      0,     95,      0,
    8,      0,      0,      0,    191,      0,     31,      0,
    255,      0,     31,      0,    223,      0,     63,      0,
    255,      0,     31,      0,     63,      0,     31,      0,
    95,      0,     31,      0,     63,      0,    127,      0,
    159,      0,     31,      0,     63,      0,     31,      0,
    223,      0,    223,      0,    191,      0,    191,      0,
    };
const uint8_t node_9[] PROGMEM = {
    226,      0,     28,      0,     28,      0,    141,      0,
    8,      0,      8,      0,    255,      0,      8,      0,
    113,      0,     28,      0,    198,      0,     85,      0,
    56,      0,    198,      0,    170,      0,     28,      0,
    8,      0,     95,      0,      8,      0,      8,      0,
    255,      0,     63,      0,     31,      0,    223,      0,
    8,      0,     31,      0,    191,      0,      8,      0,
    255,      0,    127,      0,    127,      0,    159,      0,
    115,      0,     46,      0,    255,      0,    185,      0,
    139,      0,     23,      0,    208,      0,    115,      0,
    231,      0,     69,      0,    255,      0,    162,      0,
    139,      0,    115,      0,    231,      0,     92,      0,
    };
const uint8_t node_10[] PROGMEM = {
    145,      0,      0,      0,      0,      0,    109,      0,
    0,      0,      0,      0,    255,      0,    109,      0,
    72,      0,    218,      0,      0,      0,      0,      0,
    36,      0,      0,      0,    182,      0,      0,      0,
    0,      0,    127,      0,    159,      0,    127,      0,
    159,      0,    191,      0,    223,      0,     63,      0,
    255,      0,     95,      0,     31,      0,     95,      0,
    31,      0,      8,      0,     63,      0,      8,      0,
    255,      0,      0,      0,    145,      0,      0,      0,
    182,      0,    109,      0,    109,      0,    109,      0,
    218,      0,      0,      0,     72,      0,      0,      0,
    182,      0,     72,      0,    182,      0,     36,      0,
    };
const uint8_t node_11[] PROGMEM = {
    255,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,
    255,      0,      0,      0,    218,      0,     72,     36,
    0,      0,    182,      0,      0,      0,    145,    109,
    0,      0,    127,      0,      0,      0,     42,      0,
    212,      0,      0,    212,      0,      0,    212,      0,
    0,      0,      0,      0,     42,      0,      0,      0,
    255,      0,      0,      0,    170,    170,    127,     85,
    145,      0,    109,    109,    218,    109,     72,      0,
    145,      0,     72,      0,    218,      0,    109,      0,
    182,      0,    109,      0,    255,      0,     72,      0,
    182,    109,     36,    109,    255,    109,    109,      0,
    };
const uint8_t node_12[] PROGMEM = {
    255,      0,      0,      0,    255,      0,    191,      0,
    0,      0,      0,      0,     95,      0,     63,      0,
    31,      0,      0,      0,    223,      0,    223,      0,
    0,      0,      8,      0,    159,      0,    127,      0,
    0,      0,     85,      0,     56,      0,     28,      0,
    255,      0,     28,      0,      0,      0,    226,      0,
    0,      0,    170,      0,     56,      0,    113,      0,
    198,      0,      0,      0,    113,      0,    141,      0,
    255,      0,     42,      0,    233,      0,     63,      0,
    212,      0,     85,      0,    191,      0,    106,      0,
    191,      0,     21,      0,    170,      0,      8,      0,
    170,      0,    127,      0,    148,      0,    148,      0,
    };
const uint8_t node_13[] PROGMEM = {
    255,      0,      0,      0,      0,      0,     63,      0,
    191,      0,     95,      0,     31,      0,    223,      0,
    255,      0,     63,      0,     95,      0,     63,      0,
    159,      0,      0,      0,      0,      0,    127,      0,
    72,      0,      0,      0,      0,      0,      0,      0,
    255,      0,      0,      0,      0,      0,      0,      0,
    72,      0,     72,      0,     36,      0,      8,      0,
    218,      0,    182,      0,    145,      0,    109,      0,
    255,      0,    162,      0,    231,      0,    162,      0,
    231,      0,    115,      0,    208,      0,    139,      0,
    185,      0,     92,      0,    185,      0,     46,      0,
    162,      0,     69,      0,    162,      0,     23,      0,
    };
const uint8_t node_14[] PROGMEM = {
    255,      0,      0,      0,     51,      0,      0,      0,
    0,      0,      0,      0,    102,      0,      0,      0,
    204,      0,      0,      0,    153,      0,      0,      0,
    0,      0,      0,      0,     51,      0,      0,      0,
    0,      0,      0,      0,      8,      0,     36,      0,
    255,      0,      0,      0,    182,      0,      8,      0,
    0,      0,      0,      0,     72,      0,    109,      0,
    145,      0,      0,      0,    255,      0,    218,      0,
    212,      0,      8,      0,    170,      0,      0,      0,
    127,      0,      0,      0,     85,      0,      8,      0,
    255,      0,      8,      0,    170,      0,      0,      0,
    127,      0,      0,      0,     42,      0,      8,      0,
    };
const uint8_t node_15[] PROGMEM = {
    255,      0,      0,      0,      0,      0,      0,      0,
    36,      0,      0,      0,    182,      0,      0,      0,
    218,      0,      0,      0,      0,      0,      0,      0,
    72,      0,      0,      0,    145,      0,    109,      0,
    36,      0,     36,      0,      0,      0,      0,      0,
    255,      0,      0,      0,    182,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,    109,
    218,      0,      0,      0,    145,      0,     72,     72,
    255,      0,     28,      0,    226,      0,     56,      0,
    198,      0,      0,      0,      0,      0,     28,     28,
    170,      0,      0,      0,    141,      0,      0,      0,
    113,      0,      0,      0,     85,     85,     85,     85,
    };
const uint8_t node_16[] PROGMEM = {
    255,      0,      0,      0,      0,      0,     95,      0,
    0,      0,    127,      0,      0,      0,      0,      0,
    223,      0,     95,      0,     63,      0,     31,      0,
    191,      0,      0,      0,    159,      0,      0,      0,
    0,      0,     31,      0,    255,      0,      0,      0,
    0,      0,     95,      0,    223,      0,      0,      0,
    0,      0,     63,      0,    191,      0,      0,      0,
    0,      0,      0,      0,    159,      0,    127,      0,
    141,      0,     28,      0,     28,      0,     28,      0,
    113,      0,      8,      0,      8,      0,      8,      0,
    255,      0,      0,      0,    226,      0,      0,      0,
    198,      0,     56,      0,    170,      0,     85,      0,
    };
const uint8_t node_17[] PROGMEM = {
    255,      0,      0,      0,      8,      0,      0,      0,
    182,      0,      0,      0,     72,      0,      0,      0,
    218,      0,      0,      0,     36,      0,      0,      0,
    145,      0,      0,      0,    109,      0,      0,      0,
    0,      0,     51,     25,     76,     25,     25,      0,
    153,      0,      0,      0,    127,    102,    178,      0,
    204,      0,      0,      0,      0,      0,    255,      0,
    0,      0,    102,      0,    229,      0,     76,      0,
    113,      0,      0,      0,    141,      0,     85,      0,
    0,      0,      0,      0,    170,      0,      0,      0,
    56,     28,    255,      0,      0,      0,      0,      0,
    198,      0,      0,      0,    226,      0,      0,      0,
    };
const uint8_t node_18[] PROGMEM = {
    255,      0,      8,      0,     28,      0,     28,      0,
    198,      0,     56,      0,     56,      0,     85,      0,
    255,      0,     85,      0,    113,      0,    113,      0,
    226,      0,    141,      0,    170,      0,    141,      0,
    0,      0,      0,      0,      0,      0,      0,      0,
    255,      0,      0,      0,    127,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,
    63,      0,      0,      0,    191,      0,      0,      0,
    255,      0,      0,      0,    255,      0,    127,      0,
    0,      0,     85,      0,      0,      0,    212,      0,
    0,      0,    212,      0,     42,      0,    170,      0,
    0,      0,    127,      0,      0,      0,      0,      0,
    };
const uint8_t node_19[] PROGMEM = {
    255,      0,      0,      0,      0,      0,    218,      0,
    182,      0,      0,      0,      0,      0,    145,      0,
    145,      0,     36,      0,      0,      0,    109,      0,
    109,      0,      0,      0,     72,      0,     36,      0,
    0,      0,      0,      0,    109,      0,      8,      0,
    72,      0,      0,      0,    255,      0,    182,      0,
    0,      0,      0,      0,    145,      0,      8,      0,
    36,      0,      8,      0,    218,      0,    182,      0,
    255,      0,      0,      0,      0,      0,    226,      0,
    85,      0,      0,      0,    141,      0,      0,      0,
    0,      0,      0,      0,    170,      0,     56,      0,
    198,      0,      0,      0,    113,      0,     28,      0,
    };
const uint8_t node_20[] PROGMEM = {
    255,      0,      0,      0,    113,      0,      0,      0,
    198,      0,     56,      0,     85,      0,     28,      0,
    255,      0,      0,      0,    226,      0,      0,      0,
    170,      0,      0,      0,    141,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,
    255,      0,    145,      0,    109,      0,    218,      0,
    36,      0,    182,      0,     72,      0,     72,      0,
    255,      0,      0,      0,      0,      0,    109,      0,
    36,      0,     36,      0,    145,      0,      0,      0,
    72,      0,     72,      0,    182,      0,      0,      0,
    72,      0,     72,      0,    218,      0,      0,      0,
    109,      0,    109,      0,    255,      0,      0,      0,
    };
const uint8_t node_21[] PROGMEM = {
    255,      0,      0,      0,    218,      0,      0,      0,
    145,      0,      0,      0,     36,      0,      0,      0,
    218,      0,      0,      0,     36,      0,      0,      0,
    182,      0,     72,      0,      0,      0,    109,      0,
    0,      0,      0,      0,      8,      0,      0,      0,
    255,      0,     85,      0,    212,      0,     42,      0,
    0,      0,      0,      0,      8,      0,      0,      0,
    85,      0,    170,      0,    127,      0,     42,      0,
    109,      0,    109,      0,    255,      0,      0,      0,
    72,      0,     72,      0,    218,      0,      0,      0,
    145,      0,    182,      0,    255,      0,      0,      0,
    36,      0,     36,      0,    218,      0,      8,      0,
    };
const uint8_t node_22[] PROGMEM = {
    255,      0,      0,      0,     42,      0,      0,      0,
    212,      0,      0,      0,      8,      0,    212,      0,
    170,      0,      0,      0,     85,      0,      0,      0,
    212,      0,      8,      0,    127,      0,      8,      0,
    255,      0,     85,      0,      0,      0,      0,      0,
    226,      0,     85,      0,      0,      0,    198,      0,
    0,      0,    141,      0,     56,      0,      0,      0,
    170,      0,     28,      0,      0,      0,    113,      0,
    113,      0,     56,      0,    255,      0,      0,      0,
    85,      0,     56,      0,    226,      0,      0,      0,
    0,      0,    170,      0,      0,      0,    141,      0,
    28,      0,     28,      0,    198,      0,     28,      0,
    };
const uint8_t node_23[] PROGMEM = {
    255,      0,      0,      0,    229,      0,      0,      0,
    204,      0,    204,      0,      0,      0,     76,      0,
    178,      0,    153,      0,     51,      0,    178,      0,
    178,      0,    127,      0,    102,     51,     51,     25,
    0,      0,      0,      0,      0,      0,      0,     31,
    0,      0,      0,      0,    255,      0,      0,     31,
    0,      0,      8,      0,      0,      0,    191,    159,
    127,     95,     95,      0,    223,      0,     63,      0,
    255,      0,    255,      0,    204,    204,    204,    204,
    0,      0,     51,     51,     51,     51,      0,      0,
    204,      0,    204,      0,    153,    153,    153,    153,
    153,      0,      0,      0,    102,    102,    102,    102,
    };
const uint8_t node_24[] PROGMEM = {
    170,      0,      0,      0,      0,    255,      0,      0,
    198,      0,      0,      0,      0,     28,      0,      0,
    141,      0,      0,      0,      0,    226,      0,      0,
    56,      0,      0,    113,      0,     85,      0,      0,
    255,      0,      0,      0,      0,    113,      0,      0,
    85,      0,      0,      0,      0,    226,      0,      0,
    141,      0,      0,      8,      0,    170,     56,     56,
    198,      0,      0,     56,      0,    141,     28,      0,
    255,      0,      0,      0,      0,    191,      0,      0,
    159,      0,      0,      0,      0,    223,      0,      0,
    95,      0,      0,      0,      0,     63,      0,      0,
    127,      0,      0,      0,      0,     31,      0,      0,
    };

static const uint8_t* drum_map[5][5] = 
    {
    { node_10, node_8, node_0, node_9, node_11 },
    { node_15, node_7, node_13, node_12, node_6 },
    { node_18, node_14, node_4, node_5, node_3 },
    { node_23, node_16, node_21, node_1, node_2 },
    { node_24, node_19, node_17, node_20, node_22 },
    };

//// End drum data






////////// READING AND WRITING PINS

#define COUNT 16                // We wait this long after going high before we go low (or reading a value after it went high)
#define POT_TRIGGER_MAX 32      // Below this position indicates that the user has set Pot1 or Pot2 fully to the left
#define POT_TRIGGER_RANGE 128   // The lowest pot position that changes values for Pot1 or Pot2
#define POT_TRIGGER (-1)        // When the user has set Pot1 or Pot2 fully to the left, the pot1 or pot2 variable is set to this

// current pot values
int16_t pot1 = 0;    // 0 ... 255, or POT_TRIGGER
int16_t pot2 = 0;   // 0 ... 255, or POT_TRIGGER
uint8_t pot3 = 0;    // 0 ... 2 if COMPLEX, else 0...255
uint8_t pot3Changed = 0;   // 0 or 1

// triggered reset
uint8_t reset = 0;
// manual reset
uint8_t manualReset = 0;
// triggered clock
uint8_t clock = 0;
// current step
uint8_t step = 0;
// countup for clock trigger so we're as close to max as we can get
uint8_t countup = 0;

// Drums and drum countdowns for outputs
static const uint8_t drums[3] = { DRUM_1, DRUM_2, DRUM_3 };
static uint8_t count[3] = { 0, 0, 0 };

// current random values, changed each sequence cycle
uint8_t rnd[NUM_DRUMS] = { 0, 0, 0 };


// Counts down since we last triggered a drum, and when we get to 0, we send a 0 out the line
void decrement(uint8_t drum)
    {
    if (count[drum] == 0) return;
    if (count[drum] == 1) { digitalWrite(drums[drum], 0); }
    count[drum]--; 
    }

// Triggers a drum and starts the countdown
void pulse(uint8_t drum)
    {
    digitalWrite(drums[drum], 1);
    count[drum] = COUNT;
    }

// Reads the three pot values and interprets them
void processPots()
    {
    uint16_t p1 = analogRead(CV_POT_IN1);
    uint16_t p2 = analogRead(CV_POT_IN2);
    uint16_t p3 = analogRead(CV_POT3) >> 2;

    if (p1 < POT_TRIGGER_MAX)
        {
        pot1 = POT_TRIGGER;
        }
    else if (p1 < POT_TRIGGER_RANGE)
        {
        pot1 = 0;
        }
    else
        {
// (1024 - POT_TRIGGER_RANGE) = (1024 - 128) = 896
// We want to divide by 896 and multiply by 256
// That is multiplying by 2 and dividing by 7
        pot1 = div7((p1 - POT_TRIGGER_RANGE) * (uint16_t)2);
        }

//   if (p2 < POT_TRIGGER_MAX)
//{
// pot2 = POT_TRIGGER;
// }
//   else if (p2 < POT_TRIGGER_RANGE)
// {
// pot2 = 0;
// }
//   else
// {
// // (1024 - POT_TRIGGER_RANGE) = (1024 - 128) = 896
// // We want to divide by 896 and multiply by 256
// // That is multiplying by 2 and dividing by 7
// pot2 = div7((p2 - POT_TRIGGER_RANGE) * (uint16_t)2);
// }

    pot2 = p2 >> 2;

#ifdef COMPLEX
#define SLACK 10

    pot3Changed = 0;
// Finally, we divide pot 3 into 3 parts

// We do a little trick here to deal with noise
    if (pot3 == 0)
        {
// did we get far enough?
        if (p3 > 170) { pot3Changed = 1; pot3 = 2; }
        else if (p3 > 85 + SLACK) { pot3Changed = 1; pot3 = 1; }
        }
    else if (pot3 == 1)
        {
// did we get far enough?
        if (p3 <= 85 - SLACK) { pot3Changed = 1; pot3 = 0; }
        else if (p3 > 170 + SLACK) { pot3Changed = 1; pot3 = 2; }
        }
    else // pot3 == 2
        {
// did we get far enough?
        if (p3 <= 85) { pot3Changed = 1; pot3 = 0; }
        else if (p3 <= 170 - SLACK) { pot3Changed = 1; pot3 = 1; }
        }
#else
    pot3 = p3;
#endif
    }









////////// COMPUTING AND PLAYING DRUMS

// I think this should work?  It's missing from Mutable's code
uint8_t u8Mix(uint8_t a, uint8_t b, uint8_t balance)
    {
    uint16_t alpha = (uint16_t)balance;
    return (uint8_t)(((a * alpha) + (b * (255 - alpha))) >> 8);
    }


//// This code from Mutable Instruments Grids's code.  It extracts the drum trigger data.
uint8_t readDrumMap(uint8_t step, uint8_t instrument, uint8_t x, uint8_t y) 
    {
    uint8_t i = x >> 6;
    uint8_t j = y >> 6;
    const uint8_t* a_map = drum_map[i][j];
    const uint8_t* b_map = drum_map[i + 1][j];
    const uint8_t* c_map = drum_map[i][j + 1];
    const uint8_t* d_map = drum_map[i + 1][j + 1];
    uint8_t offset = (instrument * NUM_STEPS_PER_PATTERN) + step;
    uint8_t a = pgm_read_byte(a_map + offset);
    uint8_t b = pgm_read_byte(b_map + offset);
    uint8_t c = pgm_read_byte(c_map + offset);
    uint8_t d = pgm_read_byte(d_map + offset);
    uint8_t val = u8Mix(u8Mix(a, b, x << 2), u8Mix(c, d, x << 2), y << 2);
    return val;
    }

//// This code is adapted from Mutable Instruments Grids's code.  It plays all three drums
void play(uint8_t step, uint8_t x, uint8_t y, uint8_t* complexity, uint8_t chaos) 
    {
    for (uint8_t drum = 0; drum < NUM_DRUMS; ++drum) 
        {
        uint8_t threshold = 255 - complexity[drum];
        uint8_t level = readDrumMap(step, drum, x, y);
// NOTE: I don't like Mutable's approach here, since randomness only INCREASES
// note complexity, never DECREASES it below the base value.  Better would be
// to center the randomness at the base value, but whatever -- Sean
        if (level < 255 - rnd[drum]) 
            {
            level += rnd[drum];
            } 
        else 
            {
            level = 255;
            }
        if (level > threshold) 
            {
            pulse(drum);
            }
        }
    }

//// Resets the sequencer to step 0 and turns off all drums
void _reset()
    {
    for(uint8_t drum = 0; drum < NUM_DRUMS; drum++)
        {
        digitalWrite(drums[drum], 0);
        count[drum] = 0;
        }
    step = 0;
    }

//// Clocks the sequencer and increments the step.
//// At the beginning of a sequence, we pick new random values for chaos
void _clock()
    {
    if (step >= NUM_STEPS_PER_PATTERN) 
        {
        step = 0;
        for(uint8_t drum = 0; drum < NUM_DRUMS; drum++)
            {
            rnd[drum] = ((random(64) * (uint16_t)chaos) >> 8);
            }
        }

    play(step, (uint8_t)x, (uint8_t)y, complexity, chaos);


    step++;
    }







////////// TOP-LEVEL


void setup()
    {
    randomSeed(analogRead(RANDOM_PIN));

//// Change the pin modes, even for the analog in
    pinMode(DRUM_1, OUTPUT);
    pinMode(DRUM_2, OUTPUT);
    pinMode(DRUM_3, OUTPUT);

#ifdef COMPLEX
    loadEEPROM();
#endif
//Serial.begin(115200);
    }


// This divides by 7 without doing a division.  Useful.
// Range of 0...32773
uint16_t div7(uint16_t dividend)
    {
    uint32_t invDivisor = 0x2492;
    uint16_t div = (uint16_t) ((invDivisor * (dividend + 1)) >> 16);
    return div;
    }


// The "Simple" version
void simpleLoop()
    {
// Decrement the drum release clock
    for(uint8_t drum = 0; drum < NUM_DRUMS; drum++)
        {
        decrement(drum);
        }

// this throwaway is advisable because Analog In has high impedance,
// and so sometimes it can't charge the capacitors fast enough in the ADC.
// As a result, the PREVIOUS analogRead() may bleed into this one.
// The throwaway blocks the bleed in most cases.  We might need two throwaways.
    digitalRead(RESET_IN);     

// did we reset?
    uint8_t r = digitalRead(RESET_IN);
    if (r == 0)
        {
        reset = 0;
        }
    else if (!reset)
        {
        reset = 1;
        _reset();
        }

    processPots();

// Manual reset trigger
    if (pot2 != POT_TRIGGER)
        {
        manualReset = 0;   // no reset
        }
    else if (!manualReset)
        {
        manualReset = 1;
        _reset();
        } 

// compute x, y, complexities, and chaos
    if (pot1 != POT_TRIGGER)
        x = pot1;
    if (pot2 != POT_TRIGGER)
        y = pot2;
#ifdef CHAOTIC
    chaos = pot3;
    complexity[0] = DRUM1_COMPLEXITY;
    complexity[1] = DRUM2_COMPLEXITY;
    complexity[2] = DRUM3_COMPLEXITY;
#else
    chaos = CHAOS;
    complexity[0] = pot3;
    complexity[1] = pot3;
    complexity[2] = pot3;
#endif

// Clock
    if (pot1 == POT_TRIGGER)    // clock off or no clock
        {
// kill clock
        countup = 0;
        clock = 0;
        }
    else if (!clock) 
        {
        countup++;
        if (countup >= COUNT)
            {
            countup = COUNT;
            clock = 1;  
            _clock();
            }
        }
    }


// The "Complex" version
void complexLoop()
    {
// Decrement the drum release clock
    for(uint8_t drum = 0; drum < NUM_DRUMS; drum++)
        {
        decrement(drum);
        }

// this throwaway is advisable because Analog In has high impedance,
// and so sometimes it can't charge the capacitors fast enough in the ADC.
// As a result, the PREVIOUS analogRead() may bleed into this one.
// The throwaway blocks the bleed in most cases.  We might need two throwaways.
    digitalRead(RESET_IN);     

// did we reset?
    uint8_t r = digitalRead(RESET_IN);
    if (r == 0)
        {
        reset = 0;
        }
    else if (!reset)
        {
        reset = 1;
        _reset();
        }

    processPots();

// Manual reset trigger
    if (pot2 != POT_TRIGGER)
        {
        manualReset = 0;   // no reset
        }
    else if (!manualReset)
        {
        manualReset = 1;
        _reset();
        } 

// compute x, y, complexities, and chaos
    if (pot3Changed)
        {
        saveEEPROM();
        }

// Clock
    if (pot1 == POT_TRIGGER)    // clock off or no clock
        {
// kill clock
        countup = 0;
        clock = 0;
        }
    else if (!clock) 
        {
        countup++;
        if (countup >= COUNT)
            {

// Take a snapshot of current pot values
            if (pot3 == 0)
                {
                if (pot1 != POT_TRIGGER)
                    x = pot1;
                if (pot2 != POT_TRIGGER)
                    y = pot2;
                }
            else if (pot3 == 1)
                {
                if (pot1 != POT_TRIGGER)
                    complexity[0] = pot1;
                if (pot2 != POT_TRIGGER)
                    complexity[1] = pot2;
                }
            else if (pot3 == 2)
                {
                if (pot1 != POT_TRIGGER)
                    complexity[2] = pot1;
                if (pot2 != POT_TRIGGER)
                    chaos = pot2;
                }

// clock it!
            countup = COUNT;
            clock = 1;  
            _clock();
            }
        }
    }


// Top-level loop
void loop()
    {
#ifdef COMPLEX
    complexLoop();
#else
    simpleLoop();
#endif
    }













// This is at line 1000 because my son wanted me to make the file 1000 lines long.
