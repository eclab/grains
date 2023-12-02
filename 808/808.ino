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
//
// Each of the Drum .h files is under Mozzi's license.


/// 808
///
/// 808 is five-voice drum sampler with low-rent, noisy 8-bit drum samples presently
/// taken from an 808.
/// 808 is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// 808 is pretty simple: you have five drum triggers, one each for Kick, Clap, Rim, Claves,
/// and Closed Hat.  Pot 3 will let you change the volume:  you'll need to decrease enough 
/// that the combination of all the drums hitting at the same time doesn't produce an 
/// audible click or clipping.
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.
///
/// You can swap out the claves for a High Tom, Mid Tom, or Low Tom here:

#define USE_CLAVES
// #define USE_HIGH_TOM
// #define USE_MID_TOM
// #define USE_LOW_TOM

/// I might generalize this in the future to allow any combination of samples from a larger
/// library. But that's all for now.


/// CONFIGURATION
///
/// IN 1            Rim Trigger
/// IN 2            Hat Trigger
/// IN 3            Claves Trigger
/// AUDIO IN (A)    Clap Trigger 
/// AUDIO OUT       Out
/// DIGITAL OUT (D) Kick Trigger
///
/// POT 1           [UNUSED] [Set to IN 2]
///
/// POT 2           [UNUSED] [Set to IN 2]
///
/// POT 3           Volume Adjustment


#define CONTROL_RATE 64

#include <MozziGuts.h>
#include <Sample.h>
#include "kick808.h"
#include "clap808.h"  
#include "rim808.h"
#include "claves808.h"
#include "hat808.h"
#include "tomhigh808.h"
#include "tommid808.h"
#include "tomlow808.h"

Sample<KICK_808_LENGTH, 16384> kick808(KICK_808);
Sample<CLAP_808_LENGTH, 16384> clap808(CLAP_808);
Sample<RIM_808_LENGTH, 16384> rim808(RIM_808);
Sample<HAT_808_LENGTH, 16384> hat808(HAT_808);

#ifdef USE_CLAVES
Sample<CLAVES_808_LENGTH, 16384> claves808(CLAVES_808);
#endif
#ifdef USE_HIGH_TOM
Sample<TOM_HIGH_808_LENGTH, 16384> tomHigh808(TOM_HIGH_808);
#endif
#ifdef USE_MID_TOM
Sample<TOM_MID_808_LENGTH, 16384> tomMid808(TOM_MID_808_LENGTH);
#endif
#ifdef USE_LOW_TOM
Sample<TOM_LOW_808_LENGTH, 16384> tomLow808(TOM_LOW_808_LENGTH);
#endif

#define CV_POT_IN1    A2    // Rim Trigger
#define CV_POT_IN2    A1    // Hat Trigger
#define CV_POT3       A0    // Amplitude
#define CV_IN3        A3    // Claves Trigger
#define CV_AUDIO_IN   A4    // Clap Trigger
#define CV_AUDIO_OUT  9     // Out
#define CV_GATE_OUT   8     // Kick Trigger
#define RANDOM_PIN    A5

void setup() 
    {
    pinMode(CV_GATE_OUT, INPUT);
    startMozzi();
   	kick808.setFreq(16384 / (float) KICK_808_LENGTH / (float)1.5); 
   	clap808.setFreq(16384 / (float) CLAP_808_LENGTH); 
   	rim808.setFreq(16384 / (float) RIM_808_LENGTH); 
   	hat808.setFreq(16384 / (float) HAT_808_LENGTH); 
#ifdef USE_CLAVES   	
   	claves808.setFreq(16384 / (float) CLAVES_808_LENGTH);
#endif
#ifdef USE_HIGH_TOM
   	tomHigh808.setFreq(16384 / (float) TOM_HIGH_808_LENGTH); 
#endif
#ifdef USE_MID_TOM
   	tomMid808.setFreq(16384 / (float) TOM_MID_808_LENGTH); 
#endif
#ifdef USE_LOW_TOM
   	tomLow808.setFreq(16384 / (float) TOM_LOW_808_LENGTH); 
#endif
    }

int32_t amplitude = 0;

uint8_t kickOn;
uint8_t clapOn;
uint8_t clavesOn;
uint8_t rimOn;
uint8_t hatOn;

void updateControl() 
    {
    uint8_t kickTr = digitalRead(CV_GATE_OUT);
    uint16_t clapTr = mozziAnalogRead(CV_AUDIO_IN);
    uint16_t clavesTr = mozziAnalogRead(CV_IN3);
    uint16_t rimTr = mozziAnalogRead(CV_POT_IN1);
    uint16_t hatTr = mozziAnalogRead(CV_POT_IN2);
    
    if (kickTr && !kickOn) { kickOn = true; kick808.start(); }
    else if (!kickTr) { kickOn = false; }
    
    if (clapTr > 800 && !clapOn) { clapOn = true; clap808.start(); }
    else if (!clapTr) { clapOn = false; }
    
    if (clavesTr > 800 && !clavesOn) { clavesOn = true; 
#ifdef USE_CLAVES   	
	claves808.start(); 
#endif
#ifdef USE_HIGH_TOM
	tomHigh808.start();
#endif
#ifdef USE_MID_TOM
	tomMid808.start(); 
#endif
#ifdef USE_LOW_TOM
	tomLow808.start(); 
#endif
}
    else if (!clavesTr) { clavesOn = false; }

    if (rimTr > 800 && !rimOn) { rimOn = true; rim808.start(); }
    else if (!rimTr) { rimOn = false; }

    if (hatTr > 800 && !hatOn) { hatOn = true; hat808.start(); }
    else if (!hatTr) { hatOn = false; }

    amplitude = mozziAnalogRead(CV_POT3);
    }

int updateAudio()    
    {
    return ((kick808.next() + clap808.next() + rim808.next() + hat808.next() +
#ifdef USE_CLAVES   	
	claves808.next()
#endif
#ifdef USE_HIGH_TOM
	tomHigh808.next()
#endif
#ifdef USE_MID_TOM
	tomMid808.next() 
#endif
#ifdef USE_LOW_TOM
	tomLow808.next()
#endif
    ) * amplitude) >> 9;
    }


void loop() 
    {
    audioHook();
    }
