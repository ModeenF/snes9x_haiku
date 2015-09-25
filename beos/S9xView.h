#ifndef __SNES9X_VIEW_H__
#define __SNES9X_VIEW_H__

#include <View.h>
#include <Cursor.h>

class S9xMainWindow;

class S9xView : public BView
{
public:
	S9xView(BRect frame, S9xMainWindow *window);
	virtual void MouseDown(BPoint point);
	virtual void MouseUp(BPoint point);
	virtual void MouseMoved(BPoint point, uint32 transit, const BMessage *m);
	virtual void ChangeCursor(int index);
	virtual void SetNeedsMouse(bool needs);
private:
	S9xMainWindow*	fS9xMainWindow;
	BCursor*		fCursor;
	int 			fLastindex;
	bool 			fNeedsMouse;
};

#endif
