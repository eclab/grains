# Booker

Booker is Hammond Organ simulator complete with a Leslie. Booker is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.  Booker was written by Sean Luke [sean@cs.gmu.edu]

Set your Grains to **MOZZI MODE**.  You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

Booker comes with a Leslie, which is on by default, but you can turn it off in the code or change its rate, volume adjustment, and pitch adjustment.

## Adjusting Tuning and Tracking

Grains's Inputs track 1.3V/octave, not 1V/octave: we'll need to scale them to track properly.  To do this, you can adjust the Pitch CV Scaling on Pot 1.  This GRAINS program is set up to play the C three octaves below Middle C when it receives 0V.  You should be able to use Pot 1 to scale the pitch such that high Cs play in tune as well.  Once you have things tracking well, you can then use the Pitch Tune (Audio In) to tune 0V to some other note.  Note that as GRAINS resistors warm up, the scaling will change and you will need to adjust the tracking again, at least until they are fully warmed up.

By default the note corresponding to 0V is C0, three octaves below middle C, that is MIDI note 24, or 32.7 Hz.  You can customize the tuning for this Grains program but only UP.  This can be done in two ways.  First, you can add pitch to the tuning with a CV value to Audio In.  Second, you can transpose the pitch up by changing the TRANSPOSE_OCTAVES and/or TRANSPOSE\_SEMITONES #defines in the code to positive integers.  You can also change TRANSPOSE\_BITS: a "bit" is the minimum possible change Grains can do, equal to 1/17 of a semitone.

## Volume

Some drawbar settings are quiet and can get overwhelmed by noise, others are loud and can distort.  You can compensate with the volume knob.


## Drawbar Settings

Booker also has 16 selections of drawbar settings, but in the code it comes with 98 total: just swap whatever you'd like in, or customize it as you see fit.  The default settings are, in order:

    885324588 Blues 
    888800000 Booker T. Jones 1 
    888630000 Booker T. Jones 2 
    878000456 Bright Comping 
    843000000 Dark Comping 
    888888888 Full Organ 
    808808008 Gospel 1 
    888000008 Gospel 2 
    868666568 Greg Allman 1 
    888600000 Greg Allman 2 
    886800300 Paul Shaffer 1 
    888768888 Paul Shaffer 2 
    888878678 Paul Shaffer 3 
    808000008 Reggae 
    080000000 Sweet
    876543211 Strings 
  

## Configuration

#### IN 1
Pitch CV
#### IN 2
Volume CV
#### IN 3
[UNUSED]
#### AUDIO IN (A)
Pitch Tune
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
[UNUSED]
#### POT 1
Pitch Tune  [Set the switch to IN1]
#### POT 2
Volume
#### POT 3
Choice of organ [Set the switch to MAN]

