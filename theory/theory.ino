// Copyright 2023 Sean Luke 
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License



/// THEORY
///
/// Theory is meant to run on the AE Modular GRAINS, but it could be adapted
/// to any Arduino.  Theory was written by Sean Luke [sean@cs.gmu.edu]
///
/// SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)
///
/// Theory is allows you to output digital outputs based on the mathematical expressions
/// of various real-valued inputs.  The basic configuration has three binary outputs 
/// called A, B, and C, which are computed based on real-valued inputs called
/// W, X, Y, and Z.  The four inputs range from 0.0 at 0V to 1.0 at 5V.  You code
/// the expressions below.  You can make any expression you like, and use any
/// floating-point operation avaiable to the Arduino.  Note that some operations,
/// like sin and pow, are SLOW.  We might speed it up a bit with a provided function
/// like here:  https://forum.arduino.cc/t/a-faster-sin-lookup-function/69173/6
///
/// FIVE-INPUT OPTION.  Instead of three expressions of four inputs, you can also set 
/// up Theory to have two expressions (A and B) of five inputs (W, X, Y, Z, and C,
/// now repurposed as real-valued input).
///
/// SIX-INPUT OPTION.  You can also do a single expression (A) of six real-valued inputs
/// (W, X, Y, Z, C) plus one input (B) which is digital (0 or 1).
///
///
/// CONFIGURATION
///
/// IN 1            CV for X
/// IN 2            CV for Y
/// IN 3            C
/// AUDIO IN (A)    W
/// AUDIO OUT       A
/// DIGITAL OUT (D) B
///
/// POT 1           X
/// POT 2           Y
/// POT 3           Z
///
///
/// NOTE. I have not tested Audio In (A) as a valid input -- W in our case.  It should
/// be fine, as we're doing slow stuff, but it does get put through a low-pass
/// audio-rate filter.  Would appreciate bug reports.
///
///
/// GRAINS BUG
/// 
/// There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the 
/// switch to "Man", then the range of the Pot is correct.  But if you set the switch 
/// to "In 1" (or "In 2"), then the range of the Dial is maximum (1.0 or 5V for 
/// our purposes) at about the 2 o'clock position on the Dial.  Beyond that it stays at 1.0
///
///
/// STANDARD CONFIGURATION

/// You define the three expressions by replacing these three lines:

#define A(w,x,y,z)                                                      \
    x + y + z < z             /// <--- Replace this line as an expression of w, x, y, and/or z

#define B(w,x,y,z,A)                                                    \
    abs(x - z) < y            /// <--- Replace this line as an expression of w, x, y, z, and/or A, where A is the [0=false or 1=true] output of A(...)

#define C(w,x,y,z,A,B)                                                  \
    x * y < z                 /// <--- Replace this line as an expression of w, x, y, z, and/or A and B, where A and B are the [0=false or 1=true] output of A(...) and B(...)


/// 5-INPUT ALTERNATIVE CONFIGURATION
/// Alternatively, if you uncomment these expressions, you can have two expressions
/// (outputted at A and B) on FIVE inputs (w, x, y, z, and c), where c is now an INPUT.
/// You don't need to comment out the earlier #define expressions.

// #define AA(w,x,y,z,c) \
//      w + x + y + z < c

// #define BB(w,x,y,z,c,aa) \
//      w * x * y * z * aa < c


/// 6-INPUT ALTERNATIVE CONFIGURATION
/// Alternatively, if you uncomment this expression, you can have one expression
/// (outputted at A) on SIX inputs (w, x, y, z, and c, b), where c is now an INPUT,
/// and b is now a DIGITAL INPUT (0 or 1 only).
/// You don't need to comment out the earlier #define expressions.

// #define AAA(w,x,y,z,c,b) \
//      sin(w) + cos(x) + y + z < c * b


//////// USEFUL MATH FUNCTIONS AND DEFINES
#include <math.h>

/// Arduino math functions you could use include:
/// + - * / % abs(n), max(n,m) min(m,n), pow(n,m) [which is n^m], sq(n) [which is n * n], sqrt(n), cos(n), sin(n), tan(n)
///
/// You can also use the following constants:
///
/// INFINITY    positive infinity
/// M_E                 E
/// M_PI                Pi
/// HALF_PI             1/2 * Pi
/// M_PI_2              also 1/2 * Pi
/// M_PI_4              1/4 * Pi
/// M_1_PI              1/Pi
/// M_2_PI              2/Pi
/// TWO_PI              2 * Pi
/// M_SQRT2             sqrt(2)
/// M_SQRT1_2   1/sqrt(2)
/// M_2_SQRTPI  2/sqrt(pi)
/// M_LOG2E             log_2(E)
/// M_LOG10E    log_10(E)
/// M_LN2               log_e(2)
/// M_LN10              log_e(10)
/// DEG_TO_RAD  (2 * Pi) / 360
/// RAD_TO_DEG  360 / (2 * Pi)
/// NAN                 not a number -- likely not so useful for you.  You can test to see if x is NAN with (x != x).

