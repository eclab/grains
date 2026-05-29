# Para

Para is a MIDI-driven 3-voice Paraphonic oscillator with a mixed square/saw/triangle and sine waves.
Para is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

Para can select between square and sine, saw and sine, or triangle and sine.  You select
which one you want in the code.  The default is saw and sine.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager
in your Arduino IDE.

Para also sends a Gate via IN 3 whenever all of its notes have been released and one
new one has been started.  This means it won't send a gate if you release only one note,
then play a new one, while other notes are held down.  Various grooveboxes (such as the
Polyend Tracker) behave like this when playing all new notes suddenly, so you need to put
a small timestep between the previous chord's NOTE OFF and the new notes ON.

## Gate Mode

By default Para does not stop playing notes when it receives a NOTE OFF -- 
that is the responsibility of your envelope and VCA (it will send them a gate off).
However you can change this behavior in the code.

## Mono vs Para

Para has three voices.  Mono has only one.
However Mono has twice as many waves in its memory, so it will sound less aliased for many notes.

## Choosing Saw vs Square vs Triangle

The choice of wave is determined by changing a #define in the code.


## Gate Mode

By default Mono does not stop playing notes when it receives a NOTE OFF -- 
that is the responsibility of your envelope and VCA (it will send them a gate off).
However you can change this behavior by uncommenting a #define in the code.


## MIDI Response

- Note On/Off
    - Pitch: All MIDI notes C0 through G10, but realistically C0 through B8 
    - Velocity: 0...127 for Note On.  Velocity 0 is a Note Off

- CC
    - All Notes Off (CC 123) Resets all notes, lowers Gate
    - All Sounds Off (CC 120) Resets all notes, lowers Gate



## Configuration

#### IN 1
Saw/Square/Tri Vs Sine CV
#### IN 2
[Unused]
#### IN 3
Gate Out
#### AUDIO IN (A)
[Unused]
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
MIDI In
#### POT 1
Saw/Square/Tri Vs Sine
#### POT 2
[Unused]
#### POT 3
[Unused]
