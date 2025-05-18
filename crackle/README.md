# Crackle

Crackle produces random pops and crackles like a record player or an old-time radio. Crackle is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

Crackle has two outputs: audio and digital.  Audio output generates pops at different volumes, but suffers from GRAINS's low-level PWM whine.  Digital generates clean pops, but they are all at maximum volume and the default value is 0V with the pop at 1V, which means it's not centered for audio and perhaps less useful as a result -- maybe more useful for CV?

You can control the RATE of crackling, the VARIANCE in crackling volume, and the LENGTH of a crackle.

## Variance 

At minimum, the variance sets all crackles to zero volume.  As you turn right, crackles will be soft but random in volume from one another.  As you continue to turn right, they get louder until some of them reach maximum volume.  Continuing, the maximum volume crackles will start to prevail until, at far right, about 3/4 of them are maximum volume.

## Note

Crackle is intentionally very slow in responding to pots, in an attempt to cut down a bit on the PWM whine in audio.  It'll take a bit for it to ramp up to your new request.  If you want to speed it up, try setting CONTROL_RATE to 64.



## Configuration

#### IN 1
Rate CV
#### IN 2
Gain CV
#### IN 3
[Unused]
#### AUDIO IN (A)
[Unused]
#### AUDIO OUT
Audio Crackles
#### DIGITAL OUT (D) 
Digital Crackles
#### POT 1
Rate
#### POT 2
Gain
#### POT 3
Crackle Length
