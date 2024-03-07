// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// SWITCHBLADE
///
/// Switchblade is a combination lag smoother, fuzzer, and summed attenuverter.  Switchblade
/// is an experiment in using Mozzi to provide CV out rather than audio out.
/// Switchblade is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.
///
/// SWITCHBLADE can lag-smooth a signal and it can add noise to the signal, but it cannot do 
/// both at the same time, because they share the same control.
/// 
/// SWITCHBLADE works like this:
///
/// 1. It first takes the input in IN 1, attenuated by POT 1, and adds in IN 3.  Note that IN 3
///    reaches its maximum early due to same problem as the GRAINS bug.  If nothing is plugged 
///    into IN 3, it will be 0. 
///
/// 2. It then smooths the result to the degree instructed.  
///
/// 3. It then converts the signal with the attenuverter, flipping and potentially 
///    attenuating it around 0.5.
///
/// 4. Finally, it adds noise to the result to the degree instructed.
///
/// GRAINS's output is naturally discretized to 488 steps.  That's the finest-grain it can do.
/// So do not expect this tool to be particularly clean in its output.  Also, GRAINS's output
/// circuitry is tuned for audio, not for CV.  As a result you should expect some small 
/// degree of waver.  There is nothing that can be done about this.
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
/// IN 1            Input
/// IN 2            Attenuvert CV
/// IN 3            Second Input
/// AUDIO IN (A)    [Unused]
/// AUDIO OUT       Output
/// DIGITAL OUT (D) [Unused]
///
/// POT 1           Input Attenuation
///                 [Set Switch to IN3.  Note GRAINS bug]
///
/// POT 2           Fully Inverted <-------- Fully Attenuated --------> Normal
///                 [If you're not using Attenuvert CV, set the switch to MAN]
///                 [If you are using Attenuvert CV, note GRAINS bug]
///
/// POT 3           Very Smooth <------- Normal -------> Very Noisy



#define CONTROL_RATE 256

#include <MozziGuts.h>

#define CV_POT_IN1    A2    // Attenuvert
#define CV_POT_IN2    A1    // Offset
#define CV_POT3       A0    // Smooth / Discretize
#define CV_IN3        A3    // In
#define CV_AUDIO_IN   A4    // [Unused]
#define CV_AUDIO_OUT  9     // Out
#define CV_GATE_OUT   8     // [Unused]
#define RANDOM_PIN    A5



void setup() 
    {
    randomSeed(A5);                                             // FIXME: We're using randomSeed() and random() when we could be using the faster Mozzi versions
    startMozzi();
    }

int32_t input;
int16_t output;

float oldIn = 0;
uint16_t smooth(uint16_t in, uint8_t smoothing)
    {
    float s = smoothing;
    if (oldIn == 0) oldIn = in;
    else oldIn = (oldIn * s + (in * (128 - s))) * (1.0 / 128.0);
    return (uint16_t) oldIn;
    }

int16_t addNoise(uint16_t in, uint16_t rand)
    {
    int16_t result = in + (int16_t)random(rand) - (int16_t)256;
    if (result < 0) result = 0;
    if (result > 1023) result = 1023;
    return result;
    }
        
#define MEDIAN_OF_THREE(a,b,c) (((a) <= (b)) ? (((b) <= (c)) ? (b) : (((a) < (c)) ? (c) : (a))) : (((a) <= (c)) ? (a) : (((b) < (c)) ? (c) : (b))))
uint16_t inA = 0;
uint16_t inB = 0;

int16_t xA = 0;
int16_t xB = 0;

uint16_t yA = 0;
uint16_t yB = 0;

int16_t zA = 0;
int16_t zB = 0;


int16_t previousPrevious = -1;
int16_t previous = -1;
uint8_t counter;
void updateControl() 
    {
    // ADD AND PRE-SMOOTH
    int16_t in = mozziAnalogRead(CV_POT_IN1) + mozziAnalogRead(CV_IN3);
    int16_t cv = MEDIAN_OF_THREE(in, inA, inB);
    inA = inB;
    inB = in;
    if (cv > 1023) cv = 1023;
   
    // FIRST SMOOTH OR CURVE
    uint16_t s = mozziAnalogRead(CV_POT3);
    if (s < 512)
        {
        cv = smooth(cv, (511 - s) >> 2);                // 0...128
        }
    else
        {
        cv = addNoise(cv, (s - 512));
        }
        
    // NEXT ATTENUVERT
    uint16_t y = (y == 0 ? mozziAnalogRead(CV_POT_IN2) : (y * 3 + mozziAnalogRead(CV_POT_IN2) >> 2));
    uint16_t a = MEDIAN_OF_THREE(y, yA, yB);
    yA = yB;
    yB = y;

    input = cv;
    input -= 512;   // make signed    
    if (a >= 512) input = input * (a - 512);
    else input = (512*512) - input * (512 - a);
    input = input >> 9;
    input += 512;
    Serial.print(cv);
    Serial.print(" ");
    Serial.println(input);
    
    output = (int16_t) ((input * 487) >> 10);
    }

int updateAudio()    
    {
    return output - 244;
    }


void loop() 
    {
    audioHook();
    }
