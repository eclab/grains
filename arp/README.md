# ARP

Arp is a random arpeggio generator.  Aria is meant to run on the 
AE Modular GRAINS, but it could be adapted to any Arduino.

Think of Arp as TOPOGRAF for notes.  You are selecting a random arpeggio from a three-
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

## Grains Output Warning
This program outputs note CV information from GRAINS.  If you feed this into (for example) a VCO, 2OSC/d, or 2OSC, it will not be proper 1V/oct because they pull too much amperage and this causes the GRAINS to output incorrect voltages.  Even feeding into another GRAINS will cause the pitch to drop a bit.  You can fix this by feeding into a buffered mult first.  The 555 does not have this issue (its inputs are buffered).

## CLOCK OUT Option

Mozzi isn't very fast in responding to new notes.  For example, if you connect an LFO to ARP's clock,
and also connect to an envelope with a fast attack to change the filter, you'll find that the filter
starts to open before Mozzi has changed the note.  If the attack is a bit slower it'll sound fine.
But if you want a fast envelope, you need a way for ARP to tell the envelope that it has changed the
pitch.  To do this, you can uncomment a #define in the code.

This will change IN3 from TRANSPOSE to CLOCK OUT.  You can then hook IN3 up to your envelope and things
should sound better.  But you've lost transposition.

## Range and Resolution

It looks like GRAINS's range is about 42 notes (3.5 octaves or so), starting at a bit above 0V.  It's not capable of higher or lower values.

Furthermore, the range is a bit nonlinear. I have a lookup table in the code which gets as close as I can to my own GRAINS but I do not know if it matches other people's GRAINS. Send me mail and let me know how it does on your unit.

Also note that GRAINS's resolution is only about 100 ticks per octave.  This means I can maybe get within 7 cents of a note but not nail it exactly.


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

[If using Variance CV, set switch to IN 2 and set pot to 2'oclock, else set to MAN]
#### POT 2
Random

[If using Random CV, set switch to IN 2 and set pot to 2'oclock, else set to MAN]
#### POT 3
Pattern
