# Stochastic

Stochastic is a combination random walk wanderer, random-walk sample and hold, and fuzzer.  Stochastic is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

Stochastic wanders about randomly using a RATE and a VARIANCE.  It has two ways of wandering.  First, it can do a random walk: it heads towards a randomly-chosen target, and when it reaches the target, it picks a new one relative to the old target.  Or it can wander randomly about a mean, always picking new targets relative to the mean.

The mean, if you have chosen to use it, is specified by an incoming CV on AUDIO IN.

Stochastic can also do its random-walk or walk-about-mean, but output its randomly chosen targets rather than outputting its current point as it heads to them: this is essentially a sample-and-hold.

To this random generated signal it adds some degree of FUZZ.  This is then output.

Additionally when Stochastic's random-walk wanderer / sample and hold selects a new target value (as determined by the RATE), a TRIGGER is signaled.

You can reset Stochastic to pick a new target by sending it a RESET trigger.

GRAINS's output is naturally discretized to 488 steps.  That's the finest-grain it can do. So do not expect this tool to be particularly clean in its output.  Also, GRAINS's output circuitry is tuned for audio, not for CV.  As a result you should expect some small degree of waver.  There is nothing that can be done about this.


## Grains Bug

There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the  switch to "Man", then the range of the Pot is correct.  But if you set the switch  to "In 1" (or "In 2"), then the range of the Dial is at maximum at about the 2 o'clock position on the Dial.  Beyond that it stays at maximum.


## Configuration

#### IN 1
Wander / S&H Variance CV
#### IN 2
Wander / S&H Rate CV
#### IN 3
Reset 
#### AUDIO IN (A)
Mean
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
Output Trigger
#### POT 1
Wander / S&H Variance

[If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
#### POT 2
Wander / S&H Rate

[If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
#### POT 3
Fuzz and Sample & Hold

`S&H No Fuzz <------- High Fuzz --------> Wander No Fuzz`

