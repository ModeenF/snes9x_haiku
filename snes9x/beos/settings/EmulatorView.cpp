
#include "EmulatorView.h"

#include <Box.h>
#include <CheckBox.h>

/***** Constructor ****/
EmulatorView::EmulatorView(BRect frame,const char *name, float tabHeight)
: BView(frame, name, B_FOLLOW_NONE, B_WILL_DRAW)
{
	SetViewColor(216, 216, 216);
}

/***** Destructor ****/
EmulatorView::~EmulatorView()
{
}

