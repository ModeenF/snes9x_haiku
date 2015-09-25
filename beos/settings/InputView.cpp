#include "InputView.h"

InputView::InputView(BRect rect, const char* name, float tabHeight)
	: BView(rect, name, B_FOLLOW_NONE, B_WILL_DRAW)//,
{
	SetViewColor(216, 216, 216);
	int space = 5;
	int boxHight = 100;
	int boxWidth = Bounds().Width() / 2;
	int menuWidth = 125;
	int menuTextWidth = 125;
	
// ---------------------
	BRect boxRect = BRect(Bounds().left + space, Bounds().top + space, Bounds().left + boxWidth, Bounds().top + boxHight);
	fBoxGeneral = new BBox(boxRect, "fBoxGeneral", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE, B_FANCY_BORDER);
	fBoxGeneral->SetLabel("General");
	AddChild(fBoxGeneral);
	
	BMenu* menu = new BMenu("Choose Controller");
	BMenuField* menuField = new BMenuField(BRect(boxRect.left + space, boxRect.top + space*2, menuWidth, 15), "Name", "Controller", menu);
	menu->AddItem(new BMenuItem("Controller 1",new BMessage(S9x_CONTROLLER1)));
	menu->AddItem(new BMenuItem("Controller 2",new BMessage(S9x_CONTROLLER2)));
	menu->AddItem(new BMenuItem("Controller 3",new BMessage(S9x_CONTROLLER3)));
	menu->AddItem(new BMenuItem("Controller 4",new BMessage(S9x_CONTROLLER4)));
	menu->AddItem(new BMenuItem("Controller 5",new BMessage(S9x_CONTROLLER5)));
	fBoxGeneral->AddChild(menuField);
	
	BFont headerFont(be_bold_font);
	headerFont.SetSize(14.0f);
	font_height fh1;
	headerFont.GetHeight(&fh1);
	BRect textRect1 = BRect(Bounds().left + space, boxRect.bottom + space, Bounds().left + menuTextWidth, (boxRect.bottom + space) + fh1.leading + fh1.ascent + 10);

	fStrView = new BStringView(textRect1, "header", "", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);
	fStrView->SetFont(&headerFont);
	fStrView->SetAlignment(B_ALIGN_CENTER);
	AddChild(fStrView);

//	fS9xConfigView = new S9xConfigView(BRect(Bounds().left + space, textRect1.bottom + space, Bounds().left + 510, textRect1.bottom + space + 240));
//	AddChild(fS9xConfigView);
//	SetPulseRate(100000.0);
}

/*
void 
InputView::SetInput(uint32 *data, input_interface *buffer, uint8 &allow)
{
//	fS9xConfigView->MakeFocus();
//	fS9xConfigView->SetKeys(data);
//	fS9xConfigView->SetInput(true);
//	fS9xConfigView->SetInput(buffer, allow);
}*/


void //const char* text, uint32 *data, input_interface *buffer, uint8 &allow
InputView::SetController(BString name, uint32* controll, input_interface* key, uint8* allow)
{
	fStrView->SetText(name.String());
//	fS9xConfigView->MakeFocus();
//	fS9xConfigView->SetKeys(controll);
//	fS9xConfigView->SetInput(true);
//	fS9xConfigView->SetInput(key, allow);
}

void
InputView::MessageReceived(BMessage *m)
{
	//m->PrintToStream();
	switch(m->what)
	{
/*		case S9x_CONTROLLER1:
			fStrView->SetText("Controller 1");
			printf("S9x_CONTROLLER1\n");
			break;
		case S9x_CONTROLLER2:
			fStrView->SetText("Controller 2");
			printf("S9x_CONTROLLER2\n");
			break;
		case S9x_CONTROLLER3:
			fStrView->SetText("Controller 3");
			printf("S9x_CONTROLLER3\n");
			break;
		case S9x_CONTROLLER4:
			fStrView->SetText("Controller 4");
			printf("S9x_CONTROLLER4\n");
			break;
		case S9x_CONTROLLER5:
			fStrView->SetText("Controller 5");
			printf("S9x_CONTROLLER5\n");
			break;*/
/*		case OK:
			CopyControls();
			Window()->PostMessage(B_QUIT_REQUESTED);
			break;
		case CANCEL:
			Window()->PostMessage(B_QUIT_REQUESTED);
			break;
		case CLEAR:
			ClearControls();
			break;*/
		default:
			BView::MessageReceived(m);
			break;
	}
}

/*

void
InputView::Pulse()
{
	Window()->Lock();
	Draw(Bounds());
	Window()->Unlock();
}


void
InputView::Draw(BRect updateRect)
{
	if(!Window()->IsActive())
		return;
	int keystroke = 0;
	const int len = GetLen();
	
	for(int i = 0; i < len; i++) {
		if(fKey[i].pressed){
			keystroke = i;
			break;
		}
	}
	
	if(IsInput() && keystroke) {
		acquire_sem_etc(fSem, 1, B_TIMEOUT, 200000);
		Insert(fKey[keystroke].name, keystroke);
		fKey[keystroke].pressed = 0;
	}
}


void
InputView::Insert(const BString &pad, int value)
{
	int32 i;
	S9xStringView *text;

	for(i = 0; i < SNES9x_MAX; i++) {
		text = (S9xStringView *)fInputSviews[i];
		if (text->IsFocus()) {
			fInputSviews[i]->SetText(pad.String());
			fControls[i] = value;
			break;
		}
	}
	
	switch(i) {
		case 0:
			fInputSviews[1]->MakeFocus(true);
		break;
		case 1:
			fInputSviews[2]->MakeFocus(true);
		break;
		case 2:
			fInputSviews[3]->MakeFocus(true);
		break;
		case 3:
			fInputSviews[4]->MakeFocus(true);
		break;
		case 4:
			fInputSviews[5]->MakeFocus(true);
		break;
		case 5:
			fInputSviews[6]->MakeFocus(true);
		break;
		case 6:
			fInputSviews[7]->MakeFocus(true);
		break;
		case 7:
			fInputSviews[8]->MakeFocus(true);
		break;
		case 8:
			fInputSviews[9]->MakeFocus(true);
		break;
		case 9:
			fInputSviews[10]->MakeFocus(true);
		break;
		case 10:
			fInputSviews[11]->MakeFocus(true);
		break;
		case 11:
			fInputSviews[11]->MakeFocus(false);
		break;
	}
}


int
InputView::GetLen()
{
	//return fIsAllowed ? 512 : 128;
	return settings.ui.s9x_allow ? 512 : 128;
}


void
InputView::ChangeLabel(int index, int interface)
{
	//fInputSviews[index]->SetText(fKey[interface].name.String());
}


void
InputView::SetKeys(uint32 *fWhich)
{
	this->fWhich = fWhich;
	for(int i = 0; i < SNES9x_MAX; i++) {
		ChangeLabel(i, fWhich[i]);
		fControls[i] = fWhich[i];
	}
}


void
InputView::ClearControls()
{
	for(int i = 0; i < SNES9x_MAX; i++) {
		fControls[i] = 0;
		ChangeLabel(i, fControls[i]);
	}
}


void
InputView::CopyControls()
{
	for(int i = 0; i < SNES9x_MAX; i++) {
		fWhich[i] = fControls[i];
	}
}
*/

InputView::~InputView()
{
/*	if(fSem)
		delete_sem(fSem);
	if(fTileBitmap) 
		delete fTileBitmap;*/
}

