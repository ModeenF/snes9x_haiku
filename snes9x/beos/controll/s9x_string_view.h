#ifndef __S9X_STRING_VIEW_H__
#define __S9X_STRING_VIEW_H__

#include <StringView.h>

class s9x_string_view : public BStringView
{
public:
	s9x_string_view(BRect r, const char *text);
	virtual void Draw(BRect updateRect);
	virtual void MouseDown(BPoint where);
	virtual void AttachedToWindow();
	virtual void MakeFocus(bool flag = true);
private:
	BRect bounds;
	BRect box_rect;
};

#endif
