#ifndef __SNES9X_CONFIG_WINDOW_H__
#define __SNES9X_CONFIG_WINDOW_H__

#include <MessageFilter.h>
#include "input_interface.h"

class s9x_input_view;

class s9x_config_window : public BWindow
{
public:
	s9x_config_window(const char *title, uint32 *data, BWindow *parent, uint32 msgwhat,
		input_interface *buffer, uint8 &allow);
	virtual void Quit();
private:
	s9x_input_view *view;
	BWindow *handle;
	uint32 s9xmsg;
};

#endif
