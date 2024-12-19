# Fold

Fold is a sine wave soft wave-folder: that is, it pushes a single sine wave through another sine wave (8x) rather than through a triangle wave per a hard wave-folder.  Though this aliases far less than a hard wave-folder, it's still got some distortion here and there thanks to the filtered PWM.  FOLD then sends the result through a low-pass filter: you can set the filter cutoff manually but not via CV.

Fold is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.


SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

You can set the pitch CV and tune.  You can also change the DRIVE, which changes the degree of folding.  You can get the sine wave to fold up to 8 times at present.

## Adjusting Tuning and Tracking

Grains's Inputs track 1.3V/octave, not 1V/octave: we'll need to scale them to track properly.   To do this, you can adjust the Pitch CV Scaling on Pot 1.  This GRAINS program is set up to play the C three octaves below Middle C when it receives 0V.  You should be able to use Pot 1 to scale the pitch such that high Cs play in tune as well.  Once you have things tracking well, you can then use the Pitch Tune (Audio In) to tune 0V to some other note.  Note that as GRAINS resistors warm up, the scaling will change and you will need to adjust the tracking again, at least until they are fully warmed up.

By default the note corresponding to 0V is C0, three octaves below middle C, that is MIDI note 24, or 32.7 Hz.  You can customize the tuning for this Grains program but only UP.  This can be done in two ways.  First, you can add pitch to the tuning with a CV value to Audio In.  Second, you can transpose the pitch up by changing the TRANSPOSE\_OCTAVES and/or TRANSPOSE\_SEMITONES #defines in the code to positive integers.  You can also change TRANSPOSE\_BITS: a "bit" is the minimum possible change Grains can do, equal to 1/17 of a semitone.




## Configuration

#### IN 1
Pitch CV
#### IN 2
Drive CV
#### IN 3
[Unused]
#### AUDIO IN (A)
Tune CV
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
[Unused]
#### POT 1
Pitch CV Scaling       [Set the switch to In1]
#### POT 2
Drive CV Scaling       [Set the switch to In2]
#### POT 3
Low Pass Filter Cutoff


## Grains Bug

There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the switch to "Man", then the range of the Pot is correct.  But if you set the switch to "In 1" (or "In 2"), then the range of the Dial is maximum (1.0 or 5V for our purposes) at about the 2 o'clock position on the Dial.  Beyond that it stays at 1.0
