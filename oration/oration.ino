// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the GPL 3.0 License

//  Oration is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//  See the GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program. If not, see <http://www.gnu.org/licenses/gpl.html>.


#include "Talkie.h"
#include "Vocab_US_Large.h"
#include "Vocab_Special.h"
#include "Vocab_US_TI99.h"
#include "Vocab_US_Clock.h"
#include "Vocab_US_Acorn.h"
#include "Vocab_AstroBlaster.h"
#include "Vocab_Soundbites.h"
#include "Vocab_Toms_Diner.h"


/// ORATION
///
/// Oration is my attempt at integrating Talkie in a more useful way.  Oration is meant to 
/// run on the AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// SET GRAINS TO GRAINS MODE.   Talkie uses pin 11, so we're doing Grains.
///
/// The classic GRAINS Talkie (SpellAndSpeak) has a very limited vocabulary, but it has
/// a hack to allow you to speak slower or faster.  Oration uses the latest Talkie version,
/// which is much more elaborate but lacks the ability to speak slow or fast.  However you
/// have access to the entire Talkie vocabulary and can make and choose from multiple
/// sentences and switch between them.
///
/// You can find the Talkie vocabulary among the following files:
///
/// Vocab_US_Large.h
/// Vocab_US_TI99.h
/// Vocab_US_Clock.h
/// Vocab_US_Acorn.h
/// Vocab_AstroBlaster.h
/// Vocab_Special.h
/// Vocab_Soundbites.h 
/// Vocab_Toms_Diner.h
///
/// The current Talkie can be found at https://github.com/ArminJo/Talkie/
/// I have modified it slightly to allow pitch changes.
///
///
/// SETUP
///
/// To set up Oration, you need to first state how many sentences you want to define
/// and their maximum length.  I'd make sure this didn't exceed, oh, 800 bytes.
/// The current arrangement is 32 * 3 = 96 bytes [each word is a byte].  Change this
/// as you see fit:

#define MAX_WORDS 32
#define NUM_SENTENCES 3

/// Next you need to state whether Oration will speak through its current sentence
/// or will pick random words in the sentence and say them, in random order forever.
/// To do random order forever, change the following #define to 1.

#define RANDOM_ORDERING 0

/// When picking random words, if we have THREE OR MORE words in a sentence, we can 
/// try to avoid saying the same word twice.  If you want this, set the following to 1.
/// (It only has an effect if we're doing RANDOM_ORDERING of course).

#define AVOID_REPEATS 1

/// If you're NOT doing Random Order, then Oration will speak its current sentence
/// until it is done, then it will emit a trigger on Digital Out (D).  You can feed
/// this into Audio In (A), which will cause Oration to go on to the next sentence.
/// When it has finished the last sentence, Oration will stop, unless you have set
/// the following LOOPING define to 1, which will cause Oration to restart with the
/// first sentence.  Set to 0 to prevent this behavior.

#define LOOPING 1

/// Now we define sentences.

const uint8_t* sentences[NUM_SENTENCES][MAX_WORDS] =    // don't touch this line
  {                    // don't touch this line

    // Each sentence is an array of WORDS that you can select from any of the
    // Vocabulary Files. The sentence MUST end with NULL as shown.  If you forget
    // NULL, then Bad things will happen.  Below, we have defined three sentences.

    // The number of sentences defined here MUST BE EQUAL TO NUM_SENTENCES
    
    { spt_WELL, spt_WHAT, spt_WERE, spt_YOU, spt_DOING, NULL },
    { spt_HELLO, spt_PARTNER, NULL },
    { spONE_SMALL_STEP, NULL },
    
  };    // don't touch this line


///
/// CONFIGURATION
///
/// IN 1            Select Sentence
/// IN 2            Pitch
/// IN 3            Speak Selected Sentence
/// AUDIO IN (A)    Speak Next Sentence
/// AUDIO OUT       Speech Output
/// DIGITAL OUT (D) Sentence Finished Trigger
///
/// POT 1           Select Sentence
/// POT 2           Pitch [changing pitch also changes word speed]
/// POT 3           Speed [spacing between words]






////////// PINOUTS

#define CV_POT_IN1    A2    // Sentence
#define CV_POT_IN2    A1    // Pitch
#define CV_POT3       A0    // Speed
#define CV_IN3        A3    // Next
#define CV_AUDIO_IN   A4    // Reset
#define CV_AUDIO_OUT  11    // Sentence
#define CV_GATE_OUT   8     // Finished

#define RANDOM_PIN              A5

