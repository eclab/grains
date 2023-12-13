# SCOPE

Scope is a simple analog and digital oscilloscope using the Arduino IDE.   Scope is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)

You will need to install the Arduino IDE, but you already have done that, right?

Scope uses the Arduino IDE's Serial Plotter functionality.  Do the following:

1. Attach your GRAINS to your laptop
2. Select the USB Port
3. Call up the Serial Plotter (it's under the Tools menu)
4. Set the Baud Rate to 250000
5. Watch in wonder!

You change the rate of the oscilloscope by turning POT 3.  At 0, it's fast enough to display low VCO waves at audio rates.

Scope lets you hook up Inputs 1, 2, 3, and A as either analog or digital inputs, D as a digital input, and Out as a little digital function generator.

By default, Inputs 1 and 2 are set up as analog inputs.  You should set their switches to IN1 and IN2 respectively, and tune Pots 1 and 2 to approximately the 2 0'clock position so they properly run from min (0) at 0 V to max (1023) at 5V.

By default, Inputs 3 and A are set up as DIGITAL inputs, because they don't have tuning knobs and have internal resistors messing with things. You can change them to analog inputs, but beware that they will hit 1023 at less than 5V (approximately the 3 o'clock position on a knob from a 4ATT/Mix for example). You'll have to eyeball it.

D is always a digital input.

Out is always a digital function generator.  You can change what it outputs.  By default it outputs 0V.

### Analog Noise

GRAINS's analog inputs are noisy: so that noise you're seeing is the Arduino, not your LFO or whatnot.  Now you see what I have to deal with.  I could smooth them but it'd slow the scope down.  Will think about it.

### Changing the Inputs and Outputs in Real Time from Your Laptop

It's easy!  In the Serial Plotter, just type one of the numbers or letters below and hit RETURN:

    1       Toggles Input 1 between analog and digital
    2       Toggles Input 2 between analog and digital
    3       Toggles Input 3 between analog and digital
    a or A  Toggles Input A between analog and digital
    h or H  Sets Out to generate 5V ("H" for High)
    l or L  Sets Out to generate 0V ("L" for Low)
    s or S  Sets Out to generate a square wave.  No, you can't set the frequency.

### Changing the Baud Rate

The baud rate is by default set to 250000, the fastest rate that Serial Plotter operates on my laptop (a Macbook M1).  If you are not having luck with this rate, try something lower. An obvious choice would be 115200, a common baud rate for many machines.

## Configuration

#### IN 1
Analog or Digital Input (Analog by Default)
#### IN 2
Analog or Digital Input (Analog by Default)
#### IN 3
Analog or Digital Input (Digital by Default)
#### AUDIO IN (A)
Analog or Digital Input (Digital by Default)
#### AUDIO OUT
Function Generator, outputs 0V by default
#### DIGITAL OUT (D) 
Digital Input
#### POT 1
Trim for In1 Analog Input
#### POT 2
Trim for In2 Analog Input
#### POT 3
Update Rate


