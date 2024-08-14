# Modular MIDI Conventions for Musicians
**Version 0.5**, August 14, 2024

Sean Luke, George Mason University (sean@cs.gmu.edu)

### Table of Contents

[Introduction](#introduction).  The point of Modular MIDI.  Where Modular MIDI came from.  Why you need it.  Or don't.    
[A MIDI Primer](#midiprimer).  In case you're not familiar.   
[Connecting Modular MIDI Modules](#connecting).  How you hook stuff up.  
[IDs and CCs](#ccs).  Lots of modules, not enough CC parameters.  What to do?  
[Auxiliary Parameters and Additional IDs](#auxiliary).  When you run out of CCs.   
[NRPN](#nrpn).  Even more space than CCs!  
[Loading and Saving Patches](#loading).  Program Change and friends.  
[Modulation CC Messages](#modulation).  How one module can use MIDI to modulate other modules.  
[Tables](#tables).  All the nitty-gritty at the end.  

An accompanying document, **Modular MIDI Conventions for Developers**, is a more terse description.

<a name="introduction"/></a>
## Introduction 

Modular MIDI is a set of conventions for MIDI to guarantee that multiple modules can work together to respond to your MIDI messages in a variety of ways without messing each other up.  It allows you to send MIDI to your modular synthesizer to do polyphony and timbrality, remote control of many parameters, stable pitch control and microtuning, patch saving and loading, and so on.

Modular MIDI goes under different names.  Notably **IMDI** is Tangible Waves's brand name for their implementation.

### Do you Need Modular MIDI?

Maybe not!  If you just want to play your modular synth monophonically from MIDI, you don't need any of this stuff: you can just use the MIDI input found on Master I/O revisions 1 through 5, and be done with it.

### Modular MIDI History

Modular MIDI is based on the design by John Tuffen (wonkystuff) for the modules he developed for the AE Modular system.  In his approach, MIDI entered the system via a MIDI port on a **Distributor Module** (the **wonkystuff mb/1** or **Tangible Waves MIDI Brain**), and then travelled from module to module using standard AE Modular sockets and wires, just like Gate/CV or Audio.  

Let's talk about the **mb/1** for a moment.  The mb/1 divided incoming MIDI messages up by channel, and each channel had its own dedicated outgoing AE Modular socket.  This meant that modules attached to a given MIDI socket on the mb/1 didn't need to worry about what channel they were on.  Additionally, MIDI clock messages were automatically converted to various CV messages.  The mb/1 also provided to one special socket a copy of all incoming MIDI.

Attached to the mb/1 were modules which responded to MIDI.  For example, the **mco/1** was a simple oscillator which received MIDI NOTE ON/OFF messages and changed the pitch of the oscillator accordingly.  It also sent a gate out to notify envelopes etc. that a note had been received.  Finally, it responded to a small set of MIDI CC messages to change parameters of the oscillator. There were also modules which received MIDI and outputted CV values, drum triggers, and clock division triggers.

The mco/1 had a "MIDI IN" port from which it received MIDI messages, and a "MIDI THRU" port where they passed on the same messages to other modules.  For example, this allowed two detuned oscillators to respond to the same notes.

In fall of 2023, Tuffen, Mathias Brüssel (of tangible waves), and Sean Luke (George Mason University) began work in a standardized set of MIDI conventions to extend this model to more complex scenarios.  This eventually led to a paper, ["Module-Level MIDI"](https://cs.gmu.edu/~sean/papers/modulelevelmidi.pdf), presented at the Sound and Music Computing (SMC) conference in Summer of 2024.  Those conventions led to the ones you are reading here.

### Why MIDI?

Let's start with the *disadvantages* of MIDI.  First, **MIDI is slow** and can have a lot of **latency**.  Second, many MIDI parameters are often **low resolution**.    Third, MIDI CC parameters are useful but they are **scarce**: there only about 100 of them.  If you have multiple MIDI modules, they might all want to use CC parameters, and you could easily run out of them.

If MIDI has so many problems, why bother with it?  Because its advantages are legion.

MIDI has **stable pitch tuning**.  You will never need to tune an oscillator again.  Just tell it to play a note and it plays it.  MIDI also has facilities for **microtuning**, if you wanted to customize the tuning of your notes.

MIDI only needs a single wire to **control many parameters,** reducing the number of cables.   In contrast, CV requires one wire for each parameter.  

MIDI enables **polyphony** and **multitimbralism**.  You can have multiple notes sent to an oscillator, not just one, and these notes can have different parameters (via MPE) and control different kinds of sounds.

MIDI allows modules to **save and recall parameter information** (patches).

Practically every DAW, keyboard, controller, sequencer, groovebox, drum machine, and synthesizer in the world **speak MIDI**.  This should not be discounted.

### Need for Modular MIDI Conventions

There's nothing wrong with just letting modules send MIDI to each other however they like.  So why do we need some conventions?  Because MIDI wasn't designed for some of the situations that arise inside a modular synthesizer.

- MIDI is designed for one keyboard controller, or DAW, to control one or more entire synthesizers.  It is not designed for individual *parts* of a synthesizer (made by different vendors) to all listen to messages from a controller.

- MIDI is also not designed for multiple devices to send messages to control *each other,* such as might happen inside a modular synthesizer.

- Finally, modules vary greatly in sophistication.  Some might be simple monphonic oscillators, while others might be entire polyphonic synthesizers responding to multiple MIDI channels.  We need conventions that simple devices can implement *a little of*, while other devices can implement more.

<a name="midiprimer"/></a>
## A MIDI Primer

The Music Instrument Digital Interface (MIDI) was developed to allow a controller, or a keyboard, or a DAW, or a groovebox, to play another synthesizer remotely.  

MIDI is a very simple protocol: a **sender** (perhaps a DAW or controller) sends **messages** one at a time, which are heard by one or more **receviers** (such as synthesizers). It's one-way only: a receiver cannot respond to the sender. 

Some messages are **voiced messages**.  This means that they are tagged with a **channel** -- a number from 1 to 16 -- which indicates who among the various receivers is supposed to respond to that message.  Usually a receiver is set up to respond to messages only on a single channel, but can also be set up to respond to all voiced messages regardless of channel: this is called being **OMNI**.

There are several kinds of voiced messages:

- NOTE ON messages and NOTE OFF messages instruct the receiver that a note has begun or ended.

- AFTERTOUCH or "PRESSURE" messages tell the receiver that currently pressed notes are being pressed *harder* (or less hard).

- PITCH BEND messages tell the receiver to bend the pitch of all playing notes.

- PROGRAM CHANGE or PC messages ask the receiver to load and use a new program (patch).

- CONTROL CHANGE or CC messages tell the receiver that it should change some parameter X to some new value Y.  What parameter X is, and how Y affects it, is defined by the receiver.  For example, the receiver may have documented that if it is told to change parameter 14, that's the filter cutoff.  Think of them like virtual knobs on your synthesizer.  They are roughly the equivalent of CV.

There are also a few **unvoiced** messages, that is, global messages that *everyone* always listens to.  The most common such messages are **clock** messages to keep everyone in sync.  

There are a few other MIDI conventions you should be aware of to understand stuff below.

- REGISTERED and NON-REGISTERED PROGRAM NUMBERS (RPN and NRPN).  The number of CC parameters is limited, and they have low resolution.  RPN and NRPN are a scheme which uses a special combination of CC messages to provide many more parameters, and also to greatly increase their resolution.  This fixes a lot of problems, but RPN/NRPN are not well supported by DAWs and controllers, even though they've been around for forever.  They are also somewhat slower than a single CC message.

- MIDI POLYPHONIC EXPRESSION (MPE).  MIDI doesn't have per-note parameters.  For example, f you wanted to bend one note being played on channel 3, but not the others, too bad.  They'll all get bent.  MPE fixes this by assigning each note its own channel.  A receiver would listen to a range of channels, and play the notes that arrive on those channels.  When a bend or CC message arrives on a channel, it only applies to that note.   This wastes channels, which are a precious commodity.  Still MPE is now very common, and meshes well with Modular MIDI.


<a name="connecting"/></a>
## Connecting Modular MIDI Modules

Modular MIDI flows over standard modular cabling, and goes out of and into sockets just like Gate/CV or Audio.  

### Sockets

A basic MIDI module will have at least two sockets for MIDI:

- **MIDI IN**.  This socket receives MIDI messages from elsewhere.  The module responds to some or all of these messages.

- **MIDI THRU**.  This socket sends an exact copy of the messages received on MIDI IN.  This enables other modules to respond to those messages as well.

It's possible that you might see some other sockets:

- **MIDI OUT**.  Maybe you have a Sequencer Module which generates its own MIDI for other modules to respond to.  It might send this out MIDI OUT to go to them.

- **MIDI MODULATION THRU** or **SOFT THRU**.  Some modules need to pass MIDI along but also **inject** their own MIDI into the stream, perhaps to modulate another MIDI module.  This can't happen in a standard MIDI THRU.  Thus these modules could sport a special MIDI MODULATION THRU to do this.

More sophisticated modules could have multiple copies of these sockets for different purposes.

### MIDI Distribution Modules and Voice Chains

In most cases you'll want a way to get MIDI data from a DAW or controller into your modular system.  To do this you'd use a **MIDI Distribution Module**.   This module takes MIDI from a standard 5-pin, USB, or TRS MIDI connection and distributes it to your other MIDI modules.  The **Wonkystuff mb/1** is an example of a MIDI Distribution Module.

MIDI has 16 **channels**. In Modular MIDI, usually a channel represents a monophonic **voice**, that is, MIDI data for playing a single note with a certain kind of sound.  Or a channel can be used for a **drumkit**, which plays different sounds on different notes.  For purposes of discussion here, we will consider a drumkit (with multiple drums) to be a single "voice".

Multiple modules (MIDI or not) work together to create a single voice.  You might have an oscillator, a filter, an envelope, an LFO, etc.  Collectively these are known as a **voice chain**.  Some might be controlled by MIDI, others by Gate/CV.  The ones controlled by MIDI will be connected together (MIDI THRU of one module to the next module's MIDI IN, and so on) so they all hear the same MIDI data.  The first MIDI module will be probably connected to a dedicated output port on the MIDI Distribution Module just for messages of that channel: and so the only messages a voice chain would receive would be ones for a single channel.

This model works fine for **multitimbral** synthesizers -- with different kinds of synth voices on different channels.  It also works fine with **polyphonic synthesizers** using **MIDI Polyphonic Expression** or **MPE**.  MPE is set up to send out one note (one voice) per channel.  

But it's common that your DAW or controller sends out polyphonic notes on a single channel.  It's plausible to have a special feature on your MIDI Distribution Module which distributes those notes to different voice chains.  Nothing forbids this.



<a name="ccs"/></a>
## IDs and CCs

Controlling modules on a single voice chain with CC parameters presents a major issue.  MIDI only has 128 CC parameters per channel, and realistically only about 100 are available.  But you may have multiple modules listening in on the same channel: for example, you might have a filter module, an envelope module, and an oscillator module.  They're by different vendors, and each of them can be programmed using CC messages.  How do you prevent them from accidentally overlapping?  You don't want to adjust your filter cutoff only to find that your oscillator tuning has changed as well...

### CC Learn

Some modules might avoid overlapping CCs using so-called **CC Learn**.  Here, you might press a "learn" button, and then send a CC message, and the module moves all of its CC pararameter numbers so that the lowest one is the same as that CC message.  This can work for simple devices but training many modules to avoid one another, and to avoid certain standardized CC message parameters, may be difficult.

### IDs

Other modules might avoid overlapping CCs by assigning each module a different **ID**.  An ID is just a number 1, 2, ..., 15, and each ID bestows on its module the right to use a certain range of CC parameters.  Let's call those parameters "a" through "i".  Your module's ID determines *which* CC parameters correspond to those "a" through "i".  That way modules with different IDs use different CC ranges.

The ["ID and CC Parameter Table"](#id-and-cc-parameter-table) below says which CC parameters belong which IDs.

Every module that uses CC has been assigned a **Default ID**.  The Default ID is its initial ID value.  Some simple modules cannot change this to a different ID, but more sophisticated ones can.  The Default ID is associated with the category of the module, as is shown in the ["Default ID Table"](#default-id-table). 

Some modules can have their IDs changed so that they don't conflict with one another, and this changes the CCs corresponding to the parameters "a" through "i".  In some situations, a module might even use 2 or more IDs.  If you are fortunate, you will never need to change the ID, and just leave it as the default.  

### Common CCs

Your modules may also respond to certain other CC functionality.  Modular MIDI supports:

- Bank Select (MSB and LSB)
- Modulation Wheel (MSB and LSB)
- Glide (MSB only)
- Volume (MSB only)
- Pan (MSB only)
- Expression Controller (MSB and LSB)
- Sustain Pedal
- Legato Switch
- MPE Timbre
- All Sounds Off
- Reset All Controllers
- All Notes Off

These CCs are sent to all modules, who are free to respond to them if they like.



<a name="auxiliary"/></a>
## Auxiliary Parameters and Additional IDs

Only 8 IDs have space allotted to them in the CC space.  But there is space for 7 more IDs in what we call the **Auxiliary Parameters**.  Auxiliary Parameters are set with CCs 3 and 35.  CC 3's value sets the auxiliary parameter number, and CC 35's value sets the auxiliary parameter value.  Send CC first, and then CC 35.

The ["Auxiliary Parameters Table"](#auxiliary-parameters-table) lists the auxiliary parameters for ach of IDs 9...15.  There are 16 parameters allotted to each, all 7-bit values. You can think of these as parameters "a" through "p".  Auxiliary Parameters are half the speed of regular CCs (because you need to send two CCs to do them).


<a name="nrpn"/></a>
## NRPN

If your module supports it, and your DAW or controller does, you can also be able to set up to 256 parameters in your module using NRPN.  NRPN is a bit slower than CC but it has much higher resolution and many more parameters.  The first 9 parameters in your NRPN region correspond to the CC parameters "a" through "i", if your ID is in the Auxiliary Parameter space, the first 16 parameters correspond to its parameters "a" through "p".  The ["NRPN Table"](#nrpn-table) lists which NRPN parameters belong to which ID.


<a name="loading"/></a>
## Loading and Saving Patches

Your module might support loading and saving patches.

Loading a patch is done in the usual MIDI way: via a Bank Select (both MSB and LSB), followed by a Program Change command.  The Bank Select selects your bank, and the Program Change selects the patch in the bank.

Modular MIDI also supports **Program Save**.  This is an Auxiliary Parameter, and its value indicates the patch slot to save your current parameters to.

Modular MIDI further supports **Current Program Save**.  This is an Auxiliary Parameter, CC 3 = 1, and CC 35 = 0.  It instructs the module to save its current parameters to the patch slot it is presently using.  

Finally, Modular MIDI supports **Current Program Revert**.  This is an Auxiliary Parameter, CC 3 = 1, and CC 35 = 1.  It instructs the module to reload its current parameters from the patch slot it is presently using.  

See the ["Saving and Loading Table"](#saving-and-loading-table) for information on these three commands.


<a name="modulation"/></a>
## Modulation CC Messages

Modules might not just respond to MIDI from your DAW or Controller.  They might also send messages to other modules to control them.  For example, you might have a sequencer module which sends MIDI notes to other modules to play them.

Another possibility is to have **modulator modules** sending messages to modulate **target modules**.  For example, you might have an LFO which is sending CC messages to modulate a Filter module.  

We have a scheme to make modulation simple.  A modulation module can send CCs to control any one of the first 8 parameters ("a" through "h") of your target module.  It doesn't need to know the ID of the module.  Instead it just sends it one of 8 **modulation CC messages**.

For example, your Filter might have "a" as its Cutoff and "b" as its Resonance.  Your LFO could have a knob on it where you set what it's modulating ("a" through "h").  If you set it to "a", and hook its MIDI MODULATION THRU to the Filter's MIDI IN, then the LFO will control the Filter Cutoff.

The first two modulation parameters are 14-bit high-resolution, and the rest are 7-bit low-resolution.

The ADSR's MIDI IN can also be connected to receive MIDI from your DAW; and it will pass that MIDI on to the Filter as well (as well as **injecting** the modulation CC messages necessary to control the Filter).

### Another Possibility

What if you also want to modulate your LFO from *another module*?  For example, imagine you wanted an ADSR to control the rate of your LFO, while the LFO controlled the cutoff of the Filter.  Your LFO might be able to do this.  Let's say the LFO rate is its parameter "d".  You set the ADSR to modulate "d" and attach it to the LFO.  The LFO is attached to the Filter as usual.  Attach the ADSR to receive MIDI from your DAW or controller, and it will pass it along to the LFO, which passes it on to the Filter.

Now you have to instruct the LFO not to pass modulation CCs it gets from the ADSR on to the Filter.  Then it should work.

### Yet Another Possibility

What if you want your ADSR *and* your LFO to *both* modulate your Filter?  For example, imagine you wanted an ADSR to control the filter resonance, while the LFO controls the filter cutoff.  

This is almost the same arrangement: the ADSR connects to the LFO, which connects to the Filter.  Your Filter's resonance is "b", so you set the ADSR to modulate "b".  But this time you have to instruct the LFO to *ignore and pass on* modulation CCs it gets (from the ADSR) to the Filter.


<a name="tables"/></a>
## Tables

### ID and CC Parameter Table

Below is a table, for each ID and each parameter a...i, of the CC parameter number assigned to that parameter.

Normally CC parameters are "7-bit", meaning that each CC can only be set to low-resolution values (0...127).  At its discretion a module has the option of bundling two CCs together to make a "14-bit" CC pair with much higher resolution.  When this is done, one CC is called the "Most Significant Byte" (or "MSB") of the parameter, and the other is called the "Least Significant Byte" (or "LSB").  The module has the option to bundle parameter "a" up with parameter "i". The combined parameter is just called "a", and "i" ceases to be available as a CC.  The module can additionally bundle "b" with "h" in a similar fashion.

Let's say that your module is ID 2, and it uses 14-bit b/i but not a/h.  Then parameter a is CC 14, parameter b is (14-bit) MSB 15, LSB 47, parameter c is 70, parameter d is 71, parameter e is 72, parameter f is 73, parameter g if 75, and parameter h is 47.  Your module documentation will say what the parameters a...h mean.

Some CCs have historic/traditional (and often obsolete) uses. You may find them in your DAW's list of CC names.  We list some of them in this table because certain IDs (see the ["Default ID Table"](#default-id-table)) have been lined up to conveniently correspond with these names if your module sees fit to use them.


ID  | Parameter   | 7-bit CC Parameter          | 14 bit CC Parameter MSB/LSB | Traditional Use
----|-------------|-----------------------------|-----------------------------|----------------
1   | a           | 8                           | 8 / 40                      |
1   | b           | 9                           | 9 / 41                      |
1   | c           | 65                          |                             | Glide Switch
1   | d           | 66                          |                             |
1   | e           | 67                          |                             |
1   | f           | 69                          |                             |
1   | g           | 112                         |                             |
1   | h           | 41 [only if "1 b" is 7-bit] |                             |
1   | i           | 40 [only if "1 a" is 7-bit] |                             |
2   | a           | 14                          | 14 / 46                     |
2   | b           | 15                          | 15 / 47                     |
2   | c           | 70                          |                             |
2   | d           | 71                          |                             |
2   | e           | 72                          |                             | Release
2   | f           | 73                          |                             | Attack
2   | g           | 75                          |                             | Decay
2   | h           | 47 [only if "2 b" is 7-bit] |                             |
2   | i           | 46 [only if "2 a" is 7-bit] |                             |
3   | a           | 16                          | 16 / 48                     |
3   | b           | 17                          | 17 / 49                     |
3   | c           | 76                          |                             | Vibrato Rate
3   | d           | 77                          |                             | Vibrato Depth
3   | e           | 78                          |                             | Vibrato Delay    
3   | f           | 79                          |                             |
3   | g           | 80                          |                             |
3   | h           | 49 [only if "3 b" is 7-bit] |                             |
3   | i           | 48 [only if "3 a" is 7-bit] |                             |
4   | a           | 18                          | 18 / 50                     |
4   | b           | 19                          | 19 / 51                     |
4   | c           | 81                          |                             |
4   | d           | 82                          |                             |
4   | e           | 83                          |                             |
4   | f           | 84                          |                             |
4   | g           | 85                          |                             |
4   | h           | 51 [only if "4 b" is 7-bit] |                             |
4   | i           | 50 [only if "4 a" is 7-bit] |                             |
5   | a           | 20                          | 20 / 52                     |
5   | b           | 21                          | 21 / 53                     |
5   | c           | 86                          |                             |
5   | d           | 87                          |                             |
5   | e           | 88                          |                             | High Res Velocity    
5   | f           | 89                          |                             |
5   | g           | 90                          |                             |
5   | h           | 53 [only if "5 b" is 7-bit] |                             |
5   | i           | 52 [only if "5 a" is 7-bit] |                             |
6   | a           | 12                          | 22 / 44                     | FX Control 1
6   | b           | 13                          | 23 / 45                     | FX Control 2
6   | c           | 91                          |                             | FX 1 Amount 
6   | d           | 92                          |                             | FX 2 Amount
6   | e           | 93                          |                             | FX 3 Amount  
6   | f           | 94                          |                             | FX 4 Amount
6   | g           | 95                          |                             | FX 5 Amount
6   | h           | 45 [only if "6 b" is 7-bit] |                             |
6   | i           | 44 [only if "6 a" is 7-bit] |                             |
7   | a           | 22                          | 24 / 54                     |
7   | b           | 23                          | 25 / 55                     |
7   | c           | 102                         |                             |
7   | d           | 103                         |                             |
7   | e           | 104                         |                             |
7   | f           | 105                         |                             |
7   | g           | 106                         |                             |
7   | h           | 55 [only if "7 b" is 7-bit] |                             |
7   | i           | 54 [only if "7 a" is 7-bit] |                             |
8   | a           | 24                          | 8 / 56                      |
8   | b           | 25                          | 9 / 57                      |
8   | c           | 107                         |                             |
8   | d           | 108                         |                             |
8   | e           | 109                         |                             |
8   | f           | 110                         |                             |
8   | g           | 111                         |                             |
8   | h           | 57 [only if "8 b" is 7-bit] |                             |
8   | i           | 56 [only if "8 a" is 7-bit] |                             |



### Default ID Table

Modules have a Default ID (or "Initial ID") corresponding to a category that they belong to.  You may be able to change this ID to something else.  The default IDs are:

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


### Auxiliary Parameters Table

IDs 9...15 get 16 parameters each in the Auxiliary Parameters space.  You can call them parameters "a" through "p".  Use CC 3 to set the parameter number, then use CC 35 to set its value.

CC 3 Value     | Auxiliary Parameters                      
---------------|-------------------------------
16-31          | First 16 Parameters for ID 9
32-47          | First 16 Parameters for ID 10
48-59          | First 16 Parameters for ID 11
64-79          | First 16 Parameters for ID 12
80-95          | First 16 Parameters for ID 13
96-111         | First 16 Parameters for ID 14
112-127        | First 16 Parameters for ID 15




### NRPN Table

Each ID gets 256 parameters, all 14-bit, in the NRPN space.  The first 9 parameters correspond to the parameters "a...i" in CC for IDs 1...8.  The first 16 parameters correspond to the parameters "a...p" in Auxiliary Parameters for IDs 9...15.

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


### Saving and Loading Table

CC 3 Value     | Auxiliary Parameter                      
---------------|-------------------------------
0              | Program Save (CC 35 = program number)         
1              | Current Program Save (CC 35 = 0)
1              | Current Program Revert (CC 35 = 1)


