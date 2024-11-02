# Issues with and Suggested Improvements to AE Modules

This is a list of the problems, bugs, and suggested improvements of modules and other aspects of the AE modular system which do not appear to have been discussed elsewhere.    I hope some of these issues can be improved in future versions.  

For brevity, it specifically omits all the **good features** of this system -- you can find that in many other places on the interweb.  So don't let this list get you down: AE is fun!  But I have to document the isssues with them somewhere, so here it is.

If you don't see a module here, it's because I don't have any experience with it.

### Missing Modules

Thinking of building a module?  Here are some missing ones:

- MIDI-controlled and MIDI-generating sequencers and other modulators.  See [Modular MIDI Conventions for Developers](https://github.com/eclab/grains/blob/main/info/ModularMIDIConventionsForDevelopers.md).

- More and better filters.  I think this is the weak point of the system.

- A Ring Modulator.  I'd combine this with the wavefolder into a single 1U module.

- Additional Mutable Instruments algorithmic-heavy clones.  Clouds, Rings, Grids, and Plaits (soon?) are available.  But how about Braids, Beads, Edges, Elements, Marbles, Rings, or Warps to name a few?

- Polyphonic oscillator, filter, and VCA packages.



## Bus Design

### Power

The AE Modular system is 5V power.  This has many advantages but it is also unfortunate because the signals also reach 5V.  As a result, modules have no headroom between their power voltage and their top signal output.  Many modules as a result realistically can only provide about 4.5V worth of signal output; or have to compromise in their circuitry in other ways.  It probably would have been better if the system provided 9V or 7.5V power with a 5V reference voltage.  This could still be optionally done with an additional ribbon cable.

### Signals

The ribbon cable bus carries several dedicated signals: B.START, B.STOP, B.CLOCK, B.CV, B.GATE, and B.CTRL.  Four of these are great, but two of them are a bit odd:

- B.CTRL is notionally provided by the MASTER I/O module when it receives MIDI CC 20, which is quite arbitrary.  It'd have been more useful to have B.CTRL reflect MIDI CC 1 (Modulation Wheel).

- B.STOP is sent when the MASTER I/O receives a MIDI STOP signal.  I have yet to find a valid use case for this signal.  Neither SEQ16 or TRIQ164 need it, ever.  Given how little it is used or needed, it'd be better if it were repurposed.  I'd suggest having it send MIDI Aftertouch or some other MIDI CC, such as CC 11 (Expression Pedal) or 64 (Sustain Pedal).

As of Revision 5 of the MASTER I/O module, you can at least disconnect the MASTER from any of the signals, so in theory these issues might be remedied in the future.
 


## I/O Modules

### MASTER I/O

I have used the MASTER I/O R1, R2, and R4.

- The MASTER I/O would greatly benefit from providing more amperage.  When I attach my second rack, the voltage sags badly.  And we're not talking about particularly power-hungry modules in the second rack.

- The various versions of MASTER I/O have a lot of differences, and these differences are not documented anywhere.  For me the big upgrade was getting access to GROUND on the R4. 

- The R1 and R2 versions had a trimpot which would let you tweak the reference voltage to precisely 5.0V.  The R4 and beyond doesn't have this any more.  So when you have lots of modules, you cannot trim the R4 to compensate for their voltage sag.

- I was told that you could power a MASTER I/O like any other module and it'd work fine.  But when I did that to my R2, it permanently fried its MIDI Pitch functionality (MIDI Gate still works)  So don't do that.

[AE Modular Wiki Entry on MASTER I/O](https://wiki.aemodular.com/pmwiki.php/AeManual/MASTER)


### MB/1

The Wonkystuff MB/1 does a good job of splitting MIDI signals to different outputs for different MIDI-aware devices.  It also provides a gate for MIDI CLOCK START/STOP, a MIDI START trigger, and a 24PPQ trigger pulse for MIDI CLOCK signals.

The primary weakness of the MB/1 is that can only receive MIDI signals from TRS, and not from USB nor from an auxiliary "MIDI IN" AE modular socket (from GRAINS, say, or from a future AE tracker/sequencer module).

I have a heartstopping workaround for the lack of a MIDI IN socket:  in my "DAVE" GRAINS firmware documentation, I have instructions for routing MIDI from an AE socket directly into the MB/1 MIDI jack, but this is dangerous if you do it wrong.

The secondary weakness of the MB/1 is that it does not send MIDI CLOCK messages out the various per-channel MIDI outputs.  The only way to receive MIDI CLOCK messages is via the main MIDI output (along with everything else).  

The tertiary weakness of the MB/1 is that it does not parse nor handle MPE.  Messages coming in on the MPE MASTER Channel (Channel 1 say) will just go to Channel 1 rather than be routed to all channels in the MPE zone.

A final weakness of the MB/1 is that it cannot break out polyphonic notes on a single channel and send them to separate output sockets.

The MB/1 is a first generation device and I have no doubt that it will evolve as the MIDI ecosystem matures.

[Wonkystuff's MB/1 Page](https://wonkystuff.net/product/mb-1/)

### 4 I/O

Works fine.

[AE Modular Wiki Entry on 4 I/O](https://wiki.aemodular.com/pmwiki.php/AeManual/4IO)


### HPAMP

HPAMP introduces occasional crackling sounds into the headphone output.  I have tried this on three HPAMP modules, and all three do it.  Thus I think HPAMP is adequate (but annoying) for headphone monitoring, but you should not use it to attenuate or boost output to an audio interface.

[AE Modular Wiki Entry on HPAMP](https://wiki.aemodular.com/pmwiki.php/AeManual/HPAMP)

## Development Modules

### GRAINS

I am primarily known for my hastily drawn up and poorly documented GRAINS firmware. In the process I gathered a large number of suggested improvements to GRAINS, many of which I have sent to Tangible Waves, who are working on a revised version of the module.  I do not know if this module will ever see the light of day and have no information on its timeline, though I own a prototype.

A few existing issues with GRAINS:

- Hardware UART TX and RX are not exposed. This is a big failing, as it means that GRAINS must rely on software serial to do MIDI, which is too slow to work well.  

- The filtered PWM output of GRAINS seems to have an issue above about 4.25 volts -- it doesn't track properly any more and starts distorting.  This means that audio output must be limited to run between 0.75 and 4.25 volts rather than between 0.0 and 5.0 volts.  And CV output is limited to run between 0.0 and 4.25 volts.

- The PWM output of GRAINS is not buffered, and so various devices will pull its voltage down, which presents a challenge for doing CV out that's 1V/Oct.

- The impedance on the Audio In is so high (due to choice of resistor) that Audio In may not get proper Analog readings unless you read a few times.

- All three analog inputs have strange scaling.  IN3 for example reads 1023 (maximum) at about 4V.  AUDIO IN may be the same.  You can fix this by feeding your signal into a 4ATT/MIX and attenuating it a bit (set the pot to about 3'oclock).  For similar reasons, IN1 and IN2 must have their pots set to about 70% (2 'oclock position) in order to read 1023 (maximum) at about 5V.  

- A great many 328P pins are not exposed and could be used for LEDs, buttons, another knob, more inputs and outputs, etc.

[AE Modular Wiki Entry on GRAINS](https://wiki.aemodular.com/pmwiki.php/AeManual/GRAINS)  
[My GRAINS Stuff](https://github.com/eclab/grains)


### BRAEDBOARD

I think this is a great product, especially with the fun stuff it comes with.  I don't have enough experience with this module except to lament that the breadboards aren't wider.   Lots of chips won't fit on them, notably (in my case) the [Daisy Seed](https://electro-smith.com/products/daisy-seed).  Perhaps rotate them 90 degrees?  Make a wider module?

Also, perhaps a 3.3V supply would have been useful.

[AE Modular Wiki Entry on BRAEDBOARD](https://wiki.aemodular.com/pmwiki.php/AeManual/BRAEDBOARD)  

### AEuroPi

Discussed in Modulation below.


### TBD

Discussed in Oscillators and Sound Generators below.




## Oscillators and Sound Generators

### VCO

VCO was one of the first Tangible Wave modules, and it has held up pretty well.  It is a straightforward triangle/saw/square VCO with pulse width and hard sync.  But it has some issues which could be addressed.

- VCO has an approximately 4 octave linear tracking range.  It's not too bad.  Unfortunately linear tracking seems to be tied to a specific note range.  Below roughly low C (the C below middle C) the tracking starts going nonlinear fast.  This means that VCO cannot really be used for bass.  It's fine for high notes, not for low ones.

- VCO has no sine wave.

- VCO's CV input has a low input impedance.  This means that  devices which connect to it will have a reasonably high degree of amperage pulled out of them.  This affects GRAINS: it means that its Audio Out, when abused as a CV output (as I often code it) will see its voltage scaled down when connected to VCO, essentially making it nonlinear.  I have to have special tables in GRAINS just for outputting to VCO.

[AE Modular Wiki Entry on VCO](https://wiki.aemodular.com/pmwiki.php/AeManual/VCO)  

### 2OSC and 2OSC/d

2OSC was an early noisemaker dual oscillator, and it has poor tracking.  It can track linearly perhaps for only about an octave and a half.  I do not recommend the 2OSC for anything other than as a single-note drone generator.  2OSC also has a single tuning knob which is very hard to set.

2OSC/d its replacement, and has much better tracking, perhaps about 4 octaves.  It is a useful dual oscillator.  However its tuning is still very fiddly; a fourth knob to provide fine tuning on at least one oscillator would be welcome.  

It is also unfortunate that neither provides triangle waves.  One minor result of the lack of triangle waves: 2OSC/d is not very useful in combination with WAVE FOLDER.

[AE Modular Wiki Entry on 2OSC](https://wiki.aemodular.com/pmwiki.php/AeManual/2OSC)  
[AE Modular Wiki Entry on 2OSC/d](https://wiki.aemodular.com/pmwiki.php/AeManual/2OSCD)  


### DRUMKIT010

DRUMKIT010 is a small drum rompler based around a simple low-res chip, and it is what it is.  It has 11 sounds arranged into several banks, so the same sounds appear in several banks. 

The original DRUMKIT010 had a high pitch whine, especially when changing pitch.  The new V2 version does not.

[AE Modular Wiki Entry on DRUMKIT010/d](https://wiki.aemodular.com/pmwiki.php/AeManual/DRUMKIT010)  


### 555

Kurt and Maverick's 555 VCO is the "high end" VCO available for AE modular.  It is based on a famous open 555 Timer VCO design, and sports many features, including sine waves, sync, linear and exponential FM, etc.

The 555 VCO could be improved.  It is a triangle core oscillator, so its triangle wave is perfect.  However its sawtooth wave consists of two triangle waves attached together and where they attach there is a vertical discontinuity which cannot be adequately smoothed out (the 555 has several trimpots, but you can't trim them far enough).  The square wave also has an odd discontinuity but it's minor.  The sine wave is also problematic: either the top or the bottom of the wave has a V-shaped discontinuity in it, which introduces a lot of partials into what should be a pure single-partial wave.  Linear FM on the 555 is also problematic: it's fairly nonlinear.

I'm told that many of these issues stem from the fact that the classic 555 circuit design works better with larger voltages, or at least more headroom.  Overall the 555 sounds "characterful", but I would prefer the sine wave in particular to sound better, and for linear FM to work better.

My 555 tracking goes bad every time I travel with my rack. I  believe it is because the tracking pots are located right next to the ribbon cable connector, where the folded ribbon cable rubs up against them.  They should be moved elsewhere.


### KAESTLE and KAESTLE DRUM

These are both very interesting modules with lots of promise.  Unfortunately both of them add considerable noise to the power line, which interferes greatly with certain other modules, notably KICK and DELAY.  They need to have power line filters.

[Wonkystuff's KAESTLE Page](https://wonkystuff.net/product/kaestle/)  
[Wonkystuff's KAESTLE DRUM Page](https://wonkystuff.net/product/kaestledrum/)


### KICK

Kick isn't particularly punchy, but I find it fine for its purpose.  However Kick is *very* sensitive to noise on the power line, and outputs it directly.  This puts it into conflict with a number of badly behaving modules which dump unfiltered buzz onto the power line, such as Kaestle, Kaestle Drum, Aeuropi, 2ENV, and especially TBD.

[AE Modular Wiki Entry on KICK](https://wiki.aemodular.com/pmwiki.php/AeManual/KICK)  

### MCO/1

MCO/1 is based on a wave generation model for a very simple microprocessor: and so it is very aliased.  It is what it is.

MCO/1 has a problem with 2ENV.  In many grid-based grooveboxes or trackers, it is often the case that a note will be played, then stopped *immediately* before the next note is played.  This results in MIDI emitted with a NOTE-OFF and then a NOTE-ON immediately afterwards.  The MCO/1 dutifully responds by dropping the gate and then re-raising it extremely fast, so fast in fact that it breaks 2ENV, the primary target for such gates.  

This is 2ENV's fault: but MCO/1 would do well to add in an optional very slight delay after dropping a gate before raising it.  At present it's not compatible with 2ENV!  I have a fix for this in the form of a GRAINS firmware called RESPITE.

Users of MCO/1 might also appreciate the "Note Generator Mode" in Dave, one of my GRAINS firmware.

[Wonkystuff's MCO/1 Page](https://wonkystuff.net/product/mco-1/)

### TBD

TBD has a wonderful collection of oscillator, filter, and effects options.  However it puts a dramatic amount of buzz and noise on the power line, interfering with other modules, notably Kick and Delay.  It badly needs a power line filter.

The TBD is notionally programmable: but its toolchain is very complex and nontrivial to set up on most platforms.  If you're looking to do some hobby development, I would suggest GRAINS instead.

[AE Modular Wiki Entry on TBD](https://wiki.aemodular.com/pmwiki.php/AeManual/TBD)  

### NOISE

NOISE offers pulse (or as it calls it, "digital") and analog noise.  A single rate knob controls the noise color.  The rate knob doesn't offer a wide noise spectrum unfortunately: it's too dark.  But it's adequate.

A high percentage of the people who buy NOISE will also buy 2S&H.  Both of them are very simple modules which pair naturally.  They should not be offered separately, but instead should be offered as a single module which does filtered noise, Sample and Hold, Track and Hold, and random LFO.    Indeed, there is a micromodule which does a simple version of this.  I have GRAINS firmware (RUCKUS and STOCHASTIC) which can do much of this.

[AE Modular Wiki Entry on NOISE](https://wiki.aemodular.com/pmwiki.php/AeManual/NOISE)  
[AE Modular Wiki Entry on 2S&H](https://wiki.aemodular.com/pmwiki.php/AeManual/SAMPLEHOLD)  

## Sound Modifiers

Filters are definitely AE Modular's weak point, and better filters would be welcome.  I wonder if the issue here is the 5V limit.

### FILTER (WASP TYPE)

The Wasp Filter was an early filter for AE Modular, notionally modeled after the filter on the EDP Wasp (or perhaps Doepfer's module clone).  But its minimum resonance is far too high. The AE Modular's Wasp Filter, at **zero** resonance, is as resonant as the original EDP (or Behringer) Wasp filter at 50% resonance!  The Wasp filter would benefit from having a resonance knob which starts much lower.

I have had one Wasp Filter die on me.  It was probably 5 years old.

[AE Modular Wiki Entry on FILTER (WASP TYPE)](https://wiki.aemodular.com/pmwiki.php/AeManual/FILTERWASP)  


### SV FILTER

SV Filter seems to be a straightforward 2-pole filter.  It's useful exactly because it is so ordinary.

I have three SV Filters and I am saddened to report that all three are **very** different sounding from one another. The first, an old used module, has a CV with rather little effect on the cutoff.  The second is brand new and its CV has three times of an effect.  The third is a used module, and its CV has twice the effect of the second one (!); and the overall cutoff knob goes rather brighter too.  This is all very frustrating because I need SV Filter for polyphony and so need consistency.

[AE Modular Wiki Entry on SV FILTER](https://wiki.aemodular.com/pmwiki.php/AeManual/SVFILTER)  

### DIODE FILTER (303 TYPE)

I have only tested the 303 Filter once, borrowing it from a friend: and found his to be quite noisy.

[AE Modular Wiki Entry on DIODE FILTER (303 TYPE)](https://wiki.aemodular.com/pmwiki.php/AeManual/DIODEFILTER)  

### WAVE FOLDER

The wave folder works decently.  It unfortunately only has CV control for the wave shape and not for the drive, though the drive is where a lot of the action is.  Additionally, the drive isn't extreme enough, so you don't get very many folds.  It'd be nice to have more folding.

*[The following is not a comment on WAVEFOLDER in particular, but classic Wave Folding as a practice in general]* Note that wavefolding works well for symmetric waves like triangle or sine, but poorly for sawtooth (basically warping a sawtooth into a square and then into another sawtooth) and not at all for a square wave.  Thus the wave folder is pretty much useless in combination with 2OSC or 2OSC/d, which has no triangle.

Warping a sine wave is nice.  You can get a (not great) sine wave from the 555 VCO, and you can get one from my SINE, DX, DX-M, and CHORDAL firmware for GRAINS.

[AE Modular Wiki Entry on WAVE FOLDER](https://wiki.aemodular.com/pmwiki.php/AeManual/WAVEFOLDER)  

### 2VCA

The 2VCA module seems to work just fine for its purpose.  Later "R2" versions of the module seem to have more amplification range than earlier versions (which also didn't have the Audio/DC switches).

My main gripe with the 2VCA is that it really should have **THREE** VCAs in it, if not **FOUR**. VCAs are not complicated.  There's plenty of space for knobs and switches, two more VCA's worth of free output sockets, and at least one more VCA's worth of free input sockets.

[AE Modular Wiki Entry on 2VCA](https://wiki.aemodular.com/pmwiki.php/AeManual/2VCA)  

### MULTI FX

Mutli FX is built on a common effects chip in the Eurorack world, the [Spin Semiconductor FV-1](http://spinsemi.com/products.html).  This chip has eight hard-coded effects programs and eight slots for custom programs.  Tangible Waves has provided programs for those eight custom slots.

The big problem with this module is that the FV-1's hard-coded effects are not very good.  It may be that the problem lies in the lack of parameters.  The primary two FV-1 reverbs (Reverb 1 and Reverb 2) have as their parameters "Reverb Time", "High Filter", and "Low Filter".  What they don't have is the critical "Early Reflection Time" (or "Pre-Delay") parameter.  As a result neither of them can be adjusted for room size.  Additionally, the Shimmerverb is quite bad. Note that this is just the hard-coded effects: Tangible Waves's 8 custom effects are fun.

This could have been remedied if the 8 customizable slots in the FV-1 were programmable somehow. There are [many effects programs available](https://mstratman.github.io/fv1-programs) for this chip. For example, there are lots of free reverb programs with Pre-Delay available.

[AE Modular Wiki Entry on MULTI FX](https://wiki.aemodular.com/pmwiki.php/AeManual/MULTIFX)  

### DELAY (LOFI TYPE)

DELAY is a basic grungy delay and it works pretty well.  It's a bit noisy and weird but I think it's not too bad.  I sold mine and in truth I regret it.

The big problem with DELAY is that it's sensitive to noise on the power line, just as KICK is, and will output buzz from badly behaving modules.

It's been reported that DELAY will push delay back into its input even when it's not being used.  I have never found this to be an issue.  If you're not using DELAY, just disconnect it.

[AE Modular Wiki Entry on DELAY](https://wiki.aemodular.com/pmwiki.php/AeManual/DELAY)  


## Modulation Modules

### 4BUFFER

Works fine.  It would be nice to have more than 4 buffered outs.

[AE Modular Wiki Entry on 4BUFFER](https://wiki.aemodular.com/pmwiki.php/AeManual/4BUFFER)  


### LFO

I have two original LFO modules.  They work just fine.  The only weirdness is that switching between triangle and sawtooth effects the rate of the pulse wave (?!?).  That is minor.

I have a student who had a (maybe 6 year old) LFO die on him.  

LFO is discontinued.  This is unfortunate because its replacement, 2LFO, has a serious bug.  See below....

[AE Modular Wiki Entry on LFO](https://wiki.aemodular.com/pmwiki.php/AeManual/LFO)  

### 2LFO

2LFO is a replacement for the original (single) LFO module.   On paper 2LFO should be better than LFO.  But it is not.

2LFO is marred by a bug.  On the "lower" of the two LFO units, the triangle and sawtooth are not centered at 2.5V, but rather are centered at around 2.0, with a peak around 4.0, and when they dip below 0V they clip.  There is a vertical centering trimming potentiometer on the back side of the 2LFO.  But it only affects the "upper" LFO.  

The "upper" LFO has a minor problem as well.  It rises to its highest value, but before it falls, it first "pops" up to a slightly higher value to start.  It's not a proper triangle or saw wave.  This is really only a problem for the slower rates.  But it is very noticable.

I do not know if the LFO in the MODULATORS module exhibits the same behaviors, nor the one in the uLFO module.  I would try to get two older LFO modules instead of a 2LFO, except that they are no longer offered.  My GRAINS firmware includes a few LFO options (LFO, PULSES).  There is a new dual LFO being offered by Faux Cyrillic which I know nothing of.  

[AE Modular Wiki Entry on 2LFO](https://wiki.aemodular.com/pmwiki.php/AeManual/2LFO)  


### 2ENV

2ENV is okay but it has two problems.

First, 2ENV induces a pop when its attack or release is sharp.  You can hear this pop in many places.  It means that 2ENV isn't very usable for fast envelopes.  Perhaps 2ENV needs a filter on the power line.

Second, 2ENV has bug when working with MIDI modules such as MCO/1 or MCC/4.  These devices often drop a gate and then quickly reengage it, due to instructions common to many MIDI devices such as groove boxes.  But when this happens, 2ENV doesn't see the new gate, and it just stops.   I built a GRAINS firmware, called RESPITE, to work around this bug in 2ENV.

Otherwise, 2ENV seems to more or less work as expected, through its attack and decay shapes are a bit odd for an exponential envelope.  It is unfortunate that a linear envelope isn't offered.

Some stretch ideas: 

- 2ENV only offers dual AR and ASR envelopes.  It'd have been nice to see RAMP envelopes.  For example, Knob 1 could set the final value while Knob 2 sets the rate and initial value.  Sweeping Knob 2 left to right would be: 0V fast rising ... 0V slow rising ... no movement ... 5V slow falling ... 5V fast falling.

- It'd also have been nice if it also offered a dual ADSR envelopes each of which used the same four parameters.  In the same vein, a dual AHR.

[AE Modular Wiki Entry on 2ENV](https://wiki.aemodular.com/pmwiki.php/AeManual/2ENV)  

### SEQ16

The STEP16 is an odd sequencer.  You set 5 step values to precise CV outputs.  Then you set each of the the 16 steps to one of those 5 values or to 0 volts.  When a step is reached, its respective CV value is output as well as a gate.  One of the step values is an "accent", and it can be set to ratchet (flam) and to output a special accent gate.  You can also set a step to "Pause" (rest), which drops the gate.  A dedicated 8/16 length switch changes the sequence length, or you can do the same with sequence step trigger outs going back into the reset trigger.

I can think of a number of improvements to this design.

- The SEQ8 has several advantages over the SEQ16 which the SEQ16 could really benefit from.  First, though the SEQ8 also has a similar step-value scheme (with just 4 step values plus 0V), you can also output raw CV values from each step.  Why can't you do this in the SEQ16?  The SEQ8 also has four running modes: forward, backward, ping-pong, and random.  The SEQ16 only has forward.

- The 8/16 length switch is useless.  But imagine if, when switching it to 8, the SEQ16 changed from a 16-step sequencer to **two parallel 8-step sequencers**!

- There is no way to set gate length: it's always maximal.  This is unfortunate.  You might say: yeah, but to set the gate length you need to know the whole length, which is hacky.  But you can set accent ratchets already, and they also require the whole length.

I note that the SEQ16 requires a fairly slow trigger to get it to advance.  A fast up/down trigger will be ignored.

[AE Modular Wiki Entry on SEQ16](https://wiki.aemodular.com/pmwiki.php/AeManual/SEQ16)  

### 2S&H

2S&H works well. It offers dual Sample and Hold, though unfortunately doesn't offer the rarer (and less useful) Track and Hold, which its micromodule cousin does offer.

*Repeated from NOISE above:* A high percentage of the people who buy NOISE will also buy 2S&H.  Both of them are very simple modules which pair naturally.  They should not be offered separately, but instead should be offered as a single module which does filtered noise, Sample and Hold, Track and Hold, and random LFO.    Indeed, there is a micromodule which does a simple version of this.  I have GRAINS firmware (RUCKUS and STOCHASTIC) which can do much of this as well.

[AE Modular Wiki Entry on 2S&H](https://wiki.aemodular.com/pmwiki.php/AeManual/SAMPLEHOLD)    
[AE Modular Wiki Entry on NOISE](https://wiki.aemodular.com/pmwiki.php/AeManual/NOISE)

### TRIQ164

The TRIQ164 is a 16-step 4-track drum sequencer with ratchets (flams).  Each step can either output a single trigger or a ratcheted set of triggers.  The trigger patterns are automatically saved to EEPROM 30 seconds after inactivity after the last change.  That's nice but not documented.

Suggested improvements:

- You can only mute two of the four tracks!  This was a significant oversight.  To make space, I would have gotten rid of the the B.STOP input, which is useless, and the Repeat CV input.

- It also would have been useful to manually solo a track.  This could have been done by pressing combinations of track buttons, perhaps like this:

 - 1+2		Solo 1
 - 2+3		Solo 2
 - 3+4		Solo 3
 - 1+4		Solo 4
 - 2+4		Mute All
 - 1+3		Unsolo

  Of course, adding a "solo" button and some solo triggers would also be welcome.

I note that the TRIQ164 requires a fairly slow trigger to get it to advance.  A fast up/down trigger will be ignored.

[AE Modular Wiki Entry on TRIQ16](https://wiki.aemodular.com/pmwiki.php/AeManual/TRIQ16)

### AEUROPI

Faux Cyrillic's Aeuropi is a port of the Europi module, a Raspberry Pi Pico based programmable CV controller. It has a bunch of built-in programs. It works well, though I have gotten it into strange states from which elaborate reset procedures were necessary.

The Aeuropi can be programmed using MicroPython; but it's not very easy.  You have to remove the module, (maybe?) remove the Pi, attach it to a computer, and download a new image.  If you were interested in hobby module development, I'd suggest GRAINS instead.

The Aeuropi comes with two faceplates: a plain but overly shiny white plate, and a matte black plate that is embossed with satanic imagery on the front and the text "Hail Satan Worship Doom" (or some such) on the back.  I imagine it is meant as a joke, but I'd have preferred a plain, matte black plate.

I had the stock Raspberry Pi Pico in my Aeuropi go bad on me after a firmware update, and had to replace it (the new one works fine with the update).

[AEuroPi's Vendor](https://crangle.page/)  

### QUANTIZER

Quantizer does a pretty decent job of quantizing pitches to certain scales.  There are some strange scale choices.  For example, there is Hungarian Minor (called "Gypsy Minor").  But there's no major or minor blues scale!  

Quantizer's range is limited to about 4 octaves; I suspect this is a 5V headroom issue.

[AE Modular Wiki Entry on QUANTIZER](https://wiki.aemodular.com/pmwiki.php/AeManual/QUANTIZER)

### MIDICLOCK (or BEAT) DIVIDER

The MIDIClock / Beat Divider is essential if you're receiving clock from MIDI.  MIDI clock is 24PPQ and must be divided by 6 to clock sequencers meant for 16th notes (that is, 4PPQ).  Beat Divider does this and much more, including dotted notes and triplets, and lots of other divisions.

There is one problem with MIDIClock / Beat Divider: it doesn't reset properly from the START signal on the MB/1.  Likely the MIDIClock / Beat Divider needs a very slow trigger and the MB/1's START trigger is too fast for it.  However, strangely and happily enough the R/S (run-stop gate) output on the MB/1 **will** trigger the MIDIClock / Beat Divider Reset, even though it's not a trigger!  So there you go.

Furthermore it appears that if you're using R/S, when you stop the MIDI clock, the MIDIClock / Beat Divider is reset again!  This is because it appears that the MIDIClock / Beat Divider RESET will reset on **either** a rising **or** a falling edge.

[AE Modular Wiki Entry on MIDICLOCK (or) BEAT DIVIDER](https://wiki.aemodular.com/pmwiki.php/AeManual/BEATDIVIDER)

### MM-DIV

MM-DIV works great for making lots of kinds of beat division patterns.  But if you had to pick between it and the MIDIClock / Beat Divider to do MIDI beat division **specifically**, pick the MIDIClock / Beat Divider, which has many more and better options MIDI beat division despite what the Wiki says.

[AE Modular Wiki Entry on MM-DIV](https://wiki.aemodular.com/pmwiki.php/AeManual/MM-DIV) (which is incorrect)

### 6TRIGBUTTON and 6MUTE

These modules both provide six buttons and connect or break connections depending on how the buttons are pressed.

I appreciate the fact that 6MUTE has a slight ramp to prevent pops in audio, and that 6TRIGBUTTON is debounced.  But even so, the functionality of these two modules should be in the same module.  That module should allow you to do one of three things depending on a switch setting:

- Toggle a connection when a button is pressed
- Connect or cut a connection depending on whether the button is pressed.
- Solo a connection (disconnecting all 5 other ones)

I note that Tangible Waves offers the 3VCSWITCH, which among other things allows three toggle buttons.

[Tangible Waves Page on TRIGBUTTON](https://www.tangiblewaves.com/store/p201/6TRIGBUTTON.html)  
[AE Modular Wiki Entry on 6MUTE](https://wiki.aemodular.com/pmwiki.php/AeManual/6MUTE)  
[AE Modular Wiki Entry on 3VCSWITCH](https://wiki.aemodular.com/pmwiki.php/AeManual/3VCSWITCH)

## Micro Modules

### Modules and uBASE

Be warned that uBASE cannot be installed in the first two slots on the left side of the case due to interference with the ribbon distribution crossbar.

The MicroModules are wobbly.  I don't think the design is very stable for holding them in.  A better approach would be to include a few (maybe two?) dummy header pins at the bottom left of the MicroModule, which would hold it tight.

[AE Modular Wiki Entry on uBASE](https://wiki.aemodular.com/pmwiki.php/AeManual/UBASE)  

### MCO/1

Discussed earlier in Oscillators and Sound Generators.

### MCC/4

This module can, among other things, output the CV for pitches received as notes, as well as gate for note on/off, and CV for volume. Unfortunately the pitch CV is limited in its range due to, as I understand it, restrictions on the chip.

The MCC/4 has the same exact issue with 2ENV that the MCO/1 does: its fast gate down/up may be missed by 2ENV due to the 2ENV bug.

[Wonkystuff's MCC/4 Page](https://wonkystuff.net/product/mcc-4/)

### MTR/8

Seem to trigger well.

[Wonkystuff's MTR/8 Page](https://wonkystuff.net/product/mtr-8)

### u4BUFFER

Works great.  Except for linkage options, it does everything that 4BUFFER does.

[AE Modular Wiki Entry on u4BUFFER](https://wiki.aemodular.com/pmwiki.php/AeManual/U4BUFFER)  

### u2SIGAMP

Works very well.

[AE Modular Wiki Entry on u2SIGAMP](https://wiki.aemodular.com/pmwiki.php/AeManual/u2SIGAMP)  

### uS&H/RNDM/NOISE

This isn't a full replacement for NOISE + S&H.  The Noise is very limited in its options, and there's no filter.  But it's a micromodule, you can't fit much more there.  It does have Track and Hold though.

[AE Modular Wiki Entry on uS&H/RNDM/NOISE](https://wiki.aemodular.com/pmwiki.php/AeManual/USH-RNDM-NOISE)  

### uATTENUVERT

Seems to work just fine.

[AE Modular Wiki Entry on uATTENUVERT](https://wiki.aemodular.com/pmwiki.php/AeManual/uATTENUVERT)  

### u2VCA, uBITCRSH

I have these but have not had enough time to say much about them yet.

[AE Modular Wiki Entry on u2VCA](https://wiki.aemodular.com/pmwiki.php/AeManual/U2VCA)  
[AE Modular Wiki Entry on uBITCRSH](https://wiki.aemodular.com/pmwiki.php/AeManual/uBITCRSH)  

## The Case

I don't own a QuickSwap Case.  As I understand it, the QuickSwap case doesn't allow for signals between cases.

### Case Hinges

I'm not convinced these do an adequate job.  It'd have been better to provide kits which attached to the *top* of the upper case and had long legs to prop it up.

[Tangible Waves's product page for the more common case hinge](https://www.tangiblewaves.com/store/p102/AE_modular_case_hinge_kit_Type_1.html)

### Screws

I think the older cases came with different screws than the new cases come with.  They had a slightly wider teeth diameter (that is, the difference between the "major" and "minor" diameters was bigger) and so bite better, especially when attaching to uBASEs.  It'd be nice to go back to the earlier screws with more bite.

### Blank Plates

The blank plates now come with polyester standoffs.  These easily strip.  The metal standoffs were much better.

[Tangible Waves's product page for the 1U blank plate](https://www.tangiblewaves.com/store/p31/Blank_module_1U.html)
