// Copyright 2026 Sean Luke 
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License



/// CVCO
///
/// CVCO is meant to run on the AE Modular GRAINS, but it could be adapted
/// to any Arduino.  CVCO was written by Sean Luke [sean@cs.gmu.edu]
///
/// SET GRAINS TO GRAINS MODE.
///
/// CVCO does two things:
///
/// 1. It converts gate/cv into MIDI notes for the Wonkystuff mco/1 and mco1/b.
/// This allows you to drive the mco/1 and mco/1b from a SEQ16 or other step
/// sequencer, or any other gate/cv source. This performs essentially the same 
/// task as the Wonkystuff CVMx in this regard.
///
/// 2. It maps certain incoming CV sources and pots to a variety of settings
/// to a variety of CC or other MIDI settings.  The settings depend on which
/// module you have.  Your module options are:

///  A. The original mco/1
///  B. The mco/1 with revised firmware, as of April 2024
///  C. The mco/1b

/// You set which module you are controlling by uncommenting the appropriate #define below.
/// Make sure only one module is uncommented.  The others should be commented out, that is,
/// they should have a // before them.

// #define ORIGINAL_MCO
#define NEW_MCO1
// #define MCO1B


/// You set the CC PARAMETER being output by turning POT 3.
/// You set the CC VALUE being output by turning POT 2.
///
/// When you change the CC PARAMETER, or the first time after the module has powered up,
/// You cannot change the CC value until you first turn POT 2 a significant distance (over 1/8
/// of the total travel of POT 2), to "unlock" POT 2.

/// The CC PARAMETERS vary depending on the kind of module you have set.  Specifically,
/// there are:

/// ORIGINAL MCO/1
///  1. Sawtooth Amplitude
///  2. Pulse Amplitude
///  3. Pulse Width
///  4. Pulse Width Modulation Amount
///  5. Suboscillatior Amplitude
///  6. Noise Amplitude
///  7. Velocity:    the pot or CV sets the velocity (volume) of notes played  (This is not a CC of course)
///  8. Nothing:     the pot doesn't do anything

/// REVISED MCO/1
///  1. Sawtooth Amplitude
///  2. Pulse Amplitude
///  3. Pulse Width
///  4. Pulse Width Modulation Amount
///  5. Suboscillatior Amplitude
///  6. Noise Amplitude
///  7. LFO Rate
///  8. LFO Pitch Modulation Amount
///  9. Velocity Sensitivity
/// 10. Legato Switch (On -> Turn Pot High / Off -> Turn Pot Low)
/// 11. Program Change: values 0...31    (This is not a CC of course)
/// 12. Velocity:    the pot or CV sets the velocity (volume) of notes played  (This is not a CC of course)
/// 13. Nothing:     the pot doesn't do anything

/// MCO/1b
///  1. Sawtooth Amplitude
///  2. Pulse Amplitude
///  3. Pulse Width
///  4. Pulse Width Modulation Amount
///  5. Suboscillatior Amplitude
///  6. Noise Amplitude
///  7. LFO Rate
///  8. LFO Pitch Modulation Amount
///  9. Sine Amplitude
/// 10. Velocity Sensitivity
/// 11. Transpose
/// 12. Legato Switch (On -> Turn Pot High / Off -> Turn Pot Low)
/// 13. Pitch Bend Range (0...12 semitones)    (This is not a CC of course it's RPN)
/// 14. Program Change: values 0...31    (This is not a CC of course)
/// 15. Velocity:    the pot or CV sets the velocity (volume) of notes played  (This is not a CC of course)
/// 16. Nothing:     the pot doesn't do anything

/// Thus you probably by default want to have POT 3 set to FAR RIGHT ("nothing"),
/// and the MCO or MCO/1B will only receive notes.


/// RESETTING THE MCO/1B
///
/// CVCO assumes that your mco1b has default learned MIDI settings for its notes and
/// CC parameters.  If you have changed them, you will need to change them back.  To do
/// this, you can long-press the MCO/1B button, then send it a MIDI CC 10 (any value).
/// To assist you in this, simply set the Parameter (POT 3) to FAR LEFT.  This is
/// Sawtooth Amplitude, which is CC 10.  Then put the MCO/1B in MIDI train mode, and
/// turn the Value (POT 2) knob back and forth, which will send various values of 
/// CC 10 to the MCO/1B.


/// TRANSPOSITION
/// This GRAINS program is set up by default to associate 0V with MIDI note 36 (two octaves below Middle C)
/// and 5V with MIDI note 96 (three octaves above Middle C).  You can transpose these notes up or down
/// by modifying the following #define, which is set in semitones.  For example to move the whole thing
/// down one octave, you'd say        b#define TRANSPOSE (-12)

