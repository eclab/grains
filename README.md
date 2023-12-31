# GRAINS Projects
By Sean Luke (sean@cs.gmu.edu)

This is a collection of code which runs on the Tangible Waves GRAINS module, though it can be adapted to run on any Arduino with little difficulty.  All these projects are released under Apache 2.0 except for Lattice, which is released under GPL 2.0 (as it contains Mutable Instruments code), Oration/Oration 2, which are released under GPL 3.0 (as is Talkie), and likewise Elements, released under GPL 3.0 as it used GPL code.

## Dave (Epiphany Alpha Release)

A USB Router, Note Distributor, MPE breakout, and MIDI filter.  At present requires Mozzi mode (but not Mozzi), but we might be able to tweak it to use Grains.

Status: This is an **ALPHA** release.  The code compiles and several modes work, but two are untested and others appear to require too much power for Grains to handle.  DELEGATO works, and the pots emit CCs properly. The Java program works reasonably well.  I'm providing it here early in case anyone wanted to bang on it.

## Quant

A Note Quantizer.  Requires Mozzi.

Status: Working, but requires a lookup table to deal with some nonlinearity in the pitch output of GRAINS.  The table is tuned for my GRAINS: I do not know if it matches other people's GRAINS.  I need feedback from prople regarding this.

## Ruckus

A filtered and unfiltered white noise generator.  Requires Mozzi.

Status: Working, but I was hoping to do pink noise as well, but am having some hiccups.  Ran into a lot of Mozzi bugs, reported.

## Droplets

An oscillator which produces random sine waves along a certain chord, much like a wind chime.  Requires Mozzi.

Status: Working.  A bit hissy, but what can you do, it's Mozzi.

## ADSR

An ADSR envelope generator with optional ASR and AHR configurations.  Requires Mozzi, though it could theoretically be adapted to Grains.

Status: Working.

## Scope

An oscilloscope with four switchable analog/digital inputs, an extra digital input, and a little digital function generator.  Uses the Serial Plotter on your laptop's Arduino IDE. 

Status: Working

## Multilevel

A multi-stage envelope with exponential transitions and many options, including sustain loops, whole-envelope loops, resetting, optional flat (sample-and-hold style) stages, and per-stage triggering.  You can set the minimum and maximum envelope values. Supports up to 145 stages.  Requires Mozzi, though it could theoretically be adapted to Grains.

Status: Working.  The rate calculation is obnoxious and I should come up with a better mechanism.  One or two minor bugs but it's quite usable.

## AKWF

A wavetable synthesizer over your choice of waveforms from the [Adventure Kid Waveform](https://www.adventurekid.se/akrt/waveforms/) (AKWF) collection, plus a low-pass filter.  Requires Mozzi.

Status: Working.  The waveforms are pretty aliased (not AdventureKid's fault, Mozzi's fault), and so sound pretty crummy at different positions.

## 808

A drum sampler for up to 9 samples and up to 5 triggers.  Comes with several 8-bit drum samples, including, yes, ones from the TR-808.  You can provide your own samples, and there are a variety of configurations.  Allows 8-bit raw and u-Law samples.  Requires Mozzi.

Status: Working.  This is a major upgrade from the previous 808 version. 

## Voice

A complete synthesizer voice: oscillator, 2-pole filter, and amplifier.  You provide the envelopes.  Square, Sawtooth, and Triangle waves offered, plus Low-pass, High-pass, Band-pass, and Notch filter types.  Requires Mozzi.

Status: Working, but the filter is not amazing, particularly at high resonance and frequency.  Sawtooth is a bit buzzy here and there (just not enough memory to put in more waves).

## Stochastic

A random walk generator (free or about a mean), plus sample and hold, plus added fuzz.   Requires Mozzi. 

Status: Working

## Switchblade

A combination fixed-lag smoother (slew), summed attenuverter, and noise-fuzzer.  Requires Mozzi.

Status: Working

## Chordal

A chording oscillator which can cross-fade between sine and either square, saw, or triangle (selectable in code).  Requires Mozzi.

Status: Working.  Some of the waves are buzzy in certain locations, but we don't have enough dynamic memory left to smooth things out.

## Evolution

An evolving drone synthesizer using the same additive synthesis technique as Booker.  Requires Mozzi.

Status: Working.  Pretty hissy for same reason as Booker.

## Booker

A Hammond Organ simulator with a built-in Leslie and a variety of organ drawbar combinations.  Requires Mozzi.

Status: Working, but it pushes Mozzi to its limits I think, and so is pretty hissy.

## Pattern

A four-channel boolean pattern generator based on an analog input, with 21 interesting patterns.  Can output its patterns as gates or as triggers.

Status: Working, but its use with LFOs is poor due to noise.

## Elements

A Euclidian trigger sequencer with up to 32 steps and both forward and backward sequences and various other options.

Status: Working

## Oration

A new implementation of Talkie which lets you select and speak multiple sentences, or random words within a sentence, from words selected from the entire Talkie vocabulary, not just a small subset.

Status: Working

## Oration 2

A variation of Oration which uses an older version of Talkie, and so can vary pitch and speed independently.  It's also a bit louder: but the sound quality is iffy.  Let me know which one you like better.

Status: Working

## Lattice

A clone of Mutable Instruments GRIDS.  This is essentially a "TOPOGRAF Light".  It does the same thing but, due to limitations in GRAINS's interface, it has to simplify the parameters.  There are different configurations provided for doing so.  

Status: Working

## Beats

A four-track step sequencer with song mode, capable of performing songs of up to 800 trigger patterns, each pattern between 1 and 256 steps long.  You program the patterns in the code, it's set up to be very easy. Each pattern can loop for some N iterations (specified per-pattern), or loop forever.  You can reset, advance to the next pattern, and add swing and randomness in real time.

Status: Working

## Geiger

A three-output random trigger generator and Bernoulli trigger.  You provide a clock input and the probability that, for each output, the output will generate a trigger for a given clock.  You can make the track outputs independent or exclusive (only and exactly one triggers each clock).

Status: Working

## Pulses

A two- or three-output square-wave LFO generator.  Options include two independent LFOs, one with pulsewidth; three Square-wave LFOs all with 50% pulse width; and a mixture of three Square-wave LFOs in the same output.  You can reset and sync two LFOs to the third.

Status: Working

## Theory

One, Two, or Three digital outputs set up as functions of four, five, or six analog+digital inputs.  You specify the functions in the code (it's set up to make it easy!), including special randomness and logic functions.

Status: Working, but like Pattern, its usefulness may be limited due to noise.

## Divvy

A two-track clock divider with a variety of division options.

Status: Working.  I've had a few rare premature resets: let me know if the beat gets off and when.

## Multiple

A two-track clock multiplier with a variety of multiplication options.

Status: Working.  Could use some improvement.


