#include "s9x_includes.h"
#include "S9xMainWindow.h"

extern uint32 FrameTimer;
extern s9x_settings settings;
extern int xx;
extern int yy;
extern uint32 button_d;
uint32 joypads [5] = { 0 };
int superscope_pause = 0;
int superscope_turbo = 0;
static S9xMainWindow *s9x = 0;

//Linking references
uint8 *Delta;
BLocker sound_locker;
BSoundPlayer *s9x_player;

static void sleep_until()
{
	static bigtime_t prev_tick = 0;
	static const bigtime_t frame_snooze = 1000000 / 60;
	bigtime_t snooze_time = frame_snooze - (system_time() - prev_tick);
	if (snooze_time > 0) 
		snooze(snooze_time);
	prev_tick = system_time();
}


static void ClearKeys(input_interface *ai)
{
	ai[S9X_SCRLOCK].pressed = 0;
	ai[S9X_PAUSE].pressed = 0;
	ai[S9X_NUMLOCK].pressed = 0;
	ai[S9X_CAPSLOCK].pressed = 0;
}

static int S9xCompareSDD1IndexEntries (const void *p1, const void *p2)
{
    return (*(uint32 *) p1 - *(uint32 *) p2);
}

static int Rates[8] =
{
    0, 8192, 11025, 16500, 22050, 29300, 36600, 44100
};


/* -------------------------- pragama Mark -------------------------------------------*/

void 
S9xInitDisplay()
{
	Init_2xSaI (565);
	
	memset(s9x->Screen()->Bits(), 0, s9x->Screen()->BitsLength());
	memset(s9x->SubScreen()->Bits(), 0, s9x->SubScreen()->BitsLength());
	
    GFX.Screen = (uint8 *)s9x->Screen()->Bits();
    GFX.Pitch = s9x->Screen()->BytesPerRow();
	Delta = new uint8 [(IMAGE_WIDTH + 2) * IMAGE_HEIGHT * 2];
	GFX.SubScreen = (uint8 *)s9x->SubScreen()->Bits();
	GFX.ZBuffer = new uint8[((int)(SCREEN_WIDTH) * IMAGE_HEIGHT)];
	GFX.SubZBuffer = new uint8[((int)(SCREEN_WIDTH) * IMAGE_HEIGHT)];
}


void
OutOfMemory ()
{
	Memory.Deinit ();
    S9xDeinitAPU ();
}


void
InitTimer()
{
	FrameTimer = 0;
}


int32
fThread(void *o)
{
	S9xMainWindow *w = (S9xMainWindow *)o;
	s9x = w;
	return w->OThread();
}


bool8
S9xInitUpdate ()
{
    return (TRUE);
}


bool8
S9xDeinitUpdate (int Width, int Height, bool8 sixteen_bit)
{
	if(settings.ui.s9x_vsync)
		BScreen().WaitForRetrace();
	switch(settings.ui.s9x_drawmode) {
		case DIRECT:
			s9x->DrawDirect(Width, Height);
			break;
		case OVERLAY:
			s9x->DrawOverlay(Width, Height);
			break;
		case BITMAP:
			s9x->DrawStandard(Width, Height);
			break;
	}
	if(settings.ui.s9x_snooze)
		sleep_until();
	asm("emms");
	return (true);
}


uint32 
S9xReadJoypad (int which1)
{
	return (which1 < 5) ? (0x80000000 | joypads [which1]) : 0;
}


void
S9xProcessEvents (bool8 block)
{
	s9x->ProcessEvents();
}


int32
S9xMainWindow::KeyThread(void *o)
{
	S9xMainWindow *d = (S9xMainWindow *)o;
	while(1) {
		key_info info;
		if(d->IsActive() && get_key_info(&info) == B_NO_ERROR) {
			for(int i = 0; i < 128; i++)
				d->key[i].pressed = info.key_states[i >> 3] & (1 << (7 - (i % 8)));
		}
		ClearKeys(d->key);
		snooze(100000 / 30);
	}
	return 0;
}


