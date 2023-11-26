// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// PATTERN
///
/// Pattern produces various 4-output digital patterns based on an analog value.   
/// Pattern is meant to run on the AE Modular GRAINS, but it could be adapted to 
/// any Arduino.
///
/// SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)
///
/// Pattern takes an analog input and outputs four digital outputs which are high
/// or low depending on the value of the input.  It does not output clocked triggers:
/// perhaps we might do something like that in another program.
///
/// CONFIGURATION
///
/// IN 1                Input CV                Clock/Advance
/// IN 2            [Unused]
/// IN 3            Output 3
/// AUDIO IN (A)    Output 4
/// AUDIO OUT       Output 1
/// DIGITAL OUT (D) Output 2
///
/// POT 1           [Unused, Keep High]
///
/// POT 2           Style + Zero?
///
/// POT 3           Complexity and Gate/Trigger. 
///                 Left to right: Complexity 1, 2, 3 as gates, 
///                 then Complexity 1, 2, 3 as triggers
///
///
/// WHAT IS A PATTERN?
///
/// A pattern is a sequence of 4-tuples.  For example, 0100, 1100, 1111, 0010, 0101 is a pattern.
/// A 4-tuple contains four ones or zeros, one for each output.  For each 4-tuple, the outputs 
/// in the order 4321.  For example, 0101 means that outputs 1 and 3 are ON and outputs 2 and 4 are OFF.
///
/// At any particular time, PATTERNS is outputting ONE 4-tuple in a pattern.  As you change
/// the input value, PATTERNS outputs the pattern in the sequence corresponding to the position
/// of the input.  So if, for example, you turned the knob fully from one end to the other, PATTERNS
/// would sweep through all the 4-tuples in the sequence in order.
///
/// 
/// GATES AND TRIGGERS
///
/// PATTERNS can output 4-tuples as GATES or as TRIGGERS.  As GATES, it's simply turning on 
/// the outputs which have 1s in the 4-tuple, and turning off the outputs that have 0s.  As TRIGGERS,
/// it's pulsing a trigger for the outputs which have 1s in the 4-tuple.  Triggers are pulsed
/// as soon as the 4-tuple is selected by your input.
/// 
///
/// STYLES AND COMPLEXITY
///
/// PATTERNS has 21 patterns to choose from.  Patterns come in 7 STYLES, and within each style,
/// there are three patterns of varying COMPLEXITY.
///
/// There are SEVEN styles:
///
/// MOVEMENT: a Single 1 passes from start to finish
/// MOVEMENT WITH ZERO: Same as MOVEMENT, but at the beginning, nothing is on at all
/// MOVEMENT WITH TWO ZEROS: Same as MOVEMENT, but at the beginning AND the end, nothing is on at all
/// WIDE MOVEMENT: Same as MOVEMENT WITH TWO ZEROS, but TWO, THREE, or FOUR ones pass from start to finish
/// ALTERNATIVE MOVEMENT: Various alternative styles
/// BAR GRAPH: Gradually more and more outputs output a 1
/// BINARY: The outputs follow the pattern of binary numerals
///
/// Each style has THREE levels of complexity.  In many cases the complexity indicates how many
/// outputs are involved in the pattern (2, 3, or 4).  For WIDE MOVEMENT WITH TWO ZEROS and for
/// ALTERNATIVE MOVEMENT the complexity indicates other things.
///
/// Here are the seven styles and three complexity levels, and the resulting patterns.
///
///
/// DELETE
///     MOVEMENT
///             2               0001 0010
///             3               0001 0010 0100
///             4               0001 0010 0100 1000
///     MOVEMENT WITH ZERO
///             2               0000 0001 0010
///             3               0000 0001 0010 0100
///             4               0000 0001 0010 0100 1000
/// DELETE
///     MOVEMENT WITH TWO ZEROS
///             2               0000 0001 0010 0000
///             3               0000 0001 0010 0100 0000
///             4               0000 0001 0010 0100 1000 0000
///     WIDE MOVEMENT
///             2               0000 0001 0011 0110 1100 1000 0000
///             3               0000 0001 0011 0111 1110 1100 1000 0000
///             4               0000 0001 0011 0111 1111 1110 1100 1000 0000
///     ALTERNATIVE MOVEMENT
///             Triple  0011 0110 1100
///             Jump    0011 1111 1100
///             Aliased 0000 0001 0011 0010 0110 0100 1100 1000 0000
///     BAR GRAPH       
///             2               0000 0001 0011
///             3               0000 0001 0011 0111
///             4               0000 0001 0011 0111 1111
///     BINARY
///             2               0000 0001 0010 0011
///             3               0000 0001 0010 0011 0100 0101 0110 0111
///             4               0000 0001 0010 0011 0100 0101 0110 0111 1000 1001 1010 1011 1100 1101 1110 1111
///
///
/// USING DRUMKIT010
///
/// Be sure to set DrumKit1010's TRIG MODE switch to "D" (Digital), so it properly 
/// responds to triggers.
///
/// GRAINS BUG
/// There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the switch to "Man", 
/// then the range of the Pot is correct.  But if you set the switch to "In 1" (or "In 2"), 
/// then the range of the Dial is maximum (1.0 probability for our purposes) at about the 
/// 2 o'clock position on the Dial.  Beyond that it stays at 1.0.




