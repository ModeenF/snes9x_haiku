#include "S9xMainWindow.h"
#include "S9xView.h"
#include "Cursors.h"

S9xView::S9xView(BRect rect, S9xMainWindow *window)
	: BView(rect, "", B_FOLLOW_ALL_SIDES, B_WILL_DRAW),
	fS9xMainWindow(window),
	fCursor(new BCursor(c_crosshairs_cursor)),
	fLastindex(0),
	fNeedsMouse(false)
{
}


void
S9xView::SetNeedsMouse(bool mouse)
{
	fNeedsMouse = mouse;
}


void
S9xView::ChangeCursor(int index)
{
	if(index == fLastindex)
		return;
	index == 0 ? SetViewCursor(B_CURSOR_SYSTEM_DEFAULT) : SetViewCursor(fCursor);
	fLastindex = index;
}


void
S9xView::MouseDown(BPoint point)
{
	uint32 buttons;
	if (Window()->CurrentMessage()->FindInt32("buttons", (int32 *)&buttons) == B_NO_ERROR) {
		if(fNeedsMouse) 
			fS9xMainWindow->UpdateMouse(buttons);
	}
	BView::MouseDown(point);
}


void 
S9xView::MouseUp(BPoint point)
{
	uint32 buttons;
	if (Window()->CurrentMessage()->FindInt32("buttons", (int32 *)&buttons) == B_NO_ERROR) {
		if(fNeedsMouse) 
			fS9xMainWindow->UpdateMouse(buttons);
	}
	BView::MouseUp(point);
}


void
S9xView::MouseMoved(BPoint point, uint32 transit, const BMessage *m)
{
	if(fNeedsMouse) 
		fS9xMainWindow->UpdateMousePosition((int)point.x, (int)point.y);
	BView::MouseMoved(point, transit, m);
}
