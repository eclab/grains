# GRAINS Projects
By Sean Luke (sean@cs.gmu.edu)

## WARNING

Many of these projecgts (except Oration and Multiple) use In3 and/or AudioIn as OUTPUTS.  It is not yet clear from TangibleWaves if this can be done safely.  The Arduino is designed to use them as outputs, but only with a resistor inline.  If you attach them to a trigger input then everything is fine.  But if they do not have the proper circuitry protection, and you attach them to GROUND or the 5V, or possibly (but less likely) to another output, you could destroy the In3 or AudioIn pin.    

As a result I have set most of these projects as UNCOMPILABLE.  You can remove the offending comment, but you're on your own: I won't be held responsible for damage to your Grains.

## About the Projects

This is a collection of code which runs on the Tangible Waves GRAINS module, though it can be adapted to run on any Arduino with little difficulty.  All these projects are released under Apache 2.0 except for Lattice, which is released under GPL 2.0 (as it contains Mutable Instruments code) and Oration, which is released under GPL 3.0 (as is Talkie).

## Oration

A new implementation of Talkie which lets you select and speak multiple sentences, or random words within a sentence, from words selected from the entire Talkie vocabulary, not just a small subset.

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

Status: Working

## Multiple

A two-track clock multiplier with a variety of multiplication options.

Status: Working.  Could use some improvement.


