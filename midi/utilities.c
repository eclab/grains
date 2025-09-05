#include "utilities.h"
#include "miditypes.h"

#ifdef __AVR__
#include <avr/pgmspace.h>
#endif

//////// PITCH AND FREQUENCY


#ifdef PITCH_FREQUENCY_UTILITIES
/// Piecewise Linear Interpolation Approach, all integer math

	// "A 440" frequency is MIDI pitch 69. 
	// So frequency is 2^((pitch - 69) / 12) * 440
	// This is 2^(pitch/12 - 69/12) * 440
	// This is 2^(pitch/12) * 2^(-69/12) * 440
	// This is 2^(pitch/12) * 8.1757989156

/// This table comes from 2^(x/12) * 8.1757989156 * 1024, for values x=0...32
#ifdef __AVR__
const PROGMEM UNSIGNED_16_BIT_INT FREQ_0_32[33] =
#else		// __AVR__
const UNSIGNED_16_BIT_INT FREQ_0_32[33] =
#endif		// __AVR__
	{
	8372, 8869, 9397, 9956, 10548, 11175, 11839, 12543, 13289, 14079,
	14917, 15804, 16744, 17739, 18794, 19912, 21096, 22350, 23679, 25087,
	26579, 28159, 29834, 31608, 33488, 35479, 37589, 39824, 42192, 44701,
	47359, 50175, 53152
	};
  
/// This table comes from 2^(x/12) * 8.1757989156 * 1024 / 8, for values x=32...64
/// We divide by 8 to maximize usage of 16-bit integers
#ifdef __AVR__
const PROGMEM UNSIGNED_16_BIT_INT FREQ_32_64[33] = 
#else		// __AVR__
const UNSIGNED_16_BIT_INT FREQ_32_64[33] =
#endif		// __AVR__
	{
	6644, 7039, 7458, 7902, 8372, 8869, 9397, 9956, 10548, 11175, 11839,
	12543, 13289, 14079, 14917, 15804, 16744, 17739, 18794, 19912, 21096,
	22350, 23679, 25087, 26579, 28159, 29834, 31608, 33488, 35479, 37589,
	39824, 42192
	};

/// This table comes from 2^(x/12) * 8.1757989156 * 1024 / 64, for values x=64...96
/// We divide by 64 to maximize usage of 16-bit integers
#ifdef __AVR__
const PROGMEM UNSIGNED_16_BIT_INT FREQ_64_96[33] = 
#else		// __AVR__
const UNSIGNED_16_BIT_INT FREQ_64_96[33] =
#endif		// __AVR__
	{
	5274, 5587, 5919, 6271, 6644, 7039, 7458, 7902, 8372, 8869, 9397,
	9956, 10548, 11175, 11839, 12543, 13289, 14079, 14917, 15804, 16744,
	17739, 18794, 19912, 21096, 22350, 23679, 25087, 26579, 28159, 29834,
	31608, 33488
	};

/// This table comes from 2^(x/12) * 8.1757989156 * 1024 / 256, for values x=96...128
/// We divide by 256 to maximize usage of 16-bit integers
#ifdef __AVR__
const PROGMEM UNSIGNED_16_BIT_INT FREQ_96_128[33] = 
#else		// __AVR__
const UNSIGNED_16_BIT_INT FREQ_96_128[33] =
#endif		// __AVR__
	{
	8372, 8869, 9397, 9956, 10548, 11175, 
	11839, 12543, 13289, 14079, 14917, 15804, 16744, 17739, 18794, 19912, 
	21096, 22350, 23679, 25087, 26579, 28159, 29834, 31608, 33488, 35479, 
	37589, 39824, 42192, 44701, 47359, 50175, 53159
	};


inline const UNSIGNED_16_BIT_INT getFrequencyData(const UNSIGNED_16_BIT_INT* table, unsigned char pitch) 
	{ 
#ifdef __AVR__
	return pgm_read_word_near(&table[pitch]); 
#else		//  __AVR__
	return table[pitch];
#endif		// __AVR__
	}


