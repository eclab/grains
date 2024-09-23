# Modular MIDI Conventions for Musicians

**Version 0.6**, September 22, 2024

Sean Luke, George Mason University (sean@cs.gmu.edu)

### Other Resources

<<<<<<< HEAD
[Modular MIDI Conventions for Musicians](ModularMIDIConventionsForMusicians.md).  A gentler introduction.  
[Module-Level MIDI](https://cs.gmu.edu/~sean/papers/modulelevelmidi.pdf).  An academic paper which introduces and describes the conventions.  
[Lightweight C MIDI Library](../midi). Designed for small processors, like Arduino.  Contains a Modular MIDI conventions sublibrary.  
[wonkystuff's Early Introductory Whitepaper](https://wonkystuff.net/midi-a-new-signal-type-for-ae/). This was written before the Modular MIDI conventions were set out but it still might be useful.  
[tangible waves's IMDI Page](https://www.tangiblewaves.com/imdi.html).  IMDI is tangible waves's brand name for the Modular MIDI convention.

## Table of Contents

This **tutorial** is set up in **stages**.  You'd only need to read up to the level of complexity you need for your setup.
=======
[Modular MIDI Conventions for Developers](https://github.com/eclab/grains/blob/main/info/ModularMIDIConventionsForDevelopers.md).  The spec proper.  
[Module-Level MIDI](https://cs.gmu.edu/~sean/papers/modulelevelmidi.pdf).  An academic paper which introduces and describes the conventions.  
[Lightweight C MIDI Library](https://github.com/eclab/grains/blob/main/midi/README.md). Designed for small processors, like Arduino.  Contains a Modular MIDI conventions sublibrary.  
[WonkyStuff's Early Introductory Whitepaper](https://wonkystuff.net/midi-a-new-signal-type-for-ae/). This was written before the Modular MIDI conventions were set out but it still might be useful.  
[Tangible Waves's IMDI Page](https://www.tangiblewaves.com/imdi.html).  IMDI is TangibleWaves's brand name for the Modular MIDI convention.

### Table of Contents
>>>>>>> e09e2aa223dc5294afcdcb5e1b413d40632435be

[Introduction](#introduction).  The point of Modular MIDI.  Where Modular MIDI came from.  Why you need it.  Or don't.    
[A MIDI Primer](#midiprimer).  In case you're not familiar.   

[Stage 0. I Just Need to Play a Note](#stage0) You don't need Modular MIDI yet.  
[Stage 1. Setting Up a Basic MIDI-Controlled Module](#stage1) How you hook MIDI-controlled modules up.  About Distribution Modules.  
[Stage 2. Setting Up Multiple Modules](#stage2) Getting more complicated.  
[Stage 3. Sending Parameters](#stage3) Dealing with the mess that is MIDI CC.  Also using RPN and NRPN.  And Auxiliary Parameters.  
[Stage 4. Modules that Control other Modules](#stage4)  Sequencers and such.  
[Stage 5. Polyphonic Modules](#stage5)  Options for sophisticated modules.  
[Stage 6. Saving and Loading Patches](#stage6)  Uploading and downloading.  

[Tables Appendix](#tables).  All the nitty-gritty at the end.  

An accompanying document, **Modular MIDI Conventions for Developers**, is a more terse description.

<a name="introduction"/></a>
## Introduction 

**This document discusses how to run MIDI over cables between the modules inside a modular synthesizer.**

MIDI is the most common protocol for modern electronic music devices to send messages to one another: notes, modulation information, etc.  But the modular synthesizer world, which predates MIDI, uses an earlier approach known as Control Voltage or CV.  MIDI has certain advantages (and disadvantages) compared to CV.  Modular MIDI won't replace CV.  Each has their place, and they play well together.

The problem is that MIDI isn't designed for the fine-grained nature of individual modules.  It was meant for large devices (entire synthesizers or computers or sequencers or keyboards) to send messages to other large devices.  So Modular MIDI is a set of conventions for MIDI to guarantee that multiple small modules can work together to respond to your MIDI messages in a variety of ways without messing each other up.  

Modular MIDI goes under different names.  Notably **IMDI** is Tangible Waves's brand name for their implementation.

### Do You Need Modular MIDI?

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


<a name="stage0"/></a>
## Stage 0.  I Just Need to Play a Gate/CV Note
Then you really don't need Modular MIDI.

Many people simply have a modular system where all modules respond to Gate/CV, and not to MIDI.  They just need some way to translate MIDI into Gate/CV so they can play one or more of their modules from their DAW or controller.  

To do this you'd typically use some kind of **MIDI to Gate/CV Interface**, such as AE Modular's **MASTER I/O.** Attach a keyboard controller, or DAW, or sequencer, or groovebox, to the interface to send MIDI notes to it. Then connect the Gate/CV output from the interface to the appropriate oscillators and envelopes, and you're basically done.  You might need to set the channel that each Gate/CV pair listens to.  

You could also use this to trigger drums. As certain notes arrive on MIDI, the interface sends an appropriate gate trigger out to play a certain drum on a drum module.

There's nothing wrong with this setup if it works for you. Modular MIDI is unlikely to help you here.

Where Modular MIDI comes into play is when you have modules which respond to MIDI directly.  Perhaps they use MIDI for stable pitch tuning or to receive parameter changes via MIDI CC rather than via CV; or maybe they are polyphonic or respond to MPE.  In these scenarios, a simple interface probably won't cut it.   

<a name="stage1"/></a>
## Stage 1.  Setting Up a Basic MIDI-Controlled Module 

Let's start with a simple **MIDI-controlled oscillator module.**  This module doesn't receive Gate/CV to play notes: it receives MIDI over ordinary modular cables.

To get MIDI to this module, you'll need a second module which takes normal MIDI and routes it to one or more MIDI-controlled modules.  This is called a **MIDI Distribution Module**.

### Wiring the Modules Up

Your MIDI-controlled module will have a **MIDI IN** port (using your modular cabling).  In most cases it will respond to any MIDI that is sent to it regardless of what channel the MIDI messages are being sent on.  

The MIDI Distribution Module will probably have multiple **MIDI OUT** ports, one for each channel, using your modular cabling. Pick the channel you're using, and connect that MIDI OUT port to the MIDI IN port on your MIDI-controlled module.

Now you need to get "standard" MIDI into your MIDI Distribution Module so it has something to distribute.  Your MIDI Distribution Module will probably accept MIDI via **5-Pin DIN**, or **MIDI TRS A or B**, or **USB**, or a combination of them.  Using one of these methods, attach it to your keyboard, controller, groovebox/sequencer, or DAW.

Now when you send MIDI from your keyboard (or whatever), it'll go the MIDI Distribution Module, which will then break it up by channel to various outputs, one of which is going to your MIDI-controlled module.  If you've got the module connected to the right port, it'll receive the MIDI and respond.

How do you set the channel in the MIDI-controlled module?  You don't.  **These modules typically just listen to every single channel.**  There's nothing to set.  MIDI calls this behavior **OMNI**.  But because it's only hooked up to the Channel 1 socket on the MIDI Distribution Module, your MIDI-controlled oscillator will only get messages sent on Channel 1 anyway.


<a name="stage2"/></a>
## Stage 2.  Setting Up Multiple Modules 

Many MIDI Distribution Modules will have many, perhaps 16, output ports, one for each channel, and can be configured to output MIDI to them in different ways.  We'll assume that here.

Let's consider different scenarios.

### Two Oscillators on the same voice (and thus the same MIDI channel)

Let's imagine you have not one MIDI-controlled Oscillators but two of them.  You want them to respond to the same MIDI notes (and thus the same MIDI channel) but be detuned with respect to one another: you'll mix them together to make a cool effect.

Set up the first oscillator like you did in Stage 1.

The first oscillator also will have a **MIDI THRU** port.  This port copies and sends out anything the oscillator receives on MIDI IN.  Connect the MIDI THRU port to the second oscillator's MIDI IN port.  Now both oscillators are listening to the same MIDI notes.

### Different Kinds of Modules working together on the same voice (and channel)

You might also have a MIDI-controlled oscillator and a **MIDI-controlled envelope** working together to play the same note, so they need to listen to the same MIDI.  You'd hook them up in exactly the same way as the two oscillators.

We call all the modules that work together to produce a single voice a **voice chain**, regardless of whether they're controlled by MIDI or CV.  For example: two oscillators put into a mixer, then pushed through a wave folder, a filter, and a VCA, all controlled by two envelope generators and an LFO: these modules are collectively a voice chain for one voice.


### Two Oscillators playing different sounds on different channels

Maybe you want to control the oscillators separately, with different sounds, and so on different channels.  Let's say they're channels 1 and 2.

Set up the first oscillator like you did in Stage 1.  [Let's say it's attached to the output for channel 1]

Now set up the second oscillator just like the first, only attach it to the output for channel 2 on the Distributor.

### Four Oscillators playing different notes (voices), all on the same channel

Your MIDI Distributor might be able to support **note distribution**.  Here, each new note comming in on a channel gets routed out a different MIDI OUT.  You'll need to set up your MIDI Distributor appropriately, then attach the four oscillators to four different MIDI Out ports on the Distributor.

### Four Oscillators playing different notes, all on the same channel, using MIDI Chaining

Certain MIDI-controlled oscillators might use **chaining** to distribute multiple notes on their own.  Here you'd set up oscillator 1 like you did in Stage 2, then connect its MIDI THRU to the MIDI IN of oscillator 2, then connect its MIDI THRU to the MIDI IN of oscillator 3, then connect *its* MIDI THRU to the MIDI IN of oscillator 4.

The idea is that when the first note comes in, oscillator 1 handles it.  But when the second note comes in, oscillator 1 will pass it on, so oscillator 2 will handle it, and so on.  There are a lot of gotchas and weaknesses here, but that's the general idea.

### The Whole Song: Four Oscillators using MIDI Polyphonic Expression (MPE) to play different notes (voices)

So you have a MPE device!  MPE is a scheme which overcomes certain weaknesses in playing multiple notes in MIDI.  It does this by assigning each note to its own channel.   It also sends some global information to one special "master" (or global) channel.

If your MIDI Distributor supports MPE, you need to set up the **MPE Zone** on your DAW or controller.  If you have 4 oscillators (for 4 notes), you'd probably set up the range to be the **MPE LOW Zone**.  This will use Channel 1 as the master channel, and send the four individual notes to Channels 2 through 5.  Now just hook up outputs 2 through 5 on the Distributor to your four oscillators.

### Four Oscillators on MIDI Polyphonic Expression (MPE), two oscillators playing together on another channel, and a third oscillator by itself on yet another channel!

All this can be mixed and matched -- after all, you have 16 channels.

* Set up the four MPE oscillators as described before.  They'll eat up channels 1 through 5.

* Attach one of the two "playing-together" oscillators to the output port for channel 6 on the Distributor.  Attach the MIDI IN of the second oscillator to the MIDI THRU of the first oscillator.

* Set the solo oscillator to output port 7 on the Distributor.

And so on!  You have a lot of flexibility.


<a name="stage3"/></a>
## Stage 3.  Sending Parameters 

CC, or **Channel Control**, is MIDI's basic way of remotely changing parameters in a device.  For example, if you wanted to change the filter cutoff, or the Attack for Envelope 3, you'd use CC to do it.

Each Channel has 128 parameters, called 0 through 127.  Each parameter can be set to one of 128 values (also called 0 through 127).  Some parameters have traditional functionality, such as sustain pedal or volume.  Others are free for devices to respond to any way they like.  For example, perhaps your synthesizer interprets CC 76 as Filter Cutoff, but another synthesizer interprets CC 76 as LFO Rate.

### About IDs

Let's assume that you want to send CC from your controller or DAW to change certain parameters of a MIDI-controlled oscillator in real-time.  This should be easy, but there's a gotcha. 

Imagine that you had a MIDI-controlled oscillator and also a MIDI-controlled envelope generator working together to play the same note, as described in Stage 2.  You send CC 35 to them, which the MIDI-controlled oscillator interprets as detune.  Great!  Unfortunately the MIDI-controlled envelope generator *interprets CC 35 as Decay Rate*, and modifies the Decay accordingly.

Ugh.

Modular MIDI gets around this by assigning each MIDI-controlled device its own region of CC so we don't have these conflicts.  To do this, we will **give each MIDI-controlled Device its own "ID"**.  There are 8 basic IDs.  

You can assign your device any ID you like, though  different categories of devices have different **default ID settings**, as shown in the **[Default ID Table](#defaultidtable)**.  In fact, very simple modules will be fixed to these defaults and can't be changed.

Each ID grants its module up to nine CC parameters.  Which CC parameters they are depends on the ID you set.  Certain of those CC parameters can be merged to form up to two "high resolution" (or so-called **14-bit**) CC parameters if the module so wishes.  We call these nine parameters "a" through "i".  If your module documentation doesn't say, consult the **[ID and CC Parameter Table](#idandccparametertable)** to determine which CC parameters correspond to "a" through "i" for your module's ID.

It's possible that your module doesn't support IDs at all, probably because it doesn't have any CCs, or because it uses some other scheme to avoid other modules.

It's also possible that your module requires **multiple IDs** because 9 parameters isn't enough.


### About NRPN

9 parameters isn't very much.  And the resolution of those parameters is very low (just 0...127).  And *also* even though you might have two high-resolution 14-bit CCs, very few controllers support 14-bit CC.  

What if your module needs **many** more than 9 parameters, or it needs many parameters with high resolution?  Enter **Non-Registered Parameter Numbers**, or **NPRN**.

NRPN is a MIDI scheme which gives each channel 16384 more parameters, and each parameter has a range of 0...16383.  It actually uses multiple CC messages internally, and so it is somewhat slower than CC.  But more importantly, its support among DAWs and controllers is limited.

Even so.  Each ID grants its module 256 parameters in the NRPN space.  For example, ID 1 grants its module the parameters through 511.  The first 9 of those parameters correspond exactly to the parameters "a" through "i".   See the **[NRPN Table](#nrpntable)** as to which NRPN parameters each ID affords.

Your module may support CC parameters but not NRPN parameters, at its discretion.

### About Auxiliary Parameters

Auxiliary Parameters are a "poor man's NRPN" invented for purposes of Modular MIDI.  They give you 7 more IDs (9 through 15), and each ID has 16 parameters ("a" through "p").  See the **[Auxiliary Parameters Table](#auxiliaryparameterstable)** for details.

You would primarily use Auxiliary Parameters (and IDs 9 through 15) when you have run out of IDs 1 through 8.  If you have more than 8 MIDI modules listening in on the same channel, you might start assigning the later ones to the Auxiliary space.

You set an Auxiliary Parameter by sending the parameter number in question as the value of CC 3.  Then you set the value of the parameter as the value of CC 35.

Auxiliary Parameters are low-resolution like CC: they can only be set to the values 0...127.

Depending on its ID, a module either has parameters in the CC space, or it has Auxiliary Parameters.  It doesn't have both.  But it can also have NRPN.  The first 16 Auxiliary parameters ("a" through "p") correspond to the first 16 parameters in its NRPN region.

Your module may or may not support Auxiliary parameters (and IDs 9 through 15) at its discretion.


### About RPN

RPN is a parameter scheme identical to NRPN, but reserved for official MIDI parameters.  Modular MIDI permits modules to receive RPN if they wish.

<a name="stage4"/></a>
## Stage 4.  Modules that Control other Modules 

Imagine that you had a MIDI-controlled Step Sequencer.  It received instructions via MIDI (perhaps to program it, or to step it).  But it **also controlled other MIDI modules**: it'd send the notes to play, or CC messages to respond to.

MIDI isn't designed for this.  It's meant for a single device to send messages to up to 16 other devices (one per channel), and that's it.  How can we have Device A send messages to Device B while both of them **also** receive messages from your DAW?

A module like the Step Sequencer is called a **MIDI Modulation Module**.  This module will, in addition to its standard MIDI THRU port, have a special port **MIDI MODULATION THRU** or **SOFT THRU** or some such name.

The standard MIDI THRU port just forwards the messages received at MIDI IN.  But the MIDI MODULATION THRU does the same thing, but **also** forwards additional messages that the MIDI Modulation Module has itself **injected** into the MIDI stream.

### Modulation CC Messages

The MIDI Modulation Module might just inject notes, or clock MIDI, etc.  But it could also inject CC messages meant to modulate the next module downstream.  For example, a MIDI-controlled Envelope Generator module might inject CC messages to control a downstream MIDI-controlled Filter module.

How is this done?  There are 8 special CC messages set aside just for this purpose.  MIDI-controlled modules that respond to CC or to NRPN or to Auxiliary Parameters should *also* respond to these special CC messages.  These 8 messages correspond to the first eight parameters, that is, "a" through "h".

All you have to do is attach your MIDI-controlled Filter module's MIDI IN port to the MIDI CONTROL THRU port of your MIDI-controlled Envelope Generator module.  Additionally, on the Envelope Generator there will be some kind of control to let you set which parameter it sends.  For example, your Filter might have assigned "a" to Filter Cutoff, so you'd set the Generator to "a" to change that.

And that's it!

If you don't want other modules responding to this special CC, don't connect them to the MIDI CONTROL THRU port, but just to the standard THRU port.

### Multiple-module modulation

It's possible that you might have, say, three modules, A, B, and C, and want A to modulate B while B is modulating C.  For example, you might have an LFO modulating a filter cutoff while an envelope generator is modulating the LFO's rate.  So you'd hook Module A's CONTROL THRU to the IN of module B as usual, and Module B's CONTROL THRU to the IN of module C.  

The important trixck here is that after B responds to A's injected modulation MIDI, it is expected to remove it from the stream before injecting its own and passing that on to C.   

On the other hand in some cases you might want module A *and* module B to *both* control module C.  This is easy: there ought to be a switch on Module B that instructs it to pass through modulation CC messages rather than respond to and remove them.  So there you go.



<a name="stage5"/></a>
## Stage 5.  Polyphonic Modules 

MIDI can send information for many notes at once.  This makes it useful for reducing the wiring necessary to control **polyphonic modules** which play many voices.

### Polyphony and Voice Chains

Many polyphonic modules replicate the same element N times for N voice chains.  For example, a 4-way polyphonic oscillator module or polyphonic filter module.  More sophisticated polyphonic modules might encorporate several elements, such as a module which provides 4 filters, VCAs, and envelope generators.

There's not reason why a polyphonic module can't work with many monophonic modules.  For example a polyphonic oscillator module might feed MIDI (or CV) and audio into N separate filter modules; or N separate filter modules might feed MIDI and audio into a polyphonic VCA.

### Polyphonic MIDI

There are three major ways that a polyphonic module can work with Modular MIDI.  

First, it might act as if it was N separate modules, and so attach itself to the Distribution Module with N separate MIDI wires into N separate MIDI IN (and THRU!).

Second, it might take over a whole channel and respond to all the notes played polyphonically on that channel.  Thus it might connect to the Distribution Module with a single MIDI cable: but you'd need to program it to respond to a specific channel.  In this case the polyphonic module really is free to do whatever it likes with regard to CC etc. -- it owns the entire channel.

Third, it might take over a whole MPE Zone (multiple channels) and respond to all the notes played polyphonically on that Zone.  Again, it'd only need a single MIDI Cable.  And again, it can now do whatever it likes with the channels on that Zone.

<a name="stage6"/></a>
## Stage 6.  Saving and Loading Patches 

Your module might support loading and saving patches.

Loading a patch is done in the usual MIDI way: via two Bank Select CC messages (CC 0 and CC 32), followed by a Program Change command.  The Bank Select CCs select your bank, and the Program Change selects the patch in the bank.

Modular MIDI also supports **Program Save**.  This is a special Auxiliary Parameter, and its value indicates the patch slot the module should save its current parameters to.

Modular MIDI further supports **Current Program Save**.  This is an Auxiliary Parameter, CC 3 = 1, and CC 35 = 0.  It instructs the module to save its current parameters to the patch slot it is presently using (or last saved to or loaded from).  

Finally, Modular MIDI supports **Current Program Revert**.  This is an Auxiliary Parameter, CC 3 = 1, and CC 35 = 1.  It instructs the module to reload its current parameters from the patch slot it is presently using (or last saved to or loaded from).  

It's entirely up to your module as to whether it supports one or more of these commands.  See the ["Saving and Loading Table"](#saving-and-loading-table) for information on these three commands.


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






