# Mode16


Mode16 adds patterns to the SEQ16 sequencer similar to SEQ8's "MODE CV" feature. You can make the sequencer go backwards in a loop, run forwards once and then stop, ping-pong, do random sequences, skip steps, insert rests, and more.

SET GRAINS TO GRAINS MODE.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

How does Mode16 perform this magic?  By controlling SEQ16's gate and manually stepping it. Mode16 can make the SEQ16 move to any location, then play its note; or it can make the SEQ16 play a rest.  For example, to get the SEQ16 to play the note at location 10, Mode16 will reset SEQ16, then step it 9 times, then step it once to play it.  (Yes coders, this is O(n^2), not cheap).

But during those 9 steps, how does MODE16 prevent SEQ16 from playing?  This is done by filtering out the output gate.  You attach the SEQ16's gate out to Mode16, and then Mode16 provides the actual gate.  

Mode16 comes with 12 repeating or one-shot PATTERNS, which you set with POT 3.  You can change them if you are careful. The patterns at present are (left to right):

- Play 16 steps, looping
- Play 16 steps and then end
- Play 16 steps, play them again, and then end
- Play 16 steps backwards, looping
- Ping-Pong, looping
- Random notes, looping
- Play the first 8 notes, then 8 random notes from back 8, looping
- AABA: Play the first 8 notes twice, then the second 8, then the first 8 again, looping
- Play the first 8 notes with interspersed rests, then the back 8 (without rests) twice, looping
- Play 16 steps with 1/2 probability random dropouts, looping
- Play a random note, or a rest, looping
- Play 16 steps, looping, with swing


## Setting Up

0. Be sure to set Grains to "Grains Mode", not Mozzi.
1. Connect Mode16's Clock Out (Digital Out or "D") to SEQ16's Clock
2. Connect Mode16's Reset Out (Audio Out) to SEQ16's Reset
3. Connect SEQ16's Gate Out to Mode16's Gate In (IN 3)
4. Connect your clock source to Mode16's Clock In (IN 1), set POT 1 to max, and set its switch to IN 1
5. Connect your reset source to Mode16's Reset In (IN 2), set POT 2 to max, and set its switch to IN 2
6. Connect Mode16's Gate Out (Audio In) to your gate output
7. Turn POT 3 to select the pattern.
8. Send Reset to Mode16 (IN 1) to reset SEQ16 rather than directly to the SEQ16.  Similarly, send Clock to Mode16 (IN 2) to clock SEQ16 rather than directly to the SEQ16.
9. Take the CV from SEQ16 but the Gate from Mode16's Gate Out (Audio In).


## Gotchas

Mode16 expects a square-wave-ish clock.  During the LOW PART of the square wave, it does all the clocks to forward to the proper step, then on the RISE of the square wave it plays the note. The problem with this approach is that during the low part of the square wave is when the release of your note plays, so if you have anything other than a very short release, you'll hear the other pitches as Mode16 advances to the proper step.  You can adjust things by tweaking the PWM of the square wave you send to Mode16.

Mode16 only works partially with Accent Repeats, because they conflict with Mode16's drop in gate, and because SEQ16 determines how fast to play Repeats based on previous pulses (and Mode16 pulses very fast to jump to locations).  Accent Repeats will definitely not work properly after a recent jump (such as going backwards or randomly) or after a recent skip.  They may or may not work properly when going foward normally.  Rests may slow them down.  If you find that Repeats are not what you expected, I'd set Repeats to 1.  I'll try to improve the Accent Repeats situation in the future.

Note that if you set the Pattern Knob to in-between two patterns, it won't work right, constantly getting reset.  So make sure the Pattern Knob is definitely on the pattern you want.


## Programming

Mode16 has 12 patterns in an array in the code that you can change.  Each pattern can be at most 64 elements long.  A pattern MUST end with either "END" or "LOOP" -- if it does not, bad things will happen. Likewise, don't have a LOOP without any PLAY or REST elements prior to it at all, else you'll have an infinite loop. Mode16 runs through all the elements of the chosen pattern in order.  The elements are:

Command        | Purpose
---------------|------------
END			     | Stop and do not continue
LOOP		     | Go back to the start of the pattern.
PLAY		     | Play the current step and advance it.
REST		     | Don't play anything, don't advance.  Just wait.
RR			     | If this appears before PLAY, then with 1/2 probability we won't play the note (but still advance).
SKIP		     | If this appears before PLAY, we skip one step first.  Multiple SKIPs or SSs will skip multiple steps.
SS			     | If this appears before PLAY, we skip one step first with a 1/2 probability.   Multiple SKIPs or SSs will skip multiple steps.
J1 ... J16	  | If this appears before PLAY, then we jump to step 1...16 and play *that* note instead of the current note.
JRND		     | If this appears before PLAY, then we jump to a random step and play *that* note instead of the current note.
JR1 		     | If this appears before PLAY, then we jump to a random step between 1...8 and play *that* note instead of the current note.
JR9 		     | If this appears before PLAY, then we jump to a random step between 9...16 and play *that* note instead of the current note.
JLO1... JLO16 | If this appears before JRND, then it changes the low bound for the next JRND
JHI1 ... JHI16 | If this appears before JRND, then it changes the high bound for the next JRND

## Configuration

#### IN 1
Clock In
#### IN 2
Reset in
#### IN 3
Gate In [from the SEQ16 Gate]
#### AUDIO IN (A)
Gate Out
#### AUDIO OUT
Reset Out [to the SEQ16 Reset]
#### DIGITAL OUT (D) 
Clock Out [to the SEQ16 Clock]
#### POT 1
Unused, but you must set to maximum and set switch to IN1
#### POT 2
Unused, but you must set to maximum and set switch to IN2
#### POT 3
Pattern

