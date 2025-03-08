#ifndef __MIDI_UTILITIES_H
#define __MIDI_UTILITIES_H

#include "miditypes.h"


/// PITCH BEND

// Given a base MIDI note and a MIDI bend amount, provides a new note
// and the divisions (1/256 semitone) above the new note corresponding to the
// change in pitch if the Pitch Bend range is 1 octave up and 1 octave down.
// These two values can then be passed to midiFreq to get a current frequency
void bend12(unsigned char note, UNSIGNED_16_BIT_INT amount, unsigned char* newNote, unsigned char* divisions);


// Given a MIDI Note plus some number of 1/256 divisions of a semitone,
// Returns the frequency of the note * 1024
// If you want the frequency as an integer, take this value >> 10
//
// The very highest and very lowest notes are not supported and pitch bending
// outside of them will be bounded.
//
// Normally pitch->frequency conversion requires pow().  This function instead uses
// an approximation lookup table and all integer math.
// Using this function will require 264 bytes of static ROM space (PROGMEM) for an
// interpolation table.

extern UNSIGNED_32_BIT_INT midiFreq(unsigned char midiNote, unsigned char divisions);






/// NOTE DISTRIBUTION

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
/// In general you can perform note distribution with the distributor as follows: 

// Given an incoming message:
// 	If the message is a VOICE MESSAGE
// 		If the message is a NOTE ON
// 			channel <- allocateChannel(message note)
// 			if (channel is NO CHANNEL)			// all channels allocated
// 				oldNote <- stealChannel(message note)
// 				Send a NOTE OFF to oldNote
// 				Send a NOTE ON to message note
// 		Else if the message is a NOTE OFF
// 			deallocateChannel(message note)
// 		Else if the message is a POLYPHONIC AFTERTOUCH
// 			channel <- getChannel(message note)
// 			Send a CHANNEL AFTERTOUCH or a POLYPHONIC AFTERTOUCH to the channel
// 		Else if the message is a CC, PC, or CHANNEL AFTERTOUCH
// 			for i = minChannel() to maxChannel() inclusive
// 				Send the message to channel i
// 	Else
// 		Send the message

/// You can have multiple distributors.

#define DISTRIBUTOR_NO_CHANNEL (255)
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
extern unsigned char allocateChannel(struct noteDistributor* distributor, unsigned char note);
	
// Returns the note associated with a given channel.
// If there is no such note, returns DISTRIBUTOR_NO_NOTE.
extern unsigned char getNote(struct noteDistributor* distributor, unsigned char channel);

// Deallocates the channel associated with a given note and returns it.
// If there is no such channel, returns DISTRIBUTOR_NO_CHANNEL.
extern unsigned char deallocateChannel(struct noteDistributor* distributor, unsigned char note);

// Reallocates the oldest allocated for note.  If no channel was allocated, returns DISTRIBUTOR_NO_NOTE.
// If the channel was previously used by a note, returns that note.
extern unsigned char stealChannel(struct noteDistributor* distributor, unsigned char note);

#endif