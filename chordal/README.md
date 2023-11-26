# Chordal

Chordal is a chording oscillator.  Evolution is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

Chordal can play chords of your choice, using a mixture of either square waves and sine waves, or sawtooth waves and sine waves, or triangle waves and sine waves.  (It can't be a mixture of, say, square and saw because GRAINS doesn't have enough memory.  It has to be sine.).   You select sawtooth, square, or triangle as a #define in the code. The default is square.

You can set the pitch CV and tune, the amount of square/saw/tri vs sine wave, and the chord choice.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

NOTE: Probably due to the voltage divider on all of its analog inputs, GRAINS is limited to about a 45 note range.  

## Mixing

A single note has 256 levels of mixing resolution.  Intervals have 128.  Triads and Sevenths etc. have 64.  This is a tradeoff I had to make to keep the sound quality reasonably high.  Sorry.

## Adjusting Tracking

Grains can only do about 3.75 octaves due to its circuit design.  I have positioned it to start at the C two octaves below middle C.  For the same reason, I have to "stretch" Grains from 3.75V to 5V, so to speak, in software.  I have tuned this stretch for my own Grains, but if Evolution is tracking poorly in your Grains, get with me and we might be able to customize it for you.

In short, you can play with the function that has "pow(...)" in it below to get the tracking you need.  65.41 is the frequency two octaves below middle C.  45.2 is the stretch factor appropriate for my grains -- higher is more stretched.  60 is a full 5V, 5-octave (60 note) stretch.  You might need to stretch or decrease by just a tad, like .1 or .2.  I don't know how consistent Grains' internal resistors are from unit to unit.

Once you have a desired base frequency and stretch value, you could pump out all the values from 0 to 1023 and put them into the "pitch[]" table.  I use some lisp code as below but you can choose whatever language suits your fancy.  :-)  Then use the FREQUENCIES macro instead of the pow() function, it's significantly faster.

## Chords

The chords are, in order:

- No Chord [just the note]
- Intervals:
  - minor 3
  - Major 3
  - 4
  - 5
  - minor 6
  - Major 6
  - minor 7
  - Octave
  - Octave + minor 3
  - Octave + Major 3
  - Octave + 5
- Triads:
  - minor
  - minor first inversion
  - minor second inversion
  - Major
  - Major first inversion
  - Major second inversion
- Sevenths and Octave Triads
  - 7
  - minor 7
  - Major 7
  - diminished 7
  - minor + Octave
  - Major + Octave

## Configuration

#### IN 1
Mix CV [Low: all Sine, High: all Square/Saw/Tri]
#### IN 2
[UNUSED]
#### IN 3
Pitch CV 
#### AUDIO IN (A)
[UNUSED]
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
[UNUSED]
#### POT 1
Mix [Low: all Sine, high: all Square/Saw/Tri]

[If you're not using MixCV, set the switch to MAN]
#### POT 2
Pitch Tune  [Set the switch to MAN]
#### POT 3
Chord
