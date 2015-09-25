/******************************************************************************
 * $Id: DisplayModeField.cpp
 *
 * Authors:
 *		Fredrik Modéen <fredrik@modeen.se>
 *
 * Copyright (c) 2008
 *****************************************************************************/
#include "DisplayModeField.h"

#include <stdio.h>

#define kTEXT_MARGIN	8

DisplayModeField::DisplayModeField(display_mode dm)
	: fDisplayMode(dm), fWidth(0)
{
}


void
DisplayModeField::SetDisplayMode(display_mode dm)
{
	fDisplayMode = dm;
	fWidth = 0;
}


display_mode
DisplayModeField::GetDisplayMode()
{
	return fDisplayMode;
}


const char*
DisplayModeField::String() const
{
	BString item_name;
	item_name << fDisplayMode.virtual_width << " * " << fDisplayMode.virtual_height << " - " << 
	(fDisplayMode.timing.pixel_clock * 1000) / (fDisplayMode.timing.h_total * fDisplayMode.timing.v_total) << " Hz";
	return item_name.String();
}


void
DisplayModeField::SetWidth(float width)
{
	fWidth = width;
}


float
DisplayModeField::Width()
{
	return fWidth;
}


void
DisplayModeField::SetClippedString(const char* val)
{
	fClippedString = val;
} 


const char*
DisplayModeField::ClippedString()
{
	return fClippedString.String();
}


// #pragma mark -


DisplayModeColumn::DisplayModeColumn(const char* title, float width, float minWidth,
		float maxWidth, uint32 truncate, alignment align)
	: BTitledColumn(title, width, minWidth, maxWidth, align),
	fTruncate(truncate)
{
}


void
DisplayModeColumn::DrawField(BField* _field, BRect rect, BView* parent)
{
	float width = rect.Width() - (2 * kTEXT_MARGIN);
	DisplayModeField* field = static_cast<DisplayModeField*>(_field);

	if (width != field->Width()) {
		BString out_string(field->String());

		parent->TruncateString(&out_string, fTruncate, width + 2);
		field->SetClippedString(out_string.String());
		field->SetWidth(width);
	}
	DrawString(field->ClippedString(), parent, rect);
}


int
DisplayModeColumn::CompareFields(BField* field1, BField* field2)
{
	return ICompare(((BStringField*)field1)->String(),
		(((DisplayModeField*)field2)->String()));
}


bool
DisplayModeColumn::AcceptsField(const BField *field) const
{
	return static_cast<bool>(dynamic_cast<const DisplayModeField*>(field));
}


// #pragma mark -
