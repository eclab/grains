// Copyright 2024 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// DX-M
///
/// DX-M is a MIDI-driven 2- or 3-operator FM synthesizer.  DX-M is meant to run on the AE Modular GRAINS, 
/// but it could be adapted to any Arduino.
///
/// DX-M has four FM algorithms.  One algorithm has 2 operators but one operator has self-modulation.
/// The other three algorithms have three operators, but you will have less control from the pots.
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.
///
/// DX-M also sends a Gate via IN 3 whenever a note has been played, and drops it when the note
/// has been released.  If you play a new note prior to the old one being released, it will also 
/// drop the gate and wait tiny bit before raising it for the new note to prevent legato.

/// DIFFERENCES FROM DX
///
/// Because DX must take Pitch as a CV, it has to use IN1/POT1 doing it.  DX-M doesn't do this: it takes
/// data via MIDI on DIGITAL OUT.  As a result it can output Gate Out on IN3, and reserve IN1/POT1 for
/// something more useful.  So expect the socket and pot configurations to be different from DX.

/// GATE OUT
///
/// DX-M receives MIDI data, including Note On and Off onset.  It sends this data as a GATE out IN3.

/// CHANNEL
///
/// You can set DX-M's channel to 0...15 (for channels 1...16) or to OMNI (to listen to any of them).
/// This is set here:

// Set this to 0...15, or to OMNI
#define CHANNEL OMNI


///
/// HOW FM WORKS
/// 
/// All you need to know is that FM in DX-M is organized with several sine-wave oscillators called 
/// OPERATORS.  These operators output sound, but only some are used in the final output. If an 
/// operator's sound is used in the final output, it is called a CARRIER.  You can have multiple carriers.
/// Other operators make sound but that sound is used as a modulation source to change the frequency 
/// (really the phase) of ANOTHER operator in real time, warping it into another sound wave.  
/// These oscillators are called MODULATORS.  It is possible for a modulator
/// to modulate another modulator, or to modulate itself. [I'm abusing the terms modulator and 
/// carrier here to simplify things].
///
/// Each operator has a FREQUENCY.  One carrier plays the primary PITCH.  The other operator's
/// frequencies are determined RELATIVE to that carrier, and modulators commonly have relative pitches
/// that are always integer multiples or divisors of the carrier pitch (otherwise it doesn't sound tonal).
/// 
/// The connection between a modulator and the operator it is modulating has an associated value
/// called the INDEX OF MODULATION.  This is basically how much impact the modulator has on changing
/// the frequency of the downstream operator.  If you set it to zero, for example, it's as if they're
/// disconnected.
/// 
/// Changing both the frequency and index of modulation of a modulator can have a big impact on the
/// sound of the downstream operator. However commonly you'd only change the index of modulation in
/// real time (with an envelope, say).
///
/// The arrangement of operators -- which operators modulate which other ones, and which operators
/// are carriers -- is called an ALGORITHM.  This weird term stems from Yamaha.  DX-M offers four
/// algorithms.  The first algorithm has a single modulator modulating a single carrier, and the
/// modulator can also modulate itself.  The second algorithm has two modulators both modulating
/// a single carrier.  The third algorithm has one modulator modulating two carriers.  And the
/// fourth algorithm has a modulator modulating a second modulator, while that second modulator
/// modulates a carrier.
///
///
/// THE ALGORITHMS
///
/// You set the algorithm by changing the following define to a value 1...4
///
/// Confused?  Then just stick with the 2-modulator ALGORITHM 1, ignoring self modulation, to get your
/// feet wet.


#define ALGORITHM 1


