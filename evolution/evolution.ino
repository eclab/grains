// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License
//
// WARNING: Mozzi itself is released under a broken non-open-source license, namely the 
// Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
// This license is not compatible with the LGPL (used by the Arduino itself!) and is
// also viral, AND is non-commercial only.  What a mess.  I am pushing them to change 
// their license to something reasonable like Apache or GPL but in the meantime I don't 
// have much choice but to turn my head and ignore the broken license.  So I'm releasing
// under Apache for the time being.


/// EVOLUTION
///
/// Evolution is an evolving drone synthesizer using 9-partial additive synthesis.  
/// Evolution is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// All Evolution does is randomly (and usually slowly) change the amplitudes of 8 of 
/// the partials, leaving the fundamental alone.  You have 8 choices of frequency 
/// combinations.  The frequencies of the partials can be found 
/// in partialFrequencies[] if you wanted to modify them.
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.
///
/// NOTE: Probably due to the voltage divider on all of its analog inputs, GRAINS is limited
/// to about a 45 note range.  


/// CONFIGURATION
///
/// IN 1            [Unused]
/// IN 2            [Unused]
/// IN 3            Pitch CV    [This isn't IN2, like Pitch Tune, because Pitch Tune must be +/-]
/// AUDIO IN (A)    [Unused]
/// AUDIO OUT       Out
/// DIGITAL OUT (D) Clock
///
/// POT 1           Choice of Partials [Set the switch to MAN]
///
/// POT 2           Pitch Tune  [Set the switch to MAN]
///
/// POT 3           Evolution Rate.  If set to far left, only evolves when clocked


// ADJUSTING TRACKING

// GRAINS inputs are not v/oct: they're approximately 1.3v/oct, and the exact amount
// varies depending on how warmed up the GRAINS resistors are, and also based on how close
// to 5V your MASTER I/O is (you can tune that), how much total load is on the system, etc.
// As a result, your GRAINS tracking will be out of whack.  You'll need to adjust it.  Hopefully
// you won't have to adjust it every time, if you let your GRAINS warm up first.  :-) 
//
// Below you will find a #define TRACKING_47_3.  This sets the tracking value to 47.3, which is
// the value appropriate for my own personal GRAINS.  If Grains was a perfect v/oct, this would 
// have been 60 (60 semitones = 5 octaves).  Your GRAINS value will be somewhat different from 47.3.
// You can change it to a different value, say, 42.9, which would be:     #define TRACKING_42_9
// It's always three digits in the format  #define TRACKING_XX_Y
//
// I have provided TRACKING values from 40.0 to 50.9, that is, TRACKING_40_0 to TRACKING_50_9
// If you need values outside of this, send me mail.
//
// To tune the tracking, do this:
//
// 0. Let the GRAINS warm up.  A lot.
// 1. Start with your best guess of tracking (let's say 47.3; or with another MASTER I/O I had 45)
// 2. Set the #define below to that value.
// 3. Plug in the USB cable.  
// 4. Compile and upload the code
// 5. UNPLUG the USB cable.  This is important: USB boosts GRAINS voltage, which messes things up 
// 6. Test.  How close does my code on the GRAINS get to correct tracking? 
// 7. If it's too stretched, reduce the number.  If it's too compressed, increase.
// 8. Go to step 2 and repeat until the tracking looks good.
//
// Once you have the right tracking #define, you should be able to use it in all of my mozzi projects,
// and with any luck, if you let your GRAINS warm up, you won't need to change it further, fingers crossed.

#define TRACKING_47_3



#define FREQUENCY(pitch) pgm_read_float_near(&frequencies[pitch])
#define CONTROL_RATE 100                        // More than this and we're gonna get clicks probably

#include "tracking.h"
#include <MozziGuts.h>
#include <Oscil.h>
#include <mozzi_rand.h>
#include <tables/sin256_int8.h>

