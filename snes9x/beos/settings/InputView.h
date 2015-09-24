#ifndef _INPUTVIEW_H
#define _INPUTVIEW_H

#include <Box.h>
#include <CheckBox.h>
#include <Menu.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <StringView.h>

#include "s9x_includes.h"
//#include "S9xConfigView.h"
#include "input_interface.h"

class InputView : public BView
{
public:
	InputView(BRect rect, const char* name, float tabHeight/*, input_interface *buffer, uint8 &allow*/);
	virtual ~InputView();
/*	enum { SNES9x_UP, SNES9x_DOWN, SNES9x_LEFT, SNES9x_RIGHT, SNES9x_A,
		SNES9x_B, SNES9x_X, SNES9x_Y, SNES9x_L, SNES9x_R, SNES9x_START,
			SNES9x_SELECT, SNES9x_MAX
		};
	virtual void Pulse();
	virtual void Draw(BRect updateRect);
	virtual void AttachedToWindow();
*/	virtual void MessageReceived(BMessage *m);
/*	virtual void SetInput(bool doinput) { fInput = doinput; }
	virtual bool IsInput() const { return fInput; }
	virtual void ChangeLabel(int index, int fKey);
	virtual void SetKeys(uint32 *fWhich);
	virtual void Insert(const BString &pad, int value);
	virtual void ClearControls();
	virtual void CopyControls();
	virtual int  GetLen();*/
//	void SetInput(uint32 *data, input_interface *buffer, uint8 &allow);
	void SetController(BString name, uint32* controll, input_interface* key, uint8* allow);
		
private:
	BBox*			fBoxGeneral;
//	S9xConfigView* 	fS9xConfigView;
	BStringView*	fStrView;
/*	input_interface*	fKey;
	bool 				fInput;
	BBitmap*			fTileBitmap;
	S9xStringView*		fInputSviews[SNES9x_MAX];
	uint32*				fWhich;
	BButton*			fButtonClear;
	BButton*			fButtonOk;
	BButton*			fButtonCancel;
	sem_id 				fSem;
//	uint8&				fIsAllowed;
	uint32 				fControls[SNES9x_MAX];*/
};

#endif //_INPUTVIEW_H
