#include "s9x_includes.h"
#include "S9xMainWindow.h"

#include <Path.h>
#include <Alert.h>

volatile uint32 FrameTimer = 0;
uint32 snes9x_is_32 = 0; // used inside 2xsai asm
int xx = 0, yy = 0;
uint32 button_d = 0;
static BSoundPlayer *s9x_player = 0;
static uint8 *Delta = 0;
static thread_id TimerID = -1;
static int32 s9x_timer(void *o);
s9x_settings settings;
static BLocker sound_locker;
static BLocker main_locker;

void OutOfMemory();
void InitTimer();
void S9xInitDisplay();

//Linking references
uint8 snes9x_clear_change_log = 0; // used inside 2xsai asm

S9xMainWindow::S9xMainWindow(BRect rect)
	: BDirectWindow(rect, S9x_WINDOWNAME, B_TITLED_WINDOW, B_NOT_ZOOMABLE | B_NOT_RESIZABLE),
	fConfigWindow(0),
	fRunning(false),
	fRefreshWindow(0),
	fFullScreen(false),
	fShowMenu(false),
	fGameStick(0),
	fFilePanel(0),
	fSettingsWindow(NULL),
	menubar_height(0),
	fModeIsValid(false),
	m_locker(new BLocker()),
	m_bitsperpixel(0),
	m_rowbytes(0),
	m_format(B_NO_COLOR_SPACE),
	m_numcliprects(0),
	m_cliplist(0),
	m_bits(0),
	m_connected(false),
	ispaused(false),
	exiting(true),
	needs_clear(false),
	off_screen(BRect(0, 0, 511, 477), B_RGB16, false, true),
	sub_screen(BRect(0, 0, 511, 477), B_RGB16, false, true),
	ov_bitmap(0),
	direct(0)
{
	GetSettings(settings);
	this->fGameStick = new GameStick(this->key);
	
	rect.OffsetTo(B_ORIGIN);
	InitMenuBar();
	
	rect.top = (float)menubar_height + 1;
	fS9xView = new S9xView(rect, this);
	fS9xView->SetViewColor(0, 0, 0);
	AddChild(fS9xView);
	
	AddShortcut('F', B_COMMAND_KEY, new BMessage(S9x_FULLSCREEN));
	AddShortcut('1', B_COMMAND_KEY, new BMessage(S9x_FRAME_RATE));
	AddShortcut('7', B_COMMAND_KEY, new BMessage(S9x_CHANGE_CONTROLLER));
	
	SetWindowAlignment(B_BYTE_ALIGNMENT, 8);
	
	display_mode *m_mode;
	uint32 m_nummodes;
	
	if(BScreen().GetModeList(&m_mode, &m_nummodes) == B_NO_ERROR) {
		for (unsigned int i = 0; i < m_nummodes; i++) {
			if((m_mode[i].virtual_width == 640) && (m_mode[i].space == B_RGB16)) {
				fModeIsValid = true;
				new_mode = m_mode[i];
				break;
			}
		}
	}
	BScreen().GetMode(&prev_mode);
	
	TimerID = spawn_thread(s9x_timer, "FrameTimer", B_NORMAL_PRIORITY, this);
	resume_thread(TimerID);
	
	key_id = spawn_thread(KeyThread, "let's get keys", B_NORMAL_PRIORITY, this);
	resume_thread(key_id);

	InitS9x();
	init_palette();
	ov_bitmap = new BBitmap(BRect(0, 0, 511, 223), B_BITMAP_WILL_OVERLAY, B_YCbCr422);
	bool overlay = false;
	bool direct = false;
	bool bitmap = true;
	if(ov_bitmap && ov_bitmap->InitCheck() == B_OK) {
		if(settings.ui.s9x_drawmode == OVERLAY)
			SetDrawMode(settings.ui.s9x_drawmode, true);
		overlay_restrictions r;
		ov_bitmap->GetOverlayRestrictions(&r);
		SetSizeLimits(r.source.max_width - 1, r.source.max_width * r.max_width_scale, 
			(r.source.max_height + menubar_height), r.source.max_height * r.max_height_scale);
		overlay = true;
		direct = true;
	} else {
		
		if(settings.ui.s9x_drawmode == OVERLAY){
			settings.ui.s9x_drawmode = DIRECT;
		}
		
		if(BDirectWindow::SupportsWindowMode()){
			settings.ui.s9x_drawmode = DIRECT;
		}else{
			settings.ui.s9x_drawmode = BITMAP;			
		}
	}
		
	float minw, maxw, minh, maxh;
	GetSizeLimits(&minw, &maxw, &minh, &maxh);
	
	SetSizeLimits(511, maxw, (447 + menubar_height), maxh);
	ResizeTo((SNES_WIDTH * 2) - 1, (_GetHeight() * 2) - 1);
	//printf("bitmap = %d overlay = %d direct = %d\n", bitmap, overlay, direct);
	fGameMenuBar->SetMode(bitmap, overlay, direct, settings.ui.s9x_drawmode);
	fGameMenuBar->ToggleFullScreenMenu(fModeIsValid);
}


bool
S9xMainWindow::StartS9x()
{
	int32 result;
	exiting = true;
	wait_for_thread(thread, &result);
	if(s9x_player) {
		s9x_player->Stop();
		delete s9x_player;
		s9x_player = 0;
	}
	exiting = false;
	DePause();
	thread = spawn_thread(fThread, S9x_WINDOWNAME, B_NORMAL_PRIORITY, (void *)this);
	resume_thread(thread);
	fGameMenuBar->ToggleRomInfoMenu(true);
	return true;
}


void 
S9xMainWindow::UpdateMouse(uint32 buttons)
{
	button_d = buttons;
}


