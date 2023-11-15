# Pulses

Pulses gives you various options for emitting separate pulse-wave LFOs or mixtures of them. Pulses is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

Pulses requires that you install the **FlexiTimer2** library.  A zip file for this library is provided.  You can load this zip file directly using the Arduino IDE's library facility, or just stick the enclosed directory in your Arduino library folder.

SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)

### FIRST OPTION

You can make the LFO run FAST or SLOW.  By default it is SLOW. To run FAST, modify a #define in the code as indicated in the **pulses.ino** file.


### SECOND OPTION

You have three choices for the CONFIGURATION of the LFOs. These choices are:

1. MAIN CONFIGURATION.  You have two independent Pulse-wave LFOs, and you can adjust their rates manually or via CV.  You can also adjust the pulsewidth of the first LFO either manually or via CV, but not both at the same time. You can reset both LFOs together.

2. "MULTI" CONFIGURATION.  You have THREE independent Square-wave LFOs, and you can adjust their rates manually.  The first two rates can also be adjusted by CV. The pulsewidth is 50% for all three LFOs.  You can reset all three LFOs together.

3. "MIX" CONFIGURATION.  The LFO is the MIXTURE of THREE independent Square-wave LFOs at different rates.  It changes from 1 to 0 or back whenever any combination of LFOs happen to flip.  You specify the rate of the first LFO manually or by CV.  The rate of the second LFO is specified as a PERCENTAGE of the rate of the first LFO.  It is specified manually or by CV.  The rate of the third LFO is specified the same way, except that you can adjust it manually, or by CV, but not both at the same time.  Thus your first LFO should be the SLOWEST one.  You can reset all three LFOs together.  The first LFO is also output independently: so if you feed this output back into the reset, you can sync the second and third LFOs to the first LFO each time the first LFO cycles.

The default configuration is MAIN.  You can change to MULTI or MIX by modifying #defines as indicated in the **pulses.ino** file.


### GRAINS BUG

There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the switch to "Man", then the range of the Pot is correct.  But if you set the switch to "In 1" (or "In 2"), then the range of the Dial is maximum (1.0 probability for our purposes) at about the 2 o'clock position on the Dial.  Beyond that it stays at 1.0.


## MAIN Configuration
This is the default configuration

#### IN 1
Rate 1 CV
#### IN 2
Rate 2 CV
#### IN 3
Pulsewidth 1 CV.  Only has an effect if POT 3 is at FULL
#### AUDIO IN (A)
Reset
#### AUDIO OUT
LFO 1 Out
#### DIGITAL OUT (D) 
LFO 2 Out
#### POT 1
Rate 1
#### POT 2
Rate 2
#### POT 3
Pulsewidth 1.  If FULL then Pulsewidth 1 CV is used instead


## MULTI Configuration

#### IN 1
Rate 1 CV
#### IN 2
Rate 2 CV
#### IN 3
LFO 3 Out  [Yes, it's an OUT]
#### AUDIO IN (A)
Reset
#### AUDIO OUT
LFO 1 Out
#### DIGITAL OUT (D) 
LFO 2 Out
#### POT 1
Rate 1
#### POT 2
Rate 2
#### POT 3
Rate 3


## MIX Configuration

#### IN 1
Rate 1 CV
#### IN 2
Rate 2 Percentage CV
#### IN 3
Rate 3 Percentage CV.  Only has an effect if POT 3 is at FULL
#### AUDIO IN (A)
Reset
#### AUDIO OUT
LFO 1 Out
#### DIGITAL OUT (D) 
Mixed LFO Out
#### POT 1
Rate 1
#### POT 2
Rate 2 Percentage
#### POT 3
Rate 3 Percentage, if FULL then Rate 3 Percentge CV is used instead

