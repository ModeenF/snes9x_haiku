#include "DisplayView.h"

#include <stdlib.h>
#include <stdio.h>

#include <Box.h>
#include <CheckBox.h>
#include <Application.h>

#include "s9x_constants.h"

extern "C" s9x_settings settings;

/** helper to sort modes by resolution */
/* compare_mode are from ScreenMode.cpp */
/*
 * Copyright 2005, Haiku.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Axel Dörfler, axeld@pinc-software.de
 */
static float
get_refresh_rate(display_mode& mode)
{
	// we have to be catious as refresh rate cannot be controlled directly,
	// so it suffers under rounding errors and hardware restrictions
	return rint(10 * float(mode.timing.pixel_clock * 1000) / 
		float(mode.timing.h_total * mode.timing.v_total)) / 10.0;
}


static int
compare_mode(const void* _mode1, const void* _mode2)
{
	display_mode *mode1 = (display_mode *)_mode1;
	display_mode *mode2 = (display_mode *)_mode2;

	uint16 width1, width2, height1, height2;
	width1 = mode1->virtual_width;
	height1 = mode1->virtual_height;
	width2 = mode2->virtual_width;
	height2 = mode2->virtual_height;

	if (width1 != width2)
		return width1 - width2;

	if (height1 != height2)
		return height1 - height2;

	return (int)(10 * get_refresh_rate(*mode1) - 10 * get_refresh_rate(*mode2));
}


