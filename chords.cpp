#include <string.h>

#include "chords.h"
#include "util.h"

#define CHORDS_HEIGHT 0.08
#define CHORDS_TOP 0.70

Chord** Chords = NULL;
slBU ChordCount = 0;
Chord* SelectedChord = NULL;

Chord* SpawnChord ()
{
  Chord* out = (Chord *)malloc(sizeof(Chord));
  out->box = slCreateBox();
	out->box->h = CHORDS_HEIGHT;
	out->box->bordercolor = WHITE;
	out->box->backcolor = WHITE;
	//out->box->onclick = SelectChord;
	out->selected = true;
	SelectedChord = out;
	slAddItemToList((void ***)&Chords, (slBU *)&ChordCount, (void *)out);
	return out;
}

void EditChordKeyBind (slKeyBind* kb)
{
  slBS new_root = kb->key - SDLK_0 - 1;
  bool third = (new_root == 1) || (new_root == 2) || (new_root == 5) || (new_root == 7);
  int fifth = (new_root == 7) ? DIMINISHED : PERFECT;
  RemakeChord(SelectedChord, new_root, third, fifth, 0, NO_ADDITIONS);
}

void RemakeChord (Chord* out, slBS root, Sint8 minor, Sint8 fifth,
    Sint8 inversion, Uint8 added)
{
  if (minor) minor = -1;
  out->root = root;
  out->minor = minor;
  out->fifth = fifth;
  out->inversion = inversion;
  out->added = added;

  int num_notes = 3 + HW(added);
  slBS* notes_stage1 = malloc(num_notes*sizeof(slBS));
  int i = 0;
  notes_stage1[i] = 0; i++;
  if (added && ADD_SECOND)
  {
    notes_stage1[i] = 1; i++;
  }
  notes_stage1[i] = 2;
  if (added && ADD_FOURTH)
  {
    notes_stage1[i] = 3; i++;
  }
  notes_stage1[i] = 4;
  if (added && ADD_SEVENTH)
  {
    notes_stage1[i] = 6; i++;
  }

  slBS* notes_stage2 = malloc(num_notes*sizeof(slBS));
  for (i = 0; i < num_notes; i++)
  {
    notes_stage2[i] = notes_stage1[(i + inversion) % num_notes];
  }
  memcpy(out->notes, notes_stage2, num_notes*sizeof(slBS));

  // Temporary
  slBS scale = root;
  slBS bass = root;

  out->scale = scale;
  out->bass = bass;
  out->box->backcolor = GetNoteColor(scale);
}

void NewChordAtClickPoint()
{
  slScalar mousex, mousey;
	slGetMouse(&mousex, &mousey);
	printf("New chord at %f, %f\n", mousex, mousey);
	// If it's out of bounds, don't even bother.
	if (mousey < CHORDS_TOP || mousey > CHORDS_TOP + CHORDS_HEIGHT) return;
	slScalar roll_left = GetRollLeft();
	if (mousex < roll_left || mousex > roll_left + (((GetMeasureCount() * GetBeatsPerMeasure())
                              - (GetDefaultNoteLength() - 1)) * GetBeatWidth())) return;
	// It's not out of bounds, so figure out where it goes.
	slBU start = (mousex - roll_left) / GetBeatWidth();
	Chord* chord = SpawnChord();
	chord->start = start;
	chord->duration = GetNoteLength();
}

void RepositionChords ()
{
	slScalar roll_left = GetRollLeft();
	for (slBU i = 0; i < ChordCount; i++)
	{
		Chord* chord = Chords[i];
		chord->box->w = GetBeatWidth() * chord->duration;
		chord->box->x = roll_left + (chord->start * GetBeatWidth());
		chord->box->y = CHORDS_TOP;
	};
};