void 
S9xMainWindow::UpdateMousePosition(int x, int y)
{
	static bool ignore_next = false;
	
	if(!ignore_next) {
		BPoint p(x, y);
		fS9xView->ConvertToScreen(&p);
		if(IPPU.Controller == SNES_MOUSE || IPPU.Controller == SNES_MOUSE_SWAPPED) {
			BPoint middle;
			BRect size = Bounds();
			middle.x = (size.right - size.left) / 2;
			middle.y = (size.bottom - size.top) / 2;
			fS9xView->ConvertToScreen(&middle);
		
			xx += (int)(p.x - middle.x);
			yy += (int)(p.y - middle.y);
		
			set_mouse_position((int)middle.x, (int)middle.y);
			ignore_next = true;
		} else {
			xx = (int)x / 2;
			yy = (int)y / 2;
		}
	} else
		ignore_next = false;
		
	S9xMouseOn();
}


void
S9xMainWindow::InitS9x()
{
	memset(&Settings, 0, sizeof(Settings));  
	Settings.SoundPlaybackRate = 7;
    Settings.APUEnabled = true;
    Settings.SoundBufferSize = 2048;
    Settings.CyclesPercentage = 100;
    Settings.H_Max = SNES_CYCLES_PER_SCANLINE;
    Settings.SkipFrames = AUTO_FRAMERATE;
    Settings.FrameTimePAL = 20000;
    Settings.FrameTimeNTSC = 16667;
    Settings.FrameTime = Settings.FrameTimeNTSC;
    Settings.ServerName [0] = 0;
    Settings.HBlankStart = (256 * Settings.H_Max) / SNES_HCOUNTER_MAX;        
    
	//Sound
    Settings.Stereo = settings.sound.s9x_stereo;
	Settings.SoundEnvelopeHeightReading = settings.sound.s9x_SoundEnvelopeHeightReading;
	Settings.InterpolatedSound = settings.sound.s9x_InterpolatedSound;
    Settings.DisableSoundEcho = settings.sound.s9x_DisableSoundEcho;
    Settings.NextAPUEnabled = settings.sound.s9x_NextAPUEnabled;
    Settings.DisableSampleCaching = settings.sound.s9x_DisableSampleCaching;
    Settings.DisableMasterVolume = settings.sound.s9x_DisableMasterVolume;
    Settings.ThreadSound = settings.sound.s9x_ThreadSound;
    
    //peripherial
    Settings.ShutdownMaster = settings.peripherial.s9x_ShutdownMaster;
    Settings.Mouse = settings.peripherial.s9x_Mouse;    
    Settings.SuperScope = settings.peripherial.s9x_SuperScope;   
    Settings.MultiPlayer5 = settings.peripherial.s9x_MultiPlayer5;
    Settings.ControllerOption = settings.peripherial.s9x_ControllerOption;
    
    //graphics
    Settings.DisplayFrameRate = settings.graphics.s9x_DisplayFrameRate;
    Settings.Transparency = settings.graphics.s9x_Transparency;
    Settings.ForceTransparency = settings.graphics.s9x_ForceTransparency;        
    Settings.SixteenBit = settings.graphics.s9x_SixteenBit;
    Settings.SupportHiRes = settings.graphics.s9x_SupportHiRes;

    //network
    Settings.NetPlay = settings.network.s9x_NetPlay;
#ifdef _NETPLAY_SUPPORT
    Settings.Port = settings.network.s9x_Port;
#endif

	//misc
    Settings.AutoSaveDelay = settings.misc.s9x_AutoSaveDelay;
    Settings.ApplyCheats = settings.misc.s9x_ApplyCheats;
    Settings.TurboMode = settings.misc.s9x_TurboMode;
    Settings.TurboSkipFrames = settings.misc.s9x_TurboSkipFrames;
    
    //controll
    Settings.JoystickEnabled = settings.controll.s9x_JoystickEnabled;
    
}


void
S9xMainWindow::S9xMouseOn()
{
	if (IPPU.Controller == SNES_MOUSE || IPPU.Controller == SNES_MOUSE_SWAPPED) {
    	if(!be_app->IsCursorHidden())
    		be_app->HideCursor();
    	fS9xView->ChangeCursor(0);
    } else if (	IPPU.Controller != SNES_SUPERSCOPE&&
    			IPPU.Controller != SNES_JUSTIFIER&&
    			IPPU.Controller != SNES_JUSTIFIER_2) {
    	if(be_app->IsCursorHidden())
    		be_app->ShowCursor();
    	fS9xView->ChangeCursor(0);
    } else {
    	if(be_app->IsCursorHidden())
    		be_app->ShowCursor();
    	fS9xView->ChangeCursor(1);
    }
}



