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


/// ADSR
///
/// ADSR is an Attack/Sustain/Decay/Release envelope with various options, including ASR
/// and AHR.  ADSR is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode, though it could probably be adapted to it.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.
///
/// Note: ADSR is an **exponential** rate-based envelope.  This means it'll sound best 
/// with stuff like filters, and won't be amazing for VCAs.  I might build a **linear** 
/// time-based envelope later for the VCA but not now.
///
/// ADSR is started by a GATE, which is presented at Digital Out (D).  Yes, it's an INPUT.
/// While ADSR is gated, you can RESET it back to its start position by sending a RESET TRIGGER
/// to Audio In (A). 
///
/// ADSR comes in four MODES:
///
/// - "Standard" (ADSR) mode.  This is the DEFAULT SETTING.  Here Decay and Release are the 
///   same value.  Pot 1 is the attack rate, Pot 2 is the Decay/Release rate, and Pot 3 + In 3 
///   is the sustain level.
///
/// - Separate Release mode.  Here, Release is separated from Decay, and now occupies In 3 as a CV.
///   Sustain is just Pot 3 (there is no Sustain CV).
///
/// - ASR (Attack/Sustain/Release) mode.  Here there is no Decay at all.  Instead, Pot 2 is
///   release.  This is similar to the ASR envelope in 2ENV, except that Sustain can still be
///   adjusted via Pot 3 + In 3.
///
/// - AHR (Attack/Hold/Release) mode.  This is a one-shot envelope with no sustain and no decay.
///   Instead, after attacking to MAXIMUM (5V), the envelope HOLDS at that value for a certain
///   amount of time and then automatically releases to 0V.  Hold occupies Pot 3 instead of 
///   Sustain.  AHR mode sports an ENVELOPE COMPLETION TRIGGER on In 3, which you can attach to 
///   RESET to cause the whole envelope to loop over and over again as long as gate is ON.
///   If you set Hold to 0, then this degenerates to an AR envelope similar to that found in 2ENV
///
/// You can only set one mode.  AHR takes precedence over ASR, which takes precedence over Separate
/// Release, which takes precedence over Standard.


/// SEPARATE RELEASE MODE
///
/// To turn this on, set RELEASE to 1 rather than 0 below.  This also turns off Standard Mode.

#define RELEASE 0                                       // Change to 1 to turn on SEPARATE RELEASE MODE.



/// ASR MODE
///
/// To turn this on, set ASR to 1 rather than 0 below.  This also turns off Separate Release 
/// and Standard Mode.

#define ASR 0                                   // Change to 1 to turn on ASR



/// AHR ONE-SHOT MODE
///
/// To turn on AHR ONE SHOT mode, set AHR below to 1 rather than 0.  This also turns off ASR,
/// Separate Release, and Standard Mode. 

#define AHR 0                                   // Change to 1 to turn on AHR ONE SHOT



/// GRAINS BUG
/// 
/// There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the 
/// switch to "Man", then the range of the Pot is correct.  But if you set the switch 
/// to "In 1" (or "In 2"), then the range of the Dial is at maximum at about the 
/// 2 o'clock position on the Dial.  Beyond that it stays at maximum.


/// STANDARD ADSR CONFIGURATION 
///
/// IN 1            Attack CV
/// IN 2            Decay/Release CV
/// IN 3            Sustain CV
/// AUDIO IN (A)    Reset
/// AUDIO OUT       Output
/// DIGITAL OUT (D) Gate
///
/// POT 1           Attack
///                 [If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
///
/// POT 2           Decay/Release
///                                     [If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
///
/// POT 3           Sustain Level
        
        
        
/// SEPARATE RELEASE CONFIGURATION 
///
/// IN 1            Attack CV
/// IN 2            Decay CV
/// IN 3            Release CV
/// AUDIO IN (A)    Reset
/// AUDIO OUT       Output
/// DIGITAL OUT (D) Gate
///
/// POT 1           Attack
///                 [If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
///
/// POT 2           Decay
///                                     [If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
///
/// POT 3           Sustain Level
        


/// ASR CONFIGURATION 
///
/// IN 1            Attack CV
/// IN 2            Release CV
/// IN 3            Sustain CV
/// AUDIO IN (A)    Reset
/// AUDIO OUT       Output
/// DIGITAL OUT (D) Gate
///
/// POT 1           Attack
///                 [If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
///
/// POT 2           Release
///                                     [If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
///
/// POT 3           Sustain Level
        
        


