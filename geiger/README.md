# Geiger

Geiger is a random trigger generator for three tracks.  Geiger is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.  Geiger was written by Sean Luke [sean@cs.gmu.edu]

Set your Grains to **GRAINS MODE**.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)

The randomness of the triggers for each track is controlled by a different knob.  You can also control the randomness of the first two tracks with a CV, but note that when you do so, the potentiometer's maximum value peaks at about the 2 o'clock position on the dial due to a GRAINS bug.

If you turn POT 3 to the far RIGHT, GEIGER can instead be used as a **BERNOULLI GATE.** That is, when a trigger comes in, ONLY ONE of the outputs will be triggered exclusively. The probability that the output will be OUTPUT 1 is determined by POT 1 (and IN 1). Of the remainder, the probability that the output will be OUTPUT 2 is determined by POT 2 (and IN 2).  Otherwise, it's OUTPUT 3.  So for example, to make each output trigger with equal probability, set POT 1 to 1/3 and set POT 2 to 1/2.

## Configuration
This is the default configuration

#### IN 1
CV for probability of Output 1
#### IN 2
CV for probability of Output 2
#### IN 3
Output 2		
[Yes, it's been changed to an OUT]
#### AUDIO IN (A)
Clock or Bernoulli Trigger
#### AUDIO OUT
Output 3
#### DIGITAL OUT (D) 
Output 1
#### POT 1
Probability for Output 1 [Set to "In 1" only if using In 1 CV]
#### POT 2
Probability for Output 2 [Set to "In 2" only if using In 2 CV]
#### POT 3
Probability for Output 3 [Turn to far right to change to BERNOULLI]


### USING DRUMKIT010
Be sure to set DrumKit1010's TRIG MODE switch to "D" (Digital), so it properly responds to triggers.

### GRAINS BUG

There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the switch to "Man", then the range of the Pot is correct.  But if you set the switch to "In 1" (or "In 2"), then the range of the Dial is maximum (1.0 probability for our purposes) at about the 2 o'clock position on the Dial.  Beyond that it stays at 1.0.
