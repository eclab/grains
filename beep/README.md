# BEEP

BEEP is a 9-voice MIDI-CONTROLLED sine wave generator with decay, which can make a variety of beeps and pings.  Beep is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.  

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

When you play a note, a sine wave is generated and held at its volume for a certain amount of time (the HOLD time).  Then the sine wave will decay in volume over its DECAY time until it reaches 0.

The HOLD time varies from very long to zero length.  At the far left, the hold stays on until you release the note.  In all cases, a GATE OUT is raised as long as some note is presently playing and has not yet been released.

POT3 does two things: it sets the overall volume, and it sets whether BEEP's notes are fixed at maximum velocity versus being velocity sensitive.  

### Voices

BEEP can play up to **nine notes** at once.   

### Options

* You can set the DECAY of the beeps with POT 1.
* You can set the HOLD length of the beeps with POT 2.
* You can set the VOLUME of the beeps with POT 3.  The left half of the pot goes from min to max volume, but also makes the beeps velocity sensitive.  The right half of the pot goes from min to max volume, but makes the beeps velocity INsensitive and max velocity.

## MIDI Response

- Note On
    - Pitch: All MIDI notes C0 through G10, but realistically C0 through B8 
    - Velocity: 0...127 for Note On.  Velocity 0 is a Note Off

- Note Off
	- Releases notes with indefinite hold length

- CC
    - All Notes Off (CC 123) Resets all notes
    - All Sounds Off (CC 120) Resets all notes

## Configuration

#### IN 1
Decay CV
#### IN 2
Hold CV
#### IN 3
Gate Out
#### AUDIO IN (A)
[Unused]
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
MIDI In
#### POT 1
Decay
#### POT 2
Hold
#### POT 3
Overall Volume and Velocity Sensitivity.  

- Left Half: velocity sensitive, [0 ... max volume] 
- Right half: velocity INsensitive, [0 ... max volume]


