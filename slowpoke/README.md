# Slowpoke

Slowpoke is an EXTREMELY slow loopable AR envelope with a random option.  How slow?  Its fastest attack 
or release rate (besides instantaneous) is a bit over 3 seconds, and its slowest rate is 29.5 hours.
Stochastic is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager
in your Arduino IDE.

You set the attack and release rates with the POT 1 and POT 2.  Thereafter when it is triggered,
Slowpoke will slowly start rising linearly from a MINIMUM value until it reaches a MAXIMUM value, then 
will slowly drop back down linearly to the MINIUMUM value.  When it reaches the MINIMUM value again, it
issues a trigger out to END and stays at the MINIUMUM value.

If you would like a one-shot envelope, you can do this by sending a trigger to TRIGGER.  If you would like a 
looping envelope, attach the END to one of the TRIGGERs, and send a trigger to the other TRIGGER.
When the envelope has finished, END will trigger the envelope to start again.  You stop the looping by 
disconnecting END from the TRIGGER -- the envelope will stop after it finishes its current attack and decay. 

You specify the MINIMUM and MAXIMUM with POT 3.  If POT3 is low, then the MINIMUM is 0, and POT3
sets the MAXIMUM value.  When the pot is roughly 40% the highest possible MAXIMUM value (about 4.25 volts)
is reached.  Between 40% and 60%, the minimum stays at 0 and the maximum stays at the highest value.
Then beyond 60% the MINIMUM begins to rise from 0 until it too reaches 4.25 volts when the pot is 100%.

If POT 1 and POT 2 are BOTH set to MAXIMUM ("Instantaneous"), then Slowpoke turns into a RANDOM RAMP:
When you trigger it to start, it will pick a random voltage value and slowly move towards that value.  When
it reaches the value, it will issue an END trigger.  If you trigger it gain (looping for example), it
will start at that position and move towards a new random value, and so on.  The rate of the ramp is
now determined by POT 3.

The rate of the attack and release (or ramp) depend in part on the settings of POT 1 and POT 2, and in
part on the distance that Slowpoke must cover between the current MINIMUM and MAXIMUM.  If they are
far apart (MINIMUM = 0, MAXIMUM = 4.25 volts or so, then the rate values that POT 3 sets are as follows:

    6.4843750 Seconds
    12.968750 Seconds
    25.937500 Seconds
    51.875000 Seconds
    1.7291666 Minutes
    3.4583333 Minutes
    6.9166665 Minutes
    13.833333 Minutes
    27.666666 Minutes
    55.333332 Minutes
    1.8444444 Hours
    3.6888888 Hours
    7.3777776 Hours
    14.755555 Hours
    29.511110 Hours
    Instantaneous

If the distance between MINIMUM and MAXIMUM is smaller than this, then the rates are proportionally less.

## Grains Bug
There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the 
switch to "Man", then the range of the Pot is correct.  But if you set the switch 
to "In 1" (or "In 2"), then the range of the Dial is at maximum at about the 
2 o'clock position on the Dial.  Beyond that it stays at maximum.


CONFIGURATION

IN 1            Attack Rate CV
IN 2            Decay Rate CV
IN 3            Trigger 1
AUDIO IN (A)    Trigger 2
AUDIO OUT       Output
DIGITAL OUT (D) End

POT 1           Attack Rate  [If you're not using the CV, set the switch to MAN.  Note GRAINS bug]

POT 2           Decay Rate  [If you're not using the CV, set the switch to MAN.  Note GRAINS bug]

POT 3           Shape:   [Max = 0V, Min = 0V <-------> Max = 4.25V, Min = 0 <--------> Max = 4.25V, Min = 4.25V]
                Or Random Ramp Rate (if Attack and Decay both Max)


## Configuration

#### IN 1
Attack Rate CV
#### IN 2
Decay Rate CV
#### IN 3
Trigger 1 
#### AUDIO IN (A)
Trigger 2
#### AUDIO OUT
Output
#### DIGITAL OUT (D) 
End
#### POT 1
Attack Rate  

[If you're not using the CV, set the switch to MAN.  Note GRAINS bug]

#### POT 2
Decay Rate  

[If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
#### POT 3
Shape

`Max = 0V, Min = 0V <-------> Max = 4.25V, Min = 0 <--------> Max = 4.25V, Min = 4.25V`
                
(Or Random Ramp Rate, if Attack and Decay both Max)


