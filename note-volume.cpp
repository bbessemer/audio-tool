#include "musicmap.h"

extern Note** Notes;
extern slBU NoteCount;
Note* VolAdjNote = NULL;

void OnRightClick ()
{
    printf("Trying to get clicked note.\n");
    slScalar mousex,mousey;
    slGetMouse(&mousex,&mousey);
    for (slBU cur = 0; cur < NoteCount; cur++)
    {
        VolAdjNote = *(Notes + cur);
        if (VolAdjNote->box) if (slPointOnBox(VolAdjNote->box,mousex,mousey)) return;
    };
    printf("Failed to get clicked note.\n");
    VolAdjNote = NULL;
};

void OnRightUnclick ()
{
    VolAdjNote = NULL;
};

slBox* VolAdjInfoBox;
slBox* VolAdjSlider_Back;
slBox* VolAdjSlider_Mark;
slSlider* VolAdjSlider;

void VolAdjStep ()
{
    if (VolAdjNote)
    {
        VolAdjInfoBox->visible = true;
        VolAdjSlider_Back->visible = true;
        VolAdjSlider_Mark->visible = true;
        slScalar mousey;
        slGetMouse(NULL,&mousey);
        slSetSliderPos(VolAdjSlider,mousey);
        VolAdjNote->volume = VolAdjSlider->curvalue;
    }
    else
    {
        VolAdjInfoBox->visible = false;
        VolAdjSlider_Back->visible = false;
        VolAdjSlider_Mark->visible = false;
    };
};

void VolAdjInit ()
{
    VolAdjInfoBox = slCreateBox(slRenderText("Note Volume",{0,0,0,255}));
    VolAdjInfoBox->z = 20;
    slSetBoxDims(VolAdjInfoBox,0.25,0.125,0.5,0.0625, 14);
    slKeyBind* VolumeAdjustBind = slGetKeyBind("Adjust Note Volume",0,3);
    VolumeAdjustBind->onpress = OnRightClick;
    VolumeAdjustBind->onrelease = OnRightUnclick;
    VolAdjSlider_Back = slCreateBox();
    VolAdjSlider_Back->z = 20;
    slSetBoxDims(VolAdjSlider_Back,0.475,0.25,0.05,0.5, 16);
    VolAdjSlider_Back->backcolor = {127,127,127,31};
    VolAdjSlider_Back->bordercolor = {0,0,0,255};
    VolAdjSlider_Mark = slCreateBox();
    VolAdjSlider_Mark->z = 19;
    slSetBoxDims(VolAdjSlider_Mark,0.475,0.25,0.05,0.05, 15);
    VolAdjSlider_Mark->backcolor = {0,0,255,255};
    VolAdjSlider_Mark->bordercolor = {0,0,0,255};
    VolAdjSlider = slCreateSlider(VolAdjSlider_Back,VolAdjSlider_Mark,true,true,false);
    VolAdjSlider->minvalue = 0;
    VolAdjSlider->maxvalue = 1;
};

void VolAdjQuit ()
{
    slDestroySlider(VolAdjSlider);
};