/// AHR ONE-SHOT CONFIGURATION 
///
/// IN 1            Attack CV
/// IN 2            Release CV
/// IN 3            Release Completion Trigger Output
/// AUDIO IN (A)    Reset
/// AUDIO OUT       Output
/// DIGITAL OUT (D) Gate
///
/// POT 1           Attack
///                 [If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
///
/// POT 2           Release
///                                     [If you're not using the CV, set the switch to MAN.  Note GRAINS bug]
///
/// POT 3           Hold
        
        
        
#define NUM_STAGES                      3
#define SUSTAIN_STAGE           1

float stages[NUM_STAGES][2] =
    {
    //RATE //TO LEVEL
    { 0.0, 1.0 },
    { 0.999, 0.5 },
    { 0.9995, 0.0 },
    };

float rates[256] = {
// 0., 0.00981045, 0.0197171, 0.029721, 0.039823, 0.0500242, 0.0603254, 
// 0.0707276, 0.081232, 0.0918393, 0.102551, 0.113367, 0.12429, 0.13532, 
// 0.146458, 0.157705, 0.169063, 0.180532, 0.192113, 0.203808, 0.215618, 
// 0.227544, 0.239587, 0.251748, 0.264028, 0.276428, 0.288951, 0.301596, 
// 0.314365, 0.32726, 0.340281, 0.353429,
//0.366707, 0.380115, 0.393655, 
//0.407327, 0.421134, 0.435076, 0.449154, 0.463371, 0.477728, 0.492225, 
//0.506864, 0.521647, 0.536575, 0.55165, 0.566872, 0.582244, 0.597766, 
//0.613441, 0.62927, 0.645253, 0.661394, 0.677693, 0.694152, 0.710772, 
//0.727556, 0.744504, 0.761618, 0.778901, 0.796352, 0.813975, 0.831771, 
//0.849742,

    0.0, 0.900981, 0.901972, 0.902972, 0.903982, 0.905002, 0.906033, 
    0.907073, 0.908123, 0.909184, 0.910255, 0.911337, 0.912429, 0.913532, 
    0.914646, 0.91577, 0.916906, 0.918053, 0.919211, 0.920381, 0.921562, 
    0.922754, 0.923959, 0.925175, 0.926403, 0.927643, 0.928895, 0.93016, 
    0.931437, 0.932726, 0.934028, 0.935343, 0.936671, 0.938012, 0.939365, 
    0.940733, 0.942113, 0.943508, 0.944915, 0.946337, 0.947773, 0.949222, 
    0.950686, 0.952165, 0.953658, 0.955165, 0.956687, 0.958224, 0.959777, 
    0.961344, 0.962927, 0.964525, 0.966139, 0.967769, 0.969415, 0.971077, 
    0.972756, 0.97445, 0.976162, 0.97789, 0.979635, 0.981398, 0.983177, 
    0.984974,

    0.99, 0.990098, 0.990197, 0.990297, 0.990398, 0.9905, 0.990603, 
    0.990707, 0.990812, 0.990918, 0.991026, 0.991134, 0.991243, 0.991353, 
    0.991465, 0.991577, 0.991691, 0.991805, 0.991921, 0.992038, 0.992156, 
    0.992275, 0.992396, 0.992517, 0.99264, 0.992764, 0.99289, 0.993016, 
    0.993144, 0.993273, 0.993403, 0.993534, 0.993667, 0.993801, 0.993937, 
    0.994073, 0.994211, 0.994351, 0.994492, 0.994634, 0.994777, 0.994922, 
    0.995069, 0.995216, 0.995366, 0.995516, 0.995669, 0.995822, 0.995978, 
    0.996134, 0.996293, 0.996453, 0.996614, 0.996777, 0.996942, 0.997108, 
    0.997276, 0.997445, 0.997616, 0.997789, 0.997964, 0.99814, 0.998318, 
    0.998497,

    0.999, 0.99901, 0.99902, 0.99903, 0.99904, 0.99905, 0.99906, 
    0.999071, 0.999081, 0.999092, 0.999103, 0.999113, 0.999124, 0.999135, 
    0.999146, 0.999158, 0.999169, 0.999181, 0.999192, 0.999204, 0.999216, 
    0.999228, 0.99924, 0.999252, 0.999264, 0.999276, 0.999289, 0.999302, 
    0.999314, 0.999327, 0.99934, 0.999353, 0.999367, 0.99938, 0.999394, 
    0.999407, 0.999421, 0.999435, 0.999449, 0.999463, 0.999478, 0.999492, 
    0.999507, 0.999522, 0.999537, 0.999552, 0.999567, 0.999582, 0.999598, 
    0.999613, 0.999629, 0.999645, 0.999661, 0.999678, 0.999694, 0.999711, 
    0.999728, 0.999745, 0.999762, 0.999779, 0.999796, 0.999814, 0.999832, 
    0.99985,

    0.9999, 0.999901, 0.999902, 0.999903, 0.999904, 0.999905, 0.999906, 
    0.999907, 0.999908, 0.999909, 0.99991, 0.999911, 0.999912, 0.999914, 
    0.999915, 0.999916, 0.999917, 0.999918, 0.999919, 0.99992, 0.999922, 
    0.999923, 0.999924, 0.999925, 0.999926, 0.999928, 0.999929, 0.99993, 
    0.999931, 0.999933, 0.999934, 0.999935, 

    0.999937, 0.999938, 0.999939, 
    0.999941, 0.999942, 0.999944, 0.999945, 0.999946, 0.999948, 0.999949, 
    0.999951, 0.999952, 0.999954, 0.999955, 0.999957, 0.999958, 0.99996, 
    0.999961, 0.999963, 0.999965, 0.999966, 0.999968, 0.999969, 0.999971, 
    0.999973, 0.999974, 0.999976, 0.999978, 0.99998, 0.999981, 0.999983, 
    0.999985,
    };