/***** Constructor ****/
DisplayView::DisplayView(BRect frame,const char *name, float tabHeight)
	: BView(frame, name, B_FOLLOW_NONE, B_WILL_DRAW)//,
{
	SetViewColor(216, 216, 216);
	int space = 5;
	int boxHight = 200;
	int boxWidth = (Bounds().Width() / 2) / 2;
	int chkBoxWidth = 15;
	int chkBoxHight = 15;
	
// ---------------------
	BRect boxRect = BRect(Bounds().left + space, Bounds().top + space, Bounds().left + boxWidth, Bounds().top + boxHight);
	fBoxGeneral = new BBox(boxRect, "fBoxGeneral", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE, B_FANCY_BORDER);
	fBoxGeneral->SetLabel("General");
	
	BRect rect = BRect(boxRect.left + space, boxRect.top + space, boxRect.Width() - space, boxRect.top + chkBoxWidth);
	
/*	fChkboxFulscreen = new BCheckBox(rect, "fChkboxFulscreen", "Full Screen", new BMessage(S9x_FULLSCREEN),B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	fChkboxFulscreen->SetValue(settings.ui.S9x_FULLSCREEN);
	fBoxGeneral->AddChild(fChkboxFulscreen);*/
	
//	rect.OffsetBy(0, space + chkBoxHight);
	fChkboxBiLiner = new BCheckBox(rect, "fChkboxBiLiner", "Bi-Linear Mode 7", new BMessage(S9x_BILINER7), B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);	
	fChkboxBiLiner->SetValue(settings.graphics.s9x_Mode7Interpolate);
	fBoxGeneral->AddChild(fChkboxBiLiner);

	rect.OffsetBy(0, space + chkBoxHight);	
	fChkboxShowFrameRate = new BCheckBox(rect, "fChkboxShowFrameRate", "Show Frame Rate", new BMessage(S9x_SHOWFRAMERATE), B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);	
	fChkboxShowFrameRate->SetValue(settings.graphics.s9x_DisplayFrameRate);
	fBoxGeneral->AddChild(fChkboxShowFrameRate);

/*	rect.OffsetBy(0, space + chkBoxHight);
	fChkboxStretchImage = new BCheckBox(rect, "fChkboxStretchImage", "Stretch Image", new BMessage(S9x_STRETSHIMAGE), B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);	
//	fChkboxStretchImage->SetValue(settings.ui.s9x_extended);
	fBoxGeneral->AddChild(fChkboxStretchImage);*/

	AddChild(fBoxGeneral);

// ---------------------
	boxRect.OffsetBy(space + boxRect.Width(), 0);
	fBoxSnesImage = new BBox(boxRect, "fBoxSnesImage", B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE, B_FANCY_BORDER);
	fBoxSnesImage->SetLabel("Snes Image");

	boxRect.OffsetBy(-(boxRect.Width()+ space), 0);
	rect = BRect(boxRect.left + space, boxRect.top + space, boxRect.Width() - space, boxRect.top + chkBoxHight);
	fChkboxExtendHeight = new BCheckBox(rect, "fChkboxExtendHeight", "Extend Height", new BMessage(S9x_EXTENDED), B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);	
	fChkboxExtendHeight->SetValue(settings.ui.s9x_extended);
	fBoxSnesImage->AddChild(fChkboxExtendHeight);


	rect.OffsetBy(0, space + chkBoxHight);
	fChkboxRender16Bit = new BCheckBox(rect, "fChkboxRender16Bit", "Render 16-bit", new BMessage(S9x_RENDER16BIT), B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);	
	fChkboxRender16Bit->SetValue(settings.graphics.s9x_SixteenBit);
	fBoxSnesImage->AddChild(fChkboxRender16Bit);


	rect.OffsetBy(0, space + chkBoxHight);
	fChkboxTransparency = new BCheckBox(rect, "fChkboxTransparency", "Transparency", new BMessage(S9x_TRANSPARENCY), B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);	
	fChkboxTransparency->SetValue(settings.graphics.s9x_Transparency);
	fBoxSnesImage->AddChild(fChkboxTransparency);

	rect.OffsetBy(0, space + chkBoxHight);
	fChkboxHiRes = new BCheckBox(rect, "fChkboxHiRes", "Support Hi Res", new BMessage(S9x_HIRES), B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);	
	fChkboxHiRes->SetValue(settings.graphics.s9x_SupportHiRes);
	fBoxSnesImage->AddChild(fChkboxHiRes);

	AddChild(fBoxSnesImage);

// ---------------------
//	Lock();	
	boxRect.OffsetBy(space + boxRect.Width(), 0);
	boxRect = BRect(boxRect.left, boxRect.top, boxRect.right + boxWidth - space * 3, Bounds().bottom - (tabHeight + (space*2)));
	boxRect.OffsetBy(space + boxWidth, 0);
	fBoxFullScreen = new BBox(boxRect, "fBoxFullScreen", B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE, B_FANCY_BORDER);
	fBoxFullScreen->SetLabel("Fullscreen Display Settings");
	
	rect.OffsetTo(space, space*4);
	rect = BRect(rect.left, rect.top - space, (rect.left + boxRect.Width())- space*2 , boxRect.bottom - space*2);	
	fColumnList = new DisplayModeColumnListView (rect, "ColumnList", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP_BOTTOM, B_WILL_DRAW |B_NAVIGABLE);
	fColumnList->AddColumn(new DisplayModeColumn("Video Mode", 200, 10, 150, 0),0);
	fColumnList->AddColumn(new BStringColumn("Status", 100, 10, 100, 0),1);

	fBoxFullScreen->AddChild(fColumnList);
	AddChild(fBoxFullScreen);

	display_mode *modes;
	uint32 mode_count;
	BRow *cRow;
	if (BScreen().GetModeList(&modes, &mode_count) == B_NO_ERROR) {
		qsort(modes, mode_count, sizeof(display_mode), compare_mode);
		
		display_mode prevDisplayMode;
		
		for(unsigned int i = 0; i < mode_count; i++) {			
			if(compare_mode(&prevDisplayMode, &modes[i])){
				cRow = new BRow();
				cRow->SetField(new DisplayModeField(modes[i]), 0);
				cRow->SetField(new BStringField("Supported"), 1);
				fColumnList->AddRow(cRow, (int32)i);
				prevDisplayMode = modes[i];
			}
		}
	}
//	BMessage *m = new BMessage(ID_REFRESH);
//	m->AddPointer("ColumnList", fColumnList);
//	message->AddInt32("width", mode.width);
//	message->AddInt32("height", mode.height);
//	fColumnList->SetSelectionMessage(m);
	
// ---------------------
	boxRect = BRect(Bounds().left + space, Bounds().top + boxHight + space*2, boxRect.left - space, boxHight);
	fBoxOutputImageProcessing = new BBox(boxRect, "boxOutputImageProcessing", B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE, B_FANCY_BORDER);
	fBoxOutputImageProcessing->SetLabel("Output Image Processing");
	AddChild(fBoxOutputImageProcessing);	
}

/***** Destructor ****/
DisplayView::~DisplayView()
{
}

void
DisplayView::MessageReceived(BMessage *msg)
{
//	msg->PrintToStream();
	switch(msg->what) {
		default:
			BView::MessageReceived(msg);
			break;
	}
}



