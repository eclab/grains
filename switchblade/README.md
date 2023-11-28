# Switchblade

Switchblade is a combination lag smoother, fuzzer, and summed attenuverter.  Switchblade is an experiment in using Mozzi to provide CV out rather than audio out. Switchblade is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

SWITCHBLADE can lag-smooth a signal and it can add noise to the signal, but it cannot do both at the same time, because they share the same control.

SWITCHBLADE works like this:

1. It first takes the input in IN 1, attenuated by POT1, and adds in IN 3.  Note that IN3 reaches its maximum early due to same problem as the GRAINS bug.  If nothing is plugged into IN3, it will be 0. 

2. It then smooths the result to the degree instructed.  

3. It then converts the signal with the attenuverter, flipping and potentially attenuating it around 0.5.

4. Finally, it adds noise to the result to the degree instructed.


GRAINS's output is naturally discretized to 488 steps.  That's the finest-grain it can do.  Also, GRAINS's output circuitry is tuned for audio, not for CV.  So do not expect this tool to be particularly clean in its output, particularly when making fast jumps such as from a square wave LFO.  You should expect some small  degree of waver.  There is nothing that can be done for this.

## GRAINS Bug

There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the  switch to "Man", then the range of the Pot is correct.  But if you set the switch  to "In 1" (or "In 2"), then the range of the Dial is at maximum at about the 2 o'clock position on the Dial.  Beyond that it stays at maximum.  For the same reason, IN3 reaches maximum around 1.5 times faster than it should.


## Configuration

#### IN 1
Input
#### IN 2
Attenuvert CV
#### IN 3
Second Input
#### AUDIO IN (A)
[UNUSED]
#### AUDIO OUT
Output
#### DIGITAL OUT (D) 
[Unused]
#### POT 1
Input Attenuation\
[Set Switch to IN3.  Note GRAINS bug]
#### POT 2
`Fully Inverted <-------- Fully Attenuated --------> Normal`\
[If you're not using Attenuvert CV, set the switch to MAN]\
[If you are using Attenuvert CV, note GRAINS bug]
#### POT 3
Smoothing and Noise\
`Very Smooth <------- Normal -------> Very Noisy` 

