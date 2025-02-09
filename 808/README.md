# 808

808 is a drum sampler with up to 9 voices and up to 5 triggers.  There are many options depending on the number of voices you are using.  808 comes with an assortment of 8-bit u-Law TR-808, TR-909, and LinnDrum drum samples.  808 is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

To set up 808, you just have to do two things:

- State the sample files you're using and

- State which FORMAT you're using.  The FORMAT is the layout of the 808's interface. There are 13 formats available, and they dictate both how many samples will be used which will be grouped to the same trigger (you can only play one sample from a group on a given trigger at a given time).

FORMATS differ in capability based on the number of samples involved so as to maximize GRAINS's limited interface.  FORMAT\_1 has only one sample but lets you specify the start and end sample points, and the pitch.  FORMAT\_2 has two samples but you can specify their pitch; similarly for FORMAT\_3 and FORMAT\_4.  FORMAT\_5 has up to 5 triggers.  FORMATS 6, 7, 7A, 8, and 9 have increasing numbers of samples, but they are crammed into selectable groups with smaller and smaller numbers of triggers.  

A note about pitch: Mozzi's sampler facility is very primitive, and changes in pitch will have a lot of aliasing and other artifacts.

### Drum Samples

The drum sample libraries come from various open content libraries, and I have included the original sound files as well.  Also, Roger Linn has kindly given me permission to use the LinnDrum samples.

### Memory Allocation

GRAINS does not have much memory!  You can cram in 9 samples if they're very short: toms and kicks are not short, you won't get many of those in.

808 provides about 26000 bytes of memory for your samples.  The total byte length of your samples cannot exceed this amount.

### Computational Power

As you increase the number of triggers (to say, 4 or 5), Mozzi will start to struggle to keep up.  You might occasionally got a little click.  You can reduce the clicks by reducing the CONTROL\_RATE to as low as 64, but the drum machine will start to get sloppy in its response to triggers: 64 means that it checks for triggers only 64 times a second, or every 15 ms.

### A note about the DIGITAL OUT Pin

808 uses the Digital Out pin as its first trigger input.  This pin is different from the others.  If you leave a bare disconnected wire in this pin, it will cause GRAINS to receive noise which it will interpret as a bunch of triggers, resulting in your kick  drum (or whatever) going nuts.  So disconnect the wire entirely from the socket, or plug the wire into *something* else at the other end.


## Setting the Sample Files

You set the sample files by setting up to 9 #define statements in the code to the filenames of the samples.   Each sample file is a ".h" file which has been converted from a WAV file using Mozzi's tools, and slightly tweaked so it can be loaded by 808 easily.  I provide a bunch of examples. 

You can load samples as **8-bit signed** or **8-bit u-Law**.  u-Law samples have a **much lower noise floor**, and I strongly urge you to use them instead.  All the samples in your GRAINS module must be one or the other format: you cannot mix them.  You specify which format as a #define in the code.

Is a sample too loud compared to the others?  You can adjust the relative gain of each sample by setting certain #defines in the code.



## Setting the Format

There are 13 formats, and they dictate the number of samples used, how they're grouped, and how the interface is presented.   You set the format with a #define in the code.  Here is a summary of the formats:

- **FORMAT\_0** \
A single sample, with no features, intended to reduce noise as much as possible
- **FORMAT\_1** \
A single sample, plus editable start/end points and pitch control
- **FORMAT\_2** \
Two samples, each with pitch control, plus total volume control
- **FORMAT\_3** \
Three samples, the first two with pitch control, plus total volume control
- **FORMAT\_4** \
Four samples, the first one with pitch control, plus total volume control
- **FORMAT\_5** \
Five samples, plus total volume control.  Has 5 triggers: thus not for TRIQ164 or GRAINS BEATS
- **FORMAT\_5A** \
Five samples, two of which share a trigger, plus total volume control.  An alternative to FORMAT\_5 for people with 4-trigger step sequencers like TRIQ164 or GRAINS BEATS 
- **FORMAT\_6** \
Six samples and four triggers, plus total volume control: the last three samples share a trigger
- **FORMAT\_7** \
Seven samples and four triggers, plus total volume control: the last four samples share a trigger
- **FORMAT\_7A** \
An alternative to FORMAT\_7, with three triggers, plus total volume control.  The last three samples share a trigger, and the three before that share another
- **FORMAT\_8** \
Eight samples and three triggers, plus total volume control.  The last four samples share a trigger, and the three before that share another
- **FORMAT\_9** \
Nine samples and three triggers, plus total volume control.  The last four samples share a trigger, and the four before that share another
- **FORMAT\_10** \
One sample and one trigger, but the sample produces a chord.

