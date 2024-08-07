# Memoir


Memoir is a Gate and CV recorder.  It can record one track of digital (gate/trigger) signals and one track of CV signals.  The recorded tracks are stored in EEPROM and survive power cycling.

SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.

You will need to install the Mozzi Library.  You can do this from the Library Manager in your Arduino IDE.

## Recording 

Recording is initiated by sending a trigger to the RECORD TRIGGER, which is AUDIO IN.  Thereafter recording will go on for some number of seconds, or until a PLAY TRIGGER (IN 3) is sent.  Then the recording is saved.

If we send another RECORD TRIGGER during recording, recording is erased and restarted.

## Playing 

Playing is initiated by sending a trigger to the PLAY TRIGGER, which is IN 3.   If we're still recording, it is saved initially first (which takes a few seconds).  Then replay starts.

If we send another PLAY TRIGGER during playback, playback is restarted.

## Length of Time 

The length of the recording varies depending on the setting of POT 3 when recording is initiated. POT 3 can be 4, 8, 12, 16, 20, 24, 28, or 32 seconds depending on setting (left to right).  GRAINS has no way of indicating that recording has finished; you either wait it out or send a PLAY TRIGGER.

Playback is similarly 4 ... 32 seconds depending on the setting of POT 3.  This means that you can record 4 seconds of data and then stretch it over 32 seconds (or record 32 seconds of data and compress it to 4!)


## Configuration

#### IN 1
CV In
#### IN 2
Gate In
#### IN 3
Play Trigger
#### AUDIO IN (A)
Record Trigger
#### AUDIO OUT
CV Out
#### DIGITAL OUT (D) 
Gate Out
#### POT 1
CV In

[If using CV, set switch to IN1 and POT to about 2'oclock]
#### POT 2
Gate In

[If using Y CV, set switch to IN2 and POT to about 2'oclock]
#### POT 3
Rate