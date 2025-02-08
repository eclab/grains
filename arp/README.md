# Arp

Arp is an arpeggiator.  Arp is meant to run on the AE Modular GRAINS, but it could be  adapted to any Arduino.

To use Arp, you specify the CHORD to arpeggiate and the STYLE with which to arpeggiate it.You can also optionally specify an INVERSION of the chord to shift the arpeggio.

Whenever you provide a CLOCK, the arpeggio will output as CV the next note in the STYLE. The CV will be transposed to match the scale of the current input PITCH you have provided. (the PITCH is thus the root note of scale).  You can RESET the style back to 0 at any point.

Because Arp has a bit of latency in responding to a clock, it provides its own clock out to precisely indicate when it has changed its note.  You can attach this to envelopes.  The clock out will go HIGH when Arp has changed its note after your clock in has gone high,  and will go LOW as soon as Arp detects that your clock in has gone low.

## Chords
There are 16 chords, chosen by POT 2:

| Chord			| Number of Notes in Chord (see INVERSIONS below)  |
|:---------------|:-----------------------------------------------|
| Maj 5 + 2		| 4 notes                                        |
| M6 + 2			| 5 notes                                        |
| M7 + 2			| 5 notes                                        |
| M7				| 4 notes                                        |
| 7              | 4 notes                                        |
| Maj 5			| 3 notes                                        |
| Root and 5th	| 2 notes                                        |
| min 5			| 3 notes                                        |
| m7				| 4 notes                                        |
| m7 + 2			| 5 notes                                        |
| m7 + 6 + 2		| 6 notes                                        |
| m6 + 2			| 5 notes                                        |
| min 5 + 2		| 4 notes                                        |
| dim7			   | 4 notes                                        |
| Major Scale		| 7 notes	[good for random arpeggios]            |
| Minor Scale 	| 7 notes	[good for random arpeggios]            |


## Styles
There are 12 styles:

| One Octave Styles |                                                                 |
|:------------------|:----------------------------------------------------------------|
| Up                |                                                                 |
| Down              |                                                                 |
| Up-Down           |                                                                 |
| Up-Down-+         | Plays the root at the top of the arpeggio as well as the bottom |
| Random            | Plays random notes in the chord                                 |
| Random Walk	       | Randomly wanders up or down through the chord                   |

| Two Octave Styles |                                                                 |
|:------------------|:----------------------------------------------------------------|
| Up                |                                                                 |
| Down              |                                                                 |
| Up-Down           |                                                                 |
| Up-Down-+	       | Plays the root at the top of the arpeggio as well as the bottom |
| Random            | Plays random notes in the chord                                 |
| Random Walk	       | Randomly wanders up or down through the chord                   |


## Inversions

You can invert the chord up, all the way through all its notes in the first octave. This is done by setting a #define in the code (sorry, there just aren't enough pots and inputs). The default is 0, which is "no inversion".  1 is the first inversion, and so on.  Don't go more than the number of notes in your chord (the maximum number of notes of any chord is 7, for the m7 + 6 + 2 chord and for the two scales).  See NUM NOTES IN THE CHORD in the CHORDS section above.


## Exceeding Maximum Pitch
If your arpeggio note exceeds the maximum pitch, the note will automaticlly be transposed down one or more octaves until it is within the pitch range.


## Input Pitch
Grains is odd in that the input to IN1 isn't necessarily 1V/oct.  You'll have to tune it. This is done by adjusting POT1.  I have found that if POT1 is in approximately the 2'oclock position, you'll get close to 1V/oct and you can tweak it from there.


## Output Pitch Range and Resolution

Mozzi cannot go down to 0V.  Its minimum is a little more, transposing up by about a half of a semitone.   Most oscillators can be tuned to deal with this. But GRAINS oscillators have to be manually adjusted.  So for example, if you're attaching  this program to a GRAINS oscillator like DX, you might want to change the TRANPOSE\_BITS to about -6.

Mozzi's output is capable of a range of betwen 42 and 48 notes (betwen 3.5 and 4 octaves).   This is also the quantizer's range: values above that will just get quantized to the top note.

One of the issues in using this quantizer is that GRAINS does not have a buffered: the voltage its output will produce is strongly affected by the amperage being pulled by the oscillator it's plugged into, and different AE oscillators pull different amounts. VCO is particularly bad here -- it pulls a lot of voltage, thus scaling down Quant's output so it's no longer v/oct.  555 is much better (its inputs are buffered).  And you can generally fix matters by plugging GRAINS into a buffered mult, and then attaching the buffered mult to your oscillator.  But even the buffered mults differ a bit!

I have made a few tables to match different scenarios:

1. You are plugged directly into a VCO
2. You are plugged directly into a 555
3. You are plugged directly into a uBUFFER.  The uBUFFER is attached to your oscillator(s).
4. You are plugged directly into a 4BUFFER.  The 4BUFFER is attached to your oscillator.
5. You are plugged directly into another GRAINS oscillator.  In this case, I suggest using the OUTPUT\_UBUFFER #define, setting TRANSPOSE\_BITS on the GRAINS oscillator to about -6, and tweaking the tracking via POT1.  It should work.
6. You are plugged directly into a 2OSC/d
7. You are plugged into a 2OSC: in this case, may your god have mercy on your soul.

You need to specify what you're plugged into, which will change the pitch table, using one the #defines in the code.



## Configuration

#### IN 1
Pitch In CV
#### IN 2
Chord CV
#### IN 3
Clock
#### AUDIO IN (A)
Reset
#### AUDIO OUT
Pitch Out
#### DIGITAL OUT (D) 
Clock Out
#### POT 1
Pitch In

[If using Pitch In CV, set switch to IN 1 and set pot to 2'oclock, else set to MAN]
#### POT 2
Chord

[If using Chord CV, set switch to IN 2 and set pot to 2'oclock, else set to MAN]
#### POT 3
Style
