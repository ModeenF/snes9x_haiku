#include "S9xApp.h"

#include "s9x_includes.h"

S9xApp::S9xApp()
	: BApplication("application/x-vnd.s9x")//application/x-vnd.toyoshima-house.yame
{
/*	app_info ai;
	BPath path;
	GetAppInfo(&ai);
	BEntry entry(&ai.ref);
	entry.GetPath(&path);
	path.GetParent(&path);
	chdir(path.Path());*/
	
	BRect r(200, 200, 400, 400);
	fS9xWindow = new S9xMainWindow(r);
	fS9xWindow->Show();
}

void
S9xApp::RefsReceived(BMessage *m)
{
	if(fS9xWindow) 
		fS9xWindow->PostMessage(m);
}


BWindow*
S9xApp::GetWindow()
{
	if(fS9xWindow) 
		return fS9xWindow;
}


void
S9xApp::AboutRequested(void)
{
	BString text;
	text << "BeS9x " << VERSION << " Super Nes Emulator"
	//<< "\n(All-in-Wonder revision)(Super Nes Emulator)" 
	<< "\n\n" << "Emulation Core: "
	<< "\nby The Snes9x team"
	<< "\n(http://www.snes9x.com)" 
	<< "\n\nBeOS version: "
	<< " \nby Caz (Carwyn Jones)"
	<< "\n\nUpdated to 1.43 + some changes by:"
	<< "\nModeenF (Fredrik Modéen)"
	<< "\n\nThanks to: "
	<< "\nChristian Corona - for his excellent feedback and bug reports, creating the icon for BeS9x and many other interface ideas."
	<< "\nThe Snes9x team for creating a superb Snes emulator."
	<< "\nDerek Liauw Kie Fa for his excellent 2xSaI graphics routines."
	<< "\nGoodOldGames - testing and ideas"
	<< "\nJack Burton - testing and ideas"
	<< "\nJess Tipton - bug reports/feedback"
	<< "\nAlienSoldier - bug reports/feedback";
	(new BAlert("snes9x", text.String(), "Ok", NULL, NULL, B_WIDTH_FROM_LABEL))->Go(NULL);		
}


void
S9xApp::MessageReceived(BMessage* m)
{
	m->PrintToStream();
    switch (m->what) {
        default:
            BApplication::MessageReceived(m);
    }
}

//--------------------------------------

int main()
{
	S9xApp a;
	a.Run();
	return(0);
}
