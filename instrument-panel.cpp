#include "slice.h"
slBox* ipanel_mainbox;
slBox* ipanel_infobox;
void CreateInstrumentPanel ()
{
	ipanel_mainbox = slCreateBox();
	ipanel_infobox = slCreateBox(slRenderText("Instruments (Drag onto Notes)"));
	slSetBoxDims(ipanel_mainbox,0.64,0.78,0.36,0.22,40);
	slSetBoxDims(ipanel_infobox,0,0,0.36,0.04,39);
	ipanel_mainbox->backcolor = {0,0,0,31};
	ipanel_mainbox->bordercolor = {0,0,0,95};
};
