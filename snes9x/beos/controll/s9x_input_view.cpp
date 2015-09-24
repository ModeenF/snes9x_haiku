#include <Window.h>
#include <Message.h>
#include <View.h>
#include <TranslationUtils.h>
#include <Box.h>
#include <String.h>
#include <Bitmap.h>
#include <Button.h>
#include "input_interface.h"
#include "s9x_string_view.h"
#include "s9x_input_view.h"

const uint32 CLEAR = 'clea';
const uint32 OK = 'okcl';
const uint32 CANCEL = 'cacl';

s9x_input_view::s9x_input_view(BRect rect, input_interface *interface, uint8 &allow)
	: BView(rect, "", B_FOLLOW_ALL, B_WILL_DRAW | B_PULSE_NEEDED),
	key(interface),
	input(false),
	tile_bitmap(0),
	isAllowed(allow)
{
	tile_bitmap = BTranslationUtils::GetBitmap('RAWT',"BEOS:CONTROL");
	
	BRect mRect(70, 70, 120, 90); // Up
	input_sviews[SNES9x_UP] = new s9x_string_view(mRect, "Up");
	AddChild(input_sviews[SNES9x_UP]);
	
	mRect.Set(70, 200, 120, 220); // Down
	input_sviews[SNES9x_DOWN] = new s9x_string_view(mRect, "Down");
	AddChild(input_sviews[SNES9x_DOWN]);
	
	mRect.Set(10, 140, 60, 160); // Left
	input_sviews[SNES9x_LEFT] = new s9x_string_view(mRect, "Left");
	AddChild(input_sviews[SNES9x_LEFT]);
	
	mRect.Set(130, 140, 200, 160); // Right
	input_sviews[SNES9x_RIGHT] = new s9x_string_view(mRect, "Right");
	AddChild(input_sviews[SNES9x_RIGHT]);
	
	mRect.Set(450, 140, 510, 160); // a
	input_sviews[SNES9x_A] = new s9x_string_view(mRect, "a");
	AddChild(input_sviews[SNES9x_A]);
	
	mRect.Set(350, 180, 390, 200); // b
	input_sviews[SNES9x_B] = new s9x_string_view(mRect, "b");
	AddChild(input_sviews[SNES9x_B]);
	
	mRect.Set(380, 80, 420, 100); // x
	input_sviews[SNES9x_X] = new s9x_string_view(mRect, "x");
	AddChild(input_sviews[SNES9x_X]);
	
	mRect.Set(320, 130, 360, 150); // y
	input_sviews[SNES9x_Y] = new s9x_string_view(mRect, "y");
	AddChild(input_sviews[SNES9x_Y]);
	
	mRect.Set(70, 20, 110, 40); // L
	input_sviews[SNES9x_L] = new s9x_string_view(mRect, "L");
	AddChild(input_sviews[SNES9x_L]);
	
	mRect.Set(380, 20, 420, 40); // R
	input_sviews[SNES9x_R] = new s9x_string_view(mRect, "R");
	AddChild(input_sviews[SNES9x_R]);
	
	mRect.Set(260, 120, 300, 140); // Start
	input_sviews[SNES9x_START] = new s9x_string_view(mRect, "Start");
	AddChild(input_sviews[SNES9x_START]);
	
	mRect.Set(195, 120, 240, 140); // Select
	input_sviews[SNES9x_SELECT] = new s9x_string_view(mRect, "Select");
	AddChild(input_sviews[SNES9x_SELECT]);
	
	AddChild(ok = new BButton(BRect(270, 210, 340, 235), "", "Ok", new BMessage(OK)));
	ok->ResizeToPreferred();
	AddChild(cancel = new BButton(BRect(350, 210, 410, 235), "", "Cancel", new BMessage(CANCEL)));
	cancel->ResizeToPreferred();
	AddChild(clear = new BButton(BRect(430, 210, 500, 235), "", "Clear", new BMessage(CLEAR)));
	clear->ResizeToPreferred();
	
	sem = create_sem(0, "joy_sem");
}

void s9x_input_view::AttachedToWindow()
{
	clear->SetTarget(this);
	ok->SetTarget(this);
	cancel->SetTarget(this);
	SetViewBitmap(tile_bitmap);
	BView::AttachedToWindow();
}

void s9x_input_view::MessageReceived(BMessage *m)
{
	switch(m->what)
	{
		case OK:
			CopyControls();
			Window()->PostMessage(B_QUIT_REQUESTED);
			break;
		case CANCEL:
			Window()->PostMessage(B_QUIT_REQUESTED);
			break;
		case CLEAR:
			ClearControls();
			break;
		default:
			BView::MessageReceived(m);
			break;
	}
}

void s9x_input_view::Pulse()
{
	Window()->Lock();
	Draw(Bounds());
	Window()->Unlock();
}

void s9x_input_view::Draw(BRect updateRect)
{
	if(!Window()->IsActive()) return;
	int keystroke = 0;
	const int len = GetLen();
	
	for(int i = 0; i < len; i++) {
		if(key[i].pressed) { keystroke = i; break; }
	}
	
	if(IsInput() && keystroke) {
		acquire_sem_etc(sem, 1, B_TIMEOUT, 200000);
		Insert(key[keystroke].name, keystroke);
		key[keystroke].pressed = 0;
	}
}

void s9x_input_view::Insert(const BString &pad, int value)
{
	int32 i;
	s9x_string_view *text;

	for(i = 0; i < SNES9x_MAX; i++) {
		text = (s9x_string_view *)input_sviews[i];
		if (text->IsFocus()) {
			input_sviews[i]->SetText(pad.String());
			controls[i] = value;
			break;
		}
	}
	
	switch(i) {
		case 0:
			input_sviews[1]->MakeFocus(true);
		break;
		case 1:
			input_sviews[2]->MakeFocus(true);
		break;
		case 2:
			input_sviews[3]->MakeFocus(true);
		break;
		case 3:
			input_sviews[4]->MakeFocus(true);
		break;
		case 4:
			input_sviews[5]->MakeFocus(true);
		break;
		case 5:
			input_sviews[6]->MakeFocus(true);
		break;
		case 6:
			input_sviews[7]->MakeFocus(true);
		break;
		case 7:
			input_sviews[8]->MakeFocus(true);
		break;
		case 8:
			input_sviews[9]->MakeFocus(true);
		break;
		case 9:
			input_sviews[10]->MakeFocus(true);
		break;
		case 10:
			input_sviews[11]->MakeFocus(true);
		break;
		case 11:
			input_sviews[11]->MakeFocus(false);
		break;
	}
}

int s9x_input_view::GetLen()
{
	return isAllowed ? 512 : 128;
}

void s9x_input_view::ChangeLabel(int index, int interface)
{
	input_sviews[index]->SetText(key[interface].name.String());
}

void s9x_input_view::SetKeys(uint32 *which)
{
	this->which = which;
	for(int i = 0; i < SNES9x_MAX; i++) {
		ChangeLabel(i, which[i]);
		controls[i] = which[i];
	}
}

void s9x_input_view::ClearControls()
{
	for(int i = 0; i < SNES9x_MAX; i++) {
		controls[i] = 0;
		ChangeLabel(i, controls[i]);
	}
}

void s9x_input_view::CopyControls()
{
	for(int i = 0; i < SNES9x_MAX; i++) {
		which[i] = controls[i];
	}
}

s9x_input_view::~s9x_input_view()
{
	delete_sem(sem);
	if(tile_bitmap) delete tile_bitmap;
}
