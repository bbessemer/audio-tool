#include <slice.h>

#include "music.h"
#include "interface.h"
//#include "mixer/mixer.h"

Note** notes;
slBU notesCount = 0;
int beatLength = QUARTER;

Note* getNote (int i)
{
  return notes[i];
}

slBU getNotesCount ()
{
  return notesCount;
}

Note* find_note_by_slBox (slBox* sl)
{
  int i;
  for (i = 0; i < notesCount; i++)
  {
    if (notes[i]->box == sl)
      return notes[i];
  }
  return NULL;
}

float pitch (int m, float bend, float tuning)
{
  return tuning*pow(2.0, (m + bend/100.0 - MIDI(A, 4))/12.0);
}

Note* createNote (int rel_pitch, int start_beats, int beats)
{
  Note* created = (Note *)malloc(sizeof(Note));
  created->box = slCreateBox();
  created->box->bordercolor = BLACK;
  created->box->backcolor = get_note_color(rel_pitch);
  slScalar x, y;
  find_coords(start_beats, rel_pitch, &x, &y);
  slSetBoxDims(created->box, x, y, NOTE_WIDTH, beats*BEAT_HEIGHT);
  created->pitch = 69; // Temporary
  created->rel_pitch = rel_pitch;
  created->start = start_beats*beatLength;
  created->duration = beats*beatLength;
  slAddItemToList((void ***)(&notes), &notesCount, created);
  return created;
}
