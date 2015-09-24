#include <Window.h>
#include "s9x_flags.h"

void set_flags(BWindow *window, int flags) // kludge to workaround a snes9x SetFlags macro
{
	window->SetFlags(flags);
}
