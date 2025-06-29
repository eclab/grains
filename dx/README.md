# DX

DX is a 2- or 3-operator FM synthesizer.  DX is meant to run on the AE Modular GRAINS,  but it could be adapted to any Arduino.

DX has four FM algorithms.  One algorithm has 2 operators but one operator has self-modulation. The other three algorithms have three operators, but you will have less control from the pots.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

## How FM Works

All you need to know is that FM in DX is organized with several sine-wave oscillators called  OPERATORS.  These operators output sound, but only some are used in the final output. If an  operator's sound is used in the final output, it is called a CARRIER.  You can have multiple carriers. Other operators make sound but that sound is used as a modulation source to change the frequency  (really the phase) of ANOTHER operator in real time, warping it into another sound wave.   These oscillators are called MODULATORS.  It is possible for a modulator to modulate another modulator, or to modulate itself.  [I'm abusing the terms modulator and carrier here to simplify things].

Each operator has a FREQUENCY.  One carrier plays the primary PITCH.  The other operator's frequencies are determined RELATIVE to that carrier, and modulators commonly have relative pitches that are always integer multiples or divisors of the carrier pitch (otherwise it doesn't sound tonal).

The connection between a modulator and the operator it is modulating has an associated value called the INDEX OF MODULATION.  This is basically how much impact the modulator has on changing the frequency of the downstream operator.  If you set it to zero, for example, it's as if they're disconnected.

Changing both the frequency and index of modulation of a modulator can have a big impact on the sound of the downstream operator. However commonly you'd only change the index of modulation in real time (with an envelope, say).

The arrangement of operators -- which operators modulate which other ones, and which operators are carriers -- is called an ALGORITHM.  This weird term stems from Yamaha.  DX offers four algorithms.  The first algorithm has a single modulator modulating a single carrier, and the modulator can also modulate itself.  The second algorithm has two modulators both modulating a single carrier.  The third algorithm has one modulator modulating two carriers.  And the fourth algorithm has a modulator modulating a second modulator, while that second modulator modulates a carrier.




## The Algorithms

You set the algorithm by changing the a #define in the code.

Confused?  Then just stick with the 2-modulator ALGORITHM 1, ignoring self modulation, to get your feet wet.

### Algorithm 1
This is a two-operator algorithm, with a MODULATOR and a CARRIER.  The CARRIER makes the final sound, and so you will specify the carrier's note pitch on POT1/IN1.  The  modulator has a RELATIVE PITCH to the carrier, one of 0.5, 1.0, 2.0, 3.0, ... 15.0, selectable on POT 3.  The modulator also has an INDEX OF MODULATION (the "amplitude" of the operator in Yamaha speak) which determines how much affect the modulator has on the carrier.  This is set with POT2/IN2.

This is the basic setup.  However the modulator can also be optionally SELF MODULATED, that is, it modulates itself.  You specify the INDEX OF SELF MODULATION with IN3.  Because this is often done with an envelope, you'll need a way to temper its maximum value.  This is done by setting the INDEX OF SELF MODULATION SCALING on AUDIO IN.   


### Algorithm 2
This is a three-operator algorithm, with TWO MODULATORS simultaneously modulating the same CARRIER.  Again, the CARRIER makes the final sound, and so you will specify the carrier's note pitch on  POT1/IN1.  Each modulator has a RELATIVE PITCH to the carrier, one of 0.5, 1.0, 2.0, 3.0, ... 15.0.   Modulator 1's relative pitch is selectable on POT 3.  Modulator 2's relative pitch is selectable on. AUDIO IN.  Again, each modulator also has an INDEX OF MODULATION (the "amplitude" of the operator in Yamaha speak) which determines how much affect the modulator has on the carrier.  For Modulator 1, this is set with POT2/IN2.  For Modulator 2, this is set with IN3. 


### Algorithm 3
This is another three-operator algorithm, with ONE MODULATOR simultaneously modulating TWO CARRIERS.  Each CARRIER makes a final sound -- they are summed -- and so you will specify each carrier's note pitch.  For Carrier 1, you specify the pitch on POT1/IN1.  Carrier 2's pitch is specified RELATIVE to Carrier on AUDIO IN, smoothly going from Carrier 1's pitch to to two octaves above.  The modulator also has a RELATIVE PITCH to the carrier, one of 0.5, 1.0, 2.0, 3.0, ... 15.0, selectable  on POT 3.  For each carrier, the modulator also has an INDEX OF MODULATION (the "amplitude" of the  operator in Yamaha speak) which determines how much affect the modulator has on the carrier.  The modulation of Carrier 1 is set with POT2/IN2.  The modulation of Carrier 2 is set with IN3.


