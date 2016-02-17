#include <slice.h>
#include "sliceexts.h"

#define HOOKSIZE (sizeof(void (*) ()))
#define INIT_LISTSIZE 10
#define EXPAND_BY 2
void (**SLX_Hooks) () = 0;
int num_hooks;
int hooks_len;

void slxAddHook (void (*hook) ())
{
  if (!SLX_Hooks)
  {
    SLX_Hooks = malloc(HOOKSIZE*INIT_LISTSIZE);
  }
  int i;
  for (i = 0; i < hooks_len; i++)
  {
    if (!SLX_Hooks[i])
    {
      SLX_Hooks[i] = hook;
      num_hooks++;
      return;
    }
    i++;
  }
  void (**new_hooks) () = realloc(SLX_Hooks, (hooks_len+EXPAND_BY)*HOOKSIZE);
  if (new_hooks)
  {
    SLX_Hooks = new_hooks;
    for (i = hooks_len; i < hooks_len+EXPAND_BY; i++) SLX_Hooks[i] = 0;
    i = hooks_len;
    hooks_len += EXPAND_BY;
    SLX_Hooks[i] = hook;
    num_hooks++;
    return;
  }
  else
  {
    slFatal("Slice Extension Manager: Memory allocation error", 0x1a4);
  }
}

void slxRemoveHook (void (*hook) ())
{
  int i;
  for (i = 0; i < hooks_len; i++)
  {
    if (SLX_Hooks[i] == hook)
    {
      SLX_Hooks[i] = 0;
      num_hooks--;
      return;
    }
  }
}

void slxRunHooks ()
{
  int i;
  for (i = 0; i < hooks_len; i++)
  {
    if (SLX_Hooks[i]) SLX_Hooks[i]();
  }
}
