#ifndef __SNES9X_REFRESH_WINDOW_H__
#define __SNES9X_REFRESH_WINDOW_H__

class S9xRefreshWindow : public BWindow
{
public:
	S9xRefreshWindow(BRect rect, BWindow *parent, uint32 closemsg);
	virtual void Quit();
private:
	BWindow*	fWindow;
	uint32 		fS9xMsg;
};

#endif
