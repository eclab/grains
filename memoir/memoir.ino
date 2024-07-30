// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// MEMOIR
///
/// Memoir is a Gate and CV recorder.  It can record one track of digital (gate/trigger) signals
/// and one track of CV signals.  You can use these tracks for anything you like, but one 
/// interesting choice is to record a few notes.  The recorded tracks are stored in EEPROM 
/// and survive power cycling.
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.
///
/// You can use Memoir to record a stream of CV changes plus a separate stream of Gate/Trigger 
/// events over a period of time.  The shortest time is 4 seconds, the longest time is 32 seconds.  
/// At 4 seconds, the recording is 128 updates per second.  At 32 seconds, the recording is 16 updates 
/// per second.
///
/// After you have recorded this stream, you can trigger it to play back at any time.
///
/// RECORDING 
/// 
/// Recording is initiated by sending a trigger to the RECORD TRIGGER, which is AUDIO IN.  Thereafter
/// recording will go on for some number of seconds, or until a PLAY TRIGGER (IN 3) is sent.  Then the recording
/// is saved.
///
/// If we send another RECORD TRIGGER during recording, recording is erased and restarted.
///
/// PLAYING 
///
/// Playing is initiated by sending a trigger to the PLAY TRIGGER, which is IN 3.   If we're still recording, 
/// it is saved initially first (which takes a few seconds).  Then replay starts.
///
/// If we send another PLAY TRIGGER during playback, playback is restarted.
///
/// LENGTH OF TIME 
/// 
/// The length of the recording varies depending on the setting of POT 3 when recording is initiated.
/// POT 3 can be 4, 8, 12, 16, 20, 24, 28, or 32 seconds depending on setting (left to right).  GRAINS has
/// no way of indicating that recording has finished; you either wait it out or send a PLAY TRIGGER.
///
/// Playback is similarly 4 ... 32 seconds depending on the setting of POT 3.  This means that you can record
/// 4 seconds of data and then stretch it over 32 seconds (or record 32 seconds of data and compress it to 4!)


/// CONFIGURATION
///
/// IN 1            CV In
/// IN 2            Gate In
/// IN 3            Play Trigger
/// AUDIO IN (A)    Record Trigger                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
/// AUDIO OUT       CV Out
/// DIGITAL OUT (D) Gate Out
///
/// POT 1           CV In
///
/// POT 2           Gate In
///
/// POT 3           Rate



#define CONTROL_RATE 128			// must be 128, so we can play wave for 4 secs

#define CV_POT_IN1    A2    // CV In
#define CV_POT_IN2    A1    // Gate In
#define CV_POT3       A0    // Rate
#define CV_IN3        A3    // Play Trigger
#define CV_AUDIO_IN   A4    // Record Trigger
#define CV_AUDIO_OUT  9     // CV Out
#define CV_GATE_OUT   8     // Gate Out
#define RANDOM_PIN    A5


#include <EEPROM.h>
#include <MozziGuts.h>

void setup() 
    {
    startMozzi();

	pinMode(CV_IN3, INPUT);
	pinMode(CV_AUDIO_IN, INPUT);
	pinMode(CV_GATE_OUT, OUTPUT);
    load();
    }
    
#define HIGH 600
#define LOW 400
uint8_t lastRecording = 0;
uint8_t lastPlaying = 0;
uint8_t doneRecording = 0;

uint8_t rate;
uint8_t tick = 0;
#ifdef SCALE
uint16_t scale = 1;			// 0...255, 128 is identity
int16_t shift = 0;			// -256 ... 256, 0 is identity
#endif

uint16_t analog;
uint8_t digital = 0;

#define STATE_NONE 0
#define STATE_RECORD 1
#define STATE_PLAY 2 
uint8_t state = STATE_NONE;

uint16_t count = 0;
#define MAX_COUNT 1024
uint8_t data[MAX_COUNT];

// Data is stored as follows:
//
// -- DATA X+1 ---   --- DATA X ----   
// 7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0
//             D A   A A A A A A A A
 
// To extract:
// analog = data[x] | (((uint16_t)(data[x+1] & 1)) << 8)
	// That is, take the 0th bit of data[x+1], shift it up by 8, and OR it with data[x]
// digital = data[x+1] >> 1

// To construct:
// data[x] = analog & 255;
// data[x+1] = (analog >> 8) | (digital << 1)


