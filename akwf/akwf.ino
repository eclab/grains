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


/// AKWF
///
/// AKWF is a wavetable synthesizer with waves available from the Adventure Kid Waveform (AKWF)
/// collection. AKWF is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.
///
/// AKWF can have 2, 3, or 5 waveforms evenly distributed in its wavetable.  You can modulate between
/// them and set the pitch CV and tune.
///
/// There is only one wave for each waveform available (there's no resampling for higher frequences).  
/// This means that you should expect significant aliasing as you go up in frequency.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.
///
/// You also will need to unzip the akwf.zip file.  It's 4MB but unzips to 20MB.
///
/// NOTE: Probably due to the voltage divider on all of its analog inputs, GRAINS is limited
/// to about a 45 note range.  
///
///
/// SPECIFYING THE WAVEFORMS
/// There are 4162 AKWF single-cycle waveforms provided, including the new Nintendo waveform collection.
/// AdventureKid's website is here:  https://www.adventurekid.se/akrt/waveforms/
///
/// You have to provide five waveform files even if you're only using less than 5 waves.  The waves
/// must all be different.  It won't matter what the other files are but they should be real ones.  
/// Provide them below.  The ones I stuck in here by default probably sound terrible together:
/// 

#define AKWF_1        "akwf/AKWF_theremin/AKWF_theremin_0002.h"              
#define AKWF_2        "akwf/AKWF_bw_sin/AKWF_sin_0001.h"
#define AKWF_3        "akwf/AKWF_nes/AKWF_nes_pulse_25.h"		
#define AKWF_4        "akwf/AKWF_0005/AKWF_0462.h"
#define AKWF_5        "akwf/AKWF_stringbox/AKWF_cheeze_0005.h"

/// Next you need to state how many waveforms are in your wavetable.  You can have 1, 2, 3, or 5 waves.
/// You CANNOT have any other values.  If you have (say) 3 waveforms, then those are the first three,
/// that is, AKWF_1, AKWF_2, and AKWF_3.  The others will not be used (but you still have to give them).
/// You specify the number here:

#define NUM_WAVES 5

/// And we're done!


/// CONFIGURATION
///
/// IN 1            Position in Wavetable CV
/// IN 2            [Unused]
/// IN 3            Pitch CV
/// AUDIO IN (A)    [Unused]
/// AUDIO OUT       Out
/// DIGITAL OUT (D) [Unused]
///
/// POT 1           Position in Wavetable
///
/// POT 2           [Unused]
///
/// POT 3           Pitch Tune +/-


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
// 6. Test.  How close does my code on the GRAINS get to correct tracking?  You might use a tuning 
//    phone app, tuned to C two octaves below middle C, and then and working your way up to see how
//    badly it gets off.
// 7. If it's too stretched, reduce the number.  If it's too compressed, increase.
// 8. Go to step 2 and repeat until the tracking looks good.
//
// Once you have the right tracking #define, you should be able to use it in all of my mozzi projects,
// and with any luck, if you let your GRAINS warm up, you won't need to change it further, fingers crossed.

#define TRACKING_47_3

#define FREQUENCY(pitch) pgm_read_float_near(&frequencies[pitch])
#define CONTROL_RATE 128

#include "tracking.h"
#include <MozziGuts.h>
#include <Oscil.h>
#include <ResonantFilter.h>



#include AKWF_1
Oscil<DATA_LENGTH, 16384> osc0(DATA);
uint16_t lengths0 = DATA_LENGTH;
#undef DATA
#undef DATA_LENGTH

#include AKWF_2
Oscil<DATA_LENGTH, 16384> osc1(DATA);
uint16_t lengths1 = DATA_LENGTH;
#undef DATA
#undef DATA_LENGTH

#include AKWF_3
Oscil<DATA_LENGTH, 16384> osc2(DATA);
uint16_t lengths2 = DATA_LENGTH;
#undef DATA
#undef DATA_LENGTH

#include AKWF_4
Oscil<DATA_LENGTH, 16384> osc3(DATA);
uint16_t lengths3 = DATA_LENGTH;
#undef DATA
#undef DATA_LENGTH

#include AKWF_5
Oscil<DATA_LENGTH, 16384> osc4(DATA);
uint16_t lengths4 = DATA_LENGTH;
#undef DATA
#undef DATA_LENGTH