int32
S9xMainWindow::OThread()
{
	if(GFX.ZBuffer)
		S9xExit();
		
	BEntry entry(Ref(), true);
	BPath path;
	entry.GetPath(&path);
	char *rom_filename = 0;
	
    rom_filename = strdup(path.Path());

    if (!Memory.Init () || !S9xInitAPU()) {
		OutOfMemory ();
		exiting = true;
		return 0;
	}
    uint32 saved_flags = CPU.Flags;

#ifdef GFX_MULTI_FORMAT
    S9xSetRenderPixelFormat (RGB565);
#endif

    if (rom_filename) {
		if (!Memory.LoadROM (rom_filename)) {
			exiting = true;
			return(0);
		}
		Memory.LoadSRAM (S9xGetFilename (S9x_FILESRM));
    } else {
		S9xReset();
		Settings.Paused |= 2;
    }
    CPU.Flags = saved_flags;

    S9xInitDisplay ();
    if (!S9xGraphicsInit ()) {
    	exiting = true;
		OutOfMemory ();
		return 0;
	}

    (void) S9xInitSound (Settings.SoundPlaybackRate, Settings.Stereo,
		      Settings.SoundBufferSize);
		      
    if (!Settings.APUEnabled)
    	S9xSetSoundMute (TRUE);

	char buf[513];
    sprintf (buf, "\"%s\" -  %s: %s", Memory.ROMName, TITLE, VERSION);
    S9xSetTitle (buf);
    
    InitTimer ();
    if (!Settings.APUEnabled)
    	S9xSetSoundMute (FALSE);

    while (!exiting) {
    if(exiting) 
    	break;
    main_locker.Lock();
    
	if (!Settings.Paused
#ifdef DEBUGGER
	    || (CPU.Flags & (DEBUG_MODE_FLAG | SINGLE_STEP_FLAG))
#endif
		)
	    S9xMainLoop ();

	if (Settings.Paused
#ifdef DEBUGGER
	    || (CPU.Flags & DEBUG_MODE_FLAG)
#endif
        ) {
	    S9xSetSoundMute (TRUE);
	}

#ifdef DEBUGGER
	if (CPU.Flags & DEBUG_MODE_FLAG) {
	    S9xDoDebug ();
	} else
#endif
	if (Settings.Paused)
	    S9xProcessEvents (TRUE);

	S9xProcessEvents (FALSE);
	
	if (!Settings.Paused
#ifdef DEBUGGER
	    && !(CPU.Flags & DEBUG_MODE_FLAG)
#endif	    
       ) {
	    S9xSetSoundMute (FALSE);
	}
	main_locker.Unlock();
	if(Settings.Paused)
		snooze(16000);
    }
    return (0);
}


void
S9xMainWindow::InitMenuBar()
{
	fGameMenuBar = new GameMenuBar(BRect(0, 0, 0, 0), "menu_bar", settings);
	AddChild(fGameMenuBar);
	menubar_height = (int32)fGameMenuBar->Bounds().Height();
	fGameMenuBar->SetItems(settings.ui.s9x_drawmode == DIRECT);
}


void
S9xMainWindow::S9xGUIReset()
{
	main_locker.Lock();
	sound_locker.Lock();
	DePause();
	S9xReset();
	main_locker.Unlock();
    sound_locker.Unlock();
}


void
S9xMainWindow::S9xStates(int index, bool saving)
{
	char def [PATH_MAX + 1];
    char drive [_MAX_DRIVE + 1];
    char dir [_MAX_DIR + 1];
    char ext [_MAX_EXT + 1];
	DePause();
    _splitpath (Memory.ROMFilename, drive, dir, def, ext);
    sprintf(ext, ".00%d", index);
    strcat (def, ext);
  
	main_locker.Lock();
	sound_locker.Lock();
	S9xSetSoundMute (TRUE);
	
	if(saving ? Snapshot(def) : S9xLoadSnapshot(def)) {
		saving ? sprintf (String, "Snapshot saved as: %.255s", def)
			: sprintf (String, "Snapshot Loaded: %.255s", def);
		S9xMessage(0, 0, String);
	} else {
		saving ? sprintf (String, "Error, unable to save: %.255s", def)
			: sprintf (String, "Error, unable to load: %.255s", def);
		S9xMessage(0, 0, String);
	}
    
    S9xSetSoundMute (FALSE);
    main_locker.Unlock();
    sound_locker.Unlock();
}


void
S9xMainWindow::ChangeItemBool(BMessage *m, uint8 &value)
{
	value = !value;
	main_locker.Lock();
	BMenuItem *item;
	m->FindPointer("source", (void **)&item);	
	item->SetMarked(value);
	main_locker.Unlock();
}