void
S9xMainWindow::ProcessEvents()
{
	input_interface *ai = key;
	for(int i = 0; i < 5; i++)
		joypads[i] = 0;
	
	// Controller 1
	ai[settings.controll.s9x_controller1.k_up].pressed		? joypads[0] |= SNES_UP_MASK : 0;
	ai[settings.controll.s9x_controller1.k_down].pressed		? joypads[0] |= SNES_DOWN_MASK : 0;
	ai[settings.controll.s9x_controller1.k_left].pressed		? joypads[0] |= SNES_LEFT_MASK : 0;
	ai[settings.controll.s9x_controller1.k_right].pressed 	? joypads[0] |= SNES_RIGHT_MASK : 0;
	ai[settings.controll.s9x_controller1.k_a].pressed		? joypads[0] |= SNES_A_MASK : 0;
	ai[settings.controll.s9x_controller1.k_b].pressed 		? joypads[0] |= SNES_B_MASK : 0;
	ai[settings.controll.s9x_controller1.k_x].pressed 		? joypads[0] |= SNES_X_MASK : 0;
	ai[settings.controll.s9x_controller1.k_y].pressed 		? joypads[0] |= SNES_Y_MASK : 0;
	ai[settings.controll.s9x_controller1.k_l].pressed 		? joypads[0] |= SNES_TL_MASK : 0;
	ai[settings.controll.s9x_controller1.k_r].pressed 		? joypads[0] |= SNES_TR_MASK : 0;
	ai[settings.controll.s9x_controller1.k_start].pressed 	? joypads[0] |= SNES_START_MASK : 0;
	ai[settings.controll.s9x_controller1.k_select].pressed 	? joypads[0] |= SNES_SELECT_MASK : 0;
	
	// Controller 2
	ai[settings.controll.s9x_controller2.k_up].pressed 		? joypads[1] |= SNES_UP_MASK : 0;
	ai[settings.controll.s9x_controller2.k_down].pressed 	? joypads[1] |= SNES_DOWN_MASK : 0;
	ai[settings.controll.s9x_controller2.k_left].pressed 	? joypads[1] |= SNES_LEFT_MASK : 0;
	ai[settings.controll.s9x_controller2.k_right].pressed 	? joypads[1] |= SNES_RIGHT_MASK : 0;
	ai[settings.controll.s9x_controller2.k_a].pressed 		? joypads[1] |= SNES_A_MASK : 0;
	ai[settings.controll.s9x_controller2.k_b].pressed 		? joypads[1] |= SNES_B_MASK : 0;
	ai[settings.controll.s9x_controller2.k_x].pressed 		? joypads[1] |= SNES_X_MASK : 0;
	ai[settings.controll.s9x_controller2.k_y].pressed 		? joypads[1] |= SNES_Y_MASK : 0;
	ai[settings.controll.s9x_controller2.k_l].pressed 		? joypads[1] |= SNES_TL_MASK : 0;
	ai[settings.controll.s9x_controller2.k_r].pressed 		? joypads[1] |= SNES_TR_MASK : 0;
	ai[settings.controll.s9x_controller2.k_start].pressed 	? joypads[1] |= SNES_START_MASK : 0;
	ai[settings.controll.s9x_controller2.k_select].pressed 	? joypads[1] |= SNES_SELECT_MASK : 0;
	
	// Controller 3
	ai[settings.controll.s9x_controller3.k_up].pressed 		? joypads[2] |= SNES_UP_MASK : 0;
	ai[settings.controll.s9x_controller3.k_down].pressed 	? joypads[2] |= SNES_DOWN_MASK : 0;
	ai[settings.controll.s9x_controller3.k_left].pressed 	? joypads[2] |= SNES_LEFT_MASK : 0;
	ai[settings.controll.s9x_controller3.k_right].pressed 	? joypads[2] |= SNES_RIGHT_MASK : 0;
	ai[settings.controll.s9x_controller3.k_a].pressed 		? joypads[2] |= SNES_A_MASK : 0;
	ai[settings.controll.s9x_controller3.k_b].pressed 		? joypads[2] |= SNES_B_MASK : 0;
	ai[settings.controll.s9x_controller3.k_x].pressed 		? joypads[2] |= SNES_X_MASK : 0;
	ai[settings.controll.s9x_controller3.k_y].pressed 		? joypads[2] |= SNES_Y_MASK : 0;
	ai[settings.controll.s9x_controller3.k_l].pressed 		? joypads[2] |= SNES_TL_MASK : 0;
	ai[settings.controll.s9x_controller3.k_r].pressed 		? joypads[2] |= SNES_TR_MASK : 0;
	ai[settings.controll.s9x_controller3.k_start].pressed 	? joypads[2] |= SNES_START_MASK : 0;
	ai[settings.controll.s9x_controller3.k_select].pressed 	? joypads[2] |= SNES_SELECT_MASK : 0;
	
	// Controller 4
	ai[settings.controll.s9x_controller4.k_up].pressed 		? joypads[3] |= SNES_UP_MASK : 0;
	ai[settings.controll.s9x_controller4.k_down].pressed 	? joypads[3] |= SNES_DOWN_MASK : 0;
	ai[settings.controll.s9x_controller4.k_left].pressed 	? joypads[3] |= SNES_LEFT_MASK : 0;
	ai[settings.controll.s9x_controller4.k_right].pressed 	? joypads[3] |= SNES_RIGHT_MASK : 0;
	ai[settings.controll.s9x_controller4.k_a].pressed 		? joypads[3] |= SNES_A_MASK : 0;
	ai[settings.controll.s9x_controller4.k_b].pressed 		? joypads[3] |= SNES_B_MASK : 0;
	ai[settings.controll.s9x_controller4.k_x].pressed 		? joypads[3] |= SNES_X_MASK : 0;
	ai[settings.controll.s9x_controller4.k_y].pressed 		? joypads[3] |= SNES_Y_MASK : 0;
	ai[settings.controll.s9x_controller4.k_l].pressed 		? joypads[3] |= SNES_TL_MASK : 0;
	ai[settings.controll.s9x_controller4.k_r].pressed 		? joypads[3] |= SNES_TR_MASK : 0;
	ai[settings.controll.s9x_controller4.k_start].pressed 	? joypads[3] |= SNES_START_MASK : 0;
	ai[settings.controll.s9x_controller4.k_select].pressed 	? joypads[3] |= SNES_SELECT_MASK : 0;
	
	// Controller 5
	ai[settings.controll.s9x_controller5.k_up].pressed 		? joypads[4] |= SNES_UP_MASK : 0;
	ai[settings.controll.s9x_controller5.k_down].pressed 	? joypads[4] |= SNES_DOWN_MASK : 0;
	ai[settings.controll.s9x_controller5.k_left].pressed 	? joypads[4] |= SNES_LEFT_MASK : 0;
	ai[settings.controll.s9x_controller5.k_right].pressed 	? joypads[4] |= SNES_RIGHT_MASK : 0;
	ai[settings.controll.s9x_controller5.k_a].pressed 		? joypads[4] |= SNES_A_MASK : 0;
	ai[settings.controll.s9x_controller5.k_b].pressed 		? joypads[4] |= SNES_B_MASK : 0;
	ai[settings.controll.s9x_controller5.k_x].pressed 		? joypads[4] |= SNES_X_MASK : 0;
	ai[settings.controll.s9x_controller5.k_y].pressed 		? joypads[4] |= SNES_Y_MASK : 0;
	ai[settings.controll.s9x_controller5.k_l].pressed 		? joypads[4] |= SNES_TL_MASK : 0;
	ai[settings.controll.s9x_controller5.k_r].pressed 		? joypads[4] |= SNES_TR_MASK : 0;
	ai[settings.controll.s9x_controller5.k_start].pressed 	? joypads[4] |= SNES_START_MASK : 0;
	ai[settings.controll.s9x_controller5.k_select].pressed 	? joypads[4] |= SNES_SELECT_MASK : 0;
	
	Settings.TurboMode = ai[S9X_TAB].pressed;
	if(CheckKey(S9X_F1)) 
		S9xStates(0);
	else if(CheckKey(S9X_F2)) 
		S9xStates(1);
	else if(CheckKey(S9X_F3)) 
		S9xStates(2);
	else if(CheckKey(S9X_F4)) 
		S9xStates(3);
	else if(CheckKey(S9X_F5)) 
		S9xStates(4);
	
	if(CheckKey(S9X_F6)) 
		S9xStates(0, false);
	else if(CheckKey(S9X_F7)) 
		S9xStates(1, false);
	else if(CheckKey(S9X_F8)) 
		S9xStates(2, false);
	else if(CheckKey(S9X_F9)) 
		S9xStates(3, false);
	else if(CheckKey(S9X_F10)) 
		S9xStates(4, false);
}