#define NUM_STYLES 7
#define NUM_COMPLEXITIES 3
#define MAXIMUM_LENGTH 16
#define PATTERN_LENGTH 4

uint8_t patternLengths[NUM_STYLES][NUM_COMPLEXITIES] = 
    {
    {2, 3, 4},
    {3, 4, 5}, 
    {4, 5, 6},
    {7, 8, 9},
    {3, 3, 9},
    {3, 4, 5},
    {4, 8, 16}
    };
        

uint8_t patterns[NUM_STYLES][NUM_COMPLEXITIES][MAXIMUM_LENGTH][PATTERN_LENGTH] = 
    {
    // MOVEMENT
        {
        {{0, 0, 0, 1}, {0, 0, 1, 0}},
        {{0, 0, 0, 1}, {0, 0, 1, 0}, {0, 1, 0, 0}},
        {{0, 0, 0, 1}, {0, 0, 1, 0}, {0, 1, 0, 0}, {1, 0, 0, 0}},
        },
    // MOVEMENT WITH ZERO
        {
        {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 0} },
        {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 0}, {0, 1, 0, 0} },
        {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 0}, {0, 1, 0, 0}, {1, 0, 0, 0} },
        },
    // MOVEMENT WITH TWO ZEROS
        {
        {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 0}, {0, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
        },
    // WIDE MOVEMENT WITH TWO ZEROS
        {
        {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 1}, {0, 1, 1, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 1}, {0, 1, 1, 1}, {1, 1, 1, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 1}, {0, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
        },
    // ALTERNATIVE MOVEMENT
        {
        {{0, 0, 1, 1}, {0, 1, 1, 0}, {1, 1, 0, 0}},
        {{0, 0, 1, 1}, {1, 1, 1, 1}, {1, 1, 0, 0}},
        {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 1}, {0, 1, 1, 1}, {0, 1, 1, 0}, {1, 1, 1, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
        },
    // BAR GRAPH
        {
        {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 1}},
        {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 1}, {0, 1, 1, 1}},
        {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 1}, {0, 1, 1, 1}, {1, 1, 1, 1}},
        },
    // BINARY
        {
        {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 0}, {0, 0, 1, 1}},
        {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 1, 0, 0}, {0, 1, 0, 1}, {0, 1, 1, 0}, {0, 1, 1, 1}},
        {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 1, 0, 0}, {0, 1, 0, 1}, {0, 1, 1, 0}, {0, 1, 1, 1}, {1, 0, 0, 0}, {1, 0, 0, 1}, {1, 0, 1, 0}, {1, 0, 1, 1}, {1, 1, 0, 0}, {1, 1, 0, 1}, {1, 1, 1, 0}, {1, 1, 1, 1}},
        }
    };      
        

////////// PINOUTS

#define CV_POT_IN1    A2    // Input
#define CV_POT_IN2    A1    // Style
#define CV_POT3       A0    // Complexity
#define CV_IN3        A3    // Output 3
#define CV_AUDIO_IN   A4    // Output 4
#define CV_AUDIO_OUT  11    // Output 1
#define CV_GATE_OUT   8     // Output 2

#define NUM_STYLES 7
#define NUM_COMPLEXITIES 3
#define OPTION_WAIT 16
#define TRIGGER_WIDTH 100

int8_t lastVal = -1;
int16_t input = -1;
uint8_t style = 0;
uint8_t complexity = 0;
int8_t styleCounter = -1;
int8_t complexityCounter = -1;
int8_t valueCounter = -1;
int8_t triggerCounter = -1;

#define MEDIAN_OF_THREE(a,b,c) (((a) <= (b)) ? (((b) <= (c)) ? (b) : (((a) < (c)) ? (c) : (a))) : (((a) <= (c)) ? (a) : (((b) < (c)) ? (c) : (b))))

