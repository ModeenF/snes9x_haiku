#include <Window.h>
#include <Screen.h>
#include <String.h>
#include <Menu.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <stdio.h>

#include "S9xRefreshWindow.h"

const uint32 ID_REFRESH = 'refr';

S9xRefreshWindow::S9xRefreshWindow(BRect rect, BWindow *parent, uint32 closemsg)
	: BWindow(rect, "FullScreen - Refresh Rate", B_FLOATING_WINDOW, B_NOT_ZOOMABLE | B_NOT_RESIZABLE),
	fWindow(parent),
	fS9xMsg(closemsg)
{
	display_mode *modes;
	uint32 mode_count;
	
	if(BScreen().GetModeList(&modes, &mode_count) == B_NO_ERROR) {
		BMenu *menu = new BMenu("menu");
		for(unsigned int i = 0; i < mode_count; i++) {
		//	if((modes[i].virtual_width == 640) && (modes[i].space == B_RGB16)) {
				BMessage *msg = new BMessage(ID_REFRESH);
				msg->AddPointer("refr_rate", (void *)&modes[i]);
				BString item_name;
				item_name << (modes[i].timing.pixel_clock * 1000)
					/ (modes[i].timing.h_total * modes[i].timing.v_total) << " Hz";
				menu->AddItem(new BMenuItem(item_name.String(), msg));
		//	}
		}
		BRect r(20, 20, 200, 60);
		BMenuField *field = new BMenuField(r, "field", "Refresh Rate:", menu);
		AddChild(field);
		menu->SetLabelFromMarked(true);
		menu->ItemAt(0)->SetMarked(true);
		menu->SetTargetForItems(fWindow);
	}
	Show();
}


void 
S9xRefreshWindow::Quit()
{
	fWindow->PostMessage(fS9xMsg);
	BWindow::Quit();
}
