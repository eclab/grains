// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


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
/// or will pick random words in the sentence and say them in random order.  When
/// in random order, Oration will say one random word at a time.  If POT 3 is set to
/// far right, then it will wait after each word until
/// you trigger it via IN3 or AUDIO_IN.  You might use DIGITAL OUT to do this trigger. 
/// To do random order, change the following #define to 1.

#define RANDOM_ORDERING 1

/// If you're NOT doing Random Order, then Oration 2 will speak its current sentence
/// until it is done, then it will emit a trigger on Digital Out (D).  You can feed
/// this into Audio In (A), which will cause Oration 2 to go on to the next sentence.
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
    // { sp3_ONE, sp3_FIFTEEN, sp3_SEVEN, sp5_CLEARANCE_DELIVERY, NULL },
    { spONE_SMALL_STEP, NULL },
    
  };    // don't touch this line



/// CONFIGURATION (NON-RANDOM)
///
/// IN 1            Current Sentence CV
/// IN 2            Pitch CV
/// IN 3            (Trigger) Speak Next Sentence
/// AUDIO IN (A)    (Trigger) Speak Selected Sentence
/// AUDIO OUT       Speech Output
/// DIGITAL OUT (D) Sentence Finished Outgoing Trigger
///
/// POT 1           Current Sentence
/// POT 2           Pitch
/// POT 3           Pause Between Words
///
///
///
/// CONFIGURATION (RANDOM)
///
/// IN 1            Current Sentence CV
/// IN 2            Pitch CV
/// IN 3            (Trigger) Speak Random Word in Current Sentence
/// AUDIO IN (A)    (Trigger) Pick a random sentence, then speak a Random Word in that sentence
/// AUDIO OUT       Speech Output
/// DIGITAL OUT (D) Word Finished Outgoing Trigger
///
/// POT 1           Current Sentence
/// POT 2           Pitch
/// POT 3           Pause Between Words.  Far Right is Infinite Pause (you must trigger words with In3 or Audio In Triggers)






////////// PINOUTS

#define CV_POT_IN1    A2    // Sentence
#define CV_POT_IN2    A1    // Pitch
#define CV_POT3       A0    // Speed
#define CV_IN3        A3    // Next
#define CV_AUDIO_IN   A4    // Reset
#define CV_AUDIO_OUT  11    // Sentence
#define CV_GATE_OUT   8     // Finished

#define RANDOM_PIN              A5
#define WAITING_SCALE 16
#define WAIT_FOREVER (1000 * 16)		// Note in Oration 2 this is 32767

#define MINIMUM_RESET 800
#define LOW 400
#define COUNTDOWN 100       // we need about 100 to trigger Kick drums properly

#define STOP_SPEAKING (-2)			// for sentence
#define RESET_SPEAKING (-1)			// for sentence or word

int8_t sentence = RESET_SPEAKING;       // STOP_SPEAKING means "no sentence", i.e., stop looping. RESET_SPEAKING  means "no sentence now, but if next sentence is chosen, we'll start with 0"
int8_t _word = RESET_SPEAKING;          // -1 means 
Talkie voice(false, true);				// inverted pin (pin 11) only

uint8_t sizes[NUM_SENTENCES];
int16_t waiting = 0;					// no wait initially

uint8_t _nextSentenceSelected = false;
uint8_t _sentenceSelected = false;
uint8_t _triggerSent = false;	
int8_t countdown = -1;					// countdown from after we set the trigger to when we release it.  -1 is COUNTDOWN FINISHED


// Selects a sentence.  If the user selects the "top" value, we select STOP.
void selectSentence()
  {
  if (RANDOM_ORDERING)
  	{
  	uint8_t newSentence = random(NUM_SENTENCES);
  	if (newSentence != sentence) _word = -1; 	// don't try to avoid duplicates
  	sentence = newSentence;
	nextWord();
  	}
  else
  	{
	  sentence = (analogRead(CV_POT_IN1) * (NUM_SENTENCES + 1)) >> 10;
	  _word = RESET_SPEAKING;
	  if (sentence == NUM_SENTENCES)
	    sentence = STOP_SPEAKING;  // stop the loop
	  _triggerSent = false;
    }
  }
  