/// ALGORITHM 1.  This is a two-operator algorithm, with a MODULATOR and a CARRIER.  The CARRIER makes
/// the final sound.  The 
/// modulator has a RELATIVE PITCH to the carrier, one of 0.5, 1.0, 2.0, 3.0, ... 15.0, selectable
/// on POT 3.  The modulator also has an INDEX OF MODULATION (the "amplitude" of the operator in Yamaha
/// speak) which determines how much affect the modulator has on the carrier.  This is set with POT2/IN2.
///
/// This is the basic setup.  However the modulator can also be optionally SELF MODULATED, that is, it
/// modulates itself.  You specify the INDEX OF SELF MODULATION with IN1/Pot1.  Because this is often done with
/// an envelope, you'll need a way to temper its maximum value.  This is done by setting the INDEX OF SELF
/// MODULATION SCALING on AUDIO IN.   
///
///
/// ALGORITHM 2.  This is a three-operator algorithm, with TWO MODULATORS simultaneously modulating the same
/// CARRIER.  Again, the CARRIER makes the final sound.  
/// Each modulator has a RELATIVE PITCH to the carrier, one of 0.5, 1.0, 2.0, 3.0, ... 15.0.  
/// Modulator 1's relative pitch is selectable on POT 3.  Modulator 2's relative pitch is selectable on
/// AUDIO IN.  Again, each modulator also has an INDEX OF MODULATION (the "amplitude" of the operator in
/// Yamaha speak) which determines how much affect the modulator has on the carrier.  For Modulator 1, this
/// is set with POT2/IN2.  For Modulator 2, this is set with POT1/IN1. 
///
///
/// ALGORITHM 3.  This is another three-operator algorithm, with ONE MODULATOR simultaneously modulating
/// TWO CARRIERS.  Each CARRIER makes a final sound -- they are summed.  Carrier 1's pitch is specified
/// in MIDI.  Carrier 2's pitch is specified
/// RELATIVE to Carrier on AUDIO IN, smoothly going from Carrier 1's pitch to two octaves above.  The
/// modulator also has a RELATIVE PITCH to the carrier, one of 0.5, 1.0, 2.0, 3.0, ... 15.0, selectable 
/// on POT 3.  For each carrier, the modulator also has an INDEX OF MODULATION (the "amplitude" of the 
/// operator in Yamaha speak) which determines how much affect the modulator has on the carrier.  The
/// modulation of Carrier 1 is set with POT2/IN2.  The modulation of Carrier 2 is set with POT1/IN1.
///
///
/// ALGORITHM 4.  This is a final three-operator algorithm, with MODUATOR 2 modulating MODULATOR 1, while
/// MODULATOR 1 is modulating the CARRIER.  The CARRIER makes the final sound.  
/// Each modulator has a RELATIVE PITCH to the carrier, one of 
/// 0.5, 1.0, 2.0, 3.0, ... 15.0.  For MODUATOR 1, this is selectable on POT 3.  For MODULATOR 2, this is
/// selectable on AUDIO IN.  Each modulator also has an INDEX OF MODULATION (the "amplitude" of the 
/// operator in Yamaha speak) which determines how much affect the modulator has on its downstream
/// modulator or carrier.  The index of modulation for MODULATOR 1 is set with POT2/IN2.  The index of
/// modulation for MODULATOR 2 is set with POT1/IN1.

/// VELOCITY SENSITIVITY AND NOTE OFF
///
/// By default, DX-M is neither velocity sensitive, nor does it respond to note off by shutting off sound.
/// You can turn on both of these features below.  However turning on response to note off may be annoying as
/// there is a slight background hum which is more apparent when there is no sound coming from the oscillator.
/// To turn one or both of these two features, uncomment the appropriate #defines below:

// #define VELOCITY_SENSITIVE

// #define RESPONDS_TO_NOTE_OFF



/// MIDI RESPONSE
///
/// MIDI NOTES 		Note On/Off
///						Pitch: All notes C0 through G10, but realistically C0 through B8
///						Velocity: Velocity 0...127 for Note On.  Velocity 0 is a Note Off
///						Velocity and note-off are switchable
///					
/// CC				All Notes Off		CC 123		[Resets all notes, lowers Gate]
///					All Sounds Off		CC 120		[Resets all notes, lowers Gate]


/// 2-OPERATOR CONFIGURATION
///
/// IN 1            Index of Self Modulation CV
/// IN 2            Index of Modulation CV
/// IN 3            Gate Out
/// AUDIO IN (A)    [Unused]
/// AUDIO OUT       Out
/// DIGITAL OUT (D) MIDI In
///
/// POT 1           Index of Self Modulation Scaling
///
/// POT 2           Index of Modulation Scaling
///
/// POT 3           Modulator Relative Pitch	[One of 0.5, 1, 2, 3 ... 15]


