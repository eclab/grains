# Evolution

Evolution is an evolving drone synthesizer using 9-partial additive synthesis.  Evolution is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

All Evolution does is randomly (and usually slowly) change the amplitudes of 8 of the partials, leaving the fundamental alone.  You have 8 choices of frequency combinations.  The frequencies of the partials can be found in partialFrequencies[] if you wanted to modify them.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

NOTE: Probably due to the voltage divider on all of its analog inputs, GRAINS is limited to about a 45 note range.  

## Bright and Dark Partials

Evolution has three spectral envelopes: left-to-right: Dark, Medium, and Bright.  The "Dark" envelope cuts down higher frequencies exponentially.  The "Bright" envelope does not. The "Medium" envelope is in-between.   For each envelope we have the same eight choices of partial frequency combinations: so there are 24 choices in all.

## Triggering

If you turn the Evolution Rate knob (Pot 3) to the far LEFT, so the Rate is Zero, then Evolution will instead change to new random partial amplitudes when you provide a trigger.  Note that Mozzi only checks for triggers 100 times in a second: if your trigger is faster than 1/100 a second, it may not be seen.

## Adjusting Tracking

Grains's Inputs 1, 2, and 3 (and A) track at about 1.3V/octave, not 1V/octave.  The exact amount varies from unit to unit.  You may need to tweak things in the code to get it to track better.  It's just a matter of setting a single number, but you'll probably need to adjust it.  The code has instructions for doing this.  Likely once you've identified the number, you can use it in all my Grains projects.


## Configuration

#### IN 1
[UNUSED]
#### IN 2
[UNUSED]
#### IN 3
Pitch CV 
#### AUDIO IN (A)
[UNUSED]
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
Trigger
#### POT 1
(Left third of Dial) Choice of 8 Dark Partials 
(Center third of Dial) Choice of 8 Medium Partials
(Right third of Dial) Choice of 8 Bright Partials 
[Set the switch to MAN]
#### POT 2
Pitch Tune  [Set the switch to MAN]
#### POT 3
Evolution Rate.  If set to far left, only evolves when triggered 

