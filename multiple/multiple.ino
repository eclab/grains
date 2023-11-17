// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// MULTIPLE
///
/// Multiple is a two-track step multiplier.  Multiple is meant to 
/// run on the AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// Multiple takes a clock input and attempts to produce a multiple of it.  It
/// won't be able to properly generate multiple beats until two successive initial beats when
/// you reset (for obvious reasons). 
///
/// SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)
///
/// CONFIGURATION
///
/// IN 1            [Unused]
/// IN 2            [Unused]
/// IN 3            Clock
/// AUDIO IN (A)    Reset
/// AUDIO OUT       Clock Multiple 1
/// DIGITAL OUT (D) Clock Multiple 2
///
/// POT 1           Choice of Clock Multiple 1   [Set the switch to MAN]
///
/// POT 2           Choice of Clock Multiple 2  [Set the switch to MAN]
///
/// POT 3           Pulse Width
///
/// CLOCK MULTIPLE CHOICES
///
/// At present the options are (left to right):
///
/// 16 pulses per clock in
/// 7 pulses per clock in
/// 6 pulses per clock in
/// 5 pulses per clock in
/// 4 pulses per clock in
/// 3 pulses per clock in
/// "Swing": 3 pulses per 2 clock ins, middle pulse omitted
/// 2 pulses per clock in
///
///
/// WEAKNESSES
///
/// Currently after you reset, Multiple will always wait a beat so it can measure the beat length
/// in order to get the right sub-beat.  This could be improved: resets do not change the
/// beat length per se.  I'll work on it.
///
/// If you change the speed, Multiple will not catch up until a beat or two later.  That could
/// be improved as well.
/// 
/// Multiple was meant to be able to do polyrhythms on the beat, but at present bugs are preventing
/// this from happening.
///
/// If you change the pot to a value "in between" two options, Multiple will get confused and may
/// not pulse at all.  Move the pot until Multiple is locked on.

////////// PINOUTS

#define CV_POT_IN1    A2    // Choice of Clock Multiple 1
#define CV_POT_IN2    A1    // Choice of Clock Multiple 1
#define CV_POT3       A0    // Pulsewidth
#define CV_IN3        A3    // Clock
#define CV_AUDIO_IN   A4    // Reset
#define CV_AUDIO_OUT  11    // Multiple 1
#define CV_GATE_OUT   8     // Multiple 2

#define SWING_2   6         // Position of the special SWING 2 option in the list
#define MINIMUM_RESET 800   // How high the trigger should be to trigger a reset
#define MINIMUM_CLOCK 800   // How high the trigger should be to trigger an incoming clock pulse
#define BIG_NUMBER 100    // Totals are set to this to guarantee that they are "finished"

uint32_t counter;         // A counter starting at the last pulse
uint32_t pulseInterval;   // Our estimate for how long a pulse is
uint8_t clock = false;    // Did we clock?  So we don't clock again

uint8_t option1 = 0;    // which option are we doing?
uint8_t option2 = 0;
uint8_t total1 = BIG_NUMBER;    // How many have we done?
uint8_t total2 = BIG_NUMBER;
uint32_t counter1 = 0;     // How many ticks have transpired since our initiating clock
uint32_t counter2 = 0;
uint32_t jump1 = 0;      // What is the interval?
uint32_t jump2 = 0;
uint8_t max1 = 0;
uint8_t max2 = 0;
uint8_t state1 = 0;
uint8_t state2 = 0;
uint16_t pulsewidth;
int16_t last_option1 = -1;
int16_t last_option2 = -1;

void reset()
    {
    counter = 0;
    pulseInterval = 0;
    counter1 = 0;
    counter2 = 0;
    total1 = BIG_NUMBER;
    total2 = BIG_NUMBER;
    jump1 = 0;
    jump2 = 0;
    clock = false;
    digitalWrite(CV_AUDIO_OUT, 0);
    digitalWrite(CV_GATE_OUT, 0);
    }

void setup()
    {
    // pinMode(CV_AUDIO_OUT, OUTPUT);
    // pinMode(CV_GATE_OUT, OUTPUT);
    reset();
    Serial.begin(115200);
    }


void pulse1()
    {
    digitalWrite(CV_AUDIO_OUT, 1);
    state1 = 1;
    }

void pulse2()
    {
    digitalWrite(CV_GATE_OUT, 1);
    state2 = 1;
    }

// Range of 0...32773
uint16_t div7(uint16_t dividend)
    {
    uint32_t invDivisor = 0x2492;
    uint16_t div = (uint16_t) ((invDivisor * (dividend + 1)) >> 16);
    return div;
    }

// Range of 0...65535   
uint16_t div5(uint16_t dividend)
    {
    uint32_t invDivisor = 0x3333;
    uint16_t div = (uint16_t) ((invDivisor * (dividend + 1)) >> 16);
    return div;
    }

// Range of 0...65535
uint16_t div3(uint16_t dividend)
    {
    uint32_t invDivisor = 0x5555;
    uint16_t div = (uint16_t) ((invDivisor * (dividend + 1)) >> 16);
    return div;
    }

