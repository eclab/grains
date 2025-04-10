// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// DIVVY
///
/// Divvy is a two-track clock divider with pulsewidth.  Divvy is meant to 
/// run on the AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// Divvy takes a clock input and produces a division of it.
///
/// SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)
///
/// CONFIGURATION
///
/// IN 1            [Unused]
/// IN 2            [Unused]
/// IN 3            Clock
/// AUDIO IN (A)    Reset
/// AUDIO OUT       Clock Division 1
/// DIGITAL OUT (D) Clock Division 2
///
/// POT 1           Choice of Clock Division 1   [Set the switch to MAN]
///                 [changing this value triggers a reset]
///
/// POT 2           Choice of Clock Division 2  [Set the switch to MAN]
///                 [changing this value triggers a reset]
///
/// POT 3           Pulse Width (rounded down to nearest clock.  If at 0, a trigger pulse is sent)
///
/// CLOCK DIVISION CHOICES
///
/// At present the options are (left to right):
///
/// 96 clocks per pulse                   (MIDI Clock bars)
/// 64 clocks per pulse
/// 32 clocks per pulse
/// 24 clocks per pulse                   (MIDI Clock quarter notes)
/// 16 clocks per pulse
/// 8 clocks per pulse
/// 6 clocks per pulse                    (MIDI Clock 16th notes)
/// 4 clocks per pulse offset by 2    (Binary 4 digit)
/// 4 clocks per pulse
/// 3 clocks per pulse
/// 2 clocks per pulse offset by 1    (Binary 2 digit)  (1 digit is the main clock pulse)
/// 2 clocks per pulse
///
/// You could change these any way you liked right here:

#define NUM_DIVISION_OPTIONS 12

uint8_t clockPulses[NUM_DIVISION_OPTIONS] =  { 2, 2, 3, 4, 4, 6, 8, 16, 24, 32, 64, 96 };
uint8_t clockOffsets[NUM_DIVISION_OPTIONS] = { 0, 1, 0, 0, 2, 0, 0,  0,  0,  0,  0,  0 };

/// USING DRUMKIT010
///
/// Be sure to set DrumKit1010's TRIG MODE switch to "D" (Digital), so it properly 
/// responds to triggers.
///
///
/// THANKS TO SlowScape (https://www.youtube.com/c/slowscape) who prompted me to put this together.





////////// PINOUTS

#define CV_POT_IN1    A2    // Choice of Clock Division 1
#define CV_POT_IN2    A1    // Choice of Clock Division 1
#define CV_POT3       A0    // Pulsewidth
#define CV_IN3        A3    // Clock
#define CV_AUDIO_IN   A4    // Reset
#define CV_AUDIO_OUT  11    // Division 1
#define CV_GATE_OUT   8     // Division 2

#define MINIMUM_RESET 800   // How high the trigger should be to trigger a reset
#define MINIMUM_CLOCK 800   // How high the trigger should be to trigger an incoming clock pulse
#define PULSEWIDTH_COUNTDOWN 100
#define OPTION_WAIT 16

uint32_t counter;         // A counter starting at the last pulse
uint8_t clock = false;    // Did we clock?  So we don't clock again

uint8_t option1 = 0;    // which option are we doing?
uint8_t option2 = 0;
int8_t option1Counter = -1;
int8_t option2Counter = -1;
uint16_t counter1 = 0;
uint16_t counter2 = 0;
int16_t offCounter1 = 0;
int16_t offCounter2 = 0;
uint16_t pulsewidth = 0;
int8_t pulsewidthCounter = -1;

void reset()
    {
    counter1 = clockOffsets[option1];
    counter2 = clockOffsets[option2];
    clock = false;
    digitalWrite(CV_AUDIO_OUT, 0);
    digitalWrite(CV_GATE_OUT, 0);
    }

void setup()
    {
    pinMode(CV_AUDIO_OUT, OUTPUT);
    pinMode(CV_GATE_OUT, OUTPUT);
    reset();
    //Serial.begin(115200);
    }



uint16_t getPulseWidth(uint8_t which, uint16_t pulsewidth)
    {
    if (which == 0)
        return (uint16_t) ((pulsewidth * (uint32_t)clockPulses[option1]) >> 10);
    else
        return (uint16_t) ((pulsewidth * (uint32_t)clockPulses[option2]) >> 10);
    }

void pulse1()
    {
    digitalWrite(CV_AUDIO_OUT, 1);
    counter1 = clockPulses[option1] - 1;            // no offset now
    if (getPulseWidth(0, pulsewidth) == 0) { offCounter1 = PULSEWIDTH_COUNTDOWN; }
    else offCounter1 = -1;
    }
    
void pulse2()
    {
    digitalWrite(CV_GATE_OUT, 1);
    counter2 = clockPulses[option2] - 1;            // no offset now
    if (getPulseWidth(1, pulsewidth) == 0) offCounter2 = PULSEWIDTH_COUNTDOWN;
    else offCounter2 = -1;
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

    // Do we need to turn off pulses?
    if (offCounter1 == 0) digitalWrite(CV_AUDIO_OUT, 0);
    if (offCounter1 >= 0) offCounter1--;
    if (offCounter2 == 0) digitalWrite(CV_GATE_OUT, 0);
    if (offCounter2 >= 0) offCounter2--;
    uint16_t pw = getPulseWidth(0, pulsewidth);
    if (pw && counter1 + 1 == (clockPulses[option1] - pw)) digitalWrite(CV_AUDIO_OUT, 0);
    pw = getPulseWidth(1, pulsewidth);
    if (pw && counter2 + 1 == (clockPulses[option2] - pw)) digitalWrite(CV_GATE_OUT, 0);
    
// Determine if the user changed an option

    uint8_t opt = (uint8_t)((analogRead(CV_POT_IN1) * NUM_DIVISION_OPTIONS) >> 10);
    if (opt != option1)
        {
        if (option1Counter < 0) option1Counter = OPTION_WAIT;
        else if (option1Counter == 0) { option1 = opt; reset(); option1Counter = -1; }
        else option1Counter--;
        }
    else option1Counter = -1;
    opt = (uint8_t)((analogRead(CV_POT_IN2) * NUM_DIVISION_OPTIONS) >> 10);
    if (opt != option2)
        {
        if (option2Counter < 0) option2Counter = OPTION_WAIT;
        else if (option2Counter == 0) { option2 = opt; reset(); option2Counter = -1; }
        else option2Counter--;
        }
    else option2Counter = -1;
    pulsewidth = analogRead(CV_POT3);

// Determine if we received a clock
    uint16_t a = analogRead(CV_IN3);
    if (a < MINIMUM_CLOCK)
        {
        clock = false;
        }
    else if (!clock)
        {
        clock = true;
    
        if (counter1 == 0) { pulse1();}
        else if (counter1 >= 0) counter1--;
        if (counter2 == 0) pulse2();
        else if (counter2 >= 0) counter2--;
        }
    }
