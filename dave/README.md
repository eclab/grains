# Dave

Dave is a MIDI router and utility which can do a variety of things.  Dave is meant to run 
on the AE Modular GRAINS, but it could be adapted to any Arduino.

Set your GRAINS to GRAINS Mode.  Dave does not work in Mozzi mode.

DAVE REQUIRES that you install the NeoHWSerial library from the Arduino library manager.  It has
its own customized (and bug-fixed) copy of the NeoSWSerial library, so do not install
that one (and in fact you may need to uninstall it).

## Dave's Java Program
Dave runs in a variety of modes.  Many of these modes are meant to receive MIDI
over USB and route it to WonkyStuff modules or to the MASTER I/O.  The GRAINS USB 
port cannot present itself as a MIDI Device, so [Dave works with a Java program, 
also called Dave,](https://github.com/eclab/grains/tree/main/dave/java/) which receives MIDI from your DAW or controller and sends it over 
the USB port to the GRAINS.  This program is located in the "java" subdirectory.

The general hookup to a controller as follows: 

- DAVE on GRAINS <-USB-> Dave Java Program <----> Controller Device

The hookup to a typical DAW is:  

- DAVE <-USB-> Dave Java Program <---> MIDI Loopback Device <---> DAW
  (Though Logic on the Mac doesn't need a Loopback.  Ableton does)

See the Dave java program [README file](https://github.com/eclab/grains/dave/java/README.md) for more information.

## Hacking the TRS Jacks 

You may have noticed that neither the MB/1 nor MASTER I/O have a WonkyStuff-style MIDI In port.
They just have MIDI TRS-B jacks.  However it is possible to send MIDI to them from the GRAINS
with some wiring.   See [docs/TRS.md](docs/TRS.md) for more information.
You should absolutely know what you're doing here: this is not standard use of AE modular, 
and I will not be held responsible if you fry a your MB/1 or MASTER I/O MIDI optoisolator.



## Dave's Modes

DAVE works in one or more MODES and has a few configuration options beyond that.

Here are the basic modes:

### USB CLOCK MODE
This mode takes MIDI input over USB and outputs clock signals.

- DIGITAL OUT outputs a GATE: it goes high when receiving a START or CONTINUE,
  and goes low when receiving a STOP
- AUDIO OUT outputs CLOCK in-between a MIDI START/CONTINUE and STOP.
- IN 3 outputs a trigger (RESET) when it receives START or CONTINUE
- AUDIO IN outputs a trigger when it receives a STOP
- The Pots should NOT be set to output anything


### USB ROUTER MODE
This mode takes MIDI input over USB and outputs directly to DIGITAL OUT. This can be useful
to send MIDI data directly to a one Wonkystuff MCO/1, MCC/4, MTR/8, or MDIV.  With careful
wiring you could also feed into a MASTER I/O with some caveats and disclaimers.

- DIGITAL OUT outputs MIDI from USB
- AUDIO OUT outputs CLOCK in-between a MIDI START/CONTINUE and STOP.
- IN 3 outputs a trigger (RESET) when it receives START or CONTINUE
- AUDIO IN outputs a trigger when it receives a STOP
- The Pots *can* be set to output CCs of your choice.


### USB DISTRIBUTOR MODE
This mode takes MIDI input over USB and outputs to DIGITAL OUT, but redistributes Channel 1.
to several other channels For example, the first note goes to channel 2, the second note goes
to channel 3, and so on.  The number of voices (up to 15) is set by the #define NUM_VOICES below.  
This is useful in combination when fed into with a WonkyStuff MB/1, with careful wiring:
it would allow you to have a polyphonic synthesizer controlled by a normal
MIDI Controller.

- DIGITAL OUT outputs MIDI from USB, with channels modified as described
- AUDIO OUT outputs CLOCK in-between a MIDI START/CONTINUE and STOP.
- IN 3 outputs a trigger (RESET) when it receives START or CONTINUE
- AUDIO IN outputs a trigger when it receives a STOP
- The Pots *can* be set to output CCs of your choice.


### USB MPE MODE
This mode takes MIDI input over USB and and outputs directly to DIGITAL OUT.  It assumes that we
are doing MPE with a ZONE LOW, where Channel 1 is the MPE Master channel, and the individual
channels start at 2.  The number of voices (hence channels except the Master channel) is set by the
#define NUM_VOICES below.  This is useful in combination when fed into with a WonkyStuff MB/1, 
with careful wiring to create an MPE-controlled polysynth.

- DIGITAL OUT outputs MIDI from USB, with channels modified as described
- AUDIO OUT outputs CLOCK in-between a MIDI START/CONTINUE and STOP.
- IN 3 outputs a trigger (RESET) when it receives START or CONTINUE
- AUDIO IN outputs a trigger when it receives a STOP
- The Pots *can* be set to output CCs of your choice.


### USB TRIGGERS MODE
This takes four different MIDI notes from USB and generates drum triggers, similar to a WonkyStuff
 MTR/8.  The notes are Middle C (60), D (62), E (64), and F (65), but you can change
them if you needed to for some reason by modifying the "triggerNotes" variable below: it
holds the MIDI pitch values for each of the four notes.

- AUDIO OUT outputs trigger 1 (Middle C)
- IN 3 outputs trigger 2 (D)
- AUDIO IN outputs trigger 3 (E)
- DIGITAL OUT outputs trigger 4 (F)
- The Pots do not do anything (if you can think of something useful they could do, let me know).


### INTERNAL TRIGGERS MODE
This is basically the same thing as USB TRIGGERS MODE except that the MIDI input is not
via USB but via a local MIDI socket, such as from a Wonkystuff MB/1.  There are only three
trigger notes: Middle C (60), D (62), and E (64), but you can change
them if you needed to for some reason by modifying the *first three* values in the 
"triggerNotes" variable below: it holds the MIDI pitch values for each of the three/four
notes used by the INTERNAL TRIGGERS and by USB TRIGGERS modes.

- AUDIO OUT outputs trigger 1 (Middle C)
- IN 3 outputs trigger 2 (D)
- AUDIO IN outputs trigger 3 (E)
- DIGITAL OUT takes MIDI IN
- The Pots do not do anything (if you can think of something useful they could do, let me know).


### NOTE GENERATOR MODE. 
This only produces MIDI: you could use it to trigger a WonkyStuff MCO/4
from a regular setup without USB.  It takes a pitch CV and a gate in and generates a note 
for the MCO/4, and it can also still produce the THREE output CCs, but one of them is now 
from a CV in.

- DIGITAL OUT outputs MIDI from USB, with the note data being generated
- IN 1 receives the pitch.  Set POT 1 to MAN, or else adjust POT 1 to scale things 
  (probably around 2'oclock) to 1V/oct
- AUDIO IN receives the gate
- Pots 2 and 3 can be set to output CCs of your choice.  
- IN 3 can be set to output the CC normally reserved for POT 1.  It will not output any CC value
  less than 8, as this likely indicates that IN 3 is disconnected.


### HEX MODE  
This mode takes hexadecimal input over USB from your Arduino IDE Serial Monitor and outputs 
MIDI bytes over DIGITAL OUT.  It's only meant for debugging.  Don't bother with it otherwise.

- DIGITAL OUT outputs MIDI from USB originally as hexadecimal input
- Nothing else does anything
- You can't De-Legato or do Clock Division



## About the Pot CCs

In modes other than the CLOCK modes or Hex mode, you can change up to three CC parameters with the pots.
This is not available in the USB\_CLOCK or INTERNAL\_CLOCK modes.

WARNING: if you play or release notes immediately after or during changing the pots, DAVE
may not see these notes, possibly resulting in stuck or oddly behaving note states.  This is
because a stream of pots overwhelms the MIDI output, and so DAVE may not be able to return to
the incoming notes in time.  So don't do that.  Play/release notes, or change the pots, but not
both at the same time.

You can also customize which channels will receive CCs from the pots


## De-Legato

This sends a NOTE OFF before any NOTE ON, to guarantee that we do a gate prior to changing the pitch.
The purpose of De-Legato is to work around the fact that WonkyStuff's MCO/1 module is
legato-only.  HOWEVER De-Legato will trigger a MIDI bug in the MASTER I/O: if it receives two note-offs
in a row for the same note, its gate will hang permanently at HIGH.  So if you're sending signals to
the MASTER I/O, you need to turn off De-Legato.

De-Legato is specified on a per-channel basis.  By default, it's ON (1) for ALL OUTGOING CHANNELS.  
But you can change it.   You an also turn De-Legato on or off globally.

## Clock Division

You can change the MIDI clock divisor.  By default the divisor is 1, meaning 24 PPQ.
If you made this 6, for example, you'd get 4PPQ, otherwise known as one pulse every 16th note.
If you changed this to 24, you'd get one pulse every quarter note.  The FIRST pulse is at the
very first clock pulse after a START or a CONTINUE. 

## Other Stuff

Dave also has a facility to convert .mid files to .h files that you can cut and paste into GRAINS Beats.