void 
S9xSyncSpeed ()
{
    if (!Settings.TurboMode && Settings.SkipFrames == AUTO_FRAMERATE) {
		while (FrameTimer == 0) {
		    S9xProcessEvents (FALSE);
		}

		if (FrameTimer > 10)
	    	FrameTimer = 10;

		if (FrameTimer > 1 && IPPU.SkippedFrames < 10) {
		    IPPU.SkippedFrames++;
	    	IPPU.RenderThisFrame = FALSE;
		} else {
		    IPPU.RenderThisFrame = TRUE;
	    	IPPU.SkippedFrames = 0;
		}
		FrameTimer--;
    } else {
		if (++IPPU.FrameSkip >= Settings.SkipFrames) {
	    	IPPU.FrameSkip = 0;
		    IPPU.RenderThisFrame = TRUE;
		    IPPU.SkippedFrames = 0;
		} else {
		    IPPU.SkippedFrames++;
	    	IPPU.RenderThisFrame = FALSE;
		}
    }
}


void 
S9xMessage(int type, int message_no, const char *str)
{
	S9xSetInfoString (str);
}


void 
_makepath (char *path, const char *, const char *dir, const char *fname, 
	const char *ext)
{
    if (dir && *dir) {
		strcpy (path, dir);
		strcat (path, "/");
    } else
		*path = 0;
    	strcat (path, fname);
	    if (ext && *ext) {
	        strcat (path, ".");
    	    strcat (path, ext);
    	}
}


