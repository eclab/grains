// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// BEATS
///
/// Beats is a 4-track drum trigger sequencer with song mode.  Beats is meant to 
/// run on the AE Modular GRAINS, but it could be adapted to any Arduino.
/// Beats was written by Sean Luke [sean@cs.gmu.edu]
///
/// Beats can store anywhere from 25 to 800 PATTERNS depending on pattern length.   
/// Each pattern has four drum tracks up to 256 
/// steps long.  A pattern can be repeated any number of times, at which point we
/// proceed to the next pattern.  Patterns can also be repeated FOREVER.  You have the
/// option to manually ADVANCE to the next pattern in the sequence, or to RESET the
/// sequence.  Beats has a rudimentary SWING control and RANDOMNESS features.
///
/// SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)
///
/// CONFIGURATION
///
/// IN 1            Clock, CV for Randomness
/// IN 2            Reset / Advance
/// IN 3            Drum 3      [Yes, it's a trigger output]
/// AUDIO IN (A)    Drum 4      [Yes, it's a trigger output]
/// AUDIO OUT       Drum 1
/// DIGITAL OUT (D) Drum 2
///
/// POT 1           Randomness [Minimum Randomness at 2 o'clock, Maximum a bit below 9 o'clock] 
///                 [Switch must be set to IN 1]
///                                             [Turn to far left to pause the clock]
///
/// POT 2           [Set to 9 O'clock for IN 2 to ADVANCE] 
///                 [Set to FULL for IN 2 to RESET] 
///                 [Switch must be set to IN 2]
///
/// POT 3           Swing
///
///
/// ABOUT THE RANDOMNESS CV
///
/// Normally you just attach clock to In 1, and then set randomness with the dial.  
/// But if you really wanted to, you could attach a CV for Randomness to In 1 as well.  
/// You'd do this by using the CV to attenuate the clock signal.  For example, you'd have 
/// the clock sent to a VCA set to DC mode, and use feed your CV into the VCA's CV.  
/// The output of the VCA would go into In 1.  If you did this, you probably should set 
/// POT 1 to FULL (far right).  Note that below a certain minimum (about 1/16 full) the 
/// clock will no longer trigger Beats.
///
///
/// GRAINS BUG
/// 
/// There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the 
/// switch to "Man", then the range of the Pot is correct.  But if you set the switch 
/// to "In 1" (or "In 2"), then the range of the Dial is maximum (1.0 probability for 
/// our purposes) at about the 2 o'clock position on the Dial.  Beyond that it stays at 1.0
///
///
/// PROGRAMMING BEATS
///
/// First we need to define the MAXIMUM LENGTH for any pattern in your song.  The simple approach is to just
/// stick with 256:

#define MAX_PATTERN_LENGTH 256

/// However, this restricts you to just 25 patterns in your GRAINS memory (actually 26 but who's counting?). 
/// That might be plenty, but if you need more memory, you can reduce the maximum pattern length to 
/// 128, 64, 32, 16, or 8, which will increase your available of patterns correspondingly to roughly 
/// 50, 100, 200, 400, and 800.


/// Now we define the actual number of 4-track patterns in your sequence, given available memory as discussed
/// above.  By default you can fit about 25.

#define PATTERNS 2        // Set this to the number of patterns in your sequence (any number > 0)


/// Next we define whether the whole sequence loops forever or only runs once:

#define LOOP 1          // Set this to 0 to prevent the sequence from looping forever

/// Finally, we determine whether randomness ADDS and REMOVES beats or if it just REMOVES them.

#define ADDS 1          // Set this to 0 if you want randomness to just remove beats


/// Based on this information, Beats defines a pattern (don't fool with this):
struct pattern
    {
    uint8_t length;
    uint8_t iterations;
    uint8_t track1[MAX_PATTERN_LENGTH];
    uint8_t track2[MAX_PATTERN_LENGTH];
    uint8_t track3[MAX_PATTERN_LENGTH];
    uint8_t track4[MAX_PATTERN_LENGTH];  
    };


/// Beats also uses this information to construct some #defines you will use later.  Don't fool with these:
/// The defines are EMPTY and FOREVER

#define FOREVER 0
#if MAX_PATTERN_LENGTH == 256
#define EMPTY {                                                         \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
#elif MAX_PATTERN_LENGTH == 128
#define EMPTY {                                                         \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
#elif MAX_PATTERN_LENGTH == 64
#define EMPTY {                                                         \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
#elif MAX_PATTERN_LENGTH == 32
#define EMPTY {                                                         \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
#elif MAX_PATTERN_LENGTH == 16
#define EMPTY { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
#elif MAX_PATTERN_LENGTH == 8
#define EMPTY { 0, 0, 0, 0, 0, 0, 0, 0 }
#endif


