#include "s9x_key_names.h"
#include "input_interface.h"

#include "GameStick.h"

GameStick::GameStick(input_interface *buffer)
	:fJbuffer(buffer)
{
	for(int i = 0; i < MAX_JOYSTICKS; i++)
		fSticks[i] = 0;
	
	BJoystick stick;
	char devName[B_OS_NAME_LENGTH];
	for(int i = 0; i < stick.CountDevices() && i < MAX_JOYSTICKS; i++) {
		fSticks[i] = new Joystick(i, buffer);
		if((fSticks[i]->GetDeviceName(i, devName) != B_OK) || 
			(fSticks[i]->Open(devName, true) < B_OK)) {
			delete fSticks[i];
			fSticks[i] = 0;
		} else
			fSticks[i]->InitNames();
	}
	InitKeyNames();
}


GameStick::~GameStick()
{
	for(int i = 0; i < MAX_JOYSTICKS; i++)
		if(fSticks[i]) {
			fSticks[i]->Close();
			delete fSticks[i];
		}
}


void 
GameStick::InitKeyNames()
{
	for(int i = 0; i < 128; i++)
		fJbuffer[i].name = key_name(i);
}


void
GameStick::Poll()
{
	for(int i = 0; i < MAX_JOYSTICKS; i++)
		if(fSticks[i])
			fSticks[i]->Update();
}


GameStickThread::GameStickThread()
	: ATicker(25)
{
}

void
GameStickThread::Tick(const double tickCount)
{
	this->Poll();
}
