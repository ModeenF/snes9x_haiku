#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input_interface.h"

#include "Joystick.h"

Joystick::Joystick(int index, input_interface *buffer)
	: BJoystick()
{
	this->fBuf = &buffer[KEY_END + OFFSET * index];
	this->fIndex = index;
}


void 
Joystick::InitNames()
{
	fBuf[UP].name = "";
	fBuf[UP].name << "J(" << fIndex << ") A(Up)";
	fBuf[DOWN].name = "";
	fBuf[DOWN].name << "J(" << fIndex << ") A(Down)";
	fBuf[LEFT].name = "";
	fBuf[LEFT].name << "J(" << fIndex << ") A(Left)";
	fBuf[RIGHT].name = "";
	fBuf[RIGHT].name << "J(" << fIndex << ") A(Right)";
	
	int32 numButtons = this->CountButtons();
	for(int i = 0; i < numButtons; i++) {
		fBuf[BUTTONS+i].name = "";
		fBuf[BUTTONS+i].name << "J(" << fIndex << ") " << "B(" << i << ")";
	}
}


status_t 
Joystick::Update()
{
	status_t status = BJoystick::Update();
	if(status == B_NO_ERROR) {
		UpdateAxes();
		UpdateButtons();
	}
	return(status);
}


void 
Joystick::UpdateAxes()
{
	int16 axis_count = this->CountAxes();
	int16 *axes = (int16 *) malloc(sizeof(int16) * axis_count);
	this->GetAxisValues(axes);
	fBuf[UP].pressed = axes[1] < -16384;
	fBuf[DOWN].pressed = axes[1] > 16384;
	fBuf[LEFT].pressed = axes[0] < -16384;
	fBuf[RIGHT].pressed = axes[0] > 16384;
	free(axes);
}


void
Joystick::UpdateButtons()
{
	uint32 buttons = ButtonValues();
	for(int i = 0; i < MAX_BUTTONS; i++)
		fBuf[BUTTONS+i].pressed = buttons & (1 << i) ? 1 : 0;
}
