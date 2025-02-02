# Modular MIDI Conventions for Developers

**Document Version 0.9**, December 20, 2024  
**Modular MIDI Conventions Version 0.5**

- Sean Luke, George Mason University (sean@cs.gmu.edu)
- John Tuffen, wonkystuff (john@wonkystuff.net)
- Mathias Brüssel, tangible waves (mathias@tangiblewaves.com)

### Other Resources

[Modular MIDI Conventions for Musicians](https://github.com/eclab/grains/blob/main/info/ModularMIDIConventionsForMusicians.md).  A gentler introduction.  
[Module-Level MIDI](https://cs.gmu.edu/~sean/papers/modulelevelmidi.pdf).  An academic paper which introduces and describes the conventions.  
[Lightweight C MIDI Library](https://github.com/eclab/grains/blob/main/midi/README.md). Designed for small processors, like Arduino.  Contains a Modular MIDI conventions sublibrary.  
[WonkyStuff's Early Introductory Whitepaper](https://wonkystuff.net/midi-a-new-signal-type-for-ae/). This was written before the Modular MIDI conventions were set out but it still might be useful.  
[Tangible Waves's IMDI Page](https://www.tangiblewaves.com/imdi.html).  IMDI is TangibleWaves's brand name for the Modular MIDI conventions.

### Table of Contents

[Introduction](#introduction)  
[Additions to Standard MIDI](#additions)   
[Topology and Data Format](#topology)  
[Namespaces, IDs, and CC Messages](#namespaces)  
[MIDI Injection, Program Load/Save, and System Exclusive](#injection) 

An accompanying document, **Modular MIDI Conventions for Musicians**, is a more gentle introduction for users.

<a name="introduction"/></a>
## Introduction

This document describes additions to or restrictions on MIDI intended to help modules inside a modular synthesizer talk to one another over patch cables using MIDI.  It is not meant to replace Gate/CV, and can work side-by-side with it.

MIDI was designed for large, monolithic devices to control other large, monolithic devices.  It was not designed for the finer-grained scenario of modules controlling one another, or being all controlled by a DAW or controller.  These conventions try to remedy this.

This document assumes that you are familiar with MIDI, and does not explain MIDI concepts.


<a name="additions"/></a>
## Additions to Standard MIDI

Modular MIDI only adds conventions to standard MIDI.  It does not break or significantly deviate from any MIDI standards, and everything below can be implemented as standard non-Sysex MIDI messages.  Here are the major differences:

- **Hardware.**  Modular MIDI runs over a new transport: the cabling between modules in a modular synthesizer.  In AE Modular, it presently runs at 31250 bps over plain TTL serial.  Unlike traditional MIDI, it does not require optoisolation as all the devices share common ground and power.  Hardware MIDI THRU can be achieved with a simple op-amp.

- **CC Parameters.**  Modular MIDI divides the CC space into regions assigned to **IDs**.  Each module listening in on a given channel is given one or more unique IDs, and controls the CC space for its ID.  This is a conflict avoidance mechanism.  It is not a hard-and-fast rule: if a module has complete control over a channel, it can do as it pleases.  Modular MIDI also supports many, but not all, of the standard MIDI CC parameters.

- **NRPN Parameters.**  Modular MIDI by convention also divides the NRPN space into regions assigned to **IDs**.

- **Auxiliary Parameters.**  Modular MIDI reserves CCs 3 and 35 for what it calls *Auxiliary Parameters*. This is nothing more than a poor-man's NRPN, a way to extend the CC space.  There are 128 Auxiliary Parameters. You set the parameter with CC 3, and then you set its value with CC 35.

- **Modulation CCs and Injection.**  Modular MIDI allows module A to receive MIDI and pass it on to module B, but to also simultaneously *inject* MIDI messages into the stream sent to B in order for A to modulate B itself.  Messages may also be removed from the stream, or replaced.  The most common parameters to inject are eight general CCs called *Modulation CCs*, meant to make it easy for a modulation module to modulate a target module (such as an ADSR modulating a Filter).

- **Patch Manipulation.**  Standard MIDI has Program Change and Bank Select to load new patches.  Modular MIDI extends this with a few new directives (as Auxiliary Parameters): *Program Save*, *Current Program Save*, and *Current Program Revert*.  Modular MIDI also has a proposed redefinition of the Bank Select MSB as *Module Select*, allowing one to specify *which* module (by ID) should have its patches loaded, saved, reverted, etc.

- **MIDI Polyphonic Expression (MPE) and MIDI Mode 4**  Modular MIDI can be made compatible with MIDI Mode 4 trivially, and is largely compatible with MPE with some work.
 
<a name="topology"/></a>
## Topology and Data Format

There are no requirements placed on hardware transport or speed, except that a given modular platform should standardize on one approach, and that the speed is recommended to be greater than or equal to 31250 BPS.  We provide the example below illustrating usage on the AE Modular system.  We then make general topological and organizational recommendations.

### Example Transport Hardware: AE Modular

In the AE Modular system, MIDI arrives to the modular synthesizer via standard hardware: either a 5-pin DIN MIDI jack, a TRS MIDI jack, or USB.  There are special rules regarding MIDI electronics which must be followed (such as optoisolation).

Once inside the system, MIDI is transferred from module to module as a simple 5V TTL Serial connection between two standard sockets on the modules.  There is no optoisolation.

### Speed

Within a modular system, all modules should send MIDI at the same speed.  This speed should be at least 31250 bits per second, though it can be faster.

### How Modules are Connected

We presume that in most cases, a modular setup would have a **distributor module**, which receives external MIDI, breaks it up it per-channel, and outputs messages for each channel via per channel AE modular sockets.  Non-voiced data (like clock) is ideally copied to every channel socket.  Attached to each socket is a chain of one more **receiever modules** meant to respond to messages on that channel.  These receiver modules typically are set up to be OMNI.  A distributor module should also output *all* its incoming MIDI via a general-purpose **THRU**.  The distributor module is not necessarily the only module which emits MIDI, either passed through a **THRU** or generated independently.

* Every receiver module should have at least one (and usually only one) **IN** socket from which it receives MIDI data.  It is recommended that the distributor also have an **IN** socket, to receive data instead of its external MIDI socket, in a switchable manner.

* Generally a receiver should have at least one **THRU** socket where the data from the **IN** socket is passed through unmolested.  This should be done by simply buffering the IN signal via an op-amp and re-broadcasting it out THRU. Buffering would have essentially no latency.  THRU allows multiple modules to easily be chained together to listen to the same channel.

* A receiver can receive multiple channels if it's designed to do that, though this is unusual.  It could do this via multiple incoming **IN** sockets, one per channel, or by attaching to the distributor's THRU to receive all MIDI, and sifting through it on its own.

* A receiver can also have a special **Software THRU** where it emits the MIDI data from IN as usual, but *also* emits messages it has **injected** into the MIDI stream.  This allows it to send messages of its own to downstream receivers.  For example, a MIDI-controlled envelope module might inject envelope modulation to a downstream filter, while both of them are receiving note information from the DAW.  Note that Software THRU is likely to incur some degree of latency.  A receiver that has a Software THRU for injection should also have a standard (hardware) THRU as well.

* A module can also output gate, CV, audio, anything it likes.  And it can respond to them as well.  MIDI does not preclude these in any way.

* It would be unusual -- and almost always an error -- to have a cycle in the MIDI connections.  That is, you are unlikely to see a situation where module A is connected to B, which is connected to C, which is then connected to A again.

* It is certainly possible for a MIDI receiver to emit MIDI directly.  For example, if you had a Sequencer Module, it might emit its own stream of MIDI to control downstream modules; and it could do this while being itself controlled by MIDI as well.
 
* It is possible for a device to perform **MIDI Merge** -- to take MIDI data from multiple sources, via multiple **IN** sockets, and merge the data to send out one (or more) **Software THRU**.  This is not forbidden but is a rare need, and it can cause MIDI cycles, which are bad, if the musician is not careful.

* These conventions do not consider the implications of a module sending MIDI data *out of the modular system*: you're on your own there.  Note that if an internal modular MIDI format sends data at a higher speed than 31250, then you will have to deal with slowing, and possibly buffering, the data to send it out.

In short: a distributor module receives MIDI data from your controller or DAW or groovebox, and distributes it to chains of MIDI receivers.  Many receivers are meant to respond to only one channel (though they are set to OMNI).  Some polyphonic receivers may be designed to respond to multiple channels.  Certain MIDI receivers may themselves **inject** MIDI into the stream they have received, sending the mixture of the original MIDI and the injected messages out a Software Thru to downstream receivers.  Other modules could well emit their own stream of MIDI to receivers. 

### MIDI Data Streams and Voices

In most cases, each channel represents a single *voice*, that is, a single note for a monophonic synthesizer.  In some cases multiple notes may play on the same channel -- for example a drum module might play several drums, each a different note on the same channel.

This structure allows for MPE, for polyphony, for MIDI Mode 4, and for **multitimbral synthesizers**.

Streams of MIDI data are mostly of two kinds:

- Most data streams consists of voiced messages from only a single channel, plus unvoiced messages.  A stream of this kind is meant to control a **voice chain**, which is a group of modules that all listen to that stream, and work together to form a single *voice*.  For example, a MIDI oscillator, MIDI envelope, and MIDI filter might all listen to this stream to play a single note, that is, they act like a **monophonic synthesizer.**  Since all voiced messages in these data streams are of the same channel, it is reasonable for these modules to simply respond to OMNI.  It's likely common for a voice chain to just consist of a *single* MIDI module, such as the wonkystuff mco/1, working with various non-MIDI modules (filters, envelopes, VCAs).

  Some voice chains are intended to handle multiple notes, not a single one.  The most common scenario is drum synthesizers or drum sample players, where different notes indicate different drums, possibly playing at the same time but on the same channel.
  	
- Some data streams consists of all messages, and are not filtered by channel.  A stream of this kind typically goes to a **polyphonic module**, which has been set up to listen to one or more channels and to play several voices as in response to notes on those channels.  Polyphonic modules can do what they like with this data: they could respond polyphonically to multiple notes on the same channel, or to one note per channed.

  It is also reasonable for a polyphonic module to have multiple **IN** sockets, each responding to MIDI data on a single (different) channel.  This is essentially like having multiple single voice modules in one package.
  
### Distributor MIDI Mode Suggestions

MIDI has four "Channel Modes" set by CCs 124 through 127, and which consist of the combinations of OMNI ON vs. OFF and MONO vs. POLY. 

- MIDI Modes 1 and 2 both imply that the distributor is listening in OMNI mode, and this acts against the basic purpose of Modular MIDI.  We recommend ignoring MIDI Modes 1 and 2, or routing everything to channel 1 when Modes 1 or 2 are requested.

- In MIDI Mode 3 (OMNI OFF, POLY ON) each channel receives its own independent MIDI voice messages, and can potentially play polyphonically. For a Distributor, this implies that incoming MIDI voice messages are simply routed out the MIDI THRU socket for the appropriate channel. Though MIDI Mode 4 is more appropriate for the Modular MIDI Conventions, we recommend MIDI Mode 3 be the default mode as it will be by far the most common mode used by external controllers and devices.  

  It'd be very common scenario for polyphonic notes to arrive on a single channel in MIDI Mode 4.  One option would be for the MIDI Distributor to break those notes up and send them to different outputs for separate downstream voice chains.

- MIDI Mode 4 can also be supported as described below.

#### MIDI Mode 4

MIDI Mode 4 (OMNI OFF, MONO ON) is a special and relatively obscure mode which, as it so happens, is also a very good match for external input to Modular MIDI.  MIDI Mode 4 largely exists on guitar controllers and the like.
 
In MIDI Mode 4, you can instruct an instrument to listen to a range of channels N ... N + M, and treat each one as a separate voice.  If a note arrives on a channel in the range, it immediately replaces any note currently on that channel: thus at most one note can be playing on a given channel at a time.  Furthermore there is a "Global Channel" (channel N - 1, or channel 16 if N = 1), and any data which arrives on that channel is immediately rebroadcasted in parallel to all channels N ... N + M.  

If this sounds suspiciously like MPE, it should!  But unlike MPE, which has elaborate rules, MIDI Mode 4 is very simple: the above is about all there is to it.  MIDI Mode 4 is set up for the range N ... N + M by sending two specific CC messages to channel N.  

A distributor module can respond to MIDI Mode 4 straightforwardly:

- When the appropriate setup request CCs arrive, the distributor assigns channel N-1 to be the "Global Channel" which is automatically rerouted to channels N ... N+M.  Otherwise, channels N ... N+M are unchanged.

- There can be multiple ranges, and they can accidentally overlap.  To disambiguate, we suggest: if a request arrives to set up N ... N + M, and in this range there was already a "Global Channel" X, its special Global Channel status is revoked.

- CC messages 124 ... 127 ought not be passed through to the per-THRU channel sockets.  However as they also imply an ALL NOTES OFF, then perhaps an ALL NOTES OFF could be passed on instead.

- The distributor might keep track, for non-Global channels declared as MIDI Mode 4, of which notes are outstanding.  When a new note arrives, the distributor could send a NOTE OFF to the appropriate channel THRU to guarantee that the old note has been removed.

### Distributor MPE Suggestions

The channel-per-voice approach described can be made compatible with MIDI Polyphonic Expression (MPE) with some work by the Distributor.

MPE is meant for the situation where each note (each voice) is usually sent on a separate channel.  This is mostly compatible with the distributor modules as described so far.  However there are a few considerations:

- The use of MPE on its two "Zones" should not preclude the use of non-MPE functionality on the remaining channels.

- Each MPE Zone has a "Master Channel" for voiced messages intended for every channel (every voice) in the Zone.  MPE calls these "Zone Messages".  Care must be taken as to how to merge Zone messages with per-channel messages.  For example, Pitch Bend at the Zone level might be added to pitch bend at the channel level; or one might supercede the other.  In many cases, messages on the Master Channel should simply be injected into each of the individual channels: for example the Sustain Pedal message, which might possibly only be found on the Master Channel.  It is also permitted, though discouraged, for note messages to be sent to a Master Channel.  A distributor would have to determine how to play these notes, such as redistributing them to existing zone channels.
	
- An MPE zone has a fixed number of channels, and thus normally a fixed number of notes.  What if the musician plays more than that number of notes at a time?  MPE will **spill** those notes to channels it is already using for other notes.  Most receiver modules cannot play more than one note.  The distributor *could* handle this situation for them, by sending a preemptive NOTE OFF message to turn off the old note; but this is not appropriate if the receiver module is a sophisticated polyphonic module which can play multiple notes.  Thus either this behavior should be switchable, or the distributor should allow the individual receivers to deal with it themselves.

#### Reconciling MPE and MIDI Mode 4

The MPE documentation states that "If the Channel ranges all match, then the MIDI Mode 4 device will behave identically whether or not it supports MPE."  This is not quite true.  Both Pitch Bend and Channel Pressure can be sent as Zone messages or as individual channel messages, and the MPE documentation says "If an MPE synthesizer receives Pitch Bend (for example) on both a Master and a Member Channel, it must combine the data meaningfully. The same is true for Channel Pressure".  But Mode 4 is explicit: there is no combination, but rather, they just replace one another.  We suggest if a range is declared to be both an MPE Zone and Mode 4, then Mode 4's behavior takes precedence with respect to Pitch Bend and Channel Aftertouch.

<a name="namespaces"/></a>
## Namespaces, IDs, and CC Messages

Except in the case of polyphonic modules which may have grabbed multiple channels to themselves, channels are commonly allocated to voices, not to individual modules.  This means that multiple modules working to form a single voice will likely receive the same messages.  This isn't a problem for most messages, but it presents a serious problem for CC messages.  Consider the situation where a MIDI-controlled Oscillator and a MIDI-controlled Filter are working together to form a voice.  Both of them receive a CC message that asks that CC Parameter number 43 be changed to the value 21.  The modules are from different vendors.  The Oscillator interprets CC Parameter number 43 as Detune.  But the Filter interprets it as Filter Cutoff.  Both of them change their respective features in response to this CC message -- not what the musician intended!

This collision exists because there is no module-specific namespace for CC: all the modules share the same 128 parameter numbers.  

We suggest two methods for dealing with this:

- First, a module might have the ability to do **CC Learn**.  If it has a block of 10 parameters, the user might teach it to use parameter numbers 17...26, or instead to use parameter numbers 41...50.  This has the advantage of simplicity, but unfortunately the CC parameter space is strewn with standard parameters that the musician probably wouldn't want to overlap with; and each module would have to be individually trained.

- Second, we define the notion of the **ID**.  An ID is a number 1, 2, 3, ... 15 (for the time being).  The first eight IDs each have space allocated for them among the CC parameters (nine CCs each).  The next 7 IDs have space allocated in a different way, discussed later.  All fifteen IDs have significant space allocated among the NRPN parameters.  Modules with different IDs cannot have CC conflicts because they use different parts of the CC space.

### About IDs

Every module that uses CC or NRPN should be assigned a **Default ID**.  The default ID defines the initial CC region assigned to the module.  The idea here is that in simple setups, it is less likely that two modules in the same voice chain will use the same default ID, so adding them into your system is (hopefully) plug-and-play.  The Default ID categories are as follows:

ID | Default ID Category
---|-----------------------------------------------------
1  | Oscillators, Samplers, Note --> Gate/Trigger Modules
2  | Envelopes
3  | LFOs, Sequencers, Clock --> Gate/Trigger Modules
4  | Filters
5  | VCAs, Mixers
6  | Effects, Audio Processors
7  | CC --> Gate/CV/Trigger Generators
8  | Miscellaneous

Some modules may straddle the function of multiple IDs.  In this case you can select the appropriate ID.  You may wish to select the least common ID so as to minimize possible conflicts with other modules.

Modules which employ CC Learn or some other stragegy for CC conflict avoidance are still encouraged to have a default ID which defines their default, initial CC parameters.

Default IDs will work in simple situations.  But a musician may well include modules which use the same ID, creating a conflict.  For example, the musician might use two MIDI-controlled oscillators, detuned, to make a fatter voice: but they are both by default ID 1.  To resolve this conflict, modules should try to have the ability to change their ID to any value 1...15 (typically 1...8).  You could do this by having the musician set and store the ID in Flash RAM, or via a DIP switch, etc.  This means that a module set to (say) ID 5 does not *have* to be a VCA or mixer: this is just the default setting for VCAs and mixers.  **All modules are strongly encouraged to have settable IDs**.

Modules may share parameters even if they are on different IDs.  Consider the two oscillator example.  Perhaps the oscillators are on different IDs so their parameters may be programmed separately; but they wish to share the "pitch tune" parameter.  It's possible for a second oscillator to be set to (say) ID 4 but instructed to also respond to the "pitch tune" parameter on ID 1, sharing it with the first oscillator.

Modules may also claim several IDs if they need to.  One use for this is discussed in the Section below.

A module which does not employ CCs or NRPN at all does not need to use IDs at all and may ignore all of this.

### The CC Map

MIDI defines 128 CC parameters.  These fall into three levels:

- Some parameters have fixed functions, or ones so broadly established and accepted that we should not change them (and we don't).

- Some parameters have established functions but these functions are not used much or have been lost to time; but they appear in the list of CC parameters in DAWs: we change some of these for our purposes but try to intelligently align ID regions with them for the convenience of the musician. 

- Many parameters are unassigned.  We assign these ourselves below, or we leave them "open".

The first 64 CC parameters traditionally can be used in two different ways.  First, the CCs can just be used as plain (7-bit) parameters.  Second, a CC from 0...31 can be paired with the equivalent CC from 32...63 to form a **14-bit MSB/LSB CC pair** which allows one to set parameter values with high-resolution 14 bits rather than low-resolution 7 bits.  The second 64 CC parameters can only be used as plain (7-bit) parameters.  Unfortunately relatively few controller devices and DAWs support 14-bit CCs. 

We have partitioned much of the unallocated CC space into eight regions of 9 CC parameters each.  Each region corresponds to an ID (1 through 8).  The 9 CC parameters belonging to an ID are known as parameters "a" through "i".  Different IDs will have different CCs allocated to their "a" through "i" parameters.  A module of a given ID may assume that it is the *only* module in a voice chain which responds to its 9 allocated CC parameters (or that any other module responding to them does not conflict with it).

If the module wishes it, parameters "a" and "i" may be joined to form a single 14-bit CC parameter called "a".  Similarly, parameters "b" and "h" may be joined to form the 14-bit CC parameter called "b".

We will begin with the first 64 CCs:

CC MSB / LSB    | MSB Function / LSB Function    | Related Traditional Name
----------------|--------------------------------|------------------
0/32	        | Bank Select                    |
1/33	        | Mod Wheel                      |
2/34	        | (Open)                         | Breath Controller
3/35	        | Auxiliary                      |
4/36	        | (Open)                         | Foot Controller
5/37	        | *MSB:* Glide / *LSB:* (Open)   |
6/38	        | Data Entry                     |
7/39	        | *MSB:* Volume / *LSB:* (Open)  |
8/40	        | 1a (a/i)                       |
9/41	        | 1b (b/h)                       |
10/42	        | *MSB:* Pan /  *LSB:* (Open)    |
11/43	        | Expression Controller          |
12/44	        | 6a (a/i) 					     | FX Ctrl 1		
13/45	        | 6b (b/h) 					     | FX Ctrl 2
14/46	        | 2a (a/i)						 |
15/47	        | 2b (b/h)                       |
16/48	        | 3a (a/i)						 |
17/49 	        | 3b (b/h)                       |
18/50	        | 4a (a/i)						 |
19/51	        | 4b (b/h)                       |
20/52	        | 5a (a/i)						 |
21/53	        | 5b (b/h)                       |
22/54	        | 7a (a/i)						 |
23/55	        | 7b (b/h)                       |
24/56	        | 8a (a/i)						 |                    
25/57	        | 8b (b/h)                       |
26/58	        | Modulation a                   |					
27/59	        | Modulation b                   |
28/60	        | (Open)                         |
29/61	        | (Open)                         |
30/62	        | (Open)                         |
31/63	        | (Open)                         |


#### Explanation

- **(Open)** These CCs may be used by the module for any purpose, with the strong warning that other modules may likewise have chosen to do so. 

- **Bank Select, Mod Wheel, Data Entry, and Expression Controller**.  These are standard CCs with well established functions, and are available to all modules. **Bank Select** is discussed in more detail later.  **Data Entry** is only used as part of RPN and NRPN.  Do not use it otherwise.  See RPN/NRPN in a section below.

- **Glide, Volume, Pan**.  These are standard CCs, available to all modules, but we do not allocate the LSB of these CCs, instead, leaving them open.

- **1a, 1b, ... 8a, 8b** These CCs are the first two parameters "a" and "b" available to IDs 1 ... 8.  For example, 3b is the parameter "b" (the second parameter) available to ID 3.  These parameters are 14-bit parameter MSB/LSB pairs.  If the module wishes, it may split one of these pairs into individual 7-bit CC parameters.  In this case, it should split parameter "b" into parameters "b" and "h".  The module can also split the second pair as well: in this case, it should split parameter "a" into parameters "a" and "i".    Note that ID region 6 is out of order -- this is to align it with the traditional names "FX Ctrl 1" and "FX Ctrl 2" for convenience, since the default function of ID 6 is Effects and Audio Processors.

- **Modulation a, Modulation b**. These are **Modulation CCs**, discussed later.

- **Auxiliary**. This is the **Auxiliary CC**, discussed later.

#### Rest of the CC Map

The second table contains the CCs 64 through 127.  These are individual CCs:


CC    | Function                | Related Traditional Name
------|-------------------------|------------------
64    | Sustain Pedal           | 
65    | 1c                      | Glide Switch
66    | 1d                      |
67    | 1e                      |
68    | Legato Switch           |
69    | 1f                      |
70    | 2c                      | Sound Variation
71    | 2d                      | Resonance
72    | 2e                      | Release
73    | 2f                      | Attack
74    | MPE Timbre              | 
75    | 2g                      | Decay
76    | 3c                      | Vibrato Rate
77    | 3d                      | Vibrato Depth
78    | 3e                      | Vibrato Delay
79    | 3f                      | 
80    | 3g                      |
81    | 4c                      |
82    | 4d                      |
83    | 4e                      |
84    | 4f                      |
85    | 4g                      |
86    | 5c                      |
87    | 5d                      |
88    | 5e                      | High Resolution Velocity
89    | 5f                      |
90    | 5g                      |
91    | 6c                      | FX 1 Amt
92    | 6d                      | FX 2 Amt
93    | 6e                      | FX 3 Amt
94    | 6f                      | FX 4 Amt
95    | 6g                      | FX 5 Amt
96    | Increment               |
97    | Decrement               | 
98    | NRPN LSB                |
99    | NRPN MSB                |
100   | RPN LSB                 |
101   | RPN MSB                 |
102   | 7c                      |
103   | 7d                      |
104   | 7e                      |
105   | 7f                      |
106   | 7g                      |
107   | 8c                      |
108   | 8d                      |
109   | 8e                      |
110   | 8f                      |
111   | 8g                      |
112   | 1g                      |
113   | Modulation c            |
114   | Modulation d            |
115   | Modulation e            |
116   | Modulation f            |
117   | Modulation g            |
118   | Modulation h            |
119   | (Open)                  |
120   | All Sound Off           |
121   | Reset All Controllers   |
122   | (Reserved)              |
123   | All Notes Off           |
124   | (Reserved)              |
125   | (Reserved)              |
126   | (Reserved)              |
127   | (Reserved)              |

#### Explanation

- **(Open)** These CCs may be used by the module for any purpose, with the strong warning that other modules may likewise have chosen to do so. 

- **(Reserved)** These CCs have well established functions that are not used by this specification.  Do not use or reassign them.  We note that many of the reserved parameters deal with setting MIDI Modes (mono, poly, omni).  These are obsolete and not relevant here: but these parameters unfortunately cannot be reused.  (Turning MIDI Mode 4 on and off should probably be handled at the distributor level, not internally between modules).

- **Sustain Pedal, Legato Switch, MPE Timbre, All Sound Off, Reset All Controllers, All Notes Off**.  These are standard CCs with well established functions.

- **Increment, Decrement, NRPN LSB, NRPN MSB, RPN LSB, RPN MSB**.  These CCs are used for RPN and NRPN.  Do not use them for other functions.  See the section on RPN and NRPN below.

- **Glide, Volume, Pan**.  These are standard CCs, but we do not define the LSB of these CCs, instead, leaving them open.

- **1c, 1d, ... 8f, 8g** These CCs are the parameters "c" through "g" available to IDs 1...8.  For example, 3f is the parameter "f" available to ID 3.  Note that the first two parameters "a" and "b", as well as the optional parameters "h" and "i", were discussed in the previous table.  Note that parameter 1g is out of order.  This is done to allow several parameter regions to neatly align with certain traditional functionality for convenience.  For example, parameters 3c, 3d, and 3d align with Vibrato Rate, Depth, and Decay, which is convenient as ID 3 deals with LFOs etc.

- **Modulation c ... g**. These are **Modulation CCs**, discussed later.

#### ID CC Summary

The following table shows the CC parameters associated with the parameters "a" through "i" for each ID 1 through 8.

ID | Parameter a | b   | c   | d   | e   | f   | g   | h   |  i  |
---|-------------|-----|-----|-----|-----|-----|-----|-----|-----|
1  |           8 |   9 |  65 |  66 |  67 |  69 | 112 |  41 |  40 |
2  |          14 |  15 |  70 |  71 |  72 |  73 |  75 |  47 |  46 |
3  |          16 |  17 |  76 |  77 |  78 |  79 |  80 |  49 |  48 |
4  |          18 |  19 |  81 |  82 |  83 |  84 |  85 |  51 |  50 |
5  |          20 |  21 |  86 |  87 |  88 |  89 |  90 |  53 |  52 |
6  |          12 |  13 |  91 |  92 |  93 |  94 |  95 |  45 |  44 |
7  |          22 |  23 | 102 | 103 | 104 | 105 | 106 |  55 |  54 |
8  |          24 |  25 | 107 | 108 | 109 | 110 | 111 |  57 |  56 |

If a module needs a 14-bit CC, it may sacrifice parameter "i", and merge "i" with "a" to form the 14-bit CC (now called "a").

If a module needs a second 14-bit CC, it may additionally sacrifice parameter "h", and merge "h" with "b" to form the 14-bit CC (now called "b").

### Auxiliary Parameters, RPN, and NRPN

We offer two ways to provide additional parameters beyond CCs: RPN/NRPN, and so-called Auxiliary Parameters.  RPN/NRPN are standard MIDI.  Auxiliary Parameters are not.

We begin with Auxiliary Parameters.  These are sent with CCs 3 (the MSB) and 35 (the LSB). Both the MSB and LSB must be sent, and the MSB must be sent first.  The MSB holds the function or parameter in question, and the LSB holds its value.  They are described in the table below:

Auxiliary MSB  | Function                      
---------------|-------------------------------
0              | Program Save (LSB = Program 0...127)          
1              | Current Program Save (LSB = 0), Current Program Revert (LSB = 1), Reserved (LSB > 1)
2-14           | Reserved
15             | Reset ID (LSB = 0), Change ID (LSB = 1...15), Reserved (LSB > 15)
16-31          | First 16 Parameters for ID 9
32-47          | First 16 Parameters for ID 10
48-59          | First 16 Parameters for ID 11
64-79          | First 16 Parameters for ID 12
80-95          | First 16 Parameters for ID 13
96-111         | First 16 Parameters for ID 14
112-127        | First 16 Parameters for ID 15

#### Explanation

- **Program Save, Current Program Save, Current Program Revert**.  These functions save and revert the current program (patch) of the modules.  See the section on Program Change and Program Save below for more information.

- **Reset and Change ID**.  It may be convenient to program a module's ID over MIDI, but we cannot require that the module respond to sysex.  Instead, a module may respond to *Reset ID* to reset its ID to its default state, and to *Change ID* to change the ID to a new ID.  Only one module should be listening when this message is sent.  If a module responds to multiple IDs, it should change its first ID in response to this message.
 
- **Parameters for IDs** These provide the first 16 parameters for IDs 9 through 15, which do not have parmeter space allocated to them in CC.  They are the parameters "a" through "p".  This region is largely intended as an optional spillover for additional IDs, and is not expected to be used very much.

- **Reserved** These parameters are reserved.  Do not use or reassign them.

#### NRPN Parameters

Parameters may also be set, for all 15 IDs, using NRPN.  Here space for 256 parameters is provided, and they are 14-bit high-resolution.  For IDs 1 through 8, the first 11 NRPN parameters should be considered the "high resolution" equivalent of CC parameters "a" through "i".  For IDs 9 through 15, the first 16 NRPN parameters should be considered the "high resolution" equivalent of Auxiliary parameters "a" through "p".  

NRPN Region  | Function                      
-------------|-------------------------------
0-255        | Reserved           
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
4096-16383   | Reserved

#### Explanation

- **Parameters for IDs** These are 256 parameters for each module ID.  For each ID, the first parameters should correspond to the CC or Auxiliary parameters allotted to that ID.

- **Reserved** These parameters are reserved.  Do not use or reassign them.

#### RPN Parameters

Finally, modules may respond, as appropriate, to the various defined RPN parameters in the MIDI spec.


### Modules That Need More Parameters

The scheme discussed here allows up to 8 modules on a channel to have between 7 and 9 parameters each in CC.  If this is not enough parameters for your module, you have a few options:

- Split the ID's two 14-bit high-resolution CC parameters into four 7-bit low-resolution parameters. This would give you up to 9 parameters for your ID.

- Use NRPN.  NRPN provides up to 256 high-resolution parameters for your module.

- Use Auxiliary Parameters.  Auxiliary parameters provide up to 16 low-resolution parameters for your module.  However you will need to change the ID to a value in the 9-15 range.

- Allocate an additional ID (or more).  This is permitted.  If you do this, we suggest you allocate two IDs that are consecutive, but this is not required.  For example, your module might allocate IDs 1, 2, and 3.


<a name="injection"/></a>
## MIDI Injection, Program Load/Save, and System Exclusive

### MIDI Injection and Modulation CCs

We provide eight special CCs called **Modulation CCs**, and introduce **MIDI Injection**.  The purpose of these things is to simplify situations where one module might be modulating a second module using MIDI, while both are being controlled (over MIDI) by a DAW or controller etc.

Consider the situation where an ADSR envelope receives CCs from your DAW that change its Attack, Decay, Sustain, and Release, and a Filter receives CCs from your DAW that change its cutoff frequency and resonance.   However we *also* want the ADSR envelope to change the Filter's cutoff frequency.  This could be done by connecting the two with a plain-old CV cable.  But we could also do it via MIDI using **Injection**.

Injection is where a module adds additional MIDI data to its THRU to send the next module downstream.  Clearly the THRU must now be a **Software THRU**, not a normal hardware thru.  The ADSR envelope is a **modulator**.  The Filter is a **target**.  The modulator can modulate the target by injecting CC data to modulate it.  Thus we have the scenario:

    DAW ---> ADSR ---> FILTER

There are eight modulation CCs, "a" through "h".  The first two are high-resolution, 14-bit CCs.  The rest are 7-bit CCs.  These modulation CCs should map to exactly the same parameters the first 8 parameters ("a" through "h") in the target's ID.  The modulator must be *settable* to output any one of these eight modulation CCs, perhaps via a dial.  It does not need to know what these parameters do.

What if the modulator is itself being modulated?  For example, imagine if we had an LFO which receives its Rate from your DAW via CC, and modulates the Attack of the ADSR envelope.  We now have the scenario:

    DAW ---> LFO ---> ADSR ---> FILTER

The LFO injects modulation CCs to send to the ADSR.  The ADSR can do two things with these CCs.  

- First, it could **respond to and remove** the modulation CCs.  That is the scenario described here.  The LFO is modulating the ADSR's Attack, and we don't want that modulation to continue on to the Filter.

- Second, it could **ignore and pass through** the modulation CCs.  Imagine that the LFO was intended to modulate the Resonance of the Filter, while the ADSR modulated its cutoff.  To do this, the LFO injects modulation CCs to send to the ADSR, and the ADSR ignores and passes them through to the Filter, while injecting its own modulation CCs to send to the Filter.

Thus if a modulator itself responds to incoming modulation CCs it must always remove them.  If it does not respond to those CCs, it should pass them through.  If a non-modulator target (such as the Filter) receives modulation CCs, ideally it would remove them prior to passing them on: but it is not required to do so (this would require that the Filter employed Software THRU, and it probably would not).

A modulator could always inject plain old CCs instead of modulation CCs: but in this case it must know exactly what those CCs do to the target.  The modulation CC mechanism is designed to remove this requirement and simply modulation

A modulator can also inject multiple CCs, not just one.

#### Clock and Injection

Modules can inject and remove more than just CCs.  For example, a module is free to inject or remove clock signals, perhaps to do clock multiplication or division.

### Program Change and Bank Select

Modules may have stored programs (patches), and might benefit from being able to change them.  If a module has fewer than 128 patches, it can do so simply by responding to a **Program Change** command. If a module requires more than 128 patches, it can also respond to a Bank Select CC.  Bank Select messages must always come in pairs, both MSB (CC 0) and LSB (CC 32), and together set the bank as 128 x MSB + LSB.  Then the next Program Change selects the program from that bank.   

- If a module has fewer programs than 128, and a so Program Change command is out of range, it should be ignored.

- Similarly if a Bank Select command requests a bank that is out of range for a module, it should ignore both the bank selection *and* the following Program Change.

#### Program Save, Current Program Save, and Current Program Revert

Auxiliary Parameter MSB 0 is **Program Save**, which directs the module to save its current settings to the program in the LSB.  

Auxiliary Parameter MSB 1, with LSB 0, is **Current Program Save**, which directs the module to save its current settings to its current loaded patch if any.

Auxiliary Parameter MSB 1, with LSB 1, is **Current Program Revert**, which directs the module to reload its current loaded patch if any.

Program Save and Current Program Save are a departure from MIDI tradition in that they are commands which change the permanent memory of a module (no other non-sysex command does this).  We may reconsider this in a future version of the conventions.

#### Optional Module Select

We note that other modules listening to the same channel may also respond to this command.  We are considering using the MSB of Bank Select (CC 0) as a **module select**, used to state which module IDs to respond to Program Change, Save, and Revert messages.  Implementation of Module Select would also introduce a new function for IDs beyond their use in coordinating CC messages.

This is not yet part of the conventions, however, and may never be.

### System Exclusive

System Exclusive messages are permitted.  Unfortunately, system exclusive messages may require a manufacturer's ID from the MIDI Manufacturer's Association, and this may be prohibitively expensive.  We would recommend using NRPN to upload and download parameters and patches.