void
S9xMainWindow::MessageReceived(BMessage *m)
{
//	m->PrintToStream();
	int32 index, slot;
	bool runDirect = false;
	switch(m->what) {
		case B_KEY_DOWN:
			if(m->FindInt32("key", &index) == B_NO_ERROR) {
				if(index == S9X_ESCAPE)
					PostMessage(B_QUIT_REQUESTED);
			}
			break;
		case B_SIMPLE_DATA:
		case B_REFS_RECEIVED:
			if(m->FindRef("refs", &mRef) == B_NO_ERROR){
				BPath path(&mRef);
				_SetRecentRomFile(path.Path());
				fRunning = StartS9x();
			}break;
		case S9x_RECENT_ROM:{
			BString str;
			if(m->FindString("recentRom", &str) == B_NO_ERROR){
				BEntry entry(str.String());
				entry.GetRef(&mRef);
				_SetRecentRomFile(str.String());
				fRunning = StartS9x();
			}
		}break;
		case S9x_BILINER7:
			settings.graphics.s9x_Mode7Interpolate = ! settings.graphics.s9x_Mode7Interpolate;
			break;
		case S9x_SHOWFRAMERATE:
			settings.graphics.s9x_DisplayFrameRate = ! settings.graphics.s9x_DisplayFrameRate;
			break;
//		case S9x_STRETSHIMAGE:
//			settings.graphics.s9x_Mode7Interpolate = ! settings.graphics.s9x_Mode7Interpolate;
//			break;
		case S9x_RENDER16BIT:
			settings.graphics.s9x_SixteenBit = ! settings.graphics.s9x_SixteenBit;
			break;
		case S9x_TRANSPARENCY:
			settings.graphics.s9x_Transparency = ! settings.graphics.s9x_Transparency;
			break;
		case S9x_HIRES:
			settings.graphics.s9x_SupportHiRes = ! settings.graphics.s9x_SupportHiRes;
			break;		
		case S9x_FULLSCREEN:
			if(fModeIsValid)
				TouggleFullScreen();
			break;
		case S9x_SAVE_SPC:
			if(!exiting)
				spc_is_dumping = 1;
			break;
		case S9x_SNOOZE:
			ChangeItemBool(m, settings.ui.s9x_snooze);
			break;
		case S9x_INACTIVE:
			ChangeItemBool(m, settings.ui.s9x_inactive);
			break;
		case S9x_MENUACTIVE:
			ChangeItemBool(m, settings.ui.s9x_menuactive);
			break;
		case S9x_ALLOW:
			ChangeItemBool(m, settings.ui.s9x_allow);
			break;
		case S9x_VSYNC:
			ChangeItemBool(m, settings.ui.s9x_vsync);
			break;
		case S9x_STEREO:
			settings.sound.s9x_stereo = ! settings.sound.s9x_stereo;
			//ChangeItemBool(m, settings.sound.s9x_stereo);
			break;
		case S9x_MUTE:
			//ChangeItemBool(m, settings.sound.s9x_mute);
			settings.sound.s9x_mute = ! settings.sound.s9x_mute;
			break;
		case S9x_REVERTSTEREO:
			settings.sound.s9x_ReverseStereo = ! settings.sound.s9x_ReverseStereo;
			break;
		case S9x_GAUSSIAN:
			settings.sound.s9x_InterpolatedSound = ! settings.sound.s9x_InterpolatedSound;
			break;
		case S9x_GENERATESAMPEL:
			settings.sound.s9x_SoundSync = ! settings.sound.s9x_SoundSync;
			break;
		case S9x_VOLUMEHEIGHT:
			settings.sound.s9x_SoundEnvelopeHeightReading = ! settings.sound.s9x_SoundEnvelopeHeightReading;
			break;
		case S9x_ECHO:
			settings.sound.s9x_DisableSoundEcho = ! settings.sound.s9x_DisableSoundEcho;
			break;
		case S9x_CACHEDECODER:
			settings.sound.s9x_DisableSampleCaching = ! settings.sound.s9x_DisableSampleCaching;
			break;
		case S9x_MASTERVOLUME:
			settings.sound.s9x_DisableMasterVolume = ! settings.sound.s9x_DisableMasterVolume;
			break;
		case S9x_ANTIRESON:
			settings.sound.s9x_AltSampleDecode = ! settings.sound.s9x_AltSampleDecode;
			break;
//		case S9x_FAKEMUTE:
//			settings.sound.s9x_ReverseStereo = ! settings.sound.s9x_ReverseStereo;
//			break;
		case S9x_ENABLE:
//			settings.sound.s9x_ReverseStereo = ! settings.sound.s9x_ReverseStereo;
			break;
		case S9x_16BITPLAYBACK:
			settings.sound.s9x_SixteenBitSound = ! settings.sound.s9x_SixteenBitSound;
			break;
//		case S9x_FRAMEMUTE:
//			settings.sound.s9x_ReverseStereo = ! settings.sound.s9x_ReverseStereo;
//			break;
		case S9x_EXTENDED:
			settings.ui.s9x_extended = !settings.ui.s9x_extended;
			SetGraphicsMode(settings.ui.s9x_mode);
			break;
		case S9x_ROMINFO:
			if(exiting)
				break;
			new S9xRomWindow(BRect(200, 200, 500, 550));
			break;
		case S9x_SCREENSHOT:
			save_screenshot(off_screen, SNES_WIDTH, SNES_HEIGHT);
			break;
		case S9x_LAUNCH_REFRESH:
			//if(fRefreshWindow != 0)
			//	break;
			fRefreshWindow = new S9xRefreshWindow(BRect(200, 200, 400, 300), this, 'redl');
			break;
		case 'redl':
			fRefreshWindow = 0;
			break;
		case S9x_DISPLAYMODE: {
			display_mode *mode;
			if(m->FindPointer("DirectMode", (void **)&mode) == B_NO_ERROR)
				new_mode = *mode;
		} break;
		case S9x_DIRECTWINDOW:{
			if(fRunning && !fFullScreen){
				BAlert* myAlert = new BAlert("title", "DirectWindow must be run in Fullscreen do you want to run in Fullscreen?", "Yes", "No", NULL, B_WIDTH_AS_USUAL, B_EVEN_SPACING, B_WARNING_ALERT);
				myAlert->SetShortcut(0, B_ESCAPE);
				int32 button_index = myAlert->Go();
				//printf("button_index = %d\n", button_index);
				//Question: this must be run in full window do you want to do that?
				if(button_index == 0){				
					m->AddBool("runDirectWindow", true);
				}else
					break;
			}
		}
		case S9x_OVERLAY:
		case S9x_BITMAP:
			m->FindInt32("index", &index);
			m->FindBool("runDirectWindow", &runDirect);
			SetDrawMode(index, runDirect);
			break;
		case S9x_CHANGE_CONTROLLER:
			if(exiting) 
				break;
			ChangeControllers();
			break;
		case S9x_FRAME_RATE:
			Settings.DisplayFrameRate = !Settings.DisplayFrameRate;
			break;
		case S9x_PAUSE:
			ChangeItemBool(m, Settings.Paused);
			ispaused = Settings.Paused;
			break;
		case S9x_LOAD:
			if(!fFilePanel)
				fFilePanel = new BFilePanel(B_OPEN_PANEL, new BMessenger(this), NULL, B_FILE_NODE, false);
			fFilePanel->SetPanelDirectory(_GetRomDirectory(B_USER_DIRECTORY, "snesrom").Path());
			fFilePanel->Show();
			break;
		case S9x_CONTROLLER1:
			if(fConfigWindow != 0) break;
			fConfigWindow = new s9x_config_window("S9x - Controller 1", (uint32 *)&settings.controll.s9x_controller1, this,
				S9x_KEY_QUIT, key, settings.ui.s9x_allow);
			break;
		case S9x_CONTROLLER2:
			if(fConfigWindow != 0) break;
			fConfigWindow = new s9x_config_window("S9x - Controller 2",(uint32 *)&settings.controll.s9x_controller2, this,
				S9x_KEY_QUIT, key, settings.ui.s9x_allow);
			break;
		case S9x_CONTROLLER3:
			if(fConfigWindow != 0) break;
			fConfigWindow = new s9x_config_window("S9x - Controller 3",(uint32 *)&settings.controll.s9x_controller3, this,
				S9x_KEY_QUIT, key, settings.ui.s9x_allow);
			break;
		case S9x_CONTROLLER4:
			if(fConfigWindow != 0) break;
			fConfigWindow = new s9x_config_window("S9x - Controller 4", (uint32 *)&settings.controll.s9x_controller4, this,
				S9x_KEY_QUIT, key, settings.ui.s9x_allow);
			break;
		case S9x_CONTROLLER5:
			if(fConfigWindow != 0) break;
			fConfigWindow = new s9x_config_window("S9x - Controller 5", (uint32 *)&settings.controll.s9x_controller5, this,
				S9x_KEY_QUIT, key, settings.ui.s9x_allow);
			break;
		case S9x_KEY_QUIT:
			fConfigWindow = 0;
			break;
		case S9x_RESET:
			if(!exiting)
				S9xGUIReset();
			break;
		case S9x_SAVE_STATE:
			if(exiting)
				break;
			m->FindInt32("save_slot", &slot);
			S9xStates(slot);
			break;
		case S9x_LOAD_STATE:
			if(exiting)
				break;
			m->FindInt32("load_slot", &slot);
			S9xStates(slot, false);
			break;
		case S9x_MODE0:
		case S9x_MODE1:
		case S9x_MODE2:
		case S9x_MODE3:
		case S9x_MODE4:
		case S9x_MODE5:
		case S9x_MODE6:
		case S9x_MODE7:
			m->FindInt32("index", &index);
			SetGraphicsMode(index);
			break;
		case B_ABOUT_REQUESTED:
			//Shuld work without this.. but I could not make it work..//Fredrik
			be_app->PostMessage(B_ABOUT_REQUESTED);
			break;
		case S9x_HIDEMENU:
			if(!fFullScreen){
				TouggleMenu(fShowMenu);
				fShowMenu = !fShowMenu;
			}
			break;
		case S9x_EMULATORSETTINGS:
			_BuildSettingsWindow(1);
			break;
		case S9x_INPUTSETTINGS:
			_BuildSettingsWindow(2);
			//CONTROLLER1
			/*m->AddString("name", CONTROLLER1);
			m->AddPointer("key", key);
			m->AddPointer("s9x_controller", &settings.s9x_controller1);
			m->AddData("s9x_allow", B_UINT8_TYPE, &settings.ui.s9x_allow, sizeof(uint8));
			//m->AddBool("s9x_allow", settings.ui.s9x_allow);
			fSettingsWindow->PostMessage(m);*/
			//fSettingsWindow->SetInput( (uint32 *)&settings.s9x_controller1, key, settings.ui.s9x_allow);
			break;
		case S9x_SOUNDSETTINGS:
			_BuildSettingsWindow(1);
			break;
		case S9x_DISPLAYSETTINGS:
			_BuildSettingsWindow(0);
			break;
		default:
			BDirectWindow::MessageReceived(m);
			break;
	}
}


