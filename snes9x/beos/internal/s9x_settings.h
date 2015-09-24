#ifndef __SNES9X_SETTINGS_H__
#define __SNES9X_SETTINGS_H__

#include <String.h>

struct s9x_sound
{
	uint8 
	s9x_FixFrequency,				s9x_mute, 				s9x_stereo, 
	s9x_SixteenBitSound,			s9x_ReverseStereo,		s9x_TraceSoundDSP, 	
	s9x_SoundEnvelopeHeightReading, s9x_DisableSoundEcho, 	s9x_DisableSampleCaching, 
	s9x_DisableMasterVolume, 		s9x_SoundSync,			s9x_InterpolatedSound,
	s9x_ThreadSound, 				s9x_NextAPUEnabled, 	s9x_AltSampleDecode;
};


struct s9x_ROM_image
{
	uint8 
    s9x_ForceLoROM, 		s9x_ForceHiROM, 		s9x_ForceHeader, 	
    s9x_ForceNoHeader,		s9x_ForceInterleaved,	s9x_ForceInterleaved2, 	
    s9x_ForceNotInterleaved;
};


struct s9x_Peripherial
{
	uint8 
    s9x_ForceSuperFX, 	s9x_ForceNoSuperFX, s9x_ForceDSP1, s9x_ForceNoDSP1, 
    s9x_ForceSA1, s9x_ForceNoSA1, s9x_ForceC4, s9x_ForceNoC4, s9x_ForceSDD1,
    s9x_ForceNoSDD1, s9x_MultiPlayer5, s9x_Mouse, s9x_SuperScope, s9x_SRTC,     
    s9x_ShutdownMaster, s9x_MultiPlayer5Master, s9x_SuperScopeMaster, 
    s9x_MouseMaster, s9x_SuperFX, s9x_DSP1Master, s9x_SA1, s9x_C4,
    s9x_SDD1, s9x_SPC7110, s9x_SPC7110RTC, s9x_OBC1;

    uint32 s9x_ControllerOption;	
};


struct s9x_Graphics
{
	uint8 s9x_SixteenBit, s9x_Transparency, s9x_SupportHiRes, s9x_Mode7Interpolate, 
		s9x_BGLayering,s9x_DisableGraphicWindows,s9x_ForceTransparency,
		s9x_ForceNoTransparency, s9x_DisableHDMA, s9x_DisplayFrameRate, 
		s9x_DisableRangeTimeOver;
};


struct s9x_Network
{
	uint8 	s9x_NetPlay, s9x_NetPlayServer;
    BString	s9x_ServerName;
    uint32  s9x_Port;
};


struct s9x_Misc
{
	uint8 s9x_GlideEnable, s9x_OpenGLEnable, s9x_ApplyCheats, s9x_TurboMode; 
    uint32 s9x_TurboSkipFrames, s9x_AutoMaxSkipFrames, 
    s9x_AutoSaveDelay; /* Time in seconds before S-RAM auto-saved if modified. */
};
    

struct s9x_ui
{
	uint8 			
	s9x_mode, 		s9x_snooze,	s9x_inactive, 	s9x_allow,	
	s9x_menuactive, s9x_vsync, 	s9x_extended, 	s9x_drawmode;
};


struct s9x_keys
{
	uint32 k_up, k_down, k_left, k_right, k_a, k_b, k_x, k_y, k_l,
		k_r, k_start, k_select;
};


struct RecentRoms
{
	BString recentRom1;
	BString recentRom2;
	BString recentRom3;
	BString recentRom4;
	BString recentRom5;
};


struct s9x_controll
{
	uint8 s9x_SwapJoypads, s9x_JoystickEnabled;
	s9x_keys s9x_controller1;
	s9x_keys s9x_controller2;
	s9x_keys s9x_controller3;
	s9x_keys s9x_controller4;
	s9x_keys s9x_controller5;
};


struct s9x_settings
{
	s9x_ui 			ui;
	s9x_sound 		sound;
	s9x_controll 	controll;
	s9x_ROM_image 	ROMImage;
	s9x_Peripherial	peripherial;
	s9x_Network		network;
	s9x_Graphics	graphics;
	s9x_Misc		misc;
	RecentRoms 		recentroms;
};

void GetSettings(s9x_settings &settings);
void SetSettings(const s9x_settings &settings);

#endif
