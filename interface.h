#ifndef INTERFACE_H
#define INTERFACE_H

#define RED     {255, 0,   0,   255}
#define ORANGE  {255, 128, 0,   255}
#define YELLOW  {255, 255, 0,   255}
#define GREEN   {0,   255, 0,   255}
#define BLUE    {0,   0,   255, 255}
#define MAGENTA {255, 0,   255, 255}

int ui_init ();
bool tick ();

#endif // INTERFACE_H
