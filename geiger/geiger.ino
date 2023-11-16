// Copyright 2023 Sean Luke 
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License



/// GEIGER
///
/// Geiger is meant to run on the AE Modular GRAINS, but it could be adapted
/// to any Arduino.  Geiger was written by Sean Luke [sean@cs.gmu.edu]
///
/// SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)
///
/// Geiger is a random trigger generator for three tracks.  The randomness of
/// the triggers for each track is controlled by a different knob.  You can also
/// control the randomness of the first two tracks with a CV, but note that when
/// you do so, the potentiometer's maximum value peaks at about the 2 o'clock
/// position on the dial due to a GRAINS bug.
///
/// If you turn POT 3 to the far RIGHT, GEIGER can instead be used as a BERNOULLI GATE.
/// That is, when a trigger comes in, ONLY ONE of the outputs will be triggered exclusively.
/// The probability that the output will be OUTPUT 1 is determined by POT 1 (and IN 1).
/// Of the remainder, the probability that the output will be OUTPUT 2 is determined by
/// POT 2 (and IN 2).  Otherwise, it's OUTPUT 3.  So for example, to make each output
/// trigger with equal probability, set POT 1 to 1/3 and set POT 2 to 1/2.

/// CONFIGURATION
///
/// IN 1            CV for probability of Output 1
/// IN 2            CV for probability of Output 2
/// IN 3            Output 3
/// AUDIO IN (A)    Clock or Bernoulli Trigger
/// AUDIO OUT       Output 1
/// DIGITAL OUT (D) Output 2
///
/// POT 1           Probability for Output 1 [Set to "In 1" only if using In 1 CV]
/// POT 2           Probability for Output 2 [Set to "In 2" only if using In 2 CV]
/// POT 3           Probability for Output 3 [Turn to far right to change to BERNOULLI]


/// GRAINS BUG
/// 
/// There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the 
/// switch to "Man", then the range of the Pot is correct.  But if you set the switch 
/// to "In 1" (or "In 2"), then the range of the Dial is maximum (1.0 probability for 
/// our purposes) at about the 2 o'clock position on the Dial.  Beyond that it stays at 1.0

/// THANKS to ptr for randomseed bug report




////////// PINOUTS

#define CV_POT_IN1    A2    // Probability 1
#define CV_POT_IN2    A1    // Probability 2
#define CV_POT3       A0    // Probability 3
#define CV_IN3        A3    // Output 3
#define CV_AUDIO_IN   A4    // Clock or Trigger
#define CV_AUDIO_OUT  11    // Output 1
#define CV_GATE_OUT   8     // Output 2

#define RANDOM_PIN              A5


#define CLOCK_TRIGGER 800
#define CLOCK_COUNT 16

void setup()
    {
    randomSeed(analogRead(RANDOM_PIN));

//// Change the pin modes, even for the analog in
    pinMode(CV_AUDIO_OUT, OUTPUT);
    pinMode(CV_GATE_OUT, OUTPUT);
    pinMode(CV_IN3, OUTPUT);
//    Serial.begin(9600);
    }

int state = 0;
void pulse()
    {
    state = !state;
    if (state) 
        {
        digitalWrite(CV_AUDIO_OUT, 0);
        digitalWrite(CV_GATE_OUT, 0);
        digitalWrite(CV_IN3, 0);
        return;
        }

    uint16_t threshold1 = analogRead(CV_POT_IN1);
    uint16_t threshold2 = analogRead(CV_POT_IN2);
    uint16_t threshold3 = analogRead(CV_POT3);

// We do 1000 rather than 1023 because we want fully-right
// to essentially be guaranteed to fire.  We might want to do
// that for the bottom bound too..

    if (threshold3 > 1000)
        {
// Bernoulli
        if (random(1000) < threshold1)
            {
            digitalWrite(CV_AUDIO_OUT, 1);
            }
        else if (random(1000) < threshold2)
            {
            digitalWrite(CV_GATE_OUT, 1);
            }
        else
            {
            digitalWrite(CV_IN3, 1);
            }
        }
    else
        {
        if (random(1000) < threshold1)
            {
            digitalWrite(CV_AUDIO_OUT, 1);
            }

        if (random(1000) < threshold2)
            {
            digitalWrite(CV_GATE_OUT, 1);
            }

        if (random(1000) < threshold3)
            {
            digitalWrite(CV_IN3, 1);
            }
        }

    }


int countup = 0;
int clock = 0;

void loop()
    {
// this throwaway is advisable because Analog In has high impedance,
// and so sometimes it can't charge the capacitors fast enough in the ADC.
// As a result, the PREVIOUS analogRead() may bleed into this one.
// The throwaway blocks the bleed in most cases.  We might need two throwaways.
    analogRead(CV_AUDIO_IN);     

    uint16_t a = analogRead(CV_AUDIO_IN);
    if (a < CLOCK_TRIGGER)
        {
        countup = 0;
        clock = 0;
        }
    else if (!clock)
        {
        countup++;
        if (countup >= CLOCK_COUNT)
            {
            countup = CLOCK_COUNT;
            clock = 1;
            pulse();
            }
        }
    }
