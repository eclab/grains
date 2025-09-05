#ifndef __MIDI_UTILITIES_H
#define __MIDI_UTILITIES_H

#include "miditypes.h"


/// PITCH TO FREQUENCY CALCULATION

// To use midiNoteFreq or midiFreq, you need to uncomment this, which allocates its interpolation table:
#define PITCH_FREQUENCY_UTILITIES

// To use midiNoteFreq16, you need to uncomment this, which allocates its interpolation table:
#define BASIC_PITCH_FREQUENCY_UTILITIES


#ifdef PITCH_FREQUENCY_UTILITIES

// Given a MIDI Note (0...127),
// Returns the frequency of the note * 1024
// For example, midiNoteFreq(127) returns 12844800, which is Floor[12543.853951348918 * 1024].
// The frequency of Midi Note 127 is 12543.853951348918.   This is identical accuracy as midiNoteFreq16
// for high pitches, but is 256x as accurate for low pitches.
// Using this function will require 264 bytes of static ROM space (PROGMEM) for the
// interpolation table.

UNSIGNED_32_BIT_INT midiNoteFreq(unsigned char midiNote);


// Given a MIDI Note (0...127) plus some number of 1/256 divisions of a semitone,
// Returns the frequency of the note * 1024 as a 32-bit integer
//
// This function instead uses linear interpolation in an approximation lookup table 
// and all integer math.
// Using this function will require 264 bytes of static ROM space (PROGMEM) for an
// interpolation table.

extern UNSIGNED_32_BIT_INT midiFreq(unsigned char midiNote, unsigned char divisions);
#endif


#ifdef BASIC_PITCH_FREQUENCY_UTILITIES

// Given a MIDI Note (0...127) returns the frequency of the note * 4 as the nearest 16-bit integer
// For example, midiNoteFreq16(127) returns 50175, which is Floor[12543.853951348918 * 4].
// The frequency of Midi Note 127 is 12543.853951348918.
// This is just a 16-bit table lookup, and so is fast, but is less accurate than
// midiNoteFreq(midiNote) by as much as 256x for low pitches.
// Using this function will require 256 bytes of static ROM space (PROGMEM) for lookup table.

UNSIGNED_16_BIT_INT midiNoteFreq16(unsigned char midiNote);

#endif


/// PITCH BEND

// Converts an RPN bend range of semitones and cents into N * 1/256 of a semitone
// RPN Bend Range is provided by RPN 0.  Its MSB is the range in semitones, and its LSB is the range in cents
// Requires one integer division (by 25) at present.
UNSIGNED_16_BIT_INT bendRange(unsigned char rangeSemitones, unsigned char rangeCents);

// Given a bend range in N * 1/256 of a semitone, scales a MIDI pitch bend amount (-8192 ... +8191) 
// into M * 1/256 semitones (+/-)
SIGNED_16_BIT_INT bendPitch(SIGNED_16_BIT_INT bendAmount, UNSIGNED_16_BIT_INT bendRange);

// Given a bend pitch in M * 1/256 semitones, and a MIDI note, returns the actual pitch in midi notes and range in N * 1/256 of a semitone, scales a pitch bend amount (-8192 ... +8191) 
// into M * 1/256 semitones.  Does not use 32-bit math, so bends outside MIDI pitch range will be bounded.
void bendFull(unsigned char midiNote, SIGNED_16_BIT_INT bendPitch, unsigned char* newNote, unsigned char* divisions);



// Given a base MIDI note (0...127) and a MIDI pitch bend amount(-8192...8191), provides a new note
// and the divisions (1/256 semitone) above the new note corresponding to the
// change in pitch if the Pitch Bend range is 1 octave up and 1 octave down.
// These two values can then be passed to midiFreq to get a current frequency
//
// If the bend is below MIDI note 0, it will be bounded to MIDI note 0.  Additionally,
// if the bend results in a value higher than MIDI note 127 plus 255 divisions, it will
// also be bounded.
//
// This function is reasonably efficient, using only 16-bit integers
void bend12(unsigned char midiNote, SIGNED_16_BIT_INT bendAmount, unsigned char* newNote, unsigned char* divisions);





/// NOTE DISTRIBUTION AND POLYPHONIC VOICE ALLOCATION

/// Note distribution occurs when you receive polyphonic notes from a single channel
/// and wish to send each note to its own outgoing channel, such as for MIDI MODE 4
/// or (in some sense) a pseudo-MPE.
///
/// To do this, you would first allocate a NOTE DISTRIBUTOR, specifying the range of 
/// channels over which you would like to perform distribution.  The note distributor
/// is just a data structure which keeps track of which channels have been allocated
/// and for which notes, as well as how long ago they were allocated or deallocated
///
/// The general idea is that you need to ALLOCATE a channel for an incoming note.  To
/// do this, you first try to assign it to an UNUSED channel.  You want to assign the
/// unused channel which was deallocated the FURTHEST BACK IN TIME to permit long note
/// releases.  If there is no such channel, then you need to STEAL a USED channel.  You
/// want to steal the channel that was allocated the FURTHEST BACK IN TIME.  Be sure
/// to send a NOTE OFF to the note whose channel you just stole.
///
/// It is possible that the musician may somehow play the same note multiple times
/// simultaneously.  This code does not permit this because it would require too much
/// memory: if the same note is played twice at the same time, the earlier note will
/// be dealloacated before the new note is allocated.
///
/// You can have multiple distributors.
///
/// In general you can perform note distribution with the distributor as follows: 

