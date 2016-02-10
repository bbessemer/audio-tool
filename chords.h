#ifndef CHORDS_H
#define CHORDS_H

#include "musicmap.h"

// Scale modes
#define MAJOR 0
#define DORIAN 1
#define PHRYGIAN 2
#define LYDIAN 3
#define MIXOLYDIAN 4
#define MINOR 5
#define LOCRIAN 6
#define HARMONIC 7

// Interval qualifications
#define PERFECT 0
#define AUGMENTED 1
#define DIMINISHED -1

#define ADD_SECOND  0x02
#define ADD_FOURTH  0x08
#define ADD_SEVENTH 0x40

struct Chord
{
	slBS bass;
	slBS scale;
	slBS notes[8];
	slBU num_notes;
	slBS root;
	Sint8 minor;
	Sint8 fifth;
	Sint8 inversion;
	Uint8 added;
	Note* __notes;
	slBox* box;
};

Chord* MakeChord (slBS root, Sint8 minor, Sint8 fifth, Sint8 inversion, Uint8 added);
void RecalculateChordNotes (Chord* chord);

#endif // CHORDS_H
