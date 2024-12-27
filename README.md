# GRAINS Projects
By Sean Luke (sean@cs.gmu.edu)

This is a collection of code which runs on the Tangible Waves GRAINS module, though it can be adapted to run on any Arduino with little difficulty.  All these projects are released under Apache 2.0 except for Lattice, which is released under GPL 2.0 (as it contains Mutable Instruments code), Oration/Oration 2, which are released under GPL 3.0 (as is Talkie), and likewise Elements, released under GPL 3.0 as it used GPL code.

Other GRAINS firmware repositories:

- AE Modular's [GRAINS repository](https://github.com/aemodular/GRAINS) and [GRAINS Mozzi repository](https://github.com/aemodular/MOZZI-GRAINS)
- NurdSpace's [collection of GRAINS firmware](https://nurdspace.nl/Grains#Collection_of_firmwares)
- Duddex's [GRAINS adaptations](https://github.com/duddex/)
- Keurslager Kurt's [GRAINS LFO](https://github.com/ZVanMoerkerke/GrainsAE_VC_LFO_Sine)
- WonkyStuff's [GRAINS projects](https://github.com/wonkystuff/)
- Reductionist Earth Catalog's [Experimental GRAINS projects](https://github.com/reductionistearthcatalog/AE-GRAINS-MIDI/)
- Ginkosynthese's original [GRAINS examples](https://www.ginkosynthese.com/grains-codes)

There are two **special directories**:

* **INFO**.  This contains useful documentation you might like to read.
	* [Developing for GRAINS on AE Modular](info/DevelopingForGrains.md) A collection of tips for GRAINS software developers. 
	* [Modular MIDI Convensions for Musicians](info/ModularMIDIConventionsForMusicians.md) An introduction to the Modular MIDI conventions (IMDI on AE) for interested musicians.
	* [Modular MIDI Convensions for Developers](info/ModularMIDIConventionsForDevelopers.md) The Modular MIDI conventions for software developers
	
* [**MIDI**](midi/README.md).  This contains MIDI library code useful for developers.  It's not a GRAINS firmware application. 

### A Note on GRAINS Oscillators

