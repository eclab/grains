# Elements

Elements is a one-track Euclidian drum trigger sequencer.  Elements is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino. Elements was written by Sean Luke [sean@cs.gmu.edu]

SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)

The Euclidian sequence fad started with the paper "The Euclidean Algorithm Generates Traditional Musical Rhythms" by Godfried Toussaint. It is my opinion that the claims of presence of Euclidian sequences in worldwide drum sequences is dubious at best, but whatever: musicians are enamored with it!

See https://en.wikipedia.org/wiki/Euclidean_rhythm

All a Euclidian Sequencer attempts to do is evenly distribute K beats within N >= K steps, using a certain uniform statistical pattern.  This pattern was originally derived from Bjorklund's Algorithm (which is related to Euclid's Algorithm), but in fact it is also a trivial result of Bressenham's Line Algorithm, which is simpler to compute (and we're using it below).

We have three major parameters:

- The SIZE of the sequence (N)
- The NUMBER OF BEATS in the sequence (K)
- The ROTATION of the sequence -- which step the sequence starts on (from 1..N), or 0, which steps on step 1 but changes DIGITAL OUT to only pulse once per sequence start [see below].

If you change these parameters, Elements will have to recompute and restart the sequence.  

Elements pumps out the sequence via AUDIO OUT, and the reversed sequence via DIGITAL OUT (D). Alternatively, if you're having a hard time determing the length of your sequence, then if you set the ROTATION to 0% (full left), then DIGITAL OUT (D) will change to instead trigger once each sequence iteration.

Perhaps it's hard for you to dial in the right sequence length, and you don't need or want 23 or 32 or 17 steps as options.  There is #define in the code which permits you to restrict Elements to only offering 6, 8, 9, 12, 16, 18, 24, and 32 steps as options.  That's a lot easier to dial in!  You could customize those too if you wanted.


##  Configuration

#### IN 1
Size CV, up to 32 steps
#### IN 2
Number of Beats (as a percentage of Size) CV
#### IN 3
Clock
#### AUDIO IN (A)
Reset
#### AUDIO OUT
Euclidian Rhythm Out
#### DIGITAL OUT (D) 
Reverse Euclidian Rhythm Out, or just the first step of sequence if POT3 is fully left
#### POT 1
Size, up to 32 steps
#### POT 2
Number of Beats (as a percentage of Size)
#### POT 3
Rotation (as a percentage of Size)


### USING DRUMKIT010
Be sure to set DrumKit1010's TRIG MODE switch to "D" (Digital), so it properly responds to triggers.

## GRAINS BUG

There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the switch to "Man", then the range of the Pot is correct.  But if you set the switch to "In 1" (or "In 2"), then the range of the Dial is maximum (1.0 probability for our purposes) at about the 2 o'clock position on the Dial.  Beyond that it stays at 1.0.