// Selects the next sentence
void selectNextSentence()
  {
  if (RANDOM_ORDERING)
  	{
    sentence = (analogRead(CV_POT_IN1) * NUM_SENTENCES) >> 10;
  	nextWord();
  	}
  else
  	{
	  sentence++;							// RESET_SPEAKING will go to 0
	  _word = RESET_SPEAKING;
	  if (sentence >= NUM_SENTENCES)
		{
		if (LOOPING) sentence = 0;
		else sentence = STOP_SPEAKING;
		}
	  _triggerSent = false;
	}
  }

// Extracts and returns the next word.  _word is set to the index of the word
// in the sentences array.  If there is no word, NULL is returned
const uint8_t* nextWord()
  {
   if (sentence < 0 || sentence >= NUM_SENTENCES) return NULL;
    waiting = 0;			// let's speak!  Undo inifinite pause
   if (RANDOM_ORDERING)
    {
      if (sizes[sentence] == 1)    // only one word!
      	{
      	_word = 0;
    	return sentences[sentence][_word];
      	}
      else if (sizes[sentence] > 2)
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
    if (_word >= sizes[sentence]) return NULL;
    if (_word >= 0 && sentences[sentence][_word] == NULL) return NULL;
    _word++;								// RESET_SPEAKING will go to 0
    return sentences[sentence][_word];
    }
  }


uint16_t buildSampleRate()
	{
	uint16_t a = analogRead(CV_POT_IN2);
	if (a < 512)
		{
		a = (uint16_t)((a * (uint32_t)4000) >> 9) + 4000;		// 4000 to 8000
		}
	else
		{
		a = (uint16_t)((a * (uint32_t)16000) >> 10);			// 8000 to 16000
		}
	return a;
	}


void setup()
  {
    pinMode(CV_GATE_OUT, OUTPUT);
    randomSeed(analogRead(RANDOM_PIN));

   // compute sentence sizes
   uint8_t j = 0;
    for(uint8_t i = 0; i < NUM_SENTENCES; i++)
      {
      for(j = 0; sentences[i][j] != NULL; j++);
      sizes[i] = j;
      } 

  //Serial.begin(112500);
  }
  
void loop()
  {
  // Determine if we need to go to the next sentence
  uint16_t b = analogRead(CV_IN3);
  if (b < LOW)
    {
    _nextSentenceSelected = 0;
    }
  else if (b > MINIMUM_RESET && !_nextSentenceSelected)
    {
    _nextSentenceSelected = true;
    selectNextSentence();
    }  

  // Determine if we need to go to the selected sentence.  This takes precedence.
  analogRead(CV_AUDIO_IN);    // throwaway because it has high impedance :-(
  uint16_t a = analogRead(CV_AUDIO_IN);
  if (a < LOW)
    {
      _sentenceSelected = 0;
    }
   else if (a > MINIMUM_RESET && !_sentenceSelected)
    {
    _sentenceSelected = true;
    selectSentence();
    }
  
  // is the digital write countdown finished?
  if (countdown == 0)
    {
    digitalWrite(CV_GATE_OUT, 0);
    countdown--;						// go to -1
    }
   else if (countdown > 0)
    {
    countdown--;
    }
    


  // Speak!

  // Are we talking?
  if (voice.isTalking())
    {
      // do nothing
    }
   // We're not talking.  So are we ready to say the next word?
  else if (waiting <= 0)
  	{
  	if (sentence >= 0)				// do we have a sentence?
  	 	{
  	 	if (nextWord() != NULL)		// do we have a word to say?
			{
		     buildSampleRate();
    		 //buildSpeechRate();
  			 voice.sayQ(sentences[sentence][_word]);
  			 _triggerSent = false;
    		waiting = analogRead(CV_POT3) * WAITING_SCALE + 1;		// should not be zero, so we can wait forever
    		}
    	else						// at end of sentence.  Trigger if we haven't already.
  	 		{
    		if (!_triggerSent)
    			{
    			digitalWrite(CV_GATE_OUT, 1);
    			countdown = COUNTDOWN;
    			_triggerSent = true;
    			}
  	 		}
    	}
    else				// No sentence.  We wait.
    	{
    	// do nothing
    	}
  	}
   else // We are waiting...
		{
		if (RANDOM_ORDERING)
			{
    		if (!_triggerSent)
    			{
    			digitalWrite(CV_GATE_OUT, 1);		// send word trigger
    			countdown = COUNTDOWN;
    			_triggerSent = true;
    			}
    		// now wait forever
			if (waiting < WAIT_FOREVER) waiting--;
			}
		else waiting--;
		}
}