#define TRANSPOSE (0)


/// NOTE VELOCITY
/// Instead of setting the note velocity via POT2/IN2, you can set it via the AUDIO IN CV value, and use
/// POT2/IN2 for something else.  In this case setting POT3 to "Note Velocity" would just set it to "Nothing".
///
/// Note that AUDIO IN reaches its maximum at about 4 volts.
/// 
/// You can turn on this feature by uncommenting the following #define  (remove the // in front of it)

// #define SEPARATE_VELOCITY

/// Also note that in order for changing note velocity via POT 2 to have any effect, you need to turn up 
/// note velocity sensitivity.



/// ADJUSTING TUNING AND TRACKING
///
/// Grains's Inputs track 1.3V/octave, not 1V/octave: we'll need to scale them to track properly.  
/// To do this, you can adjust the Pitch CV Scaling on Pot 1.    However you need to get the tracking
/// (scaling) properly trimmed or that won't happen.  The right setting on Pot 1 is approximately the 
/// 2.5 o'clock position.  Note that if you don't get it quite right, you'll probably get occasional 
/// incorrect pitches, even on QUANTIZER, as noise pushes the pitch values just barely into the next
/// pitch area.  You'll need to tweak it slightly.  Also note that even if you have it set right, 
/// SEQ16 and SEQ8 may still cause occasional incorrect pitches if they're sitting right on the border 
/// between two note pitches.  You'll need to adjust their trim a tiny bit towards the pitch you wanted.

///
/// Note that as GRAINS resistors warm up, the scaling will change and you will need to adjust the 
/// tracking again, at least until they are fully warmed up.


/// SOFT SERIAL

/// We presume you are using a GRAINS, not a GRAINS V2, of which at present I'm the only
/// owner in the world outside of Tangible Waves.  If you ARE on a GRAINS V2, you will want
/// to use hardware serial output, not soft serial, which isn't very reliable.  To do that
/// you would comment out the following #define (normally don't do this):

#define USE_SOFT_SERIAL




/// CONFIGURATION
///
/// IN 1            Note Pitch
/// IN 2            CC Value CV
/// IN 3            Gate
/// AUDIO IN (A)    Optional Note Velocity CV
/// AUDIO OUT       [UNUSED]
/// DIGITAL OUT (D) MIDI Out to MCO/1 or MCO/1b (on Grains)
///
/// POT 1           Note Pitch Scaling.  Set switch to IN1.  Set pot to approximately the 2 O'clock position, adjust from there.
/// POT 2           CC Value
/// POT 3           CC Parameter
///
/// Serial RX		MIDI Out to MCO/1 or MCO/1B (on Grains V2)        



////////// PINOUTS

#define CV_POT_IN1    A2    // Note Pitch CV
#define CV_POT_IN2    A1    // CC Value CV
#define CV_POT_3      A0    // CC Parameter
#define CV_IN3        A3    // Note Gate
#define CV_AUDIO_IN   A4    // Optional Note Velocity CV
#define CV_AUDIO_OUT  11    // Unused
#define CV_GATE_OUT   8     // MIDI Out

#define BLANK_SERIAL  5		// Blank Serial Pin
#define PIN_UNUSED 	  255
#define MIDI_RATE 	  31250




#define NOTHING								255			// Causes the pot to do nothing
#define VELOCITY							254			// Sets the velocity (volume) of notes; otherwise they default to 128

// options for the ORIGINAL mco/1
#define OLD_MCO1_SAWTOOTH_AMPLITUDE			6
#define OLD_MCO1_PULSE_AMPLITUDE			7
#define OLD_MCO1_PULSE_WIDTH				8
#define OLD_MCO1_PULSE_WIDTH_MODULATION		1
#define OLD_MCO1_SUBOSCILLATOR_AMPLITUDE	5
#define OLD_MCO1_NOISE_AMPLITUDE			2

// options for the REVISED mco/1
#define MCO1_SAWTOOTH_AMPLITUDE				4
#define MCO1_PULSE_AMPLITUDE				5
#define MCO1_PULSE_WIDTH					65
#define MCO1_PULSE_WIDTH_MODULATION			66
#define MCO1_SUBOSCILLATOR_AMPLITUDE		67
#define MCO1_NOISE_AMPLITUDE				69
#define MCO1_LFO_RATE						37
#define MCO1_LFO_PITCH_MODULATION			1
#define MCO1_VELOCITY_SENSITIVITY			36
#define MCO1_LEGATO							68
#define MCO1_PROGRAM_CHANGE					253

