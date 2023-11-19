// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the GPL 3 License


/// ELEMENTS
///
/// Elements is a one-track Euclidian drum trigger sequencer.  Elements is meant  
/// to run on the AE Modular GRAINS, but it could be adapted to any Arduino.
/// Elements was written by Sean Luke [sean@cs.gmu.edu]
///
/// SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)
///
/// The Euclidian sequence fad started with the paper "The Euclidean 
/// Algorithm Generates Traditional Musical Rhythms" by Godfried Toussaint.
/// It is my opinion that the claims of presence of Euclidian sequences
/// in worldwide drum sequences is dubious at best, but whatever: musicians are
/// enamored with it!
///
/// See https://en.wikipedia.org/wiki/Euclidean_rhythm
///
/// All a Euclidian Sequencer attempts to do is evenly distribute K beats
/// within N >= K steps, using a certain uniform statistical pattern.  This
/// pattern was originally derived from Bjorklund's Algorithm (which is 
/// related to Euclid's Algorithm), but in fact it is also a trivial 
/// result of Bressenham's Line Algorithm, which is simpler to compute 
/// (and we're using it below).
///
/// We have three major parameters:
///
/// - The SIZE of the sequence (N)
/// - The NUMBER OF BEATS in the sequence (K)
/// - The ROTATION of the sequence -- which step the sequence starts on (from 0..N-1)
///
/// If you change these parameters, Elements will have to recompute and restart
/// the sequence.  
///
/// Elements pumps out the sequence via AUDIO OUT, and the reversed sequence via DIGITAL_OUT.
///
/// OPTION
///
/// Having a hard time determining the length of your sequence?  Instead of pumping out
/// the reversed sequence, DIGITAL_OUT can be set to trigger every time the sequence starts,
/// which makes it much easier to count the beats.  You do this by changing the following to 1:

#define TRIGGER_ON_SEQUENCE_START 1

///
/// CONFIGURATION
///
/// IN 1            Size CV, up to 32 steps
/// IN 2            Number of Beats (as a percentage of Size) CV
/// IN 3            Clock
/// AUDIO IN (A)    Reset
/// AUDIO OUT       Euclidian Rhythm Out
/// DIGITAL OUT (D) Reverse Euclidian Rhythm Out, or first step of sequence if TRIGGER_ON_SEQUENCE_START is 1
///
/// POT 1           Size, up to 32 steps
/// POT 2           Number of Beats (as a percentage of Size)
/// POT 3           Rotation (as a percentage of Size)
///
///
/// GRAINS BUG
/// There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the switch to "Man", 
/// then the range of the Pot is correct.  But if you set the switch to "In 1" (or "In 2"), 
/// then the range of the Dial is maximum (1.0 probability for our purposes) at about the 
/// 2 o'clock position on the Dial.  Beyond that it stays at 1.0.

////////// PINOUTS

#define CV_POT_IN1    A2    // Size
#define CV_POT_IN2    A1    // Number of Beats
#define CV_POT3       A0    // Rotation
#define CV_IN3        A3    // Clock
#define CV_AUDIO_IN   A4    // Reset
#define CV_AUDIO_OUT  11    // Euclidian Rhythm Out
#define CV_GATE_OUT   8     // Reverse Euclidian Rhythm Out

#define LOW_CLOCK 200
#define HIGH_CLOCK 800
#define TRIGGER_WIDTH 100
#define MAX_LENGTH 32
uint8_t rhythm[MAX_LENGTH];
int8_t size = -1;
int8_t beats = -1;
int8_t rotation = -1;
uint8_t position = 0;
int8_t counter1 = -1;
int8_t counter2 = -1;

void setup()
    {
    pinMode(CV_AUDIO_OUT, OUTPUT);
    pinMode(CV_GATE_OUT, OUTPUT);
  Serial.begin(115200);
    }

// From https://github.com/ducroq/EuclidSeqNano/blob/master/src/EuclidRhythm.cpp
// Which is GPL 3

