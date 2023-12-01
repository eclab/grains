# 808

808 is five-voice drum synthesizer with low-rent, noisy 8-bit drum samples presently taken from a Roland TR-808.  808 is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

808 is pretty simple: you have five drum triggers, one each for Kick, Clap, Rim, Claves, and Closed Hat.  Pot 3 will let you change the volume: you'll need to decrease enough that the combination of all the drums hitting at the same time doesn't produce an audible click or clipping.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

You can swap out the claves for a High Tom, Mid Tom, or Low Tom in the Code.

In the future I may generalize this to do more drum samples.

## Configuration

#### IN 1
Rim Trigger
#### IN 2
Hat Trigger
#### IN 3
Claves Trigger 
#### AUDIO IN (A)
Clap Trigger
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
Kick Trigger
#### POT 1
UNUSED [Set the switch to IN1]
#### POT 2
UNUSED [Set the switch to IN2]
#### POT 3
Volume Adjustment