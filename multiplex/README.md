# MULTIPLEX

MULTIPLEX is a simple 1 or 2 input, 4 output digital multiplexer.  MULTIPLEX was meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.

SET GRAINS TO GRAINS MODE.  If you'd like to do Mozzi mode, change CV_AUDIO_OUT to 11.

MULTIPLEX takes 1 or 2 input signals and sends out 4 signals based on their values. It can be used, for example, to turn on one of 4 output signals (to change the length of SEQ16 to 4 different values for example), or to choose between four different drum sounds in the DRUMKIT010.

MULTIPLEX has four modes, which are set (left to right) with POT 3.

## Modes

### MODE 1: SIMPLE MODE
In this mode, the value of POT 2 determines which outputs will output the incoming signal.  There are only eight options:

NONE \
OUT 1 \
OUT 2 \
OUT 3 \
OUT 4 \
OUT 1, 2 \
OUT 3, 4 \
OUT 1, 2, 3, 4

IN 1 is the incoming digital signal (set the switch to IN1 and set POT1 to maximum).   Or set the switch to MAN and turn POT1 to change the signal.


### MODE 2: BINARY MODE
In this mode, the value of POT 2 determines which outputs will output the incoming signal.  There are 16 options, in binary order:

NONE \
OUT 1 \
OUT 2 \
OUT 1, 2 \
OUT 3 \
OUT 1, 3 \
OUT 2, 3 \
OUT 1, 2, 3 \
OUT 4 \
OUT 1, 4 \
OUT 2, 4 \
OUT 1, 2, 4 \
OUT 3, 4 \
OUT 1, 3, 4 \
OUT 2, 3, 4 \
OUT 1, 2, 3, 4

Again IN 1 is the incoming digital signal (set the switch to IN1 and set POT1 to maximum).  Or set the switch to MAN and turn POT1 to change the signal.


### MODE 3: MULTIPLEX MODE
In this mode, IN2/POT2 is now a second digital signal.  The combination of IN1/POT1 and IN2/POT2 determines which single output will output high (the others will output low):

| POT1 | POT2       | Output   |
|------|------------|----------|
| LOW  | LOW		| Output 1 |
| HIGH | LOW		| Output 2 |
| LOW  | HIGH		| Output 3 |
| HIGH | HIGH		| Output 4 |


### MODE 4: PASS MODE
In this mode, IN2/POT2 is now a second digital signal.  Whatever is passed into IN1/POT1 gets sent out OUTPUT 1, and whatever gets passed into IN2/POT2 gets sent out OUTPUT 2: and the inverse of IN1/POT1 goes to OUTPUT3, while the inverse of IN2/POT2 goes to OUTPUT 4.  

What's the point of this, you ask?  Well, you can use the pots to shut off or allow IN1 and IN2 to pass through.



## Configuration

#### IN 1
Input Signal 1
#### IN 2
Multiplexer or Input Signal 2
#### IN 3
Output 3
#### AUDIO IN (A)
Output 4
#### AUDIO OUT
Output 1
#### DIGITAL OUT (D) 
Output 2
#### POT 1
Input Signal 1 

If in Modes 1, 2, or 3, set Switch to IN1 and keep at roughly 2 o'clock position

If in Mode 4, set Switch to IN3 and use pot to cut off signal

#### POT 2
Multiplexer or Input Signal 2

If in Mode 1 or 2, set Switch to MAN

If in Mode 3, set Switch to IN2 and keep at roughly 2 o'clock position

If in Mode 4, set Switch to IN2 and use pot to cut off signal

#### POT 3
Method

