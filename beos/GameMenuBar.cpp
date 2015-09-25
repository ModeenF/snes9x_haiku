#include "GameMenuBar.h"

#include <stdio.h>

#include "S9xMainWindow.h"

GameMenuBar::GameMenuBar(BRect frame, const char *title, s9x_settings s)
	: BMenuBar(frame, title),
	fSettings(s)
{
	_MakeFileMenu();	//Load File Menu
	_MakeOptionsMenu();	//Load Options Menu	
	_MakeInputMenu();	//Load Input Menu	
	_MakeSoundMenu();	//Load Sound Menu	
	_MakeWindowMenu();	//Load Window Menu	
	_MakeHelpMenu();	//Load Help Menu
}


void 
GameMenuBar::SetMode(bool suportBitmap, bool suportOverlay, bool suportDirect, int index)
{
	fModeMenu->ItemAt(index)->SetMarked(true);
	switch(index) {
		case DIRECT:
			fModeMenu->ItemAt(1)->SetEnabled(suportOverlay);
			fModeMenu->ItemAt(2)->SetEnabled(suportBitmap);
		break;
		case OVERLAY:
			fModeMenu->ItemAt(0)->SetEnabled(suportDirect);
			fModeMenu->ItemAt(2)->SetEnabled(suportBitmap);
		break;
		case BITMAP:
			fModeMenu->ItemAt(0)->SetEnabled(suportDirect);
			fModeMenu->ItemAt(1)->SetEnabled(suportOverlay);
		break;
	}
}


void 
GameMenuBar::SetSelectedMode(int index)
{
	fModeMenu->ItemAt(0)->SetMarked(false);
	fModeMenu->ItemAt(1)->SetMarked(false);
	fModeMenu->ItemAt(2)->SetMarked(false);
	fModeMenu->ItemAt(index)->SetMarked(true);
}


void 
GameMenuBar::ToggleFullScreenMenu(bool what)
{
	fItemFullscreen->SetEnabled(what);
}


void 
GameMenuBar::ToggleRomInfoMenu(bool what)
{
	fItemRomInfo->SetEnabled(what);
}


bool
GameMenuBar::GetMenuWindowMarked(uint32 command)
{
	BMenuItem* item = fWindowMenu->FindItem(command);
	return item->IsMarked();
}


void 
GameMenuBar::ToggleMenuWindowMarked(bool what, uint32 command)
{
	BMenuItem* item = fWindowMenu->FindItem(command);
	item->SetMarked(what);
}


void
GameMenuBar::SetItems(bool ch)
{
	int count = fGraphicsMenu->CountItems();
	for(int i = 1; i < count; i++) {
		BMenuItem *ii = fGraphicsMenu->ItemAt(i);
		if(ii) 
			ii->SetEnabled(ch);
	}
}


void
GameMenuBar::MakeFocus(bool focus)
{
	BMenuBar::MakeFocus(focus);
	//dynamic_cast<s9x_main_window *>(Window())->MenuActivated(IsFocus());
}