#include <tables/sin512_int8.h>
Oscil<SIN512_NUM_CELLS, 16384> sine(SIN512_DATA);



#define CV_POT_IN1    A2    // Position in Wavetable 
#define CV_POT_IN2    A1    // Filter Cutoff
#define CV_POT3       A0    // Pitch Tuning
#define CV_IN3        A3    // Pitch CV
#define CV_AUDIO_IN   A4    // Resonance
#define CV_AUDIO_OUT  9     // Out
#define CV_GATE_OUT   8     // [Unused]
#define RANDOM_PIN    A5


// Median-Of-Three Filter for Pitch, dunno if we need it
#define MEDIAN_OF_THREE(a,b,c) (((a) <= (b)) ? (((b) <= (c)) ? (b) : (((a) < (c)) ? (c) : (a))) : (((a) <= (c)) ? (a) : (((b) < (c)) ? (c) : (b))))
uint16_t inA = 0;
uint16_t inB = 0;
uint16_t inC = 0;
uint16_t inD = 0;
uint16_t pitchCV;
uint16_t tuneCV;
uint16_t pitch;
uint16_t pos;
uint16_t p;
uint16_t resonance;
int16_t samples[8];


void setup() 
    {
    p = analogRead(CV_POT_IN1);
    tuneCV = analogRead(CV_POT3);
    pitchCV = analogRead(CV_IN3);
    inA = pitchCV;
    inB = pitchCV;
    
    startMozzi();
    //Serial.begin(115200);
    }


uint8_t alpha;
uint8_t sampleNum;

void updateControl() 
    {
    uint16_t in = mozziAnalogRead(CV_POT_IN1);
    pos = MEDIAN_OF_THREE(in, inC, inD);
    inC = inD;
    inD = in;
    
    // let's try a median of three filter to reduce some of the jumpiness
    tuneCV = (tuneCV * 7 + mozziAnalogRead(CV_POT3)) >> 3;
    pitchCV = (pitchCV + (tuneCV >> 1) + mozziAnalogRead(CV_IN3)) >> 1;
    uint16_t _pitch = MEDIAN_OF_THREE(pitchCV, inA, inB);
    inA = inB;
    inB = pitchCV;
    
    if (pitch != _pitch)
    	{
    	pitch = _pitch;
    	float frequency = FREQUENCY(pitch);
		
		osc0.setFreq(frequency);
	#if (NUM_WAVES > 1)
		osc1.setFreq(frequency);
	#if (NUM_WAVES > 2)
		osc2.setFreq(frequency);
	#if (NUM_WAVES > 3)
		osc3.setFreq(frequency);
		osc4.setFreq(frequency);
	#endif
	#endif
	#endif
		}

#if (NUM_WAVES == 2)
     alpha = pos >> 2;					// 0...255
#elif (NUM_WAVES == 3)
     sampleNum = pos >> 9;  // (pos > 511 ? 1 : 0);		// 0...1
     alpha = (pos - (sampleNum ? 512 : 0));	// 0...255
#else
     sampleNum = pos >> 8;				// 0...3
     alpha = (pos - (in << 8));			// 0...255
#endif

    }
    
int updateAudio()    
    {
     samples[0] = osc0.next();
#if (NUM_WAVES > 1)
     samples[1] = osc1.next();
#if (NUM_WAVES > 2)
     samples[2] = osc2.next();
#if (NUM_WAVES > 3)
     samples[3] = osc3.next();
     samples[4] = osc4.next();
#endif
#endif
#endif


#if (NUM_WAVES == 1)
	return MonoOutput::from16Bit(samples[0] * 256);
#elif (NUM_WAVES == 2)
	 return  MonoOutput::from16Bit(samples[0] * (255 - alpha) + samples[1] * alpha);
#elif (NUM_WAVES == 3)
	return  MonoOutput::from16Bit(samples[sampleNum] * (255 - alpha) + samples[sampleNum + 1] * alpha);
#else
	return MonoOutput::from16Bit(samples[sampleNum] * (255 - alpha) + samples[sampleNum + 1] * alpha);
#endif
    }


void loop() 
    {
    audioHook();
    }
