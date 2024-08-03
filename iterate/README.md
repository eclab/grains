# Iterate

ITERATE is a sequential switch, similar to the Doepfer A151.  It takes a series of gates or triggers and breaks them up among up to four digital outputs.  You can use this to trigger different drums, for example, based on a single input.  You can also RESET the trigger sequence at any time.  Note: ITERATE is not debounced, so noise on the gates or triggers might cause it to advance prematurely.

You can set the number of outputs to 2, 3, or 4 outputs, or to set them to 3 or 4 outputs where a randomly different output is chosen each time (the same output is never chosen twice).

### Audio Out Warning
Audio Out (Output #4 in Iterate) is slow because of its PMW filter -- it's not meant for this purpose. Expect triggers and gates on Audio Out to be a bit late.

### Grains Bug

There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the switch to "Man", then the range of the Pot is correct.  But if you set the switch to "In 1" (or "In 2"), then the range of the Dial is maximum (1.0 or 5V for our purposes) at about the 2 o'clock position on the Dial.  Beyond that it stays at 1.0


## Configuration

#### IN 1
Input
#### IN 2
Reset
#### IN 3
Output 2
#### AUDIO IN (A)
Output 3
#### AUDIO OUT
Output 4
#### DIGITAL OUT (D) 
Output 1
#### POT 1
[Unused.  Set to IN1 and turn knob to approximately 2'oclock]
#### POT 2
[Unused.  Set to IN2 and turn knob to approximately 2'oclock]
#### POT 3
Number of Outputs (left to right: 2, 3, 4, 3 random, 4 random) 

