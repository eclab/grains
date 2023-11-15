// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// PULSES
///
/// Pulses gives you various options for emitting separate pulse-wave LFOs or
/// mixtures of them. Pulses is meant to run on the AE Modular GRAINS, but
/// it could be adapted to any Arduino.  Pulses was written by Sean Luke 
/// [sean@cs.gmu.edu]
///
/// Pulses requires that you install the **FlexiTimer2** library.  A zip file
/// for this library is provided.  You can load this zip file directly using
/// the Arduino IDE's library facility, or just stick the enclosed directory
/// in your Arduino library folder.
///
/// SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)
///
/// FIRST OPTION.  You can make the LFO run FAST or SLOW.  By default it is SLOW.
/// To run FAST, uncomment (remove the // in front of) the following #define:


// #define FAST


/// SECOND OPTION.  You have three choices ffor the CONFIGURATION of the LFOs.
/// These choices are:
///
/// 1. MAIN CONFIGURATION.  You have two independent Pulse-wave LFOs, and you can
///    adjust their rates manually or via CV.  You can also adjust the pulsewidth
///    of the first LFO either manually or via CV, but not both at the same time.
///    You can reset both LFOs together.
///
/// 2. "MULTI" CONFIGURATION.  You have THREE independent Square-wave LFOs, and you can
///    adjust their rates manually.  The first two rates can also be adjusted by CV.
///    The pulsewidth is 50% for all three LFOs.  You can reset all three LFOs together.
///
/// 3. "MIX" CONFIGURATION.  The LFO is the MIXTURE of THREE independent Square-wave
///    LFOs at different rates.  It changes from 1 to 0 or back whenever any combination
///    of LFOs happen to flip.  You specify the rate of the first LFO manually or by
///    CV.  The rate of the second LFO is specified as a PERCENTAGE of the rate of the
///    first LFO.  It is specified manually or by CV.  The rate of the third LFO is
///    specified the same way, except that you can adjust it manually, or by CV, but
///    not both at the same time.  Thus your first LFO should be the SLOWEST one.  You
//     can reset all three LFOs together.  The first LFO is also output independently:
///    so if you feed this output back into the reset, you can sync the second and third
///    LFOs to the first LFO each time the first LFO cycles.
///
/// The default configuration is MAIN.  You can change to MULTI or MIX by uncommenting
/// (removing the // in front of) one of the following two #defines:

// #define MULTI
// #define MIX


/// GRAINS BUG
///
/// There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the
/// switch to "Man", then the range of the Pot is correct.  But if you set the switch
/// to "In 1" (or "In 2"), then the range of the Dial is maximum at about the 2 o'clock
/// position on the Dial.  Beyond that it stays at maximum.


/// MAIN CONFIGURATION
///
/// IN 1            Rate 1 CV
/// IN 2            Rate 2 CV
/// IN 3            Pulsewidth 1 CV.  Only has an effect if POT 3 is at FULL
/// AUDIO IN (A)    Reset
/// AUDIO OUT       LFO 1 Out
/// DIGITAL OUT (D) LFO 2 Out
///
/// POT 1           Rate 1
/// POT 2           Rate 2
/// POT 3           Pulsewidth 1.  If FULL then Pulsewidth 1 CV is used instead
///
///
/// ALTERNATIVE "MULTI" CONFIGURATION
///
/// IN 1            Rate 1 CV
/// IN 2            Rate 2 CV
/// IN 3            LFO 3 Out
/// AUDIO IN (A)    Reset
/// AUDIO OUT       LFO 1 Out
/// DIGITAL OUT (D) LFO 2 Out
///
/// POT 1           Rate 1
/// POT 2           Rate 2
/// POT 3           Rate 3
///
///
/// ALTERNATIVE "MIX" CONFIGURATION
///
/// IN 1            Rate 1 CV
/// IN 2            Rate 2 Percentage CV
/// IN 3            Rate 3 Percentage CV.  Only has an effect if POT 3 is at FULL
/// AUDIO IN (A)    Reset
/// AUDIO OUT       LFO 1 Out
/// DIGITAL OUT (D) Mixed LFO Out
///
/// POT 1           Rate 1
/// POT 2           Rate 2 Percentage
/// POT 3           Rate 3 Percentage, if FULL then Rate 3 Percentge CV is used instead



