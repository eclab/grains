# Chordal

Chordal is a chording oscillator.  Chordal is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

Chordal can play chords of your choice, using a mixture of either square waves and sine waves, or sawtooth waves and sine waves, or triangle waves and sine waves.  (It can't be a mixture of, say, square and saw because GRAINS doesn't have enough memory.  It has to be sine.).   You select sawtooth, square, or triangle as a #define in the code. The default is square.

You can set the pitch CV and tune, the amount of square/saw/tri vs sine wave, and the chord choice.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.


## Mixing

A single note has 256 levels of mixing resolution.  Intervals have 128.  Triads and Sevenths etc. have 64.  This is a tradeoff I had to make to keep the sound quality reasonably high.  Sorry.

## Adjusting Tuning and Tracking

Grains's Inputs track 1.3V/octave, not 1V/octave: we'll need to scale them to track properly.  To do this, you can adjust the Pitch CV Scaling on Pot 1.  This GRAINS program is set up to play the C three octaves below Middle C when it receives 0V.  You should be able to use Pot 1 to scale the pitch such that high Cs play in tune as well.  Once you have things tracking well, you can then use the Pitch Tune (Audio In) to tune 0V to some other note.  Note that as GRAINS resistors warm up, the scaling will change and you will need to adjust the tracking again, at least until they are fully warmed up.

By default the note corresponding to 0V is C0, three octaves below middle C, that is MIDI note 24, or 32.7 Hz.  You can customize the tuning for this Grains program but only UP.  This can be done in two ways.  First, you can add pitch to the tuning with a CV value to Audio In.  Second, you can transpose the pitch up by changing the TRANSPOSE_OCTAVES and/or TRANSPOSE_SEMITONES #defines in the code to positive integers.

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
Pitch CV
#### IN 2
Mix CV [Low: all Sine, High: all Square/Saw/Tri]
#### IN 3
[Unused]
#### AUDIO IN (A)
Pitch CV Tune
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
[UNUSED]
#### POT 1
Pitch CV Scaling
#### POT 2
Mix [Low: all Sine, high: all Square/Saw/Tri]

[If you're not using MixCV, set the switch to MAN]
#### POT 3
Chord
