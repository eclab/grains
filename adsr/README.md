# ADSR

ADSR is an Attack/Sustain/Decay/Release envelope, except that the Decay and Release are normally the same value (because we don't have enough pots!).  Alternatively it can be an Attack/Hold/Release (AHR) envelope.  ADSR is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode, though it could probably be adapted to it.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

Note: ADSR is an **exponential** rate-based envelope.  This means it'll sound best with stuff like filters, and won't be amazing for VCAs.  I might build a **linear** time-based envelope later for the VCA but not now.

## Standard ADSR Mode

ADSR is easy to use: Pot 1 is the attack rate, Pot 2 is the Decay/Release rate, and Pot 3 is the sustain level.  ADSR's attack and decay are triggered when the gate is ON and the release is begin when the gate turns OFF thereafter.  When the envelope is finished, ADSR sends its envelope completion trigger.

Note that Gate (Input) is on **D**.

## Separate Release Mode

Ordinarily RELEASE AND DECAY are the same thing, controlled by Pot 2.  However you can separate them, forming a true ADSR envelope, by changing a #define in the code.  In this configuration, RELEASE will be specified by a CV on IN 3, and DECAY will stay on Pot 2.  


## AHR One-Shot Mode

You can change ADSR to be a one-shot AHR envelope (Attack Hold Release) instead.  This envelope has no SUSTAIN or DECAY.  It attacks as usual, but instead of DECAYING it HOLDS at 1.0 for the duration of its DECAY stage, then RELEASES when that stage is done.  Pot 2 changes to a HOLD TIME knob (basically the "decay time but not actually decaying"), and Pot 3 changes to a dedicated RELEASE TIME knob. 

You cannot have both RELEASE mode and AHR mode at the same time.  AHR mode will disable RELEASE mode.

When AHR has completed its envelope, the envelope completion trigger is sent.  However if the gate has not yet been turned off, and the trigger output is attached to the RESET input, the whole envelope will loop until the gate is released. 


## GRAINS BUG

There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the 
switch to "Man", then the range of the Pot is correct.  But if you set the switch 
to "In 1" (or "In 2"), then the range of the Dial is at maximum at about the 
2 o'clock position on the Dial.  Beyond that it stays at maximum.


## Grains Bug

There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the  switch to "Man", then the range of the Pot is correct.  But if you set the switch  to "In 1" (or "In 2"), then the range of the Dial is at maximum at about the 2 o'clock position on the Dial.  Beyond that it stays at maximum.


## Standard ADSR Configuration

#### IN 1
Attack CV
#### IN 2
Decay / Release CV
#### IN 3
Release Completion Trigger Output 
#### AUDIO IN (A)
Reset
#### AUDIO OUT
Output
#### DIGITAL OUT (D) 
Gate
#### POT 1
Attack

[If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
#### POT 2
Decay / Release

[If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
#### POT 3
Sustain


## Separate Release Configuration

#### IN 1
Attack CV
#### IN 2
Decay CV
#### IN 3
Release CV 
#### AUDIO IN (A)
Reset
#### AUDIO OUT
Output
#### DIGITAL OUT (D) 
Gate
#### POT 1
Attack

[If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
#### POT 2
Decay

[If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
#### POT 3
Sustain


## AHR One-Shot Configuration

#### IN 1
Attack CV
#### IN 2
Hold CV
#### IN 3
Release Completion Trigger Output 
#### AUDIO IN (A)
Reset
#### AUDIO OUT
Output
#### DIGITAL OUT (D) 
Gate
#### POT 1
Attack

[If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
#### POT 2
Hold

[If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
#### POT 3
Release