// Given an incoming message:
// 	If the message is a VOICE MESSAGE
// 		If the message is a NOTE ON
// 			channel <- allocateChannel(message note)
// 			if (channel is CHANNEL ALREADY ALLOCATED)
// 			    channel <- getChannel(note)
// 				Send a NOTE OFF to note on channel
// 			else if (channel is NO CHANNEL)					// all channels allocated
//				channel <- oldestChannel(true)
// 				oldNote <- stealChannel(message note)	// will steal oldest channel
// 				Send a NOTE OFF to oldNote on channel
// 			Send a NOTE ON to message note on channel
// 		Else if the message is a NOTE OFF
// 			channel <- deallocateChannel(message note)
//			if (channel != NO CHANNEL)					// otherwise what's going on?
//				Send a NOTE OFF to message note on channel
// 		Else if the message is a POLYPHONIC AFTERTOUCH
// 			channel <- getChannel(message note)
//			if (channel != NO CHANNEL)					// otherwise what's going on?
// 				Send a CHANNEL AFTERTOUCH or a POLYPHONIC AFTERTOUCH to the channel
// 		Else if the message is a CC, PC, BEND, or CHANNEL AFTERTOUCH
// 			for i = minChannel() to maxChannel() inclusive
// 				Send the message to channel i
// 	Else
// 		Send the message

/// You can use this same code to do POLYPHONIC VOICE ALLOCATION.  Let's say you have some
/// N <= 16 voices and you want to allocate them to incoming notes arriving on a channel.  Just treat
/// the voices as "channels", from 0 to 15, and use the note distributor with the algorithm above
/// modified as follows:

// Given an incoming message:
//  If the message is a CC, PC, BEND, or non-VOICE message
//		Process message
// 	Else if the message is a CHANNEL AFTERTOUCH message
// 		for all voices
//			Process aftertouch on voice
//  Else if the message is a VOICE MESSAGE
// 		If the message is a NOTE ON
// 			voice <- allocateChannel(message note)
// 			if (voice is CHANNEL ALREADY ALLOCATED)
// 			    voice <- getChannel(note)
//				Release note on voice if necessary
// 			else if (voice is NO CHANNEL)			// all channels allocated
//				voice <- oldestChannel(true)
// 				oldNote <- stealChannel(message note)	// will steal oldest channel
//				Release oldNote on voice if necessary
// 			Process message note on voice
// 		Else if the message is a NOTE OFF
// 			voice <- deallocateChannel(message note)
//			if (voice != NO CHANNEL)					// otherwise what's going on?
//				Release message note on voice
// 		Else if the message is a POLYPHONIC AFTERTOUCH
// 			voice <- getChannel(message note)
//			if (voice != NO CHANNEL)					// otherwise what's going on?
// 				Process aftertouch on voice




#define DISTRIBUTOR_NO_CHANNEL (255)
#define DISTRIBUTOR_CHANNEL_ALREADY_ALLOCATED (254)
#define DISTRIBUTOR_NO_NOTE (255)

/// The note distribution structure.  This uses up 35 bytes even if you don't have a full channel range
/// to distribute over, sorry.
struct noteDistributor
	{
	unsigned char channelNotes[16];			// 0...127 indicates note
	unsigned char heap[17];					// Oldest allocated ... newest allocated, NO CHANNEL*, newest unallocated ... oldest unallocated
	unsigned char minChannel;					// minimum channel in distribution range
	unsigned char maxChannel;					// maximum channel in distribution range
	};

// Initializes a Note Distributor over the range from minChannel to maxChannel inclusive
extern void initializeDistributor(struct noteDistributor* distributor, unsigned char minChannel, unsigned char maxChannel);

// Returns the note associated with a given channel.
// If there is no such note, returns DISTRIBUTOR_NO_NOTE.
extern unsigned char getNote(struct noteDistributor* distributor, unsigned char channel);

// Returns the channel associated with a given note.
// If there is no such channel, returns DISTRIBUTOR_NO_CHANNEL.
extern unsigned char getChannel(struct noteDistributor* distributor, unsigned char note);
	
// Returns the minimum channel in the channel range
extern unsigned char getMinChannel(struct noteDistributor* distributor);
	
// Returns the maximum channel in the channel range
extern unsigned char getMaxChannel(struct noteDistributor* distributor);
	
// Returns 1 if the channel is between the min and max channels
extern unsigned char isChannelInRange(struct noteDistributor* distributor, unsigned char channel);
	
// Returns the oldest allocated or unallocated channel, depending on ALLOCATED
// If there is no such channel, returns DISTRIBUTOR_NO_CHANNEL.
extern unsigned char oldestChannel(struct noteDistributor* distributor, unsigned char allocated);

// Allocates an unallocated channel for note and returns the channel.  
// If there are no unallocated channels, returns DISTRIBUTOR_NO_CHANNEL
// If there already is a channel allocated for the given note, then returns
// DISTRIBUTOR_CHANNEL_ALREADY_ALLOCATED.  In this case you can retrieve the
// channel by calling getChannel(distributor, note);
extern unsigned char allocateChannel(struct noteDistributor* distributor, unsigned char note);
	
// Deallocates the channel associated with a given note and returns it.
// If there is no such channel, returns DISTRIBUTOR_NO_CHANNEL.
extern unsigned char deallocateChannel(struct noteDistributor* distributor, unsigned char note);

// Reallocates for note the oldest allocated channel.  Returns the note for which the channel was 
// previously allocated.  If no channel has been allocated for any note, does not reallocate and 
// instead returns DISTRIBUTOR_NO_NOTE.
extern unsigned char stealChannel(struct noteDistributor* distributor, unsigned char note);

#endif