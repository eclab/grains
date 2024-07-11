# GRAINS on AE Modular

This is a set of notes on programming GRAINS that might be useful to you if you're working on a GRAINS project.

## Kinds of GRAINS Modules

There are three versions of the GRAINS AE Modular module in the wild:

- The original GRAINS module had its AUDIO OUT socket attached to Pin 11 of the Atmel 328P as required by the original Ginkosynthese Grains design.

- The revised GRAINS had a switch to set AUDIO OUT to Pin 11, or to Pin 9, as required by the Mozzi Library.  Why the difference?  Because Pin 9 has high-resolution PWM, resulting in much higher quality sound output.  It's not clear why Ginkosynthese used Pin 11 in the first place.

- A prototype version of "GRAINS v2.0", which many people have seen in my rack, exposes all the pins that the original GRAINS versions did not.  This includes a few LEDs, some buttons, more digital ins/outs, and some analog ins.  Critically, hardware serial transmit and receive are exposed as sockets (enabling better MIDI).  Robert has changed the USB port to USB C.  I'd like to see a fourth pot as well but it's not in my prototype, and I think it's probably not going to happen.  I do not know when, or even if, Robert will release a v2.0; he is focusing on other stuff for the time being.  Who knows if it'll happen.  I will not release software for this version until after it's released.

## Coding Options

You have three coding options:

- Use Mozzi.  Mozzi is good for making oscillators, CV generators, and CV converters.  It *can* be used to make audio converters (like filters or effects) but you (and the musician) have to modify your Mozzi library, and the sound is truly terrible.  I'd not generate audio converters.  I have one in my collection, called BIT.

- Use the Ginkosynthese GRAINS library.  I think Mozzi is superior to it in all ways, so I do not use it.  However the Ginkosynthese GRAINS library does work on old GRAINS modules whereas Mozzi does not.

- Code it yourself.  If your output is digital, you easily can build it yourself without any library.  Drum triggers, MIDI utilities, clocks, gate generators, etc.  But if you need to output a real-valued signal, be it CV or audio, you'll need to use Mozzi or Ginkosynthese GRAINS, which handle the complexities of outputting filtered PWM.


## Licenses

Most of my projects are licensed as Apache 2.0, which isn't viral, so you can mostly use them as you like as long as you retain the license.  Read the license to verify.

Some of my projects are GPL because they use viral GPL code and thus must be GPL.

Mozzi used to be distibuted under a broken, viral, non-open-source license: Creative Commons CC-BY-NC-SA 4.0.  This license was not meant for code and has all manner of problems.  Thankfully, Mozzi has seen the light and is now redistributed as LGPL.

The Arduino Library is LGPL.

All code below is licensed as Apache 2.0.

## Explicit Numerical Types

C's numerical types are vague.  Is a short 16-bit or 32-bit?  Is an int 16-bit, 32-bit, or 64-bit?  Is a char unsigned?  And so on.  

Because the Arduino is an 8-bit machine with 16-bit and 32-bit options, its numerical type sizes are different from what you'd see on a 32-bit machine.  Because the size of the type matters so much in signal processing, it's best not to refer to int, short, long, or char at all.  Instead, you should refer to:

- uint8\_t (an unsigned 8-bit value: an unsigned byte)
- int8\_t (a signed 8-bit value: a signed byte)
- uint16\_t (an unsigned 16-bit value)
- int16\_t (a signed 16-bit value)
- uint32\_t (an unsigned 32-bit value)
- int32\_t (a signed 32-bit value)
- float (a 32-bit floating-point value)

Get used to using these religiously in Arduino code: they'll make your life much easier, especially for coding for GRAINS.

You can use bool to describe boolean values, but it's just the same thing as a uint8\_t, and false is just the same thing as 0 while true is the same thing as 1 (or non-zero).  I don't bother with that.

## Math Tips

GRAINS is nothing more than an Atmel 328P: it's basically an Arduino Nano.  If you're experienced with coding on the Arduino, then you're more than halfway there.  But here are a few tips.

### Floating Point

