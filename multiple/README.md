# Multiple

Multiple is a two-track clock multiplier.  Multiple is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.  Multiple was written by Sean Luke [sean@cs.gmu.edu]

Set your Grains to **GRAINS MODE**.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)

Multiple takes a clock input and attempts to produce a multiple of pulses of it.  You can specify the pulsewidth.  The options for multiples are presently:

- 16 pulses per clock in
-  7 pulses per clock in
-  6 pulses per clock in
-  5 pulses per clock in
-  4 pulses per clock in
-  3 pulses per clock in
-  "Swing": 3 pulses per 2 clock ins, middle pulse omitted
-  2 pulses per clock in

### Notes

After you reset, Multiple will always wait a beat so it can measure the beat length in order to get the right sub-beat.  This could be improved in some cases, so it does not wait: I'll work on it.  

If you change the speed, Multiple will not catch up until a beat or two later.  That could be improved as well.

Multiple was meant to be able to do polyrhythms on the beat, but at present bugs are preventing this from happening.

If you change the pot to a value "in between" two options, Multiple will get confused and may not pulse at all.  Move the pot until Multiple is locked on.


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
Clock Multiple 1
#### DIGITAL OUT (D) 
Clock Multiple 2
#### POT 1
Choice of Clock Multiple 1   [Set the switch to MAN]
#### POT 2
Choice of Clock Multiple 2   [Set the switch to MAN]
#### POT 3
Pulsewidth


### USING DRUMKIT010
Be sure to set DrumKit1010's TRIG MODE switch to "D" (Digital), so it properly responds to triggers.

