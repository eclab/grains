# Evolution

Evolution is an evolving drone synthesizer using 9-partial additive synthesis.  Evolution is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

All Evolution does is randomly (and usually slowly) change the amplitudes of 8 of the partials, leaving the fundamental alone.  You have 8 choices of frequency combinations.  The frequencies of the partials can be found in partialFrequencies[] if you wanted to modify them.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.


## Bright and Dark Partials

Evolution has three spectral envelopes: left-to-right: Dark, Medium, and Bright.  The "Dark" envelope cuts down higher frequencies exponentially.  The "Bright" envelope does not. The "Medium" envelope is in-between.   For each envelope we have the same eight choices of partial frequency combinations: so there are 24 choices in all.

## Triggering

If you turn the Evolution Rate knob (Pot 3) to the far LEFT, so the Rate is Zero, then Evolution will instead change to new random partial amplitudes when you provide a trigger.  Note that Mozzi only checks for triggers 100 times in a second: if your trigger is faster than 1/100 a second, it may not be seen.

## Adjusting Tuning and Tracking

Grains's Inputs track 1.3V/octave, not 1V/octave: we'll need to scale them to track properly.  To do this, you can adjust the Pitch CV Scaling on Pot 1.  This GRAINS program is set up to play the C three octaves below Middle C when it receives 0V.  You should be able to use Pot 1 to scale the pitch such that high Cs play in tune as well.  Once you have things tracking well, you can then use the Pitch Tune (Audio In) to tune 0V to some other note.  Note that as GRAINS resistors warm up, the scaling will change and you will need to adjust the tracking again, at least until they are fully warmed up.

By default the note corresponding to 0V is C0, three octaves below middle C, that is MIDI note 24, or 32.7 Hz.  You can customize the tuning for this Grains program but only UP.  This can be done in two ways.  First, you can add pitch to the tuning with a CV value to Audio In.  Second, you can transpose the pitch up by changing the TRANSPOSE\_OCTAVES and/or TRANSPOSE\_SEMITONES #defines in the code to positive integers.  You can also change TRANSPOSE\_BITS: a "bit" is the minimum possible change Grains can do, equal to 1/17 of a semitone.


## Configuration

#### IN 1
Pitch CV
#### IN 2
[UNUSED]
#### IN 3
[UNUSED]
#### AUDIO IN (A)
Pitch Tune
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
Trigger
#### POT 1
Pitch Scaling	[Set the switch to IN1]
#### POT 2
- (Left third of Dial) Choice of 8 Dark Partials 

- (Center third of Dial) Choice of 8 Medium Partials

- (Right third of Dial) Choice of 8 Bright Partials 

[Set the switch to MAN]
#### POT 3
Evolution Rate.  If set to far left, only evolves when triggered 

