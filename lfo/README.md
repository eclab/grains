# LFO

LFO is a warpable LFO.  You have a choice of Tri, Sine, or Pulse, all with adjustable GAIN
and adjustable SHAPE. When the LFO completes its period, a TRIGGER is emitted on
digital out -- you could use this to sync things to the LFO for example.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You can adjust the SHAPE of the wave.  The shape of a Pulse wave is its Pulse Width,
hence PWM.  The shape of the Triangle wave goes from Sawtooth to Triangle to Ramp.
The shape of the Sine wave goes from being squished almost into a Sawtooth, to Cosine,
to being squished almost into a Ramp.

The LFO has adjustable RATE of course.  There are three ranges in which you can adjust
the rate: Short, Medium, and Long.  Within a range you can adjust the rate with the RATE
knob or CV.

## Centering

Mozzi has a limited top range of 4.1V for its CV.  If your LFO is centered at 2.5V, this 
means that Mozzi can go as low as 2.5V below that position but only 1.6V above it.  LFO
give you two options here.  You can either recenter the LFO (not about 2.5V) so it goes 
0V to 4.1V, or you can keep it centered and symmetric but reduce its range to 0.9V to 4.1V.

The first option is the default.  If you'd like it centered, there's an option in the code.

## Gain

Gain is provided on Audio IN.  By default, of course, that is 0.  So by default we disable
gain entirely.  If you'd like to control the LFO gain via CV on Audio In, there is an
option for that in the code.



Pot 3 controls both the TYPE of the LFO wave (Tri, Sine, Pulse) and the RANGE (Short, Medium, Long).  Thus there are 9 options in total.

The LFO can also be RESET with a trigger.  

Note that the LFO can only reach about, oh, 4V due to limitations in GRAINS.

Looking for a RANDOM, NOISY, or SAMPLE AND HOLD LFO?  Try my GRAINS firmware STOCHASTIC.

Looking for multiple complex pulse wave LFOs?  Try my GRAINS firmware PULSES.


## Grains Bug

There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the  switch to "Man", then the range of the Pot is correct.  But if you set the switch  to "In 1" (or "In 2"), then the range of the Dial is at maximum at about the 2 o'clock position on the Dial.  Beyond that it stays at maximum.



## Configuration

#### IN 1
Shape CV
#### IN 2
Rate CV
#### IN 3
Reset
#### AUDIO IN (A)
Gain
#### AUDIO OUT
Output
#### DIGITAL OUT (D) 
Period Completion Trigger
#### POT 1
Shape/PWM

[If not using the CV, set switch to MAN, else set to IN2 and put pot at roughly 2'oclock]
#### POT 2
Rte

[If not using the CV, set switch to MAN, else set to IN2 and put pot at roughly 2'oclock]
#### POT 3
Type (Short Saw/Tri/Ramp, Short Sine, Short Pulse, Medium Saw/Tri/Ramp, Medium Sine, Medium Pulse, Long Saw/Tri/Ramp, Long Sine, Long Pulse)

