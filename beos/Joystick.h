#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include <Joystick.h>

#define MAX_BUTTONS 32
#define OFFSET 32
#define KEY_END 32

class input_interface;

class Joystick : public BJoystick {
public:
	Joystick(int index, input_interface *buffer);
	enum { UP, DOWN, LEFT, RIGHT, BUTTONS };
	virtual void UpdateAxes();
	virtual void UpdateButtons();
	virtual status_t Update();
	virtual void InitNames();
private:
	input_interface*	fBuf;
	int 				fIndex;
};

#endif