#include <FlexiTimer2.h>


////////// PINOUTS

#define CV_POT_IN1    A2  		// Rate 1 CV
#define CV_POT_IN2    A1		// Rate 2 CV
#define CV_POT3       A0		// Pulsewidth 1, Rate 3, or Rate 3 percentage
#define CV_IN3        A3		// Pusewidth 1 CV, Rate 3 percentage CV, or LFO 3 Out
#define CV_AUDIO_IN   A4		// Reset
#define CV_AUDIO_OUT  11		// LFO 1 Out 
#define CV_GATE_OUT   8			// LFO 2 Out or Mixed LFO Out


#define MAXIMUM (1000)
#define HIGH

// dividend can have a range 0 ... 1824
uint16_t div25(uint16_t dividend)
{
  uint32_t invDivisor = 0x0a3c;
  uint16_t div = (uint16_t) ((invDivisor * (dividend + 1)) >> 16);
  return div;
}

// dividend can have a range 0 ... 1874
uint16_t div125(uint16_t dividend)
{
  uint32_t invDivisor = 0x020c;
  uint16_t div = (uint16_t) ((invDivisor * (dividend + 1)) >> 16);
  return div;
}

boolean running = false;

uint16_t rate1;
uint16_t pw1;
uint16_t state1;

int16_t rate2;      // note int16_t, see pulseMix()
uint16_t pw2;
uint16_t state2;

int16_t rate3;      // note int16_t, see pulseMix()
uint16_t pw3;
uint16_t state3;

boolean lfo1 = false;
boolean lfo2 = false;
boolean lfo3 = false;

void pulseMain()
{
  // grab current values

  uint16_t r1 = analogRead(CV_POT_IN1);
  uint16_t r2 = analogRead(CV_POT_IN2);
  uint16_t p1 = analogRead(CV_POT3);
  if (p1 > 1000)
  {
    p1 = analogRead(CV_IN3) + 1;
  }
  else p1 = (int16_t)min(1023, (((uint32_t)p1) * (uint32_t) 131 / 125)) + 1;

  // ugh costly division, but div125 doesn't go far enough
  // We multiply by 131 rather than 128 to give a little breathing room
  // at the top before we jump straight to the analog input:
  // values 977 through 1000 will be bumped to 1024.


  // filter values

  if (!running)
  {
    rate1 = r1;
    pw1 = p1;
    rate2 = r2;
    running = true;
  }
  else
  {
    rate1 = (rate1 + r1) >> 1;
    pw1 = (pw1 + p1) >> 1;
    rate2 = (rate2 + r2) >> 1;
  }
  pw2 = 512;

  // update states

  state1 = state1 + (rate1 + 1);
  state2 = state2 + (rate2 + 1);

  // send LFO signals

  // state1 UP
  if (
#ifdef FAST
    state1 >= 1024
#else
    state1 >= 1024 * 16
#endif
  )
  {
    state1 = 0;
    digitalWrite(CV_AUDIO_OUT, 1);
    lfo1 = true;
  }
  // state1 DOWN
  else if (lfo1 &&
#ifdef FAST
           state1 >= ((1024 * (uint32_t)pw1) >> 10)
#else
           state1 >= ((1024 * 16 * (uint32_t)pw1) >> 10)
#endif
          )
  {
    digitalWrite(CV_AUDIO_OUT, 0);
    lfo1 = false;
  }

  // state2 UP
  if (
#ifdef FAST
    state2 >= 1024
#else
    state2 >= 1024 * 16
#endif
  )
  {
    state2 = 0;
    digitalWrite(CV_GATE_OUT, 1);
    lfo2 = true;
  }
  // state2 DOWN
  else if (lfo2 &&
#ifdef FAST
           state2 >= ((1024 * (uint32_t)pw2) >> 10)
#else
           state2 >= ((1024 * 16 * (uint32_t)pw2) >> 10)
#endif
          )
  {
    digitalWrite(CV_GATE_OUT, 0);
  }
}


