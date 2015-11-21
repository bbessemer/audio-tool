#ifndef UTIL_H
#define UTIL_H

void* find_item_by_slBox (slBox* sl, void** in, slBU len)
{
  int i;
  for (i = 0; i < len; i++)
  {
    if (in[i]->box == sl)
      return (void *)in[i];
  }
  return NULL;
}

#endif // UTIL_H
