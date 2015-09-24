#include <Window.h>
#include <View.h>
#include <Message.h>
#include "s9x_config_window.h"
#include "s9x_input_view.h"

s9x_config_window::s9x_config_window(const char *title, uint32 *data, BWindow *parent,
	uint32 msgwhat, input_interface *buffer, uint8 &allow)
	: BWindow(BRect(200, 200, 710, 440), title, B_FLOATING_WINDOW,
		B_NOT_CLOSABLE | B_NOT_ZOOMABLE | B_NOT_RESIZABLE),
	handle(parent),
	s9xmsg(msgwhat)
{
	view = new s9x_input_view(Bounds(), buffer, allow);
	AddChild(view);
	SetPulseRate(100000.0);
	view->MakeFocus();
	view->SetKeys(data);
	view->SetInput(true);
	Show();
}

void s9x_config_window::Quit()
{
	handle->PostMessage(s9xmsg);
	BWindow::Quit();
}