### Algorithm 4
This is a final three-operator algorithm, with MODUATOR 2 modulating MODULATOR 1, while MODULATOR 1 is modulating the CARRIER.  The CARRIER makes the final sound and so you will specify the carrier's note pitch on POT1/IN1.  Each modulator has a RELATIVE PITCH to the carrier, one of  0.5, 1.0, 2.0, 3.0, ... 15.0.  For MODUATOR 1, this is selectable on POT 3.  For MODULATOR 2, this is selectable on AUDIO IN.  Each modulator also has an INDEX OF MODULATION (the "amplitude" of the  operator in Yamaha speak) which determines how much affect the modulator has on its downstream modulator or carrier.  The index of modulation for MODULATOR 1 is set with POT2/IN2.  The index of modulation for MODULATOR 2 is set with IN3.



## Index of Modulation Scaling

The index of modulation of MODULATOR 1 can be scaled DOWN with Pot 2.  But the index of modulation of MODULATOR 2 goes full blast (we don't have enough pots) in algorithms 2, 3, and 4.  You can at least reduce it with another #define in the code.

## Adjusting Tuning and Tracking

Grains's Inputs track 1.3V/octave, not 1V/octave: we'll need to scale them to track properly.   To do this, you can adjust the Pitch CV Scaling on Pot 1.  This GRAINS program is set up to play  the C three octaves below Middle C when it receives 0V.  You should be able to use Pot 1 to scale  the pitch such that high Cs play in tune as well.  Note that as GRAINS resistors  warm up, the scaling will change and you will need to adjust the tracking again, at least until  they are fully warmed up.

By default the note corresponding to 0V is C0, three octaves below middle C, that is MIDI note 24,  or 32.7 Hz.  You can customize the tuning for this Grains program but only UP. To do this, you  can transpose the pitch up by changing the TRANSPOSE\_OCTAVES and/or TRANSPOSE\_SEMITONES #defines  in the code to positive integers.  You can also change TRANSPOSE\_BITS: a "bit" is the minimum possible change Grains can do, equal to 1/17 of a semitone.

IMPORTANT NOTE: unlike other grains oscillators in my repository, DX does not have a separate Pitch Tune option on Audio In, because it needs to use Audio In for other purposes.  You can only use TRANSPOSE\_OCTAVES, TRANSPOSE\_SEMITONES, or TRANSPOSE\_BITS.



## Algorithm 1 (2-Operator) Configuration

#### IN 1
Pitch CV
#### IN 2
Index of Modulation CV
#### IN 3
Index of Self Modulation CV
#### AUDIO IN (A)
Index of Self Modulation Scaling [Note this is inverted: maximum Audio IN = 0 scaling ("off")]
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
[Unused]
#### POT 1
Pitch Scaling       [Set the switch to In1] 
#### POT 2
Index of Modulation Scaling
#### POT 3
Modulator Relative Pitch	[One of 0.5, 1, 2, 3 ... 15]


## Algorithm 2 (2-Modulator, 1-Carrier) Configuration

#### IN 1
Pitch CV
#### IN 2
Index of Modulation CV, Modulator 1 to Carrier
#### IN 3
Index of Modulation CV, Modulator 2 to Carrier
#### AUDIO IN (A)
Modulator 2 Relative Pitch		[One of 0.5, 1, 2, 3 ... 15]  [There is no Tune CV]
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
[Unused]
#### POT 1
Pitch Scaling       [Set the switch to In1] 
#### POT 2
Index of Modulation Scaling
#### POT 3
Modulator 1 Relative Pitch	[One of 0.5, 1, 2, 3 ... 15]


## Algorithm 3 (1-Modulator, 2-Carrier) Configuration

#### IN 1
Pitch CV
#### IN 2
Index of Modulation CV, Modulator to Carrier 1
#### IN 3
Index of Modulation CV, Modulator to Carrier 2
#### AUDIO IN (A)
Carrier 2 Relative Pitch		[Same octave as, to two octaves above, Carrier 1] [There is no Tune CV]
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
[Unused]
#### POT 1
Pitch Scaling       [Set the switch to In1] 
#### POT 2
Index of Modulation Scaling
#### POT 3
Modulator 1 Relative Pitch	[One of 0.5, 1, 2, 3 ... 15]


## Algorithm 4 (2-Modulator, 1-Carrier in SERIES) Configuration

#### IN 1
Pitch CV
#### IN 2
Index of Modulation CV, Modulator 1 to Carrier
#### IN 3
Index of Modulation CV, Modulator 2 to Modulator 1
#### AUDIO IN (A)
Modulator 2 Relative Pitch		[One of 0.5, 1, 2, 3 ... 15]  [There is no Tune CV]
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
[Unused]
#### POT 1
Pitch Scaling       [Set the switch to In1] 
#### POT 2
Index of Modulation Scaling, Modulator 1 to Carrier
#### POT 3
Modulator 1 Relative Pitch	[One of 0.5, 1, 2, 3 ... 15]


## Grains Bug

There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the switch to "Man", then the range of the Pot is correct.  But if you set the switch to "In 1" (or "In 2"), then the range of the Dial is maximum (1.0 or 5V for our purposes) at about the 2 o'clock position on the Dial.  Beyond that it stays at 1.0