GRAINS output is noisy.  GRAINS uses filtered PWM to output sound, and filtered PWM creates a low-level buzz or hum which is not filtered out.  To remove this, I suggest including at the beginning of your song a multi-second stretch of silence.  Then load your song into Audacity, and use the silent region (which will have the hum) as the noise profile for the "Noise Reduction" plugin (I'd use dB=7, Sensitivity=7.0, Smoothing=7).

### Firmware Summary

| Oscillators | MIDI Osc   | Digital Mod | CV Modulation | Other |
|:------------|:-----------|:------------|:--------------|:------|
| DX          | DX-M       | Tangle      | Multiplex     | Dave  |
| Pluck       | Pluck-M    | Tardy       | Sequence      | Bit   |
| Booker      | Booker-M   | Chrono      | LFO           | Scope |
| Droplets    | VS         | Pattern     | Arp           |       |
| AKWF        | Mono       | Elements    | Quant         |       |
| 808         | Para       | Lattice     | ADSR          |       |
| Voice       | MJQ        | Beats       | Multilevel    |       |
| Chordal     | Beep       | Geiger      | Stochastic    |       |
| Byte        | Trajectory | Pulses      | Switchblade   |       |
| Evolution   |            | Divvy       | Memoir        |       |
| Oration     |            | Multiple    |               |       |
| Oration 2   |            | Respite     |               |       |
| Fold        |            | Iterate     |               |       |
|             |            | Theory      |               |       |
|             |            | Mode16      |               |       |

## NEW: Mode16

Extends the SEQ16 to include programmable patterns of all kinds, like forward, backward, ping-pong, random patterns, rests, random rests, and so on.  It's like magic.

Status: Working.

## 808

A drum sampler for up to 9 samples and up to 5 triggers.  Comes with several 8-bit drum samples, including, yes, ones from the TR-808.  You can provide your own samples, and there are a variety of configurations.  Allows 8-bit raw and u-Law samples.  Requires Mozzi.

Status: Working.  This is a major upgrade from the previous 808 version. 

## ADSR

An ADSR envelope generator with optional ASR and AHR configurations.  Requires Mozzi.

Status: Working.

## AKWF

A wavetable synthesizer over your choice of waveforms from the [Adventure Kid Waveform](https://www.adventurekid.se/akrt/waveforms/) (AKWF) collection, plus a low-pass filter.  Requires Mozzi.

Status: Working.  The waveforms are pretty aliased (not AdventureKid's fault, Mozzi's fault), and so sound pretty crummy at different positions.

## Arp

A 16-note random melody generator.  Think of it as Topograf / Lattice for notes rather than drums.  Requires Mozzi.

Status: Working, but GRAINS's severe limits on note output range can hamper things.

## Beats

A four-track step sequencer with song mode, capable of performing songs of up to 800 trigger patterns, each pattern between 1 and 256 steps long.  You program the patterns in the code, it's set up to be very easy. Each pattern can loop for some N iterations (specified per-pattern), or loop forever.  You can reset, advance to the next pattern, and add swing and randomness in real time.

Status: Working

## Beep

A 9-voice MIDI-controlled sine wave generator with built-in hold, velocity sensitivity, and decay.  Can produce a variety of beeps, bloops, beeps, dings, and pings.

Status: Working.  When decays get quiet, they go hissy and buzzy fast thanks to GRAINS's PWM, which can't handle small values well without lots of artifacts.

## Bit

A nasty bitcrusher.  Requires Mozzi.

Status: Working.  It helps that the arduino is already pretty aliased!  Mozzi must be *modified* to use Bit, which may not be worthwhile.

## Booker

A Hammond Organ simulator with a built-in Leslie and a variety of organ drawbar combinations.  Requires Mozzi.

Status: Working, but it pushes Mozzi to its limits I think, and so is pretty hissy.

## Booker-M

A MIDI-Controlled Hammond Organ simulator with a built-in Leslie and a variety of organ drawbar combinations.  Requires Mozzi.

Status: Working, but it pushes Mozzi to its limits I think, and so is pretty hissy.

## Byte

A ByteBeat emitter in the style of AlgoDrone or ByteBeat-Xfade.  You can choose from 16 Bytebeats, *sort of* change their pitch and speed, change their volume, and reset them.  You can of course define your own ByteBeats.  Requires Mozzi.

Status: Working.  Maybe at some point I'll figure out how to hack Mozzi to allow real pitch control.

## Chordal

A chording oscillator which can cross-fade between sine and either square, saw, or triangle (selectable in code).  Requires Mozzi.

Status: Working.  Some of the waves are buzzy in certain locations, but we don't have enough dynamic memory left to smooth things out.

## Chrono

A master clock with a tappable tempo.  Very simple.  Saves the tempo and reloads it when you power cycle.

Status: Working.

## Dave 

A USB Router, Note Distributor, MPE breakout, and MIDI filter.

Status: All current modes are working.  Lots of stuff, like USB MIDI clock, drum triggers, MIDI routing, MPE, note distribution, removing legato from Wonkystuff modules, CC injection from pots, creation of MIDI notes from gate/cv, etc.  There's also instructions for routing MIDI directly from GRAINS into your MB/1 or MASTER I/O via their TRS inputs, if you're daring and willing to accidently fry your MASTER I/O or MB/1.  :-)

## Divvy

A two-track clock divider with a variety of division options.

Status: Working.  I've had a few rare premature resets: let me know if the beat gets off and when.

## Droplets

An oscillator which produces random sine waves along a certain chord, much like a wind chime.  Requires Mozzi.

Status: Working.  A bit hissy, but what can you do, it's Mozzi.

## DX

An FM sine-based oscillator.  Has algorithms for 2 operators with self-modulation, and for 3 operators in three different configurations.

Status: Working

## DX-M

A MIDI-based version of DX, an FM sine-based oscillator.  Has algorithms for 2 operators with self-modulation, and for 3 operators in three different configurations.

Status: Working

## Elements

A Euclidian trigger sequencer with up to 32 steps and both forward and backward sequences and various other options.

Status: Working

## Evolution

An evolving drone synthesizer using the same additive synthesis technique as Booker.  Requires Mozzi.

Status: Working.  Pretty hissy for same reason as Booker.

## Fold

A wave folding oscillator.  A sine is pushed through a soft (sine wave type) wave folder, then through a low pass filter.

Status: Working.

## Geiger

A three-output random trigger generator and Bernoulli trigger.  You provide a clock input and the probability that, for each output, the output will generate a trigger for a given clock.  You can make the track outputs independent or exclusive (only and exactly one triggers each clock).

Status: Working

## Info

This directory contains documents with useful information for developing GRAINS, using MIDI, etc.

Status: A work in progress, I guess!

## Iterate

A multiplexer, in the same vein as, but not the same as, the Doepfer A151 sequential switch.  Successive gates or triggers are multiplexed out to up to four different outputs, either in series or randomly selected.

Status: Working.

## Lattice

A clone of Mutable Instruments GRIDS.  This is essentially a "TOPOGRAF Light".  It does the same thing but, due to limitations in GRAINS's interface, it has to simplify the parameters.  There are different configurations provided for doing so.  

Status: Working

## LFO

A warpable LFO with pulse (with pulse width), warpable saw/tri/ramp, and warpable sine waves, with a wide range of rates.  Can be reset, and outputs a sync trigger.  Requires Mozzi.

Status: Working.  However GRAINS can only reach about 4V, so it goes about 0-4V or so.  LFO's pulse waves are by necessity pretty rounded: if you want sharp pulse waves, see PULSES instead. 

## Memoir

A real-time CV recorder.  Can record between 4 and 32 seconds (depending on desired resolution) of CV information and an accompanying track of gate/trigger information.  Requires Mozzi.

Status: Working. 

## MIDI

Not a GRAINS project per se.  This is a lightweight MIDI library designed for small controllers.  I have developed it for the various MIDI-controlled projects on this page.

If you're looking for a MIDI router or toolkit application for GRAINS, check out DAVE.

Status: Under Development.

## MJQ

A MIDI-controlled three-voice marimba simulator.  You can control brightness, decay, and overall volume.

Status: Working.  When decays get quiet, they go hissy and buzzy fast thanks to GRAINS's PWM, which can't handle small values well without lots of artifacts.

## Mono

A MIDI-Controlled monophonic oscillator.  You can choose between Sine and either Saw, Triangle, or Square.  Requires Mozzi.

Status: Working.  But every once in a while I'll get a MIDI error due to the use of software serial.  This likely isn't fixable until we move to a hardware UART if/when TW decides to come out with Grains V2 (no, I have no information on that).

## Multilevel

A multi-stage envelope with exponential transitions and many options, including sustain loops, whole-envelope loops, resetting, optional flat (sample-and-hold style) stages, and per-stage triggering.  You can set the minimum and maximum envelope values. Supports up to 145 stages.  Requires Mozzi.

Status: Working.  The rate calculation is obnoxious and I should come up with a better mechanism.  One or two minor bugs but it's quite usable.

## Multiple

A two-track clock multiplier with a variety of multiplication options.

Status: Working.  Could use some improvement.

## Multiplex

A 1- or 2-in, 4-out multiplexer with a variety of modes.  Meant to allow you to select various outputs with a simple switch or multiplexed input.

Status: Working

## Oration

A new implementation of Talkie which lets you select and speak multiple sentences, or random words within a sentence, from words selected from the entire Talkie vocabulary, not just a small subset.

Status: Working

## Oration 2

A variation of Oration which uses an older version of Talkie, and so can vary pitch and speed independently.  It's also a bit louder: but the sound quality is iffy.  Let me know which one you like better.

Status: Working

## Para

A MIDI-Controlled 3-voice paraphonic oscillator, meant to play chords from your keyboard or DAW or groovebox.  You can choose between Sine and either Saw, Triangle, or Square.  Requires Mozzi.

Status: Working.  But every once in a while I'll get a MIDI error due to the use of software serial.  This likely isn't fixable until we move to a hardware UART if/when TW decides to come out with Grains V2 (no, I have no information on that).

## Pattern

A four-channel boolean pattern generator based on an analog input, with 21 interesting patterns.  Can output its patterns as gates or as triggers.

Status: Working, but its use with LFOs is poor due to noise.

## Pluck

A 4-voice Karplus-Strong string plucking simulator.  Requires Mozzi.

Status: Working, but because it has to commit to a pitch immediately (to build the Karplus-Strong sample table) sometimes it can get off pitch on some notes thanks to GRAINS/Mozzi's inaccuracy in Analog Input reads.  Could use some work there.

## Pluck-M

A MIDI-based version of Pluck, a 4-voice Karplus-Strong string plucking simulator.  Requires Mozzi.

Status: Working.  Unlike Pluck, pitch is very stable.

## Pulses

A two- or three-output square-wave LFO generator.  Options include two independent LFOs, one with pulsewidth; three Square-wave LFOs all with 50% pulse width; and a mixture of three Square-wave LFOs in the same output.  You can reset and sync two LFOs to the third.

Status: Working.  If you want more LFO waves, see the LFO firmware.

## Quant

A Note Quantizer.  Requires Mozzi.

Status: Working, but requires a lookup table to deal with some nonlinearity in the pitch output of GRAINS. The table is tuned for my GRAINS: let me know if it's working for yours as well.

## Respite

A fix to make it possible to use 2ENV with fast gate release / raise combinations that often occur with MIDI notes.

Status: Working.

## Ruckus

A filtered and unfiltered white noise generator, plus various forms of Sample and Hold.  Requires Mozzi.

Status: Working, but I was hoping to do pink noise as well, but am having some hiccups.  Ran into a lot of Mozzi bugs, reported.

## Scope

An oscilloscope with four switchable analog/digital inputs, an extra digital input, and a little digital function generator.  Uses the Serial Plotter on your laptop's Arduino IDE.

Status: Working

## Sequence

A step sequencer with 8 patterns, each of which can have up to 16 steps (8 is typical), including rests.  You can select the gate length, increment through patterns or select them manually, and edit all the steps entirely from the knobs on the GRAINS.  

Status: Working.  It's quite fiddly because there are only three knobs, but it's doable.  16 steps is particularly fiddly, perhaps not worth the effort.  But 8 works great.

## Stochastic

A random walk generator (free or about a mean), plus sample and hold, plus added fuzz.   Requires Mozzi. 

Status: Working

## Switchblade

A combination fixed-lag smoother (slew), summed attenuverter, and noise-fuzzer.  Requires Mozzi.

Status: Working

## Tangle

A combined digital logic function and buffered mult which can do various things, including trigger merges, inversion, thresholding, gate comparison, and so on.

Status: Working

## Tardy

A short delay for digital signals.  Takes up to 3 inputs and outputs each of them, delayed by a settable amount.  Meant primarily to sync up drum triggers to line up with other things slowed by latency.

Status: Working

## Theory

One, Two, or Three digital outputs set up as functions of four, five, or six analog+digital inputs.  You specify the functions in the code (it's set up to make it easy!), including special randomness and logic functions.

Status: Working, but like Pattern, its usefulness may be limited due to noise.

## Trajectory

A vector synthesizer similar to VS: but with four fixed waves (Sine, Saw, Square, Noise, with optional silence).  This is much more limited than VS, but the advantage is that these waves can be arranged to have many fewer aliasing issues than the waves in VS.

Status: Working.  Also works great with AE's JOYSTICK.

## Voice

A complete synthesizer voice: oscillator, 2-pole filter, and amplifier.  You provide the envelopes.  Square, Sawtooth, and Triangle waves offered, plus Low-pass, High-pass, Band-pass, and Notch filter types.  Requires Mozzi.

Status: Working, but the filter is not amazing, particularly at high resonance and frequency.  Sawtooth is a bit buzzy here and there (just not enough memory to put in more waves).

## VS

A MIDI-based monophonic vector synthesis oscillator, with most of the waves from the Prophet VS.  Requires Mozzi.

Status: Working.  Also works great with AE's JOYSTICK.  However, highly aliased as it only has one copy of each VS waveform and cannot resample (the same problem as AKWF).  See TRAJECTORY for a vector synthesizer with fewer aliasing issues.



