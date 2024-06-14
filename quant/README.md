# Quant

Quant is CV quantizer with a variety of scales.  Quant is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

Quant is pretty simple.  You provide a CV on IN 1, and specify the scale you would like to quantize to. The pitch is quantized to AUDIO OUT.

## Grains Output Warning
This program outputs note CV information from GRAINS.  If you feed this into (for example) a VCO, 2OSC/d, or 2OSC, it will not be proper 1V/oct because they pull too much amperage and this causes the GRAINS to output incorrect voltages.  Even feeding into another GRAINS will cause the pitch to drop a bit.  You can fix this by feeding into a buffered mult first.  The 555 does not have this issue (its inputs are buffered).

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

## Scales

Scales come in three categories.  You provide the category via POT 2, and then the scale in the category via POT 3.  There are three categories and 10 scales per category.

### Category 1: Chromatic and 7-Tone Scales
Major

Harmonic Minor

Melodic Minor

Dorian

Phrygian

Lydian

Mixolydian

Aeolian (Relative Minor)

Locrian

Chromatic

### Category 2: Other Scales
Blues Minor

Pentatonic

Minor Pentatonic

Japanese Pentatonic

Whole Tone

Hungarian Gypsy

Phrygian Dominant

Persian

Diminished (Octatonic)

Augmented (Hexatonic)

### Category 3: Chords
Octave

5th + Octave 

Major Triad

Minor Triad

Augmented Triad

Dominant 7

Major 7

Minor 7

Diminished 7

Minor-Major 7


## Adjusting Tuning and Tracking

Grains's Inputs track 1.3V/octave, not 1V/octave: we'll need to scale them to track properly.  
To do this, you can adjust the Pitch CV Scaling on Pot 1.  This GRAINS program is set up to 
associate 0V with note 0, and 5V with note 60 (of course).  However you need to get the tracking
(scaling) properly trimmed or that won't happen.  To do this, the easiest thing to do is to set the
scale to OCTAVES (Pot 2 fully right, Pot 3 fully left), play notes, and adjust the tracking to get
the octaves to scale properly.
Note that as GRAINS resistors warm up, the scaling will change and you will need to adjust the 
tracking again, at least until they are fully warmed up.

## Configuration

#### IN 1
Pitch In
#### IN 2
[UNUSED]
#### IN 3
[UNUSED]
#### AUDIO IN (A)
[UNUSED]
#### AUDIO OUT
Quantized Pitch Out
#### DIGITAL OUT (D) 
[UNUSED]
#### POT 1
Pitch In CV Tracking Adjust

[Set the switch to IN1]
#### POT 2
Scale Category

[Set the switch to MAN]
#### POT 3
Scale