## Configurations

Each format has a different configuration.  Yes, I know the pins in many of the configurations seem arbitrarily ordered.  Believe me, there's a logic to it internally: it's got to be this way to work at all in some cases.

### FORMAT\_0
In this format there is a single sample, SAMPLE\_1.  There are no options: the goal is to reduce noise as much as possible.  Additionally, when Drum 1 is played, TRIGGER OUT is output.  This allows you to line up other drums (like KICK) precisely, since Mozzi has some degree of latency.

    POT 3                UNUSED
    POT 2                UNUSED
    POT 1                UNUSED
    GATE OUT             TRIGGER 1
    AUDIO IN             TRIGGER OUT
    IN 3                 UNUSED
    IN 2                 UNUSED
    IN 1                 UNUSED

### FORMAT\_1
In this format there is a single sample, SAMPLE\_1.  You can change its pitch and start and end points with the pots.  Additionally, when Drum 1 is played, TRIGGER OUT is output.  This allows you to line up other drums (like KICK) precisely, since Mozzi has some degree of latency.

    POT 3       END 1      [If START > END, they are swapped]    [Set to MAN]
    POT 2       START 1    [If START > END, they are swapped]    [Set to MAN]
    POT 1       PITCH 1    [CENTER: Original Pitch]
    GATE OUT    UNUSED
    AUDIO IN    TRIGGER OUT
    IN 3        TRIGGER 1
    IN 2        UNUSED
    IN 1        PITCH CV 1

### FORMAT\_1
In this format there is a single sample, SAMPLE\_1.  You can change its pitch and start and end points with the pots.  Additionally, when Drum 1 is played, TRIGGER OUT is output.  This allows you to line up other drums (like KICK) precisely, since Mozzi has some degree of latency.

    POT 3       END 1      [If START > END, they are swapped]    [Set to MAN]
    POT 2       START 1    [If START > END, they are swapped]    [Set to MAN]
    POT 1       PITCH 1    [CENTER: Original Pitch]
    GATE OUT    UNUSED
    AUDIO IN    TRIGGER OUT
    IN 3        TRIGGER 1
    IN 2        UNUSED
    IN 1        PITCH CV 1

### FORMAT\_2
There are two samples, SAMPLE\_1 and SAMPLE\_2.  You can change the pitch of each one and the total volume with the pots  Set the volume such that when the two  samples play simultaneously you don't have pops or clipping.  Additionally, when Drum 1 is played, TRIGGER OUT is output.  This allows you to line up other drums (like KICK) precisely, since Mozzi has some degree of latency.

    POT 3       VOLUME
    POT 2       PITCH 2    [CENTER: Original Pitch]    [Note GRAINS BUG ABOVE]
    POT 1       PITCH 1    [CENTER: Original Pitch]    [Note GRAINS BUG ABOVE]
    GATE OUT    TRIGGER 1
    AUDIO IN    TRIGGER OUT
    IN 3        TRIGGER 2
    IN 2        PITCH CV 2
    IN 1        PITCH CV 1 

### FORMAT\_3
There are three samples, SAMPLE\_1 through SAMPLE\_3.  You can change the pitch of SAMPLE\_1 and SAMPLE\_2 with the pots, as well as the total volume. Set the volume such that when the two samples play simultaneously you don't have pops or clipping.

    POT 3       VOLUME
    POT 2       PITCH 2    [CENTER: Original Pitch]    [Note GRAINS BUG ABOVE]
    POT 1       PITCH 1    [CENTER: Original Pitch]    [Note GRAINS BUG ABOVE]
    GATE OUT    TRIGGER 1
    AUDIO IN    TRIGGER 2
    IN 3        TRIGGER 3
    IN 2        PITCH CV 2
    IN 1        PITCH CV 1 