void
GameMenuBar::_MakeFileMenu()
{
	// File Menu
	fFileMenu = new BMenu("File");
	BMenuItem *item;
	fFileMenu->AddItem(item = new BMenuItem("Load Rom", new BMessage(S9x_LOAD), 'O'));
	
	fRecentRomMenu = new BMenu("Recent Rom");
	_BuildRecentRom(fRecentRomMenu);
	fFileMenu->AddItem(fRecentRomMenu);
	
	fFileMenu->AddItem(item = new BMenuItem("Save Spc", new BMessage(S9x_SAVE_SPC), 'S'));
	BMenu *saveMenu = new BMenu("Save State");
	for(int i = 0; i < 5; i++)
	{
		BMessage *msg = new BMessage(S9x_SAVE_STATE);
		BString item_name;
		
		item_name << "Slot " << i+1;
		msg->AddInt32("save_slot", i);
		
		saveMenu->AddItem(new BMenuItem(item_name.String(), msg));
	}
	fFileMenu->AddItem(saveMenu);
	
	BMenu *loadMenu = new BMenu("Load State");
	for(int i = 0; i < 5; i++)
	{
		BMessage *msg = new BMessage(S9x_LOAD_STATE);
		BString item_name;
		
		item_name << "Slot " << i+1;
		msg->AddInt32("load_slot", i);
		
		loadMenu->AddItem(new BMenuItem(item_name.String(), msg));
	}
	fFileMenu->AddItem(loadMenu);
//	fileMenu->AddItem(item = new BMenuItem("Load MultiCard (NA?)", NULL));
//	item->SetEnabled(false);	
//	fileMenu->AddSeparatorItem();
//	fileMenu->AddItem(item = new BMenuItem("Save Other (NA)", NULL));
//	item->SetEnabled(false);	
	fFileMenu->AddItem(fItemRomInfo = new BMenuItem("Rom Info", new BMessage(S9x_ROMINFO), 'D'));
	fItemRomInfo->SetEnabled(false);
	fFileMenu->AddSeparatorItem();
		
	fFileMenu->AddItem(item = new BMenuItem("Reset", new BMessage(S9x_RESET), 'R'));
	fFileMenu->AddItem(item = new BMenuItem("Pause", new BMessage(S9x_PAUSE), 'P'));	
	fFileMenu->AddItem(item = new BMenuItem("Exit", new BMessage(B_QUIT_REQUESTED), 'Q'));
	AddItem(fFileMenu);
}


void
GameMenuBar::RecentRom(s9x_settings s)
{
	fSettings = s;	
	fRecentRomMenu->RemoveItems(0, fRecentRomMenu->CountItems(), true);
	_BuildRecentRom(fRecentRomMenu);
}


void
GameMenuBar::_BuildRecentRom(BMenu* recentRomMenu)
{
	BMessage* msg;
	if(fSettings.recentroms.recentRom1.Length() != 0)
	{
		msg = new BMessage(S9x_RECENT_ROM);
		msg->AddString("recentRom", fSettings.recentroms.recentRom1.String());
		BMenuItem* item = new BMenuItem(_GetNameFromPath(fSettings.recentroms.recentRom1.String()).String(), msg);
		recentRomMenu->AddItem(item);
	}
	
	if(fSettings.recentroms.recentRom2.Length() != 0)
	{
		msg = new BMessage(S9x_RECENT_ROM);
		msg->AddString("recentRom", fSettings.recentroms.recentRom2.String());
		BMenuItem* item = new BMenuItem(_GetNameFromPath(fSettings.recentroms.recentRom2.String()).String(), msg);
		recentRomMenu->AddItem(item);
	}
	
	if(fSettings.recentroms.recentRom3.Length() != 0)
	{
		msg = new BMessage(S9x_RECENT_ROM);
		msg->AddString("recentRom", fSettings.recentroms.recentRom3.String());
		BMenuItem* item = new BMenuItem(_GetNameFromPath(fSettings.recentroms.recentRom3.String()).String(), msg);
		recentRomMenu->AddItem(item);
	}
	
	if(fSettings.recentroms.recentRom4.Length() != 0)
	{
		msg = new BMessage(S9x_RECENT_ROM);
		msg->AddString("recentRom", fSettings.recentroms.recentRom4.String());
		BMenuItem* item = new BMenuItem(_GetNameFromPath(fSettings.recentroms.recentRom4.String()).String(), msg);
		recentRomMenu->AddItem(item);
	}
	
	if(fSettings.recentroms.recentRom5.Length() != 0)
	{
		msg = new BMessage(S9x_RECENT_ROM);
		msg->AddString("recentRom", fSettings.recentroms.recentRom5.String());
		BMenuItem* item = new BMenuItem(_GetNameFromPath(fSettings.recentroms.recentRom5.String()).String(), msg);
		recentRomMenu->AddItem(item);
	}
}

BString
GameMenuBar::_GetNameFromPath(const char* path)
{
	BString nameStr(path);
	int place = nameStr.FindLast('/');
	nameStr = nameStr.Remove(0, place+1);
	return nameStr;
}

