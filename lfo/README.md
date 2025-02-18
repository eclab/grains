# LFO

LFO is a warpable LFO.  You have a choice of Tri, Sine, or Pulse, all with adjustable GAIN and adjustable SHAPE. When the LFO completes its period, a TRIGGER is emitted on digital out -- you could use this to sync things to the LFO for example.  You can also configure the LFO code to do sample and hold on its waves.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You can adjust the SHAPE of the wave.  The shape of a Pulse wave is its Pulse Width, hence PWM.  The shape of the Triangle wave goes from Sawtooth to Triangle to Ramp. The shape of the Sine wave goes from being squished almost into a Sawtooth, to Cosine, to being squished almost into a Ramp.

The LFO has adjustable RATE of course.  There are three ranges in which you can adjust the rate: Short, Medium, and Long.  Within a range you can adjust the rate with the RATE knob or CV.

Note that the Pulse wave is fairly rounded, kinda like a moderate exponential attack and  decay of an envelope.  This is because of the filter built into AUDIO_OUT.  If you're  looking for a sharper Pulse wave, consider the PULSES GRAINS firmware instead.

Pot 3 controls both the TYPE of the LFO wave (Tri, Sine, Pulse) and the RANGE (Short, Medium, Long). Thus there are 9 options in total.  You will probably find MEDIUM to be the most useful range.

The LFO can also be RESET with a trigger to IN 3.

Note that the LFO can only reach about, oh, 4V due to limitations in GRAINS.

Looking for a RANDOM, NOISY, or RANDOM SAMPLE AND HOLD LFO?  Try my GRAINS firmware STOCHASTIC.

Looking for multiple complex pulse wave LFOs?  Try my GRAINS firmware PULSES.



## Track and Hold

The LFO also has a built-in TRACK AND HOLD.  If you send a gate to DIGITAL OUT, it will sample the current LFO value and only output that value as long as the gate is high.  When the gate drops, the LFO will resume as normal.

How the LFO behaves during Track and Hold depends on the BACKGROUND #define.  Normally, as long as the gate is high, the LFO is essentially frozen, so when the gate is dropped, the LFO begins right where it left off.  This is the default behavior.  An alternative behavior is for the LFO to continue to run in the background while the gate is high, but the output is held fixed during that time.  Thus when the gate is dropped, the LFO will continue wherever it currently is located, causing a discontinuity.  If you want this alternative behavior, uncomment a specific #define in the code.



## Sample and Hold

Track and Hold can be converted to a SAMPLE AND HOLD.  Here, every time you send a TRIGGER to DIGITAL OUT, the LFO will sample its current wave value and only output that until you send the next trigger.  Note that SAMPLE_AND_HOLD automatically turns on BACKGROUND.  To turn on Sample and hold, uncomment a specific #define in the code.

Note that this is *not* a Random Sample and Hold.  It's just doing sample and hold on the underlying LFO wave.  For a random sample and hold, try my firmware STOCHASTIC.



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
Track and Hold / Sample and Hold Trigger
#### POT 1
Shape/PWM

[If not using the CV, set switch to MAN, else set to IN2 and put pot at roughly 2'oclock]
#### POT 2
Rate

[If not using the CV, set switch to MAN, else set to IN2 and put pot at roughly 2'oclock]
#### POT 3
Type (Short Saw/Tri/Ramp, Short Sine, Short Pulse, Medium Saw/Tri/Ramp, Medium Sine, Medium Pulse, Long Saw/Tri/Ramp, Long Sine, Long Pulse)

