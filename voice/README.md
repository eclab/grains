# Voice

Voice is a complete synthesizer voice, with an oscillator, a filter, and an amplifier. Voice is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

Voice has a saw, square, or triangle oscillator, four filter options, and an amplifier.   You can control the pitch, amplitude, and filter cutoff with CV values.   You can also control the pitch, filter cutoff, and resonance with the pots, but not the amplitude (not enough pots!)

You can choose from square, triangle, and sawtooth waves, and from low-pass, high-pass,  band-pass, and notch filter types.  This is done by changing #defines in the code, it's easy.  There is no pulse width or PWM. The filter is a simple two-pole filter consisting of two one-pole filters plus resonance. It's what's provided by the Mozzi distribution but it's not great, particularly at high frequencies and high resonance.  I might replace it with my own filter later.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.


## Adjusting Tuning and Tracking

Grains's Inputs track 1.3V/octave, not 1V/octave: we'll need to scale them to track properly.  To do this, you can adjust the Pitch CV Scaling on Pot 1.  This GRAINS program is set up to play the C three octaves below Middle C when it receives 0V.  You should be able to use Pot 1 to scale the pitch such that high Cs play in tune as well.  Once you have things tracking well, you can then use the Pitch Tune (Audio In) to tune 0V to some other note.  Note that as GRAINS resistors warm up, the scaling will change and you will need to adjust the tracking again, at least until they are fully warmed up.

By default the note corresponding to 0V is C0, three octaves below middle C, that is MIDI note 24, or 32.7 Hz.  You can customize the tuning for this Grains program but only UP.  This can be done in two ways.  First, you can add pitch to the tuning with a CV value to Audio In.  Second, you can transpose the pitch up by changing the TRANSPOSE\_OCTAVES and/or TRANSPOSE\_SEMITONES #defines in the code to positive integers.  You can also change TRANSPOSE\_BITS: a "bit" is the minimum possible change Grains can do, equal to 1/17 of a semitone.

## Configuration

#### IN 1
Pitch CV
#### IN 2
Filter Cutoff CV
#### IN 3
Amplitude CV
#### AUDIO IN (A)
Pitch Tune
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
[UNUSED]
#### POT 1
Pitch Scaling	[Set the switch to IN1]
#### POT 2
Filter Cutoff

[If you're not using Filter Cutoff CV, set the switch to MAN]
#### POT 3
Resonance
