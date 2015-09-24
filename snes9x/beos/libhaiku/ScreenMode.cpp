/*
 * Copyright 2005, Haiku.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Axel Dörfler, axeld@pinc-software.de
 */


#include "ScreenMode.h"

#include <InterfaceDefs.h>

#include <stdlib.h>
#include <stdio.h>


/* Note, this headers defines a *private* interface to the Radeon accelerant.
 * It's a solution that works with the current BeOS interface that Haiku
 * adopted.
 * However, it's not a nice and clean solution. Don't use this header in any
 * application if you can avoid it. No other driver is using this, or should
 * be using this.
 * It will be replaced as soon as we introduce an updated accelerant interface
 * which may even happen before R1 hits the streets.
 */

//#include "multimon.h"	// the usual: DANGER WILL, ROBINSON!


static combine_mode
get_combine_mode(display_mode& mode)
{
	if (mode.flags & B_SCROLL == 0)
		return kCombineDisable;

	if (mode.virtual_width == mode.timing.h_display * 2)
		return kCombineHorizontally;

	if (mode.virtual_height == mode.timing.v_display * 2)
		return kCombineVertically;

	return kCombineDisable;
}


static float
get_refresh_rate(display_mode& mode)
{
	// we have to be catious as refresh rate cannot be controlled directly,
	// so it suffers under rounding errors and hardware restrictions
	return rint(10 * float(mode.timing.pixel_clock * 1000) / 
		float(mode.timing.h_total * mode.timing.v_total)) / 10.0;
}



//	#pragma mark -


int32
screen_mode::BitsPerPixel() const
{
	switch (space) {
		case B_RGB32:	return 32;
		case B_RGB24:	return 24;
		case B_RGB16:	return 16;
		case B_RGB15:	return 15;
		case B_CMAP8:	return 8;
		default:		return 0;
	}
}


bool
screen_mode::operator==(const screen_mode &other) const
{
	return !(*this != other);
}


bool
screen_mode::operator!=(const screen_mode &other) const
{
	return width != other.width || height != other.height
		|| space != other.space || refresh != other.refresh
		|| combine != other.combine
		|| swap_displays != other.swap_displays
		|| use_laptop_panel != other.use_laptop_panel
		|| tv_standard != other.tv_standard;
}


void
screen_mode::SetTo(display_mode& mode)
{
	width = mode.virtual_width;
	height = mode.virtual_height;
	space = (color_space)mode.space;
	combine = get_combine_mode(mode);
	refresh = get_refresh_rate(mode);

	if (combine == kCombineHorizontally)
		width /= 2;
	else if (combine == kCombineVertically)
		height /= 2;

	swap_displays = false;
	use_laptop_panel = false;
	tv_standard = 0;
}


//	#pragma mark -


ScreenMode::ScreenMode(BWindow* window)
	:
	fWindow(window),
	fUpdatedModes(false)
{
	BScreen screen(window);
	if (screen.GetModeList(&fModeList, &fModeCount) == B_OK) {
		// sort modes by resolution and refresh to make
		// the resolution and refresh menu look nicer
		qsort(fModeList, fModeCount, sizeof(display_mode), compare_mode);
	} else {
		fModeList = NULL;
		fModeCount = 0;
	}
}


ScreenMode::~ScreenMode()
{
	free(fModeList);
}

screen_mode
ScreenMode::ModeAt(int32 index)
{
	if (index < 0)
		index = 0;
	else if (index >= (int32)fModeCount)
		index = fModeCount - 1;

	screen_mode mode;
	mode.SetTo(fModeList[index]);

	return mode;
}


int32
ScreenMode::CountModes()
{
	return fModeCount;
}