// interpolates between value #pitch and value #(pitch+1) in the table, using divisions/128
inline UNSIGNED_32_BIT_INT inter(unsigned char pitch, unsigned char divisions, const UNSIGNED_16_BIT_INT* table)
	{
	return getFrequencyData(table, pitch) * (UNSIGNED_32_BIT_INT)(127 - divisions) + getFrequencyData(table, pitch + 1) * (UNSIGNED_32_BIT_INT)divisions;
	}


// Given a MIDI Note (0...127) returns the frequency of the note * 1024
// If you want the frequency as an integer, take this value >> 10
UNSIGNED_32_BIT_INT midiNoteFreq(unsigned char midiNote)
	{
	if (midiNote < 32)
		{
		return getFrequencyData(FREQ_0_32, midiNote);
		}
	else if (midiNote < 64)
		{
		return getFrequencyData(FREQ_32_64, midiNote - 32);
		}
	else if (midiNote < 96)
		{
		return getFrequencyData(FREQ_64_96, midiNote - 64);
		}
	else // if (midiNote < 128)
		{
		return getFrequencyData(FREQ_96_128, midiNote - 96);
		}
	}


// Given a MIDI Note (0...127) plus some number of 1/256 divisions of a semitone
// Returns the frequency of the note * 1024
// If you want the frequency as an integer, take this value >> 10
UNSIGNED_32_BIT_INT midiFreq(unsigned char midiNote, unsigned char divisions)
	{
	if (midiNote < 32)
		{
		return inter(midiNote, divisions, FREQ_0_32) >> 8;
		}
	else if (midiNote < 64)
		{
		return inter(midiNote - 32, divisions, FREQ_32_64) >> 5;
		}
	else if (midiNote < 96)
		{
		return inter(midiNote - 64, divisions, FREQ_64_96) >> 2;
		}
	else // if (midiNote < 128)
		{
		return inter(midiNote - 96, divisions, FREQ_96_128);
		}
	}
#endif


#ifdef BASIC_PITCH_FREQUENCY_UTILITIES


/// This table comes from 2^(x/12) * 8.1757989156 * 4, for values x=0...127
#ifdef __AVR__
const PROGMEM UNSIGNED_16_BIT_INT BASE_FREQ[128] = 
#else		// __AVR__
const UNSIGNED_16_BIT_INT BASE_FREQ[128] =
#endif		// __AVR__
{
32, 34, 36, 38, 41, 43, 46, 48, 51, 54, 58, 61, 65, 69, 73, 77, 82,
87, 92, 97, 103, 109, 116, 123, 130, 138, 146, 155, 164, 174, 184,
195, 207, 219, 233, 246, 261, 277, 293, 311, 329, 349, 369, 391, 415,
439, 466, 493, 523, 554, 587, 622, 659, 698, 739, 783, 830, 879, 932,
987, 1046, 1108, 1174, 1244, 1318, 1396, 1479, 1567, 1661, 1759,
1864, 1975, 2093, 2217, 2349, 2489, 2637, 2793, 2959, 3135, 3322,
3519, 3729, 3951, 4186, 4434, 4698, 4978, 5274, 5587, 5919, 6271,
6644, 7039, 7458, 7902, 8372, 8869, 9397, 9956, 10548, 11175, 11839,
12543, 13289, 14079, 14917, 15804, 16744, 17739, 18794, 19912, 21096,
22350, 23679, 25087, 26579, 28159, 29834, 31608, 33488, 35479, 37589,
39824, 42192, 44701, 47359, 50175
};

// Given a MIDI Note (0...127) returns the frequency of the note * 4
UNSIGNED_16_BIT_INT midiNoteFreq16(unsigned char midiNote)
	{
#ifdef __AVR__
	return pgm_read_word_near(&BASE_FREQ[midiNote]); 
#else		//  __AVR__
	return BASE_FREQ[midiNote];
#endif		// __AVR__
	}

#endif


// Converts an RPN bend range of semitones and cents into N * 1/256 of a semitone
// RPN Bend Range is provided by RPN 0.  Its MSB is the range in semitones, and its LSB is the range in cents
UNSIGNED_16_BIT_INT bendRange(unsigned char rangeSemitones, unsigned char rangeCents)
	{
	return (rangeCents * 64) / 25 + rangeSemitones * 256;
	}