void
S9xMainWindow::SetDrawMode(int d_mode, bool runDirectWindow)
{
	main_locker.Lock();
	sound_locker.Lock();
	settings.ui.s9x_drawmode = d_mode;
	
	fS9xView->ClearViewOverlay();
	if(settings.ui.s9x_drawmode == OVERLAY) {
		rgb_color col;
		fS9xView->SetViewOverlay(ov_bitmap, ov_bitmap->Bounds(), fS9xView->Bounds(), &col,
				B_FOLLOW_ALL, B_OVERLAY_FILTER_HORIZONTAL | B_OVERLAY_FILTER_VERTICAL);
		fS9xView->SetViewColor(col);
		clear_ycbr((uint8 *)ov_bitmap->Bits(), ov_bitmap->Bounds().IntegerHeight() + 1,
			ov_bitmap->BytesPerRow());
		SetFlags(B_NOT_ZOOMABLE);
	} else {
		fS9xView->SetViewColor(0, 0, 0);
		SetGraphicsMode(settings.ui.s9x_mode, false);
		SetFlags(B_NOT_ZOOMABLE | B_NOT_RESIZABLE);
	}
	
	fS9xView->Invalidate();
	
	if(settings.ui.s9x_drawmode == DIRECT && runDirectWindow && !fFullScreen){
		TouggleFullScreen();
	}
	
	fGameMenuBar->SetSelectedMode(settings.ui.s9x_drawmode);
	main_locker.Unlock();
	sound_locker.Unlock();
}


void
S9xMainWindow::ChangeControllers()
{
	static char *controllers [] = {"Multiplayer 5 on #0", "Joypad on #0", "Mouse on #1", 
		"Mouse on #0", "Superscope on #1", "Justifier 1 on #1", "Justifier 2 on #1"};
		
	S9xNextController();
	S9xSetInfoString(controllers [IPPU.Controller]);
	
	bool needs_mouse = (IPPU.Controller == SNES_SUPERSCOPE || IPPU.Controller == SNES_MOUSE ||
			IPPU.Controller == SNES_MOUSE_SWAPPED);
	fS9xView->SetNeedsMouse(needs_mouse);
	S9xMouseOn();
}


int
S9xMainWindow::_GetHeight()
{
	if(settings.ui.s9x_extended)
		return SNES_HEIGHT_EXTENDED;
	return SNES_HEIGHT;
}