/// OSCILLATORS

Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel1(SIN256_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel2(SIN256_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel3(SIN256_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel4(SIN256_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel5(SIN256_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel6(SIN256_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel7(SIN256_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel8(SIN256_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel9(SIN256_DATA);

Oscil<SIN256_NUM_CELLS, AUDIO_RATE> oscils[9] = 
    { 
    tonewheel1, tonewheel2, tonewheel3,
    tonewheel4, tonewheel5, tonewheel6, 
    tonewheel7, tonewheel8, tonewheel9 
    };

const float partialFrequencies[8][9] = 
    { 
    { 1, 2, 3, 4, 5, 6, 7, 8, 9 },
    { 1, 2, 4, 6, 8, 10, 12, 14, 16 },
    { 1, 3, 5, 7, 9, 11, 13, 15, 17 },
    { 1, 4, 7, 10, 13, 16, 19, 22, 25 },
    { 1, 2, 3, 4, 1.05, 2.1, 3.15, 4.2, 5 },
    { 1, 2, 3, 1.05, 2.1, 3.15, 1.1, 2.2, 3.3 },
    { 1, 2, 3, 4, 5, 6, 1.1, 2.2, 3.3 },
    { 1, 1.01, 1.02, 1.03, 1.04, .99, .98, .97, .96 },
    };
        
uint8_t cutdowns[8][9];

#define CV_POT_IN1    A2    // Partials Selection
#define CV_POT_IN2    A1    // Pitch Tune
#define CV_POT3       A0    // Evolution Rate
#define CV_IN3        A3    // Pitch
#define CV_AUDIO_IN   A4    // [Unused]
#define CV_AUDIO_OUT  9     // Out
#define CV_GATE_OUT   8     // Clock
#define RANDOM_PIN    A5

uint8_t targets[9];
uint8_t amplitudes[9];
uint8_t cutdownAmplitudes1[8][9];
uint8_t cutdownAmplitudes2[8][9];
uint8_t finalAmplitudes[9];

void setup()
    {
    startMozzi();
    randSeed(RANDOM_PIN);
    pinMode(CV_GATE_OUT, INPUT_PULLUP);           // duh
  
    amplitudes[0] = 255;
    for(uint8_t i = 1; i < 9; i++)
        {
        targets[i] = rand(256);
        amplitudes[i] = rand(256);
        }
        
    for(uint8_t i = 0; i < 8; i++)
        {
        for(uint8_t j = 0; j < 9; j++)
            {
            cutdownAmplitudes1[i][j] = (uint8_t)(255.0 / partialFrequencies[i][j]);                 // dark
            cutdownAmplitudes2[i][j] = (uint8_t)(255.0 / sqrt(partialFrequencies[i][j]));   // medium
            }
        }
    }

uint8_t locked = true;          // initially TRUE so that if we have nothing plugged into CV_GATE_OUT, we never clock because we think we already have.  Arduino only has a pullup, not a pull-down.
uint8_t rate = 0;
uint8_t counter = 1;

void loop()
    {
    audioHook();
    }


// Median-Of-Three Filter for Pitch, dunno if we need it
#define MEDIAN_OF_THREE(a,b,c) (((a) <= (b)) ? (((b) <= (c)) ? (b) : (((a) < (c)) ? (c) : (a))) : (((a) <= (c)) ? (a) : (((b) < (c)) ? (c) : (b))))
uint16_t inA = 0;
uint16_t inB = 0;
uint16_t pitchCV;
uint16_t tuneCV;
uint16_t pitch;
uint8_t gainDivide = 11;
uint8_t draw = 0;

// For the time being we're making sure that the amplitudes follow a sawtooth/square cut-down with frequency
void convertAmplitudes(uint8_t draw)
    {
    if (draw < 8)
        {
        gainDivide = 10;                // they're dark and quiet
        for(uint8_t i = 1; i < 9; i++)
            finalAmplitudes[i] = (amplitudes[i] * cutdownAmplitudes1[draw][i]) >> 8;
        }
    else if (draw < 16)
        {
        gainDivide = 10;                // they're medium
        for(uint8_t i = 1; i < 9; i++)
            finalAmplitudes[i] = (amplitudes[i] * cutdownAmplitudes2[draw][i]) >> 8;
        }
    else
        {
        gainDivide = 11;                // they're bright and loud
        for(uint8_t i = 1; i < 9; i++)
            finalAmplitudes[i] = amplitudes[i];
        }
    }

int8_t drawCounter = -1;
void updateControl()                          
    {
    uint16_t in = mozziAnalogRead(CV_IN3);
    // let's try a median of three filter to reduce some of the jumpiness
    uint16_t pitchCV = MEDIAN_OF_THREE(in, inA, inB);
    inA = inB;
    inB = in;
    tuneCV = mozziAnalogRead(CV_POT_IN2);
//      tuneCV = (tuneCV * 7 + mozziAnalogRead(CV_POT_IN2)) >> 3;
    pitch = (pitch * 7 + pitchCV + (tuneCV >> 1)) >> 3;
        
    float frequency = FREQUENCY(pitch);
        
    uint8_t d = (mozziAnalogRead(CV_POT_IN1) * 24) >> 10;
    if (d != draw)
        {
        if (drawCounter == -1) drawCounter = 10;
        else if (drawCounter == 0) draw = d;
        drawCounter--;
        }
    else drawCounter = -1;
        
    // set the partials
    const float* freq = partialFrequencies[draw >= 16 ? draw - 16: draw >= 8 ? draw - 8 : draw];
    for(uint8_t i = 0; i < 9; i++)
        {
        oscils[i].setFreq(frequency * freq[i]);
        }
                
    // determine the rate
    rate = (mozziAnalogRead(CV_POT3) >> 4);
    if (rate > 0)
        {
        if (counter > 64)
            {
            counter = 1;            // rate must be > 0 here remember
            // update the targets
            for(uint8_t i = 1; i < 9; i++)
                {
                if (amplitudes[i] < targets[i])
                    {
                    if (targets[i] - amplitudes[i] >= 2) amplitudes[i] += 2;
                    else targets[i] = rand(256);
                    }
                else
                    {
                    if (amplitudes[i] - targets[i] >= 2) amplitudes[i] -= 2;
                    else targets[i] = rand(256);
                    }
                }
            convertAmplitudes(draw);
            }
        else counter += rate;
        }
    else
        {
        counter = 1;
        if (digitalRead(CV_GATE_OUT))
            {
            if (!locked)
                {
                locked = true;
                for(uint8_t i = 1; i < 9; i++)
                    {
                    amplitudes[i] = rand(256);
                    }
                convertAmplitudes(draw);
                }
            }
        else { locked = false; }
        }
    }                                             


int updateAudio()                             
    {
    int32_t val = oscils[0].next() * 256;
    for(uint8_t i = 1; i < 9; i++)
        {
        val += (oscils[i].next() * finalAmplitudes[i]);
        }
    return (int)(val >> gainDivide);              // seems to get low enough?
    }