#define MINIMUM_RESET 800
#define LOW 400
#define COUNTDOWN 100       // we need about 100 to trigger Kick drums properly
#define WAITING_SCALE 16
#define WAIT_FOREVER (1000 * 16)

int8_t sentence = -2;
int8_t _word = -1;
Talkie voice(false, true);		// inverted pin (pin 11) only

uint8_t sizes[NUM_SENTENCES];
int16_t waiting = 0;

void selectedSentence()
  {
  sentence = (analogRead(CV_POT_IN1) * (NUM_SENTENCES + 1)) >> 10;
  if (sentence == NUM_SENTENCES)
    sentence = -2;  // stop the loop
  _word = -1;
  waiting = -1;
  }

void nextSentence()
  {
    if (RANDOM_ORDERING)
      {
      waiting = -1;
      return;
      }
      
  sentence++;
  if (sentence >= NUM_SENTENCES)
    {
    if (LOOPING) sentence = 0;
    else sentence = -2;
    }
  _word = -1;
  waiting = -1;
  }

const uint8_t* nextWord()
  {
    if (sentence < 0 || sentence >= NUM_SENTENCES) return NULL;
   if (RANDOM_ORDERING)
    {
      if (sizes[sentence] == 1) return 0;
      else if (AVOID_REPEATS && sizes[sentence] > 2)
        {
          for(uint8_t tries = 0; tries < 255; tries++)    // try up to 256 times
            {
            uint8_t newWord = random(sizes[sentence]);
            if (newWord != _word)
              {
              _word = newWord;
              return sentences[sentence][_word];
              }
            }
         }
      // not avoiding repeats, or giving up
     _word = random(sizes[sentence]);
     return sentences[sentence][_word];
    }
   else
    {
    if (_word > -1 && sentences[sentence][_word] == NULL) return NULL;
    _word++;
    return sentences[sentence][_word];
    }
  }
  

void setup()
  {
    randomSeed(analogRead(RANDOM_PIN));
  pinMode(CV_AUDIO_OUT, OUTPUT);
    pinMode(CV_GATE_OUT, OUTPUT);

   // compute sentence sizes for randomnessn
   uint8_t j = 0;
    for(uint8_t i = 0; i < NUM_SENTENCES; i++)
      {
      for(j = 0; sentences[i][j] != NULL; j++);
      sizes[i] = j;
      } 

  voice.initializeHardware();
//Serial.begin(112500);
  }
  
uint8_t _selectedSentence = 0;
uint8_t _nextSentence = 0;
int8_t countdown = -1;
uint8_t triggered = false; // prevent the first null
void loop()
  {
  // is the digital write countdown finished?
  if (countdown == 0)
    {
    digitalWrite(CV_GATE_OUT, 0);
    countdown--;
    }
   else if (countdown > 0)
    {
    countdown--;
    }
    
  // Determine if we need to go to the next sentence
  
  uint16_t b = analogRead(CV_IN3);
  if (b < LOW)
    {
    _nextSentence = 0;
    }
  else if (b > MINIMUM_RESET && !_nextSentence)
    {
    _nextSentence = true;
    nextSentence();
    }  

  // Determine if we need to go to the selected sentence.  This takes precedence.
  
  analogRead(CV_AUDIO_IN);    // throwaway because it has high impedance :-(
  uint16_t a = analogRead(CV_AUDIO_IN);
  if (a < LOW)
    {
      _selectedSentence = 0;
    }
   else if (a > MINIMUM_RESET && !_selectedSentence)
    {
    _selectedSentence = true;
    selectedSentence();
    }
  
  // Speak!

  // Are we talking?
  if (voice.isTalking())
    {
    // don't do anything
    triggered = true;
    waiting = -1;
    }
  // Okay, we're done talking.  Is there another word and we've not yet started waiting?
  else if (waiting == -1 && nextWord() != NULL)   // this loads the word, so we don't call nextWord() again
  	{
  	waiting = analogRead(CV_POT3) * WAITING_SCALE;
  	}
   // Okay, so we're waiting.  
   else if (waiting > 0 && sentences[sentence][_word] != NULL)
    {
    if (!RANDOM_ORDERING ||  waiting < WAIT_FOREVER) waiting--;
    }
   // Okay, we're done waiting and there's another word.  So say it and trigger a gate out
   else if (waiting == 0 && sentences[sentence][_word] != NULL)
    {
  	voice.sayQ(sentences[sentence][_word]);
    triggered = true;
    }
   // Okay, we're done with the sentence.  Time to indicate it, and set triggered so we don't indicate it a second time.
   else if (triggered)
     {
    digitalWrite(CV_GATE_OUT, 1);
    countdown = COUNTDOWN;
    triggered = false;
     }
   }
