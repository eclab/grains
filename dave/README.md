# Dave (Epiphany ALPHA Release)

## Note about the ALPHA Release

Dave runs in several modes.  It works pretty well in USB\_ROUTER\_MODE, USB\_MPE\_MODE, and USB\_DISTRIBUTOR\_MODE.  DELEGATO also works as a feature, and so do the POT CCs.  I have not tested the CLOCK modes yet.

What does NOT work are the various BREAKOUT modes, because DAVE simply isn't fast enough to pump out more than one serial port when getting serious MIDI as input, resulting in stuck or hung notes.  This is unfortunate because it means you can't use Dave to do polyphony without the MB/1.   Also the POT CCs can put stress on MIDI as discussed below.

## About Dave

Dave is a MIDI router and utility which can do a variety of things.  Dave is meant to run 
on the AE Modular GRAINS, but it could be adapted to any Arduino.

DAVE REQUIRES that you install the NeoHWSerial library from the Arduino library manager.  It has
its own customized (and bug-fixed) copy of the NeoSWSerial library, so do not install
that one (and in fact you may need to uninstall it).

Dave runs in a variety of modes.  Many of these modes are meant to receive MIDI
over USB and route it to WonkyStuff modules or to the MASTER I/O.  The GRAINS USB 
port cannot present itself as a MIDI Device, so Dave works with a Java program, 
also called Dave, which receives MIDI from your DAW or controller and sends it over 
the USB port to the GRAINS.  This program is located in the "java" subdirectory.

You may have noticed that neither the MB/1 nor MASTER I/O have a WonkyStuff-style MIDI In port.
They just have MIDI TRS-B jacks.  However it is possible to send MIDI to them from the GRAINS
with some wiring.   See the file "docs/TRS.md" in the dave directory for more information.

DAVE works in one or more MODES and has a few configuration options beyond that.


### Other Stuff

Dave also has a facility to convert .mid files to .h files that you can cut and paste into GRAINS Beats.


## The Modes

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


### What Doesn't Work Yet (or At All)
There are a few other modes which have not been tested yet or are not working properly due
to speed problems with GRAINS's serial ports:

- USB DISTRIBUTOR BREAKOUT MODE.  This is like USB Distributor Mode but routes Channel 2
  out DIGITAL OUT, routes Channel 3 out AUDIO IN, and Routes Channel 4 out IN 3.
  Unfortunately IN 3 is not reliable as a MIDI transmitter, and two output serial ports
  (DIGITAL OUT, AUDIO IN) are too slow, sometimes resulting in stuck notes.

- USB MPE BREAKOUT MODE.  This is like USB MPE Mode but routes Channel 2
  out DIGITAL OUT, routes Channel 3 out AUDIO IN, and Routes Channel 4 out IN 3.
  Unfortunately IN 3 is not reliable as a MIDI transmitter, and two output serial ports
  (DIGITAL OUT, AUDIO IN) are too slow, sometimes resulting in stuck notes.

- INTERNAL CLOCK MODE.  This is similar to USB Clock Mode but takes MIDI Input from DIGITAL IN
  rather than USB.  It's not been tested yet.


## About the Pot CCs

In modes other than the CLOCK modes, you can change up to three CC parameters with the pots.
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
