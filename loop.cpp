/*
 * This program is licensed under the GNU General Public Licence.
 */

#include <slice.h>

#include "interface.h"
#include "music.h"
//#include "mixer/mixer.h"

GridBox** gridBoxes = 0;
slBU gridBoxCount = 0;
slScalar leftBorder = 0;
Note* beingMoved = 0;
Note* beingStretched = 0;

GridBox* find_grid_by_slBox (slBox* sl)
{
  int i;
  for (i = 0; i < gridBoxCount; i++)
  {
    if (gridBoxes[i]->box == sl)
      return gridBoxes[i];
  }
  return NULL;
}

GridBox* find_grid_by_coords (int i, int j)
{
  int k;
  GridBox* test = gridBoxes[7*i+j];
  if (test->i == i && test->j == j)
    return test;
  else for (k = 0; k < gridBoxCount; k++)
  {
    test = gridBoxes[k];
    if (test->i == i && test->j == j)
      return test;
  }
}

void grid_box_click (slBox* self)
{
  GridBox* grid_box = find_grid_by_slBox(self);
  createNote(grid_box->j, grid_box->i, 1.0);
}

SDL_Color get_note_color (int relative)
{
  SDL_Color ret;
  switch (relative)
  {
    case 0: ret = RED; break;
    case 1: ret = ORANGE; break;
    case 2: ret = YELLOW; break;
    case 3: ret = GREEN; break;
    case 4: ret = BLUE; break;
    case 5: ret = PURPLE; break;
    case 6: ret = MAGENTA; break;
    default: ret = WHITE;
  }
  return ret;
}

void note_click ()
{
  Note* note;
  slScalar x, y;
  slGetMouse(&x, &y);
  int i;
  for (i = 0; i < getNotesCount(); i++)
  {
   if (slPointOnBox(getNote(i)->box, x, y))
    {
      Note* note = getNote(i);
      slBox* box = note->box;
      slScalar local_y = (y-box->y)/box->h;
      if (local_y < NOTE_END_SIZE || local_y > 1-NOTE_END_SIZE)
        beingStretched = note;
      else
      {
        int pos_i, pos_j;
        find_gridpos(x, y, &pos_i, &pos_j);
        GridBox* grid = find_grid_by_coords(pos_i, pos_j);
        beingMoved = note;
      }
      break;
    }
  }
}

void find_coords (int i, int j, slScalar* x, slScalar* y)
{
  *x = leftBorder+j*NOTE_WIDTH;
  *y = i*BEAT_HEIGHT;
}

void find_gridpos (slScalar x, slScalar y, int* i, int* j)
{
  *j = (int)((x-leftBorder)/NOTE_WIDTH);
  *i = (int)(y/BEAT_HEIGHT);
}

int ui_init ()
{
  int i, j;
  for (i = 0; i < (int)1/BEAT_HEIGHT; i++)
  {
    for (j = 0; j < 7; j++)
    {
      GridBox* grid_box = (GridBox *)malloc(sizeof(GridBox));
      grid_box->box = slCreateBox();
      grid_box->box->bordercolor = WHITE;
      leftBorder = 0.5-OCTAVE_WIDTH/2;
      slScalar x, y;
      find_coords(i, j, &x, &y);
      slSetBoxDims(grid_box->box, x, y, NOTE_WIDTH, BEAT_HEIGHT);
      grid_box->box->onclick = grid_box_click;
      grid_box->i = i;
      grid_box->j = j;
      slAddItemToList((void ***)(&gridBoxes), &gridBoxCount, grid_box);
    }
  }
  return 0;
}

bool tick ()
{
  notes_move(beingMoved, beingStretched);
  slCycle();
  return !slGetExitReq();
}