void
GameMenuBar::_MakeOptionsMenu()
{
	// Options Menu
	BMenu *options = new BMenu("Options");

	BMenuItem *item;

//	options->AddItem(item = new BMenuItem("Display Settings", new BMessage(S9x_DISPLAYSETTINGS)));
//	item->SetEnabled(false);
	
	// options->Graphics
	fGraphicsMenu = new BMenu("Graphics");
	fGraphicsMenu->SetRadioMode(true);
#if 0
	fGraphicsMenu->AddItem(new BMenuItem("Standard", new BMessage(S9x_MODE0)));
#endif
	fGraphicsMenu->AddItem(new BMenuItem("Double", new BMessage(S9x_MODE1)));
	fGraphicsMenu->AddItem(new BMenuItem("Scanlines", new BMessage(S9x_MODE2)));
	fGraphicsMenu->AddItem(new BMenuItem("Lcd", new BMessage(S9x_MODE3)));
	fGraphicsMenu->AddItem(new BMenuItem("2xSaI", new BMessage(S9x_MODE4)));
	fGraphicsMenu->AddItem(new BMenuItem("Super2xSaI", new BMessage(S9x_MODE5)));
	fGraphicsMenu->AddItem(new BMenuItem("SuperEagle", new BMessage(S9x_MODE6)));
	fGraphicsMenu->AddItem(new BMenuItem("Max Scan(FullScreen only)", new BMessage(S9x_MODE7)));
	fGraphicsMenu->ItemAt(fSettings.ui.s9x_mode)->SetMarked(true);
	options->AddItem(fGraphicsMenu);	
	options->AddSeparatorItem();
	
	// Options->Other
	BMenu *otherMenu = new BMenu("Other");
	options->AddItem(otherMenu);	
	otherMenu->AddItem(new BMenuItem("Refresh Rate", new BMessage(S9x_LAUNCH_REFRESH)));	
	otherMenu->AddItem(item = new BMenuItem("Vsync", new BMessage(S9x_VSYNC)));
	item->SetMarked(fSettings.ui.s9x_vsync);
//	otherMenu->AddItem(item = new BMenuItem("Extended Height", new BMessage(S9x_EXTENDED)));
//	item->SetMarked(fSettings.ui.s9x_extended);	
	otherMenu->AddItem(item = new BMenuItem("Snooze", new BMessage(S9x_SNOOZE)));
	item->SetMarked(fSettings.ui.s9x_snooze);
	otherMenu->AddItem(item = new BMenuItem("Pause On InActive", new BMessage(S9x_INACTIVE)));
	item->SetMarked(fSettings.ui.s9x_inactive);
//	options->AddItem(item = new BMenuItem("Pause On MenuActive", new BMessage(S9x_MENUACTIVE)));
//	item->SetMarked(fSettings.ui.s9x_menuactive);
/*	options->AddSeparatorItem();
	options->AddItem(item = new BMenuItem("Emulator Settings", new BMessage(S9x_EMULATORSETTINGS)));*/

	AddItem(options);
}


void
GameMenuBar::_MakeInputMenu()
{
	BMenu *input = new BMenu("Input");
	
	BMenuItem *item;
	
/*	input->AddItem(item = new BMenuItem("Customize Hotkeys (NA)", NULL));
	item->SetEnabled(false);*/
	
	//Input->Controls
	BMenu *controlsMenu = new BMenu("Controls");
	controlsMenu->AddItem(new BMenuItem("Controller 1", new BMessage(S9x_CONTROLLER1)));
	controlsMenu->AddItem(new BMenuItem("Controller 2", new BMessage(S9x_CONTROLLER2)));
	controlsMenu->AddItem(new BMenuItem("Controller 3", new BMessage(S9x_CONTROLLER3)));
	controlsMenu->AddItem(new BMenuItem("Controller 4", new BMessage(S9x_CONTROLLER4)));
	controlsMenu->AddItem(new BMenuItem("Controller 5", new BMessage(S9x_CONTROLLER5)));
	controlsMenu->AddItem(item = new BMenuItem("Joysticks Enabled", new BMessage(S9x_ALLOW)));
	item->SetMarked(fSettings.ui.s9x_allow);
	input->AddItem(controlsMenu);

/*	input->AddSeparatorItem();
	input->AddItem(item = new BMenuItem("Input Settings", new BMessage(S9x_INPUTSETTINGS)));*/
	AddItem(input);
}