// options for the mco/1b
#define MCO1B_SAWTOOTH_AMPLITUDE			10
#define MCO1B_PULSE_AMPLITUDE				11
#define MCO1B_PULSE_WIDTH					12
#define MCO1B_PULSE_WIDTH_MODULATION		13
#define MCO1B_SUBOSCILLATOR_AMPLITUDE		14
#define MCO1B_NOISE_AMPLITUDE				15
#define MCO1B_LFO_RATE						16
#define MCO1B_LFO_PITCH_MODULATION			1
#define MCO1B_SINE_AMPLITUDE				17
#define MCO1B_VELOCITY_SENSITIVITY			18
#define MCO1B_TRANSPOSE						19
#define MCO1B_LEGATO						68
#define MCO1B_PITCH_BEND_RANGE				252
#define MCO1B_PROGRAM_CHANGE				251



#include "emitmidi.c"
#ifdef USE_SOFT_SERIAL
#include "NeoSWSerial.h"
NeoSWSerial softSerial(BLANK_SERIAL, CV_GATE_OUT, PIN_UNUSED);
#endif

struct midiEmitter midiEmitter;

int params[16] = 
#if defined(ORIGINAL_MCO1)
	{ OLD_MCO1_SAWTOOTH_AMPLITUDE, OLD_MCO1_PULSE_AMPLITUDE, OLD_MCO1_PULSE_WIDTH, OLD_MCO1_PULSE_WIDTH_MODULATION, OLD_MCO1_SUBOSCILLATOR_AMPLITUDE, OLD_MCO1_NOISE_AMPLITUDE, VELOCITY, NOTHING, NOTHING, NOTHING, NOTHING, NOTHING, NOTHING, NOTHING, NOTHING, NOTHING };
#elif defined(NEW_MCO1)
	{ MCO1_SAWTOOTH_AMPLITUDE, MCO1_PULSE_AMPLITUDE, MCO1_PULSE_WIDTH, MCO1_PULSE_WIDTH_MODULATION, MCO1_SUBOSCILLATOR_AMPLITUDE, MCO1_NOISE_AMPLITUDE, MCO1_LFO_RATE, MCO1_LFO_PITCH_MODULATION, MCO1_VELOCITY_SENSITIVITY, MCO1_LEGATO, MCO1_PROGRAM_CHANGE, VELOCITY, NOTHING, NOTHING, NOTHING, NOTHING };
#elif defined(MCO1B)
	{ MCO1B_SAWTOOTH_AMPLITUDE, MCO1B_PULSE_AMPLITUDE, MCO1B_PULSE_WIDTH, MCO1B_PULSE_WIDTH_MODULATION, MCO1B_SUBOSCILLATOR_AMPLITUDE, MCO1B_NOISE_AMPLITUDE, MCO1B_LFO_RATE, MCO1B_LFO_PITCH_MODULATION, MCO1B_SINE_AMPLITUDE, MCO1B_VELOCITY_SENSITIVITY, MCO1B_TRANSPOSE, MCO1B_LEGATO, MCO1B_PITCH_BEND_RANGE, MCO1B_PROGRAM_CHANGE, VELOCITY, NOTHING };
#endif


int8_t oldGate = -1;					// this was the last gate value.  It's -1 so it's guaranteed to be different from the new value
int16_t oldCV = -1;						// this was the last cv value.  It's -1 so it's guaranteed to be different from the new value
int8_t lastVal = -1;					// the last CC value.  We only update it if its new.  It's -1 so it's guaranteed to be different from the new value

void emitMidi(struct midiEmitter* emitter, unsigned char byte)
	{
#ifdef USE_SOFT_SERIAL
	softSerial.write(byte);
#else
	Serial.write(byte);
#endif
	}

void setup()
    {
#ifdef USE_SOFT_SERIAL
    softSerial.begin(MIDI_RATE);
#else GRAINS_V2
	Serial.begin(MIDI_RATE);
#endif
    initializeEmitter(&midiEmitter, 0);
    }


#define UNLOCK_DISTANCE (128)
#define DEFAULT_VELOCITY (127)
int16_t lockPos = 0;
uint8_t locked = true;
uint8_t param = 0;
uint8_t velocity = DEFAULT_VELOCITY;

void setLock()
	{
	lockPos = analogRead(CV_POT_IN2);
	locked = true;
	}

