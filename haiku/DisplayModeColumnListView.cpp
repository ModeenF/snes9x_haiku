/******************************************************************************
 * $Id: DisplayModeColumnListView.cpp
 *
 * Authors:
 *		Fredrik Modéen <fredrik@modeen.se>
 *
 *****************************************************************************/

#include "DisplayModeColumnListView.h"
#include "DisplayModeField.h"
#include "s9x_constants.h"
#include "S9xApp.h"

#include "s9x_settings.h"

#include <stdio.h>

DisplayModeColumnListView::DisplayModeColumnListView(BRect rect, const char *name, uint32 resizingMode,
	uint32 drawFlags) 
	: BColumnListView (rect, name, resizingMode, drawFlags){}

DisplayModeColumnListView::~DisplayModeColumnListView(){}


void
DisplayModeColumnListView::SelectionChanged()
{
	BRow* row = CurrentSelection();
	BField* field = row->GetField(0);
	DisplayModeField *stringField = static_cast<DisplayModeField *>(field);
	display_mode dm = stringField->GetDisplayMode();
	//printf("SelectionChanged, Field = %s\n", stringField->String());
	BMessage *msg = new BMessage(S9x_DISPLAYMODE);
	msg->AddPointer("DirectMode", &dm);
	//msg->AddPointer("refr_rate", (void *)&modes[i]);
/*	status_t AddData(const char *name, type_code type, 
      const void *data, 
      ssize_t numBytes, 
      bool fixedSize = true, 
      int32 numItems = 1)*/
	((S9xApp *)be_app)->GetWindow()->PostMessage(msg);
}