void store() 
	{ 
	// stretch first
	if (count >= 2 && count <= MAX_COUNT)
		{
		for(uint16_t i = count; i < MAX_COUNT; i++)
			{
			data[i] = data[i - 2];
			data[i + 1] = data[i + 1 - 2];
			}
		}
	for(uint16_t i = 0; i < MAX_COUNT; i++)
		EEPROM.update(i, data[i]); 
	}

// load wave sequence
void load() 
	{ 
	for(uint16_t i = 0; i < MAX_COUNT; i++)
		data[i] = EEPROM.read(i); 
	}
	
uint8_t readDigital()
	{
	uint16_t d = mozziAnalogRead(CV_POT_IN2);
	if (d < LOW) digital = 0;
	else if (d > HIGH) digital = 1;
	return digital;
	}
	
void updateControl() 
    {
    // Load recording / playing triggers
    
    uint8_t recording = 0;
    uint8_t playing = 0;
    uint16_t v = mozziAnalogRead(CV_AUDIO_IN);
    if (v > HIGH) recording = 1;
    else if (v < LOW) recording = 0;
    v = mozziAnalogRead(CV_IN3);
    if (v > HIGH) playing = 1;
    else if (v < LOW) playing = 0;
    
    rate = mozziAnalogRead(CV_POT3) >> 7 + 1;			// 1...8
    
    tick++;
    if (tick >= rate)
		{
		tick = 0;
		
		// Recording takes precedence
		if (recording && !lastRecording)	// we just sent the trigger
			{
			// Let's start recording
			doneRecording = false;
			count = 0;
			analog = mozziAnalogRead(CV_POT_IN1) >> 1;		// 0...511
			digital = readDigital();
			data[count] = analog & 255;
			data[count + 1] = (analog >> 8) | (digital << 1);
			count += 2;
			state = STATE_RECORD;
			}
		else if (state == STATE_RECORD && !doneRecording)					// trigger is up
			{
			if (count < MAX_COUNT - 1)
				{
				analog = mozziAnalogRead(CV_POT_IN1) >> 1;	// 0...511
				digital = readDigital();
				data[count] = analog & 255;
				data[count + 1] = (analog >> 8) | (digital << 1);
				count += 2;
			    digitalWrite(CV_GATE_OUT, digital);
				}
				
			if (count >= MAX_COUNT)
				{
				count = MAX_COUNT;
				if (!doneRecording) store();
				doneRecording = true;
				}
			}

		if (playing && !lastPlaying)	// we just raised the trigger
			{
			// store the recording
			if (state == STATE_RECORD && !doneRecording)
				{
				if (!doneRecording) store();
				doneRecording = true;
				}

#ifdef SCALE
			scale = mozziAnalogRead(CV_POT_IN1) >> 3;
			shift = (mozziAnalogRead(CV_POT_IN1) >> 1) - (int16_t)256;
#endif			

			count = 0;
			analog = data[count] | (((uint16_t)(data[count + 1] & 1)) << 8);
			digital = data[count + 1] >> 1;
			count += 2;
			state = STATE_PLAY;
    		digitalWrite(CV_GATE_OUT, digital);
			}
		else if (state == STATE_PLAY)
			{
			doneRecording = true;	// just to make sure
			if (count < MAX_COUNT - 1)
				{
				analog = data[count] | (((uint16_t)(data[count + 1] & 1)) << 8);
				digital = data[count + 1] >> 1;
				count += 2;
			    digitalWrite(CV_GATE_OUT, digital);
				}
				
			if (count >= MAX_COUNT)
				{
				count = MAX_COUNT;
				}
			}
		lastPlaying = playing;
		lastRecording = recording;
		}
    }


/** Maps 0...512 to -244 ... +167 */ 
inline int16_t scaleAudioMediumBiased(int16_t val)
	{
	/*
#ifdef SCALE
	if (state == STATE_PLAY)
		{
		int16_t val = ((int16_t)((((val * (uint32_t)scale) >> 7) * (uint32_t)103) >> 7)) - 244 + shift;
		if (val < -244) val = -244;
		if (val > 243) val = 243;
		return val;
		}
	else
		{
		return ((int16_t)((val * (uint32_t)103) >> 7)) - 244;
		}
#else
*/
	return ((int16_t)((val * (uint32_t)103) >> 7)) - 244;
//#endif
	}

int updateAudio()    
    {
    return(scaleAudioMediumBiased(analog));
    }

void loop() 
    {
    audioHook();
    }