The 328P does not have floating point arithmetic in hardware.  You should avoid floating-point when possible (and it's not always possible), as arithmetic with it will be very costly since it will have to be done entirely in software.

### Integer Division and Right Shifts

The 328P does not have integer division.  Integer division is very expensive as it must be done in software.  You can divide by constant powers of 2, which GCC ought to convert to right shifts, thus the following are equivalent:

    a = x / 8;
    a = x >> 3;
    
This sounds great, but there's a catch: the 328P can only right-shift by **one** in assembly code.  So to shift by 3, it must shift by 1 three times.

This isn't so bad: for example, shifting by 9 is just a shift by 8 (moving a byte over) followed by a shift by 1.  And shifting by 4 can be done with a special swap instruction.  Unfortunately GCC doesn't do many of these tricks yet.  Still, better than nothing.

If you want to divide by constants OTHER than powers of 2, it might make sense to use certain classic tricks:

    
    // Range of 0...65535
    uint16_t div3(uint16_t dividend)
        {
        uint32_t invDivisor = 0x5555;
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

    // Range of 0...16385
    uint16_t div6(uint16_t dividend)
        {
        uint32_t invDivisor = 0x2AAA;
        uint16_t div = (uint16_t) ((invDivisor * (dividend + 1)) >> 16);
        return div;
        }
    
    // Range of 0...32773
    uint16_t div7(uint16_t dividend)
        {
        uint32_t invDivisor = 0x2492;
        uint16_t div = (uint16_t) ((invDivisor * (dividend + 1)) >> 16);
        return div;
        }

    // Range of 0...9369
    uint16_t div9(uint16_t dividend)
        {
        uint32_t invDivisor = 0x1C71; 
        uint16_t div = (uint16_t) ((invDivisor * (dividend + 1)) >> 16);
        return div;
        }
            
    // Range of 0...9369
    uint16_t div10(uint16_t dividend)
        {
        uint32_t invDivisor = 0x1999;
        uint16_t div = (uint16_t) ((invDivisor * (dividend + 1)) >> 16);
        return div;
        }

	// 20% slower version, but Range of 0...43690 
    uint16_t div10slow(uint16_t dividend)
        {
        uint16_t div = ((dividend >> 1) + dividend) >> 1;
        div = ((div >> 4) + div); 
        div = ((div >> 8) + div) >> 3;
    
        uint16_t remainder = ((div << 2) + div) << 1;
        remainder = dividend - remainder;
        if (remainder >= 10) 
            {
            remainder = remainder - 10;
            div = div + 1;
            }
        return div;
        }

    // Range of 0...16391
    uint16_t div12(uint16_t dividend)
        {
        uint32_t invDivisor = 0x1555; 
        uint16_t div = (uint16_t) ((invDivisor * (dividend + 1)) >> 16);
        return div;
        }
    
    uint32_t div100(uint32_t n) 
        {
        uint32_t q, r;
        q = (n >> 1) + (n >> 3) + (n >> 6) - 
            (n >> 10) + (n >> 12) + (n >> 13) - (n >> 16);
        q = q + (q >> 20);
        q = q >> 6;
        r = n - q * 100;
        // return q + ((r + 28) >> 7);    // an alternative
        return q + (r > 99);
        }
        
Note that these all use 32-bit integers here and there.

### Scaling Without Division

One of the most common tasks you'll find yourself doing is scaling the analog inputs, which have a range 0...1023, into something smaller.  If you were scaling into another power of 2, you could always do division or shifts.  For example, scaling 0...1023 to 0...3 is just dividing by 256, otherwise known as shifting right by 8:

    x = x / 256;    // or
    x = x >> 8;
    
If you need to scale by some other constant, the best way to do it is to MULTIPLY by the constant, then DIVIDE by 1024.  For example, to scale to 0...5, you'd do:

    x = (x * 6) / 1024;		// or
    x = (x * 6) >> 10;
 
Note that an unsigned 16-bit integer has is at most 64 times larger than 1024.  So if you needed to scale to (say) 0...99, you don't have enough headroom.  In this case, divide the 1024 by 2 to give you enough headroom, like this:

    x = ((x / 2) * 100) / 512;   // or
    x = ((x >> 1) * 100) >> 9; 


### Right Shifts with Signed Integers

Work just like you'd want them to.  You'll often scale 16-bit signed audio down to 8-bit etc. this way.

### Integer Sizes

The 328P is an 8-bit processor with only one or two instructions to assist in 16-bit arithmetic.  16-bit and 32-bit arithmetic are faked with multiple 8-bit instructions.  Generally they're 2x and 4x the cost of 8-bit arithmetic (you can google to find exact timings for Arduinos).  You want to stay 8-bit (uint8\_t) as much as you can, but you'll be doing a variety of things in 16-bit (uint16\_t).  Sometimes you have no choice but to bump to 32-bit (uint32\_t) but you want to drop out of it as soon as you can.

### Fixed-Point Arithmetic

Mozzi makes a lot of noise about its (very limited) fixed-point arithmetic package as a way to get much of the value of floating-point arithmetic with much less computational cost.  But fixed-point arithmetic is really simple.  It's just integer arithmetic where you imagine your integers as being some power of two times your decimal (or fractional) values.

For example, let's say you need two decimals worth of precision (100).  The power of two that encompasses that is 7 bits or 128.  So let's do 128.  In this case if you have a 16-bit number, you have 9 bits left for the integer portion, or 512.  So your biggest value is 511 + 127/128 or just below 512.

So how do you do this?  Let's say you have 92.42.  You convert it to your fixed-point number as:

    uint16_t fixed1 = (uint16_t) (92.42 * 128);

128 is the **scaling factor**.  It's the size of your fractional range.  

You can add two fixed point numbers together, and you can multiply a fixed point number by a normal integer.  To multiply two fixed point numbers together, you'll wind up with a fixed-point number with a squared scaling factor (128 * 128 = 16384).  You'll ultimately need to divide it by 128 to get it back down to 128 again.

    uint32_t fixed3 = (fixed2 * fixed1) / 128;   // or
    uint32_t fixed3 = (fixed2 * fixed1) >> 7;
    
Note that even though we're dividing by 128, by multiplying two 16-bit integers we're going to go into a 32-bit integer range.

You can get the integer portion of the fixed number by dividing by the scaling factor (128), as in:

	uint16_t intportion = fixed1 / 128; 	// or
	uint16_t intportion = fixed1 >> 7;
	
From there you can get the "fractional" portion so to speak.  Our "fractional" portion is parts of 128, so you we can say:

	uint16_t fracportion = fixed1 & 127;

This will be a value 0...127.

To go back to floating-point, you can do:

	float result = fixed1 / 128.0;		// Note 128.0 is a float

The examples I gave were for unsigned fixed-point numbers.  But you can totally do signed numbers.  But you should be consistent. And watch out for rolling over in addition or subtraction, you don't want that.

Anyway, the point is that fixed-point arithmetic isn't remotely as complex and mysterious as Mozzi's package makes it out to be.

### Floating Point Exponentiation

POW and EXP are unbelievably expensive operations, potentially thousands of times more expensive than a multiply.  Unfortunately, to convert pitch to frequency, you have to do them.  But you can't afford it.

For example, let's say that your starting pitch is called 0 (perhaps it's C2, which is 32.703Hz).  You go up N semitones from there.  What's the frequency of pitch N?  

    pow(2.0, n/12.0) * 32.703
    
Ugh, exponentiation of 2 by a floating-point value.  You can't do that realistically.

Instead, consider a lookup table.  For example, in many of my GRAINS oscillators, I have a table called FREQUENCIES.  It's a big table of 1536 values starting at 32.703.    It rises with fractions of pitches.  Specifically, AE Modular is 5V, thus 5 octaves, and since 5V on the arduino is 1023, C to C 5 octaves above (inclusive) would be 0...1024.  So my 5 octaves of pitches range 0...1023. I just look it up in the table.  The table is bigger than this to handle 2.5 octaves of additional transposition in case I need it.

You can't store 1536 32-bit floating-point values in 2K of RAM.  What to do?  You store it in the code space, using PROGMEM.  We'll discuss that later.

### Floating Point Sine

Sine is also far too expensive.  It's best to just store it as a lookup table.  I'd store it as a lookup table of 8-bit or 16-bit integers.  Mozzi has some tables like that if you don't want to generate them yourself.

### Random Number Generation

Arduino's random number generator is really bad.  It'll do for noise but it's not amazing.  It's also a bit pokey.  The traditional way to randomize is to call randomSeed(...) on an analog pin.  The best pin choice for this in GRAINS is A5.  You'd do this in setup().

Another way to get different random seeds each time you start up GRAINS is to load an unsigned 16-bit integer from the EEPROM, set it to 1 if it's 0, call randomSeed(...) on it, then increment it and save it back to EEPROM. 

If you need a faster random() function, Mozzi has an xorshift library which will do okay.

## PROGMEM

PROGMEM is a C macro which allows you to store constant data in the 32K code space of the 328P.   Any large const table or array should be stored this way.  This includes sound waves, wavetables, pitch to frequency lookup tables, you name it.

Mozzi instead uses a macro called CONSTTABLE\_STORAGE.  This allows Mozzi to switch to the proper const storage mechanism depending on the platform.  For the 328P it just gets expanded to PROGMEM.

PROGMEM access is slightly slower than accessing RAM, but it's not bad at all.  You should use it liberally.

To get the PROGMEM facility, you can do 

    #include <avr/pgmspace.h>
    
If you're doing Mozzi, it already includes this file, so you don't need to.  

You create a PROGMEM table of uint8_t like this:

    const PROGMEM uint8_t myTable[5] = { 1, 2, 3, 4, 5 };
    
You can make tables of any numerical type (uint16\_t, int8\_t, int32\_t, float, whatever).  

To access an element, you can't access it directly -- it's in a completely different memory space.  You have to use a special function, such as pgm\_read\_byte\_near(...).  This function takes a pointer to an element in the table (like where you'd expect the data), and looks it up in *code space*.  To do all this, I would make a little macro.

    #define getMyTable(index) ((uint8_t) pgm_read_byte_near(&myTable[index]))

Notice I'm casting the byte into the type I want (uint8\_t in this case).  Now you can access elements in your table like this:

    uint8_t element2 = getMyTable(2);	// should return a 3

To get a 16-bit integer you need to use pgm\_read\_word\_near(...), which casts into the signed/unsigned form you want (here, int16\_t).

    #define getMySecondTable(index) ((int16_t) pgm_read_word_near(&mySecondTable[index]))

To get a 32-bit integer you need to use pgm\_read\_dword\_near(...), which casts into the signed/unsigned form you want (here, int32\_t).

    #define getMyThirdTable(index) ((int32_t) pgm_read_dword_near(&myThirdTable[index]))

To get a float you need to use pgm\_read\_float\_near(...)

    #define getMyFourthTable(index) pgm_read_float_near(&myFourthTable[index])
 
Notice the term "near".  There are also "far" versions of these functions.  They are for much larger tables, in 32-bit addressed spaces.  You don't have that much memory on the GRAINS, it's only 32K, which fits into 16-bit addressing.  So don't worry about them unless GRAINS is upgraded to a bigger chip.

It is possible to do other stuff, like tables of const stucts, or tables of const pointers, but it gets complex and I'd avoid it.  Notably you can hack things up to do multdimensional tables.  I've done that occasionally.

## Analog and Digital Pins

GRAINS has seven inputs and outputs.  They're commonly defined as follows:

	#define CV_POT_IN1    A2    
	#define CV_POT_IN2    A1   
	#define CV_POT3       A0 
	#define CV_IN3        A3
	#define CV_AUDIO_IN   A4 
	#define CV_AUDIO_OUT  9 // On Mozzi.  On Ginkosynthese GRAINS, it's 11
	#define CV_GATE_OUT   8  
	
To this collection I typically add the following, which I use to seed random number generators (see the math section earlier).
	
	#define RANDOM_PIN    A5

- IN 1.  This is attenuated by POT1 or alternatively replaced by POT1.  Note that when attenuated, if you set POT1 to about the 2 o'clock position, IN1 will be maximal.  Higher than that and you're just multiplying IN1 by larger values, but they're clipped at 1023 in the Arduino.  I consider this a bug.

- IN 2.  This is just like IN1 (with its own POT2), and with the same bug.

- IN 3.  This is a pure CV input.  It's filtered a bit.

- Pot 3.  This is the input from potentiometer 3.  Pot 3 and IN 3 have nothing to do with each other.

- Audio In.  This input can be used for CV input, and when using a library such as Mozzi, you can modify the library to allow this to be used for audio input (badly).  The problem with Audio In is that its impedance is very different from the other inputs, and so it takes longer for the analog pin to read voltage.  It takes so long in fact that analogRead(...) often won't work right.  If you do an analogRead(...) on IN3 and then do an analogRead(...) on AudioIn, often AudioIn will be just set to what IN3 was set to!  You have to throw that away and do a *second* analogRead(...) on AudioIn to get what you need.  Note that you don't have this option in Mozzi.  Mozzi does all the analog reading before you even get to their special analog read functions, which just report what it read.  So you'll have to hope for the best there.

- Audio Out.  This is a digital output which goes through a low-pass filter.  This means it's slow if you use it as a digital out.  It's set up like this so that libraries like Mozzi can use it to do filtered PWM.  If you're using Mozzi (etc.), you'd not output on Audio Out, you let Mozzi handle it.  If you're writing your own code, you can use Audio Out as a slow digital out.

- Gate ("Digital") Out.  This is the one digital pin that has nothing special messing with it.  It's fast.  I use it as MIDI In for my MIDI-based oscillators (it's the only one that works right).

Though these pins are by default meant for certain input and output functions, they in fact can be changed to be used for different purposes:

- IN 1.  Can be used for analog or digital input, keeping in mind that POT2 is attenuating the voltage.
 
- IN 2.  Can be used for analog or digital input, keeping in mind that POT2 is attenuating the voltage.

- IN 3.  Can be used for analog or digital input, or for digital output (but a bit slow due to the filter).
 
- POT 3.  Can be used for analog input only.

- Audio In.  Can be used for analog or digital input, or for digital output (but a bit slow due to the filter).

- Audio Out.  Can be used for analog (via filtered PWM) or digital output.  Digital output will be slow due to the filter.

- Gate ("Digital") Out.  Can be used for digital input or output.

This will be strange to users, seeing OUTPUTS on the left side and INPUTS on the right, but it is often necessary.

You can a pin to be output or input with pinMode(...).  Do this in setup(...). 

### Using Analog Inputs as Digital

You can use digitalRead(...) on analog pins to read them as if they were digital pins.  This just does an analogRead(...) and tests to see if it's over 512.  This isn't very good because there's noise in analogRead(...), so if you're near 512, it may drop down again for a bit, then rise again, resulting in multiple on/off readings.  You don't want that.

Instead you might consider doing something like this to avoid noise:
 
    #define HIGH 612
    #define LOW 412
    
    /// This part is inside a function
    uint16_t val = analogRead(pin);
    if (val >= 612)
        {
        // I am definitely high
        // do stuff here
        }
    else if (val < 412)
        {
        // I am definitely low
        // do stuff here
        }

You can also use this approach to trigger on *going* high or low, rather than *being* high or low:

    #define HIGH 612
    #define LOW 412
    uint8_t state = 0;
    
    /// This part is inside a function
    uint16_t val = analogRead(pin);
    if (state == 0 && val >= 612)
        {
        // I just went high
        // do stuff here in response
        state = 1;
        }
    else if (state == 1 && val < 412)
        {
        // I just went low
        // do stuff here in response
        state = 0;
        }
        
### About Audio Out

Audio Out is a digital pin which the Mozzi and Ginkosynthese GRAINS libraries set to have its PWM turned on.  The result is then pushed through a filter to approximate a DAC.

In Mozzi, this filtered PWM is designed for audio rates, not CV.  However it can be used for CV, but is affected by the impedance of the item its connected to.  This particularly causes problems if the CV output is meant to be volt per octave for pitches.   For example, let's say that you are using Mozzi to output CV values from 0...5V corresponding to notes.  Depending on the oscillator you attach this CV to, the oscillator may pull significant amperage from Audio Out, resulting in it scaling down its voltage.  Instead of outputting 3.2 volts you might instead output 3.1 volts.  This would be fine (you could write a table to adjust for it) except that many oscillators are different.  For example, the 555 oscillator has a built-in op-amp and doesn't cause this much.  But the VCO has significant issues.

I have found that GRAINS + Mozzi filtered PWM has problems in audio as well.  Notionally, filtered PWM has an amplitude range of -244 ... +243, corresponding to voltages from 0 to 5V.  Because this is between 8 and 9 bits, Mozzi calls this "8.5 Bit".  Ideally you'd like to scale your audio from (typically) -128 ... +127 to   -244 ... +243 in order to make GRAINS as loud as possible to reduce noise.  However I have found that the upper portion of this range is significantly distorted above +175.  So realistically you can only do -244 ... +175 for CV, or if you want to keep your audio properly centered at 2.5V, you can only do -175 ... +175.  I wrote a few functions you can use in Mozzi's updateAudio(...) function to do this scaling:

    /** Maps -128 ... +127 to -168 ... +167 */ 
    inline int16_t scaleAudioSmall(int16_t val)
    	{
    	return (val * 21) >> 4;
    	}
    	
    /** Maps -128 ... +127 to -244 ... +170 */ 
    inline int16_t scaleAudioSmallBiased(int16_t val)
    	{
    	return ((val * 13) >> 3) - 36;
    	}
    
    /** Maps -32768 ... +32767 to -168 ... +167 */ 
    inline int16_t scaleAudio(int16_t val)
    	{
    	return ((val >> 5) * 21) >> 7;
    	}
	
    /** Maps -32768 ... +32767 to -244 ... +171 */ 
    inline int16_t scaleAudioBiased(int16_t val)
    	{
    	return (((val >> 4) * 13) >> 7) - 36;
    	}

The main problem with these functions is that, though the math is not complex (it's as simple as I can make it), it can be complex enough to be too slow for Mozzi if you're doing lots of other stuff to generate the audio.  You have two other options:

- Just output in the range -128 ... +127.  This is quieter though.

- Provide a gain knob.  Let's say that POT 3 was your gain knob, and you read it earlier in updateControl(...) and scaled it down to 0...7, where 4 is identity.  You might do:

        // Scales -128...+127 to whatever, given pot3 gain.  pot3 goes 0...7
        inline int16_t scale(int16_t val)
        	{
        	return (val * pot3) >> 2;
        	}
        	
 Obviously if the musician turns the knob up too much, this will produce values outside the -244 ... +243 range and clip.  But that's to be expected.
 
## Tips for Using Mozzi

The big challenge in using Mozzi lies in its updateControl(...) method.  This method is called every once in a while so you can read in analog values etc. from the pots or incoming CV.  In contrast, updateAudio(...) is called constantly.

You specify how often updateControl(...) is called using the CONTROL\_RATE #define, which must be set before you #include Mozzi.  For example:

    #define CONTROL_RATE 128

This says that updateControl(...) will be called 128 times a second.  Note that updateAudio(...) is being called 16384 times a second (GRAINS's sampling rate on Mozzi), so this means updateControl(...) is being called every 128 updateAudio(...) calls.

Mozzi suggests that CONTROL\_RATE should be a power of two, though I have found non-powers-of-two to work okay.

If CONTROL\_RATE is too slow, then your program will be too slow to respond well to incoming note and CV changes.  If you're doing notes for pads, you might be able to get away with 64.  If you don't need the knobs or CV inputs to respond in real-time at all, you could do even slower.  But if you're doing triggers for drum notes, you want something fast, ideally even 256 if you can get away with it.

However, updateControl(...) takes time away from updateAudio(...), and this is reflected in a soft but infuriating whine you'll hear in the output.  The pitch of the whine depends on the setting of CONTROL\_RATE and on how computationally expensive updateControl(...) is, that is, how much time you spend in it.  You want to make updateControl(...) short if possible, but computing pitch to frequency conversion, etc., is expensive.

And if the CONTROL\_RATE is too high, you'll start getting audible clicks in your sound due to updateAudio(...) sitting and waiting for updateControl(...) to finish.

The Filtered PWM is also not very good for soft sounds.  If you've got a large amplitude sound wave, it's great.  If it dwindles to a very soft wave, you'll hear weird whiny and noisy distortion.  

MIDI applications have a big advantage over non-MIDI applications here, because you don't have to poll for changes in pitch in every call to updateControl(...), so you don't have to do mozziAnalogRead(...) and do frequency conversion etc.  Instead, you just check for incoming MIDI messages, and if there are none, you're done.  You only change pitch when a MIDI message comes in.  This makes MIDI both simpler and faster, which is valuable here (and pitch is stable, so your frequency table can be very small, just one frequency for each semitone).

updateAudio(...) must be completed very quickly.  If it's done too slowly, it can't keep up with the sampling rate and you'll get distortion, whines, clicks, etc.  It's the most time-sensitive function by far.

Mozzi uses the Oscil, MetaOscil, and Sample classes to define oscillators which play sounds.  These classes have weaknesses.  For one, thanks to Mozzi's use of C++ templating, you will find it difficult or impossible to put these classes into arrays.  Second, the classes chew up a lot of memory, seemingly unecessarily.  So beyond the tables you have to load for waves, you have to consider the memory cost of each Oscil or each MetaOscil.  You will find that you can at most drive about 6 MetaOscil or 9 Sample instances at most, and fewer instances will sound better and cause less whining in updateAudio(...).

## MIDI

MIDI is achieved by setting up a serial port at 32150 BPS and reading from it.  I have a package called "MIDI" in my collection which is designed to make it easy to parse MIDI and respond to messages.  It's set up so that you can turn off everything you don't want, which automatically makes the code in the library smaller and faster.

MIDI has a big advantage over Gate/CV in GRAINS because you only update pitch and volume information etc. when MIDI messages arrive, as opposed to constantly polling gate/CV information in every call to updateControl(...).  As a result, many of my MIDI apps run at a CONTROL\_RATE of 256, which is pretty good.

The Arduino can do serial in two ways. First, it has a single UART (hardware to handle Serial).  These are pins 0 (Receive) and 1 (Transmit).  A UART is the very best option -- it's fast and has a buffer and doesn't hold up anything while it's doing its work.  It's perfect.  Unfortunately, these pins are not exposed as sockets.  They do connect to the USB serial connection, so you can use the UART to read MIDI sent to it over serial on USB.  I have a program called Dave.java in my DAVE package in my collection which can help transferring MIDI over serial from your DAW etc.

The other option is software serial.  There are several software serial packages available for the Arduino, and they are all bad.  Notably *writing* MIDI is very bad because it holds up everything while it's writing -- including incoming MIDI data to the software serial, which will then get dropped on the floor before we can read it.

The fastest software serial option that works for GRAINS is NeoSWSerial.  You can find it in any of my MIDI packages, such as PARA-M.  If you're making a MIDI-based oscillator, you'd read on DIGITAL OUT (GATE OUT), which is by far your best pin option.  It's all set up with my MIDI parser like this:

    #include "NeoSWSerial.h"
    #include "parsemidi.c"
    #define MIDI_RATE 31250
    #define BLANK_SERIAL 5		// I will get rid of this, I modified NeoSWSerial to do something...
    #define PIN_UNUSED 255
    
    NeoSWSerial softSerial(CV_GATE_OUT, BLANK_SERIAL, PIN_UNUSED);
    midiParser parse;

Next you follow the instructions in parsemidi.c and parsemidi.h to set up MIDI how you'd like.  Then in setup() you do:

    initializeParser(&parse, OMNI, 0, 0);   // or pick a channel
    softSerial.begin(MIDI_RATE);
    
Then you define some MIDI functions according to what you turned on in parsemidi.c/h, for example 

    void noteOn(midiParser* parser, unsigned char note, unsigned char velocity)
        {
        // update pitches here...
        }

If and when we get an upgraded GRAINS which exposes the hardware UART transmit and receive ports, then we'll be able to do highly reliable MIDI over serial.  But right now all we have is software serial.

## EEPROM

The GRAINS has 1K of EEPROM.  This allows you to store data that survives power cycling and then reload it.  I use it to store sequencer note data for example.  It's particularly useful to store preferences, or perhaps stored patch data (such as CCs) sent over MIDI.

You should read up about how to use the EEPROM in the Arduino documentation.  It's important that you never use EEPROM.write().  Instead, always use EEPROM.update(), which will help keep the EEPROM from burning out prematurely.

Never call EEPROM.update() in a loop or call it every time you have updateControl() called etc.  It should be called very sparingly, such as ONCE when a user needs to save something.  Remember that the EEPROM has roughly 100,000 write cycles.