uint16_t computeJump(uint8_t out)
    {
    uint8_t option = (out == 0 ? option1 : option2);
    switch(option)
        {
        case 0: // 16
            if (out == 0)  max1 = 16; else  max2 = 16;
            return pulseInterval >> 4;
            break;
//case 1: // 12
//  if (out == 0)  max1 = 12; else  max2 = 12;
//  return div3(pulseInterval >> 2);   // div12 doesn't have a large enough range
//break;
//case 2: // 9
//  if (out == 0)  max1 = 9; else  max2 = 9;
//  return div3(div3(pulseInterval));   // div9 doesn't have a large enough range
//break;
//case 3: // 8
//   if (out == 0)  max1 = 8; else  max2 = 8;
// return pulseInterval >> 3;
//break;
        case 1: // 7
            if (out == 0)  max1 = 7; else  max2 = 7;
            return div7(pulseInterval);
            break;
        case 2: // 6
            if (out == 0)  max1 = 6; else  max2 = 6;
            return div3(pulseInterval >> 1);    // div6 doesn't have a large enough range
            break;
        case 3: // 5
            if (out == 0)  max1 = 5; else  max2 = 5;
            return div5(pulseInterval);
            break;
        case 4: // 4
            if (out == 0)  max1 = 4; else  max2 = 4;
            return pulseInterval >> 2;
            break;
        case 5: // 3
            if (out == 0)  max1 = 3; else  max2 = 3;
            return div3(pulseInterval);
            break;
        case 6: // 2 Swing
            if (out == 0)  max1 = 3; else  max2 = 3;
            return div3(pulseInterval);
            break;
        case 7: // 2
            if (out == 0)  max1 = 2; else  max2 = 2;
            return pulseInterval >> 1;
            break;
//case 11: // 3/2
//   if (out == 0)  max1 = 3; else  max2 = 3;
// return (pulseInterval * 3) >> 2;
//break;
//case 12: // 4/3
//   if (out == 0)  max1 = 4; else  max2 = 4;
// return div3(pulseInterval * 4);
//break;
//case 13: // 5/4
//   if (out == 0)  max1 = 5; else  max2 = 5;
// return (pulseInterval * 5) >> 2;
//break;
//case 14: // 3/4
//  if (out == 0)  max1 = 3; else  max2 = 3;
//  return div3(pulseInterval * 4);
//break;
//case 15: // 2/3
//   if (out == 0)  max1 = 2; else  max2 = 2;
// return (pulseInterval * 3) >> 1;
//break;
        }
    }

void loop()
    {

// Determine if we need to reset

    analogRead(CV_AUDIO_IN);    // throwaway because it has high impedance :-(
    uint16_t b = analogRead(CV_AUDIO_IN);
    if (b > MINIMUM_RESET)
        {
        reset();
        return;
        }

// Update our counters
    counter++;
    counter1++;
    counter2++;


// Compute pulsewidth
    pulsewidth = analogRead(CV_POT3) + 1;


// Determine if we need to turn OFF outgoing triggers

// max1 originally 3??

    if (state1 && total1 <= max1 && counter1 >= (jump1 * (total1 - 1) + ((jump1 * pulsewidth) >> 10)))
        {
        digitalWrite(CV_AUDIO_OUT, 0);
        state1 = 0;
        }

    if (state2 && total2 <= max2 && counter2 >= (jump2 * (total2 - 1) + ((jump2 * pulsewidth) >> 10)))
        {
        digitalWrite(CV_GATE_OUT, 0);
        state2 = 0;
        }


// Determine if the user changed an option

    uint8_t opt = analogRead(CV_POT_IN1) >> 7;
    if (opt != option1) { option1 = opt; total1 = BIG_NUMBER; }   // reset
    opt = analogRead(CV_POT_IN2) >> 7;
    if (opt != option2) { option2 = opt; total2 = BIG_NUMBER; }   // reset

// Determine if we received a clock

    uint16_t a = analogRead(CV_IN3);
    if (a < MINIMUM_CLOCK)
        {
        clock = false;
        }
    else if (!clock)
        {
// update the pulse interval and lock the clock
        pulseInterval = counter;
        counter = 0;
        clock = true;

// If either of our outgoing triggers is done with their sequence,
// pulse them the first time in sync with the clock and get them going

        if (total1 >= max1)
            {
            jump1 = computeJump(0);
            pulse1();
            total1 = 1;
            counter1 = 0;
            }
        if (total2 >= max2)
            {
            jump2 = computeJump(1);
            pulse2();
            total2 = 1;
            counter2 = 0;
            }
        }

// Determine if we need to do a pulse

    if (total1 < max1 && counter1 >= jump1 * total1) // time to jump!
        {
        if (option1 != SWING_2 || total1 != 1) pulse1();
        total1++;
        }

    if (total2 < max2 && counter2 >= jump2 * total2) // time to jump!
        {
        if (option2 != SWING_2 || total2 != 1) pulse2();
        total2++;
        }
    }
