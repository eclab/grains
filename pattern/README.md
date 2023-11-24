# Pattern

PATTERN produces various 4-output digital patterns based on an analog value.  PATTERN is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)

PATTERN takes an analog input and outputs four digital outputs which are high or low depending on the value of the input.  You can output gates or triggers.

## NOTE

Pattern is kind of useless because the Arduino's analog inputs are noisy.  I was hoping to cleanly generate a consistent discretized or beat pattern based on a sawtooth LFO or a triangle LFO but the amount of noise smoothing I am forced to add results in a kind of janky movement.  It works well with SEQ16 though.

So I'm not sure where Pattern is going -- I may make big changes in it towards a divider like MM-DIV...

## What is a Pattern?

PATTERN outputs, well, patterns.  A pattern is a sequence of 4 ones or zeros, such as 0111, which here I will call a 4-tuple.  For example, 0100, 1100, 1111, 0010, 0101 is a pattern. The four ones or zeros in a 4-tuple correspond to each of the four outputs, in the order 4321.  For example, 0101 means that outputs 1 and 3 are ON and outputs 2 and 4 are OFF.

At any particular time, PATTERN is outputting ONE 4-tuple in the current pattern.  As you change the input value, PATTERN outputs the 4-tuple in the pattern corresponding to the position of the input.  So if, for example, you turned the knob fully from one end to the other, PATTERN would sweep through all the 4-tuples in the pattern sequence in order.


## Outputs, Gates, and Triggers 

PATTERN can output its 4-tuples as GATES or as TRIGGERS.  As GATES, it's simply turning on the outputs which have 1s in the 4-tuple, and turning off the outputs that have 0s.  As TRIGGERS, it's pulsing a trigger for the outputs which have 1s in the 4-tuple.  Triggers are pulsed as soon as the 4-tuple is selected by your input.

## Styles and Complexity

PATTERN has 21 patterns to choose from.  Patterns come in 7 STYLES, and within each style, there are three patterns of varying COMPLEXITY.

There are SEVEN styles:

- MOVEMENT: a Single 1 passes from start to finish
- MOVEMENT WITH ZERO: Same as MOVEMENT, but at the beginning, nothing is on at all
- MOVEMENT WITH TWO ZEROS: Same as MOVEMENT, but at the beginning AND the end, nothing is on at all
- WIDE MOVEMENT: Same as MOVEMENT WITH TWO ZEROS, but TWO, THREE, or FOUR ones pass from start to finish
- ALTERNATIVE MOVEMENT: Various alternative styles
- BAR GRAPH: Gradually more and more outputs output a 1
- BINARY: The outputs follow the pattern of binary numerals

Each style has THREE levels of complexity.  In many cases the complexity indicates how many
outputs are involved in the pattern (2, 3, or 4).  For WIDE MOVEMENT WITH TWO ZEROS and for
ALTERNATIVE MOVEMENT the complexity indicates other things.

Here are the seven styles and three complexity levels, and the resulting patterns.

| Style                   | Complexity | Pattern                                                                     |
| ----------------------- | ---------- | --------------------------------------------------------------------------- |
| MOVEMENT                | 2          | ``0001 0010`` |
|                         | 3          | ``0001 0010 0100`` |
|                         | 4          | ``0001 0010 0100 1000`` |
| MOVEMENT WITH ZERO      | 2          | ``0000 0001 0010`` |
|                         | 3          | ``0000 0001 0010 0100`` |
|                         | 4          | ``0000 0001 0010 0100 1000`` |
| MOVEMENT WITH TWO ZEROS | 2          | ``0000 0001 0010 0000`` |
|                         | 3          | ``0000 0001 0010 0100 0000`` |
|                         | 4          | ``0000 0001 0010 0100 1000 0000`` |
| WIDE MOVEMENT           | 2          | ``0000 0001 0011 0110 1100 1000 0000`` |
|                         | 3          | ``0000 0001 0011 0111 1110 1100 1000 0000`` |
|                         | 4          | ``0000 0001 0011 0111 1111 1110 1100 1000 0000`` |
| ALTERNATIVE MOVEMENT    | Triple     | ``0011 0110 1100`` |
|                         | Jump       | ``0011 1111 1100`` |
|                         | Aliased    | ``0001 0011 0010 0110 0100 1100 1000`` |
| BAR GRAPH               | 2          | ``0000 0001 0011`` |
|                         | 3          | ``0000 0001 0011 0111`` |
|                         | 4          | ``0000 0001 0011 0111 1111`` |
| BINARY                  | 2          | ``0000 0001 0010 0011`` |
|                         | 3          | ``0000 0001 0010 0011 0100 0101 0110 0111`` |
|                         | 4          | ``0000 0001 0010 0011 0100 0101 0110 0111`` <br> ``1000 1001 1010 1011 1100 1101 1110 1111`` |


## Configuration

#### IN 1
Input CV
#### IN 2
[UNUSED]
#### IN 3
Output 3
#### AUDIO IN (A)
Output 4
#### AUDIO OUT
Output 1
#### DIGITAL OUT (D) 
Output 2
#### POT 1
Input
#### POT 2
Style
#### POT 3
Complexity and Gate/Trigger. There are six values, left-to-right: Complexity 1, 2, 3 as gates, then Complexity 1, 2, 3 as triggers

### Using DRUMKIT010

Be sure to set DRUMKIT010's TRIG MODE switch to "D" (Digital), so it properly responds to triggers.

### GRAINS Bug

There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the switch to "Man", then the range of the Pot is correct.  But if you set the switch to "In 1" (or "In 2"), then the range of the Dial is maximum at about the 2 o'clock position on the Dial.  Beyond that it stays at maximum.


