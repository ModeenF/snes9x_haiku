#include "SettingsWindow.h"
#include <Button.h>
#include <Message.h>

#include "SettingTabView.h"

#include "s9x_constants.h"
#include "S9xApp.h"

#include "s9x_settings.h"

extern "C" s9x_settings settings;


/***** Constructor ****/
SettingsWindow::SettingsWindow(BRect frame,const char *title, int32 selectTab)
: BWindow(frame, title, B_DOCUMENT_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL,B_NOT_ZOOMABLE,0)
{
	int space = 5;

	BView* view = new BView(Bounds(),"View1",B_FOLLOW_NONE,B_WILL_DRAW);
	view->SetViewColor(216, 216, 216);
	
	BRect tabView = BRect(Bounds().left, Bounds().top, Bounds().right, Bounds().bottom - space);
	tabView.InsetBy(5,5);
	fSettingTabView = new SettingTabView(tabView,"SettingTabView");
	fSettingTabView->Select(selectTab);
	
	view->AddChild(fSettingTabView);
	AddChild(view);
}

SettingsWindow::~SettingsWindow()
{
}


void
SettingsWindow::MessageReceived(BMessage *msg)
{
//	msg->PrintToStream();
	switch(msg->what) {
		case S9x_CONTROLLER1:
//			fSettingTabView->SetController("Controller 1", "S9x - Controller 1", (uint32 *)&settings.s9x_controller1, key, settings.ui.s9x_allow);
			break;
		case S9x_CONTROLLER2:
//			fSettingTabView->SetController("Controller 2");
			printf("S9x_CONTROLLER2\n");
			break;
		case S9x_CONTROLLER3:
//			fSettingTabView->SetController("Controller 3");
			printf("S9x_CONTROLLER3\n");
			break;
		case S9x_CONTROLLER4:
//			fSettingTabView->SetController("Controller 4");
			printf("S9x_CONTROLLER4\n");
			break;
		case S9x_CONTROLLER5:
//			fSettingTabView->SetController("Controller 5");
			printf("S9x_CONTROLLER5\n");
			break;
		/*
		case ID_REFRESH:
		{
			printf("ID_REFRESH\n");
			BColumnListView* columnList;
			msg->FindPointer("ColumnList", (void **) columnList);
			BRow* row = columnList->CurrentSelection();
			//BStringField* filed =  dynamic_cast<BStringField *>(row->GetField(0));
			//printf("Data = %s\n", filed->String());
		}break;			*/
		case S9x_INPUTSETTINGS:
		{
			const uint8* allow;
			ssize_t size;
			// Kolla hur man fixar detta med att skicka pointer eller datat 
			// på annat sätt. kan användas när man väljer control i menyn också. men då msåte man ta new BMessage(S9x_INPUTSETTINGS) och 
			// göra en som kommer till denna efter att ha plockats upp i main med S9x_CONTROLLERX
			s9x_keys contoll;
			input_interface key;
			BString name;
			msg->FindPointer("key", (void **) &key);
			msg->FindPointer("s9x_controller", (void **) &contoll);
			//msg->FindBool("s9x_allow", &allow);
			msg->FindData("s9x_allow", B_UINT8_TYPE, (const void**) &allow, &size);
			msg->FindString("name", &name);
			//printf("name = %s\n", name.String());
//			(uint32 *)&settings.s9x_controller1, key, settings.ui.s9x_allow)
			fSettingTabView->SetController(name, (uint32 *)&settings.controll.s9x_controller1, &key, const_cast<uint8*>(allow));
//			msg->AddBool("s9x_allow", settings.ui.s9x_allow);
			
		}break;
		//case ID_REFRESH:
		case S9x_BILINER7:
		case S9x_SHOWFRAMERATE:
		case S9x_STRETSHIMAGE:
		case S9x_RENDER16BIT:
		case S9x_TRANSPARENCY:
		case S9x_HIRES:
		case S9x_MUTE:
		case S9x_STEREO:
		case S9x_REVERTSTEREO:
		case S9x_GAUSSIAN:
		case S9x_GENERATESAMPEL:
		case S9x_VOLUMEHEIGHT:
		case S9x_ECHO:
		case S9x_CACHEDECODER:
		case S9x_MASTERVOLUME:
		case S9x_ANTIRESON:
//		case S9x_FAKEMUTE:
		case S9x_ENABLE:
		case S9x_16BITPLAYBACK:
//		case S9x_FRAMEMUTE:
		case S9x_EXTENDED:
			((S9xApp *)be_app)->GetWindow()->PostMessage(msg);
			break;
		default:
			BWindow::MessageReceived(msg);
			break;
	}
}

