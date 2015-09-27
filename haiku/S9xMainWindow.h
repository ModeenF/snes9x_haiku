#ifndef __SNES9X_MAIN_WINDOW_H__
#define __SNES9X_MAIN_WINDOW_H__

#include <Application.h>
#include <DirectWindow.h>
#include <Screen.h>
#include <Locker.h>
#include <Bitmap.h>
#include <Path.h>
#include <Entry.h>
#include <FilePanel.h>
#include <FindDirectory.h>

#include "s9x_config_window.h"
#include "input_interface.h"
#include "s9x_settings.h"
#include "S9xRefreshWindow.h"

#include "S9xView.h"
#include "GameStick.h"
#include "SettingsWindow.h"

class S9xConfigWindow;
class GameMenuBar;

int32 mainwindow_thread(void *o);

enum {
	DIRECT, OVERLAY, BITMAP
};

struct direct_info {
	int x;
	int y;
	int w;
	int h;
};

class S9xMainWindow : public BDirectWindow
{
public:
	S9xMainWindow(BRect);
	virtual ~S9xMainWindow();
	virtual bool QuitRequested() {be_app->PostMessage(B_QUIT_REQUESTED); return(true); }
	virtual void MessageReceived(BMessage *m);
	virtual void DirectConnected(direct_buffer_info *info);
	virtual void WindowActivated(bool active);
	virtual void UpdateMouse(uint32 buttons);
	virtual void UpdateMousePosition(int x, int y);
	virtual void S9xMouseOn();
	virtual void ChangeControllers();;
	virtual void ScreenChanged(BRect frame, color_space mode);
	virtual void InitMenuBar();
	virtual void DrawDirect(int Width, int Height);
	virtual void DrawOverlay(int Width, int Height);
	virtual void DrawStandard(int Width, int Height);
	virtual bool TouggleFullScreen();
	void TouggleMenu(bool show);
	virtual void InitS9x();
	virtual bool StartS9x();
	virtual void S9xGUIReset();
	virtual void S9xStates(int index, bool saving = true);
	virtual void SetGraphicsMode(int g_mode, bool clear = true);
	virtual void SetDrawMode(int d_mode, bool runDirectWindow);
	virtual void DirectClear(bool clear);
	virtual void ResizeTo(float width, float height);
	virtual void ClearScreen();
	virtual BBitmap *Screen() { return &off_screen; }
	virtual BBitmap *SubScreen() { return &sub_screen; }
	int32 OThread();
	virtual void DePause();
	const entry_ref *Ref() { return &mRef; }
	virtual void ProcessEvents();
	virtual int CheckKey(int code);	
	virtual void MenuActivated(bool active);
	static int32 KeyThread(void *o);
	virtual void ChangeItemBool(BMessage *m, uint8 &ch);
	void RenderNormal();
	void Render2xsai();
	void RenderS2xsai();
	void RenderSeagle();
	void RenderMulti(int w);
	void RenderScanOrDouble(const int type, bool scanlines);
	void RenderLcd(const int type);
	void SetDirectInfo();
private:
	int 				_GetHeight();
	BPath				_GetRomDirectory(directory_which which, const char* spath);	
	void				_BuildSettingsWindow(int32 tabnr);
	void				_SetRecentRomFile(const  char* name);
	
	S9xView*			fS9xView;
	s9x_config_window*	fConfigWindow;
	S9xRefreshWindow*	fRefreshWindow;
	GameStick*			fGameStick;
	bool				fShowMenu;
	bool				fFullScreen;
	GameMenuBar*		fGameMenuBar;
	BFilePanel*			fFilePanel;
	SettingsWindow*		fSettingsWindow;
		
	int32 				menubar_height;	
	input_interface 	key[512];
	display_mode 		prev_mode, new_mode, cur_mode;
	bool 				fModeIsValid, fRunning;
	
	// DirectWindow variables
	BLocker *m_locker;
	int32 m_bitsperpixel;
	int32 m_rowbytes;
	color_space m_format;
	uint32 m_numcliprects;
	clipping_rect *m_cliplist;
	clipping_rect m_bounds;
	uint8 *m_bits;
	bool m_connected;
	
	thread_id thread;
	bool ispaused;
	volatile bool exiting;
	volatile bool needs_clear;
	entry_ref mRef;
	BBitmap off_screen;
	BBitmap sub_screen;
	BBitmap *ov_bitmap;
	thread_id key_id;
	direct_info *direct;
};

#endif
