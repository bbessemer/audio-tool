#include <slice.h>

//#include "interface.h"
//#include "mixer/mixer.h"
#include "musicmap.h"

int main ()
{
	slInit();
	slSetCustomDrawStage_Middle(DrawGrid);
	//ui_init();
	//music_init();
	while (!slGetReqt())
	{
		RepositionNotes();
		slCycle();
	};
	slQuit();
	// return 0; is not necessary
	// because slQuit exits the program with a zero return
};
