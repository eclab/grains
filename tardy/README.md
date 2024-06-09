# Tardy

Tardy takes gates or triggers at its three inputs and re-echoes them to its outputs with a small and settable amount of delay (up to about a half-second). This is primarily meant to get drums etc. in sync with other devices that have some degree of latency.  Tardy was written by Sean Luke [sean@cs.gmu.edu]

SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)

If you turn the Delay knob, it'll take Tardy a few seconds to converge to the new delay value: this is a consequence of Tardy's way of dealing with noise in the delay knob.  Be patient.

**NOTE.*  Output to Digital Out is likely to be EVER SO SLIGHTLY FASTER than output to Audio In or Audio Out.  If Output 1 is a bit ahead of Outputs 2 and 3, let me know and I can try to tweak things.



## Configuration

#### IN 1
Input 1
#### IN 2
Input 2
#### IN 3
Input 3
#### AUDIO IN (A)
Output 3
#### AUDIO OUT
Output 1
#### DIGITAL OUT (D) 
Output 2
#### POT 1
[Unused.  Set to IN1 and turn knob to approximately 2'oclock]
#### POT 2
[Unused.  Set to IN2 and turn knob to approximately 2'oclock]
#### POT 3
Degree of Delay 


### GRAINS BUG

There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the switch to "Man", then the range of the Pot is correct.  But if you set the switch to "In 1" (or "In 2"), then the range of the Dial is maximum (1.0 or 5V for our purposes) at about the 2 o'clock position on the Dial.  Beyond that it stays at 1.0
