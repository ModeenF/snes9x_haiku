#ifndef __GAMESTICK_H__
#define __GAMESTICK_H__

#include "atime.h"
#include "Joystick.h"

#define MAX_JOYSTICKS 5

class GameStickThread : public ATicker
{
public:
	GameStickThread();			
	virtual	void Tick(const double tickCount);
	virtual void Poll() = 0;
};

class GameStick : public GameStickThread
{
public:
	GameStick(input_interface *buffer);
	virtual ~GameStick();
	virtual void Poll();
	virtual void InitKeyNames();
private:
	Joystick*			fSticks[MAX_JOYSTICKS];
	input_interface*	fJbuffer;
};

#endif
