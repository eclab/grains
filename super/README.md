# Super

Super is barely a supersaw: it's just three detuned sawtooth waves, one at the note pitch, one detuned
above, and one detuned below.  I can't add more waves: there's not enough memory in GRAINS, using Mozzi.
Super is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

Super has a saw, square, or triangle oscillator avaiable.  The default wave is, of course, saw.  
There is no pulse width or PWM. You can control the pitch and detune with CV values.  
You can also set the amplitude of the two detuned waves relative to the main wave.  The options
are 0 vs 15, 1 vs 13, 2 vs 11, 3 vs 9, 4 vs 7, and 5 v 5

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.


## Choosing Saw vs. Square vs. Triangle

The choice of wave is determined by a #define in the code as indicated.

## Adjusting Tuning and Tracking

Grains's Inputs track 1.3V/octave, not 1V/octave: we'll need to scale them to track properly.  To do this, you can adjust the Pitch CV Scaling on Pot 1.  This GRAINS program is set up to play the C three octaves below Middle C when it receives 0V.  You should be able to use Pot 1 to scale the pitch such that high Cs play in tune as well.  Once you have things tracking well, you can then use the Pitch Tune (Audio In) to tune 0V to some other note.  Note that as GRAINS resistors warm up, the scaling will change and you will need to adjust the tracking again, at least until they are fully warmed up.

By default the note corresponding to 0V is C0, three octaves below middle C, that is MIDI note 24, or 32.7 Hz.  You can customize the tuning for this Grains program but only UP.  This can be done in two ways.  First, you can add pitch to the tuning with a CV value to Audio In.  Second, you can transpose the pitch up by changing the TRANSPOSE\_OCTAVES and/or TRANSPOSE\_SEMITONES #defines in the code to positive integers.  You can also change TRANSPOSE\_BITS: a "bit" is the minimum possible change Grains can do, equal to 1/17 of a semitone.

## Configuration

#### IN 1
Pitch CV
#### IN 2
Detune CV
#### IN 3
[Unused]
#### AUDIO IN (A)
Pitch Tune
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
[UNUSED]
#### POT 1
Pitch Scaling	[Set the switch to IN1]
#### POT 2
Detune [If you're not using Detune CV, set the switch to MAN]
#### POT 3
Relative Wave Amplitude


