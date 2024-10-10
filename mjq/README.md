# MJQ

MJQ is a MIDI-CONTROLLED marimba simulator.  In honor of Modern Jazz Quartet, I tried to get a resonator working, but GRAINS is not fast enough.  However there is some natural LFO resonance, enough to *sort of* sound like a vibraphone.

MJQ is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.  

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

### Voices

MJQ can play up to **three notes** at once.   

### Options

* You can set the DECAY of the marimba bars with POT 1.
* You can set the OVERALL VOLUME with POT 2 -- you'll need to decrease this as you increase the decay.
* You can set the BRIGHTNESS of the marimba bars with POT 3.  This increases the volume of the higher two partials, so you may need to reduce the overall volume as well.

### Voices and Velocity

Notes can either be fixed at maximum velocity, or can be velocity sensitive.  By default they're velocity sensitive.  To make them velocity insensitive and max velocity, just uncomment a certain #define in the code.

### Aliasing

At extremely high frequencies, MJQ's partials will go over Nyquist and
you may hear some antialiasing effects.


## MIDI Response

- Note On
    - Pitch: All MIDI notes C0 through G10, but realistically C0 through B8 
    - Velocity: 0...127 for Note On.  Velocity 0 is a Note Off

- Note Off
	- Does nothing

- CC
    - All Notes Off (CC 123) Resets all notes
    - All Sounds Off (CC 120) Resets all notes

## Configuration

#### IN 1
Decay CV
#### IN 2
Volume CV
#### IN 3
[Unused]
#### AUDIO IN (A)
[Unused]
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
MIDI In
#### POT 1
Decay
#### POT 2
Volume
#### POT 3
Brightness

