# Tangle

Tangle is a combined digital logic function, merge, inverter, and buffered mult. You can use it to combine two digital signals, or to take two inputs and see to it that they both go to a single output (such as multiple ways to reset the SEQ16), or take a signal and buffer it.

SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)

Tangle has two INPUTS, three BUFFERED OUTPUTS, and an INVERTED OUTPUT.

It takes the values of the two inputs, runs them through a function you select via POT 3, then outputs the result to the three outputs, and outputs its inverse on the inverted output.

Given the two inputs A and B, the functions on POT 3 are (left to right):

- A					[B is ignored]
- A or B			[If either is HIGH, we output HIGH]
- A and B			[Both must be HIGH to output HIGH]
- A nequal (xor) B	[If A and B differ we output HIGH]


HOW YOU COULD USE TANGLE

1. As a buffered mult.  Set the function to just "A", and the input on A gets routed to buffered outputs 1, 2, and 3.

2. As an inverter.  Set the function to just "A", and the input on A gets inverted and set out the inverted output.

3. As a trigger merge.  Set the function to "A or B", and the inputs on A and B both get sent out the buffered outputs 1, 2, 3.  I use this to reset the SEQ16 while still allowing it to have only 7 steps.  SEQ16 step 7 output goes into B on TANGLE, and TANGLE's Output 1 goes to RESET on SEQ16.  My manual reset goes into A on TANGLE.

4. As a threshold.  You can test for when an analog CV is over a value, and only output high when that happens.  To do this, you set the function to "A" and change POT2 to your desired threshold (the maximum threshold, about 5V, is at the 2'oclock position, the minimum is far left).  

5. As a gate difference tester.  Set the function to "A xor B".  We output HIGH when either A or B are high, but if both are high, we output LOW to cut them off.  

6. As a tester for when two gates are high at the same time.  Set to "A and B".  You could threshold both of them as well (see "threshold" above).


### GRAINS BUG

There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the switch to "Man", then the range of the Pot is correct.  But if you set the switch to "In 1" (or "In 2"), then the range of the Dial is maximum (1.0 or 5V for our purposes) at about the 2 o'clock position on the Dial.  Beyond that it stays at 1.0


## Configuration

#### IN 1
Input 1
#### IN 2
Input 2
#### IN 3
Output 1
#### AUDIO IN (A)
Output 2
#### AUDIO OUT
Output 3
#### DIGITAL OUT (D) 
Inverted Output
#### POT 1
[Unused.  Set to IN1 and turn knob to approximately 2'oclock]
#### POT 2
[Unused.  Set to IN2 and turn knob to approximately 2'oclock]
#### POT 3
Degree of Delay 
