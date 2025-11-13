# Cascade

Cascade is a MIDI player which can play a song of up to about 6000 notes on GRAINS,
and more on certain other Arduino configurations.  Cascade is meant to run on the
AE Modular GRAINS, but it could be adapted to any Arduino.

Cascade can store several thousand notes or other MIDI events and can replay them over
Modular MIDI.  You could attach it to a single MIDI module, such as the mco/1 or mcc/4,
or you can feed it into the IMDI HEART and break the MIDI into multiple channels for
multiple downstream devices.

#### Bug in IMDI HEART

Cascade is particularly useful in combination with IMDI HEART: you feed Cascade's Modular MIDI into IMDI HEART's "IMDI" input.  To do this, you must change ICN, IRT, and ISY so that IMDI HEART receives its MIDI from its IMDI input.  However the menu options for these three settings is mixed up.  To set these to IMDI, you must instead set them to **NONE**.  (NONE and IMDI are mixed up in the menu).



## The song.h File

To load MIDI into Cascade, you create a file called "song.h" and place it right next
to the file "cascade.ino".  This file should look like this:

    #define BPM ...						// This is your speed in beat per minute.  It should be an integer > 0, such as 120.
    #define TIMELENGTH ...				// This is the length the timestep array below.
    #define LONGTYPE						// This line might not exist.  If it exists, then TIMETYPE is 32-bit.  If not, 16-bit.

    // This little macro sets our integer type for the timestep array.
    // It must be here.
    #ifdef LONGTYPE
    #define TIMETYPE uint32_t
    #else
    #define TIMETYPE uint16_t
    #endif

    // This is the array of timestamps for each MIDI message.  They're either 16-bit or 32-bit integers.
    PROGMEM const TIMETYPE timestep[TIMELENGTH] = { ..., ..., ... }; 
   
    #define DATALENGTH ...				// This is the length of the data array below.  

    // This array holds the bytestream of MIDI Messages.  There are exactlyt TIMELENGTH messages in total,
    // but they can be various bytes in length each.
    PROGMEM const uint8_t data[DATALENGTH] = { ..., ..., ... };

The **data** array is the bytes for all the MIDI Messages in order.  You can have any messages you like,
but you are not permitted to insert the single-byte System Realtime messages (such as Clock Pulse)
in the middle of some message, which is normally permitted by MIDI but which is too complex for
Cascade's trivial parser.  Most DAWs don't do it, but it might happen.

Cascade comes with a "song.h" file which is my submission to AETHERWAVES vol 7.  It's too big to fit on
GRAINS, but works well on the Arduino Nano Every.



## Using Dave and Seq

The Dave Java program, part of the Dave GRAINS firmware in my firmware collection, can be used to convert
a standard MIDI file into a "song.h" file.  You can download the Dave Java program here:

    https://github.com/eclab/grains/tree/main/dave/java

See Dave's menu option "File -> Convert .mid file to Cascade .txt"

If you need a sequencer which can output to MIDI files, try Seq, my sequencer.

    https://github.com/eclab/seq


## Some Notes on Timing and Music Speed

The <timestep> array are timestamps for when the MIDI Messages should appear.  The timestamps are in 
TICKS, where there are a certain number of ticks per beat.  At present this value is is 192, which
is common; but your DAW may output something else.  In the "song.h" file you also define BPM, 
which is the beats permit (the speed) of your song.  Commonly BPM might be 120.  Cascade converts 
time (in ms) into ticks with the following equation:

   	return (time * BPM) / (60000 / TICKS_PER_BEAT);

If your DAW has a TICKS PER BEAT of, say, 360, then you may need to change the #define for
TICKS_PER_BEAT.  By default it is 192.

Note that if BPM=120, the maximum
possible value Cascade can compute for the time is 2^32 / BPM = 2^32 / 120 = 4294967295 / 120.
That value is 35791394 milliseconds, or about 35791 seconds, or 596 minutes, or about 10 hours
before Cascade overflows in its time computation and things go haywire.  If you increase your BPM 
significantly, the number of hours will drop correspondingly.

Also remember that MIDI runs at 3125 bytes per second.  A typical MIDI message is 3 byes long,
so you can at most have 1000 messages per second.  A note needs a note-on and a note-off message,
so this means you can play about 500 notes per second.  If you increase your BPM dramatically,
or have very heavy MIDI traffic, you may force cascade to output more notes than it is capable.


## Some Notes on Memory

Cascade can represent timestamps with either 16-bit integers or 32-bit integers.  With 16-bit 
integers, a typical MIDI message drops from (roughly) 8 bytes to 6 bytes, giving you more memory
in which to store your song.  But this means that your maximum timstamp must be no larger than 
65535 ticks.  If it exceeds this, you have to move to 32-bit integer timestamps.

Other arduinos (such as the Arduino Nano Every, or the Arduino Mega) have more memory available
than GRAINS (which is the same memory as a Nano or an Uno).


## Cascade on the Grains vs. the Nano Every

By default Cascade is set up to run on the Nano Every.  You can easily set up a Nano Every to
run on your BRAEDBOARD, and it's cheap to buy.  For instructions on setup, see here:  
https://github.com/eclab/grains/tree/main/v2/info

The Nano Every uses Serial1 as it MIDI output.

Alternatively you can use Cascade on a GRAINS, where DIGITAL OUT (D) is the MIDI Output. 
To do this you need to change two things:

- Unzip the files "NeoSWSerial.h" and "NeoSWSerial.cpp" located in this directory
- Uncomment the following #define:

    // #define USE\_SOFT\_SERIAL 

This sets up Cascade to use Software Serial.  Be warned that this may drop a byte here
or there but it should work okay.  

BTW if you have a Grains V2 (and presently I am the only person in the world outside of
Tangible Waves, so that's unlikely) you could use it as well.  Serial1 is the MIDI output.
In this case, you'd instead uncomment the following #define:

    // #define GRAINS_V2 



## Controlling Cascade

Cascade has a trigger which starts the sequence running or resets it while it is running.
Cascade also has a trigger which stops a sequence.  When a sequence is stopped, Cascade
will send ALL NOTES OFF to all channels, and a NOTE OFF (actually a NOTE ON with velocity 0) 
to every single pitch on channel 0.



## Configuration

#### IN 1
[Unused]
#### IN 2
[Unused]
#### IN 3
Stop
#### AUDIO IN (A)
Start / Reset
#### AUDIO OUT
[Unused]
#### DIGITAL OUT (D) 
MIDI Output (on GRAINS)
#### POT 1
[Unused]
#### POT 2
[Unused]
#### POT 3
[Unused]
#### Serial 1
MIDI Output (on Nano Every)
#### Serial RX
MIDI Output (on Grains V2)        