// From https://stackoverflow.com/questions/480960/how-do-i-calculate-the-median-of-five-in-c
static double medianOfFive(double a, double b, double c, double d, double e)
    {
    return b < a ? d < c ? b < d ? a < e ? a < d ? e < d ? e : d
        : c < a ? c : a
        : e < d ? a < d ? a : d
        : c < e ? c : e
        : c < e ? b < c ? a < c ? a : c
        : e < b ? e : b
        : b < e ? a < e ? a : e
        : c < b ? c : b
        : b < c ? a < e ? a < c ? e < c ? e : c
        : d < a ? d : a
        : e < c ? a < c ? a : c
        : d < e ? d : e
        : d < e ? b < d ? a < d ? a : d
        : e < b ? e : b
        : b < e ? a < e ? a : e
        : d < b ? d : b
        : d < c ? a < d ? b < e ? b < d ? e < d ? e : d
        : c < b ? c : b
        : e < d ? b < d ? b : d
        : c < e ? c : e
        : c < e ? a < c ? b < c ? b : c
        : e < a ? e : a
        : a < e ? b < e ? b : e
        : c < a ? c : a
        : a < c ? b < e ? b < c ? e < c ? e : c
        : d < b ? d : b
        : e < c ? b < c ? b : c
        : d < e ? d : e
        : d < e ? a < d ? b < d ? b : d
        : e < a ? e : a
        : a < e ? b < e ? b : e
        : d < a ? d : a;
    }
                                
void setup()
    {
    pinMode(CV_IN3, OUTPUT);
    pinMode(CV_AUDIO_IN, OUTPUT);
    pinMode(CV_AUDIO_OUT, OUTPUT);
    pinMode(CV_GATE_OUT, OUTPUT);
    Serial.begin(115200);
    }

void sendTriggers()             // FIXME broke these out in case we want to do a clock later...
    {
    digitalWrite(CV_AUDIO_OUT, patterns[style][complexity - 3][lastVal][0]);
    digitalWrite(CV_GATE_OUT, patterns[style][complexity - 3][lastVal][1]);
    digitalWrite(CV_IN3, patterns[style][complexity - 3][lastVal][2]);
    digitalWrite(CV_AUDIO_IN, patterns[style][complexity - 3][lastVal][3]);
    triggerCounter = TRIGGER_WIDTH;
    }
        
int16_t ins[5];

void loop()
    {
    // reset triggers
    if (triggerCounter == 0)
        {
        digitalWrite(CV_AUDIO_OUT, 0);
        digitalWrite(CV_GATE_OUT, 0);
        digitalWrite(CV_IN3, 0);
        digitalWrite(CV_AUDIO_IN, 0);
        }
                
    if (triggerCounter >= 0)
        {
        triggerCounter--;
        } 

    // smoothed input
    analogRead(CV_POT_IN1);
    int16_t in0 = analogRead(CV_POT_IN1);
    int16_t in1 = analogRead(CV_POT_IN1);
    int16_t in2 = analogRead(CV_POT_IN1);
    int16_t in3 = analogRead(CV_POT_IN1);
    int16_t in = (in0 + in1 + in2 + in3) >> 2;
    
    ins[0] = ins[1];
    ins[1] = ins[2];
    ins[2] = ins[3];
    ins[3] = ins[4];
    ins[4] = in;

    //in = MEDIAN_OF_THREE(ins[2],ins[3],ins[4]);
    if (abs(in - input) > 32) 
        //{Serial.print(in); Serial.print(" -> " ); Serial.println(input); 
        input = in; 
    //}

    uint8_t opt = (uint8_t)((analogRead(CV_POT_IN2) * NUM_STYLES) >> 10);
    if (opt != style)
        {
        if (styleCounter < 0) styleCounter = OPTION_WAIT;
        else if (styleCounter == 0) { style = opt; styleCounter = -1; }
        else styleCounter--;
        }
    else styleCounter = -1;
    
    opt = (uint8_t)((analogRead(CV_POT3) * NUM_COMPLEXITIES * 2) >> 10);
    if (opt != complexity)
        {
        if (complexityCounter < 0) complexityCounter = OPTION_WAIT;
        else if (complexityCounter == 0) { complexity = opt; complexityCounter = -1; }
        else complexityCounter--;
        }
    else complexityCounter = -1;
    
    uint8_t val = (uint8_t)((input * patternLengths[style][complexity > 2 ? complexity - 3 : complexity]) >> 10);
    if (lastVal != val)
        {
        Serial.println(val);
        lastVal = val;
        if (complexity >= 3) // trigger
            {
            sendTriggers();
            }
        else
            {
            digitalWrite(CV_AUDIO_OUT, patterns[style][complexity][val][0]);
            digitalWrite(CV_GATE_OUT, patterns[style][complexity][val][1]);
            digitalWrite(CV_IN3, patterns[style][complexity][val][2]);
            digitalWrite(CV_AUDIO_IN, patterns[style][complexity][val][3]);
            }
        }
    }
    