bool bresenhamEuclidean(int n, int k, int o, uint8_t *s)
/*
	Constructs a cyclic n-bit binary sequence with k 1s,
	such that the 1s are distributed as evenly as possible.
	@param n is the length of the sequence (beats or pulses)
	@param k is the number of 1s (onsets)
	@param o is the offset (shift)
	@param s is a pointer to store the resulting sequence
	returns true on success and false on failure
*/
{
	if (k > n)
		return false;

   if (k == 0)
    {
    for(uint8_t i = 0; i < n; i++)
      s[i] = 0;
    return true;
    }
    
	uint8_t c[n];
	float slope = float(k) / float(n);
	uint8_t prev_y = -1;

	for (uint8_t i = 0; i < n; i++)
	// approximate a pixelated line and mark vertical changes
	{
		uint8_t y = (uint8_t)floor((float(i) * slope));
		c[i] = (y != prev_y) ? 1 : 0;
		prev_y = y;
	}
	for (uint8_t i = 0; i < n; i++)
		// shift the pattern to produce sequence
		s[i] = (i - (int8_t)o >=  0) ? c[i - (int8_t)o] : c[i - (int8_t)o + n];
	//	(i + o < n) ? c[i + o] : c[i + o - n];

	return true;
}

void printIt()
  {
  Serial.print("Size ");
  Serial.println(size);
  Serial.print("Beats ");
  Serial.println(beats);
  Serial.print("Rotation ");
  Serial.println(rotation);
  for(int i = 0; i < size; i++)
    {
    Serial.print(rhythm[i]);
    Serial.print(" ");
    }
  Serial.println();
  }

#define COUNT_TO 10
uint8_t sizeCount = 0;
uint8_t beatsCount = 0;
uint8_t rotationCount = 0;
uint8_t clock = 0;
void loop()
    {
	// reset triggers
	if (counter1 == 0)
		{
		digitalWrite(CV_AUDIO_OUT, 0);
		}
		
	if (counter1 >= 0)
		{
		counter1--;
		} 
		
	if (counter2 == 0)
		{
		digitalWrite(CV_GATE_OUT, 0);
		}
		
	if (counter2 >= 0)
		{
		counter2--;
		} 
    
    // Determine if we need to recompute the rhythm

    boolean change = false;
    uint8_t newSize = (uint8_t)((analogRead(CV_POT_IN1) * (uint32_t)(MAX_LENGTH - 1)) >> 10) + 1;
    if (newSize != size)
      {
        if (sizeCount++ > COUNT_TO)
          {
            Serial.println("Changed");
            size = newSize;
            sizeCount = 0;
            change = true;
          }  
      }
        else sizeCount = 0;      
    uint8_t newBeats = (uint8_t)((analogRead(CV_POT_IN2) * (uint32_t)(newSize + 1)) >> 10);
    if (newBeats != beats)
      {
        if (beatsCount++ > COUNT_TO)
          {
            beats = newBeats;
            beatsCount = 0;
            change = true;
          }        
      }
        else beatsCount = 0;      
    uint8_t newRotation = (uint8_t)((analogRead(CV_POT3) * (uint32_t)(newSize)) >> 10);
   if (newRotation != rotation)
      {
        if (rotationCount++ > COUNT_TO)
          {
            rotation = newRotation;
            rotationCount = 0;
            change = true;
          }        
     }
         else rotationCount = 0;      
    
    if (change)
    	{
    	// rebuild
    	position = 0;
    	bresenhamEuclidean(newSize, newBeats, newRotation, rhythm);
      printIt();
    	}
  
    // Reset and Clock

	if (size == 0) return;

	analogRead(CV_AUDIO_IN);		// throwaway
	if (analogRead(CV_AUDIO_IN) > HIGH_CLOCK)
		{
		position = 0;
		return;
		}
	
	uint16_t clockVal = analogRead(CV_IN3);
	if (clockVal < LOW_CLOCK)
		{
		clock = false;
		}
	else if (clockVal > HIGH_CLOCK && !clock)
		{
		clock = true;
		if (rhythm[position] == 1) 
			{
			digitalWrite(CV_AUDIO_OUT, 1);
			counter1 = TRIGGER_WIDTH;
			}
		if (TRIGGER_ON_SEQUENCE_START)
			{
			if (position == 0) 
				{
				digitalWrite(CV_GATE_OUT, 1);
				counter2 = TRIGGER_WIDTH;
				}
			}
		else
			{
			if (rhythm[size - position] == 1) 
				{
				digitalWrite(CV_GATE_OUT, 1);
				counter2 = TRIGGER_WIDTH;
				}
			}
    position++;
    if (position >= size) position = 0;
		}
    }
