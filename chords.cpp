#include "chords.h"
#include "util.h"

Chord* MakeChord (slBS root, Sint8 minor, Sint8 fifth, Sint8 inversion, Uint8 added)
{
  Chord* out = (Chord *)malloc(sizeof(Chord));

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

  //scale = out->notes[0];

}