/// Also I am providing frand(), which returns a random value between 0.0 and 1.0 not including 1.0
/// Additionally, frand1(), which returns a random value between 0.0 and 1.0 INCLUDING BOTH 0.0 and 1.0

float frand() { return random(16777216) / ((float)(1 << 24)); }

float frand1() 
    { 
    int v = random(16777217);
    if (v == 16777216) return 1.0;
    else return v / ((float)(1 << 24)); 
    }

/// Example: 
/// #define A(w,x,y,z) \
///            x + y + frand() * z
///

/// Also I am providing some macros on floating-point numbers which treat them as if 
/// they were logical values.  0 is any value < 0.5 and 1 is any value >= 0.5.
///
/// Example: 
/// #define A(w,x,y,z) \
///            OR(AND(x,y), AND(w,z + 0.3))
///

#define PUSH(x) (x < 0.5)         // you can ignore this one
#define NOT(x) (1 - PUSH(x))
#define INV(x) (NOT(x))                         // Same thing as NOT
#define AND(x,y) (PUSH(x) * PUSH(y))
#define NAND(x,y) (1 - AND(x,y))
#define OR(x,y) (max(PUSH(x), PUSH(y)))
#define NOR(x,y) (1 - OR(x,y))
#define EQL(x,y) (PUSH(x) == PUSH(y))   // "Logically" Equal
#define XOR(x,y) (1 - EQL(x,y))







////////// PINOUTS

#define CV_POT_IN1    A2    // X
#define CV_POT_IN2    A1    // Y
#define CV_POT3       A0    // Z
#define CV_IN3        A3    // C
#define CV_AUDIO_IN   A4    // W
#define CV_AUDIO_OUT  11    // A
#define CV_GATE_OUT   8     // B

#define RANDOM_PIN              A5

void setup()
    {
    randomSeed(analogRead(RANDOM_PIN));

#if defined(AAA)
    pinMode(CV_AUDIO_OUT, OUTPUT);
    pinMode(CV_GATE_OUT, INPUT);
#elif defined(AA)
    pinMode(CV_AUDIO_OUT, OUTPUT);
    pinMode(CV_GATE_OUT, OUTPUT);
#else
    pinMode(CV_AUDIO_OUT, OUTPUT);
    pinMode(CV_GATE_OUT, OUTPUT);
    pinMode(CV_IN3, OUTPUT);
#endif
//    Serial.begin(9600);
    }


void loop()
    {
// this throwaway is advisable because Analog In has high impedance,
// and so sometimes it can't charge the capacitors fast enough in the ADC.
// As a result, the PREVIOUS analogRead() may bleed into this one.
// The throwaway blocks the bleed in most cases.  We might need two throwaways.    
    analogRead(CV_AUDIO_IN);

    float w = analogRead(CV_AUDIO_IN) * 0.0009775171;   // that is, / 1023
    float x = analogRead(CV_POT_IN1) * 0.0009775171;   // that is, / 1023
    float y = analogRead(CV_POT_IN2) * 0.0009775171;   // that is, / 1023
    float z = analogRead(CV_POT3) * 0.0009775171;   // that is, / 1023

#if defined(AAA)
    float c = analogRead(CV_IN3) * 0.0009775171;   // that is, / 1023
    float b = digitalRead(CV_GATE_OUT);
    uint8_t a = AAA(w,x,y,z,c,b);

    digitalWrite(CV_AUDIO_OUT, a);
#elif defined(AA)
    float c = analogRead(CV_IN3) * 0.0009775171;   // that is, / 1023
    uint8_t a = AA(w,x,y,z,c);
    uint8_t b = BB(w,x,y,z,c,a); 

    digitalWrite(CV_AUDIO_OUT, a);
    digitalWrite(CV_GATE_OUT, b);
#else
    uint8_t a = A(wq,x,y,z);
    uint8_t b = B(w,x,y,z,a);
    uint8_t c = C(w,x,y,z,a,b);

    digitalWrite(CV_AUDIO_OUT, a);
    digitalWrite(CV_GATE_OUT, b);
    digitalWrite(CV_IN3, c);
#endif
    }





