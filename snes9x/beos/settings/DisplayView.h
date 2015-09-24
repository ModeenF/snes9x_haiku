#ifndef _DISPLAYVIEW_H
#define _DISPLAYVIEW_H

#include <View.h>

#include <Window.h>
#include <Screen.h>
#include <String.h>
#include <Menu.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <stdio.h>

#include "DisplayModeColumnListView.h"
#include "DisplayModeField.h"

#include "s9x_settings.h"

const uint32 ID_REFRESH = 'refr';

class BBox;
class BCheckBox;

class DisplayView : public BView
{
public:
	DisplayView(BRect frame, const char* name, float tabHeight);
	~DisplayView();
	
	virtual void MessageReceived(BMessage *m);

private:
	void 		_FillRefreshSettings();
	void 		_CreateFileList();
//---------------------------------------
	BBox*		fBoxGeneral;
	BCheckBox*	fChkboxFulscreen;
	BCheckBox*	fChkboxBiLiner;
	BCheckBox*	fChkboxShowFrameRate;
	BCheckBox*	fChkboxStretchImage;
	
//---------------------------------------
	BBox*		fBoxOutputImageProcessing;
	
//---------------------------------------
	BBox*		fBoxSnesImage;	
	BCheckBox*	fChkboxExtendHeight;
	BCheckBox*	fChkboxRender16Bit;
	BCheckBox*	fChkboxTransparency;
	BCheckBox*	fChkboxHiRes;

//---------------------------------------
	BBox*						fBoxFullScreen;
	DisplayModeColumnListView* 	fColumnList;
	uint32 						fS9xMsg;
};

#endif //_DISPLAYVIEW_H