// Given a bend range in N * 1/256 of a semitone, scales a pitch bend amount (-8192 ... +8191) 
// into M * 1/256 semitones (+/-)
SIGNED_16_BIT_INT bendPitch(SIGNED_16_BIT_INT bendAmount, UNSIGNED_16_BIT_INT bendRange)
	{
	SIGNED_32_BIT_INT bend = (bendAmount * (SIGNED_32_BIT_INT) bendRange) / 8192;		// >> 13
	if (bend > 32767) bend = 32767;
	if (bend < -32768) bend = -32768;
	return (SIGNED_16_BIT_INT) bend;
	}
	
// Given a bend pitch in M * 1/256 semitones, and a MIDI note, returns the actual pitch in midi notes and range in N * 1/256 of a semitone, scales a pitch bend amount (-8192 ... +8191) 
// into M * 1/256 semitones.  Does not use 32-bit math, so bends outside MIDI pitch range will be bounded.
void bendFull(unsigned char midiNote, SIGNED_16_BIT_INT bendPitch, unsigned char* newNote, unsigned char* divisions)
	{
	UNSIGNED_16_BIT_INT mn = midiNote * 256;
	if (bendPitch >= 0)
		{
		SIGNED_16_BIT_INT _mn = (SIGNED_16_BIT_INT)(65383 - mn);
		if (bendPitch >= _mn)
			{
			bendPitch = _mn;
			}
		}
	else 
		{
		SIGNED_16_BIT_INT _mn = 0 - (SIGNED_16_BIT_INT)mn;
		if (bendPitch <= _mn)
			{
			bendPitch = _mn;
			}
		}
	UNSIGNED_16_BIT_INT bent = (UNSIGNED_16_BIT_INT)(mn + bendPitch);
	*newNote = bent / 256;
	*divisions = (bent - *newNote * 256);
	}



// divides dividend by 12 and returns the result
UNSIGNED_16_BIT_INT div12(UNSIGNED_16_BIT_INT dividend)
    {
    UNSIGNED_32_BIT_INT invDivisor = 0x1555; 
    UNSIGNED_16_BIT_INT div = (UNSIGNED_16_BIT_INT) ((invDivisor * (dividend + 1)) >> 16);
    return div;
    }

// Given a base MIDI note and a MIDI bend amount, provides a new note
// and the divisions (1/256 semitone) above the new note corresponding to the
// change in pitch if the Pitch Bend corresponds to 1 octave up and 1 octave down.
// These two values can then be passed to midiFreq to get a current frequency
void bend12(unsigned char midiNote, SIGNED_16_BIT_INT bendAmount, unsigned char* newNote, unsigned char* divisions)
	{
	if (bendAmount >= 0)
		{
		unsigned char dNote = (unsigned char)div12((UNSIGNED_16_BIT_INT)bendAmount);
		UNSIGNED_16_BIT_INT residual = (bendAmount - dNote * 12);
		*newNote = dNote + midiNote;
		*divisions = (unsigned char)((residual * 384) >> 10);
		}
	else
		{
		bendAmount = -bendAmount;
		unsigned char dNote = (unsigned char)div12((UNSIGNED_16_BIT_INT)bendAmount);
		UNSIGNED_16_BIT_INT residual = (bendAmount - dNote * 12);
		if (residual == 0)
			{
			signed char n = midiNote - (signed char)dNote;
			if (n <= 0)
				{
				*newNote = 0;
				*divisions = 0;
				}
			else
				{
				*newNote = n - 1;
				unsigned char div = (unsigned char)((residual * 384) >> 10);
				*divisions = 255 - div;
				}
			}
		}
	}





//////// NOTE DISTRIBUTION


