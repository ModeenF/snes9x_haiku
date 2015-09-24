#ifndef __SYSTEM_MENU_BAR_H__
#define __SYSTEM_MENU_BAR_H__

#include <MenuBar.h>
#include <MenuItem.h>

#include "s9x_settings.h"
#include "s9x_constants.h"

class GameMenuBar : public BMenuBar
{
public:
	GameMenuBar(BRect frame, const char *title, s9x_settings s);
	virtual void MakeFocus(bool focus = true);
	virtual void SetMode(bool suportBitmap, bool suportOverlay, bool suportDirect, int index = 0);
	virtual	void SetSelectedMode(int index = 0);
	virtual void ToggleFullScreenMenu(bool what);
	virtual void ToggleRomInfoMenu(bool what);
	virtual void ToggleMenuWindowMarked(bool what, uint32 command);
	virtual bool GetMenuWindowMarked(uint32 command);
	virtual void SetItems(bool ch);
	void RecentRom(s9x_settings s);
	
private:
	void _MakeFileMenu();
	void _MakeOptionsMenu();
	void _MakeInputMenu();
	void _MakeSoundMenu();
	void _MakeWindowMenu();
	void _MakeHelpMenu();
	void _BuildRecentRom(BMenu* recentRomMenu);
	BString _GetNameFromPath(const char* path);
	
	s9x_settings 	fSettings;
	BMenu*			fFileMenu;
	BMenu*			fRecentRomMenu;
	BMenu*			fModeMenu;
	BMenu*			fGraphicsMenu;
	BMenu*			fWindowMenu;
	BMenuItem*		fItemFullscreen;
	BMenuItem*		fItemRomInfo;
};

#endif
