#include <slice.h>
int main ()
{
	slInit();
	while (!slGetReqt) slCycle();
	slQuit();
};
