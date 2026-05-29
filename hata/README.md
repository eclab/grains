# Hata

Hata is meant to run on the AE Modular GRAINS, but it could be adapted
to any Arduino.  Hata was written by Sean Luke [sean@cs.gmu.edu]

Hata converts four trigger inputs into MIDI notes for the Wonkystuff Playa.
This allows you to drive Playa from a TRIQ164 or other trigger emitter.
This performs essentially the same task as the Wonkystuff CVMx, only for
just four triggers, not eight.

You can also set POT 3 to do any ONE of the following:

- Reduce the Bit Depth on the Playa
- Reduce the Sample Rate on the Playa
- Change the Pitch on the Playa
- Set the note velocity (volume) on the Playa (the default is maximum)
- Select which four drum notes on the Playa are being played from the
  Hata, from 8 combo presets that you have created.  This is the default
  POT 3 parameter.

You specify the effect of POT 3 with various #defines in the code.  You can also change the 8 combo presets: they are a simple array in the code.

Hata uses SoftSerial to do MIDI because GRAINS does not expose its serial port.  SoftSerial is a bit flaky, but it works.  If you have a GRAINS V2 (note: I think at present I'm the only person in the world outside Tangible Waves which has one, so you probably don't have one) then you can use hardware serial, and there is an option to use that instead with a #define.  If you're using an ordinary Nano or Nano Every, it'd not be hard to set it up with hardware serial as well (see https://github.com/eclab/grains/tree/main/v2/info and let me know you're attempting it, and I can tweak things in the code for you).

## Note

Hata assumes that your playa has default learned MIDI settings for its notes and
CC parameters.  If you have changed them, you will need to change them back.  To do
this, you can long-press the Playa button, then send it a MIDI CC 32 (any value),
and also play a MIDDLE C (MIDI 60).




CONFIGURATION

IN 1            Trigger Input 1
IN 2            Trigger Input 2
IN 3            Trigger Input 3
AUDIO IN (A)    Trigger Input 4
AUDIO OUT       [UNUSED]
DIGITAL OUT (D) MIDI Out to Playa (on Grains)

POT 1           [UNUSED] Set to maximum value and turn on IN1
POT 2           [UNUSED] Set to maximum value and turn on IN2
POT 3           Sets parameter as described earlier.

Serial RX		MIDI Out to Playa (on Grains V2)        




## Configuration
This is the default configuration

#### IN 1
Trigger Input 1
#### IN 2
Trigger Input 2
#### IN 3
Trigger Input 3
#### AUDIO IN (A)
Trigger Input 4
#### AUDIO OUT
[Unused]
#### DIGITAL OUT (D) 
MIDI Out to the Playa (on Grains)
#### POT 1
[UNUSED] Set to maximum value and set switch to IN1
#### POT 2
[UNUSED] Set to maximum value and set switch to IN2
#### POT 3
Sets parameter as described earlier

#### (Serial RX)
MIDI Out to Playa (on Grains V2 only)        
