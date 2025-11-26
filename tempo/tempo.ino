// Copyright 2025 Sean Luke 
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License



/// TEMPO
///
/// Tempo is a master clock is meant to run on the AE Modular GRAINS, but it could be adapted
/// to any Arduino.  Theory was written by Sean Luke [sean@cs.gmu.edu]
///
/// SET GRAINS TO GRAINS OR MOZZI MODE.  (It doesn't matter, we don't use Audio Out)
///
/// Tempo allows you to specify a beat, either by dialing in a Beats Per Minute on
/// POT 1 and POT 2, or by providing a tap tempo.  Then you can add swing or random
/// delay noise to the beat via POT 3.  Tempo then outputs the beat.
///
/// To specify a beat in beats per minute, you set POT 1 and POT 2.  POT 1 is divided
/// into sixteen sections, representing the "coarse" values 
/// 0, 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, and 240
/// POT 2 is similarly divided into sixteen sections, representing the values 0...15.
/// The BPM is the two added together.
///
/// If you turn BOTH POT 1 AND POT 2 to ZERO, Tempo will instead read its beat from
/// tap tempo derived from taps (triggers) that you provide via IN 3.
///
/// Tempo outputs triggers at the given BPM out Digital Out.
///
/// POT 3 indicates some amount of DELAY that is added to the output triggers.  At full left,
/// the DELAY is ZERO -- it's just the beat itself.  As you increase the pot clockwise,
/// Tempo increasingly performs looser and looser swing on every other beat.  When POT 3
/// passes its midpoint, Tempo starts with a zero Delay again, and then increasingly 
/// performs larger and larger random delays (humanization).

///
/// Tempo will only run if it receives a trigger on AUDIO IN.  Thereafter it will run
/// until it receives a second trigger to stop it.  You can then restart it with a third
/// trigger, and so on.
///
/// By default Tempo doesn't run until you trigger it.  If you like, you can change this
/// behavior so that Tempo immediately starts running on startup.  To do that, uncomment
/// the following #define (remove the '//' before it):

// #define FREE_START


/// CONFIGURATION
///
/// IN 1            CV for Coarse BPM
/// IN 2            CV for Fine BPM
/// IN 3            Tap Tempo
/// AUDIO IN (A)    Start/Stop
/// AUDIO OUT       [UNUSED]
/// DIGITAL OUT (D) Beat Out
///
/// POT 1           Coarse BPM
/// POT 2           Fine BPM
/// POT 3           Swing / Noise added to Beat  
///                 [No Swing--> Increasing Swing ---> No Noise ---> Increasing Noise]



////////// PINOUTS

#define CV_POT_IN1    A2    // 
#define CV_POT_IN2    A1    // 
#define CV_POT3       A0    //
#define CV_IN3        A3    // 
#define CV_AUDIO_IN   A4    // 
#define CV_AUDIO_OUT  11    // 
#define CV_GATE_OUT   8     // 

#define RANDOM_PIN    A5

void setup()
    {
    randomSeed(analogRead(RANDOM_PIN));

    pinMode(CV_GATE_OUT, OUTPUT);
    pinMode(CV_IN3, INPUT);
    pinMode(CV_AUDIO_IN, INPUT);
    // Serial.begin(115200);
    }


uint8_t in3_state = 0;
uint8_t ain_state = 0;
#ifdef FREE_START
uint8_t running = 1;
uint8_t starting = 1;
#else
uint8_t running = 0;
uint8_t starting = 0;
#endif
uint8_t reset = 0;
uint16_t rate = 1000;		 // something big
uint32_t nextTime = 0;
uint16_t timeInterval = 500;			// 120 BPM to start
uint32_t dropTime = 0;
uint32_t raiseTime = 0;
uint8_t triggered = 0;
uint8_t raised = 0;
uint8_t oddBeat = 0;
uint8_t swing = 0;
uint32_t lastTap = 0;
#define DROP_INTERVAL (64)
#define EXCESS_TAP_DIFF (12000) 		// our excess tap is 5 BPM
#define MAX_TAP_DIFF (10000) 			// our slowest tap is 6 BPM
#define TAP_POT_BOUNDARY (32)

void tap()
	{
	uint32_t time = millis();
	if (lastTap >= time || (time - lastTap) > EXCESS_TAP_DIFF)
		{
		// this is our first time
		}
	else
		{
		uint32_t diff = (time - lastTap);
		if (diff > MAX_TAP_DIFF) diff = MAX_TAP_DIFF;
		timeInterval = (uint16_t) diff;
		}
	lastTap = time;
	}
	
void toggle()
	{
	if (!running) starting = 1;
	running = !running;
	}
	
void trigger(uint16_t delay)
	{
	dropTime = nextTime + DROP_INTERVAL + delay;
	triggered = true;
	if (delay == 0)
		{
		digitalWrite(CV_GATE_OUT, 1);
		raised = true;
		}
	else if (swing)
		{
		if (oddBeat)
			{
			raiseTime = nextTime + delay;
			raised = false;
			}
		else
			{
			digitalWrite(CV_GATE_OUT, 1);
			raised = true;
			}
		}
	else		// random non-zero delay
		{
		raiseTime = nextTime + delay;
		raised = false;
		}
	oddBeat = !oddBeat;
	nextTime = nextTime + timeInterval;
	}
	
void loop()
    {
    // Potentially raise the trigger
    uint32_t time = millis();
    if (triggered && !raised && time >= raiseTime && time <= dropTime)
    	{
    	digitalWrite(CV_GATE_OUT, 1);
    	raised = true;
    	}

    // Potentially lower the trigger
    if (triggered && time >= dropTime)
    	{
    	digitalWrite(CV_GATE_OUT, 0);
    	triggered = false;
    	}
    
    // Read the inputs
	uint16_t pot3 = analogRead(CV_POT3);
	uint16_t beatDelay = 0;
	swing = 0;
	if (pot3 < 128)
		{
		// do nothing
		}
	else if (pot3 < 512)
		{
		beatDelay = (uint16_t)((timeInterval * (uint32_t)(pot3 - 64)) >> 9);
		swing = 1;
		}
	else if (pot3 < 512 + 64)
		{
		// do nothing
		}
	else // (pot3 < 512)
		{
		beatDelay = (uint16_t)((timeInterval * (uint32_t)random(pot3 - 512 - 64)) >> 9);
		}

	uint16_t pot1 = analogRead(CV_POT_IN1);
	uint16_t pot2 = analogRead(CV_POT_IN2);
    uint16_t newRate = ((pot1 >> 6) * 16) + (pot2 >> 6);
    if (newRate != rate)
    	{
    	if (newRate == 0) // uh
    		{
    		// do nothing, it's determined already through taps
    		}
    	else
    		{
	    	rate = newRate;
		    timeInterval = 60000 / rate;
		    }
	    }

    uint8_t i = digitalRead(CV_IN3);
    if (i == 0) in3_state = 0;
    else if (in3_state == 0) 
    	{
    	in3_state = 1; 
    	if (pot1 < TAP_POT_BOUNDARY && pot2 < TAP_POT_BOUNDARY)
    		{
    		tap();
    		} 
    	}
    
   	i = digitalRead(CV_AUDIO_IN);
    if (i == 0) ain_state = 0;
    else if (ain_state == 0) { ain_state = 1; toggle(); }
        
    // trigger
    if (running)
    	{
    	if (starting)
    		{
    		oddBeat = 0;
    		starting = 0;
    		nextTime = time;			// set it up as the initial time
    		trigger(beatDelay);
    		}
    	else if (time >= nextTime)
			{
			trigger(beatDelay);
			}
    	}
    
    }
    
    