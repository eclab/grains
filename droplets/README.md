# Droplets

Droplets produces a tinkly chord of sine waves, like a wind chime. Droplets is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

The tinkling is done by choosing a Droplet chord from POT 3.  You can then specify the degree of tinkle (release) with POT 2.  Then every time you trigger Droplets it will issue one tinkle.  

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.


CHOICE OF DROPLET CHORD

You current choices are (in order left to right): 

- Maj
- min
- Maj7
- min7
- 7
- dim7
- Aug7
- Pentatonic
- Whole Tone
- Chromatic
- 5+Oct
- Oct

Except for Whole Tone and Chromatic, most droplet chords range 3 or 4 octaves, with an emphasis on the fundamental.


RELEASE AND VOLUME

One potentiometer controls both the length of release of the individual tinkles and their volume. Left to right, you have three broad regions: low volume, medium volume, and high volume. Within a region, you have five levels of release from fast release to long release.  Why would you want lower volume?  Because longer release tinkles, depending on the chord, will cause the sine waves to overlap and add up to the point that they cause clipping.


NOISE

Droplets are pretty hissy.  Most of the problem is due to Mozzi I'm afraid.


ADJUSTING TUNING AND TRACKING

Grains's Inputs track 1.3V/octave, not 1V/octave: we'll need to scale them to track properly. To do this, you can adjust the Pitch CV Scaling on Pot 1.  This GRAINS program is set up to play the C two octaves below Middle C when it receives 0V.  You should be able to use Pot 1 to scale the pitch such that high Cs play in tune as well.  Once you have things tracking well, you can then use the Pitch Tune (Audio In) to tune 0V to some other note.  Note that as GRAINS resistors warm up, the scaling will change and you will need to adjust the tracking again, at least until they are fully warmed up.

By default the note corresponding to 0V is C0, three octaves below middle C, that is MIDI note 24, or 32.7 Hz.  You can customize the tuning for this Grains program but only UP.  This can be done in two ways.  First, you can add pitch to the tuning with a CV value to Audio In.  Second, you can transpose the pitch up by changing the TRANSPOSE_OCTAVES and/or TRANSPOSE_SEMITONES #defines in the code to positive integers.

I recognize that Droplets will be hard to tune because of the tinkles.  By far the easiest to tune with is OCTAVES, which is the very last droplet option (turn the droplet knob fully to the right).

CONFIGURATION

IN 1            Pitch CV
IN 2            Release and Volume CV
IN 3            Trigger
AUDIO IN (A)    Pitch Tune
AUDIO OUT       Out
DIGITAL OUT (D) [Unused]

POT 1           Pitch Scaling	[Set the switch to In1]

POT 2           Release and Volume

POT 3           Choice of Droplets


## Configuration

#### IN 1
Pitch CV
#### IN 2
Release and Volume CV
#### IN 3
Trigger
#### AUDIO IN (A)
Pitch Tune
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
[UNUSED]
#### POT 1
Pitch CV Scaling
#### POT 2
Release and Volume

[If you're not using Release and Volume CV, set the switch to MAN]
#### POT 3
Choice of Droplets
