# Modular MIDI Conventions for Musicians

**A tutorial.**

**Document Version 0.7**, September 25, 2024  
**Modular MIDI Conventions Version 0.5**

Sean Luke, George Mason University (sean@cs.gmu.edu)

### Other Resources

[Modular MIDI Conventions for Developers](ModularMIDIConventionsForDevelopers.md).  Much more terse.   
[Module-Level MIDI](https://cs.gmu.edu/~sean/papers/modulelevelmidi.pdf).  An academic paper which introduces and describes the conventions.  
[Lightweight C MIDI Library](../midi). Designed for small processors, like Arduino.  Contains a Modular MIDI conventions sublibrary.  
[wonkystuff's Early Introductory Whitepaper](https://wonkystuff.net/midi-a-new-signal-type-for-ae/). This was written before the Modular MIDI conventions were set out but it still might be useful.  
[tangible waves's IMDI Page](https://www.tangiblewaves.com/imdi.html).  IMDI is tangible waves's brand name for the Modular MIDI conventions.

## Table of Contents

This **tutorial** is set up in **stages**.  You'd only need to read up to the level of complexity you need for your setup.

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

MIDI is the most common protocol for modern electronic music devices to send messages to one another.  But the modular synthesizer world, which predates MIDI, uses an earlier approach known as Control Voltage or CV.  **This document discusses how to run MIDI over cables between the modules inside a modular synthesizer, just like CV.**  This lets us take advantage of both protocols.  We don't intend Modular MIDI to replace CV.  They play well together.

Modular MIDI goes under different names.  Notably, **IMDI** is Tangible Waves's brand name for their implementation.

### Do You Need Modular MIDI?

Maybe not!  If you just want to play your modular synth monophonically from MIDI, you don't need any of this stuff: you can just use a MIDI to CV converter and be done with it.

### Early Modular MIDI

Modular MIDI is based on the design by John Tuffen (wonkystuff) for the modules he developed for the AE Modular system.  In his approach, MIDI entered the system via a MIDI port on a **Distributor Module** (such as the **wonkystuff mb/1** or **Tangible Waves MIDI Brain**), and then travelled from module to module using standard AE Modular sockets and cables, just like Gate/CV or Audio.  MIDI can be slow because each hop from module to module would go through an **optoisolator**, a slow electronic device.  Modular MIDI devices don't need this.

A Distributor Module such as the mb/1 divided incoming MIDI messages up by channel, where each channel had its own dedicated outgoing modular socket. MIDI clock messages were automatically converted to CV messages.  The mb/1 also provided one special socket which outputted a copy of all incoming MIDI.

Attached to the mb/1's per-channel sockets were modules which responded to MIDI.  For example, the **mco/1** was a simple oscillator which received MIDI NOTE ON/OFF messages and changed its pitch accordingly.  It also sent a gate out to notify envelopes etc. that a note had been received.  Finally, it responded to a small set of MIDI CC messages to change oscillator parameters. There were also modules which received MIDI and outputted CV values, drum triggers, etc.

The mco/1 had a "MIDI IN" port from which it received MIDI messages from the mb/1, and a "MIDI THRU" port which passed on those messages to other modules.  For example, this allowed two detuned oscillators to respond to the same note.

In fall of 2023, Tuffen, Mathias Brüssel (of tangible waves), and Sean Luke (George Mason University) began work in a standardized set of MIDI conventions to extend this model to more complex scenarios.  This eventually led to a paper, **["Module-Level MIDI"](https://cs.gmu.edu/~sean/papers/modulelevelmidi.pdf)**, presented at the Sound and Music Computing (SMC) conference in Summer of 2024.  Those conventions led to the ones you are reading here.

### Why MIDI?

Let's start with the **disadvantages** of MIDI.  First, **MIDI is slow** and can have **latency**.  Second, many MIDI parameters are **low resolution**.    Third, MIDI CC parameters are useful but they are **scarce**: there only about 100 of them.  If you have multiple MIDI modules, they might all want to use the same CC parameters, and you could easily run out of them.

If MIDI has so many problems, why bother with it?  Because its advantages are legion.

MIDI has **stable pitch tuning**.  You don't need to tune an oscillator: just tell it to play a note and it plays it.  MIDI also has facilities for **microtuning**.

MIDI only needs one cable to **control many parameters**.   In contrast, CV requires one cable for each parameter.  

MIDI enables **polyphony** and **multitimbralism**.  You can have multiple notes sent to an oscillator, not just one, and these notes can have different parameters  and control different kinds of sounds.

MIDI allows modules to **save and recall parameter information** (patches).

Practically every DAW, keyboard, controller, sequencer, groovebox, drum machine, and synthesizer in the world **speaks MIDI**.  This should not be discounted.

### Need for Modular MIDI Conventions

There's nothing wrong with just letting modules send MIDI to each other however they like.  So why do we need some conventions?  Because MIDI wasn't designed for some of the situations that arise inside a modular synthesizer.

- MIDI is designed for one keyboard controller, or DAW, to control one or more entire synthesizers.  It is not designed for individual *parts* of a synthesizer (modules, made by different vendors) to all listen to messages from a controller.

- MIDI is also not designed for multiple devices (such as modules) to send messages to control *each other*.

- Finally, modules vary greatly in sophistication.  Some might be simple monphonic oscillators, while others might be entire polyphonic synthesizers responding to multiple MIDI channels.  We need conventions that simple devices can implement *a little bit of*, while other devices can implement more.

<a name="midiprimer"/></a>
## A MIDI Primer

The Music Instrument Digital Interface (MIDI) was developed to allow a controller, or a keyboard, or a DAW, or a groovebox, to play another synthesizer remotely.  

MIDI is a very simple protocol: a **sender** (perhaps a DAW or controller) sends **messages** one at a time, which are heard by one or more **receviers** (such as synthesizers). It's one-way only: a receiver cannot respond to the sender. 

Some messages are **voiced messages**.  This means that they are tagged with a **channel** -- a number from 1 to 16 -- which indicates who among the various receivers is supposed to respond to that message.  This allows a sender to send different information to different receivers.  Usually a receiver is set up to respond to messages only on a single channel, but can also be set up to respond to all voiced messages regardless of channel: this is called being **OMNI**.

There are several kinds of voiced messages:

- NOTE ON messages and NOTE OFF messages instruct the receiver that a note has begun or ended.

- AFTERTOUCH or "PRESSURE" messages tell the receiver that currently pressed notes are being pressed *harder* (or less hard).

- PITCH BEND messages tell the receiver to bend the pitch of all playing notes.

- PROGRAM CHANGE or PC messages ask the receiver to load and use a new program (patch).

- CONTROL CHANGE or CC messages tell the receiver that it should change some parameter X to some new value Y.  What parameter X is, and what Y means, is defined by the receiver.  For example, the receiver may have documented that parameter X=14 changes its filter cutoff.  Think CC messages like virtual knobs remote-controlling your synthesizer.  They are roughly the equivalent of CV information.

  Normally CC messages are "7-bit", meaning that their values can only be in the range 0...127.  But MIDI has longstanding convention by whereby certain CC messages can be paired together to form a "14-bit" message which has a higher resolution range of 0...16383.  Unfortunately, this is not often supported by DAWs, though Novation controllers support it well. 

There are also a few **unvoiced** messages, that is, messages that *everyone* always listens to.  The most common such messages are **clock** messages to keep everyone in sync.  

There are a two other MIDI conventions you should be aware of.

- REGISTERED and NON-REGISTERED PROGRAM NUMBERS (RPN and NRPN).  The number of CC parameters is limited, and they have low resolution.  RPN and NRPN are schemes which use a special combination of CC messages to provide many more parameters, and also to greatly increase their resolution.  This fixes many problems, but RPN/NRPN are not well supported by DAWs and controllers, even though they've been around forever.  They are also somewhat slower than a CC message.

- MIDI POLYPHONIC EXPRESSION (MPE).  MIDI doesn't have per-note parameters.  For example, if you wanted to bend one note but not bend the others, too bad.  They'll all get bent.  MPE fixes this by assigning each note its own channel.  A receiver listens to not one but a range of channels, and plays the notes that arrive on them.  When a bend or CC message arrives on a channel, it only applies to the note currently on that channel.  This uses up channels, which are a precious commodity.  But MPE is now very common, and meshes well with Modular MIDI.

Okay, let's get started.

<a name="stage0"/></a>
## Stage 0.  I Just Need to Play a Gate/CV Note
Then you really don't need Modular MIDI.

Many people just have a modular system where all modules respond to Gate/CV.  They just need some way to translate MIDI into Gate/CV so they can play one or more of their modules from their DAW or controller.  

To do this you'd typically use some kind of **MIDI to Gate/CV Interface**, such as AE Modular's **MASTER I/O.** Attach a controller or DAW or whatnot to the interface to send MIDI notes to it. Make sure your controller is sending out on the channel the interface is listening to. Then connect the Gate/CV output from the interface to the appropriate oscillators and envelopes, and you're basically done. 

There's nothing wrong with this setup if it works for you.  Instead, Modular MIDI would be useful when you have modules which respond to MIDI directly, because they need more than simple CV signals to work their magic.

<a name="stage1"/></a>
## Stage 1.  Setting Up a Basic MIDI-Controlled Module 

In the simplest scenario, you need two modules:

- A **MIDI Distribution Module**.  This module takes external MIDI and breaks it up to send to other modules.  Examples include the wonkystuff mb/1 and the tangible waves MIDI Brain.

- A module which responds to MIDI.  Let's start with a simple **MIDI-controlled oscillator module.**  This module doesn't receive Gate/CV to play notes: just MIDI.  And it doesn't get its MIDI in the classic way: it receives MIDI over modular cables from other modules (likely the Distribution Module).

### Wiring the Modules Up

First, let's hook up the MIDI Distribution Module, so it has something to distribute.  Your MIDI Distribution Module will probably accept MIDI via **5-Pin DIN**, or **MIDI TRS A or B**, or **USB**.  Using one of these sockets, attach it to your keyboard, controller, groovebox/sequencer, or DAW.

The MIDI Distribution Module will probably have one **MIDI OUT** port for each channel (16 of them!). Each MIDI OUT port uses normal modular cabling. Your MIDI-controlled oscillator will have a **MIDI IN** port to receive MIDI.  Pick the MIDI OUT port for the channel you're using, and connect it to the oscillator's MIDI IN port.

When you send MIDI from your keyboard (or DAW or whatever), it'll go into the MIDI Distribution Module, which will then break it up by channel to various outputs.  One of those outputs is connected to your MIDI-controlled module, which receives the MIDI and responds to it.

How do you set the channel in the MIDI-controlled module?  You don't.  **These modules typically just listen to every single channel.**  There's nothing to set.  MIDI calls this behavior **OMNI**.  But because it's only hooked up to the Channel 1 socket on the MIDI Distribution Module, your MIDI-controlled oscillator will only get messages sent on Channel 1.


<a name="stage2"/></a>
## Stage 2.  Setting Up Multiple Modules 

This is where the fun begins.  Your MIDI-controlled oscillator will likely have a special socket called **MIDI THRU**.  This socket emits an exact copy of the MIDI received on its MIDI IN socket.  You can use this to chain *another* module to your oscillator so they both respond to MIDI (if they're listening in on the same channel).  And so on.  Alternatively, if the other module needs to listen to a different channel, connect it to the appropriate socket on the Distributor.

This leads to a lot of interesting configurations.

### Two Oscillators on the same MIDI channel

You now have not one MIDI-controlled Oscillator but two of them, detuned.  You want them to respond to the same MIDI notes (thus the same MIDI channel).  You'll mix them together to make a cool effect.

Set up the first oscillator like you did in [Stage 1](#stage1).  Now just connect the second oscillator's MIDI IN to the first oscillator's MIDI THRU.  And you're done!

<a name="different"/></a>
### Different kinds of modules working together to make a sound

You might also have a MIDI-controlled oscillator and a **MIDI-controlled envelope generator**.  The envelope generator controls the filter that the oscillator is connected to.  Both the oscillator and the envelope generator need to be informed of the same MIDI note messages.

This is easy: hook them up just like you did the two MIDI-controlled oscilators.

We call all the modules that work together to produce a single voice a **voice chain**, regardless of whether they're controlled by MIDI or CV.  For example: two oscillators put into a mixer, then pushed through a wave folder, a filter, and a VCA, all controlled by two envelope generators and an LFO: these modules are collectively a voice chain for one voice.

### Two oscillators playing different notes on different channels

Maybe you want to control the oscillators separately, with different sounds, and so on different channels.  Let's say they're channels 1 and 2.

Set up the first oscillator like you did in [Stage 1](#stage1).  Let's say it's attached to the output for channel 1.  Now set up the second oscillator just like the first, only attach it to the output for channel 2 on the Distributor.

### Four oscillators playing different notes, all on the same channel, using note distribution

Your MIDI Distributor might be able to support **note distribution**.  Here, each new note ariving on a channel gets intelligently routed out a *different MIDI OUT*.  You'll need to set up your MIDI Distributor to route to four sockets, then attch the oscillators to those sockets.

### Four oscillators playing different notes, all on the same channel, using chaining

Certain MIDI-controlled oscillators might use **chaining** to distribute multiple notes on their own.  The idea is that when an oscillator receives a note on its MIDI IN, and it's already playing another note, it hands off that note to the next oscillator (and so on).

To do this, your oscillators will need to have a special socket called a **MIDI SOFT THRU.**  This THRU doesn't necessarily copy everything from the MIDI IN: in this case, it'll remove notes that the oscillator is already handling so downstream oscillators don't play them too. 

You'd set up oscillator 1 like you did in [Stage 1](#stage1), then connect its MIDI SOFT THRU to the MIDI IN of oscillator 2, then connect its MIDI SOFT THRU to the MIDI IN of oscillator 3, then connect *its* MIDI SOFT THRU to the MIDI IN of oscillator 4.

### Four oscillators playing different notes, using MIDI Polyphonic Expression (MPE)

So you have a MPE controller!  MPE is a scheme which overcomes certain weaknesses in playing multiple notes in MIDI.  It does this by assigning each note to its own channel.  It also sends some global information to a special "master channel".

If your MIDI Distributor supports MPE, you'll need to set up the **MPE Zone** on your DAW or controller.  You'd probably set up the **MPE LOW Zone**.  This will use Channel 1 as the master channel, and send the four individual notes to Channels 2 through 5 respectively.  Now just hook up outputs 2 through 5 on the Distributor to your four oscillators.

### Mixing and matching

All this can be mixed and matched -- after all, you have 16 channels.  You could set up MPE on the sockets for channels 1 through 5, set up a different oscillator on the socket for channel 6, and up two "playing together" oscillators on the socket for channel 7, say.

And so on!  You have a lot of flexibility.


<a name="stage3"/></a>
## Stage 3.  Sending Parameters 

CC, or **Channel Control**, is MIDI's basic way of remotely changing parameters on a device.  For example, if you wanted to change the filter cutoff, or the attack for Envelope 3, you'd use CC to do it.

Each Channel has 128 CC parameters, called parameters 0 through 127.  Each CC parameter can be set to one of 128 values (also 0 through 127).  Some parameters have dedicated or traditional functions, such as sustain pedal or volume.  Others are available for devices to respond to any way they like.  Perhaps your synthesizer interprets CC 76 as Filter Cutoff, but another synthesizer interprets CC 76 as LFO Rate.

### About IDs

Suppose that you had a MIDI-controlled oscillator and *also* a MIDI-controlled envelope generator working together to play the same note, as described in [Stage 2](#different).  You want to send CC 35 from your DAW to change a parameter on the oscillator, which it interprets as, oh, detune.  But there's a gotcha. Because  the MIDI-controlled envelope generator *interprets CC 35 as Decay Rate*.  So when you send CC 35, *they both change their parameters*.

Ugh.

MIDI wasn't designed for CCs to be sent to individual modules, so they can step on each other's toes like this.  Modular MIDI fixes this by gives each MIDI-controlled device its own personal region of CC parameters. To do this, it **assigns each MIDI-controlled Device its own "ID"**.  There are 15 IDs avaialable, but usually you'd deal with the first 8.  

You can assign a module any ID you like, but various module categories have **default ID settings**, as shown in the **[Default ID Table](#defaultidtable)**.  For example, oscillators are by default ID 1.  In fact, very simple modules might be fixed to their defaults and unable to be changed.

An ID grants its module its own region of nine CC parameters, and different IDs have different parameter regions.  We call these nine parameters "a" through "i".   These CC parameters are all "7-bit" parameters: but one or two pairs of CCs may be merged to form up to two "14-bit" high-resolution CCs if the module so wishes.
If your module documentation doesn't say, consult the **[ID and CC Parameter Table](#idandccparametertable)** to determine which CC parameters correspond to "a" through "i" for your module's assigned ID.

It's possible that your module doesn't support IDs at all, probably because it doesn't have any CCs, or because it uses some other scheme to avoid other modules.

It's also possible that your module requires **multiple IDs** because 9 parameters isn't enough.  This is permissable.


### About NRPN

9 parameters isn't very much.  And the resolution of most of those parameters is very low (just 0...127).  And *also* even though you might have two high-resolution 14-bit CCs, very few controllers support 14-bit CC.  

What if your module needs **many** more than 9 parameters, or it needs many parameters with high resolution?  Enter **Non-Registered Parameter Numbers**, or **NPRN**.

NRPN is a MIDI scheme which gives each channel 16384 more parameters, and each parameter has a range of 0...16383.  It actually uses multiple CC messages internally, and so it is somewhat slower than CC.  But more importantly, its support among DAWs and controllers is limited (though better than 14-bit CC support).

Even so.  Each ID grants its module 256 NRPN parameters of its own.  For example, ID 1 grants its module the NRPN parameters 256 through 511.  The first 9 of those parameters correspond exactly to the CC parameters "a" through "i".   See the **[NRPN Table](#nrpntable)** as to which NRPN parameters each ID affords.

Your module may support CC parameters but not NRPN parameters, at its discretion.

### About Auxiliary Parameters

Auxiliary Parameters are a "poor man's NRPN" invented for purposes of Modular MIDI.  They give you 7 more IDs (9 through 15), and each ID is granted 16 parameters ("a" through "p"), with values 0..127.  See the **[Auxiliary Parameters Table](#auxiliaryparameterstable)** for details.

You would primarily use Auxiliary Parameters (and IDs 9 through 15) when you have run out of IDs 1 through 8.  If you have more than 8 MIDI modules listening in on the same channel, you might start assigning the later ones to the Auxiliary space.

You set an Auxiliary Parameter by sending the parameter number in question as the value of CC 3.  Then you set the value of the parameter as the value of CC 35.  For example, if you wanted to set auxiliary parameter 92 to the value 24, you'd set CC 3 to be 92, then CC 35 to be 24.

Depending on its ID, a module either has parameters in the CC space, or it has Auxiliary Parameters.  It doesn't have both.  But it can also have NRPN in both cases.  The first 16 Auxiliary parameters ("a" through "p") correspond to the first 16 parameters in its NRPN region.

Your module may or may not support Auxiliary parameters (and IDs 9 through 15) at its discretion.


### About RPN

RPN is a parameter scheme identical to NRPN, but reserved for official MIDI parameters.  Modular MIDI permits modules to receive RPN if they wish.

<a name="stage4"/></a>
## Stage 4.  Modules that Control other Modules 

Imagine that you had a MIDI-controlled Step Sequencer.  It receives instructions via MIDI (perhaps to program it, or to step it).  But it **also controls other MIDI modules**: it'd send the notes for them to play, or CC messages for them to respond to.  And while it's doing it, perhaps you'd like to remotely control parameters on those modules.

MIDI isn't designed for this.  It's meant for a single device to send messages to up to 16 other devices, and that's it.  It's not meant for a device to be both a sender and receiver.  And how could we have Device A send messages to Device B while both of them **also** receive messages from your DAW?

We call a module like the Step Sequencer a **MIDI Modulation Module**.  This module will, in addition to its standard MIDI THRU port, have a special **MIDI SOFT THRU** port. The standard MIDI THRU port just forwards the messages received at MIDI IN.  But the MIDI SOFT THRU does the same thing, but **also** might remove messages or forward additional messages that the MIDI Modulation Module has itself **injected** into the MIDI stream.

### Modulation CC Messages

The MIDI Modulation Module could just inject notes, or clock MIDI, etc.  But it very often will need to inject CC messages meant to modulate the next module downstream.  For example, a MIDI-controlled Envelope Generator module might inject CC messages to control a downstream MIDI-controlled Filter module.  

This is too complicated: you'd have to set exactly which of the 128 CC messages on both the modulation module and its downstream target module.  But Modular MIDI has a simple solution.

In Modular MIDI there are 8 special CC messages set aside just for this purpose.  MIDI-controlled modules that respond to CC or to NRPN or to Auxiliary Parameters should *also* respond to these special CC messages.  These 8 messages correspond to the first eight parameters, that is, "a" through "h".

On the Envelope Generator there will be some kind of control to let you set which parameter it sends (a through h).  For example, your Filter might have designated "a" to be Filter Cutoff, so to modulate Filter Cutoff, you'd set the Envelope Generator to output "a".  The attach your MIDI-controlled Filter module's MIDI IN port to the MIDI SOFT THRU port of your MIDI-controlled Envelope Generator module.  

And that's it!

If you don't want other modules responding to this special CC, don't connect them to the MIDI SOFT THRU port, but instead to the standard THRU port.

### Multiple-module modulation

It's possible that you might have, say, three modules, A, B, and C, and want A to modulate B while B is simultaneously modulating C.  For example, you might have an LFO modulating a filter cutoff while an envelope generator is modulating the LFO's rate.  So you'd hook Module A's SOFT THRU to the IN of module B as usual, and Module B's SOFT THRU to the IN of module C.  

This should work because after B responds to A's injected modulation MIDI, it is normally expected to remove that MIDI from the stream before injecting its own and passing that on to C.  That way C doesn't get A's injected modulation MIDI as well.

But what if you want module A *and* module B to *both* control module C?  To do this, there should be a switch on Module B that instructs it to ignore modulation messages from A but instead pass them straight through to C (injecting is own messages as well).


<a name="stage5"/></a>
## Stage 5.  Polyphonic Modules 

MIDI can send information for many notes at once.  This makes it useful for reducing the wiring necessary to control **polyphonic modules** which play many voices.

### Polyphony and Voice Chains

Many polyphonic modules replicate the same element N times for N voice chains.  For example, a 4-way polyphonic oscillator module or polyphonic filter module.  More sophisticated polyphonic modules might encorporate several elements, such as a module which provides 4 filters, VCAs, and envelope generators.

There's not reason why a polyphonic MIDI module can't work with many monophonic MIDI modules.  For example a polyphonic oscillator module might feed both MIDI (or CV) and audio into N separate filter modules; or N separate filter modules might feed MIDI and audio into a single polyphonic VCA.

### Polyphonic MIDI

There are four major ways that a polyphonic module can work with Modular MIDI.  

- It might act as if it were N separate modules, and so attach itself to the Distribution Module with N separate MIDI cables into N separate MIDI IN (and THRU!).

- It might act as if it were N separate modules, but attach to the Distribution Module's MIDI THRU with a single MIDI cable.  As MIDI THRU sends all messages regardless of channel, you'd need to be able to set which channels the polyphonic module is listening to.

- It might take over a whole channel and respond to all the notes played polyphonically on that channel.  Here you'd connect it to the Distribution Module with a single MIDI cable as  usual.  Because it has taken over a whole channel, the polyphonic module doesn't need Modular MIDI's CC or NRPN rules: it could do as it pleased (though perhaps it'd be nice if it followed them to be consistent).

- It might take over a whole MPE Zone (multiple channels) and respond to all the notes played polyphonically on that Zone.  You could connect it with multiple cables, but it'd be more convenient to connect a single cable to the Distribution Module's MIDI THRU and manually set up which channels are in the Zone.  And again, it can now do whatever it likes with the channels on that Zone.

<a name="stage6"/></a>
## Stage 6.  Saving and Loading Patches 

Your module might support loading and saving patches.

Loading a patch is done in the usual MIDI way: via two Bank Select CC messages in order (CC 0 and CC 32), followed by a Program Change command.  The Bank Select CCs select your bank (the bank number is [CC0 value] x 128 + [CC 32 value]), and the Program Change selects which patch in the bank (0..127).

Modular MIDI also supports **Program Save**.  This is a special Auxiliary Parameter, and its value indicates the patch slot the module should save its current parameters to.

Modular MIDI further supports **Current Program Save**.  This is an Auxiliary Parameter, CC 3 = 1, and CC 35 = 0.  It instructs the module to save its current parameters to the patch slot it is presently using (or last saved to or loaded from).  

Finally, Modular MIDI supports **Current Program Revert**.  This is an Auxiliary Parameter, CC 3 = 1, and CC 35 = 1.  It instructs the module to reload its current parameters from the patch slot it is presently using (or last saved to or loaded from).  

It's entirely up to your module as to whether it supports one or more of these commands.  See the **[Saving and Loading Table](#savingandloadingtable)** for information on these three commands.


<a name="tables"/></a>
## Tables

<a name="idandccparametertable"/></a>
### ID and CC Parameter Table

Below is a table, for each ID and each parameter a...i, of the CC parameter number assigned to that parameter.

Normally CC parameters are "7-bit", meaning that each CC can only be set to low-resolution values (0...127).  At its discretion a module has the option of bundling certain pairs of CCs together to make a "14-bit" CC pair with much higher resolution.  When this is done, one CC is called the "Most Significant Byte" (or "MSB") of the parameter, and the other is called the "Least Significant Byte" (or "LSB").  

The first 14-bit bundle are parameters "b" and "h", and the combined parameter is just called "b", and "h" ceases to be available as a CC.  The module can additionally a second bundle, "a" and "i", and the resulting 14-bit parameter is called "b".

Let's say that your module is ID 2, and it uses the first 14-bit bundle (b/i), but doesn't additionally bundle a/h.  Then parameter a is CC 14, parameter b consists of (MSB) CC 15 and (LSB) CC 47, parameter c is 70, parameter d is 71, parameter e is 72, parameter f is 73, parameter g if 75, and parameter h is 47.  Your module documentation will say what the parameters a...h actually do.

Some CCs have historic/traditional (and often obsolete) uses. You may find them in your DAW's list of CC names.  We list some of them in this table because certain IDs (see the **[Default ID Table](#default-id-table)** have been lined up to conveniently correspond with these names if your module sees fit to use them.  For example, we have made sure that the Traditional Release, Attack, and Decay CCs (71, 72, and 73) fall within the ID range meant for Envelopes.


ID  | Parameter   | 7-bit CC Parameter          | 14 bit CC Parameter MSB / LSB | Traditional Use
----|-------------|-----------------------------|-------------------------------|----------------  
1   | a           | 8                           | 8 / 40                        |
1   | b           | 9                           | 9 / 41                        |
1   | c           | 65                          |                               | Glide Switch
1   | d           | 66                          |                               |
1   | e           | 67                          |                               |
1   | f           | 69                          |                               |
1   | g           | 112                         |                               |
1   | h           | 41 [only if "1 b" is 7-bit] |                               |
1   | i           | 40 [only if "1 a" is 7-bit] |                               |
2   | a           | 14                          | 14 / 46                       |
2   | b           | 15                          | 15 / 47                       |
2   | c           | 70                          |                               |
2   | d           | 71                          |                               |
2   | e           | 72                          |                               | Release
2   | f           | 73                          |                               | Attack
2   | g           | 75                          |                               | Decay
2   | h           | 47 [only if "2 b" is 7-bit] |                               |
2   | i           | 46 [only if "2 a" is 7-bit] |                               |
3   | a           | 16                          | 16 / 48                       |
3   | b           | 17                          | 17 / 49                       |
3   | c           | 76                          |                               | Vibrato Rate
3   | d           | 77                          |                               | Vibrato Depth
3   | e           | 78                          |                               | Vibrato Delay    
3   | f           | 79                          |                               |
3   | g           | 80                          |                               |
3   | h           | 49 [only if "3 b" is 7-bit] |                               |
3   | i           | 48 [only if "3 a" is 7-bit] |                               |
4   | a           | 18                          | 18 / 50                       |
4   | b           | 19                          | 19 / 51                       |
4   | c           | 81                          |                               |
4   | d           | 82                          |                               |
4   | e           | 83                          |                               |
4   | f           | 84                          |                               |
4   | g           | 85                          |                               |
4   | h           | 51 [only if "4 b" is 7-bit] |                               |
4   | i           | 50 [only if "4 a" is 7-bit] |                               |
5   | a           | 20                          | 20 / 52                       |
5   | b           | 21                          | 21 / 53                       |
5   | c           | 86                          |                               |
5   | d           | 87                          |                               |
5   | e           | 88                          |                               | High Res Velocity    
5   | f           | 89                          |                               |
5   | g           | 90                          |                               |
5   | h           | 53 [only if "5 b" is 7-bit] |                               |
5   | i           | 52 [only if "5 a" is 7-bit] |                               |
6   | a           | 12                          | 22 / 44                       | FX Control 1
6   | b           | 13                          | 23 / 45                       | FX Control 2
6   | c           | 91                          |                               | FX 1 Amount 
6   | d           | 92                          |                               | FX 2 Amount
6   | e           | 93                          |                               | FX 3 Amount  
6   | f           | 94                          |                               | FX 4 Amount
6   | g           | 95                          |                               | FX 5 Amount
6   | h           | 45 [only if "6 b" is 7-bit] |                               |
6   | i           | 44 [only if "6 a" is 7-bit] |                               |
7   | a           | 22                          | 24 / 54                       |
7   | b           | 23                          | 25 / 55                       |
7   | c           | 102                         |                               |
7   | d           | 103                         |                               |
7   | e           | 104                         |                               |
7   | f           | 105                         |                               |
7   | g           | 106                         |                               |
7   | h           | 55 [only if "7 b" is 7-bit] |                               |
7   | i           | 54 [only if "7 a" is 7-bit] |                               |
8   | a           | 24                          | 8 / 56                        |
8   | b           | 25                          | 9 / 57                        |
8   | c           | 107                         |                               |
8   | d           | 108                         |                               |
8   | e           | 109                         |                               |
8   | f           | 110                         |                               |
8   | g           | 111                         |                               |
8   | h           | 57 [only if "8 b" is 7-bit] |                               |
8   | i           | 56 [only if "8 a" is 7-bit] |                               |



<a name="defaultidtable"/></a>
### Default ID Table

Modules have a Default ID (or "Initial ID") corresponding to a category of work that they fall under.  You may be able to change this ID to something else if you have two conflicting modules in the same voice chain.  The default IDs are:

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


<a name="auxiliaryparameterstable"/></a>
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




<a name="nrpntable"/></a>
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


<a name="savingandloadingtable"/></a>
### Saving and Loading Table

Certain Auxiliary Parameters do special tasks if your module supports it. Send CC 3 first, then CC 35. You may wish to set the Bank (using CC 0 and CC 32 as usual) prior to Program Save.

CC 3 Value     | CC 35 Value    | Auxiliary Parameter                      
---------------|----------------|-----------------------
0              | Program Number | Program Save       
1              | 0              | Current Program Save
1              | 1              | Current Program Revert





