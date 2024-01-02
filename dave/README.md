# Dave (New Year's Alpha Release)

## Note about the ALPHA Release

This code compiles, but I have so far only tested USB_ROUTER MODE with and without DELEGATO.  Also the pots emit CCs properly.  I have tested absolutely nothing else and likely nothing else works yet.  But I thought you'd like to see it so far.

## About Dave (once we get the rest of the stuff working)

Dave is a MIDI router and utility which can do many things.  Dave is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

Dave can receive MIDI locally in your synth or over USB.  The GRAINS USB port cannot present itself as a MIDI Device, so Dave works with a Java program, also called Dave, which receives MIDI from your DAW or controller and sends it over the USB port to the GRAINS.  The source code for this program is located in the "java" subdirectory, and binaries for MacOS, Windows, and Linux can be found in "java/install".

DAVE has a lot of configuration options.  However it also has six basic MODES, and you can just choose a mode and most of the configurations will be automatically set for you to make things simple.  Several of these modes are desiged to work with the WonkyStuff MB/1; others are designed to replace it in a limited fashion; and others work without it at all.

Let's start with the basics:

0. In order to connect DAVE to send MIDI to a Wonkystuff MB/1 or to a MASTER I/O, you'll need to connect the GRAINS MIDI output to the TRS sockets of the MB/1 or MASTER I/O.  To do this you need a Screw Terminal Block.  They're easy and cheap to buy.  I have [instructions for wiring them in docs/TRS.md](docs/TRS.md).  Hopefully future versions of the MB/1 and MASTER I/O will have MIDI IN sockets so this is not necessary.

1. DAVE has FOUR MIDI ports, called USB, PORT1, PORT2, and PORT3

   - USB
   - PORT1	(DIGITAL OUT)
   - PORT2	(IN 3)
   - PORT3	(AUDIO IN)

These are in fact the ONLY ports that GRAINS can send or receive MIDI on!  

2. DAVE receives MIDI either over USB or via PORT1.  It's impossible to receive
   MIDI over PORT2 or PORT3.

3. DAVE sends MIDI on any combination of PORT1, PORT2, and PORT3.  You can set
   the channel that each of these ports listens in on.  (Technically the GRAINS
   could also send MIDI out USB, but DAVE is not set up to do this).

4. DAVE always sends CLOCK signals on AUDIO OUT.  It only sends them between
   a MIDI START / CONTINUE and a MIDI STOP.

5. Depending on how it's set up, DAVE can also send RESET (START), STOP,
   or START/STOP GATE signals on PORT1, PORT2, and/or PORT3 as CV (not MIDI).

6. You can set each of the three pots to output a CC.  You can choose the CC
   parameter and the CC channel, and when you change a pot, it will INJECT a CC
   into the stream for PORT1, PORT2, and/or PORT3.  This allows you to change
   the MIDI parameters of the WonkyStuff MCO/1 for example.

7. DAVE is capable of various filtering facilities:

   - DAVE can do MPE (Low Zone).  It will treat Channel 1 as the Master Channel,
     and any data received on that channel will be copied to the other channels
     in the MPE Zone.

   - Dave can distribute notes from Channel 1 to other channels.  If you had 4
     other channels, Dave could redistribute notes from channel 1 to them round-robin,
     which would give you a 4-voice polysynth on Channel 1.

   - Dave can "De-Legato" notes: each time it receives a NOTE ON, it first sends
     a NOTE OFF to eliminate the previous note before sending the NOTE ON.  This
     counters an issue in the MCO/1, which only plays legato.

8. DAVE can send Debug messages back to its Java program via USB

9. At present DAVE can in many cases receive and forward Sysex messages no larger 
   than 32 bytes in length, not including the trailing F7.  You can increase this
   in the code.


## Modes
Here are the basic modes:

### USB Clock Mode
This mode takes MIDI input over USB and outputs clock signals. Specifically:
- AUDIO OUT outputs gated clock as usual
- DIGITAL OUT (PORT 1) outputs a GATE: it goes high when receiving a START or CONTINUE,
  and goes low when receiving a STOP
- IN 3 (PORT 2) outputs a trigger (RESET) when it receives START or CONTINUE
- AUDIO IN (PORT 3) outputs a trigger when it receives a STOP
- The Pots should NOT be set to output anything


### USB Router Mode
This mode takes MIDI input over USB and outputs directly to PORT 1. This can be useful
to send MIDI to the Wonkystuff MB/1, or as a simple way to send MIDI data directly to
a Wonkystuff MCO/1, MCC/4, MTR/8, or MDIV.  You could also use this to route MIDI data to
the MASTER I/O.  Additionally:
- AUDIO OUT outputs gated clock as usual
- IN 3 (PORT 2) outputs a trigger (RESET) when it receives START or CONTINUE
- AUDIO IN (PORT 3) outputs a trigger when it receives a STOP
- The Pots *can* be set to output CCs of your choice.


