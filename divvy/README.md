# Divvy

Divvy is a two-track beat divider with pulsewidth. Divvy is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.  Divvy was written by Sean Luke [sean@cs.gmu.edu]

Set your Grains to **GRAINS MODE**.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)

Multiple takes a clock input and produces a division of it.  By default your options are:

## Clock Division Choices

At present the options are (left to right):

- 96 clocks per pulse		     (MIDI Clock bars)
- 64 clocks per pulse
- 32 clocks per pulse
- 24 clocks per pulse		     (MIDI Clock quarter notes)
- 16 clocks per pulse
- 8 clocks per pulse
- 6 clocks per pulse		     (MIDI Clock 16th notes)
- 4 clocks per pulse offset by 2 (Binary 4 digit)
- 4 clocks per pulse
- 3 clocks per pulse
- 2 clocks per pulse offset by 1 (Binary 2 digit)  (1 digit is the main clock pulse)
- 2 clocks per pulse

You can easily change these options in the code, to anything you liked.

## Configuration
This is the default configuration

#### IN 1
[UNUSED]
#### IN 2
[UNUSED]
#### IN 3
Clock
#### AUDIO IN (A)
Reset
#### AUDIO OUT
Clock Division 1
#### Division OUT (D) 
Clock Multiple 2
#### POT 1
Choice of Clock Division 1   [Set the switch to MAN].  Note that changing this value triggers a reset.
#### POT 2
Choice of Clock Division 2   [Set the switch to MAN].  Note that changing this value triggers a reset.
#### POT 3
Pulsewidth, rounded down to nearest clock.  If at 0, a trigger pulse is sent instead.


### USING DRUMKIT010
Be sure to set DrumKit1010's TRIG MODE switch to "D" (Digital), so it properly responds to triggers.

