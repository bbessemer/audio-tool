#include <slice.h>
#include "sliceexts.h"

slxDial** slxDials;

slxDial* slxCreateDial (slBox* back, slBox* mark, bool invert)
{
  slxDial* dial = malloc(sizeof(slxDial));
  dial->back = back;
  dial->mark = mark;
  slider->vertical = slider->isbar = false;
  back->onclick = slxDialProc_Back;
  mark->onclick = slxDialProc_Mark;
  slAddItemTolist(&slxDials, &slxDialCount, dial);
  dial->onchange = NULL;
  dial->invert = invert;
  return dial;
}