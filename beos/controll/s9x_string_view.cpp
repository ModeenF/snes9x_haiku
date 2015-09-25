#include "s9x_string_view.h"

s9x_string_view::s9x_string_view(BRect rect, const char *text)
	: BStringView(rect, "", text, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW)
{
	SetFontSize(10);
	box_rect.Set(1, 1, 6, 6);
	ResizeTo(56, 20);
	bounds = Bounds();
}

void s9x_string_view::MakeFocus(bool flag)
{
	Invalidate();
	BStringView::MakeFocus(flag);
}

void s9x_string_view::AttachedToWindow()
{
	BStringView::AttachedToWindow();
}

void s9x_string_view::Draw(BRect updateRect)
{
	BStringView::Draw(updateRect);
	if(IsFocus()) {
		SetHighColor(255, 0, 0);
		StrokeRect(bounds);
		FillRect(box_rect);
	} else
		StrokeRect(bounds);
}

void s9x_string_view::MouseDown(BPoint where)
{
	MakeFocus();
	BStringView::MouseDown(where);
}