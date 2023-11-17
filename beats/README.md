# Beats

Beats is a 4-track drum trigger sequencer with song mode.  Beats is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

Beats can store anywhere from 25 to 800 PATTERNS depending on pattern length.   Each pattern has four drum tracks up to 256 STEPS long.  Each pattern can be repeated any number of times, at which point we proceed to the next pattern.  Patterns can also be repeated FOREVER.  You have the option to manually ADVANCE to the next pattern in the sequence, or to RESET the sequence.  Beats has a rudimentary SWING control and RANDOMNESS features.

SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)

## Setup

In the code you will define:

- The maximum pattern length (which impacts on the maximum number of patterns available)
- The number of patterns in your sequence
- Whether we loop the whole sequence forever, or just play it once
- Whether randomness ADDS and REMOVES triggers, or just REMOVES them

Then you will describe the patterns.  Each pattern has a LENGTH, a NUMBER OF ITERATIONS (including FOREVER), and then four tracks of ones and zeros representing the steps.  It's pretty easy.

##  Configuration

#### IN 1
Clock, CV for Randomness
#### IN 2
Reset / Advance
#### IN 3
Drum 3.  Yes, it's a trigger output now.
#### AUDIO IN (A)
Drum 4.  Yes, it's a trigger output now.
#### AUDIO OUT
Drum 1
#### DIGITAL OUT (D) 
Drum 2
#### POT 1
Randomness: further right is LESS randomness.  Because of the Grains Bug below, Minimum Randomness at 2 o'clock, Maximum a bit below 9 o'clock.  If you turn to the far left, you will PAUSE the CLOCK.  The switch must be set to IN 1.
#### POT 2
Sets whether IN 2 Resets or Advances.  Set to about 9 O'clock for IN 2 to ADVANCE.  Set to FULL (far right) for IN 2 to RESET.  The switch must be set to IN 2.
#### POT 3
Degree of Swing.  No swing is at FULL (far right), then increasing swing as you turn to the left.  

### Randomness CV

Normally you just attach clock to In 1, and then set randomness with the dial.  But if you really wanted to, you could attach a CV for Randomness to In 1 as well.  You'd do this by using the CV to attenuate the clock signal.  For example, you'd have the clock sent to a VCA set to DC mode, and feed your CV into the VCA's CV.  The output of the VCA would go into In 1.  If you did this, you probably should set POT 1 to FULL (far right).  Note that below a certain minimum (about 1/16 full) the clock will no longer trigger Beats.

### GRAINS BUG

There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the switch to "Man", then the range of the Pot is correct.  But if you set the switch to "In 1" (or "In 2"), then the range of the Dial is maximum (1.0 probability for our purposes) at about the 2 o'clock position on the Dial.  Beyond that it stays at 1.0.


