# Theory

Theory is allows you to output digital outputs based on the mathematical expressions
of various real-valued inputs.  Theory is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.  Theory was written by Sean Luke [sean@cs.gmu.edu]

Set your Grains to **GRAINS MODE**.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)

The basic configuration has three binary outputs called **A, B, and C,** which are computed based on real-valued inputs called **W, X, Y, and Z.**  The four inputs range from 0.0 at 0V to 1.0 at 5V.  You code
the expressions where indicated in the theory.ino file.  You can make any expression you like, and use any
floating-point operation avaiable to the Arduino.  Note that some operations, like sin and pow, are SLOW.  We might speed it up a bit with a provided function [like here.](https://forum.arduino.cc/t/a-faster-sin-lookup-function/69173/6)

### FIVE-INPUT OPTION

Instead of three expressions of four inputs, you can also set up Theory to have two expressions (A and B) of five inputs (W, X, Y, Z, and C, now repurposed as real-valued input).

### SIX-INPUT OPTION

You can also do a single expression (A) of six real-valued inputs (W, X, Y, Z, C) plus one input (B) which is digital (0 or 1).  Note: if nothing is plugged into B, then it will register a 1.


## Configuration
This is the default configuration

#### IN 1
CV for X
#### IN 2
CV for Y
#### IN 3
C		
#### AUDIO IN (A)
W
#### AUDIO OUT
A
#### DIGITAL OUT (D) 
B
#### POT 1
X
#### POT 2
Y
#### POT 3
Z

### GRAINS BUG

There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the switch to "Man", then the range of the Pot is correct.  But if you set the switch to "In 1" (or "In 2"), then the range of the Dial is maximum (1.0 or 5V for our purposes) at about the 2 o'clock position on the Dial.  Beyond that it stays at 1.0
