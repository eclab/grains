# Booker-M

Booker is MIDI-CONTROLLED Hammond Organ simulator complete with a Leslie.  It's the MIDI version of Booker.  Booker-m is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.  Booker-M was written by Sean Luke [sean@cs.gmu.edu]

Set your Grains to **MOZZI MODE**.  You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

Booker-M comes with a Leslie, which is on by default, but you can turn it off in the code or change its rate, volume adjustment, and pitch adjustment.

## Leslie

The Leslie's volume can be adjusted with a knob (POT  2).  The Leslie's pitch variance can also be adjusted, but with a #define in the code.  You can also change its rate similarly.


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
  

## MIDI Response

- Note On/Off
    - Pitch: All MIDI notes C0 through G10, but realistically C0 through B8 
    - Velocity: 0...127 for Note On.  Velocity 0 is a Note Off

- CC
    - All Notes Off (CC 123) Resets all notes, lowers Gate
    - All Sounds Off (CC 120) Resets all notes, lowers Gate

## Configuration

#### IN 1
Overall Volume CV
#### IN 2
Leslie Volume CV
#### IN 3
Gate Out
#### AUDIO IN (A)
[Unused]
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
MIDI In
#### POT 1
Overall Volume
#### POT 2
Leslie Volume CV
#### POT 3
Choice of organ / drawbars

