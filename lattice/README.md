# Lattice

Lattice is a "Light" version of [Mutable Instruments's Grids](https://www.google.com/search?client=firefox-b-d&q=Mutable+Instruments+Grids) drum pattern generator.  It is designed to run on the Tangible Waves AE-Modular GRAINS module, though you could modify it to run on most any arduino. Lattice was written by Sean Luke [sean@cs.gmu.edu]

Unlike other code in this repository, Lattice is licensed under GPL.

Set your Grains to **GRAINS MODE**.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)

It implements much of Mutable Instruments's Grids algorithm except for swing, accents, and internal tempo.  It is not nearly as convenient as [TOPOGRAF](https://www.tangiblewaves.com/store/p76/TOPOGRAF.html) because Grains does not have as many knobs and sockets exposed, but it can do quite a lot and has a variety of configuration options.

Lattice has three configurations:

- A SIMPLE configuration
- A SIMPLE "CHAOS" configuration
- A COMPLEX configuration.  

## SIMPLE Configuration
This is the default configuration

#### IN 1
Clock In, plus X CV
#### IN 2
Y CV
#### IN 3
Drum 3 Trigger Out    [yes, it's changed to an OUT]
#### AUDIO IN (A)
Reset In
#### AUDIO OUT
Drum 1 Trigger Out
#### DIGITAL OUT (D) 
Drum 2 Trigger Out
#### POT 1
(Always set to "In 1")
X
If you turn it to zero, clock is paused (ignored).  Otherwise, the range is from a little past zero (roughly 9 o'clock) to about 2 o'clock [due to GRAINS bug, see discussion below]
#### POT 2
(Set to "In 2" if using In 2, else "Man")
Y
The range is from a little past zero (roughly 9 o'clock) to about 2 o'clock if set to "In 2" [due to GRAINS bug, see discussion below], or to fully right if set to "Man"
#### POT 3
Complexity for ALL THREE DRUMS

If you set POT 1 to less than 1/8, it will just output zero.  If you turn POT 1 fully left, it will also turn the clock off.

IN 2 is an alternative CV in for POT 2, and if you set Pot 2 to "In 2", Pot 2 will temper the CV, else if you set it to "Man", it will just use Pot 2 (no CV).

IN 1 is **BOTH** a CV for POT 1 **AND** a CLOCK source.  You should ALWAYS set Pot 1 to "In 2", **not** to "Man".  If you don't care about the CV, just provide a clock signal and you're done.  If you care about the CV, the CV is simply the voltage of the clock.  You can do this by putting a 5V CLOCK signal into the INPUT of a 2VCA module set to **DC**.  Put the CV signal into the CV of the 2VCA module.  The output of the 2VCA is the two multiplied, so take that and feed it into IN 1 on your GRAINS.

### CHAOS

The Chaos is fixed to 0 (no randomness), but you can change it by changing a #define in the code.  You can set it to any value from 0 to 255 (maximum chaos).  Do not exceed those values.

### USING DRUMKIT010
Be sure to set DrumKit1010's TRIG MODE switch to "D" (Digital), so it properly responds to triggers.

### GRAINS BUG.  

If you set Pot P1 or P2 to "Man" then these pot have full range.  But if you set them to "In1 1" or "In 2", then the maximum possible value is no longer at full right, but rather is at about the 2 o'clock position: the range is cut down by 1/3.  I think it is an error in the GRAINS pot circuitry.  This is a particular problem for us because P1 **must** be set to "In 1" due to the incoming clock.  Additionally, we take out about 1/8 at the other end to handle the clock not being on, so the total range of these pots, at "In 1" or "In 2", is about 1/2.  :-( 




## SIMPLE "CHAOS" Configuration

This is turned on by modifying a #define in the code.

In this configuration, POT 3 does not set complexity: instead it sets the degree of chaos.

The complexity for all three drums is set to 127, but you can change them by changing three #defines in the code.  The values you can set are between 0 and 255 -- do not go outside these ranges.


## COMPLEX CONFIGURATION

This is turned on by modifying a #define in the code.

The Complex configuration changes POT 3 into an OPTION pot for changing the meaning of IN 1 and IN 2.  

The OPTION potentiometer can be set to three values: WEST, NORTH, and EAST.  When turned WEST, then Pots/Inputs 1 and 2 control the X and Y dimensions of the grid as usual.  When turned NORTH, the Pots/Inputs 1 and 2 control the Drum 1 and Drum 2 complexity.  Finally, when turned EAST, the Pots/Inputs 1 and 2 control the Drum 3 complexity and Chaos.

This gives you full access to customizing the complexity and chaos of the three drums.  However, as you move the option dial from option to option, you'll have to readjust the potentiometers.  For example, if the dial is at WEST and you have set up X and Y parameters, then you turn the dial to NORTH, the Drum 1 Complexity and Drum 2 Complexity parameters will suddenly get set to those values!  Okay so you change them back to the values you want.  Then you turn the dial to EAST, an the Drum 3 Complexity and Chaos parameters will suddenly get set to the new values again!  This will get tiring quickly.

In complex mode, Lattice **saves the settings you had made to EEPROM.**  These are stored in the first five bytes of the EEPROM.  Initially the EEPROM has 255 for these values, the first time you'll initially get crazy results until you set them manually.  You'll also get crazy results if you use any other program which saves to EEPROM, possibly overwriting your values.  Just so you knew.