### FORMAT\_4
There are four samples, SAMPLE\_1 through SAMPLE\_4.  You can change the pitch of SAMPLE\_1 and the total volume with the pots.
Set the volume such that when the two samples play simultaneously you don't have pops or clipping.

    POT 3       VOLUME
    POT 2       UNUSED     [SET TO MAXIMUM, AND SET SWITCH TO IN2]
    POT 1       PITCH 1    [CENTER: Original Pitch]    [Note GRAINS BUG ABOVE]
    GATE OUT    TRIGGER 1
    AUDIO IN    TRIGGER 2
    IN 3        TRIGGER 3
    IN 2        TRIGGER 4
    IN 1        PITCH CV 1 

### FORMAT\_5
There are five samples, SAMPLE\_1 through SAMPLE\_5.  You can change the total volume with a pot.
Set the volume such that when the two samples play simultaneously you don't have pops or clipping.

    POT 3       VOLUME
    POT 2       UNUSED    [SET TO MAXIMUM, AND SET SWITCH TO IN2]
    POT 1       UNUSED    [SET TO MAXIMUM, AND SET SWITCH TO IN1]
    GATE OUT    TRIGGER 1
    AUDIO IN    TRIGGER 2
    IN 3        TRIGGER 3
    IN 2        TRIGGER 4
    IN 1        TRIGGER 5 

### FORMAT\_5A
There are five samples, SAMPLE\_1 through SAMPLE\_5.  You can change the total volume with a pot.  Set the volume such that when the two samples play simultaneously you don't have pops or clipping. There are only FOUR triggers.  SAMPLE\_4 and SAMPLE\_5 share a trigger: a  pot determines which sample plays.  The purpose of this format is to be more convenient for people with 4-trigger sequencers such as TRIQ164 or BEATS on the GRAINS.

    POT 3       VOLUME
    POT 2       UNUSED    [SET TO MAXIMUM, AND SET SWITCH TO IN2]
    POT 1       SELECT SAMPLES FOR TRIGGER 4 -> { 4, 5 }          [SET TO MAN]
    GATE OUT    TRIGGER 1
    AUDIO IN    TRIGGER 2
    IN 3        TRIGGER 3
    IN 2        TRIGGER 4
    IN 1        UNUSED 


### FORMAT\_6
There are six samples, SAMPLE\_1 through SAMPLE\_6.  You can change the total volume with a pot.  Set the volume such that when the two samples play simultaneously you don't have pops or clipping. There are only FOUR triggers.  SAMPLE\_4, SAMPLE\_5, and SAMPLE\_6 share a trigger: a pot determines which sample plays.  

    POT 3       VOLUME
    POT 2       UNUSED    [SET TO MAXIMUM, AND SET SWITCH TO IN2]
    POT 1       SELECT SAMPLES FOR TRIGGER 4 -> { 4, 5, 6 }       [SET TO MAN]
    GATE OUT    TRIGGER 1
    AUDIO IN    TRIGGER 2
    IN 3        TRIGGER 3
    IN 2        TRIGGER 4
    IN 1        UNUSED 

### FORMAT\_7
There are seven samples, SAMPLE\_1 through SAMPLE\_7.  You can change the total volume with a pot.  Set the volume such that when the two samples play simultaneously you don't have pops or clipping. There are only FOUR triggers.  SAMPLE\_4, SAMPLE\_5, SAMPLE\_6, and SAMPLE\_7 share a trigger: a pot determines which sample plays.  If you would like to spread the sharing love over two triggers, try FORMAT\_7A instead.

    POT 3       VOLUME
    POT 2       UNUSED    [SET TO MAXIMUM, AND SET SWITCH TO IN2]
    POT 1       SELECT SAMPLES FOR TRIGGER 4 -> { 4, 5, 6, 7 }    [SET TO MAN]
    GATE OUT    TRIGGER 1
    AUDIO IN    TRIGGER 2
    IN 3        TRIGGER 3
    IN 2        TRIGGER 4
    IN 1        UNUSED 

