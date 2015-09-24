#ifndef __SNES9X_APP_H__
#define __SNES9X_APP_H__

#include <Application.h>
#include <Roster.h>
#include <unistd.h>
#include "S9xMainWindow.h"

class S9xApp : public BApplication
{
	public:
		S9xApp();
		virtual void 	RefsReceived(BMessage *m);
		virtual void 	AboutRequested(void);
		virtual void	MessageReceived(BMessage* message);
		BWindow* 		GetWindow();
	private:
		S9xMainWindow*	fS9xWindow;
};

#endif //__SNES9X_APP_H__