void
_splitpath (const char *path, char *drive, char *dir, char *fname, char *ext)
{
    *drive = 0;

    char *slash = strrchr (path, '/');
    if (!slash)
		slash = strrchr (path, '\\');

    char *dot = strrchr (path, '.');

    if (dot && slash && dot < slash)
		dot = NULL;

    if (!slash) {
		strcpy (dir, "");
		strcpy (fname, path);
        	if (dot) {
			    *(fname + (dot - path)) = 0;
			    strcpy (ext, dot + 1);
        	} else
		    	strcpy (ext, "");
    } else {
		strcpy (dir, path);
		*(dir + (slash - path)) = 0;
		strcpy (fname, slash + 1);
        if (dot) {
		    *(fname + (dot - slash) - 1) = 0;
    	    strcpy (ext, dot + 1);
		} else
		    strcpy (ext, "");
    }
}


const char*
S9xBasename (const char *f)
{
    const char *p;
    if ((p = strrchr (f, '/')) != NULL || (p = strrchr (f, '\\')) != NULL)
		return (p + 1);

    if ((p = strrchr (f, SLASH_CHAR)))
		return (p + 1);

    return (f);
}


const char*
S9xGetFilename (const char *ex)
{
    static char filename [PATH_MAX + 1];
    char drive [_MAX_DRIVE + 1];
    char dir [_MAX_DIR + 1];
    char fname [_MAX_FNAME + 1];
    char ext [_MAX_EXT + 1];

    _splitpath (Memory.ROMFilename, drive, dir, fname, ext);
    strcpy (filename, S9xGetSnapshotDirectory ());
    strcat (filename, SLASH_STR);
    strcat (filename, fname);
    strcat (filename, ex);

    return (filename);
}


const char*
GetHomeDirectory ()
{
    return (getenv ("HOME"));
}


const char*
S9xGetSnapshotDirectory ()
{
	static char filename [PATH_MAX];
	BPath path;
	if(find_directory(B_USER_SETTINGS_DIRECTORY, &path, true) != B_OK)
	path.SetTo(S9x_SETTINGSPATH);
	path.Append(S9x_SNAPSHOTDIR);
	strcpy(filename, path.Path());
	mkdir (filename, 0777);
	return filename;
}