### FORMAT\_7A
There are seven samples, SAMPLE\_1 through SAMPLE\_7.  You can change the total volume with a pot.  Set the volume such that when the two samples play simultaneously you don't have pops or clipping. There are only THREE triggers.  SAMPLE\_2, SAMPLE\_3, SAMPLE\_4 share a trigger, and SAMPLE\_5, SAMPLE\_6, and SAMPLE\_7 share a different trigger.  Two pots determine which samples play on these triggers.  If you would like to group more samples on one trigger and off the other, try FORMAT\_7 instead.

    POT 3       VOLUME
    POT 2       SELECT SAMPLES FOR TRIGGER 2 -> { 2, 3, 4 }      [SET TO MAN]
    POT 1       SELECT SAMPLES FOR TRIGGER 3 -> { 5, 6, 7 }      [SET TO MAN]
    GATE OUT    TRIGGER 1
    AUDIO IN    TRIGGER 2
    IN 3        TRIGGER 3
    IN 2        UNUSED
    IN 1        UNUSED 

### FORMAT\_8
There are eight samples, SAMPLE\_1 through SAMPLE\_8.  You can change the total volume with a pot.  Set the volume such that when the two samples play simultaneously you don't have pops or clipping. There are only THREE triggers.  SAMPLE\_2, SAMPLE\_3, SAMPLE\_4 share a trigger, and SAMPLE\_5, SAMPLE\_6, SAMPLE\_7, and SAMPLE\_8 share a different trigger.  Two pots determine which samples play on these triggers.

    POT 3       VOLUME
    POT 2       SELECT SAMPLES FOR TRIGGER 2 -> { 2, 3, 4 }      [SET TO MAN]
    POT 1       SELECT SAMPLES FOR TRIGGER 3 -> { 5, 6, 7, 8 }   [SET TO MAN]
    GATE OUT    TRIGGER 1
    AUDIO IN    TRIGGER 2
    IN 3        TRIGGER 3
    IN 2        UNUSED
    IN 1        UNUSED 

### FORMAT\_9
There are nine samples, SAMPLE\_1 through SAMPLE\_9.  You can change the total volume with a pot.  Set the volume such that when the two samples play simultaneously you don't have pops or clipping. There are only THREE triggers.  SAMPLE\_2, SAMPLE\_3, SAMPLE\_4, and SAMPLE\_5 share a trigger, and SAMPLE\_6, SAMPLE\_7, SAMPLE\_8, and SAMPLE\_9 share a different trigger.  Two pots determine which samples play on these triggers.

    POT 3       VOLUME
    POT 2       SELECT SAMPLES FOR TRIGGER 2 -> { 2, 3, 4, 5 }   [SET TO MAN]
    POT 1       SELECT SAMPLES FOR TRIGGER 3 -> { 6, 7, 8, 9 }   [SET TO MAN]
    GATE OUT    TRIGGER 1
    AUDIO IN    TRIGGER 2
    IN 3        TRIGGER 3
    IN 2        UNUSED
    IN 1        UNUSED 

### FORMAT\_10
In this format there is a single sample, SAMPLE\_1, but it is played as a chord.  You can change its pitch and the chord type.  Note that the samples in the chord will be different lengths -- you'll need to make sure you envelope them. When the longest (lowest) sample note has finished playing, the OUTPUT TRIGGER will be set.

    POT 3       CHORD CHOICE
    POT 2       VOLUME
    POT 1       PITCH				[Unlike other Formats, this pitch is standard note pitch]
    GATE OUT    TRIGGER
    AUDIO IN    PITCH CV TUNE
    IN 3        OUTPUT TRIGGER
    IN 2        VOLUME CV
    IN 1        PITCH CV 

#### CHORDS

The chords are, in order:

- No Chord
- Intervals: minor 3, Major 3, 4, 5, minor 6, Major 6, minor 7, octave, octave + minor 3, octave + major 3, octave + 5
- Triads: minor, minor first inversion, minor second inversion, Major, Major first inversion, Major second inversion
- Sevenths: 7, minor 7, Major 7, diminished 7
- Octave Triads: minor + octave, Major + octave  