void pulseMulti()
{
  // grab current values

  uint16_t r1 = analogRead(CV_POT_IN1);
  uint16_t r2 = analogRead(CV_POT_IN2);
  uint16_t r3 = analogRead(CV_POT3);
  analogRead(CV_AUDIO_IN);   // Let this one bleed because the input impedance on the GRAINS is too high
//  analogRead(CV_AUDIO_IN);   // Let this one bleed because the input impedance on the GRAINS is too high

  // filter values

  if (!running)
  {
    rate1 = r1;
    rate2 = r2;
    rate3 = r3;
    running = true;
  }
  else
  {
    rate1 = (rate1 + r1) >> 1;
    rate2 = (rate2 + r2) >> 1;
    rate3 = (rate3 + r3) >> 1;
  }
  pw1 = 512;
  pw2 = 512;
  pw3 = 512;

  // update states

  state1 = state1 + (rate1 + 1);
  state2 = state2 + (rate2 + 1);
  state3 = state3 + (rate3 + 1);

  // send LFO signals

  // state1 UP
  if (
#ifdef FAST
    state1 >= 1024
#else
    state1 >= 1024 * 16
#endif
  )
  {
    state1 = 0;
    digitalWrite(CV_AUDIO_OUT, 1);
    lfo1 = true;
  }
  // state1 DOWN
  else if (lfo1 &&
#ifdef FAST
           state1 >= ((1024 * (uint32_t)pw1) >> 10)
#else
           state1 >= ((1024 * 16 * (uint32_t)pw1) >> 10)
#endif
          )
  {
    state1 = 0;
    digitalWrite(CV_AUDIO_OUT, 0);
    lfo1 = false;
  }

  // state2 UP
  if (
#ifdef FAST
    state2 >= 1024
#else
    state2 >= 1024 * 16
#endif
  )
  {
    state2 = 0;
    digitalWrite(CV_GATE_OUT, 1);
    lfo2 = true;
  }
  // state2 DOWN
  else if (lfo2 &&
#ifdef FAST
           state2 >= ((1024 * (uint32_t)pw2) >> 10)
#else
           state2 >= ((1024 * 16 * (uint32_t)pw2) >> 10)
#endif
          )
  {
    state1 = 0;
    digitalWrite(CV_GATE_OUT, 0);
    lfo2 = false;
  }

  // state3 UP
  if (
#ifdef FAST
    state3 >= 1024
#else
    state3 >= 1024 * 16
#endif
  )
  {
    state3 = 0;
    digitalWrite(CV_IN3, 1);
    lfo3 = true;
  }
  // state3 DOWN
  else if (lfo3 &&
#ifdef FAST
           state3 >= ((1024 * (uint32_t)pw3) >> 10)
#else
           state3 >= ((1024 * 16 * (uint32_t)pw3) >> 10)
#endif
          )
  {
    state3 = 0;
    digitalWrite(CV_IN3, 0);
    lfo3 = false;
  }
}