/// 3-OPERATOR (2 MODULATOR, 1 CARRIER) CONFIGURATION
///
/// IN 1            Index of Modulation CV, Modulator 2 to Carrier
/// IN 2            Index of Modulation CV, Modulator 1 to Carrier
/// IN 3            Gate Out
/// AUDIO IN (A)    Modulator 2 Relative Pitch			[One of 0.5, 1, 2, 3 ... 15]  [There is no Tune CV]
/// AUDIO OUT       Out
/// DIGITAL OUT (D) MIDI In
///
/// POT 1           Index of Modulation Scaling, Modulator 2 to Carrier
///
/// POT 2           Index of Modulation Scaling, Modulator 1 to Carrier
///
/// POT 3           Modulator 1 Relative Pitch	[One of 0.5, 1, 2, 3 ... 15]


/// 3-OPERATOR (1 MODULATOR, 2 CARRIER) CONFIGURATION
///
/// IN 1            Index of Modulation CV, Modulator to Carrier 2
/// IN 2            Index of Modulation CV, Modulator to Carrier 1
/// IN 3            Gate Out
/// AUDIO IN (A)    Carrier 2 Relative Pitch	[Same octave as, to 2 octaves above, Carrier 1] [There is no Tune CV]
/// AUDIO OUT       Out
/// DIGITAL OUT (D) MIDI In
///
/// POT 1           Index of Modulation Scaling, Modulator to Carrier 2
///
/// POT 2           Index of Modulation Scaling, Modulator to Carrier 1
///
/// POT 3           Modulator 1 Relative Pitch	[One of 0.5, 1, 2, 3 ... 15]



/// 3-OPERATOR (2 MODULATOR, 1 CARRIER IN SERIES) CONFIGURATION
///
/// IN 1            Index of Modulation, Modulator 2 to Modulator 1
/// IN 2            Index of Modulation, Modulator 1 to Carrier
/// IN 3            Gate Out
/// AUDIO IN (A)    Modulator 2 Relative Pitch			[One of 0.5, 1, 2, 3 ... 15]  [There is no Tune CV]
/// AUDIO OUT       Out
/// DIGITAL OUT (D) MIDI In
///
/// POT 1           Index of Modulation Scaling, Modulator 2 to Modulator 1
///
/// POT 2           Index of Modulation Scaling, Modulator 1 to Carrier
///
/// POT 3           Modulator 1 Relative Pitch	[One of 0.5, 1, 2, 3 ... 15]





PROGMEM const float frequencies[128] =
{
16.352, 17.3243, 18.3545, 19.4459, 20.6022, 21.8273, 23.1252,
24.5003, 25.9572, 27.5007, 29.136, 30.8685, 32.704, 34.6487, 36.709,
38.8918, 41.2045, 43.6546, 46.2504, 49.0006, 51.9144, 55.0014,
58.2719, 61.7369, 65.408, 69.2974, 73.418, 77.7837, 82.4089, 87.3092,
92.5009, 98.0013, 103.829, 110.003, 116.544, 123.474, 130.816,
138.595, 146.836, 155.567, 164.818, 174.618, 185.002, 196.003,
207.657, 220.005, 233.088, 246.948, 261.632, 277.189, 293.672,
311.135, 329.636, 349.237, 370.004, 392.005, 415.315, 440.011,
466.175, 493.895, 523.264, 554.379, 587.344, 622.269, 659.271,
698.474, 740.007, 784.01, 830.63, 880.022, 932.35, 987.791, 1046.53,
1108.76, 1174.69, 1244.54, 1318.54, 1396.95, 1480.01, 1568.02,
1661.26, 1760.04, 1864.7, 1975.58, 2093.06, 2217.52, 2349.38,
2489.08, 2637.09, 2793.89, 2960.03, 3136.04, 3322.52, 3520.09,
3729.4, 3951.16, 4186.11, 4435.03, 4698.75, 4978.15, 5274.17,
5587.79, 5920.06, 6272.08, 6645.04, 7040.17, 7458.8, 7902.33,
// Obviously beyond this it's moot
8372.22, 8870.06, 9397.5, 9956.31, 10548.3, 11175.6, 11840.1,
12544.2, 13290.1, 14080.3, 14917.6, 15804.7, 16744.4, 17740.1,
18795., 19912.6, 21096.7, 22351.2, 23680.2, 25088.3 
};




