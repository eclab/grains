# Pluck

Pluck is a Karplus-Strong string plucking thing.  You pluck a string by providing a PITCH
and then triggering the TRIGGER.  Pluck maintains up to 4 voices of strings being plucked at 
one time.  No two plucks will sound exactly alike, nor will they have the same decay nor volume.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager
in your Arduino IDE.

There are three options for plucking.  First, you can set the DECAY of the pluck.  Note that higher
note plucks will decay much faster regardless, but you can stretch them out this way.  Second, you
can set the GAIN (the volume).  Second, you can set the ATONALITY of the pluck: how much
it will sound like a string versus an atonal drum or crash sound.  
 Basically more atonality rapidly adds more and more noise to the sound.  Third, you can set the DECAY of the pluck.  Longer decays will sound increasingly metallic and when they pile up you'll need to reduce the GAIN

You can't have both ATONALITY and DECAY on the IN 2 knob and input: one is relegated to CV via IN 3.  
By default ATONALITY is on IN 3 and DECAY gets the IN2 knob/input.  But if you'd like to swap that,
just uncomment a #define in the code.

Note that if you change the atonality, pitch, gain, or decay, it will not affect the currently plucked
notes: it only affects later notes.  This trick helps the computational efficiency of the program and
thus the quality of the note sound: but in fact changing any of these four elements in real time adds
weird and undesirable artifacts in Mozzi, so you wouldn't want it anyway.


## Tuning

Unlike other Mozzi projects, Pluck has to compute the wave array size immediately in order to play it.  This means 
it that has to make a snap judgment about the current pitch, and sometimes it's wrong because GRAINS / Mozzi
are very sloppy and noisy in providing proper pitch information.  So occasionally it'll be off when you play it.
I will try to work on it, but expect Pluck to not be exactly on pitch every time you do a pluck.  I can make it
more accurate, but at the expense of much more latency (and Pluck has plenty of latency already).

Also, you can adjust tuning, but note that Karplus-Strong is limited in its ability precisely match
a given pitch.  High plucks will be far, far less pitch accurate than low plucks.  The lowest 
pluck you can do is 64 Hz (roughly a C two octaves below middle C).



## Adjusting Tuning and Tracking

Grains's Inputs track 1.3V/octave, not 1V/octave: we'll need to scale them to track properly.  
To do this, you can adjust the Pitch CV Scaling on Pot 1.  This GRAINS program is set up to play 
the C three octaves below Middle C when it receives 0V.  You should be able to use Pot 1 to scale 
the pitch such that high Cs play in tune as well.  Once you have things tracking well, you can 
then use the Pitch Tune (Audio In) to tune 0V to some other note.  Note that as GRAINS resistors 
warm up, the scaling will change and you will need to adjust the tracking again, at least until 
they are fully warmed up.

By default the note corresponding to 0V is C0, three octaves below middle C, that is MIDI note 24, 
or 32.7 Hz.  You can customize the tuning for this Grains program but only UP.  This can be done 
in two ways.  First, you can add pitch to the tuning with a CV value to Audio In.  Second, you 
can transpose the pitch up by changing the TRANSPOSE\_OCTAVES and/or TRANSPOSE\_SEMITONES #defines 
in the code to positive integers.


## Grains Bug

There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the switch to "Man", 
then the range of the Pot is correct.  But if you set the switch to "In 1" (or "In 2"), then 
the range of the Dial is maximum (1.0 or 5V for our purposes) at about the 2 o'clock position 
on the Dial.  Beyond that it stays at 1.0.

## Configuration

#### IN 1
Pitch CV
#### IN 2
Decay (or Atonality) CV
#### IN 3
Atonality (or Decay)
#### AUDIO IN (A)
Pitch Tune
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
Trigger
#### POT 1
Pitch CV Scaling	[Set the switch to IN1]
#### POT 2
Decay (or Atonality)
[If you're not using Decay/Atonality CV, set the switch to MAN]
#### POT 3
Gain

