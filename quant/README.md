# Quant

Quant is CV quantizer with a variety of scales.  Quant is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

Quant is pretty simple.  You provide a CV on IN 1, and specify the scale you would like to quantize to. The pitch is quantized to AUDIO OUT.

## Range and Resolution

Mozzi's output is capable of a range of 42 notes. That's about 3.5 octaves.  This is also the quantizer's range: values above that will just get quantized to the top note. 

The range is a bit nonlinear.  I have a lookup table in QUANT which gets as close as I can to my own GRAINS but I do not yet know:

- Does it match other people's GRAINS?

- Does it change as the GRAINS warms up?  This is hard to nail down because I am driving VCOs, which also warm up...

- I'm trying to match my QUANTIZER.  But I'm not sure if it's properly tuned. :-(

I could use some help here: if I can't get it to match, it'll be hard to output notes from GRAINS for some other projects I have in mind.  Send me mail and let me know how it does on your unit.

Also note that GRAINS's resolution is only about 100 ticks per octave.  This means I can maybe get within 7 cents of a note but not nail it exactly.


## Scales

Scales come in three categories.  You provide the category via POT 1, and then the scale in the category via POT 2.  There are three categories and 10 scales per category.

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
Out
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
