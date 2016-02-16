/*
 * Copyright (C) 2015 Brent Bessemer
 * The Slice Engine is the property of Seth Carter and Glassblade Games.
 */

#include <slice.h>
#include "sliceexts.h"

bool fps_counter_on;
slBox* fps_counter = 0;
slScalar t;
slBU frame_count;
slScalar fps;
char fps_text[10];
slKeyBind* fps_counter_kb;

void (*old_custom_drawstage) (SDL_Window*, SDL_Renderer*) = 0;

void count_fps (void)
{
  if (t > 0.1) {
    if ((fps = frame_count/t) < 10000)
    {
      snprintf(fps_text, 10, "%.1f fps", fps);
      fps_counter->texref = slRenderText(fps_text);
    }
    else if (fps < 1000000)
    {
      snprintf(fps_text, 10, "%.1fk fps", fps/1000.0);
      fps_counter->texref = slRenderText(fps_text);
    }
    else
    {
      snprintf(fps_text, 10, "1M+ fps");
      fps_counter->texref = slRenderText(fps_text);
    }
    t = 0;
    frame_count = 0;
  }
  else
  {
    t += slGetDelta();
    frame_count++;
  }
}

void drawstage_wrapper ()
{
  //if (old_custom_drawstage) old_custom_drawstage(window, renderer);
  if (fps_counter_on) count_fps();
}

void slxEnableFpsCounter_wrapper (void)
{
  slxEnableFpsCounter();
}

void slxEnableFpsCounter (int toggle_key = 0, bool sticky = true)
{
  fps_counter_on = true;
  if (!fps_counter)
  {
    fps_counter = slCreateBox();
    fps_counter->bordercolor = {255, 255, 255, 255};
    slSetBoxDims(fps_counter, 0.8, 0, 0.2, 0.07);
  }
  else
  {
    fps_counter->bordercolor = {255, 255, 255, 255};
  }

  // If there is already a custom 'Middle' draw stage, back it up so that it
  // runs alongside count_fps.

  //old_custom_drawstage = ???

  slSetCustomDrawStage_Front(drawstage_wrapper);

  if (toggle_key)
  {
    fps_counter_kb = slGetKeyBind("Show/Hide FPS Meter", toggle_key);
    if (sticky) fps_counter_kb->onpress = slxToggleFpsCounter;
    else
    {
      fps_counter_kb->onpress = slxEnableFpsCounter_wrapper;
      fps_counter_kb->onrelease = slxDisableFpsCounter;
    }
    slxDisableFpsCounter();
  }
}

void slxDisableFpsCounter ()
{
  fps_counter->bordercolor = {0, 0, 0, 0};
  fps_counter->texref = NULL;
  fps_counter_on = false;
}

void slxToggleFpsCounter ()
{
  if (fps_counter_on) slxDisableFpsCounter();
  else slxEnableFpsCounter();
}

void slxDestroyFpsCounter ()
{
  slDestroyBox(fps_counter);
  fps_counter_kb->onpress = NULL;
  fps_counter_kb->onrelease = NULL;
}

slScalar slxGetFps ()
{
  return fps;
}
