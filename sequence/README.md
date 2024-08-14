# SEQUENCE

SEQUENCE is 4, 8, or 16-step, 8-pattern step sequencer for values and rests. SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode, though it could probably be adapted to it.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.


__NOTE:__ Normally a step sequencer has a lot of knobs.  But GRAINS has only 3.  It can still do all the functions of a step sequencer, but things are gonna be pretty fiddly.  That's life with three knobs.

The sequencer is CLOCKED and can be RESET.  It contains 8 patterns, each of up to 16 steps (8 by default). You can shorten the number of steps of any of the patterns as you like.  Each clock, the sequencer will output the next CV value, and it will also output a GATE which will be high for a period; or if the step is a REST, it the GATE will be low for the entire step.  

__SAVING:__ If you change to LOCK MODE, your current settings are automatically saved and made available next time you power up SEQUENCE.

### Grains Output Warning
This program outputs note CV information from GRAINS.  If you feed this into (for example) a VCO, 2OSC/d, or 2OSC, it will not be proper 1V/oct because they pull too much amperage and this causes the GRAINS to output incorrect voltages.  Even feeding into another GRAINS will cause the pitch to drop a bit.  You can fix this by feeding into a buffered mult first.  The 555 does not have this issue (its inputs are buffered).

## Max Number of Steps

By default Sequence has a maximum of 8 steps per pattern.  You can change this to 16 or to 4 but nothing else.  Note that 16 is extremely fiddly.  To do so, just change a #define in the code as shown.  You can always make a sequence shorter than the maximum by adding an END, see below.

## Initialization

Initially Sequence will have garbage in its patterns because it has loaded from your EEPROM, and who knows what's in there the first time.  You can change these values manually, but it might be easier to just set everything to medium (about 2V), set the current pattern to pattern 1, and set the gate length to maximum.   To do this, just uncomment the the #define in the code as shown.  Now upload Sequence into your Grains and turn it on.  This will clear the EEPROM. Thereafter re-comment that #define and reupload Sequence into your Grains.

You don't have to do this -- you can just initialize things manually yourself (see "HOW TO CLEAR OUT A PATTERN" below).

## The Four Modes

Programming and using Sequence is fiddly because Grains only has 3 knobs.  But you'll get the hang of it!  Sequence has four MODES, set by POT 3:

### Lock Mode
In this mode, turning the knobs does nothing.  It's used to turn the other knobs in preparation for a different mode.  Sequence will also automatically save its settings when you ENTER LOCK MODE.

### Entry Mode
In this mode, Pot 1 specifies the STEP (1...16) and Pot 2 specifies its VALUE.  The minimum value is designated to be a REST (the gate is dropped) and the maximum value is designated to indicate the END of the SEQUENCE -- that way you can stipulate how long the sequence is. Between these two extremes lie the remaining values, namely 0V ... about 4V.

### Pattern Mode
In this mode, Pot 1 specifies the PATTERN (1...8).  Pot 2 sets the GATE LENGTH.  This mode is meant for real-time manual performance.

### Pattern Step Mode
In this mode, a trigger sent to IN 1 increments the PATTERN, and Reset also resets the pattern to 0. Pot 2 sets the GATE LENGTH.  This mode is meant for automatic or triggered incrementing of the pattern.


## Usage Suggestions

### HOW TO UNLOCK POT 1 OR POT 2

In ENTRY MODE, POT 2 is locked.  In PATTERN MODE, Pots 1 and 2 are locked.  In PATTERN STEP MODE, Pot 2 is locked.  What does this mean?  It means that when first go into one of these modes, the values of the pot are ignored until you turn the pot a little bit (about 1/32 its total distance). You'll need to do that to get Sequence to start listening to the pot.

### HOW TO START PROGRAMMING A PATTERN

Set to PATTERN MODE.  Unlock POT 1.  Set POT 1 to the desired pattern (pattern 1, for example, is far left). For good measure, unlock and turn POT 2 to far right (maximum gate).   Then turn back to LOCK MODE.

### HOW TO CLEAR OUT A PATTERN (SETTING THEM ALL TO THE SAME VALUE)

Once you have set your pattern, go to LOCK MODE.  Set POT 1 to the far left position (which will be step 1). Only after you have done this, switch to ENTRY MODE.  Unlock Pot 2 and turn it to, let's say, the dead center position, which is the medium CV value, around 2V.  (Remember that far left is NOT 0 V, but it a REST, and far right is NOT maximum volts, but is END).  Now let's sweep the steps: turn Pot 1 moderately slowly from far left clear to far right.   Now they're all set to 2V.  Go back to LOCK MODE.

### HOW TO PROGRAM A SINGLE STEP

Set to LOCK MODE.  Turn POT1 to the STEP you wish to set.  Then set to ENTRY MODE. Unlock and turn POT2 to the VALUE you wish to set the step to.  Then turn back to LOCK MODE.

### HOW TO PROGRAM A SERIES OF STEPS

Set to LOCK MODE.  Turn POT1 to the the first STEP to set.  Then set to ENTRY MODE. Unlock and turn POT2 to the VALUE you wish to set the step to.  Then change POT1 to the next step. Turn POT2 to the VALUE you wish to set the next step to.  And so on.  When you are done, Set to LOCK MODE again.

### REAL-TIME PERFORMANCE USAGE

Usually the two things you want to do in real-time is [a] change the pattern and [b] change the gate.  For this reason, I suggest setting to PATTERN MODE and keeping it there. While the sequencer is running you can unlock and use POT 2 to change the gate, and unlock and use POT 1 to change the pattern.  Both of these can be changed via CV using IN 1 and IN 2 respectively as well (once unlocked).

### AUTOMATICALLY TRIGGERING PATTERNS

You might want to increment patterns automatically.  You can do this by switching to PATTERN STEP MODE, and set POT1 to maximum (and set the switch to IN1).  This allows you to send a trigger to IN1, and when you do so, the pattern will be incremented.  Additionally, when you trigger a RESET, it will not only reset the steps but also set the pattern to 1.



## Configuration

#### IN 1
Step / Pattern CV

In ENTRY MODE, this specifies which step.  
In PATTERN MODE, this specifies which pattern.  
In PATTERN STEP MODE, this increments the pattern.

#### IN 2
Value / Gate CV

In ENTRY MODE, this specifies step value.  
In PATTERN MODE, this specifies gate length.
In PATTERN STEP MODE, this specifies gate length.

#### IN 3
Clock 
#### AUDIO IN (A)
Reset
#### AUDIO OUT
Output
#### DIGITAL OUT (D) 
Gate Output
#### POT 1
Step / Pattern

In ENTRY MODE, this specifies which step.
In PATTERN MODE, this specifies which pattern.  
In PATTERN STEP MODE, this increments the pattern.

[Set the switch to MAN if not using the CV, else set to IN 1 and set POT 1 to about 2 o'clock]
#### POT 2
Release

[Set the switch to MAN if not using the CV, else set to IN 2 and set POT 2 to about 2 o'clock]
#### POT 3
Mode

[Left: Entry, Middle Left: Lock, Middle Right: Pattern, Right: Pattern Step]


