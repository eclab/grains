# Aria

Aria is a monophonic sequencer with song mode.  Aria is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

In Aria you define one or more PATTERNS, which are essentially melodies, and then you define one of more SONGS.  A PATTERN is a list of notes and their lengths. Lengths can be 16th notes, 8th notes, quarter notes, half notes, or whole notes. Notes can be regular, staccato, or legato (no new gate).  You can also have RESTS  for those lengths.  You can extend lengths further with TIES.

A SONG is a list of patterns.  For each pattern in a song you can say how many times each pattern is played before going on to the next pattern.  You can also stipulate that a pattern is played FOREVER until the musician advances to the next pattern in the song.  Songs can also be set up to loop forever.

Songs are begun by choosing a song, then raising a GATE.  Then sixteenth notes are counted each time Aria receives a CLOCK signal, and notes are outputted appropriately. When the GATE is dropped again, the song ends if it has not already done so.  As long as the gate is raised, you can also trigger an ADVANCE to force the song to go to the next pattern.

Songs are output as PITCH OUT and as GATE OUT, which can be used to drive an oscillator and a VCA envelope.

At present the gate for a sixteenth note is only dropped just before a new note, so there's no release, unless you defined staccato.  This may change in the future.

## Grains Output Warning
This program outputs note CV information from GRAINS.  If you feed this into (for example) a VCO, 2OSC/d, or 2OSC, it will not be proper 1V/oct because they pull too much amperage and this causes the GRAINS to output incorrect voltages.  Even feeding into another GRAINS will cause the pitch to drop a bit.  You can fix this by feeding into a buffered mult first.  The 555 does not have this issue (its inputs are buffered).


## Defining Patterns and Songs

PATTERNS and SONGS are defined in the code as shown.  At some point in the near future I might make a utility to convert MIDI files into patterns, so the patterns and/or songs may be moved out to a separate file.


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
Advance
#### IN 2
Transpose CV
#### IN 3
Clock
#### AUDIO IN (A)
Gate In
#### AUDIO OUT
Pitch Out
#### DIGITAL OUT (D) 
Gate Out
#### POT 1
[Unused]

[Set switch to IN1 and set pot to far right]
#### POT 2
Transpose

[If using Transpose CV, set switch to IN 2 and set pot to 2'oclock]

[If NOT using Transpose CV, set switch to MAN]

[If NOT transposeing at all, set switch to MAN and set pot to far left]
#### POT 3
Select Song