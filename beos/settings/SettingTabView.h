#ifndef _SETTINGSTABVIEW_H
#define _SETTINGSTABVIEW_H

#include <TabView.h>

#include "DisplayView.h"
#include "EmulatorView.h"
#include "InputView.h"
#include "SoundView.h"



class SettingTabView : public BTabView
{
public:
	// publics functions
	SettingTabView(BRect frame,const char * name);
	~SettingTabView();
	void	MessageReceived(BMessage *msg);
//	void	SetInput(uint32 *data, input_interface *buffer, uint8 &allow);
	void 	SetController(BString name, uint32* controll, input_interface* key, uint8* allow);

protected:
	// protected members
	DisplayView*	fDisplayView;
	EmulatorView*	fEmulatorView;
	InputView*		fInputView;
	SoundView*		fSoundView;
};

#endif //_SETTINGSTABVIEW_H
