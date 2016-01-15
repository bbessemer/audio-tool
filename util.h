#ifndef UTIL_H
#define UTIL_H

/*void* find_item_by_slBox (slBox* sl, void** in, slBU len)
{
  int i;
  for (i = 0; i < len; i++)
  {
    if (in[i]->box == sl)
      return (void *)in[i];
  }
  return NULL;
}*/

int HW (int x) {
  x = (x & (0x55555555)) + ((x >> 1) & (0x55555555));
  x = (x & (0x33333333)) + ((x >> 2) & (0x33333333));
  x = (x & (0x0f0f0f0f)) + ((x >> 4) & (0x0f0f0f0f));
  x = (x & (0x00ff00ff)) + ((x >> 8) & (0x00ff00ff));
  x = (x & (0x0000ffff)) + ((x >> 16) & (0x0000ffff));
  return x;
}

#endif // UTIL_H
