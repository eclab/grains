# Bit 

BIT is a bitcrusher.  It takes an audio input at AUDIO IN, and reduces its bits
via POT 1 and its sampling rate via POT 2.  Then it changes the gain, likely clipping,
by POT 3.  The output is sent to AUDIO OUT.  That's it!

BIT is pretty filthy, aliased, and anharmonic.  If you need to mess something up, it's a good choice.
Note that Grains inputs at a resolution of 1024, but outputs at most at a resolution of 488.
Thus we're already bitcrushing in the output to begin with!

Because it takes Audio input, BIT requires turning on that ability in the Mozzi code, so you'll have
to muck about a bit with the Mozzi library itself.  I give some instructions how to do that.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

## Configuration

#### IN 1
Number of Bits CV
#### IN 2
Frequency CV
#### IN 3
[UNUSED]
#### AUDIO IN (A)
Sound Input
#### AUDIO OUT
Sound Output
#### DIGITAL OUT (D) 
[UNUSED]
#### POT 1
Number of Bits (normal is full left)

[If using Bits CV, set switch to IN1 and set pot to about 2'oclock]
#### POT 2
Frequency (normal is full left)

[If using Frequency CV, set switch to IN1 and set pot to about 2'oclock]
#### POT 3
Gain
