# AKWF

AKWF is a wavetable synthesizer with waves available from the [Adventure Kid Waveform](https://www.adventurekid.se/akrt/waveforms/) (AKWF) collection. AKWF is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

AKWF can have 2, 3, or 5 waveforms evenly distributed in its wavetable.  You can modulate between them and set the pitch CV and tune.  There is also a low-pass filter available.

There is only one wave for each waveform available (there's no resampling for higher frequences).  This means that you should expect considerable aliasing.

You will need to install the Mozzi Library.  You can do this from the Library Manager
in your Arduino IDE.

## Unpacking

You need to unzip the akwf.zip file.  It's 4MB but unzips to 20MB.

## Specifying the Waveforms

There are 4162 AKWF single-cycle waveforms provided, including the new Nintendo waveform collection.  AdventureKid's website is here:  https://www.adventurekid.se/akrt/waveforms/

You specify the five waveform files by filename in the code, as well as the number of waveforms used.

## Adjusting Tuning and Tracking

Grains's Inputs track 1.3V/octave, not 1V/octave: we'll need to scale them to track properly.  To do this, you can adjust the Pitch CV Scaling on Pot 1.  This GRAINS program is set up to play the C three octaves below Middle C when it receives 0V.  You should be able to use Pot 1 to scale the pitch such that high Cs play in tune as well.  Once you have things tracking well, you can then use the Pitch Tune (Audio In) to tune 0V to some other note.  Note that as GRAINS resistors warm up, the scaling will change and you will need to adjust the tracking again, at least until they are fully warmed up.

By default the note corresponding to 0V is C0, three octaves below middle C, that is MIDI note 24, or 32.7 Hz.  You can customize the tuning for this Grains program but only UP.  This can be done in two ways.  First, you can add pitch to the tuning with a CV value to Audio In.  Second, you can transpose the pitch up by changing the TRANSPOSE\_OCTAVES and/or TRANSPOSE\_SEMITONES #defines in the code to positive integers.  You can also change TRANSPOSE\_BITS: a "bit" is the minimum possible change Grains can do, equal to 1/17 of a semitone.

## Configuration

#### IN 1
Pitch CV
#### IN 2
Position in Wavetable CV
#### IN 3
[Unused] 
#### AUDIO IN (A)
Pitch Tuning
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
[UNUSED]
#### POT 1
Pitch

[If you're not using Position in Wavetable CV, set the switch to MAN]
#### POT 2
Position in Wavetable

[If you're not using Pitch CV, set the switch to MAN]
#### POT 3
Filter Cutoff
