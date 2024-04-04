# MIDI

This is not a GRAINS project, though it is used in several projects.  It is a low-footprint
MIDI emitting and parsing library meant for small microprocessors, and contains additional
features for the Modular MIDI conventions.  It's currently under development.

### emitmidi.c and emitmidi.h
Utility code for emitting MIDI

### parsemidi.c and parsemidi.h
Code for allocating, setting up, and using MIDI parsers for incoming MIDI

### miditypes.h
Minor code shared in common by parsemidi.h and emitmidi.h

### testmidi.c
Test code for emitmidi.c and parsemidi.c

### parsemodular.c and parsemodular.h
Utility code to make it easier to determine which parameter is which for modular synth modules

### emitmodular.c and emitmodular.h
Utility code to make it easy to emit MIDI to other modules

At present there is no test code for parse/emitmodular.c/h, which could be full of bugs for all I knew.
