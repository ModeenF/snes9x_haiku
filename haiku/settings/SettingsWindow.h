#ifndef _SETTINGSWINDOW_H
#define _SETTINGSWINDOW_H

#include <Window.h>

#include "input_interface.h"

class SettingTabView;
class BButton;

class SettingsWindow : public BWindow
{
public:
	SettingsWindow(BRect frame,const char * title, int32 selectTab);
	~SettingsWindow();
	void MessageReceived(BMessage *msg);

protected:
	SettingTabView*		fSettingTabView;
};

#endif //_SETTINGSWINDOW_H
