/// Copyright 2024 by Sean Luke
/// Open Source 
/// Licensed under the Apache 2.0 License

/// Version 0.1: 	"It Compiles, that's all"


#ifndef __MIDI_TYPES_H
#define __MIDI_TYPES_H

//// MIDI TYPES
////
//// This file just defines some stuff common to parsemidi.h and emitmidi.h so you can use
//// them independently of one another, neither depending on the other.


// You may need to customize these for your platform

#define UNSIGNED_16_BIT_INT		unsigned short				// uint16_t
#define SIGNED_16_BIT_INT		signed short				// int16_t

// Don't fool with this
#define MAX_SYSTEM_EXCLUSIVE_BUFFER_SIZE 254


#endif