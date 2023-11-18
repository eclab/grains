# Oration 2

Oration 2 is a variation of Oration using older Talkie code, which has advantages and disadvantages.  Oration is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.  Oration was written by Sean Luke [sean@cs.gmu.edu]

Set your Grains to **GRAINS MODE**.  Talkie does not work in Mozzi mode.

### Oration 2 vs. Oration

Both Oration and Oration 2 give you access to the entire Talkie vocabulary and let you make and choose from multiple sentences and switch between them.

The primary advantages of Oration 2 over Oration are:

- Rather Louder
- Able to change both pitch and talking speed independently

The primary disadvantage is:

- Sound quality seems lower: loud elements tend to distort due to some kind of clipping


## About Oration 2

Oration 2 is different in several ways from the [SpellOrSpeak](https://github.com/aemodular/GRAINS/tree/master/GrainsSpellOrSpeak) program that's part of AE Modular's GRAINS distribution:

- It gives you access to a much larger vocabulary: about 820 words in total.
- In Oration 2, you set up multiple full sentences to speak, then choose which sentence you want (or it will speak words at random from a "sentence" -- a collection of words you have designated).  You can realistically speak sentences containing up to about 800 (non-unique) words, but the actual number of unique words will be limited to your available memory.
- It's non-blocking: you can cancel or change sentences after any word.
- You can select pauses in-between words

## Setup

You'll need to specify the maximum number of words per sentence, the number of sentences, whether talkie will speak through the whole sentences or pick random words from them, and whether it will speak all the sentences in order ONCE, or loop through them forever.

You then define the sentences as simple lists of vocabulary words in the code.  It's not hard.

## Vocabulary

Oration 2 has a large vocabulary.  You can build sentences out of any of the Talkie files:

- Vocab_US_Large.h
- Vocab_US_TI99.h
- Vocab_US_Clock.h
- Vocab_US_Acorn.h
- Vocab_AstroBlaster.h
- Vocab_Special.h			(pauses, etc.)
- Vocab_Soundbites.h 		(various soundbites, such as "That's one Small Step for Man...")
- Vocab_Toms_Diner.h		(yes, this really is the entire Suzanne Vega song)

You can make your own words and sounds too!  See [the Talkie page](https://github.com/ArminJo/Talkie#own-vocabulary) for more information.

## Non-Random Configuration

If you would like Oration 2 to speak all the sentences one at a time, just take the Speech Finished Trigger (Digital Out (D)) and feed it into the Trigger for Speaking the Next Sentence (Audio In (A)).

#### IN 1
Current Sentence to Speak (CV)
#### IN 2
Pitch CV [Changing pitch also changes speaking speed]
#### IN 3
Trigger Speaking the Next Sentence
#### AUDIO IN (A)
Trigger Speaking the Current Sentence
#### AUDIO OUT
Speech Output
#### DIGITAL OUT (D) 
Sends Trigger when a Sentence is finished
#### POT 1
Current Sentence to Speak
#### POT 2
Pitch
#### POT 3
Speech Rate

## Random Configuration

This configuration is used when RANDOM_ORDERING has been turned on in the code.

#### IN 1
Current Sentence to Speak (CV)
#### IN 2
Pitch CV [Changing pitch also changes speaking speed]
#### IN 3
Trigger Speaking Random Word in Current Sentence
#### AUDIO IN (A)
Trigger Selecting a Random Sentence, then Speaking a Random Word in that Sentence
#### AUDIO OUT
Speech Output
#### DIGITAL OUT (D) 
Sends Trigger when a Word is finished
#### POT 1
Current Sentence to Speak
#### POT 2
Pitch
#### POT 3
Speech Rate

### GRAINS BUG

There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the switch to "Man", then the range of the Pot is correct.  But if you set the switch to "In 1" (or "In 2"), then the range of the Dial is maximum at about the 2 o'clock position on the Dial.  Beyond that it stays at maximum.
