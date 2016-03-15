#include "musicmap.h"
#include "interface.h"
#include <sliceopts.h>

bool already_created = false;
slBox* BackgroundBox;
slBox* CloseMenuBox;
slSlider* VolumeSlider = NULL;
char VolText[20] = "Master Volume:  20%";

void slxSetRelBoxDims (slBox* box, slBox* container,
  slScalar x, slScalar y, slScalar w, slScalar h)
{
  x *= container->w;
  w *= container->w;
  y *= container->h;
  h *= container->h;
  x += container->x;
  y += container->y;
  slSetBoxDims(box, x, y, w, h);
  box->z = container->z - 1;
}

void OpenOptionsMenu (void)
{
  EnableOptionsMenu(true);
  opInformCustomVisi(true); // Tell sliceopts the menu is open!
}

void CloseOptionsMenu (void)
{
  EnableOptionsMenu(false);
  opInformCustomVisi(false); // Tell sliceopts the menu is closed!
}

void set_volslider (slSlider* sld)
{
  slScalar vol = sld->curvalue;
  SetMasterVolume(vol);
  char vol_str[3];
  if (vol < 0.095)
  {
    sprintf(vol_str, "  %d%%", slRound(vol*100));
  }
  if (vol < 0.995)
  {
    sprintf(vol_str, " %d%%", slRound(vol*100));
  }
  else
  {
    sprintf(vol_str, "%d%%", slRound(vol*100));
  }
  memcpy((void *)(VolText + 15), vol_str, 4);
  sld->behind->texref = slRenderText(VolText);
}

void create_volslider ()
{
  slBox* behind = slCreateBox(slRenderText(VolText));
  slBox* mark = slCreateBox();
  slxSetRelBoxDims(behind, BackgroundBox, 0.2, 0.1, 0.6, 0.1);
  behind->bordercolor = WHITE;
  behind->backcolor = {64, 64, 64, 255};
  slxSetRelBoxDims(mark, behind, 0.175, 0.0, 0.05, 1.0);
  mark->bordercolor = WHITE;
  mark->backcolor = {255, 255, 255, 220};
  VolumeSlider = slCreateSlider(behind, mark);
  VolumeSlider->minvalue = 0.0;
  VolumeSlider->maxvalue = 1.0;
  VolumeSlider->onchange = set_volslider;
}

void set_keyslider (slSlider* sld)
{
  slSetSliderValue(sld, slRound(sld->curvalue*12));
}

void create_keyslider ()
{
  slBox* behind = slCreateBox(slRenderText(GetNoteName(GetKeyNote(), 0)));
  slBox* mark = slCreateBox();
  slxSetRelBoxDims(behind, BackgroundBox, 0.2, 0.3, 0.6, 0.1);
  behind->bordercolor = WHITE;
  behind->backcolor = {64, 64, 64, 255};
}

void EnableOptionsMenu (bool visible)
{
  if (visible)
  {
    if (!already_created)
    {
      BackgroundBox = slCreateBox();
      slSetBoxDims(BackgroundBox, 0.1, 0.1, 0.8, 0.8);
      BackgroundBox->backcolor = {64, 64, 64, 220};
      BackgroundBox->z = 2;

      CloseMenuBox = slCreateBox(slRenderText("Close Menu"));
      slxSetRelBoxDims(CloseMenuBox, BackgroundBox, 0.35, 0.8, 0.3, 0.08);
      CloseMenuBox->bordercolor = WHITE;
      CloseMenuBox->backcolor = {64, 64, 64, 255};
      CloseMenuBox->hoverable = true;
      CloseMenuBox->hoverbordercolor = WHITE;
      CloseMenuBox->hoverbackcolor = {80, 80, 80, 255};
      CloseMenuBox->onclick = CloseOptionsMenu;

      create_volslider();
      create_keyslider();

      already_created = true;
    }
    else
    {
      BackgroundBox->visible = true;
      CloseMenuBox->visible = true;
      VolumeSlider->behind->visible = true;
      VolumeSlider->mark->visible = true;
    }
  }
  else if (already_created)
  {
    BackgroundBox->visible = false;
    CloseMenuBox->visible = false;
    VolumeSlider->behind->visible = false;
    VolumeSlider->mark->visible = false;
  }
}
