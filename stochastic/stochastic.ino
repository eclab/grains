// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// STOCHASTIC
///
/// Stochastic is a combination random walk wanderer, random-walk sample and hold, and fuzzer.
/// Stochastic is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.
///
/// Stochastic wanders about randomly using a RATE and a VARIANCE.  It has two ways of wandering.  
/// First, it can do a random walk: it heads towards a randomly-chosen target, and when it reaches 
/// the target, it picks a new one relative to the old target.  Or it can wander randomly about a 
/// mean, always picking new targets relative to the mean.
/// 
/// The mean, if you have chosen to use it, is specified by an incoming CV on AUDIO IN.
/// 
/// Stochastic can also do its random-walk or walk-about-mean, but output its randomly chosen 
/// targets rather than outputting its current point as it heads to them: this is essentially a 
/// sample-and-hold.
/// 
/// To this random generated signal it adds some degree of FUZZ.  This is then output.
/// 
/// Additionally when Stochastic's random-walk wanderer / sample and hold selects a new target 
/// value (as determined by the RATE), a TRIGGER is signaled.
/// 
/// You can reset Stochastic to pick a new target by sending it a RESET trigger.
/// 
///
/// GRAINS's output is naturally discretized to 488 steps.  That's the finest-grain it can do.
/// So do not expect this tool to be particularly clean in its output.  Also, GRAINS's output
/// circuitries is tuned for audio, not for CV.  As a result you should expect some small 
/// degree of waver.  There is nothing that can be done about this.
///
///
/// Looking for a more traditional sample and hold, random sample and hold, and track and hold?
/// Try RUCKUS.
///
///
/// GRAINS BUG
/// 
/// There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the 
/// switch to "Man", then the range of the Pot is correct.  But if you set the switch 
/// to "In 1" (or "In 2"), then the range of the Dial is at maximum at about the 
/// 2 o'clock position on the Dial.  Beyond that it stays at maximum.


/// CONFIGURATION
///
/// IN 1            Wander / S&H Variance CV
/// IN 2            Wander / S&H Rate CV
/// IN 3            Reset
/// AUDIO IN (A)    Mean
/// AUDIO OUT       Output
/// DIGITAL OUT (D) Output Trigger
///
/// POT 1           Variance / Choice of Random Walk vs Walk About Mean
///                                     Low Variance, Random Walk <----> High Variance <----> Low Variance, About Mean
///                 [If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
///
/// POT 2           Wander / S&H Rate
///                 [If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
///
/// POT 3           S&H No Fuzz <------- High Fuzz --------> Wander No Fuzz



#define CONTROL_RATE 256

#include <MozziGuts.h>

#define CV_POT_IN1    A2    // Wandering Variance
#define CV_POT_IN2    A1    // Wandering Rate
#define CV_POT3       A0    // Fuzz / Sample and Hold
#define CV_IN3        A3    // Reset
#define CV_AUDIO_IN   A4    // [Unused]
#define CV_AUDIO_OUT  9     // Out
#define CV_GATE_OUT   8     // Tick
#define RANDOM_PIN    A5

/*
// This log implementation is from 
// https://stackoverflow.com/questions/11376288/fast-computing-of-log2-for-64-bit-integers
const int tab32[32] = {
0,  9,  1, 10, 13, 21,  2, 29,
11, 14, 16, 18, 22, 25,  3, 30,
8, 12, 20, 28, 15, 17, 24,  7,
19, 27, 23,  6, 26,  5,  4, 31};

int log2_32 (uint32_t value)
{
value |= value >> 1;
value |= value >> 2;
value |= value >> 4;
value |= value >> 8;
value |= value >> 16;
return tab32[(uint32_t)(value*0x07C4ACDD) >> 27];
}
*/

void setup() 
    {
    randomSeed(RANDOM_PIN);                                             // FIXME: We're using randomSeed() and random() when we could be using the faster Mozzi versions
    pinMode(CV_IN3, INPUT);
    pinMode(CV_GATE_OUT, OUTPUT);
    startMozzi();
    }
        
uint16_t randomState = 512;
uint16_t lastTarget = 0;
uint16_t randomTarget = 512;
uint16_t time = 0;
uint16_t rate = 0;
uint16_t current;
uint16_t output;
uint8_t counter = 0;
#define TRIGGER_OFF_COUNT 100
#define NUM_TRIES 16

void updateControl() 
    {
    if (mozziAnalogRead(CV_IN3) > 800) { randomState = randomTarget; }
    uint16_t variance = mozziAnalogRead(CV_POT_IN1);
    
    rate = mozziAnalogRead(CV_POT_IN2);  // log2_32(mozziAnalogRead(CV_POT_IN2)) + 1) * 4;
    uint16_t fuzz = mozziAnalogRead(CV_POT3);
    boolean sampleAndHold = (fuzz < 512);
    fuzz = 511 - (fuzz >= 512 ? fuzz - 512 : 511 - fuzz);
    
    // fuzz ranges 0..511
    
    if (randomState == randomTarget)
        {
        counter = TRIGGER_OFF_COUNT;
        digitalWrite(CV_GATE_OUT, 1);
        // pick a new randomTarget
        for(uint8_t tries = 0; tries < NUM_TRIES; tries++)      // hopefully this is enough!!!!
            {
            int16_t newTarget;
                
            if (variance >= 512)
                {
                uint16_t mean = mozziAnalogRead(CV_AUDIO_IN);
                variance = 1023 - variance;
                newTarget = mean - (variance >> 1) + random(variance + 1);
                }
            else
                {
                newTarget = randomTarget - (variance >> 1) + random(variance + 1);
                }
            if (newTarget >= 0 && newTarget < 1024 && newTarget != randomTarget &&
                    ((lastTarget < randomTarget && newTarget < randomTarget) ||
                    (lastTarget > randomTarget && newTarget > randomTarget))) 
                { lastTarget = randomTarget; randomTarget = newTarget; break; }
            }
        time = 0;
        current = randomState;          // for S&H
        }

    if (sampleAndHold)
        {
        output = current;
        }
    else
        {
        output = randomState;
        }
                
    for(uint8_t tries = 0; tries < NUM_TRIES; tries++)  // hopefully this is enough!!!!
        {
        output = output -fuzz + random(fuzz + 1);
        if (output >= 0 && output < 1024) break;
        }
                
    // convert to 0...488
    output = ((((uint32_t)output) * 488) >> 10);
    }

int updateAudio()    
    {
    time++;
    if (time >= rate)
        {
        time = 0;
        if (randomState < randomTarget)
            {
            randomState++;
            }
        else if (randomState > randomTarget)
            {
            randomState--;
            }
        }

    return output - 244;
    }


void loop() 
    {
    if (counter == 1) digitalWrite(CV_GATE_OUT, 0);
    if (counter > 0) counter--;
    audioHook();
    }
