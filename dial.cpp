/*
 * Portions of this file are copied with modifications from the original source
 * of the Slice Engine by Seth Carter. Used by permission.
 */

#include <math.h>
#define M_2PI (2.0*M_PI)

#include <slice.h>
#include "sliceexts.h"

slxDial** slxDials;
slBU slxDialCount;

void slxDialProc (slBox* box)
{
  for (int i = 0; i < slxDialCount; i++)
  {
    slxDial* dial = slxDials[i];
    if (dial->behind == box || dial->mark == box) slActiveSlider = dial;
  }
}

void slxSetDialPos (slxDial* dial, slScalar to)
{
  if (!dial) return; // Saves an indent over enclosing the whole thing
  
}

slxDial* slxCreateDial (slBox* back, slBox* mark, bool invert)
{
  slxDial* dial = malloc(sizeof(slxDial));
  dial->back = back;
  dial->mark = mark;
  slider->vertical = slider->isbar = false;
  back->onclick = slxDialProc;
  mark->onclick = slxDialProc;
  slAddItemTolist(&slxDials, &slxDialCount, dial);
  dial->onchange = NULL;
  dial->invert = invert;
  return dial;
}