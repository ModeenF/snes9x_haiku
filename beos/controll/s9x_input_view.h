#ifndef __SNES9X_INPUT_VIEW_H__
#define __SNES9X_INPUT_VIEW_H__

class s9x_string_view;

class s9x_input_view : public BView
{
public:
	s9x_input_view(BRect rect, input_interface *buffer, uint8 &allow);
	virtual ~s9x_input_view();
	enum { SNES9x_UP, SNES9x_DOWN, SNES9x_LEFT, SNES9x_RIGHT, SNES9x_A,
		SNES9x_B, SNES9x_X, SNES9x_Y, SNES9x_L, SNES9x_R, SNES9x_START,
			SNES9x_SELECT, SNES9x_MAX
		};
	virtual void Pulse();
	virtual void Draw(BRect updateRect);
	virtual void AttachedToWindow();
	virtual void MessageReceived(BMessage *m);
	virtual void SetInput(bool doinput) { input = doinput; }
	virtual bool IsInput() const { return input; }
	virtual void ChangeLabel(int index, int key);
	virtual void SetKeys(uint32 *which);
	virtual void Insert(const BString &pad, int value);
	virtual void ClearControls();
	virtual void CopyControls();
	virtual int GetLen();
private:
	input_interface *key;
	bool input;
	BBitmap *tile_bitmap;
	s9x_string_view *input_sviews[SNES9x_MAX];
	uint32 *which;
	BButton *clear, *ok, *cancel;
	sem_id sem;
	uint8 &isAllowed;
	uint32 controls[SNES9x_MAX];
};

#endif