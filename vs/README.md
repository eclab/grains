# VS

VS is a MIDI-driven 1-voice vector synthesizer.

VS is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino. VS only responds to MIDI, via the wonkystuff mb/1, IMDI MAIN module, or from DAVE. 

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

VS crossfades between four waveforms in a special way.  You can control the cross-fading manually with two CVs, such as with a joystick; and you can also use the same to program about 4 seconds of CV crossfading movement.

## Warning: use of EEPROM

VS saves your vector recording to the GRAINS's EEPROM.  The EEPROM is like a little hard drive, and it's designed for this purpose.  It can only be written to about 100,000 times.  So it's just fine to use, except if a program goes crazy and writes to it over and over again and wears it out until it doesn't work any longer.  I have added some safeguards, and I'm pretty sure that VS will not go nuts, but you have been warned.  

## How Vector Synthesis Works

Vector synthesis dynamically changes the cross-fade between four different sound waves. To control the cross-fade mix you modify two CVs called X and Y.  Imagine that these CV values each go from 0 to 1 inclusive, so together they define a square region from (0,0) to (1,1).  The amplitude of the four waves is defined as:

    AMPLITUDE OF WAVE 1 = (1-x) * (1-y)
    AMPLITUDE OF WAVE 2 =    x  *    y
    AMPLITUDE OF WAVE 3 = (1-x) *    y
    AMPLITUDE OF WAVE 4 =    x  * (1-y)

In short: each wave has a corner in the square where it is the only sound.  Wave 1 is solo in the corner (x=0, y=0), Wave 2 in the corner (x=1, y=1), wave 3 in (x=0, y=1), and wave 4 in (x=1, y=0).  All four waves are mixed together (25% each) in the center.  

You can either manually change these CVs to your heart's content, or you can record up to 4 seconds of CV-twiddling, and that will be played back each time a new note/chord is played.


## Setting the Waves

VS allows you set the waves you'd like to use.  You have to provide waveform files for all four waves, and they must all be different.  You can use any of the files in the "vs" directory, and you can also use any of the AKWF waveform files from the AKWF project  (just copy them over to the "vs" directory and use them from there).  

By the way, the "vs" directory contains waveforms drawn from the original waves on the Prophet VS.


## Playing Modes

VS has three MODES, specified by POT 3.

### Free Mode
The wave mix is set by changing IN1/POT1 and IN2/POT2.

### Record mode
The wave mix is **recorded** by changing IN1/POT1 and IN2/POT2 for up to 4 seconds. When a note is played, the GATE OUT is raised, and when a note is released, the GATE OUT is lowered.  Recording stops when (1) the time has elapsed or (2) the note is released or (3) the mode is changed back to PLAY or FREE.

When you have stopped recording, VS will save your internal recording to the EEPROM so that it's available even after you reboot.  

### Play Mode

When a note is played, the recorded wave mix is played back.  When a note is played, the GATE OUT is raised, and when a note stops being played, the GATE OUT is lowered.

## Looping Option

If you uncomment a certain #define in the code,  you can get VS to loop its vector envelope while you play it. This will only work if the loop was recorded to its maximum length: if you recorded to something shorter, it won't loop.


## Using VS With the AE Modular JOYSTICK

VS works perfectly with JOYSTICK.  Just attach JOYSTICK's CV X to GRAINS IN 1, and CV Y to IN 2. Make sure that POT 1 is set to IN 1 and POT 2 is set to IN 2.  Then you'll need to trim the pots to get the range right.  I find that setting POT 1 (and 2) to a little before the 3 o'clock position is the right position.  You can test by setting VS to FREE mode and see if each of the JOYSTICK's corners correspond to a pure sound.



## MIDI Channel

You can set VS's channel to 0...15 (for channels 1...16) or to OMNI (to listen to any of them). This is set with a #define in the code.



## MIDI Response

- Note On/Off
    - Pitch: All MIDI notes C0 through G10, but realistically C0 through B8 
    - Velocity: 0...127 for Note On.  Velocity 0 is a Note Off

- CC
    - All Notes Off (CC 123) Resets all notes, lowers Gate
    - All Sounds Off (CC 120) Resets all notes, lowers Gate



## Configuration

#### IN 1
X CV
#### IN 2
Y CV
#### IN 3
Gate Out
#### AUDIO IN (A)
[Unused]
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
MIDI In
#### POT 1
X

[If using X CV, set switch to IN1 and POT 1about 2'oclock]
#### POT 2
Y

[If using Y CV, set switch to IN2 and POT 2about 2'oclock]
#### POT 3
Mode: FREE (Left), PLAY (Center), RECORD (Right)
