# CHRONO

CHRONO is a tappable tempo clock.  CHRONO is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

CHRONO is as simple as it gets: it provides triggers at a constant rate out DIGITAL OUT. You provide taps (gates, triggers) in IN 1 and it will adjust its trigger output rate  appropriately.  On top of it, it'll save that rate so that next time you power up, the rate will still be there.  NOTE: depending on what kind of data was in your EEPROM before, your FIRST rate may be crazy.  Don't worry about it, just tap in a new rate.

You can also "tap" a rate by twisting POT 1 from a position lower than 50% to a position greater than 50% (and back), if you have the switch set to MAN.

You can also RESET the clock.

CHRONO's tempo is taken directly from the difference in your last two taps.  Maybe at some point we might change this to have it average over several taps.  But not for now.  So your tapping will have to be accurate.


## GRAINS BUG

There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the  switch to "Man", then the range of the Pot is correct.  But if you set the switch to "In 1" (or "In 2"), then the range of the Dial is maximum (1.0 probability for our purposes) at about the 2 o'clock position on the Dial.  Beyond that it stays at 1.0



## Configuration

#### IN 1
Tap Tempo CV
#### IN 2
[Unused]
#### IN 3
[Unused]
#### AUDIO IN (A)
Reset
#### AUDIO OUT
[Unused]
#### DIGITAL OUT (D) 
Output Trigger
#### POT 1
Tap Tempo 

If you want to tap tempo via CV, set the Switch to IN1, and turn knob to about 2'oclock.
If you want to tap tempo by turning the knob, set the Switch is MAN
#### POT 2
[Unused]
#### POT 3
[Unused]

