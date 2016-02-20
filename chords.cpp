#include <string.h>

#include "chords.h"
#include "util.h"

#define CHORDS_HEIGHT 0.08
#define CHORDS_TOP 0.70

Chord** Chords = NULL;
slBU ChordCount = 0;
Chord* SelectedChord = NULL;

void SelectChord (slBox* cbox)
{
  if (SelectedChord) SelectedChord->box->bordercolor = WHITE;
  Chord* chord = 0;
  int i;
  for (i = 0; i < ChordCount; i++)
  {
    if (Chords[i]->box == cbox)
    {
      chord = Chords[i];
      chord->box->bordercolor = BLACK;
      break;
    }
  }
  SelectedChord = chord;
}


Chord* SpawnChord ()
{
  if (SelectedChord) SelectedChord->box->bordercolor = WHITE;
  Chord* out = (Chord *)malloc(sizeof(Chord));
  out->box = slCreateBox();
	out->box->h = CHORDS_HEIGHT;
	out->box->bordercolor = BLACK;
	out->box->backcolor = GREY;
	out->box->onclick = SelectChord;
	out->num_notes = 0;
	out->__notes = NULL;
	out->selected = true;
	SelectedChord = out;
	slAddItemToList((void ***)&Chords, (slBU *)&ChordCount, (void *)out);
	return out;
}

void DestroyChord (Chord* chord)
{
  if (SelectedChord == chord) SelectedChord = NULL;
  slDestroyBox(chord->box);
  int i;
  for (i = 0; i < chord->num_notes; i++)
  {
   if (chord->__notes[i]) DespawnNote(chord->__notes[i]);
  }
  slRemoveItemFromList(&Chords,&ChordCount,chord);
  free(chord);
};

void EditChordKeyBind (slKeyBind* kb)
{
  if (SelectedChord)
  {
    slBS new_root = kb->key - SDLK_0 - 1;
    bool minor = (new_root == 1) || (new_root == 2) || (new_root == 5) || (new_root == 6);
    int fifth = (new_root == 6) ? DIMINISHED : PERFECT;
    RemakeChord(SelectedChord, new_root, minor, fifth, 0, NO_ADDITIONS);
    RecalculateChordNotes(SelectedChord);
  };
}

void RemoveChordKeyBind ()
{
  if (SelectedChord) DestroyChord(SelectedChord);
}

char* GetRomanNumeral (slBS root, Sint8 minor, Sint8 fifth,
    Sint8 inversion, Uint8 added)
{
  const char sup0[4] = {0xE2, 0x81, 0xB0, 0};
  char* base = malloc(10);
  if (minor)
  {
    switch (root)
    {
      case 0: base = "i"; break;
      case 1: base = "ii"; break;
      case 2: base = "iii"; break;
      case 3: base = "iv"; break;
      case 4: base = "v"; break;
      case 5: base = "vi"; break;
      case 6: base = "vii"; break;
    }
  }
  else
  {
    switch (root)
    {
      case 0: base = "I"; break;
      case 1: base = "II"; break;
      case 2: base = "III"; break;
      case 3: base = "IV"; break;
      case 4: base = "V"; break;
      case 5: base = "VI"; break;
      case 6: base = "VII"; break;
    }
  }
  /*if (fifth == DIMINISHED)
    strcat(base, sup0);*/

  return base;
}

void RemakeChord (Chord* out, slBS root, Sint8 minor, Sint8 fifth,
    Sint8 inversion, Uint8 added)
{
  int i;
  if (out->__notes)
  {
    for (i = 0; i < out->num_notes; i++)
    {
      if (out->__notes[i]) DespawnNote(out->__notes[i]);
    }
  }

  if (minor) minor = -1;
  out->root = root;
  out->minor = minor;
  out->fifth = fifth;
  out->inversion = inversion;
  out->added = added;

  int num_notes = 3 + HW(added);
  slBS* notes_stage1 = malloc(num_notes*sizeof(slBS));
  i = 0;
  notes_stage1[i] = 0; i++;
  if (added && ADD_SECOND)
  {
    notes_stage1[i] = 1; i++;
  }
  notes_stage1[i] = 2; i++;
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
  free(notes_stage1);
  memcpy(out->notes, notes_stage2, sizeof(slBS)*num_notes);

  // Temporary
  slBS bass = (out->notes[0] + root) % 7;
  slBS scale = bass;

  out->scale = scale;
  out->bass = bass;
  out->num_notes = num_notes;
  out->box->backcolor = GetNoteColor(scale);
  SDL_Color textcolor;
  if (scale == 2) textcolor = BLACK;
  else textcolor = WHITE;
  char* roman = GetRomanNumeral(root, minor, fifth, inversion, added);
  out->box->texref = slRenderText(roman, textcolor);
  free(roman);
}

void RecalculateChordNotes (Chord* chord)
{
  int i;
  Note** newptr = realloc(chord->__notes, chord->num_notes*sizeof(Note*));
  if (newptr) chord->__notes = newptr;
  else slDeadBeef();
  for (i = 0; i < chord->num_notes; i++)
  {
    Note* new_note = SpawnHiddenNote();
    new_note->channel = chord->notes[i];
    new_note->start = chord->start;
    new_note->duration = chord->duration;
    new_note->accidental = 0;
    RecalculateNotePitch(new_note, chord->scale);
    chord->__notes[i] = new_note;
  }
}

void NewChordAtClickPoint ()
{
  slScalar mousex, mousey;
	slGetMouse(&mousex, &mousey);
	//printf("New chord at %f, %f\n", mousex, mousey);
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
	for (slBU cur = 0; cur < ChordCount; cur++)
  {
    Chord* other = *(Chords + cur);
    if ((chord->start > other->start && chord->start < other->start + other->duration)
     || (chord->start + chord->duration > other->start && chord->start + chord->duration < other->start + other->duration))
    {
      DestroyChord(chord);
      break;
    };
  }
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