void
GameMenuBar::_MakeSoundMenu()
{
	BMenu *sound = new BMenu("Sound");
	BMenuItem *item;

	//Sound->PlaybackRate
	BMenu *soundMenu = new BMenu("Playback Rate");
	soundMenu->AddItem(item = new BMenuItem("Mute", new BMessage(S9x_MUTE), 'M'));	
	item->SetMarked(fSettings.sound.s9x_mute);
	soundMenu->AddSeparatorItem();
	sound->AddItem(soundMenu);

/*	sound->AddItem(item = new BMenuItem("Buffer Length (NA)", NULL));
	item->SetEnabled(false);
	sound->AddItem(item = new BMenuItem("Channels (NA)", NULL));
	item->SetEnabled(false);
	sound->AddItem(item = new BMenuItem("16-bit Sound (NA)", NULL));
	item->SetEnabled(false);*/
//	sound->AddItem(item = new BMenuItem("Stereo", new BMessage(S9x_STEREO)));
//	item->SetMarked(fSettings.ui.s9x_stereo);
/*	sound->AddItem(item = new BMenuItem("Reverse Stereo (NA)", NULL));
	item->SetEnabled(false);
	sound->AddSeparatorItem();

	sound->AddItem(item = new BMenuItem("Interpolated (NA)", NULL));
	item->SetEnabled(false);
	sound->AddItem(item = new BMenuItem("Sync Sound (NA)", NULL));
	item->SetEnabled(false);*/
	sound->AddSeparatorItem();
	sound->AddItem(item = new BMenuItem("Sound Settings", new BMessage(S9x_SOUNDSETTINGS)));
	AddItem(sound);
}


void
GameMenuBar::_MakeWindowMenu()
{
	fWindowMenu = new BMenu("Window");
	BMenuItem *item;
		
	fWindowMenu->AddItem(item = new BMenuItem("Hide MenuBar", new BMessage(S9x_HIDEMENU),'M', B_COMMAND_KEY));
	fWindowMenu->AddItem(fItemFullscreen = new BMenuItem("FullScreen", new BMessage(S9x_FULLSCREEN), 'F', B_COMMAND_KEY));
//	fWindowMenu->AddItem(item = new BMenuItem("Strech Image (NA)", NULL));
//	item->SetEnabled(false);
	fWindowMenu->AddSeparatorItem();
	
	//Window->Mode
	fModeMenu = new BMenu("Windows Mode");
	//fModeMenu->SetRadioMode(true);	
	fModeMenu->AddItem(new BMenuItem("DirectWindow", new BMessage(S9x_DIRECTWINDOW)));
	fModeMenu->AddItem(new BMenuItem("Overlay", new BMessage(S9x_OVERLAY)));
	fModeMenu->AddItem(new BMenuItem("Bitmap", new BMessage(S9x_BITMAP)));
	fModeMenu->ItemAt(fSettings.ui.s9x_drawmode)->SetMarked(true);
	fWindowMenu->AddItem(fModeMenu);
	fWindowMenu->AddSeparatorItem();
	fWindowMenu->AddItem(item = new BMenuItem("Display Settings", new BMessage(S9x_DISPLAYSETTINGS)));
	AddItem(fWindowMenu);
}


void
GameMenuBar::_MakeHelpMenu()
{
	BMenu *help = new BMenu("Help");
	help->AddItem(new BMenuItem("About BeS9x", new BMessage(B_ABOUT_REQUESTED)));
	AddItem(help);
}
