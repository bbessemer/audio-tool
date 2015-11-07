#include <slice.h>

#include "interface.h"
#include "mixer/mixer.h"

int main ()
{
	slInit();
	ui_init();
	//music_init();
	while (tick());
	slQuit();
	return 0;
}
