
/******************************************************************************
 * $Id: DisplayModeField.h
 *
 * Authors:
 *		Fredrik Modéen <fredrik@modeen.se>
 *
 * Copyright (c) 2008
 *
 *****************************************************************************/

#ifndef DISPLAYMODEFIELD
#define DISPLAYMODEFIELD

#include <Screen.h>

#include <ColumnListView.h>
#include <ColumnTypes.h>

class DisplayModeField : public BField
{
	public:
		DisplayModeField(display_mode dm);
		void 			SetDisplayMode(display_mode dm);
		display_mode	GetDisplayMode();
		void 			SetWidth (float);
		float 			Width();
		const char*		String() const;
		void			SetClippedString(const char* string);
		const char*		ClippedString();
	private:
		display_mode 	fDisplayMode;
		float			fWidth;
		BString			fClippedString;
};


//--------------------------------------------------------------------

class DisplayModeColumn : public BTitledColumn
{
	public:
		DisplayModeColumn(const char *title, float width, float minWidth,
							float maxWidth, uint32 truncate, alignment align = B_ALIGN_LEFT);
		virtual void	DrawField(BField* field, BRect rect, BView* parent);
		virtual int		CompareFields(BField* field1, BField* field2);
		virtual	bool	AcceptsField (const BField* field) const;
	private:
		uint32				fTruncate;
};

#endif /*DISPLAYMODEFIELD*/