#define CONTROL_RATE 256

#include <MozziGuts.h>

#define CV_POT_IN1    A2    // Attack
#define CV_POT_IN2    A1    // [ADSR] Decay/Release [RELEASE] Decay [ASR] Release [AHR] Release
#define CV_POT3       A0    // [ADSR] Sustain [RELEASE] Sustain [ASR] Sustain [AHR] Hold
#define CV_IN3        A3    // [ADSR] Sustain CV [RELEASE] Release CV [ASR] Sustain CV [AHR] Release Trigger
#define CV_AUDIO_IN   A4    // Reset 
#define CV_AUDIO_OUT  9     // Output
#define CV_GATE_OUT   8     // Gate
#define RANDOM_PIN    A5

#define TRIGGER_OFF_COUNT 100
uint8_t terminationCounter = 0;


void setup() 
    {
#if (AHR == 1)
    pinMode(CV_IN3, OUTPUT);
    pinMode(CV_GATE_OUT, INPUT);
    pinMode(CV_AUDIO_IN, INPUT);
#elif (ASR == 1)
    pinMode(CV_IN3, INPUT);
    pinMode(CV_GATE_OUT, INPUT);
    pinMode(CV_AUDIO_IN, INPUT);
#elif (RELEASE == 1)
    pinMode(CV_IN3, INPUT);
    pinMode(CV_GATE_OUT, INPUT);
    pinMode(CV_AUDIO_IN, INPUT);
#else
    pinMode(CV_IN3, INPUT);
    pinMode(CV_GATE_OUT, INPUT);
    pinMode(CV_AUDIO_IN, INPUT);
#endif
    startMozzi();
    }
        
#define HOLD 1
#define DONE (-1)
int8_t stage = DONE;

uint8_t reset = 0;
uint8_t gate = 0;
float target = 0;

// When smaller, stages take longer to focus in on their targets
// But since our minimum resolution is 1/448, this seems to be
// a reasonable value without jumping.
#define EPSILON (0.001)
float state = 0;

void doReset()
    {
    stage = 0;
    state = 0.0;
    }

void doGate()
    {
    gate = true;
    stage = 0;
    }

void doRelease()
    {
    gate = false;
    }
                
void updateStateMachine()
    {
#if (AHR == 1)
    if (terminationCounter == 1) digitalWrite(CV_IN3, 0);
    if (terminationCounter > 0) terminationCounter--;
#else
// nothing
#endif

    if (gate)
        {
        if (stage == DONE)                      
            {
            // do nothing
            }
        else if (stage == SUSTAIN_STAGE + 1 && !AHR)
            {
            // do nothing, we're sustaining
            }
        else if (stage >= NUM_STAGES)
            {
            // all done: probably one-shot
            stage = DONE;
#if (AHR == 1)
            digitalWrite(CV_IN3, 1);
            terminationCounter = TRIGGER_OFF_COUNT;
#else
// nothing
#endif
            }
        else
            {
            float target = stages[stage][1];
            float alpha = stages[stage][0];
            float t = target;
#if (AHR == 1)
            if (stage == HOLD)      // drop to zero but we won't count it
                t = 0;
#endif

            state =  alpha * state + (1.0 - alpha) * t;
            if (abs(state - t) <= EPSILON)
                {
                // Completed the stage
                state = target;
                stage++;
                }
            }
        }
    else                    // GATE OFF
        {
        if (stage == DONE)
            {
            // Do nothing
            }
        else if (stage >= NUM_STAGES)
            {
            // all done
            stage = DONE;
#if (AHR == 1)
            digitalWrite(CV_IN3, 1);
            terminationCounter = TRIGGER_OFF_COUNT;
#else
// nothing
#endif
            }
        else if (stage <= SUSTAIN_STAGE)
            {
            stage = SUSTAIN_STAGE + 1;
            }
        else
            {
            float target = stages[stage][1];
            float alpha = stages[stage][0];
            state =  alpha * state + (1.0 - alpha) * target;
                        
            if (abs(state - target) <= EPSILON)  // we're done
                {
                // Completed the stage
                state = target;
                stage++;
                }
            }
        }
    }

