#ifndef _EMULATORVIEW_H
#define _EMULATORVIEW_H

#include <View.h>

class BBox;
class BCheckBox;

class EmulatorView : public BView
{
public:
	// publics functions
	EmulatorView(BRect frame, const char* name, float tabHeight);
	~EmulatorView();

protected:
	// protected members
};

#endif //_EMULATORVIEW_H