/// The crucial feature is how the note distributor's heap works.  The heap is large enough
/// to contain a stack growing up from 0 of all the ALLOCATED CHANNELS, then at least one
/// DISTRIBUTOR_NO_CHANNEL as a buffer space between, and then a stack growing down from n-1
/// of all the UNALLOCATED CHANNELS.  Since the total number of channels is 16, with one buffer
/// space at least, we only need 17 bytes.  When we remove a channel from one or the other stack
/// it shrinks and the buffer space in-between grows.  And when we add a channel the buffer space
/// shrinks, but the two stacks can never touch.  Initially all the channels are in the UNALLOCATED
/// CHANNELS stack.

// Initializes a Note Distributor over the range from minChannel to maxChannel inclusive
void initializeDistributor(struct noteDistributor* distributor, unsigned char minChannel, unsigned char maxChannel)
	{
	for(unsigned char i = 0; i < 16; i++)
		{
		distributor->channelNotes[i] = DISTRIBUTOR_NO_NOTE;
		}
	for(unsigned char i = 0; i < 17; i++)
		{
		distributor->heap[i] = DISTRIBUTOR_NO_CHANNEL;
		}
	unsigned char pos = 16;
	for(unsigned char i = minChannel; i <= maxChannel; i++)
		{
		distributor->heap[pos] = i;
		pos--;
		}
	distributor->minChannel = minChannel;
	distributor->maxChannel = maxChannel;
	}

// Removes the given channel from the allocated channels
unsigned char removeAllocated(struct noteDistributor* distributor, unsigned char channel)
	{
	for(unsigned char i = 0; i < 16; i++)
		{
		if (distributor->heap[i] == DISTRIBUTOR_NO_CHANNEL)  // uh oh
			{
			return DISTRIBUTOR_NO_CHANNEL;
			}
		else if (distributor->heap[i] == channel) // found it
			{
			for(unsigned char j = i; j < 16; j++)
				{
				distributor->heap[j] = distributor->heap[j + 1];
				if (distributor->heap[j] == DISTRIBUTOR_NO_CHANNEL) // all done
					{
					break;
					}
				}
			return channel;
			}
		}
	return DISTRIBUTOR_NO_CHANNEL;		// won't happen
	}

// Removes the oldest allocated channel and returns it
unsigned char removeOldestAllocated(struct noteDistributor* distributor)
	{
	unsigned char channel = distributor->heap[0];
	if (channel == DISTRIBUTOR_NO_CHANNEL) return channel;
	for(unsigned char i = 0; i < 16; i++)
		{
		distributor->heap[i] = distributor->heap[i + 1];
		if (distributor->heap[i] == DISTRIBUTOR_NO_CHANNEL) // all done
			{
			break;
			}
		}
	return channel;
	}
	
// Removes the oldest unallocated channel and returns it.
// If all channels are allocated, returns DISTRIBUTOR_NO_CHANNEL.
unsigned char removeOldestUnallocated(struct noteDistributor* distributor)
	{
	unsigned char channel = distributor->heap[16];
	if (channel == DISTRIBUTOR_NO_CHANNEL) return channel;
	for(unsigned char i = 16; i > 0; i--)
		{
		distributor->heap[i] = distributor->heap[i - 1];
		if (distributor->heap[i] == DISTRIBUTOR_NO_CHANNEL) // all done
			{
			break;
			}
		}
	return channel;
	}

// Inserts the channel at the front of the allocated channels
void insertAllocated(struct noteDistributor* distributor, unsigned char channel)
	{
	for(unsigned char i = 0; i < 16; i++)
		{
		if (distributor->heap[i] == DISTRIBUTOR_NO_CHANNEL) // all done
			{
			distributor->heap[i] = channel;
			break;
			}
		}
	}

// Inserts the channel at the front of the unallocated channels
void insertUnallocated(struct noteDistributor* distributor, unsigned char channel)
	{
	for(unsigned char i = 16; i > 0; i--)
		{
		if (distributor->heap[i] == DISTRIBUTOR_NO_CHANNEL) // all done
			{
			distributor->heap[i] = channel;
			break;
			}
		}
	}

