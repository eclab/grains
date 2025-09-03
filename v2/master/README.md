# Master


Master handles and redistributes MIDI Mode 4 or MPE Master Zone messages in combination with an IMDI HEART.
Master can only be used in scenarios involving a SINGLE MPE Master Zone.  This normally should be the LOWER ZONE,
but it is possible to configure it for an Upper Zone.

Explanation.  MIDI Polyphonic Expression (MPE) takes up multiple channels.  These channels are divided into
a single MASTER CHANNEL, and then some N ZONE CHANNELS.  When notes arrive in MPE they show up in the ZONE CHANNELS.
Other messages, such as CC or Pitch Bend, also arrive individually on the zone channels, but they can also show
up on the MASTER CHANNEL, in which they are interpreted as affecting all the zone channels together.  But Modular MIDI
on the AE system doesn't think about a Master Channel, so it has no way to route the Master's messages to the zone channels.

If you need to route the Master's messages to zones, that's what Master is for.
The most common scenario is to route CC or PC to all your polyphonic oscillators or other voice modules.

You'll need to set up some constants to define the MPE Zone in question.

## Configuration

You will need an IMDI HEART.  Master can be set up to run on one of two configurations:

1. Some kind of Arduino, such as an Arduino Nano or Arduino Nano Every.  You will connect to its hardware UART,
   that its RX and TX ports.  You will also need to set ```HARD\_SERIAL``` to the appropriate device.  On most Arduinos,
   this device is     ```Serial```     But on the Nano Every this device is      ```Serial1```      
   
   Accompanying this file is a PDF file showing configurations of the Arduino Nano Every on
   a BRAEDBOARD which might be useful for you in setting up the hardware.

2. GRAINS V2.  This doesn't exist yet: I have a prototype but it may never go into production.
   You will connect to the RX and TX ports.   Uncomment ```GRAINS\_V2``` And set ```HARD\_SERIAL``` to    ```Serial```   
   
   Additionally, pressing the TOP BUTTON will send both an ALL NOTES OFF and an ALL SOUNDS OFF to all Zone channels.


The arrangement is as follows.  We will modify the IMDI HEART so that it routes to its 16 outputs data coming
from the IMDI input instead of from MIDI or USB.  To do this, we set ICN=NONE.  [It should be ICN=IMDI, but there
is a bug in IMDI HEART which accidentally swapped IMDI and NONE in the ICN menu!]

Next, we connect the socket labelled "MIDI IN -> IMDI" (very confusing) to RX on our GRAINS V2 or Arduino.
Finally, we attach TX on the GRAINS V2 or Arduino to the top left "IMDI" socket on the IMDI HEART.

What will happen is this.  MIDI comes into the MIDI socket or USB on the IMDI HEART.  It is then routed directly to
the Grains or Arduino.  The Grains or Arduino then modifies the MIDI and sends it back to the IMDI HEART in the IMDI
socket.  This is then redirected to the 16 sockets.

Pitch Bend is the current problem child.  The MPE spec specifies that Bend messages must be receivable both on
the Master Channel and also on the per-note channels, and furthermore that these may have different bend ranges,
and a synthesizer must be able to add them together in a meaningful way.  We cannot easily do that, and it is costly
to compute.  So at present, ```DISTRIBUTE\_BEND``` is turned off, and if you turn it on, MASTER simply redistributes
the Master Channel bend message to the children.


# Distribution Rules

Not all MIDI data is routed: some is ignored because it is inappropriate to route.  Here are the rules.


NOTE ON --- Master messages are ignored.  Channel messages are passed through.

NOTE OFF--- Master messages are ignored.  Channel messages are passed through.

POLY AFTERTOUCH --- Master messages are ignored.  Channel messages (discouraged) are passed through.

CHANNEL AFTERTOUCH --- Master messages are distributed to all channels.  Channel messages (discouraged) are passed through.

BANK / PROGRAM CHANGE (PC) --- Master messages are distributed to all channels.  Channel messages are passed through.

CONTROL CHANGE (CC) --- Master messages are distributed to all channels.  Channel messages are passed through.
PITCH BEND		Master messages are distributed to all channels only if DISTRIBUTE\_BEND.  Channel messages are passed through.

SONG POSITION POINTER, SONG SELECT --- All messages are passed through.

START, STOP, CONTINUE, CLOCK PULSE --- All messages are passed through.

SYSTEM EXCLUSIVE	All --- messages are passed through.

ACTIVE SENSING, SYSTEM RESET, TUNE REQUEST --- All messages are ignored.

MIDI TIME CODE QUARTER FRAME --- All messages are ignored.

## Latency

To do this magic, MASTER must parse entire MIDI messages and then emit them along multiple channels.  Thus there
is a SMALL amount of latency until MASTER has received an entire MIDI message before it can send it out.  There also
will be a SIGNIFICANT amount of latency if MASTER has to repeat this message multiple times to multiple zone channels.

For example, if your MPE Zone is 15 channels, and MASTER receives a CC message on the Master Channel, it will have to
emit 15 separate CC messages, one for each channel.
