// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License

/// SCOPE
///
/// Scope is a simple analog and digital oscilloscope using the Arduino IDE.   Scope 
/// is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// SET GRAINS TO GRAINS MODE.  (If you want MOZZI mode, change CV_AUDIO_OUT to 9)
///
/// You will need to install the Arduino IDE, but you already have done that, right?
///
/// Scope uses the Arduino IDE's Serial Plotter functionality.  Do the following:
///
/// 1. Attach your GRAINS to your laptop
/// 2. Select the USB Port
/// 3. Call up the Serial Plotter (it's under the Tools menu)
/// 4. Set the Baud Rate to 250000
/// 5. Watch in wonder!
///
/// You change the rate of the oscilloscope by turning POT 3.  
///
/// Scope lets you hook up Inputs 1, 2, 3, and A as either analog or digital inputs,
/// D as a digital input, and Out as a little digital function generator.
///
/// By default, Inputs 1 and 2 are set up as analog inputs.  You should set their switches
/// to IN1 and IN2 respectively, and tune Pots 1 and 2 to approximately the 2 0'clock position
/// so they properly run from min (0) at 0 V to max (1023) at 5V.
///
/// By default, Inputs 3 and A are set up as DIGITAL inputs, because they don't have tuning
/// knobs. You can change them to analog inputs, but beware that they will hit 1023 at less
/// than 5V (approximately the 3 o'clock position on a knob from a 4ATT/Mix for example).
/// You'll have to eyeball it.
///
/// D is always a digital input.
///
/// Out is always a digital function generator.  You can change what it outputs.  By default
/// it outputs 0V.
///
/// CHANGING THE INPUTS AND OUTPUTS IN REAL TIME FROM YOUR LAPTOP
///
/// It's easy!  In the Serial Plotter, just type one of the numbers or letters below and hit RETURN:
///
/// 1       Toggles Input 1 between analog and digital
/// 2       Toggles Input 2 between analog and digital
/// 3       Toggles Input 3 between analog and digital
/// a or A  Toggles Input A between analog and digital
/// h or H  Sets Out to generate 5V ("H" for High)
/// l or L  Sets Out to generate 0V ("L" for Low)
/// s or S  Sets Out to generate a square wave.  No, you can't set the frequency.
///
///
/// CHANGING THE BAUD RATE
///
/// The baud rate is by default set to 250000, the fastest rate that Serial Plotter operates
/// on my laptop (a Macbook M1).  If you are not having luck with this rate, try something lower.
/// An obvious choice would be 115200, a common baud rate for many machines.

#define BAUD_RATE 250000      // Try changing 250000 to 115200




#define CV_POT_IN1    A2 
#define CV_POT_IN2    A1
#define CV_POT3       A0
#define CV_IN3        A3
#define CV_AUDIO_IN   A4
#define CV_AUDIO_OUT  11
#define CV_GATE_OUT   8
#define RANDOM_PIN    A5

void setup() 
	{
 	Serial.begin(BAUD_RATE);
 	pinMode(CV_GATE_OUT, INPUT);
 	pinMode(CV_AUDIO_OUT, OUTPUT);
 	digitalWrite(CV_AUDIO_OUT, 0);
	}

uint8_t digitalIn1 = 0;
uint8_t digitalIn2 = 0;
uint8_t digitalIn3 = 1;
uint8_t digitalAud = 1;

void loop() 
	{
	uint16_t in1;
	uint16_t in2;
	uint16_t in3;
	uint16_t aud;
	uint16_t dig;
	
	if (digitalIn1) in1 = digitalRead(CV_POT_IN1) * 1023;
	else { in1 = analogRead(CV_POT_IN1); }
	if (digitalIn2) in2 = digitalRead(CV_POT_IN2) * 1023;
	else { in2 = analogRead(CV_POT_IN2); }
	if (digitalIn3) in3 = digitalRead(CV_IN3) * 1023;
	else { in3 = analogRead(CV_IN3); }
	analogRead(CV_AUDIO_IN);		// high impedance throwaway
	if (digitalAud) aud = digitalRead(CV_AUDIO_IN) * 1023;
	else { aud = analogRead(CV_AUDIO_IN); }
	dig = digitalRead(CV_GATE_OUT) * 1023;
		
  uint16_t rate = (analogRead(CV_POT3) * 13) >> 10; // 0...12   (delay: 0...4096)
  if (rate > 0) delay(1 << (rate - 1));
	uint8_t first = true;
  Serial.print("1:"); Serial.print(in1); 
  Serial.print(",2:"); Serial.print(in2); 
  Serial.print(",3:"); Serial.print(in3); 
  Serial.print(",A:"); Serial.print(aud); 
  Serial.print(",D:"); Serial.print(dig); 
  Serial.print(",Max:1023,Min:0,");
  Serial.println();

	// Read
	if (Serial.available() > 1)
		{
		char byte = Serial.read();
    if (byte == '\n' || byte == '\r') return;
		else if (byte == '1') digitalIn1 = !digitalIn1;
		else if (byte == '2') digitalIn2 = !digitalIn2;
		else if (byte == '3') digitalIn3 = !digitalIn3;
		else if (byte == 'a' || byte == 'A') digitalAud = !digitalAud;
		else if (byte == 'h' || byte == 'H') digitalWrite(CV_AUDIO_OUT, 1);
		else if (byte == 'l' || byte == 'L') digitalWrite(CV_AUDIO_OUT, 0);
		else if (byte == 's' || byte == 'S') analogWrite(CV_AUDIO_OUT, 128);
		}
	}