void
S9xMainWindow::DirectConnected(direct_buffer_info *info)
{
	uint32 j;
	
	switch(info->buffer_state & B_DIRECT_MODE_MASK){
		case B_DIRECT_START:
		case B_DIRECT_MODIFY:
			m_locker->Lock();
			if (m_cliplist){
				free(m_cliplist);
				m_cliplist = 0;
			}
			
			if(direct){
				free(direct);
				direct = 0;
			}
			
			m_numcliprects = info->clip_list_count; 
			m_cliplist = (clipping_rect *)
				malloc(m_numcliprects * sizeof(clipping_rect));
			direct = (direct_info *)
				malloc(m_numcliprects * sizeof(direct_info));
			if (m_cliplist){
				memcpy(m_cliplist, info->clip_list, m_numcliprects*sizeof(clipping_rect));				
				m_rowbytes = info->bytes_per_row;
				m_format = info->pixel_format;
				snes9x_is_32 = m_format == B_RGB32 ? 32 : 16;
				m_bitsperpixel = info->bits_per_pixel >> 3;
				m_bounds = info->window_bounds;
				
				if(!IsFullScreen()){
					m_bounds.top += menubar_height + 1;
					m_bits = (uint8 *)info->bits + (m_bounds.top * m_rowbytes);
					for(j = 0; j < m_numcliprects; j++){
						if (m_cliplist[j].top <= (long)(info->window_bounds.top + menubar_height))
							m_cliplist[j].top = info->window_bounds.top + menubar_height + 1;
					}
				}else{
					switch(settings.ui.s9x_mode){
						case 0 ... 5:
							for(j = 0; j < (uint32)m_numcliprects; j++){
								m_cliplist[j].left = info->window_bounds.left + ((info->window_bounds.right)/2) - ((int32)off_screen.Bounds().right/2);
								m_cliplist[j].top = info->window_bounds.top + ((info->window_bounds.bottom)/2) - ((int32)_GetHeight());
								m_cliplist[j].bottom = ((m_cliplist[j].top)) + ((int32)((_GetHeight() * 2) - 1));
								m_cliplist[j].right = (m_cliplist[j].left) + ((int32)(off_screen.Bounds().right));
							}
						break;
					}
					m_bits = (uint8 *)info->bits + (m_cliplist[0].top * m_rowbytes);
				}
				SetDirectInfo();
				m_connected = true;
			}
			m_locker->Unlock();
			break;
		case B_DIRECT_STOP:
			m_locker->Lock();
			m_connected = false;
			m_locker->Unlock();
			break;
	}
}


void
S9xMainWindow::SetDirectInfo()
{
	bool mFull = IsFullScreen();
	for(uint32 i = 0; i < m_numcliprects; i++) {
		clipping_rect *cl = &m_cliplist[i];
		direct[i].x = mFull ? 0 : (cl->left - m_bounds.left);
		direct[i].y = mFull ? 0 : (cl->top - m_bounds.top);
		direct[i].w = (cl->right - cl->left);
		direct[i].h = (cl->bottom - m_bounds.top);
	}
}


void
S9xMainWindow::Render2xsai()
{
	clipping_rect *clip = m_cliplist;
	for(uint32 i = 0; i < m_numcliprects; i++, clip++) {
		int32 width = (clip->right - clip->left)+1;
		int32 height = (clip->bottom - clip->top)+1;
		if(width == 512 && height == _GetHeight() * 2)
			if(m_format == B_RGB16 || m_format == B_RGB32)
				_2xSaI((uint8 *)off_screen.Bits(), off_screen.BytesPerRow(), 
				(uint8 *)Delta, m_bits + clip->left * m_bitsperpixel, m_rowbytes, 
				width >> 1, height >> 1);
	}
}


void
S9xMainWindow::RenderS2xsai()
{
	clipping_rect *clip = m_cliplist;
	for(uint32 i = 0; i < m_numcliprects; i++, clip++){
		int32 width = (clip->right - clip->left) + 1;
		int32 height = (clip->bottom - clip->top) + 1;
		if(width == 512 && height == _GetHeight() * 2){
			if(m_format == B_RGB16 || m_format == B_RGB32){
				Super2xSaI((uint8 *)off_screen.Bits(), off_screen.BytesPerRow(), 
				(uint8 *)Delta, m_bits + clip->left * m_bitsperpixel, m_rowbytes, 
				width >> 1, height >> 1);
			}
		}
	}
}


void
S9xMainWindow::RenderSeagle()
{
	clipping_rect *clip = m_cliplist;
	for(uint32 i = 0; i < m_numcliprects; i++, clip++){
		int32 width = (clip->right - clip->left) + 1;
		int32 height = (clip->bottom - clip->top) + 1;
		if(width == 512 && height == _GetHeight() * 2){
			if(m_format == B_RGB16 || m_format == B_RGB32){
				SuperEagle((uint8 *)off_screen.Bits(), off_screen.BytesPerRow(), 
				(uint8 *)Delta, m_bits + clip->left * m_bitsperpixel, m_rowbytes, 
				width >> 1, height >> 1);
			}
		}
	}
}


void
S9xMainWindow::RenderScanOrDouble(const int type, bool scanlines)
{
	clipping_rect *clip = m_cliplist;
	direct_info *di = (direct_info *)direct;
	int32 height = _GetHeight() * 2;
	const int multi = scanlines ? 2 : 1;
	uint8 *data = (uint8 *)off_screen.Bits();
	int32 rbytes = off_screen.BytesPerRow();
	
	for(uint32 i = 0; i < m_numcliprects; i++, clip++, di++){
		for(int32 y = 0; y < height; y += multi){
			if(y >= di->y && y <= di->h) {
				uint8 *dst = (uint8 *)(m_bits + ((y) * (m_rowbytes)) +((clip->left) * m_bitsperpixel));
				uint8 *src = (uint8 *)data + ((y >> 1) * rbytes) + (((di->x) >> 1) * type);
				switch(m_format) {
					case B_CMAP8:
						draw_line16to8double(src, dst, (di->w + 1) >> 1, type);
					break;
					case B_RGB16:
						draw_line16to16double(src, dst, (di->w + 1) >> 1, type);
					break;
					case B_RGB32:
						draw_line16to32double(src, dst, (di->w + 1) >> 1, type);
					break;
					default:
					break;
				}
			}
		}
	}
}

