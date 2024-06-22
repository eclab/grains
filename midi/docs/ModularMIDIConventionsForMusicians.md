# Modular MIDI Conventions for Musicians
**Version 0.5**, June 22, 2024

- Sean Luke, George Mason University (sean@cs.gmu.edu)

## Introduction 

Modular MIDI is based on the design by John Tuffen (wonkystuff) for the modules he developed for the AE Modular system.  In his approach, MIDI entered the system via a TRS-B MIDI port on a **Distributor Module** (the **wonkystuff mb/1** or **MIDI Brain**), and then travelled from module to module using standard AE Modular sockets and wires, just like Gate/CV or Audio.  

The mb/1 Distributor Module divided incoming MIDI messages up by channel, and each channel had its own dedicated outgoing AE Modular socket.  This meant that modules attached to a given MIDI socket on the Distributor didn't need to care what channel they were on, and could just operate in so-called OMNI mode (ignoring channel information).  Additionally, MIDI clock messages were converted into a variety of CV messages.  The mb/1 also provided a copy of all incoming MIDI.

Attached to the mb/1 were various modules which responded to MIDI.  For example, the **mco/1** was a simple oscillator which received MIDI NOTE ON/OFF messages and changed the pitch of the oscillator accordingly.  It also sent a gate out to notify envelopes etc. that a note had been received.  Finally, it responded to a small set of MIDI CC messages to change parameters of the oscillator. There were also modules which received MIDI and outputted CV values, drum triggers, and clock division triggers.

Each of these modules had a "MIDI IN" port from which they received MIDI messages, and a "MIDI THRU" port where they passed on the same messages to other modules, so multiple modules could listen for the same messages.  For example, this allowed two detuned oscillators to respond to the same notes.