void
S9xAutoSaveSRAM ()
{
    Memory.SaveSRAM (S9xGetFilename (S9x_FILESRM));
}

void S9xParseArg (char **argv, int &i, int argc)
{
}


void S9xSetPalette()
{
}


void
S9xGenerateSound ()
{
}


void
S9xExtraUsage ()
{
}


bool8
S9xReadSuperScopePosition (int &x, int &y, uint32 &buttons)
{
    x = xx;
    y = yy;
    buttons = button_d | (superscope_turbo << 2) | 
              (superscope_pause << 3);
    return (TRUE);
}


bool8
S9xReadMousePosition (int which1, int &x, int &y, uint32 &buttons)
{
	if (which1 == 0) {
      x = xx;
      y = yy;
      buttons = button_d;
      return (TRUE);
    }
    return (FALSE);
}


bool
JustifierOffscreen()
{
	return (bool)((button_d&2)!=0);
}


void
JustifierButtons(uint32& justifiers)
{
	if(IPPU.Controller==SNES_JUSTIFIER_2){
		if((button_d&1)||(button_d&2))
			justifiers|=0x00200;

		if(button_d&4)
			justifiers|=0x00800;
			
	}else{
		if((button_d&1)||(button_d&2))
			justifiers|=0x00100;

		if(button_d&4)
			justifiers|=0x00400;
	}
}


extern "C" char* 
osd_GetPackDir()
{
  static char filename[_MAX_PATH];
  memset(filename, 0, _MAX_PATH);
  
  if(strlen(S9xGetSnapshotDirectory())!=0)
    strcpy (filename, S9xGetSnapshotDirectory());
  else {
    char dir [_MAX_DIR + 1];
    char drive [_MAX_DRIVE + 1];
    char name [_MAX_FNAME + 1];
    char ext [_MAX_EXT + 1];
    _splitpath(Memory.ROMFilename, drive, dir, name, ext);
    _makepath(filename, drive, dir, NULL, NULL);
  }
  
  if(!strncmp((char*)&Memory.ROM [0xffc0], "SUPER POWER LEAG 4   ", 21)) {
    if (getenv("SPL4PACK"))
		return getenv("SPL4PACK");
    else 
      	strcat(filename, "/SPL4-SP7");
  } else if(!strncmp((char*)&Memory.ROM [0xffc0], "MOMOTETSU HAPPY      ",21)) {
    if (getenv("MDHPACK"))
		return getenv("MDHPACK");
    else 
      	strcat(filename, "/SMHT-SP7");
  } else if(!strncmp((char*)&Memory.ROM [0xffc0], "HU TENGAI MAKYO ZERO ", 21)) {
    if (getenv("FEOEZPACK"))
    	return getenv("FEOEZPACK");
    else 
      	strcat(filename, "/FEOEZSP7");
  } else if(!strncmp((char*)&Memory.ROM [0xffc0], "JUMP TENGAIMAKYO ZERO",21)) {
    if (getenv("SJNSPACK"))
    	return getenv("SJNSPACK");
    else 
		strcat(filename, "/SJUMPSP7");
  } else 
  	strcat(filename, "/MISC-SP7");
  return filename;
}


const char*
S9xGetFilenameInc (const char *e)
{
    static char filename [_MAX_PATH + 1];
    char drive [_MAX_DRIVE + 1];
    char dir [_MAX_DIR + 1];
    char fname [_MAX_FNAME + 1];
    char ext [_MAX_EXT + 1];
    char *ptr;
    struct stat buf;

    if (strlen (S9xGetSnapshotDirectory())) {
        _splitpath (Memory.ROMFilename, drive, dir, fname, ext);
        strcpy (filename, S9xGetSnapshotDirectory());
        strcat (filename, "/");
        strcat (filename, fname);
        ptr = filename + strlen (filename);
        strcat (filename, "00/");
        strcat (filename, e);
    } else {
        _splitpath (Memory.ROMFilename, drive, dir, fname, ext);
        strcat (fname, "00/");
        _makepath (filename, drive, dir, fname, e);
        ptr = strstr (filename, "00/");
    }

    do {
        if (++*(ptr + 2) > '9') {
            *(ptr + 2) = '0';
            if (++*(ptr + 1) > '9') {
                *(ptr + 1) = '0';
                if (++*ptr > '9')
                    break;
            }
        }
    } while( stat(filename, &buf) == 0 );

    return (filename);
}


