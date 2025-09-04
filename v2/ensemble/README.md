# Ensemble

Ensemble is a note distributor.  A note distributor takes notes coming in on one channel,
or on OMNI, and distributes them to up to N channels, possibly to up to 10 separate output
sockets.  This allows you to play chords or polyphony and have your AE Modular rack play
all of the notes.

For example, if you play C E G, then the C might get get routed to Channel or socket 0,
the E is routed to channel or socket 1, and the G is routed to channel or socket 2.  To do
this, the distributor must keep track of which notes are assigned to which channels so it
can turn them OFF when you let go of keys.

There are two basic ways you could set ensemble up.  First, you could use an Arduino 
device which has multiple serial ports, such as the Arduino Nano Every.  Here, MIDI data
would come in one serial port, then get distributed to N channels and each channel would
be set up to go out a separate serial port and thus a separate socket.  You'd need a device
such as an MB/1 (or an IMDI HEART) to get the MIDI data in the first place.
Accompanying this file is a PDF file (**NanoEvery.pdf**, in the **../info** directory) showing configurations of the Arduino Nano Every on
a BRAEDBOARD which might be useful for you in setting this up.

Alternatively you could use an Arduino device with a single serial port, but rig it up with
the IMDI HEART to break out to separate sockets.  Here, MIDI data would come into the IMDI
HEART and immediately routed to your Arduino's input serial port.  The Arduino would distribute
the notes to separate channells and write them out its output serial port.  This would then
get fed into the IMDI Input on the IMDI Heart, and the IMDI Heart would then break the channels
out to separate sockets.  You have to do a minor menu setting on the IMDI Heart to pull this off.

Additionally, when it receives an ALL NOTES OFF or ALL SOUNDS OFF message, ENSEMBLE will both send this message on to all of its voices, and will also reset itself.

Ensemble uses the note distributor code found in utility.c; you may find this useful for
other projects.


## Configurations
Here are some configurations.  Note that I am have only tested #1.

1. **GRAINS V2 PROTOTYPE OR NANO OR NANO EVERY IN COMBINATION WITH AN IMDI HEART.**
   I presently own a prototype GRAINS V2 from Tangible Waves, but
   it is far from clear if this prototype will ever go into production.  The prototype
   has buttons and LEDs on it and exposed RX and TX pins.  I can use it to distribute
   incoming MIDI notes on a single channnel (or on OMNI) to up to 16 channels output.
   I then route the output back into IMDI HEART and it breaks it up into the 16 sockets!
   to do this, I set ICN=NONE on IMDI HEART (that's a bug on my unit, ICN=NONE is actually
   ICN=IMDI and ICN=IMDI is ICN=NONE).  This causes IMDI HEART to route IMDI, not MIDI,
   into the 16 breakout sockets.  Then I attach my MIDI device to IMDI HEART, and connect 
   the socket labelled "MIDI IN -> IMDI" (very confusing) to RX on my GRAINS.  Then I
   attach TX on my GRAINS to the top left "IMDI" socket on the IMDI HEART.  And it works!

   You could do this same trick with a standard Arduino Nano, or with a Nano Every.  In
   the second case, if you have set up the Nano Every as shown in the diagrams that accompany
   ensemble.ino, and have modified your Arduino files to allow Serial1, Serial2, and Serial3,
   then you will need to set HARD\_SERIAL to Serial1.
   
   Additionally, pressing the TOP BUTTON will send both an ALL NOTES OFF and an ALL SOUNDS OFF to all its voices.


2. **ARDUINO NANO EVERY.**  The NANO EVERY has four serial ports, three of which can be used
   for our purposes.  This can be used without an IMDI HEART.  Each serial port will output
   a separate voice.  This means that we can have up to *3* voices (not 16).  To do this,
   first set up your Nano Every as shown in the diagrams that accompany ensemble.ino, 
   and modify your Arduino files to allow Serial1, Serial2, and Serial3.  Next, uncomment
   MULTI\_OUTPUT.  Make sure that NUM\_VOICES is <= 3.  For information on setting up the
   Arduino Nano Every,
   See https:forum.arduino.cc/t/arduino-nano-every-access-to-4-serial/614358/2
   See https:emalliab.wordpress.com/2021/12/16/arduino-nano-every-serial-ports/


3. **ORIGINAL GRAINS IN COMBINATION WITH IMDI HEART.**  In this case you do not have exposed
   hardware ports and have to use software serial.  Software serial turns off interrupts
   for too long when writing bytes, and as a result it may miss a read byte.  So expect 
   note drops and general flakiness. I would only use this if your notes are very SLOW 
   and SEPARATE from one another, or only for testing purposes.  Here, uncomment SOFT\_SERIAL.  
   Set things up as in #1 above.  Your RX socket will be AUDIO IN ("A") and your TX socket
   will be DIGITAL OUT ("B").  No buttons or LEDs for you.

   For this configuration, you need to move the NeoSWSerial.cpp and NeoSWSerial.h files to
   the same directory as the Ensemble.ino file.  They're presently in the "library" directory.

You could probably get other Arduinos working, such as an Arduino Uno in the case of #1,
or an Arduino Mega in the case of #1 or #2.



## Configuration (Grains V2 Prototype)

#### IN 1
[Unused]
#### IN 2
[Unused]
#### IN 3
[Unused]
#### AUDIO IN (A)
[Unused]
#### IN 5
[Unused]
#### AUDIO OUT
[Unused]
#### D10 OUT
[Unused]
#### DIGITAL OUT (D) 
[Unused]
#### RX
RX (MIDI IN)
#### TX
TX (MIDI OUT)
#### LED 1 OUT
Lit to show first note
#### LED 2 OUT
Lit to show second note
#### LED 3 OUT
Lit to show third note
#### POT 1
[Unused]
#### POT 2
[Unused]
#### POT 3
[Unused]
#### SWITCH 1
Reset
#### SWITCH 2
[Unused]
#### SWITCH 3
[Unused]

## Configuration (Arduino Nano Every)

See the file **NanoEvery.pdf** for configuration information.

## Configuration (Grains with Soft Serial)

#### IN 1
[Unused]
#### IN 2
[Unused]
#### IN 3
[Unused]
#### AUDIO IN (A)
RX (MIDI IN)
#### AUDIO OUT
[Unused]
#### DIGITAL OUT (D) 
TX (MIDI OUT)
#### POT 1
[Unused]
#### POT 2
[Unused]
#### POT 3
[Unused]