void S9xMainWindow::RenderLcd(const int type)
{
	clipping_rect *clip = m_cliplist;
	direct_info *di = (direct_info *)direct;
	int32 height = _GetHeight() * 2;
	uint8 *data = (uint8 *)off_screen.Bits();
	int32 rbytes = off_screen.BytesPerRow();
	
	for(uint32 i = 0; i < m_numcliprects; i++, clip++, di++){
		for(int32 y = 0; y < height; y += 2){
			if(y >= di->y && y <= di->h) {
				uint8 *dst = (uint8 *)(m_bits + ((y) * (m_rowbytes)) +((clip->left) * m_bitsperpixel));
				uint8 *src = (uint8 *)data + ((y >> 1) * rbytes) + (((di->x) >> 1) * type);
				switch(m_format) {
					case B_CMAP8:
						draw_line16to8lcd(src, dst, (di->w + 1) >> 1, type);
					break;
					case B_RGB16:
						draw_line16to16lcd(src, dst, (di->w + 1) >> 1, type);
					break;
					case B_RGB32:
						draw_line16to32lcd(src, dst, (di->w + 1) >> 1, type);
					break;
					default:
					break;
				}
			}
		}
	}
}


void
S9xMainWindow::RenderMulti(int w)
{
	const int type = w > 256 ? 8 : 4;
	clipping_rect *clip = m_cliplist;
	for(uint32 i = 0; i < m_numcliprects; i++, clip++) {
		uint8 *src = (uint8 *)off_screen.Bits();
		uint8 *dst = (uint8 *)m_bits;
		draw_16_to_16_multi(src, dst, type, 0, 512/2, _GetHeight(), 640, 480, off_screen.BytesPerRow(), m_rowbytes);
	}
}


void
S9xMainWindow::RenderNormal()
{
	clipping_rect *clip = m_cliplist;
	direct_info *di = (direct_info *)direct;
	int32 height = _GetHeight();
	
	for(uint32 i = 0; i < m_numcliprects; i++, clip++, di++){
		for(int32 y = 0; y < height; y += 1){
			if(y >= di->y && y <= di->h) {
				uint8 *dst = (uint8 *)(m_bits + ((y) * (m_rowbytes)) +((clip->left) * m_bitsperpixel));
				uint8 *src = (uint8 *)off_screen.Bits() + ((y) * off_screen.BytesPerRow()) + (((di->x)) * 2);
				switch(m_format) {
					case B_CMAP8:
						draw_line16to8(src, dst, (di->w + 1));
						break;
					case B_RGB16:
						draw_line16to16(src, dst, (di->w + 1));
						break;
					case B_RGB32:
						draw_line16to32(src, dst, (di->w + 1));
						break;
					default:
						break;
				}
			}
		}
	}
}


void
S9xMainWindow::DrawDirect(int Width, int Height)
{
	const int type = Width > 256 ? 4 : 2;
	if(needs_clear){
		ClearScreen(); 
		needs_clear = false;
	}
	m_locker->Lock();
	if(m_connected) {
		switch(settings.ui.s9x_mode) {
			case 0:
				RenderScanOrDouble(type, 0);
				break;
			case 1:
				RenderScanOrDouble(type, 1);
				break;
			case 2:
				RenderLcd(type);
				break;
			case 3:
				Render2xsai();
				break;
			case 4:
				RenderS2xsai();
				break;
			case 5:
				RenderSeagle();
				break;
			case 6:
				if(IsFullScreen()) RenderMulti(Width);
				break;
		}
	}
	m_locker->Unlock();
}


void
S9xMainWindow::DirectClear(bool clear)
{
	if(!clear)
		return;
	#include "system_clear_direct.h"
}


void
S9xMainWindow::ResizeTo(float width, float height)
{
	height += menubar_height + 1;
	BDirectWindow::ResizeTo(width, height);
}


void
S9xMainWindow::WindowActivated(bool active)
{
	if(active) 
		needs_clear = true;

	if(!exiting && settings.ui.s9x_inactive && !ispaused) 
		Settings.Paused = !active;

	for(int i = 0; i < 128; i++) 
		key[i].pressed = 0;
		
	BDirectWindow::WindowActivated(active);
}


void
S9xMainWindow::MenuActivated(bool active)
{
	if(!exiting && settings.ui.s9x_menuactive && !ispaused && IsActive())
		Settings.Paused = active; // whew!!
}


void
S9xMainWindow::DePause()
{
	ispaused = false;
	BMenuItem *item = fGameMenuBar->FindItem(S9x_PAUSE);
	if(item) 
		item->SetMarked(false);
}


void
S9xMainWindow::ClearScreen()
{
	if(Delta) {
		int delta_len = (IMAGE_WIDTH + 2) * IMAGE_HEIGHT * 2;
		uint32 *start = (uint32 *)Delta;
		uint32 *end = (uint32 *)Delta + (delta_len >> 2);
		while(start < end)
			*start++ ^= ~0;
	}
	needs_clear = false;
}


void
S9xMainWindow::SetGraphicsMode(int g_mode, bool clear)
{
	settings.ui.s9x_mode = g_mode;
	ResizeTo((SNES_WIDTH * 2) - 1, (_GetHeight() * 2) - 1);
	if(settings.ui.s9x_mode > 0 && m_connected && settings.ui.s9x_drawmode != OVERLAY)
		DirectClear(clear);
}


void
S9xMainWindow::ScreenChanged(BRect frame, color_space mode)
{
	if(!IsFullScreen())
		BScreen().GetMode(&prev_mode);
}