void
S9xSetTitle (const char *string)
{
	s9x->SetTitle(string);
}


bool8
S9xOpenSnapshotFile (const char *fname, bool8 read_only, STREAM *file)
{
    char filename [PATH_MAX + 1];
    char drive [_MAX_DRIVE + 1];
    char dir [_MAX_DIR + 1];
    char ext [_MAX_EXT + 1];

    _splitpath (fname, drive, dir, filename, ext);

    if (*drive || *dir == '/' || (*dir == '.' && (*(dir + 1) == '/'))) {
		strcpy (filename, fname);
		if (!*ext)
		    strcat (filename, ".s96");
    } else {
	strcpy (filename, S9xGetSnapshotDirectory ());
	strcat (filename, SLASH_STR);
	strcat (filename, fname);
	if (!*ext)
	    strcat (filename, ".s96");
    }
    
#ifdef ZLIB
    if (read_only) {
		if ((*file = OPEN_STREAM (filename, "rb")))
		    return (TRUE);
    } else {
		if ((*file = OPEN_STREAM (filename, "wb"))) {
		    chown (filename, getuid (), getgid ());
	    	return (TRUE);
		}
    }
#else
    char command [PATH_MAX];
    
    if (read_only) {
		sprintf (command, "gzip -d <\"%s\"", filename);
		if (*file = popen (command, "r"))
	    	return (TRUE);
    } else {
		sprintf (command, "gzip --best >\"%s\"", filename);
		if (*file = popen (command, "wb"))
	    	return (TRUE);
    }
#endif
    return (FALSE);
}


void
S9xCloseSnapshotFile (STREAM file)
{
#ifdef ZLIB
    CLOSE_STREAM (file);
#else
    pclose (file);
#endif
}


void
S9xExit()
{
	if(GFX.ZBuffer) {
		delete [] GFX.ZBuffer;
		GFX.ZBuffer = 0; 
	}
	
	if(GFX.SubZBuffer) {
		delete [] GFX.SubZBuffer;
		GFX.SubZBuffer = 0;
	}
	Memory.SaveSRAM (S9xGetFilename (".srm"));
    Memory.Deinit ();
    S9xDeinitAPU ();
}


void
BufferProc(void *cookie, void *buff, size_t len, const media_raw_audio_format &format)
{
	if(settings.sound.s9x_mute) {
		memset(buff, 0, len); 
		return;
	}
	sound_locker.Lock();
	S9xMixSamples((uint8 *)buff,(so.sixteen_bit?so.buffer_size>>1:so.buffer_size));
	sound_locker.Unlock();
}


bool8
S9xOpenSoundDevice (int mode, bool8 stereo, int buffer_size)
{
	media_raw_audio_format format;
    int playback_rate = Rates [mode & 7];
    int sixteen_bit = TRUE;
    int in_stereo = stereo;

    so.mute_sound = TRUE;
    
    format.frame_rate = playback_rate;
    format.channel_count = (in_stereo ? 2 : 1);
    format.format = media_raw_audio_format::B_AUDIO_SHORT;
    format.buffer_size = buffer_size;
    format.byte_order = B_MEDIA_LITTLE_ENDIAN;
    
 	s9x_player = new BSoundPlayer(&format, "Snes9x", BufferProc);
 	
 	if(s9x_player->InitCheck() != B_OK) {
 		s9x_player->Stop();
 		delete s9x_player;
 		s9x_player = 0;
		return (FALSE);
    }
#if DEBUG
    printf("buffer_size: %ld\n", format.buffer_size);
    printf("Initialized Audio: %f hz %s\n", format.frame_rate, (format.channel_count == 1 ? "Mono" : "Stereo"));
#endif
	s9x_player->Start();
	so.stereo = in_stereo;
	so.sixteen_bit = sixteen_bit;
	so.playback_rate = playback_rate;
	so.buffer_size = buffer_size;
	so.encoded = FALSE;
	s9x_player->SetHasData(true);
	
#if DEBUG
    printf ("Rate: %d, Buffer size: %d, 16-bit: %s, Stereo: %s, Encoded: %s\n",
	    so.playback_rate, so.buffer_size, so.sixteen_bit ? "yes" : "no",
	    so.stereo ? "yes" : "no", so.encoded ? "yes" : "no");
#endif
    so.mute_sound = FALSE;

    return (true);
}


