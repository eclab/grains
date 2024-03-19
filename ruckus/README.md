# Ruckus

Ruckus is a noise and sample and hold generator.  Ruckus is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

## Making Noise 

Ruckus can make:

- Pure white noise
- Pure pulse noise
- Pulse noise pushed through a resonant 12db lowpass filter with adjustable cutoff
- Pulse noise pushed through a resonant 12db high pass filter with adjustable cutoff
- Pulse noise pushed through a resonant 12db bandpass filter with adjustable cutoff
- Sample and Hold of input CV
- Sample and Hold of random values
- Trigger and Hold of input CV
	
## About Pulse Noise

This is just noise which consists entirely of maximum (5V) or minimum (0V) values.  It sounds
something like white noise but I can make it much louder.

## About Sample and Hold

Ruckus can do three kinds of sample and hold:

- Random Sample and Hold: when the TRIGGER/GATE is raised, it will sample and hold a RANDOM value
- Sample and Hold: when the TRIGGER/GATE is raised, it will sample and hold a value from IN 3
- Track and Hold: as long as the TRIGGER/GATE is raised, it will sample and use the value from IN3.
  as soon as the TRIGGER/GATE is lowered, it will use the last sampled value.

In these cases, IN1 serves to SCALE the sample and hold signal, and IN2 serves to SHIFT it. A shift of 0 is dead center.

Note that Sample and Hold only goes up to under 4V or so, due to limitations of Mozzi.  If IN3
is inputting a value larger than this, it will be clipped.

Looking for a random wanderer rather than a traditional sample and hold?  Try STOCHASTIC.  


## Configuration

#### IN 1
12DB Filter Cutoff Frequency CV, or Scale CV
#### IN 2
12DB Resonance CV, or Shift CV
#### IN 3
Sample and Hold Input
#### AUDIO IN (A)
[UNUSED]
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
Trigger / Gate
#### POT 1
12DB Filter Cutoff Frequency, or Scale

[If CV is unused, set the switch to MAN]
#### POT 2
12DB Resonance, or Shifrt

[If CV is unused, set the switch to MAN]
#### POT 3
Noise Type: White, Pulse, Low Pass, High Pass, Band Pass, Random Sample and Hold, Input Sample and Hold, Input Track and Hold