void 
S9xMainWindow::DrawOverlay(int Width, int Height)
{
	if(LockWithTimeout(50000) == B_OK) {
		draw_16_to_ycbr_multi((uint8 *)off_screen.Bits(), (uint8 *)ov_bitmap->Bits(),
			Width, Height, 512, 224, off_screen.BytesPerRow(), ov_bitmap->BytesPerRow());
		Unlock();
	}
}


void
S9xMainWindow::DrawStandard(int Width, int Height)
{
	if(LockWithTimeout(50000) == B_OK) {
		fS9xView->DrawBitmap(&off_screen, BRect(0, 0, Width - 1, Height - 1), fS9xView->Bounds());
		Unlock();
	}
}


S9xMainWindow::~S9xMainWindow()
{
	int32 result;
	exiting = true;
	wait_for_thread(thread, &result);
	if(s9x_player) {
		s9x_player->Stop();
		delete s9x_player;
		s9x_player = 0;
	}
	delete m_locker;
	delete fGameStick;
	delete fFilePanel;
	SetSettings(settings);
	S9xExit();
	kill_thread(TimerID);
	kill_thread(key_id);
	if(IsFullScreen()) BScreen().SetMode(&prev_mode);
}


/* Debounce keyboard buttons */
int 
S9xMainWindow::CheckKey(int code) // handy function from smsplus
{
    static int lastpressed = 0;

    if((!key[code].pressed) && (lastpressed == code))
        lastpressed = 0;

    if((key[code].pressed) && (lastpressed != code)){
        lastpressed = code;
        return (1);
    }
    return (0);
}


void 
S9xMainWindow::TouggleMenu(bool show)
{
	if (show){
		AddChild(fGameMenuBar);
		fS9xView->MoveTo(0, menubar_height + 1);
		fS9xView->ResizeBy(0, -menubar_height - 1);
	}else{
		RemoveChild(fGameMenuBar);
		fS9xView->MoveTo(B_ORIGIN);
		fS9xView->ResizeBy(0, menubar_height + 1);
	}
}


bool
S9xMainWindow::TouggleFullScreen()
{
	if(SetFullScreen(!fFullScreen) == B_OK){
		if (fFullScreen) {
			TouggleMenu(true);
			BScreen().SetMode(&prev_mode);
		} else {
			TouggleMenu(false);
			BScreen().SetMode(&new_mode);
			memset(m_bits, 0, ((int32)Bounds().bottom*m_rowbytes));
			if(!needs_clear) 
				needs_clear = true;
		}
		fGameMenuBar->ToggleMenuWindowMarked(!fFullScreen, S9x_FULLSCREEN);
		fFullScreen = !fFullScreen;
		return true;
	}
	return false;
}


BPath
S9xMainWindow::_GetRomDirectory(directory_which which, const char* cpath)
{
	BPath path;
	status_t error = B_ERROR;
	if (find_directory(which, &path) == B_OK)
	{
		error = B_OK;
		path.Append(cpath);
		BEntry prefsent(path.Path(), true);
		if (!prefsent.Exists())
		{
			error = create_directory(path.Path(),0555);
			if(error != B_OK)
				printf("ERROR %s\n", strerror(error));
		}
	}
	return path;
}


void
S9xMainWindow::_BuildSettingsWindow(int32 tabnr)
{
	BRect rect = BRect(285,87,920,375);
//	if(fSettingsWindow == NULL)
	fSettingsWindow = new SettingsWindow(rect,"Settings", tabnr);
	fSettingsWindow->Show();
}


void
S9xMainWindow::_SetRecentRomFile(const char* name)
{
	if(name == settings.recentroms.recentRom5){
		settings.recentroms.recentRom5 =  settings.recentroms.recentRom4;
		settings.recentroms.recentRom4 =  settings.recentroms.recentRom3;
		settings.recentroms.recentRom3 =  settings.recentroms.recentRom2;
		settings.recentroms.recentRom2 =  settings.recentroms.recentRom1;
		settings.recentroms.recentRom1 =  name;
	}else if(name == settings.recentroms.recentRom4){
		settings.recentroms.recentRom4 =  settings.recentroms.recentRom3;
		settings.recentroms.recentRom3 =  settings.recentroms.recentRom2;
		settings.recentroms.recentRom2 =  settings.recentroms.recentRom1;
		settings.recentroms.recentRom1 =  name;
	}else if(name == settings.recentroms.recentRom3){
		settings.recentroms.recentRom3 =  settings.recentroms.recentRom2;
		settings.recentroms.recentRom2 =  settings.recentroms.recentRom1;
		settings.recentroms.recentRom1 =  name;
	}else if(name == settings.recentroms.recentRom2){
		settings.recentroms.recentRom2 =  settings.recentroms.recentRom1;
		settings.recentroms.recentRom1 =  name;
	}else if(name == settings.recentroms.recentRom1){
		settings.recentroms.recentRom1 =  name;
	}else{
		settings.recentroms.recentRom5 =  settings.recentroms.recentRom4;
		settings.recentroms.recentRom4 =  settings.recentroms.recentRom3;
		settings.recentroms.recentRom3 =  settings.recentroms.recentRom2;
		settings.recentroms.recentRom2 =  settings.recentroms.recentRom1;
		settings.recentroms.recentRom1 =  name;
	}
	fGameMenuBar->RecentRom(settings);
}


int32
s9x_timer(void *o) // from Be's Millipede game(thanks Be)
{
	bigtime_t nextTime, snoozeTime, baseTime;
	baseTime = system_time();
	if((Settings.FrameTime) == 0)
		Settings.FrameTime = 16667;
	while(1) {
		nextTime = baseTime + Settings.FrameTime;
		snoozeTime = (nextTime - system_time());
		
		while (snoozeTime >= 1.0) {
			snooze(snoozeTime);
			snoozeTime = nextTime - system_time();
		}
		FrameTimer++;
		baseTime = nextTime;
	}
	return(0);
}

