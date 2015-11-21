#ifndef INTERFACE_H
#define INTERFACE_H

#define BLACK   {0,   0,   0,   255}
#define WHITE   {255, 255, 255, 255}
#define RED     {255, 0,   0,   255}
#define ORANGE  {255, 128, 0,   255}
#define YELLOW  {255, 255, 0,   255}
#define GREEN   {0,   255, 0,   255}
#define BLUE    {0,   0,   255, 255}
#define PURPLE  {128, 0,   128, 255}
#define MAGENTA {255, 0,   255, 255}

SDL_Color get_note_color (int relative);

#define OCTAVE_WIDTH 0.2
#define NOTE_WIDTH OCTAVE_WIDTH/7
#define BEAT_HEIGHT 0.0625
#define NOTE_END_SIZE 0.009


struct GridBox
{
  slBox* box;
  int i, j;
};

int ui_init ();
bool tick ();
void find_coords(int, int, slScalar*, slScalar*);
void find_gridpos(slScalar, slScalar, int*, int*);

#endif // INTERFACE_H
