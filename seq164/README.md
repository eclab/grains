# Seq164

Seq164 turns TRIQ164 into a quantized note sequencer.  Seq164 is meant to run on the AE Modular GRAINS, 
but it could be adapted to any Arduino.

Seq164 takes all four of the trigger outputs of TRIQ164 and interprets them as one of 15 notes, outputting
the note CV and a trigger.  This allows you to enter note patterns into TRIQ16 and play them as notes or
as rests.

To use Seq164, you connect Outs 1, 2, 3, and 4 on your TRIQ164 to trigger inputs "1, 2, 3, and 4" on the
GRAINS (namely IN1, IN2, IN3, and AUDIO IN respectively).  GRAINS will produce CV output as AUDIO OUT and
will send a trigger on DIGITAL OUT whenever it receives triggers from Seq164.  

Seq164 can output 15 different notes.  These notes are selected from a SCALE (or CHORD).  You choose the 
Scale with POT 3.  Seq164 will choose the Nth note, going up octaves if necessary.  For example, if you
chose MAJOR -- let's imagine it was C major for discussion here -- then the 15 notes would be
C, D, E, F, G, A, B,  then [from the next octave] C, D, E, F, G, A, B, then [from the next octave] C.

Additionally, if NO TRIGGERS are being output from TRIQ164, then Seq164 will not change its chord and will 
not trigger a note.  You can think of this as a rest.

The four triggers determine the note (or rest) as follows (0 means OFF and 1 means ON, that is, TRIGGERED):


    OUT 1	OUT 2	OUT 3	OUT 4	VALUE
    0		0		0		0		Rest	[No trigger, CV doesn't change]
    1		0		0		0		Note 1
    0		1		0		0		Note 2
    1		1		0		0		Note 3
    0		0		1		0		Note 4
    1		0		1		0		Note 5
    0		1		1		0		Note 6
    1		1		1		0		Note 7
    0		0		0		1		Note 8
    1		0		0		1		Note 9
    0		1		0		1		Note 10
    1		1		0		1		Note 11
    0		0		1		1		Note 12
    1		0		1		1		Note 13
    0		1		1		1		Note 14
    1		1		1		1		Note 15

You will find this pretty cumbersome to program because TRIQ164 can only show one output at a time.
You might find Seq164 more useful as a tool for happy accidents: just plug in random stuff to
TRIQ164 and see what happens


## Scales

The scale or chord is chosen by POT 3.  They are at present (left to right):

    Major
    Melodic Minor 
    Dorian
    Mixolydian
    Aeolian (Relative Minor)
    Blues Minor
    Pentatonic
    Minor Pentatonic
    Japanese Pentatonic
    Chromatic
    Major Triad + Major 2nd + Major 6th
    Major Triad + Major 2nd + Major 7th
    Minor Triad + Major 2nd + Minor 6th
    Minor Triad + Major 2nd + Minor 7th
    Minor Triad + Major 2nd + Minor 6th + Minor 7th
    Fully Diminished 7th


## Repeats

Seq164 might work with TRIQ164 repeats or it might not, because repeats are individual triggers,
and if you don't have all the triggers of a given step repeating, the note might change during
the repeat.  At least that's what I presume: so far I've not gotten Seq164 and TRIQ164 to behave
badly in this way.


## Latency

Seq164 is a little pokey.  It needs about 1/128 of a second to determine that all the triggers
have arrived from TRIQ164 and it can buid the note.  Then it takes about 1/80 of a second to
get GRAINS to output the right note pitch before it can issue its trigger output on Digital Out.
This is the nature of the GRAINS hardware.  To improve on this latency would start creating clicks
and (theoretically) missed triggers. 


## Output Pitch Range and Resolution

Mozzi cannot go down to 0V.  Its minimum is a little more, transposing up by about a half
of a semitone.   Most oscillators can be tuned to deal with this.
But GRAINS oscillators have to be manually adjusted.  So for example, if you're attaching 
this program to a GRAINS oscillator like DX, you might want to change the TRANPOSE_BITS
to about -6.

Mozzi's output is capable of a range of betwen 42 and 48 notes (betwen 3.5 and 4 octaves).  
This is also the quantizer's range: values above that will just get quantized to the 
top note.

One of the issues in using this firmware is that
the voltage GRAINS's output will produce is strongly affected by the amperage being pulled
by the oscillator it's plugged into, and different AE oscillators pull different amounts.
VCO is particularly bad here -- it pulls a lot of voltage, thus scaling down GRAINS's output
so it's no longer v/oct.  555 is much better (its inputs are buffered).  And you can
generally fix matters by plugging GRAINS into a buffered mult, and then attaching
the buffered mult to your oscillator.  But even the buffered mults differ a bit!

I have made a few tables to match different scenarios:

1. You are plugged directly into a VCO
2. You are plugged directly into a 555
3. You are plugged directly into a uBUFFER.  The uBUFFER is attached to your oscillator(s).
4. You are plugged directly into a 4BUFFER.  The 4BUFFER is attached to your oscillator.
5. You are plugged directly into another GRAINS oscillator.  In this case, I suggest
   using the OUTPUT_UBUFFER #define below, setting TRANSPOSE_BITS on the GRAINS oscillator 
   to about -6, and tweaking the tracking via POT1.  It should work.
6. You are plugged directly into a 2OSC/d
7. You are plugged into a 2OSC: in this case, may your god have mercy on your soul.

You need to specify what you're plugged into, using a #define in the code.  The default is 555.



## Configuration

#### IN 1
Trigger Input 1
#### IN 2
Trigger Input 2
#### IN 3
Trigger Input 3
#### AUDIO IN (A)
Trigger Input 4
#### AUDIO OUT
Quantized Pitch CV Out
#### DIGITAL OUT (D) 
Trigger Out
#### POT 1
[Set to Maximum and Set switch to IN1]
#### POT 2
[Set to Maximum and Set switch to IN2]
#### POT 3
Scale/Chord
