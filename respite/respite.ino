// Copyright 2023 Sean Luke 
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License



/// RESPITE
///
/// Respite delays a raised gate so that it occurs at least N milliseconds after the 
/// gate was previously lowered.  It can do this for two independent digital signals.  You
/// can set the value of N up to about 35 milliseconds.
///
/// What is the purpose of this?
///
/// Respite fixes a bug in AE Modular's 2ENV module which is triggered especially by
/// incoming MIDI data.  If the 2ENV module has just received a lowered gate, it needs
/// to take a little break (a few milliseconds) before the next raised gate, or otherwise 
/// it will completely ignore the raised gate.   This is a problem for MIDI data because
/// many MIDI groveboxes, such as the Polyend Tracker, by default play new notes IMMEDIATELY
/// after the previous note has completed, and so this new note will not trigger the 2ENV.
///
/// SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)
///
/// NOTE: we'd be able to do this for three signals, but IN1 and IN2 have large capacitors
/// on them and are too slow to respond.  So they're out.

/// CONFIGURATION
///
/// IN 1            UNUSED
/// IN 2            UNUSED
/// IN 3            Input 2
/// AUDIO IN (A)    Output 2
/// AUDIO OUT       Output 1
/// DIGITAL OUT (D) Input 1
///
/// POT 1           UNUSED
/// POT 2           UNUSED
/// POT 3           Degree of Delay.  Tune this to get the envelope working properly.





////////// PINOUTS

#define CV_POT_IN1    A2   
#define CV_POT_IN2    A1  
#define CV_POT3       A0  // Delay Amount
#define CV_IN3        A3  // Input 2
#define CV_AUDIO_IN   A4  // Output 2
#define CV_AUDIO_OUT  11  // Output 1
#define CV_GATE_OUT   8   // Input 1
#define RANDOM_PIN    A5

void setup()
    {
    pinMode(CV_AUDIO_IN, OUTPUT);
    pinMode(CV_AUDIO_OUT, OUTPUT);
    pinMode(CV_GATE_OUT, INPUT);
    }

uint8_t _delay;					// how many loop iterations we should wait after a gate is lowered

// Current gate state (low/high)
uint8_t gate1 = 0;
uint8_t gate2 = 0;

// Current gate countdown counter
uint8_t gateCounter1 = 0;
uint8_t gateCounter2 = 0;

// Should we go high when the counter runs out?
uint8_t goHi1 = 0;
uint8_t goHi2 = 0;

// Thresholds for high/low
#define HIGH 600
#define LOW 400

void loop()
    {
    // Decrease Gate counter 1 and go hi if needed
    if (gateCounter1 > 0)
    	{
    	if (gateCounter1 == 1 && goHi1)
    		{
			digitalWrite(CV_AUDIO_OUT, 0);
   			gate1 = 0;
			goHi1 = 0;
    		}
    	gateCounter1--;
    	}
    
    // Decrease Gate counter 2 and go hi if needed
    if (gateCounter2 > 0)
    	{
    	if (gateCounter2 == 1 && goHi2)
    		{
			digitalWrite(CV_AUDIO_IN, 0);
   			gate2 = 0;
			goHi2 = 0;
    		}
    	gateCounter2--;
    	}
    
    // Read inputs
    // An analog read can be done 9615 times a sec, so four of them can be done 2404 times a sec.
    // Maybe we could do this faster but it's probably okay...
    uint16_t in1 = digitalRead(CV_GATE_OUT);
    uint16_t in2 = analogRead(CV_IN3);
   	_delay = analogRead(CV_POT3) >> 2;
   	
   	
   	/// Test for Gate 1
   	if (gate1 && !in1) // (in1 < LOW))	// we went low
   		{
   		digitalWrite(CV_AUDIO_OUT, 0);
   		gate1 = 0;
   		goHi1 = 0;
   		gateCounter1 = _delay;
   		}
   	else if (!gate1 && in1) // (in1 > HIGH))	// we went high
   		{
   		if (gateCounter1 > 0) goHi1 = 1;
   		else 
   			{ 
   			digitalWrite(CV_AUDIO_OUT, 1); 
   			gate1 = 1;
   			goHi1 = 0; 
   			}
   		}



   	/// Test for Gate 2
   	if (gate2 && (in2 < LOW))	// we went low
   		{
   		digitalWrite(CV_AUDIO_IN, 0);
   		gate2 = 0;
   		goHi2 = 0;
   		gateCounter2 = _delay;
   		}
   	else if (!gate2 && (in2 > HIGH))	// we went high
   		{
   		if (gateCounter2 > 0) goHi2 = 1;
   		else 
   			{ 
   			digitalWrite(CV_AUDIO_IN, 1); 
   			gate2 = 1;
   			goHi2 = 0; 
   			}
   		}
 }


