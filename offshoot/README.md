# Offshoot

Offshoot is more or less a copy of [Mutable Instruments Branches](https://pichenettes.github.io/mutable-instruments-documentation/modules/branches/).  That is, it is a dual Bournoulli Gate.  Offshoot was written by Sean Luke [sean@cs.gmu.edu]

SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV\_AUDIO_OUT to 9)

Offshoot has two inputs for triggers or gates (1 and 2).  Each input trigger/gate is randomly routed 
to one of two outputs (A and B).  Thus trigger/gate4 1 gets routed to either output 1A or 1B,
and trigger/gate 2 gets routed to either output 2A or 2B.  The probability of routing
is determined by the PROBABILITY knob (Pot 3).

The probability knob works as follows.  If the knob is turned fully to the left,
then the output will always be A.  As you turn the knob to the right, the probability
increases that B will be chosen instead of A, until you reach the halfway point, where
the probability is equal.  Now if you continue to turn the knob to the right, the
behavior changes: the probability is now the likelihood that the output will SWITCH from
whatever it was last time.  Initially this is high, and as you turn the knob to the far
right the probability decreases to the point where it stays with its previous value
for a long time and ultimately permanently.

NOTE: triggers/gates sent to Audio Out are sloppy and slow.

## Four-Trigger-Gate / CV Option

You can chance Offshoot to select among all four outputs from a single input
trigger/gate.  To do this, uncomment a certain #define as shown in the code.

If you do this, then Pot 2 / CV 2 will be used as a SECOND probability knob, to determine 
whether Outputs 1A / 1B will be used or if Outputs 2A / 2B will be used.  Then Pot 3 is 
used to choose between A and B.  In addition to allowing four outputs, this also allows 
CV control over the probability.  For example, if you set POT 3 to FULL LEFT, so A is 
always chosen, then you can send CV in to IN2 (set POT 2 to about 2 o'clock) and the CV 
will choose between outputs 1A and 2A.

## Latch Mode

You can also cause Offshoot to only reset an output (set it to 0) upon triggering a new output.  Mutable Instruments Branches calls this "Latch Mode".  To turn on Latch Mode, uncomment a certain #define as shown in the code.



## Configuration

#### IN 1
Input 1
#### IN 2
*Normally:* Input 2

*Four-Output option:*  Probability of selecting between Outputs 1 and Outputs 2
#### IN 3
Output 1B
#### AUDIO IN (A)
Output 2A
#### AUDIO OUT
Output 2B
#### DIGITAL OUT (D) 
Output 1A
#### POT 1
[Unused.  Set to IN1 and turn knob to approximately 2'oclock]
#### POT 2
*Normally:* [Unused.  Set to IN1 and turn knob to approximately 2'oclock]

*Four-Output option:*  CV for probability of selecting between Outputs 1 and Outputs 2.  If Unused, set to IN2 and turn knob to approximately 2'oclock 

#### POT 3
Probability (Selecting between A and B)

### GRAINS BUG

There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the switch to "Man", then the range of the Pot is correct.  But if you set the switch to "In 1" (or "In 2"), then the range of the Dial is maximum (1.0 or 5V for our purposes) at about the 2 o'clock position on the Dial.  Beyond that it stays at 1.0