#define INITIAL (1024)
uint16_t at = INITIAL;
uint16_t de = INITIAL;
uint16_t su = INITIAL;
uint16_t re = INITIAL;

#define TRIGGER_UP (500)
#define TRIGGER_DOWN (100)

void updateControl() 
    {
    // Test for incoming gate changes
    uint16_t g = digitalRead(CV_GATE_OUT);
    // Test for reset
    uint16_t r = mozziAnalogRead(CV_AUDIO_IN);

    at = (at == INITIAL ? mozziAnalogRead(CV_POT_IN1) : (at * 7 + mozziAnalogRead(CV_POT_IN1)) >> 3);
    de = (de == INITIAL ? mozziAnalogRead(CV_POT_IN2) : (de * 7 + mozziAnalogRead(CV_POT_IN2)) >> 3);
    su = (su == INITIAL ? mozziAnalogRead(CV_POT3) : (su * 7 + mozziAnalogRead(CV_POT3)) >> 3);

#if (AHR == 1)
    stages[0][0] = rates[(at * 8) >> 5];                    // attack
    stages[1][0] = rates[(su * 8) >> 5];                    // sustain knob now is HOLD RATE (decay rate)
    stages[1][1] = 1.0;                                                             // sustain stays at 1.0
    stages[2][0] = rates[(de * 8) >> 5];                    // decay knob now is RELEASE
#elif (ASR == 1)
    re = (re == INITIAL ? mozziAnalogRead(CV_IN3) : (re * 7 + mozziAnalogRead(CV_IN3)) >> 3);
    stages[0][0] = rates[(at * 8) >> 5];                    // attack
    stages[0][1] = (su + re) / 1023.0;                              // attack goes to sustain level
    if (stages[0][1] > 1.0) stages[0][1] = 1.0;
    stages[1][0] = 0;                                                               // no decay
    stages[1][1] = stages[0][1];                                            // sustain
    stages[2][0] = rates[(de * 8) >> 5];                    // release uses decay rate
#elif (RELEASE == 1)
    re = (re == INITIAL ? mozziAnalogRead(CV_IN3) : (re * 7 + mozziAnalogRead(CV_IN3)) >> 3);
    stages[0][0] = rates[(at * 8) >> 5];                    // attack
    stages[1][0] = rates[(de * 8) >> 5];                    // decay
    stages[1][1] = su / 1023.0;                                             // sustain
    stages[2][0] = rates[(re * 8) >> 5];                    // release is now IN3
#else
    re = (re == INITIAL ? mozziAnalogRead(CV_IN3) : (re * 7 + mozziAnalogRead(CV_IN3)) >> 3);
    stages[0][0] = rates[(at * 8) >> 5];                    // attack
    stages[1][0] = rates[(de * 8) >> 5];                    // decay
    stages[1][1] = (su + re) / 1023.0;                              // sustain
    if (stages[1][1] > 1.0) stages[1][1] = 1.0;
    stages[2][0] = stages[1][0];                                    // release
#endif  
    if (g && !gate)
        {
        gate = 1;
        doGate();
        }
    else if (!g && gate)
        {
        gate = 0;
        doRelease();
        }

    // Only reset if gate is on
    if (r > TRIGGER_UP && !reset && gate)
        {
        reset = 1;
        doReset();
        }
    else if (r < TRIGGER_DOWN)
        {
        reset = 0;
        }
    }


int updateAudio()    
    {
    updateStateMachine();
#if (AHR == 1)
    if (stage == HOLD)
        return ((int16_t)(stages[0][1]) * 487) - 244;
    else
#endif
        return ((int16_t)(state * 487)) - 244;
    }

void loop() 
    {
    audioHook();
    }