In fall of 2023, Tuffen, Mathias Brussel (of tangible waves), and Sean Luke (George Mason University) began work in a standardized set of MIDI conventions to extend this model to more complex scenarios.  This eventually led to a paper, ["Module-Level MIDI"](https://cs.gmu.edu/~sean/papers/modulelevelmidi.pdf), presented at the Sound and Music Computing (SMC) conference in Summer of 2024.  Those conventions led to the ones you are reading here.

### A MIDI Primer

The Music Instrument Digital Interface (MIDI) was developed to allow a controller, or a keyboard, or a DAW, or a groovebox, to play another synthesizer remotely.  MIDI is a very simple protocol: a **sender** sends **messages** one at a time, which are heard by one or more **receviers**.  Perhaps a DAW or controller is a sender, and one or more synthesizers are receivers.  It's one-way only: a receiver cannot respond to the sender.  These messages consist of between one and three bytes each, and there is one special kind of message ("System Exclusive") that can be longer.  

Some messages are **voiced messages**.  This means that they are tagged with a **channel** -- a number 1...16 -- which indicates who among the various receivers is supposed to respond to that message.  A receiver commonly is set up to respond to messages on a single channel, but a receiver can also be set up to respond to all voiced messages regardless of channel: this is called being OMNI.

There are several kinds of voiced messages:

- NOTE ON messages and NOTE OFF messages instruct the receiver that a note has begun or ended.

- AFTERTOUCH or "PRESSURE" messages tell the receiver that currently pressed notes are being pressed *harder* (or less hard).

- PITCH BEND messages tell the receiver to bend the pitch of all playing notes.

- PROGRAM CHANGE or PC messages ask the receiver to load and use a new program (patch).

- CONTROL CHANGE or CC messages tell the receiver that it should change some parameter X to some new value Y.  What parameter X is, and how Y affects it, is defined by the receiver.  For example, the receiver may have documented that if it is told to change parameter 14, that's the filter cutoff.  Think of them like virtual knobs on your synthesizer.  They are roughly the equivalent of CV.

There are also a few **unvoiced** messages, that is, global messages that *everyone* always listens to.  The most common such messages are **clock** messages to keep everyone in sync.  For example, CLOCK START tells everyone that the timing clock has reset and started.  The CLOCK PULSE message is sent out 24 times per quarter note.  CLOCK START stops the clock.  CLOCK CONTINUE restarts the clock from where Clock Continue stopped it, rather than ressetting it.

There are a few other MIDI conventions you should be aware of to understand stuff below.

- REGISTERED and NON-REGISTERED PROGRAM NUMBERS (RPN and NRPN).  The number of CC parameters is limited, and they have low resolution.  RPN and NRPN are a scheme which uses a combination of a few special CC messages to provide many more parameters, and also to greatly increase their resolution.  This fixes a lot of problems, but even though RPN and NRPN have been around for a long time, they are still not well supported by DAWs and controllers, which is a tremendous shame.  They are also somewhat slower than a single CC message.

- MIDI POLYPHONIC EXPRESSION (MPE).  MIDI doesn't have per-note parameters.  For example, if you wanted to bend one note being played on channel 3, but not the others, too bad.  They'll all get bent.  The same thing goes for CC messages.  MPE fixes this by assigning each note its own channel.  A receiver would listen not to a single channel but to (say) 8 channels, and play the notes that arrive on those channels.  When a bend or CC message arrives on a channel, it only applies to that channel's note.   But this wastes channels, which are a precious commodity.  Nonetheless MPE is becoming increasingly common, and it it meshes well with Modular MIDI.

- SYSTEM EXCLUSIVE (or SYSEX) MESSAGES.  Sysex is a special kind of unvoiced, variable-length message whose meaning is left to the receiver.  Sysex can be used for anything the receiver likes, but to use sysex, a manufacturer must register and pay for a Manufacturer's ID from the MIDI Manufacturer's Association.  There are also certain standardized sysex protocols.  The most important, for our purposes, is the **MIDI Tuning Standard**, which allows the musician to customize the tunings of notes in a module.


### Why MIDI?

Let's start with the *disadvantages* of MIDI.  First, **MIDI is slow**.  It runs at 32150 bits per second.   Additionally, depending on how modules hand messages off to one another, it could also build up **latency** in some situations.  Second, MIDI parameters are often pretty low resolution: if you used CC for a filter sweep, it might create a stepping effect for example.  Third, MIDI CC parameters are useful but there aren't many of them: only about 100 usable ones.  If you have multiple modules, they might all want to use CC parameters, and you could easily run out of them.

We'll address ways around some of these issues.  But if MIDI has so many problems, why bother with it?  Because its advantages are legion.

MIDI has **stable pitch tuning**.  You will never need to tune an oscillator again.  Just tell it to play a note and it plays it.  MIDI also has facilities for **microtuning** via the **MIDI Tuning Standard**, if you wanted to customize the tuning of your notes.

MIDI only needs a single wire to **control many parameters,** reducing the number of cables.   In contrast, CV requires one wire for each parameter.  

MIDI enables **polyphony** and **multitimbralism**.  You can have multiple notes sent to an oscillator, not just one, and these notes can have different parameters (via MPE) and control different kinds of sounds.

MIDI allows modules to **save and recall parameter information** (patches).

Practically every DAW, keyboard, controller, sequencer, groovebox, drum machine, and synthesizer in the world **speak MIDI.**  This should not be discounted.

### Need for Modular MIDI Conventions

There's nothing wrong with just letting modules send MIDI to each other however they like.  So why do we need some conventions?  Because MIDI wasn't designed for some of the situations that arise inside a modular synthesizer.

MIDI is designed for one keyboard controller, or DAW, to control one or more entire synthesizers.  It is not designed for individual *parts* of a synthesizer (made by different vendors) to all listen to messages from a controller.

MIDI is also not designed for multiple devices to send messages to control *each other,* such as might happen inside a modular synthesizer.

Finally, modules vary greatly in sophistication.  Some might be simple monphonic oscillators, while others might be entire polyphonic synthesizers responding to multiple MIDI channels.  We need conventions that simple devices can implement *a little of*, while other devices can implement more.

## Connecting Modules

Modular MIDI flows over standard modular cabling, and goes out of and into sockets just like Gate/CV or Audio.  A basic MIDI module will have at least two sockets for MIDI:

- **MIDI IN**.  This socket receives MIDI messages from elsewhere.  The module responds to some or all of these messages.

- **MIDI THRU**.  This socket sends an exact copy of the messages received on MIDI IN.  This enables other modules to respond to those messages as well.

It's possible that you might see other sockets:

- **MIDI OUT**.  This socket sends newly created MIDI from the module.  For example, perhaps a Sequencer moodule might generate MIDI note data to drive other modules.

- **MIDI MODULATION THRU** or **SOFT THRU**.  This is like the MIDI THRU, except the module might insert some new MIDI data to send out the THRU.  That would never happen in a standard MIDI THRU.

More sophisticated modules could have multiple copies of these sockets for different purposes.

### MIDI Distribution Modules and Voice Chains

In most cases you'll want a way to get MIDI data from a DAW or controller into your modular system.  To do this you'd use a **MIDI Distribution Module**.   This module takes MIDI from a standard 5-pin, USB, or TRS MIDI connection and distributes it to your other MIDI modules.  The **Wonkystuff mb/1** is an example of a MIDI Distribution Module.

MIDI has 16 **channels**. In Modular MIDI, usually a channel represents a monophonic **voice**, that is, MIDI data for playing a single note with a certain kind of sound.  Or a channel can be used for a **drumkit**, which plays different sounds on different notes.  For purposes of discussion here, we will consider a drumkit (with multiple drums) to be a single "voice".

Multiple modules (MIDI or not) work together to create a single voice.  You might have an oscillator, a filter, an envelope, an LFO, etc.  Collectively these are known as a **voice chain**.  Some might be controlled by MIDI, others by Gate/CV.  The ones controlled by MIDI will be connected together (MIDI THRU of one module to the next module's MIDI IN, and so on) so they all hear the same MIDI data.  The first MIDI module will be probably connected to a dedicated output port on the MIDI Distribution Module just for messages of that channel: thus the only messages a voice chain would receive would be ones for a single channel.

## IDs and CCs

Controlling modules on a voice chain with CC parameters is a major issue.  MIDI only has 128 CC parameters per channel, and realistically only about 100 are available.  But you may have multiple modules listening in on the same channel: for example, you might have a filter module, an envelope module, and an oscillator module.  They're by different vendors, and each of them can be programmed using CC messages.  How do you prevent them from accidentally overlapping?  You don't want to adjust your filter cutoff only to find that your oscillator tuning has changed as well...

One simple scheme to avoid this is to have modules which do so-called **CC Learn**.  Here, you might press a "learn" button, and then send a CC message, and the module moves all of its CC pararameter numbers so that the lowest one is the same as that CC message.  This can work for simple devices but training many modules to avoid one another, and to avoid certain standardized CC message parameters, may be difficult.

The alternative scheme we propose here is to assign each module an **ID**.  This is just a number 1, 2, ..., 15.  The ID of the module determines which CC parameters it's allowed to use.  Each ID is allotted 9 CC parameters, "a" through "i".  Additionally, your module has the option of bundling parameter "a" up with parameter "i" to create (in MIDIs-speak) a single "14-bit CC" parameter, consisting of two CCs, to allow higher resolution.  One CC is called the MSB, and the second is called the LSB.  The resulting parameter is just called "a", and "i" doesn't exist any more.  Your module can also bundle together parameters "b" and "h" to form a second 14-bit parameter called "b"; and "h" would no longer exist.

Below is a table, for each ID and each parameter a...i, of the CC parameter number assigned to that parameter.  For example, if your module was ID 2, and it used "a/i" as a 14-bit parameter but kept "b" and "h" separate, then the CCs for "a/i" would be the 14-bit parameter 14 and 46 (MSB and LSB), "b" would be 15, "c" would be 70, "d" would be 71, "e" would be 72, "f", would be 73, "g" would be 75, and "h" would be 47.

Modules can have their IDs changed so that they don't conflict with one another, and thus the CCs associated with their respective parameter numbers can change too.

Some CCs have traditional (non-required) uses. You may find them in your DAW's list of CC names.  We list some of them in this table, because certain IDs (see the next table after this one) have been lined up to conveniently correspond with these names if your module sees fit.


ID  | Parameter   | 7-bit CC Parameter | 14 bit CC Parameter MSB/LSB | Traditional Use
----|-------------|--------------------|-----------------------------|----------------
1   | a           | 8                  | 8 / 40
    | b           | 9                  | 9 / 41
    | c           | 65                 |                             | Glide Switch
    | d           | 66                 |
    | e           | 67                 |    
    | f           | 69                 |
    | g           | 112                |
    | h           | 41   [only if "1 b" is 7-bit] | 
    | i           | 40   [only if "1 a" is 7-bit] |    
2   | a           | 14                 | 14 / 46
    | b           | 15                 | 15 / 47
    | c           | 70                 | 
    | d           | 71                 |
    | e           | 72                 |                             | Release
    | f           | 73                 |                             | Attack
    | g           | 75                |                              | Decay
    | h           | 47 [only if "2 b" is 7-bit] | 
    | i           | 46 [only if "2 a" is 7-bit] |   
3   | a           | 16                 | 16 / 48
    | b           | 17                 | 17 / 49
    | c           | 76                 |                             | Vibrato Rate
    | d           | 77                 |                             | Vibrato Depth
    | e           | 78                 |                             | Vibrato Delay    
    | f           | 79                 |
    | g           | 80                 |
    | h           | 49 [only if "3 b" is 7-bit] | 
    | i           | 48 [only if "3 a" is 7-bit] |    
4   | a           | 18                 | 18 / 50
    | b           | 19                 | 19 / 51
    | c           | 81                 | 
    | d           | 82                 |
    | e           | 83                 |    
    | f           | 84                 |
    | g           | 85                 |
    | h           | 51 [only if "4 b" is 7-bit] | 
    | i           | 50 [only if "4 a" is 7-bit] |   
5   | a           | 20                 | 20 / 52
    | b           | 21                 | 21 / 53
    | c           | 86                 | 
    | d           | 87                 |
    | e           | 88                 |                             | High Res Velocity    
    | f           | 89                 |
    | g           | 90                 |
    | h           | 53 [only if "5 b" is 7-bit] | 
    | i           | 52 [only if "5 a" is 7-bit] |    
6   | a           | 12                 | 22 / 44                     | FX Control 1
    | b           | 13                 | 23 / 45                     | FX Control 2
    | c           | 91                 |                             | FX 1 Amount 
    | d           | 92                 |                             | FX 2 Amount
    | e           | 93                 |                             | FX 3 Amount  
    | f           | 94                 |                             | FX 4 Amount
    | g           | 95                 |                             | FX 5 Amount
    | h           | 45 [only if "6 b" is 7-bit] | 
    | i           | 44 [only if "6 a" is 7-bit] |    
7   | a           | 22                  | 24 / 54
    | b           | 23                  | 25 / 55
    | c           | 102                 | 
    | d           | 103                 |
    | e           | 104                 |    
    | f           | 105                 |
    | g           | 106                 |
    | h           | 55 [only if "7 b" is 7-bit] | 
    | i           | 54 [only if "7 a" is 7-bit] |    
8   | a           | 24                  | 8 / 56
    | b           | 25                  | 9 / 57
    | c           | 107                 | 
    | d           | 108                 |
    | e           | 109                 |    
    | f           | 110                 |
    | g           | 111                 |
    | h           | 57 [only if "8 b" is 7-bit] | 
    | i           | 56 [only if "8 a" is 7-bit] |    

Every module that uses CC has assigned a **Default ID**.  The Default ID is its initial ID value.  Some simple modules cannot change this to a different ID, but more sophisticated ones can.  The Default ID is associated with the category of the module.  The Default ID categories are as follows:

ID | Default ID Category
---|-----------------------------------------------------
1  | Oscillators, Samplers, and Note --> Gate/Trigger Modules
2  | Envelopes
3  | LFOs, Sequencers, and Clock --> Gate/Trigger Modules
4  | Filters
5  | VCAs and Mixers
6  | Effects and Audio Processors
7  | CC --> Gate/CV/Trigger Generators
8  | Miscellaneous

If you are fortunate, you will never need to change the ID, just leaving it as the default.  But if you have two modules listening in on the same channel with the same ID, you may need to change one to some other unused ID to keep them from conflicting.

Your modules may also respond to certain other CC functionality, such as Bank Select, Modulation Wheel, Glide MSB, Volume MSB, Pan MSB, Expression Controller, Sustain Pedal, Legato Switch, MPE Timbre, All Sounds O/ff, Reset All Controllers, and All Notes Off.

## Auxiliary Parameters and Additional IDs

Only 8 IDs have space allotted to them in the CC space.  But there is space for 7 more IDs in what we call the **Auxiliary Parameters**.  Auxiliary Parameters are set with CCs 3 and 35.  CC 3's value sets the auxiliary parameter number, and CC 35's value sets the auxiliary parameter value.  Send CC first, and then CC 35.  Notice that 

CC 3 Value     | Auxiliary Parameters                      
---------------|-------------------------------
16-31          | First 16 Parameters for ID 9
32-47          | First 16 Parameters for ID 10
48-59          | First 16 Parameters for ID 11
64-79          | First 16 Parameters for ID 12
80-95          | First 16 Parameters for ID 13
96-111         | First 16 Parameters for ID 14
112-127        | First 16 Parameters for ID 15

Notice that each of the IDS 9...15 have 16 parameters allotted to them.  They are all "7-bit".  You can think of these as parameters "a" through "p".

We will discuss Program Save, Current Program Save, and Current Program Revert in a section below.

## NRPN

If your module supports it, you can also be able to set up to 256 parameters in your module using NRPN.  NRPN is a bit slower than CC but it has much higher resolution and more parameters.  Unfortunately many DAWs and controllers do not support it.  The first 9 parameters in your NRPN region correspond to the CC parameters "a" through "i": and if your ID is in the Auxiliary Parameter space, the first 16 parameters correspond to its parameters "a" through "p".

NRPN Region  | Function                      
-------------|-------------------------------
256-511      | 256 Parameters for ID 1
512-767      | 256 Parameters for ID 2
768-1023     | 256 Parameters for ID 3
1024-1279    | 256 Parameters for ID 4
1280-1535    | 256 Parameters for ID 5
1536-1791    | 256 Parameters for ID 6
1792-2047    | 256 Parameters for ID 7
2048-2303    | 256 Parameters for ID 8
2304-2559    | 256 Parameters for ID 9
2560-2815    | 256 Parameters for ID 10
2816-3071    | 256 Parameters for ID 11
3072-3327    | 256 Parameters for ID 12
3328-3583    | 256 Parameters for ID 13
3584-3839    | 256 Parameters for ID 14
3840-4095    | 256 Parameters for ID 15


## Loading and Saving Patches

Your module might support loading and saving patches.

Loading a patch is done in the usual MIDI way: via a Bank Select (both MSB and LSB), followed by a Program Change command.  The Bank Select selects your bank, and the Program Change selects the patch in the bank.

Modular MIDI also supports **Program Save**.  This is an Auxiliary Parameter, and its value indicates the patch slot to save your current parameters to.

Modular MIDI further supports **Current Program Save**.  This is an Auxiliary Parameter, CC 3 = 1, and CC 35 = 0.  It instructs the module to save its current parameters to the patch slot it is presently using.  

Finally, Modular MIDI supports **Current Program Revert**.  This is an Auxiliary Parameter, CC 3 = 1, and CC 35 = 1.  It instructs the module to reload its current parameters from the patch slot it is presently using.  

The following table shows the Auxiliary Parameters for these commands:

CC 3 Value     | Auxiliary Parameter                      
---------------|-------------------------------
0              | Program Save (CC 35 = program number)         
1              | Current Program Save (CC 35 = 0)
1              | Current Program Revert (CC 35 = 1)



## Modulation CC Messages

Modules might not just respond to MIDI from your DAW or Controller.  They might also send messages to other modules to control them.  For example, you might have a sequencer module which sends MIDI notes to other modules to play them.

Another possibility is to have **modulator modules** sending messages to modulate **target modules**.  For example, you might have an LFO which is sending CC messages to modulate a Filter module.  

We have a scheme to make modulation simple.  A modulation module can send CCs to control any one of the first 8 parameters ("a" through "h") of your target module.  It doesn't need to know the ID of the module.  Instead it just sends it one of 8 **modulation CC messages**.

For example, your Filter might have "a" as its Cutoff and "b" as its Resonance.  Your LFO could have a knob on it where you set what it's modulating ("a" through "h").  If you set it to "a", and hook its MIDI MODULATION THRU to the Filter's MIDI IN, then the LFO will control the Filter Cutoff.

The first two modulation parameters are 14-bit high-resolution, and the rest are 7-bit low-resolution.

The ADSR's MIDI IN can also be connected to receive MIDI from your DAW; and it will pass that MIDI on to the Filter as well (as well as **injecting** the modulation CC messages necessary to control the Filter).

### Another Possibility

What if you also want to modulate your LFO from *another module*?  For example, imagine you wanted an ADSR to control the rate of your LFO, and the LFO controlled the cutoff of the Filter.  Your LFO might be able to do this.  Let's say the LFO rate is its parameter "d".  You set the ADSR to modulate "d" and attach it to the LFO.  The LFO is attached to the Filter as usual.  Attach the ADSR to receive MIDI from your DAW or controller, and it will pass it along to the LFO, which passes it on to the Filter.

You have to instruct the LFO not to pass modulation CCs it gets from the ADSR on to the Filter.  Then it should work.

### Yet Another Possibility

What if you want your ADSR *and* your LFO to *both* modulate your Filter?  For example, imagine you wanted an ADSR to control the filter resonance, while the LFO controls the filter cutoff.  

This is almost the same arrangement: the ADSR connects to the LFO, which connects to the Filter.  Your Filter's resonance is "b", so you set the ADSR to modulate "b".  But this time you have to instruct the LFO to *ignore and pass on* modulation CCs it gets (from the ADSR) to the Filter.



