# AKWF

AKWF is a wavetable synthesizer with waves available from the [Adventure Kid Waveform](https://www.adventurekid.se/akrt/waveforms/) (AKWF) collection. AKWF is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

AKWF can have 2, 3, or 5 waveforms evenly distributed in its wavetable.  You can modulate between them and set the pitch CV and tune.

There is only one wave for each waveform available (there's no resampling for higher frequences).  This means that you should expect considerable aliasing.

You will need to install the Mozzi Library.  You can do this from the Library Manager
in your Arduino IDE.

NOTE: Probably due to the voltage divider on all of its analog inputs, GRAINS is limited
to about a 45 note range.  

## Unpacking

You need to unzip the akwf.zip file.  It's 4MB but unzips to 20MB.

## Specifying the Waveforms

There are 4162 AKWF single-cycle waveforms provided, including the new Nintendo waveform collection.  AdventureKid's website is here:  https://www.adventurekid.se/akrt/waveforms/

You specify the five waveform files by filename in the code, as well as the number of waveforms used.

## Configuration

#### IN 1
Position in Wavetable
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
Position in Wavetable

[If you're not using Position CV, set the switch to MAN]
#### POT 2
[UNUSED]
#### POT 3
Pitch Tune  [Set the switch to MAN]