/// Now finally we get to the part that you will code.  This the SEQUENCE.  It is a list
/// of PATTERNS.  Each PATTERN has an INTERATIONS indicator, followed by for TRACKS.  Each TRACK
/// is an array of ones and zeros, representing the BEATS.  The PATTERNS must be the the number 
/// you had said before or Bad Things will Happen.  Furthermore, Strange Things will happen if
/// you don't set each pattern length to the actual number of beats used.  Don't do that.
///

PROGMEM const struct pattern seq[PATTERNS] =      // don't fool with this line
    {                                               // nor this line

// In this example we have TWO PATTERNS.   

// Here is the first PATTERN.  
// Note it starts with a { and ends with a } and then a comma.
// A pattern has a LENGTH (with a comma)
// A pattern then has an ITERATIONS (with a comma),
// followed by four TRACKS (each ending with a comma)
        {
        32,           // THE LENGTH.   Set this to 1..255 steps, or to 0 for 256 steps
        3,            // THE ITERATIONS.  This must be >= 1, or FOREVER (0) to loop forever until the musician ADVANCES
// Here are the tracks.  Each track starts with a {, then has LENGTH 1 or 0 values, then ends with a } and a comma
        { 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
        },


// Here is the second PATTERN.  Our second, third, and fourth tracks
// are empty (no triggers), so instead of making you type lots of zeros,
// you can instead substitue the constant EMPTY, which is an empty track.
// Either way, it doesn't matter.
// [If you have for some reason changed MAX_PATTERN_LENGTH to something other than 256, 128, 64, 32, 16, or 8,
// EMPTY won't be defined.  So don't do that. ]
        {
        16,     // THE LENGTH.  Set this to 1..255 steps, or to 0 for 256 steps
        1,      // THE ITERATIONS.  This must be >= 1, or FOREVER (0) to loop forever until the musician ADVANCES
// Here are the tracks.  Each track starts with a {, then has LENGTH 1 or 0 values, then ends with a } and a comma
// But here we have three EMPTY tracks (note the commas)
        { 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1 },
        EMPTY,     
        EMPTY,
        EMPTY,
        },


    };                                            // don't fool with this line


///// And we're done!  A hint: if you're not LOOPing, you might include a final one-iteration sequence that has a single
///// first beat, to provide a final beat to the song.





////////// PINOUTS

#define CV_POT_IN1    A2    // Clock, CV for Randomness
#define CV_POT_IN2    A1    // Reset / Advance
#define CV_POT3       A0    // Swing
#define CV_IN3        A3    // Drum 3
#define CV_AUDIO_IN   A4    // Drum 4
#define CV_AUDIO_OUT  11    // Drum 1
#define CV_GATE_OUT   8     // Drum 2

#define RANDOM_PIN              A5

#define ADVANCE_TRIGGER 64      // How high must IN 2 be to trigger an advance?
#define RESET_TRIGGER 800       // How high must IN 2 be to trigger a reset?

#define CLOCK_TRIGGER 64        // How high must IN 1 be to trigger a clock?  This has to be low so we can have a large sweep of the pot for randomness.  But we might trigger early?
#define CLOCK_COUNT 20          // How long must we wait after receiving the clock to do a pulse (so we have an accurate measure for randomness) 
#define TRIGGER_OFF_COUNT 100   // How long do we wait after triggering before we set the pulses OFF?  It looks like Kick requires about 100 when the pitch is high and the decay is long.  :-(
#define MAX_RANDOMNESS 105      // 11025

struct pattern sequence;        // local sequence in memory

void loadSequence(uint16_t s)
    {
    memcpy_P(&sequence, &seq[s], 128 * 4 + 16);
    }


uint16_t patternPos;      // Which pattern are we at?  If >= PATTERNS, we are stopped
uint8_t patternCount;     // How many times have we repeated the pattern?
uint8_t step;             // Which step in the pattern are we at?
uint32_t counter;         // A counter starting at the last pulse
uint32_t pulseInterval;   // Our estimate for how long a pulse is
uint32_t swingInterval;   // Our estimate for how long a swing is (<= pulseInterval)
uint8_t swung;            // Have we swung this pulse?  If so, don't pulse again
uint8_t even;             // Is the beat an even beat?  To determine if we should swing
uint8_t doAdvance;        // Did the user advance?
uint16_t randomness;      // Degree of randomness in beats
int16_t triggerOff;       // Countdown timer to turn off triggers

void reset()
    {
    swung = 0;
    counter = 0;
    swung = false;
    even = true;
    doAdvance = false;
    patternPos = 0;
    patternCount = 0;
    step = 0;
    randomness = 0;
    triggerOff = -1;
    loadSequence(0);   
    }

void setup()
    {
    randomSeed(analogRead(RANDOM_PIN));
    pinMode(CV_AUDIO_OUT, OUTPUT);
    pinMode(CV_AUDIO_IN, OUTPUT);
    pinMode(CV_GATE_OUT, OUTPUT);
    pinMode(CV_IN3, OUTPUT);
    reset();
//Serial.begin(115200);
    }

void advance()
    {
    doAdvance = true;
    }

void pulse()    // randomness goes 0...11025 (0 least random)
    {
    if (patternPos >= PATTERNS) return;

    pulseInterval = counter;
    counter = 0;
    swingInterval = (analogRead(CV_POT3) * pulseInterval) >> 10;
    if (step >= (sequence.length == 0 ? 256 : sequence.length))
        {
        step = 0;
        patternCount++;
        if (doAdvance || 
            (patternCount >= sequence.iterations && sequence.iterations != FOREVER))
            {
            patternCount = 0;
            doAdvance = false;
            patternPos++;
            if (patternPos >= PATTERNS)
                {
                if (LOOP > 0)
                    {
                    patternPos = 0;
                    }
                else return;
                }
            loadSequence(patternPos);   
            }
        }

// play!
    uint8_t s = sequence.track1[step];
    if (random(MAX_RANDOMNESS) < randomness)
        {
        if (ADDS)
            {
            s = !s;
            }
        else
            {
            s = 0;
            }
        }

    if (s) { digitalWrite(CV_AUDIO_OUT, 1); }
    if (s) digitalWrite(CV_AUDIO_OUT, 1);

    s = sequence.track2[step];
    if (random(MAX_RANDOMNESS) < randomness)
        {
        if (ADDS)
            {
            s = !s;
            }
        else
            {
            s = 0;
            }
        }

    if (s) digitalWrite(CV_GATE_OUT, 1);

    s = sequence.track3[step];
    if (random(MAX_RANDOMNESS) < randomness)
        {
        if (ADDS)
            {
            s = !s;
            }
        else
            {
            s = 0;
            }
        }

    if (s) digitalWrite(CV_IN3, 1);

    s = sequence.track4[step];
    if (random(MAX_RANDOMNESS) < randomness)
        {
        if (ADDS)
            {
            s = !s;
            }
        else
            {
            s = 0;
            }
        }

    if (s) digitalWrite(CV_AUDIO_IN, 1);

    step++;
    triggerOff = TRIGGER_OFF_COUNT;
    }

// Range of 0...4110   
uint16_t div15(uint16_t dividend)
    {
    uint32_t invDivisor = 0x1110;
    uint16_t div = (uint16_t) ((invDivisor * (dividend + 1)) >> 16);
    return div;
    }

// Returns a value from 0 (least random) to 11025 (most random / inverted)
uint16_t computeRandomness(uint16_t input)
    {
// we square the randomness to make the least random parts more sensitive
    if (input >= 896) return 0;
    else if (input < 64)  return 105; // * 105;
    uint16_t sq = 105 - ((input - 56) >> 3);
    return sq;  //* sq;
    }

int8_t countdown = 0;
uint8_t clock = 0;
uint8_t advanced = false;

void loop()
    {
    triggerOff--;
    if (triggerOff == 0)
        {
        digitalWrite(CV_AUDIO_OUT, 0);
        digitalWrite(CV_GATE_OUT, 0);
        digitalWrite(CV_IN3, 0);
        digitalWrite(CV_AUDIO_IN, 0);
        }
    else if (triggerOff < 0) 
        {
        triggerOff = -1;
        }

    counter++;    // this could wrap around if we're not pulsing....
    uint16_t a = analogRead(CV_POT_IN1);
    uint16_t b = analogRead(CV_POT_IN2);
    if (b >= RESET_TRIGGER)
        {
        reset();
        advanced = true;
        return;
        }
    else if (b >= ADVANCE_TRIGGER && !advanced)
        {
        advanced = true;
        advance();
        }
    else if (b < ADVANCE_TRIGGER / 2)   // we dropped far enough
        {
        advanced = false;
        }

// If the counter is up, we haven't swung yet, and it's even, we need to pulse now
    if (counter > swingInterval && even && !swung)
        {
        pulse();    // swing pulse
        swung = true;
        } 

// Compute randomness only N steps after CLOCK_TRIGGER (which is just at 64 -- it needs to be like 20 steps)
    if (a < CLOCK_TRIGGER)
        {
        countdown = -2;
        }
    else if (countdown == -2)            // start the trigger clock
        {
        countdown = CLOCK_COUNT;
        }
    else
        {
        if (countdown > -1)
            {
            countdown--;
            if (countdown == 0)
                {
                randomness = computeRandomness(a);
                }
            }
        }

// compute trigger for pulse.  This is only 64, so we might trigger early -- it's maybe another 20 steps before we have settled down.  
// But if randomness is low, we're triggering right on time.  So dunno what to do.
    if (a < CLOCK_TRIGGER)
        {
        clock = 0;
        }
    else if (!clock)            // start the trigger clock
        {
        clock = 1;
        if (!even || !swung)        // do we pulse?  We do if we're ODD or if it's even but we haven't swung yet
            {
            pulse();    // non-swing pulse
            }
        swung = false;         // maybe we'll swing next time
        counter = 0;            // reset counter, we're done
        even = !even;
        }
    }
