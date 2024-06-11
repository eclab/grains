# Pluck-M

Pluck is a Karplus-Strong string plucking thing.  It's the MIDI version of PLUCK.  You pluck a string by providing a PITCH and then triggering the TRIGGER.  Pluck maintains up to 4 voices of strings being plucked at  one time.  No two plucks will sound exactly alike, nor will they have the same decay nor volume.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

# Pluck-M Versus Pluck

PLUCK-M is the MIDI version of PLUCK.  This allows it to have precise tuning, which is quite difficult for PLUCK.  It also allows PLUCK-M to free up POT1 for other duties, so now both Decay and Atonality can have their own pots.  Pluck-M also is velocity sensitive, and Pluck is not.

## Using Pluck-M

There are three options for plucking.  First, you can set the DECAY of the pluck.  Note that higher note plucks will decay much faster regardless, but you can stretch them out this way.  Second, you can set the GAIN (the volume).  Longer decays will sound increasingly metallic and when they pile  up you'll need to reduce the gain.  Third, you can set the ATONALITY of the pluck: how much it will sound like a string versus an atonal drum or crash sound.    Basically more atonality rapidly adds more and more noise to the sound.  

You can't have both ATONALITY and DECAY on the IN 2 knob and input: one is relegated to CV via IN 3.   By default ATONALITY is on IN 3 and DECAY gets the IN2 knob/input.  But if you'd like to swap that, just uncomment a #define in the code.

Note that if you change the atonality, pitch, gain, or decay, it will not affect the currently plucked notes: it only affects later notes.  This trick helps the computational efficiency of the program and thus the quality of the note sound: but in fact changing any of these four elements in real time adds weird and undesirable artifacts in Mozzi, so you wouldn't want it anyway.

Note that Karplus-Strong is limited in its ability precisely match a given pitch.  High plucks will be far, far less pitch accurate than low plucks.  The lowest  pluck you can do is 64 Hz (roughly a C two octaves below middle C).

## Backround Noise

I have found that Pluck-M sometimes has a bit of a background hum.  This hum is loud if the gain is turned far too high and a note is played which overwhelms the gain, and it's also oddly present  if the gain is very low.  But for moderate gains the hum disappears.  If you hear it, try tweaking.

## Grains Bug

There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the switch to "Man",  then the range of the Pot is correct.  But if you set the switch to "In 1" (or "In 2"), then  the range of the Dial is maximum (1.0 or 5V for our purposes) at about the 2 o'clock position  on the Dial.  Beyond that it stays at 1.0.

## MIDI

- Note On/Off
	- Pitch: All MIDI Notes C3 through G10, but high notes will be very off in pitch.
    - Velocity: 0...127 for Note On.  Velocity 0 is a Note Off

## Configuration

#### IN 1
Decay CV
#### IN 2
Atonality CV
#### IN 3
[Unused]
#### AUDIO IN (A)
[Unused]
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
MIDI In
#### POT 1
Decay	

[Set the switch to IN if you're using CV, else set to MAN]
#### POT 2
Atonality 

[Set the switch to IN if you're using CV, else set to MAN]
#### POT 3
Gain