boolean mixState = false;
void pulseMix()
{
  // grab current values

  uint16_t r1 = analogRead(CV_POT_IN1);
  uint16_t r2 = analogRead(CV_POT_IN2);
  uint16_t r3 = analogRead(CV_POT3);
  if (r3 > 1000)
  {
    r3 = analogRead(CV_IN3) + 1;
  }
  else r3 = (int16_t)min(1023, (((uint32_t)r3) * (uint32_t) 131 / 125)) + 1;

  // ugh costly division, but div125 doesn't go far enough
  // We multiply by 131 rather than 128 to give a little breathing room
  // at the top before we jump straight to the analog input:
  // values 977 through 1000 will be bumped to 1024.

  // filter values

  if (!running)
  {
    rate1 = r1;
    rate2 = (uint16_t)(r1 * (uint32_t) r2) / 1024;
    rate3 = (uint16_t)(r1 * (uint32_t) r3) / 1024;
    running = true;
  }
  else
  {
    rate1 = (rate1 + r1) >> 1;
    r2 = (uint16_t)(r1 * (uint32_t) r2) / 1024;
    rate2 = (rate2 + r2) >> 1;
    r3 = (uint16_t)(r1 * (uint32_t) r3) / 1024;
    rate3 = (rate3 + r3) >> 1;
  }
  pw1 = 512;
  pw2 = 512;
  pw3 = 512;

  // update states

  state1 = state1 + (rate1 + 1);
  state2 = state2 + (rate2 + 1);
  state2 = state2 + (rate3 + 1);

  // send LFO signals

  // state1 UP
  if (
#ifdef FAST
    state1 >= 1024 || state2 >= 1024 || state3 >= 1024
#else
    state1 >= 1024 * 16 || state2 >= 1024 * 16 || state3 >= 1024 * 16
#endif
  )
  {
#ifdef FAST
    if (state1 >= 1024)
      state1 = 0;
    if (state2 >= 1024)
      state2 = 0;
    if (state3 >= 1024)
      state3 = 0;
#else
    if (state1 >= 1024 * 16)
      state1 = 0;
    if (state2 >= 1024 * 16)
      state2 = 0;
    if (state3 >= 1024 * 16)
      state3 = 0;
#endif
    mixState = !mixState;
    digitalWrite(CV_AUDIO_OUT, state1);
    digitalWrite(CV_GATE_OUT, mixState);
  }
}

void pulse()
{
#if defined(MIX)
  pulseMix();
#elif defined(MULTI)
  pulseMulti();
#else
  pulseMain();
#endif
}


#define RESET_TRIGGER 800
#define RESET_LOW 400

void setup()
{
  //// Change the pin modes, even for the analog in
#if defined(MIX)
  pinMode(CV_AUDIO_OUT, OUTPUT);
  pinMode(CV_GATE_OUT, INPUT);
#elif defined(MULTI)
  pinMode(CV_AUDIO_OUT, OUTPUT);
  pinMode(CV_GATE_OUT, OUTPUT);
  pinMode(CV_IN3, OUTPUT);
#else
  pinMode(CV_AUDIO_OUT, OUTPUT);
  pinMode(CV_GATE_OUT, OUTPUT);
#endif

  FlexiTimer2::set(1, 1.0 / 1024, pulse);
  FlexiTimer2::start();

  Serial.begin(115200);
}


boolean reset = false;
void loop()
{
  // Handle resets

// NOTE: input impedance on analogRead is too high, so it might not
// charge the caps fully and the PREVIOUS analogRead -- of a different pin! --
// might be returned instead.  To deal with this, elsewhere I have a throwaway
// analogRead(CV_AUDIO_IN) after the various analogRead(...) calls on the timer.
//
// Alternatively I could put the throwaway here, an wrap them with a nointerrupts()
// so the other anaogReads can't sneak in in-between from the timer, but 
// it would just cut the frequency of the analogReads() in half.
  uint16_t a = analogRead(CV_AUDIO_IN);
  if (a <= RESET_LOW)
  {
    reset = false;
  }
  else if (a >= RESET_TRIGGER && !reset)
  {
    state1 = 0;
    state2 = 0;
    state3 = 0;
    reset = true;
  }

  /*
    #ifdef MIX
      else
        {
        uint16_t b = analogRead(CV_GATE_OUT);
        if (b >= RESET_TRIGGER)
          {
        state2 = 0;
        state3 = 0;
          }
        }
    #endif
  */
}