// Allocates an unallocated channel for note and returns the channel.  
// If there are no unallocated channels, returns DISTRIBUTOR_NO_CHANNEL
// If there already is a channel allocated for the given note, then returns
// DISTRIBUTOR_CHANNEL_ALREADY_ALLOCATED.  In this case you can retrieve the
// channel by calling getChannel(distributor, note);
unsigned char allocateChannel(struct noteDistributor* distributor, unsigned char note)
	{
	if (getChannel(distributor, note) != DISTRIBUTOR_NO_CHANNEL)
		{
		return DISTRIBUTOR_CHANNEL_ALREADY_ALLOCATED;
		}
		
	unsigned char channel = removeOldestUnallocated(distributor);
	if (channel == DISTRIBUTOR_NO_CHANNEL)  // all are allocated
		{
		return DISTRIBUTOR_NO_CHANNEL;
		}
	else
		{
		distributor->channelNotes[channel] = note;
		insertAllocated(distributor, channel);
		return channel;
		}
	}
	
// Reallocates the oldest channel for note.  If the channel was unused, returns DISTRIBUTOR_NO_NOTE.
// If the channel was previously used by a note, returns that note.
unsigned char stealChannel(struct noteDistributor* distributor, unsigned char note)
	{
	unsigned char channel = removeOldestAllocated(distributor);
	if (channel == DISTRIBUTOR_NO_CHANNEL)  // all unallocated, unlikely to happen
		{
		return DISTRIBUTOR_NO_NOTE;
		}
	else
		{
		unsigned char oldNote = distributor->channelNotes[channel];
		distributor->channelNotes[channel] = note;
		insertAllocated(distributor, channel);
		return oldNote;
		}
	}

// Returns the channel associated with a given note.
// If there is no such channel, returns DISTRIBUTOR_NO_CHANNEL.
unsigned char getChannel(struct noteDistributor* distributor, unsigned char note)
	{
	for(int i = distributor->minChannel; i <= distributor->maxChannel; i++)
		{
		if (distributor->channelNotes[i] == note)
			{
			return i;
			}
		}
	return DISTRIBUTOR_NO_CHANNEL;
	}
	
// Returns the minimum channel in the channel range
unsigned char getMinChannel(struct noteDistributor* distributor)
	{
	return distributor->minChannel;
	}
	
// Returns the maximum channel in the channel range
unsigned char getMaxChannel(struct noteDistributor* distributor)
	{
	return distributor->maxChannel;
	}
	
// Returns 1 if the channel is between the min and max channels
unsigned char isChannelInRange(struct noteDistributor* distributor, unsigned char channel)
	{
	return (channel >= distributor->minChannel && channel <= distributor->maxChannel);
	}
	
unsigned char oldestChannel(struct noteDistributor* distributor, unsigned char allocated)
	{
	if (allocated)
		{
		return distributor->heap[0];
		}
	else
		{
		return distributor->heap[16];
		}
	}

	
// Returns the note associated with a given channel.
// If there is no such note, returns DISTRIBUTOR_NO_NOTE.
unsigned char getNote(struct noteDistributor* distributor, unsigned char channel)
	{
	if (distributor->channelNotes[channel] >= 128)
		{
		return DISTRIBUTOR_NO_NOTE;
		}
	else
		{
		return distributor->channelNotes[channel];
		}
	} 

// Deallocates the channel associated with a given note and returns it.
// If there is no such channel, returns DISTRIBUTOR_NO_CHANNEL.
unsigned char deallocateChannel(struct noteDistributor* distributor, unsigned char note)
	{
	unsigned char channel = getChannel(distributor, note);
	if (channel == DISTRIBUTOR_NO_CHANNEL)
		{
		return DISTRIBUTOR_NO_CHANNEL;
		}
	distributor->channelNotes[channel] = DISTRIBUTOR_NO_NOTE;
	channel = removeAllocated(distributor, channel);
	if (channel == DISTRIBUTOR_NO_CHANNEL)				// uh, should not happen
		{
		return DISTRIBUTOR_NO_CHANNEL;
		}
	insertUnallocated(distributor, channel);
	return channel;
	}












//// NOTES ON OTHER FREQUENCY METHODS

