#ifndef __INPUT_INTERFACE_H__
#define __INPUT_INTERFACE_H__

#include <String.h>

class input_interface
{
public:
	input_interface() : pressed(0), name("unknown") {}
	int pressed;
	BString name;
};

#endif
