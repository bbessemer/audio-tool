#ifndef CHORDS_H
#define CHORDS_H

#include "musicmap.h"

struct Chord
{
	slBS bass, scale, root;
	slBS notes[8];
	slBU num_notes;
	Sint8 minor;
	Sint8 fifth;
	Sint8 inversion;
	Uint8 added;
	Note** __notes;
	slBox* box;
	bool selected;
  slScalar start, duration;
};
Chord* MakeChord (slBS root, Sint8 minor, Sint8 fifth, Sint8 inversion, Uint8 added);
void RecalculateChordNotes (Chord* chord);
void RepositionChords ();
void NewChordAtClickPoint ();
void RemakeChord(Chord*, slBS, Sint8, Sint8, Sint8, Uint8);
void DestroyChord (Chord* chord);
void RemoveChordKeyBind ();
void EditChordKeyBind (slKeyBind*);

#endif // CHORDS_H
