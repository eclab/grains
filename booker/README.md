# Booker

Booker is Hammond Organ simulator complete with a Leslie. Booker is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.  booker was written by Sean Luke [sean@cs.gmu.edu]

Set your Grains to **MOZZI MODE**.  You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

Booker comes with a Leslie, which is on by default, but you can turn it off in the code or change its rate, volume adjustment, and pitch adjustment.

## Adjusting Tracking

Grains can only do about 3.75 octaves due to its circuit design.  I have positioned it to start at the C two octaves below middle C.  For the same reason, I have to "stretch" Grains from 3.75V to 5V, so to speak, in software.  I have tuned this stretch for my own Grains, but if Booker is tracking well in your Grains, get with me and we might be able to customize it for you.

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
#### Division OUT (D) 
[UNUSED]
#### POT 1
Choice of organ [Set the switch to MAN]
#### POT 2
Pitch Tune  [Set the switch to MAN]
#### POT 3
Volume

