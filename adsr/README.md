# ADSR

ADSR is an Attack/Sustain/Decay/Release envelope, except that the Decay and Release are normally the same value (because we don't have enough pots!).  Alternatively it can be an Attack/Hold/Release (AHR) envelope.  ADSR is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode, though it could probably be adapted to it.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

Note: ADSR is an **exponential** rate-based envelope.  This means it'll sound best with stuff like filters, and won't be amazing for VCAs.  I might build a **linear** time-based envelope later for the VCA but not now.

ADSR is started by a GATE, which is presented at Digital Out (D).  Yes, it's an INPUT. While ADSR is gated, you can RESET it back to its start position by sending a RESET TRIGGER to Audio In (A). 

## Modes

ADSR comes in four MODES, settable with #defines in the code:

- "Standard" (ADSR) mode.  This is the DEFAULT SETTING.  Here Decay and Release are the same value.  Pot 1 is the attack rate, Pot 2 is the Decay/Release rate, and Pot 3 + In 3  is the sustain level.

- Separate Release mode.  Here, Release is separated from Decay, and now occupies In 3 as a CV. Sustain is just Pot 3 (there is no Sustain CV).

- ASR (Attack/Sustain/Release) mode.  Here there is no Decay at all.  Instead, Pot 2 is release.  This is similar to the ASR envelope in 2ENV, except that Sustain can still be adjusted via Pot 3 + In 3.

- AHR (Attack/Hold/Release) mode.  This is a one-shot envelope with no sustain and no decay. Instead, after attacking to MAXIMUM (not quite 5V), the envelope HOLDS at that value for a certain amount of time and then automatically releases to 0V.  Hold occupies Pot 3 instead of Sustain.  AHR mode sports an ENVELOPE COMPLETION TRIGGER on In 3, which you can attach to RESET to cause the whole envelope to loop over and over again as long as gate is ON. If you set Hold to 0, then this degenerates to an AR envelope similar to that found in 2ENV

You can only set one mode.  AHR takes precedence over ASR, which takes precedence over Separate Release, which takes precedence over Standard.


## Attack Level

There is a #define in the code to set the level to which Attack rises before Decay sets in.  It also affects the
Hold level in AHR but has no effect on ASR.


## Inverting

There is a #define in the code you can set to invert the envelope: thus high values become low and vice versa.  This is particularly useful for attaching to a VCA to do ducking.  Note that if inverting is turned on, then the Attack Level is also inverted: thus if you change the Attack Level from 1.0 to 0.8, then it will be inverted to 0.2 rather than 0.0.  This is useful to duck but not completely remove a sound.


## Grains Bug

There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the  switch to "Man", then the range of the Pot is correct.  But if you set the switch  to "In 1" (or "In 2"), then the range of the Dial is at maximum at about the 2 o'clock position on the Dial.  Beyond that it stays at maximum.


## Standard ADSR Configuration

#### IN 1
Attack CV
#### IN 2
Decay / Release CV
#### IN 3
Sustain CV 
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


## ASR Configuration

#### IN 1
Attack CV
#### IN 2
Release CV
#### IN 3
Sustain CV 
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
Release

[If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
#### POT 3
Sustain



## AHR One-Shot Configuration

#### IN 1
Attack CV
#### IN 2
Release CV
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
Release

[If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
#### POT 3
Hold

