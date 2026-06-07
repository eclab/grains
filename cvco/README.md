# CVCO

CVCO is meant to run on the AE Modular GRAINS, but it could be adapted
to any Arduino.  CVCO was written by Sean Luke [sean@cs.gmu.edu]

SET GRAINS TO GRAINS MODE.

CVCO does two things:

1. It converts gate/cv into MIDI notes for the Wonkystuff mco/1 and mco1/b.
This allows you to drive the mco/1 and mco/1b from a SEQ16 or other step
sequencer, or any other gate/cv source. This performs essentially the same 
task as the Wonkystuff CVMx in this regard.

2. It maps certain incoming CV sources and pots to a variety of settings
to a variety of CC or other MIDI settings.  The settings depend on which
module you have.  Your module options are:

         A. The original mco/1
         B. The mco/1 with revised firmware, as of April 2024
         C. The mco/1b

You set which module you are controlling by uncommenting a #define in the code.

You set the CC PARAMETER being output by turning POT 3.

You set the CC VALUE being output by turning POT 2.

When you change the CC PARAMETER, or the first time after the module has powered up,
You cannot change the CC value until you first turn POT 2 a significant distance (over 1/8
of the total travel of POT 2), to "unlock" POT 2.

The CC PARAMETERS vary depending on the kind of module you have set.  Specifically,
there are:

### Original MCO/1
 1. Sawtooth Amplitude
 2. Pulse Amplitude
 3. Pulse Width
 4. Pulse Width Modulation Amount
 5. Suboscillatior Amplitude
 6. Noise Amplitude
 7. Velocity:    the pot or CV sets the velocity (volume) of notes played  (This is not a CC of course)
 8. Nothing:     the pot doesn't do anything

### Revised MCO/1
 1. Sawtooth Amplitude
 2. Pulse Amplitude
 3. Pulse Width
 4. Pulse Width Modulation Amount
 5. Suboscillatior Amplitude
 6. Noise Amplitude
 7. LFO Rate
 8. LFO Pitch Modulation Amount
 9. Velocity Sensitivity
10. Legato Switch (On -> Turn Pot High / Off -> Turn Pot Low)
11. Program Change: values 0...31    (This is not a CC of course)
12. Velocity:    the pot or CV sets the velocity (volume) of notes played  (This is not a CC of course)
13. Nothing:     the pot doesn't do anything

### MCO/1b
 1. Sawtooth Amplitude
 2. Pulse Amplitude
 3. Pulse Width
 4. Pulse Width Modulation Amount
 5. Suboscillatior Amplitude
 6. Noise Amplitude
 7. LFO Rate
 8. LFO Pitch Modulation Amount
 9. Sine Amplitude
10. Velocity Sensitivity
11. Transpose
12. Legato Switch (On -> Turn Pot High / Off -> Turn Pot Low)
13. Pitch Bend Range (0...12 semitones)    (This is not a CC of course it's RPN)
14. Program Change: values 0...31    (This is not a CC of course)
15. Velocity:    the pot or CV sets the velocity (volume) of notes played  (This is not a CC of course)
16. Nothing:     the pot doesn't do anything


Thus you probably by default want to have POT 3 set to FAR RIGHT ("nothing"),
and the MCO or MCO/1B will only receive notes.


## Resetting the MCO/1B

CVCO assumes that your mco1b has default learned MIDI settings for its notes and
CC parameters.  If you have changed them, you will need to change them back.  To do
this, you can long-press the MCO/1B button, then send it a MIDI CC 10 (any value).
To assist you in this, simply set the Parameter (POT 3) to FAR LEFT.  This is
Sawtooth Amplitude, which is CC 10.  Then put the MCO/1B in MIDI train mode, and
turn the Value (POT 2) knob back and forth, which will send various values of 
CC 10 to the MCO/1B.




## Transposition
This GRAINS program is set up by default to associate 0V with MIDI note 36 (two octaves below Middle C)
and 5V with MIDI note 96 (three octaves above Middle C).  You can transpose these notes up or down
by modifying a #define in the code, which is set in semitones.  


## Note Velocity
Instead of setting the note velocity via POT2/IN2, you can set it via the AUDIO IN CV value, and use
POT2/IN2 for something else.  In this case setting POT3 to "Note Velocity" would just set it to "Nothing".

Note that AUDIO IN reaches its maximum at about 4 volts.

You can turn on this feature by uncommenting a #define in the code.

Also note that in order for changing note velocity via POT 2 to have any effect, you need to turn up note velocity sensitivity.



## Adjusting Tuning and Tracking

Grains's Inputs track 1.3V/octave, not 1V/octave: we'll need to scale them to track properly.  
To do this, you can adjust the Pitch CV Scaling on Pot 1.    However you need to get the tracking
(scaling) properly trimmed or that won't happen.  The right setting on Pot 1 is approximately the 2.5 o'clock position.  Note that if you don't get it quite right, you'll probably get occasional incorrect pitches, even on QUANTIZER, as noise pushes the pitch values just barely into the next pitch area.  You'll need to tweak it slightly.  Also note that even if you have it set right, SEQ16 and SEQ8 may still cause occasional incorrect pitches if they're sitting right on the border between two note pitches.  You'll need to adjust their trim a tiny bit towards the pitch you wanted.

Note that as GRAINS resistors warm up, the scaling will change and you will need to adjust the 
tracking again, at least until they are fully warmed up.


## Soft Serial

We presume you are using a GRAINS, not a GRAINS V2, of which at present I'm the only
owner in the world outside of Tangible Waves.  If you ARE on a GRAINS V2, you will want
to use hardware serial output, not soft serial, which isn't very reliable.  To do that
you would comment out a #define in the code.

## Configuration
This is the default configuration

#### IN 1
Note Pitch CV
#### IN 2
CC Value CV
#### IN 3
Gate		
#### AUDIO IN (A)
Optional Note Velocity CV
#### AUDIO OUT
[Unused]
#### DIGITAL OUT (D) 
MIDI Out to MCO/1 or MCO/1B (on Grains)
#### POT 1
Note Pitch Scaling.  Set switch to IN1.  Set pot to approximately the 2 O'clock position, adjust from there.
#### POT 2
CC Value
#### POT 3
CC Parameter
#### Serial RX
MIDI Out to MCO/1 or MCO/1B (on Grains V2)        
