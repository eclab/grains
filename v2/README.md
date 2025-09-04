# V2 Programs

This is a collection of programs meant to work with one or more of:

* An Arduino Nano Every on a BRAEDBOARD, or in some cases other Arduinos

* A GRAINS V2.  The only copy of this module, for the time being, is a prototype I received from Tangible Waves.  There is a fair chance that it will never be sold.

* A Standard GRAINS, but very flakey.

## Info

This subdirectory contains instructions for how to set up an Arduino Nano Every on a BRAEDBOARD to expose its three serial ports.

## Ensemble

A polyphonic note distributor, which takes polyphonic notes coming in on a single MIDI channel and distributes them to several other channels.  

* GRAINS V2 + IMDI HEART (up to 16 voices)
* GRAINS + IMDI HEART (very flakey, only for fooling around)
* Arduino Nano Every + IMDI HEART or MB/1 (3 voices only)
* Arduino Nano Every + IMDI HEART (up to 16 voices)

## Master

An MPE Master Channel distributor, which takes CC, PC, etc. from the MPE Master Channel and distributes it to the various MPE Zone channels.  **Not at all tested.**  Configurations:

* GRAINS V2 + IMDI HEART
* Arduino Nano Every + IMDI HEART

## Spectacle

A light show for your GRAINS V2.  Spectacle lights up the module's LEDs in a variety of ways.