int16_t p = -1;
void setParameter()
	{
	int16_t newp = analogRead(CV_POT_3);
	if (p == -1) p = newp;
	else p = (p + newp) >> 1;			// cut down the noise a bit
	
#if defined(ORIGINAL_MCO1)
	uint8_t newParam = (p * 8) >> 10;
#elif defined(NEW_MCO1)
	uint8_t newParam = (p * 13) >> 10;
#elif defined(MCO1B)
	uint8_t newParam = (p * 16) >> 10;
#endif

	newParam = params[newParam];
	
	if (newParam != param)
		{
		// we changed the param, relock
		setLock();
		param = newParam;
		}
	}
	
int16_t v = -1;
void setValue()
	{
	int16_t newv = analogRead(CV_POT_IN2);
	if (v == -1) v = newv;
	else v = (v + newv) >> 1;			// cut down the noise a bit

	if (locked && abs(v - lockPos) < 128) return;		// we're locked
	else
		{
		locked = false;
		updateCC(param, v >> 3);
		}
	}

void updateCC(uint8_t param, uint16_t value)
	{
	velocity = DEFAULT_VELOCITY;
	
	if (param == NOTHING)
		{
		// do nothing
		}
#ifndef SEPARATE_VELOCITY
else if (param == VELOCITY)
		{
		velocity = value;
		}
#endif
	else if (param == MCO1_PROGRAM_CHANGE)
		{
		uint8_t pc = value >> 2;			// The first 32 are the ones that matter for MCO/1
		emitPC(&midiEmitter, pc, 0);
		}
	else if (param == MCO1B_PITCH_BEND_RANGE)
		{
		uint8_t bend = (12 * value);			// shift to range 0...12 * 128 (MSB)
		emitNRPN(&midiEmitter, 0, bend, true, 0);
		}
	else if (param == MCO1B_PROGRAM_CHANGE)
		{
		uint8_t pc = (42 * value) >> 7;			// shift to range 0...41
		emitPC(&midiEmitter, pc, 0);
		}
	else
		{
		emitCC(&midiEmitter, param, value, 0);
		}
	}
    
    
#define SEQ16_CV_DELAY	(10)		// Misfeature in SEQ16.  When the gate raises, the CV has not yet gotten to the value it needs to be.  Then the CV rises to the right value over the course of the next 5ms or so.  So if when the gate raises you sample the CV, it will actually be the PREVIOUS CV.  :-(

#define MEDIAN_OF_THREE(a,b,c) (((a) <= (b)) ? (((b) <= (c)) ? (b) : (((a) < (c)) ? (c) : (a))) : (((a) <= (c)) ? (a) : (((b) < (c)) ? (c) : (b))))

void loop()
    {
	// Send CC  -- should we do this more slowly?
	setParameter();
	setValue();

    // Read gate/CV
    uint8_t gate = (uint8_t)(analogRead(CV_IN3) >= 900);		// The SEQ16
    
    
// Send Note
	if (gate == 0 && oldGate != 0 && oldCV >= 0)
		{
		emitNoteOff(&midiEmitter, (uint8_t)oldCV, 0);
		oldGate = 0;
		}
	else if (gate == 1 && oldGate != 1)
		{
		delay(SEQ16_CV_DELAY);		// wait until the CV stabilizes
		
		// Now read the note...
/*
		uint16_t a = analogRead(CV_POT_IN1);
		uint16_t b = analogRead(CV_POT_IN1);
		uint16_t c = analogRead(CV_POT_IN1);
    uint8_t cv = (MEDIAN_OF_THREE(a,b,c) *  (uint16_t)60) >> 10;		// switch to values 0...59   (analogRead is int16_t ugh)
*/
		uint16_t a = (analogRead(CV_POT_IN1) + analogRead(CV_POT_IN1) + analogRead(CV_POT_IN1) + analogRead(CV_POT_IN1)) >> 2;
		// +
		//			  analogRead(CV_POT_IN1) + analogRead(CV_POT_IN1) + analogRead(CV_POT_IN1) + analogRead(CV_POT_IN1)) >> 3;
		uint8_t cv = (a * (uint16_t)60) >> 10;		// switch to values 0...59   (analogRead is int16_t ugh)

// adjust pitch to the presumed range
	cv += 21;				// range of the mco1/b is 21 ... 91
	cv += TRANSPOSE;
	if (cv < 0) cv = 0;
	if (cv > 127) cv = 127;

#ifdef SEPARATE_VELOCITY
	analogRead(CV_AUDIO_IN);		// throwaway
	velocity = analogRead(CV_AUDIO_IN) >> 3;
#endif
		emitNoteOn(&midiEmitter, (uint8_t)cv, velocity, 0);
		oldGate = 1;
		oldCV = cv;
		}
    }
