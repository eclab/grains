# MOTIF

Motif is a random short melody generator.  Aria is meant to run on the 
AE Modular GRAINS, but it could be adapted to any Arduino.

Think of Motif as TOPOGRAF for notes.  You are selecting a random melody from a three-
dimensional space.  The three parameters in question are the PATTERN, the VARIANCE,
and DETERMINISTIC RANDOM NOISE.  

The PATTERN is determined by the POT 3.  It is one of 32 values.  The first 16 values are different
basic patterns of up and down in MINOR.  The second 16 values are different basic patterns of up and 
down in MAJOR.

To the basic pattern we will add a fixed NOISE PATTERN to make the melody more interesting.  The
AMOUNT of noise -- the amount of jumping around and deviating from the basic pattern -- is determined
by the VARIANCE KNOB, POT 1.  For each level of variance there are 32 different noise patterns,
determined by the RANDOM KNOB, POT 2.

Once you have selected your pattern, you can CLOCK the pattern to produce a new note, and can RESET 
the pattern back to its start position.  

You also have the option to TRANSPOSE the pattern with a CV input (perhaps attached to a 4/ATTMIX),
but beware that it's not volt/octave at present, and GRAINS has a very limited range, only about 
3.5 octaves, so it's easy to go out of bounds by transposing.  By default a CV input value of 0
(detatched) is dead-center tranposed.

### Grains Output Warning
This program outputs note CV information from GRAINS.  If you feed this into (for example) a VCO, 2OSC/d, or 2OSC, it will not be proper 1V/oct because they pull too much amperage and this causes the GRAINS to output incorrect voltages.  Even feeding into another GRAINS will cause the pitch to drop a bit.  You can fix this by feeding into a buffered mult first.  The 555 does not have this issue (its inputs are buffered).

## CLOCK OUT Option

Mozzi isn't very fast in responding to new notes.  For example, if you connect an LFO to Motif's clock,
and also connect to an envelope with a fast attack to change the filter, you'll find that the filter
starts to open before Mozzi has changed the note.  If the attack is a bit slower it'll sound fine.
But if you want a fast envelope, you need a way for Motif to tell the envelope that it has changed the
pitch.  To do this, you can uncomment a #define in the code.

This will change IN3 from TRANSPOSE to CLOCK OUT.  You can then hook IN3 up to your envelope and things
should sound better.  But you've lost transposition.

## Output Pitch Range and Resolution

Mozzi cannot go down to 0V.  Its minimum is a little more, transposing up by about a half of a semitone.   Most oscillators can be tuned to deal with this. But GRAINS oscillators have to be manually adjusted.  So for example, if you're attaching  this program to a GRAINS oscillator like DX, you might want to change the TRANPOSE\_BITS to about -6.

Mozzi's output is capable of a range of betwen 42 and 48 notes (betwen 3.5 and 4 octaves).   This is also the quantizer's range: values above that will just get quantized to the top note.

One of the issues in using this quantizer is that GRAINS does not have a buffered: the voltage its output will produce is strongly affected by the amperage being pulled by the oscillator it's plugged into, and different AE oscillators pull different amounts. VCO is particularly bad here -- it pulls a lot of voltage, thus scaling down Quant's output so it's no longer v/oct.  555 is much better (its inputs are buffered).  And you can generally fix matters by plugging GRAINS into a buffered mult, and then attaching the buffered mult to your oscillator.  But even the buffered mults differ a bit!

I have made a few tables to match different scenarios:

1. You are plugged directly into a VCO
2. You are plugged directly into a 555
3. You are plugged directly into a uBUFFER.  The uBUFFER is attached to your oscillator(s).
4. You are plugged directly into a 4BUFFER.  The 4BUFFER is attached to your oscillator.
5. You are plugged directly into another GRAINS oscillator.  In this case, I suggest using the OUTPUT\_UBUFFER #define, setting TRANSPOSE\_BITS on the GRAINS oscillator to about -6, and tweaking the tracking via POT1.  It should work.
6. You are plugged directly into a 2OSC/d
7. You are plugged into a 2OSC: in this case, may your god have mercy on your soul.

You need to specify what you're plugged into, which will change the pitch table, using one the #defines in the code.


## Configuration

#### IN 1
Variance CV
#### IN 2
Random CV
#### IN 3
Transpose CV
#### AUDIO IN (A)
Reset
#### AUDIO OUT
Pitch Out
#### DIGITAL OUT (D) 
Clock
#### POT 1
Variance

[If using Variance CV, set switch to IN 1 and set pot to 2'oclock, else set to MAN]
#### POT 2
Random

[If using Random CV, set switch to IN 2 and set pot to 2'oclock, else set to MAN]
#### POT 3
Pattern