void
S9xLoadSDD1Data ()
{
    char filename [_MAX_PATH + 1];
    char index [_MAX_PATH + 1];
    char data [_MAX_PATH + 1];
    char patch [_MAX_PATH + 1];

    Memory.FreeSDD1Data ();

    strcpy (filename, S9xGetSnapshotDirectory ());

    if (strncmp (Memory.ROMName, "Star Ocean", 10) == 0)
		strcat (filename, "/socnsdd1");
    else
		strcat (filename, "/sfa2sdd1");

    DIR *dir = opendir (filename);

    index [0] = 0;
    data [0] = 0;
    patch [0] = 0;

    if(dir){
		struct dirent *d;
	
		while ((d = readdir (dir))){
	    	if (strcasecmp (d->d_name, "SDD1GFX.IDX") == 0){
				strcpy (index, filename);
				strcat (index, "/");
				strcat (index, d->d_name);
	    	}else if(strcasecmp (d->d_name, "SDD1GFX.DAT") == 0){
				strcpy (data, filename);
				strcat (data, "/");
				strcat (data, d->d_name);
			}else if (strcasecmp (d->d_name, "SDD1GFX.PAT") == 0){
				strcpy (patch, filename);
				strcat (patch, "/");
				strcat (patch, d->d_name);	    		
	    	}
		}
		closedir (dir);

		if(strlen (index) && strlen (data)){
		    FILE *fs = fopen (index, "rb");
	    	int len = 0;

		    if(fs){
				// Index is stored as a sequence of entries, each entry being
				// 12 bytes consisting of:
				// 4 byte key: (24bit address & 0xfffff * 16) | translated block
				// 4 byte ROM offset
				// 4 byte length
				fseek (fs, 0, SEEK_END);
				len = ftell (fs);
				rewind (fs);
				Memory.SDD1Index = (uint8 *) malloc (len);
				fread (Memory.SDD1Index, 1, len, fs);
				fclose (fs);
				Memory.SDD1Entries = len / 12;
	
				if(!(fs = fopen (data, "rb"))){
			    	free ((char *) Memory.SDD1Index);
				    Memory.SDD1Index = NULL;
			    	Memory.SDD1Entries = 0;
				}else{
			    	fseek (fs, 0, SEEK_END);
				    len = ftell (fs);
				    rewind (fs);
			    	Memory.SDD1Data = (uint8 *) malloc (len);
			    	fread (Memory.SDD1Data, 1, len, fs);
				    fclose (fs);

				    if(strlen (patch) > 0 && (fs = fopen (patch, "rb"))){
						fclose (fs);
		    		}
#ifdef MSB_FIRST
			    	// Swap the byte order of the 32-bit value triplets on
			    	// MSBFirst machines.
			    	uint8 *ptr = Memory.SDD1Index;
		    		for(int i = 0; i < Memory.SDD1Entries; i++, ptr += 12){
						SWAP_DWORD ((*(uint32 *) (ptr + 0)));
						SWAP_DWORD ((*(uint32 *) (ptr + 4)));
						SWAP_DWORD ((*(uint32 *) (ptr + 8)));
				    }
#endif
			    	qsort(Memory.SDD1Index, Memory.SDD1Entries, 12, S9xCompareSDD1IndexEntries);
				}
	    	}
		}else{
	    	printf ("Decompressed data pack not found in '%s'.\n", filename);
		}
    }
}
