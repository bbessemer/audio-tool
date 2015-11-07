/*
 * This program is licensed under the GNU General Public Licence.
 */

#include <slice.h>

#include "interface.h"
#include "mixer/mixer.h"

int ui_init ()
{
  return 0;
}

bool tick ()
{
  slCycle();
  return !slGetExitReq();
}