### USB Distributor Mode
This mode takes MIDI input over USB and redistributes Channel 1 to several other channels.
It only sends MIDI out PORT 1.
It's particularly useful to use in combination with a WonkyStuff MB/1, which would then
break out those channels to make a polysynth. Additionally:
- AUDIO OUT outputs gated clock as usual
- IN 3 (PORT 2) outputs a trigger (RESET) when it receives START or CONTINUE
- AUDIO IN (PORT 3) outputs a trigger when it receives a STOP
- The Pots *can* be set to output CCs of your choice.
- If there aren't any available channels for voices, voice stealing just steals channel 2

To use USB DISTRIBUTOR MODE you have to set highest CHANNEL NUMBER  Let's say that number
is N.  Then DAVE will redistribute MIDI on channel 1 to channels 2 ... N inclusive.  Thus 
if you wanted to use the MB/1 to set up a 4-voice polysynth, you'd set N = 5, and DAVE would
redistribute channel 1 to channels 2, 3, 4, and 5.  To set this highest channel number, you
make a change in the source code as shown.


### USB MPE Mode
This mode takes MIDI input over USB and and outputs directly to PORT 1.  It assumes that we
are doing MPE with a ZONE LOW, where Channel 1 is the MPE Master channel, and the individual
channels are again channels 2...FILTER_MAX_CHANNEL.  This would be useful to connect to the MB/1
to create an MPE-controlled polysynth.  Additionally:
- AUDIO OUT outputs gated clock as usual
- IN 3 (PORT 2) outputs a trigger (RESET) when it receives START or CONTINUE
- AUDIO IN (PORT 3) outputs a trigger when it receives a STOP
- The Pots *can* be set to output CCs of your choice.


### USB MPE MODE
This mode takes MIDI input over USB and and outputs directly to PORT 1.  It assumes that we
are doing MPE with a ZONE LOW, where Channel 1 is the MPE Master channel, and the individual
channels are again channels 2...FILTER_MAX_CHANNEL.  This would be useful to connect to the MB/1
to create an MPE-controlled polysynth.  Additionally:
- AUDIO OUT outputs gated clock as usual
- IN 3 (PORT 2) outputs a trigger (RESET) when it receives START or CONTINUE
- AUDIO IN (PORT 3) outputs a trigger when it receives a STOP
- The Pots *can* be set to output CCs of your choice.


### USB Distributor Breakout Mode
This mode is just like USB DISTRIBUTOR MODE except that it outputs channel 2 to PORT1,
channel 3 to PORT2, and channel 4 to PORT3.  This would be useful if you DIDN'T have
a Wonkystuff MB/1, and still wanted to have at least a 3-voice single-channel polysynth.
- AUDIO OUT outputs gated clock as usual
- The Pots *can* be set to output CCs of your choice.
- If there aren't any available channels for voices, voice stealing just steals channel 2


### USB MPE Mreakout Mode
This mode is just like USB MPE MODE except that it outputs channel 2 to PORT1,
channel 3 to PORT2, and channel 4 to PORT3.  This would be useful if you DIDN'T have
a Wonkystuff MB/1, and still wanted to have at least a 3-voice MPE-based polysynth.
- AUDIO OUT outputs gated clock as usual
- The Pots *can* be set to output CCs of your choice.


### Internal Distributor Breakout Mode
This mode is just like USB DISTRIBUTOR BREAKOUT MODE except that it takes INPUT from PORT1,
notionally from the Wonkystuff MB/1 or another Wonkystuff module, and distributes channel 1 to
channels 2 and 3, then outputs them as PORT2 and PORT3.  This not a common case, but would be 
useful to build a 2-voice polysynth.
- AUDIO OUT outputs gated clock as usual
- The Pots *can* be set to output CCs of your choice.
- If there aren't any available channels for voices, voice stealing just steals channel 2


### Internal Clock Mode
This mode is just like USB CLOCK MODE except that it takes INPUT from PORT1,
and outputs reset and gate on PORT2 and PORT3. 
- AUDIO OUT outputs gated clock as usual
- IN 3 (PORT 2) outputs a trigger (RESET) when it receives START or CONTINUE
- AUDIO IN (PORT 3) outputs a GATE
- The Pots should NOT be set to output anything


### No Mode (Full Customization)
You can customize everything yourself.


## Specifying the Pot CC Outputs

Remember, you can't set the Pot CCs if you're doing USB_CLOCK or INTERNAL_CLOCK modes.

The CC PARAMETER for each Pot is set in the source code.

You can also customize which ports (PORT1, PORT2, PORT3) will receive CCs from the pots by
modifying the source code as shown.


## De-Legato

This sends a NOTE OFF before any NOTE ON, to guarantee that we do a gate prior to changing the pitch.

IMPORTANT NOTE: the purpose of De-Legato is to work around the fact that WonkyStuff's MCO/1 module is
legato-only.  HOWEVER De-Legato will trigger a MIDI bug in the MASTER I/O: if it receives two note-offs
in a row for the same note, its gate will hang permanently at HIGH.  So if you're sending signals to
the MASTER I/O via its MIDI IN jack, you need to turn off De-Legato.

De-Legato is specified on a per-channel basis.  By default, it's ON (1) for ALL OUTGOING CHANNELS.
You can turn it on/off globally or per-channel in the code. 


## Full Customization

If you set MODE to NONE, you have the option of completely customizing everything.  Options are in the source code.

