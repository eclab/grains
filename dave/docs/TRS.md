# Connecting GRAINS MIDI to your MB/1 or MASTER I/O 

While the WonkyStuff MB/1 has a great many MIDI OUT sockets, the WonkyStuff MB/1 and TangibleWaves MASTER I/O only have MIDI IN via their TRS jacks.  That is unfortunate.  It's highly desirable to connect Dave to them, particularly to the MB/1, to break out the MIDI it generates on a per-channel basis, to drive polyphonic module chains for example.  But how do you do this?

## FIRST A DISCLAIMER

I have so connected to the MB/1 and the MASTER I/O (R4) and both seem to work okay.  From discussion with both Tangible Waves and WonkyStuff, it seems that it's likely you won't hurt anything in these modes by
connecting to them, but **I WILL NOT BE HELD RESPONSIBLE** for any damage you incur trying this.

## Basic Information

Standard 5-pin DIN MIDI cabling only uses three wires, which MIDI calls the SOURCE, the SINK, and the GROUND.  The GROUND is just the shielding on the cable.  When synthesizer A is connected to send MIDI to synthesizer B, synthesizer A provides 5V on its SOURCE and provides the MIDI data on its SINK.  I know, it's not intuitive.  The idea is that as the MIDI data goes up and down, power is drawn from the SOURCE to drive an LED in synthesizer B's optoisolator, and goes out the SINK.

TRS MIDI is exactly the same as 5-PIN DIN MIDI: three wires.  However there are two versions, TRS A and TRS B.  They differ as to which wire is the source and which is the sink.  Both the MASTER I/O and the MB/1 use TRS B, in which the SOURCE is the TIP (or "LEFT") and the sink is the RING (or "RIGHT").  The ground is the SLEEVE.

## How To

All we need to do is connect 5V to the SOURCE (TIP) and connect the GRAINS's MIDI out to the SINK (RING).  We leave the ground (sleeve) DISCONNECTED.

To do this, you first need to buy a **3.5 mm Male TRS Screw Terminal Block**.  That's the magic term.  [Google for it](https://www.google.com/search?q=3.5+mm+male+TRS+Screw+Terminal+Block).  They're very cheap, like $5.

You can attach bare wires, Dupont 0.1" cables, or your nice TangibleWaves cables to the block: they all work fine.

The screw terminal block breaks out the TRS tip, ring, and sleeve to three wire screws so we can easily attach our two wires.  Here's the configuration I am using.  Note that my block says "L" (for "Left" -- the TIP), "R" (for "Right" -- the RING) and the Ground symbol for the Sleeve.  I have connected the GRAINS output to "R" and +5V to "L".  You can get +5V from current Master I/O units, or from modules 4ATT/MIX, or even directly from the bus cable itself if you're careful.  I'd make the +5V wire red to make it clear what's what.

Once you've hooked up the wires, you just plug the terminal block into the TRS socket.  Once again, I'm providing this information to early adopters, but **I WILL NOT BE HELD RESPONSIBLE** if this information is wrong and/or somehow you damage something.  Caveat Emptor.

![Configuration of the screw terminal block attached to an MB/1.](TRS.png)

## Why This Works

MIDI requires that there be certain resistors in place to prevent the LED in the optoisolator from frying from too much current.  Specifically, you a 220 ohm or larger resistor in series with the SOURCE (Resistor Ra on the schematic below), a second 220 ohm built-in into the internal TRS jack (Rb on the schematic) and a third 220 ohm or larger resister in series with the SINK (Rc on the schematic).  All told this is 660 ohms or more.

The 220 ohm Rb resistor is built into the jacks.  The 5V SOURCE from the MASTER I/O, and the same from the BRAEBOARD, have a 100 ohm resistor in series.  This isn't enough, the SINK (the Digital OUT port) has a 1K ohm resistor in series.  All told this is 1320 ohms.

Finally, the 5V SOURCE from the 4ATT/MIX has a 1K ohm resistor rather than a 100 ohm resistor, and the MIDI schematic has a diode for reverse polarity if you mix things up (but don't do that).

![MIDI Electrical Diagram](MIDI.jpg)
