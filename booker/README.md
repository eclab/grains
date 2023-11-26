# Booker

Booker is Hammond Organ simulator complete with a Leslie. Booker is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.  Booker was written by Sean Luke [sean@cs.gmu.edu]

Set your Grains to **MOZZI MODE**.  You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

Booker comes with a Leslie, which is on by default, but you can turn it off in the code or change its rate, volume adjustment, and pitch adjustment.

## Adjusting Tracking

Grains's Inputs 1, 2, and 3 (and A) track at about 1.3V/octave, not 1V/octave.  The exact amount varies from unit to unit.  You may need to tweak things in the code to get it to track better.  It's just a matter of setting a single number, but you'll probably need to adjust it.  The code has instructions for doing this.  Likely once you've identified the number, you can use it in all my Grains projects.

## Volume

Some drawbar settings are quiet, others are loud.  You can compensate with the volume knob.


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
[UNUSED]
#### IN 2
[UNUSED]
#### IN 3
Pitch
#### AUDIO IN (A)
[UNUSED]
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
[UNUSED]
#### POT 1
Choice of organ [Set the switch to MAN]
#### POT 2
Pitch Tune  [Set the switch to MAN]
#### POT 3
Volume

