# Lightweight MIDI Library

Sean Luke  
December 1, 2024

This is not a GRAINS project, though it is used in several projects.  It is a low-footprint MIDI emitting and parsing library meant for small microprocessors, and contains additional features for the Modular MIDI conventions.  It's currently under development.

For more info on Modular MIDI, see [Modular MIDI Conventions for Developers](https://github.com/eclab/grains/blob/main/info/ModularMIDIConventionsForDevelopers.md).

The library can be used to (1) emit MIDI (2) parse MIDI or (3) do both.  


## Parsing MIDI

### Files
Include the following files in your project:

- parsemidi.h
- parsemidi.c
- miditypes.h

### Usage

Modify the miditypes.h file so that its signed and unsigned 16-bit integer type sizes are correct.  By default it has types set up for the ATmega 328P chip family (Arduino series).

Next, you'll need to modify the parsemidi.h file as described in that file, and write callback functions in your project for the midi types you've asked it to parse.

See the parsemidi.h file for instructions on how to build and use a MIDI parser. 


## Emitting MIDI

### Files
Include the following files in your project:

- emitmidi.h
- emitmidi.c
- miditypes.h

### Usage

Modify the miditypes.h file so that its signed and unsigned 16-bit integer type sizes are correct.  By default it has types set up for the ATmega 328P chip family (Arduino series).

See the emitmidi.h file for instructions on how to build and use a MIDI emitter.


## Testing the Parser and Emitter

### Files
Include the following files in your project:

- emitmidi.h
- emitmidi.c
- parsemidi.h
- parsemidi.c
- miditypes.h
- testmidi.c

### Usage

Modify the miditypes.h file so that its signed and unsigned 16-bit integer type sizes are correct.  By default it has types set up for the ATmega 328P chip family (Arduino series).

The testmidi.c file has a main().  Compile and run it:

    gcc testmidi.c
    ./a.out


## MIDI Utilities

### Files
Include the following files in your project:

- utilities.h
- utilities.c

### Usage

These are utility functions that may be of use to you.  They include:

* Pitch bend calculations
* Conversions from pitch to frequency with all integer math
* Note distribution and voice allocation tools

**Note that the utilities are at present ENTIRELY UNTESTED.**


## Parsing Modular MIDI Conventions

The [Modular MIDI Conventions](https://github.com/eclab/grains/blob/main/info/ModularMIDIConventionsForDevelopers.md) are a set of conventions for implementing MIDI *between* modules in a modular synthesizer.  If you don't care about that, this section is not relevant to you.

### Dependencies
Set up a MIDI Parser.  See "Parsing MIDI" above.

### Files
Include the following additional files in your project:

- parsemodular.h
- parsemodular.c

### Usage

The MIDI parsing facility is just a bunch of utility functions to make things easier for you.  See the parsemodular.h file for instructions on how to use them.


## Emitting Modular MIDI Conventions

The [Modular MIDI Conventions](https://github.com/eclab/grains/blob/main/info/ModularMIDIConventionsForDevelopers.md) are a set of conventions for implementing MIDI *between* modules in a modular synthesizer.  If you don't care about that, this section is not relevant to you.

### Dependencies
Set up a MIDI Emitter.  See "emitting MIDI" above.

### Files
Include the following additional files in your project:

- emitmodular.h
- emitmodular.c

### Usage

The MIDI emitting facility is just a bunch of utility functions to make things easier for you.  See the emitmodular.h file for instructions on how to use them.


## Note

At present there is no test code for testing the Modular MIDI conventions emitting and parsing utilities.