/***

inline float fastPow(float a, float b) {
  union {
    float d;
    uint32_t x;
  } u = { a };
  u.x = (int)(b * (u.x - 1072632447) + 1072632447);
  return u.d;
}



float pitchToFrequency(float pitch)
{
	// "A 440" frequency is MIDI pitch 69. 
	// So frequency is 2^((pitch - 69) / 12) * 440
	// This is 2^(pitch/12 - 69/12) * 440
	// This is 2^(pitch/12) * 2^(-69/12) * 440
	// This is 2^(pitch/12) * 8.1757989156
	//
	// We could implement this as f = pow(2.0, pitch/12.0) * 8.1757989156
	// But pow is exceptionally costly.  Instead here is an eighth-order
	// curve fit which fits 2^(pitch/12) * 8.1757989156 pretty well for pitch = 0...127
	
	float p1 = pitch;
	float p2 = p1 * p1;
	float p3 = p2 * p1;
	float p4 = p2 * p2;
	float p5 = p2 * p3;
	float p6 = p3 * p3;
	float p7 = p3 * p4;
	float p8 = p4 * p4;
	
	return 8.486937818348999 
		+ 0.11331199053719736 * p2
		- 0.007559879123740043 * p3
		+ 0.00030460111260290676 * p4
		- 6.282585764882156e-6 * p5
		+ 7.529071420682345e-8 * p6
		- 4.713694567546835e-10 * p7
 		+ 1.3267547025263724e-12 * p8;
 	
 	// Here's a ninth order one which fits -12 ... +139 well, for full pitch bends
 	
 	// float p9 = p3 * p3;

	// return 7.974344064453711 
	//     + 0.35274513848781464 * p1
	// 	+ 0.024383433447036966 * p2
	// 	+ 0.0009752497466947595 * p3
	// 	- 0.00008938678230638642 * p4
	// 	+ 3.6438693279032113-6 * p5
	// 	- 6.877210136997593e-8 * p6
	// 	+ 7.304240638407312e-10 * p7
 	// 	+ 4.024698669465269e-12 * p8
 	// 	- 9.851562824801711e-15 * p9;
 	 }





#include <stdio.h>
#include <stdint.h>
#include <math.h>


// From http://www.machinedlearnings.com/2011/06/fast-approximate-logarithm-exponential.html

float fastpow2 (float p)
{
  union { float f; uint32_t i; } vp = { p };
  int sign = (vp.i >> 31);
  int w = p;
  float z = p - w + sign;
  union { uint32_t i; float f; } v = { (1 << 23) * (p + 121.2740838f + 27.7280233f / (4.84252568f - z) - 1.49012907f * z) };
  return v.f;
}

// WARNING: this code has been updated.  Do not use this version.
// Instead, see http://code.google.com/p/fastapprox for the latest version.

// This isn't very good

float
fasterpow2 (float p)
{
union { uint32_t i; float f; } v = { (1 << 23) * (p + 126.94269504f) };
return v.f;
}

// This approximates pow(i/12.0) *  8.1757989156
// pretty well without requiring either pow or division.
// But it's 16 multiplies
float fastPow2_12(float p1)
	{
	float p2 = p1 * p1;
	float p3 = p2 * p1;
	float p4 = p2 * p2;
	float p5 = p2 * p3;
	float p6 = p3 * p3;
	float p7 = p3 * p4;
	float p8 = p4 * p4;
	
	return 8.486937818348999 
		+ 0.11331199053719736 * p2
		- 0.007559879123740043 * p3
		+ 0.00030460111260290676 * p4
		- 6.282585764882156e-6 * p5
		+ 7.529071420682345e-8 * p6
		- 4.713694567546835e-10 * p7
 		+ 1.3267547025263724e-12 * p8;
 }


int main(int argc, char* argv[])
	{
	for(int i = 0; i < 128; i++)
		{
		printf("%d %f %f %f %f\n", i, fasterpow2(i/12.0) * 8.1757989156, fastpow2(i/12.0) * 8.1757989156, fastPow2_12(i), 8.1757989156 * pow(2.0, i/12.0));
		}
	}

***/

