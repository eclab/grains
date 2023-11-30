# Voice

Voice is a complete synthesizer voice, with an oscillator, a filter, and an amplifier. Voice is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

Voice has an saw, square, or triangle oscillator, four filter options, and an amplifier.   You can control the pitch, amplitude, filter cutoff, and resonance with CV values.   You can also control the amplitude and filter cutoff with the pots, and you can tune the pitch (+/-) with a pot.  You cannot control resonance from the pot (not enough pots!)

You can choose from square, triangle, and sawtooth waves, and from low-pass, high-pass,  band-pass, and notch filter types.  There is no pulse width or PWM. The filter is a simple two-pole filter consisting of two one-pole filters plus resonance. It's what's provided by the Mozzi distribution but it's not great, particularly at high frequencies and high resonance.  I might replace it with my own filter later.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

NOTE: Probably due to the voltage divider on all of its analog inputs, GRAINS is limited to about a 45 note range.  

## Adjusting Tracking

Grains's Inputs 1, 2, and 3 (and A) track at about 1.3V/octave, not 1V/octave.  The exact amount varies from unit to unit.  You may need to tweak things in the code to get it to track better.  It's just a matter of setting a single number, but you'll probably need to adjust it.  The code has instructions for doing this.  Likely once you've identified the number, you can use it in all my Grains projects.

## Configuration

#### IN 1
Filter Cutoff CV
#### IN 2
Amplitude CV
#### IN 3
Pitch CV 
#### AUDIO IN (A)
Resonance CV
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
[UNUSED]
#### POT 1
Filter Cutoff

[If you're not using Filter Cutoff CV, set the switch to MAN]
#### POT 2
Amplitude

[If you're not using Amplitude Cutoff CV, set the switch to MAN]
#### POT 3
Pitch Tune +/-
