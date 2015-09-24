#include "SettingTabView.h"
#include <stdio.h>

#include "s9x_constants.h"

/***** Constructor ****/
SettingTabView::SettingTabView(BRect frame,const char *name)
: BTabView(frame,name)
{
	BRect rectTabs = Bounds();
	fDisplayView = new DisplayView(rectTabs, "Display", TabHeight());
//	fEmulatorView = new EmulatorView(rectTabs, "Emulator", TabHeight());
//	fInputView = new InputView(rectTabs, "Input", TabHeight());
	fSoundView = new SoundView(rectTabs, "Sound", TabHeight());
	
	AddTab(fDisplayView, new BTab);
//	AddTab(fEmulatorView, new BTab);
//	AddTab(fInputView, new BTab);
	AddTab(fSoundView, new BTab);	
}

/***** Destructor ****/
SettingTabView::~SettingTabView()
{
}


void 
SettingTabView::SetController(BString name, uint32* controll, input_interface* key, uint8* allow)
{
	fInputView->SetController(name, controll, key, allow);
}


void
SettingTabView::MessageReceived(BMessage *msg)
{
//	msg->PrintToStream();
	switch(msg->what) {
		default:
			BView::MessageReceived(msg);
			break;
	}
}

