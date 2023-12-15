# Multilevel

Multilevel is a multistage envelope with exponential rate-based transitions, sustain loops, whole-envelope loops, resetting, flat (sample-and-hold style) stages, and per-stage triggering as options.   Multilevel is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode, though it could probably be adapted to it.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

At present you can probably have about 145 stages.  If you needed more, we could probably convert the stages to PROGMEM and that would get you another 3000 stages or so.  But I'm guessing 145 will be more than plenty!

Note: Multilevel is an **exponential** rate-based envelope.  This means it'll sound best with stuff like filters, and won't be amazing for VCAs.  I might build a **linear** time-based envelope later for the VCA but not now.

In Multilevel you define the envelope here in code.  It's not too hard.  Then in real time you can change the RATE, the MAXIMUM LEVEL, and the MINIMUM LEVEL. (If the maximum level is less than the minimum level, the two are swapped). When the envelope is GATED it starts its run.  When it completes the SUSTAIN stage, it either holds there, or it loops back to the LOOP START STAGE, depending on  whether we have turned LOOPING on.  When the gate is RELEASED, the envelope jumps jumps to the stage just beyond the SUSTAIN stage, and continues from there until it has terminated.

Multilevel has three MODES.  The first mode is REGULAR, which acts like a regular envelope with sustain possibly a looping sustain.  The second mode is ONE SHOT, which ignores sustain and just progresses through the envelope until it reaches the end.  The third mode is STAGE TRIGGERING, which does not advance from stage to stage unless it receives a STAGE TRIGGER. There is no sustain or sustain loop.   STAGE TRIGGERING does not use GATE so the trigger is located in that socket.  

You can also RESET the envelope.  As long as the GATE is ON, resetting will cause the envelope to reset to its starting position and continue from there.  If the mode is SUSTAIN TRIGGERING then reset works at any time (since there is no gate).

When the envelope terminates, it triggers the ENVELOPE COMPLETION TRIGGER.  Among other things, you can connect the envelope completion trigger to the reset in a ONE SHOT  envelope, and it will loop forever as long as the gate is ON.

We can also state whether our stages are FLAT or CHANGING.  A FLAT stage is kind of like a Sample and Hold: the stage immediately goes to and outputs its target value until the underlying curve has reached the target value as usual. 

Pot 3 is used to change the mode and to set whether we are flat or not.


## Grains Bug

There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the  switch to "Man", then the range of the Pot is correct.  But if you set the switch  to "In 1" (or "In 2"), then the range of the Dial is at maximum at about the 2 o'clock position on the Dial.  Beyond that it stays at maximum.


## Configuration

#### IN 1
Maximum Level CV
#### IN 2
Minimum Level CV
#### IN 3
Gate or Stage Trigger Input 
#### AUDIO IN (A)
Reset
#### AUDIO OUT
Output
#### DIGITAL OUT (D) 
Envelope Completion Trigger Output
#### POT 1
Maximum Level

[If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
#### POT 2
Minimum Level

[If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
#### POT 3
Options

Left to right: CHANGING REGULAR, CHANGING ONE SHOT, CHANGING STAGE TRIGGER, FLAT REGULAR, FLAT ONE SHOT, FLAT STAGE TRIGGER
