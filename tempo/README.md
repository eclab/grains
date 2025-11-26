# Tempo

Tempo is a master clock is meant to run on the AE Modular GRAINS, but it could be adapted
to any Arduino.  Theory was written by Sean Luke [sean@cs.gmu.edu]

SET GRAINS TO GRAINS OR MOZZI MODE.  (It doesn't matter, we don't use Audio Out)

Tempo allows you to specify a beat, either by dialing in a Beats Per Minute on
POT 1 and POT 2, or by providing a tap tempo.  Then you can add swing or random
delay noise to the beat via POT 3.  Tempo then outputs the beat.

To specify a beat in beats per minute, you set POT 1 and POT 2.  POT 1 is divided
into sixteen sections, representing the "coarse" values 
0, 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, and 240
POT 2 is similarly divided into sixteen sections, representing the values 0...15.
The BPM is the two added together.

If you turn BOTH POT 1 AND POT 2 to ZERO, Tempo will instead read its beat from
tap tempo derived from taps (triggers) that you provide via IN 3.

Tempo outputs triggers at the given BPM out Digital Out.

POT 3 indicates some amount of DELAY that is added to the output triggers.  At full left,
the DELAY is ZERO -- it's just the beat itself.  As you increase the pot clockwise,
Tempo increasingly performs looser and looser swing on every other beat.  When POT 3
passes its midpoint, Tempo starts with a zero Delay again, and then increasingly performs larger and larger random delays
(humanization).

Tempo will only run if it receives a trigger on AUDIO IN.  Thereafter it will run
until it receives a second trigger to stop it.  You can then restart it with a third
trigger, and so on.

By default Tempo doesn't run until you trigger it.  If you like, you can change this
behavior so that Tempo immediately starts running on startup.  To do that, uncomment
a #define as shown in the code.


## Configuration

#### IN 1
CV for Coarse BPM
#### IN 2
CV for Fine BPM
#### IN 3
Tap Tempo	
#### AUDIO IN (A)
Start / Stop
#### AUDIO OUT
[UNUSED]
#### DIGITAL OUT (D) 
Beat Out
#### POT 1
Coarse BPM
#### POT 2
Fine BPM
#### POT 3
Swing / Noise added to Beat 
[No Swing--> Increasing Swing ---> No Noise ---> Increasing Noise]