#define CV_POT_IN1    A2    // Note In, Pitch Scaling 
#define CV_POT_IN2    A1    
#define CV_POT3       A0    
#define CV_IN3        A3
#define CV_AUDIO_IN   A4
#define CV_AUDIO_OUT  9     // Out
#define CV_GATE_OUT   8
#define RANDOM_PIN    A5



#define FREQUENCY(pitch) pgm_read_float_near(&frequencies[pitch])
#define CONTROL_RATE 128

#include <MozziGuts.h>
#include <Oscil.h>

#include <tables/sin512_int8.h>
#include "NeoSWSerial.h"   // you have to install this via the library manager
#include "parsemidi.c"
#define MIDI_RATE 31250

#define BLANK_SERIAL	  5		// Blank Serial Pin
#define PIN_UNUSED 255
NeoSWSerial softSerial(CV_GATE_OUT, BLANK_SERIAL, PIN_UNUSED);
midiParser parse;



Oscil<SIN512_NUM_CELLS, AUDIO_RATE> carrier(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> modulator(SIN512_DATA);
#if (ALGORITHM > 1)
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> modulator2(SIN512_DATA);
#endif
#define carrier2 modulator2			// for convenience


void setup() 
    {
    pinMode(CV_IN3, OUTPUT);
    startMozzi();
	frequency = FREQUENCY(60);		// Middle C

	/// Setup MIDI
	initializeParser(&parse, CHANNEL, 0, 1);
	softSerial.begin(MIDI_RATE);
    }

uint8_t on = 0;
uint8_t gate = 0;
uint8_t timer = 0;
uint8_t _velocity = 127;
uint8_t vel = 0;
uint8_t sineVel = 0;
uint8_t pitch;

uint16_t audioIn;
uint16_t pot3;
float frequency;

void cc(midiParser* parser, unsigned char parameter, unsigned char value)
	{
	if (parameter == 120 || parameter == 123)	// All Sound Off or All Notes Off
		{
		on = 0;		
		// everyone is off, lower gate
		digitalWrite(CV_IN3, 0);
		gate = 0;
		}
	}
	
void noteOn(midiParser* parser, unsigned char note, unsigned char velocity)
	{
    audioIn = mozziAnalogRead(CV_AUDIO_IN);
	pot3 = mozziAnalogRead(CV_POT3);
	frequency = FREQUENCY(note);
	
	pitch = note;
	updateOperatorFrequencies();
#ifdef VELOCITY_SENSITIVE
	_velocity = velocity;
#endif
		
	on = 1;
			
	if (gate)
		{
		timer = 2;
		digitalWrite(CV_IN3, 0);
		gate = 0;
		}
	else
		{
		digitalWrite(CV_IN3, 1);
		gate = 1;		
		timer = 0;
		}
	}

void noteOff(midiParser* parser, unsigned char note, unsigned char velocity)
	{
	if (note == pitch)
		{
		on = 0;
		timer = 0;
		gate = 0;
		digitalWrite(CV_IN3, 0);
#ifdef RESPONDS_TO_NOTE_OFF
		_velocity = 0;
#endif
		}
	}


uint16_t indexOfModulation = 0;
uint16_t indexOfModulation2 = 0;
float relativeFrequency = 0;
float relativeFrequency2 = 0;
int16_t lastModulationValue = 0;

void updateOperatorFrequencies()
	{
#if (ALGORITHM == 1)			// 2 Operator
	uint8_t rf = pot3 >> 5;		// 0 ... 15
	relativeFrequency = (rf == 0 ? frequency * 0.5 : frequency * rf);
    carrier.setFreq(frequency);
    modulator.setFreq(relativeFrequency);
#elif (ALGORITHM == 2)			// 2 Modulators, 1 Carrier
	uint8_t rf = pot3 >> 5;		// 0 ... 15
	relativeFrequency = (rf == 0 ? frequency * 0.5 : frequency * rf);
	rf = audioIn >> 5;		// 0 ... 15
	relativeFrequency2 = (rf == 0 ? frequency * 0.5 : frequency * rf);
    carrier.setFreq(frequency);
    modulator.setFreq(relativeFrequency);
    modulator2.setFreq(relativeFrequency2);
#elif (ALGORITHM == 3)
	uint8_t rf = pot3 >> 5;		// 0 ... 15
	relativeFrequency = (rf == 0 ? frequency * 0.5 : frequency * rf);
	relativeFrequency2 = frequency * ((audioIn * 1.0 / 1024.0) + 1.0) * 2.0;
    carrier.setFreq(frequency);
    modulator.setFreq(relativeFrequency);
    carrier2.setFreq(relativeFrequency2);
#else
	uint8_t rf = pot3 >> 5;		// 0 ... 15
	relativeFrequency = (rf == 0 ? frequency * 0.5 : frequency * rf);
	rf = audioIn >> 5;		// 0 ... 15
	relativeFrequency2 = (rf == 0 ? frequency * 0.5 : frequency * rf);
    carrier.setFreq(frequency);
    modulator.setFreq(relativeFrequency);
    modulator2.setFreq(relativeFrequency2);
#endif		
	}

void updateControl() 
    {
	pot3 = mozziAnalogRead(CV_POT3);
    audioIn = mozziAnalogRead(CV_AUDIO_IN);
	uint16_t pot2 = mozziAnalogRead(CV_POT_IN2);
	uint16_t pot1 = mozziAnalogRead(CV_POT_IN1);

    if (timer > 0)
    	{
    	if (timer == 1)
    		{
    		digitalWrite(CV_IN3, 1);
    		gate = 1;
    		}
    	timer--;
    	}
    	
    uint8_t val = softSerial.available();
	for(uint8_t i = 0; i < val; i++)
		{
		parseMidi(&parse, softSerial.read());
		}

	indexOfModulation = pot2;
	indexOfModulation2 = pot1;
	
	updateOperatorFrequencies();
    }


int updateAudio()    
    {
#if (ALGORITHM == 1)
    // indexOfModulation is 1024 in range.  Going >> 1 and we have 512 in range.
    // lastModulationValue is 128 in range.  Multiply it in and we have 65536 in range.
	Q15n16 selfpm = (indexOfModulation2 >> 1) * lastModulationValue;
    // indexOfModulation is 1024 in range.  INDEX_OF_MODULATION_SCALE is 4 in range.
    // Multiply them and >> 3 and we have 512 in range.  
    // The modulator is 128 in range.  Multiply it in and we have 65536 in range.
	Q15n16 pm = (indexOfModulation >> 1) * (uint32_t)(lastModulationValue = modulator.phMod(selfpm));
    int16_t s1 = carrier.phMod(pm);
    return (s1 * _velocity) >> 7;
#elif (ALGORITHM == 2)
    // indexOfModulation is 1024 in range.
    // INDEX_OF_MODULATION_2_SCALING is 8 in range.  Multiply them and >> 1 and 
    // we are 512 in range.
    // The modulator is 128 in range.  Multiply it in and we have 65536 in range.
	Q15n16 pm2 = (((indexOfModulation2) >> 1) * ((uint32_t)modulator2.next())) >> 1;
	// Like PM above
	Q15n16 pm = ((indexOfModulation >> 1) * ((uint32_t)modulator.next())) >> 1;
    int16_t s1 = carrier.phMod(pm + pm2);
    return (s1 * _velocity) >> 7;
#elif (ALGORITHM == 3)
	uint32_t mod = modulator.next();
	// Like PM2 above
	Q15n16 pm2 = ((indexOfModulation2) >> 1) * mod;
	// Like PM above
	Q15n16 pm = (indexOfModulation >> 1) * mod;
    int16_t s1 = (carrier.phMod(pm) + carrier2.phMod(pm2)) >> 1;
    return (s1 * _velocity) >> 7;
#else
	uint32_t mod = modulator2.next();
	// Like PM2 above
	Q15n16 pm2 = ((indexOfModulation2) >> 1) * mod;
	// Like PM above
	Q15n16 pm = (indexOfModulation >> 1) * modulator.phMod(pm2);
    int16_t s1 = carrier.phMod(pm);
    return (s1 * _velocity) >> 7;
#endif	
    }


void loop() 
    {
    audioHook();
    }