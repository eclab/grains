# Trajectory

TRAJECTORY is a MIDI-driven 1-voice vector synthesizer similar to VS, but with different design choices.  TRAJECTORY is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.  TRAJECTORY only responds to MIDI, via the wonkystuff mb/1, IMDI MAIN module, or from DAVE. 

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

TRAJECTORY crossfades between four waveforms in a special way.  You can control the cross-fading manually with two CVs, such as with a joystick; and you can also use the same to program about 4 seconds of CV crossfading movement.

TRAJECTORY is different from VS in the following ways:

1. TRAJECTORY has four fixed waves that you cannot change, whereas with VS you can pick your waves.
2. VS is heavily aliased because it has only one copy of each wave.  But because TRAJECTORY has the waves it does, it has copies of them at different frequencies and so is much less aliased.
3. TRAJECTORY has half the vector envelope resolution of VS, because it needs extra space to fit its wave information.

## How Vector Synthesis Works

Vector synthesis dynamically changes the cross-fade between four different sound waves. To control the cross-fade mix you modify two CVs called X and Y.  Imagine that these CV values each go from 0 to 1 inclusive, so together they define a square region from (0,0) to (1,1).  The amplitude of the four waves is defined as:

    AMPLITUDE OF WAVE 1 = (1-x) * (1-y)
    AMPLITUDE OF WAVE 2 = x * y			[opposite corner from WAVE 1]
    AMPLITUDE OF WAVE 3 = (1-x) * y
    AMPLITUDE OF WAVE 4 = x * (1-y)		[opposite corner from WAVE 3]

In short: each wave has a corner in the square where it is the only sound.  Wave 1 is solo in the corner (x=0, y=0), Wave 2 in the corner (x=1, y=1), wave 3 in (x=0, y=1), and wave 4 in (x=1, y=0).  All four waves are mixed together (25% each) in the center.  

You can either manually change these CVs to your heart's content, or you can record up to 4 seconds of CV-twiddling, and that will be played back each time a new note/chord is played.


## Setting the Waves

By default TRAJECTORY sets its four waves to be:

    WAVE 1	Sine
    WAVE 2  White Noise						[opposite corner from Sine]
    Wave 3	Square
    Wave 4  Sawtooth						[opposite corner from Square]

You have three options here:

* OPTION 1: Play the four waves as-is
* OPTION 2: Replace the SINE with SILENCE
* OPTION 3: Replace the WHITE NOISE with SILENCE

You set options 2 and 3 with #defines in the code.

Additionally, if you find the noise too bright, you can darken (and quiet) it a bit by uncommenting a #define in the code.


## Playing Modes

TRAJECTORY has four MODES, specified by POT 3.

### Free Mode
The wave mix is set by changing IN1/POT1 and IN2/POT2.

### Record Mode
The wave mix is **recorded** by changing IN1/POT1 and IN2/POT2 for up to 4 seconds. When a note is played, the GATE OUT is raised, and when a note is released, the GATE OUT is lowered.  
Recording stops when (1) the time has elapsed or (2) the note is released or (3) the mode is changed back to PLAY or FREE.

When you have stopped recording, TRAJECTORY will save your internal recording to the EEPROM so that it's
available even after you reboot.

### Play Mode
when a note is played, the recorded wave mix is played back.  When a note is played, the GATE OUT is raised, and when a note stops being played, the GATE OUT is lowered.


## Looping Option

If you uncomment a certain #define in the code,  you can get TRAJECTORY to loop its vector envelope while you play it. This will only work if the loop was recorded to its maximum length: if you recorded to something shorter, it won't loop.


## Using TRAJECTORY With the AE Modular JOYSTICK

TRAJECTORY works perfectly with JOYSTICK.  Just attach JOYSTICK's CV X to GRAINS IN 1, and CV Y to IN 2. Make sure that POT 1 is set to IN 1 and POT 2 is set to IN 2.  Then you'll need to trim the pots to get the range right.  I find that setting POT 1 (and 2) to a little before the 3 o'clock position is the right position.  You can test by setting TRAJECTORY to FREE mode and see if each of the JOYSTICK's corners correspond to a pure sound.

## Channel 

You can set TRAJECTORY's channel to 0...15 (for channels 1...16) or to OMNI (to listen to any of them). This is set with a #define in the code.here:



## MIDI Response

- Note On/Off
    - Pitch: All MIDI notes C0 through G10, but realistically C0 through B8 
    - Velocity: 0...127 for Note On.  Velocity 0 is a Note Off

- CC
    - All Notes Off (CC 123) Resets all notes, lowers Gate
    - All Sounds Off (CC 120) Resets all notes, lowers Gate



## Configuration

#### IN 1
X
#### IN 2
Y
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
#### POT 2
Y
#### POT 3
Mode: FREE (Left), PLAY (Center), RECORD (Right)
