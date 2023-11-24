# Evolution

Evolution is an evolving drone synthesizer using 9-partial additive synthesis.  Evolution is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

All Evolution does is randomly (and usually slowly) change the amplitudes of 8 of the partials, leaving the fundamental alone.  You have 8 choices of frequency combinations.  The frequencies of the partials can be found in partialFrequencies[] if you wanted to modify them.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

NOTE: Probably due to the voltage divider on all of its analog inputs, GRAINS is limited to about a 45 note range.  

## Bright and Dark Partials

Evolution has two spectral envelopes.  The "Dark" envelope cuts down higher frequencies linearly.  The "Light" envelope does not. For each envelope we have the same eight choices of partial frequency combinations: so there are sixteen choices in all.

## Triggering

If you turn the Rate knob (Pot 3) to the far LEFT, so the Rate is Zero, then Evolution will instead change to new random partial amplitudes when you provide a trigger.  Note that Mozzi only checks for triggers 100 times in a second: if your trigger is faster than 1/100 a second, it may not be seen.

## Adjusting Tracking

Grains can only do about 3.75 octaves due to its circuit design.  I have positioned it to start at the C two octaves below middle C.  For the same reason, I have to "stretch" Grains from 3.75V to 5V, so to speak, in software.  I have tuned this stretch for my own Grains, but if Evolution is tracking poorly in your Grains, get with me and we might be able to customize it for you.

In short, you can play with the function that has "pow(...)" in it below to get the tracking you need.  65.41 is the frequency two octaves below middle C.  45.2 is the stretch factor appropriate for my grains -- higher is more stretched.  60 is a full 5V, 5-octave (60 note) stretch.  You might need to stretch or decrease by just a tad, like .1 or .2.  I don't know how consistent Grains' internal resistors are from unit to unit.

Once you have a desired base frequency and stretch value, you could pump out all the values from 0 to 1023 and put them into the "pitch[]" table.  I use some lisp code as below but you can choose whatever language suits your fancy.  :-)  Then use the FREQUENCIES macro instead of the pow() function, it's significantly faster.


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
(Left half of Dial) Choice of Dark Partials 
(Right half of Dial) Choice of Bright Partials 
[Set the switch to MAN]
#### POT 2
Pitch Tune  [Set the switch to MAN]
#### POT 3
Evolution Rate.  If set to far left, only evolves when triggered 

