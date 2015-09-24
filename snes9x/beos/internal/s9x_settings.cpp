#include <Message.h>
#include <File.h>
#include <Path.h>
#include <string.h>
#include <FindDirectory.h>
#include <stdio.h>

#include "s9x_settings.h"
#include "s9x_constants.h"
#include "s9x_key.h"

#include "input_interface.h"
#include "s9x_settings.h"
#include "s9x_includes.h"
#include "S9xView.h"
#include "GameStick.h"
#include "SettingsWindow.h"

void SetDefault(s9x_settings &settings)
{
	memset(&settings, 0, sizeof(settings));

	// Sound
	settings.sound.s9x_mute = 0;
	settings.sound.s9x_stereo = 1;
	settings.sound.s9x_SixteenBitSound = 1;
	settings.sound.s9x_InterpolatedSound = 1;
	settings.sound.s9x_SoundEnvelopeHeightReading = 1;
	settings.sound.s9x_DisableSoundEcho = 0;
	settings.sound.s9x_DisableSampleCaching = 0;
	settings.sound.s9x_DisableMasterVolume = 0;
	settings.sound.s9x_ThreadSound = 0;
	settings.sound.s9x_NextAPUEnabled = 1;
			
	settings.sound.s9x_ReverseStereo = 0;
	settings.sound.s9x_TraceSoundDSP = 0;
	settings.sound.s9x_SoundSync = 0;
	settings.sound.s9x_AltSampleDecode = 0;
	settings.sound.s9x_FixFrequency = 0;
	
	//s9x_ROM_image
    settings.ROMImage.s9x_ForceLoROM = 0;
    settings.ROMImage.s9x_ForceHiROM = 0;
    settings.ROMImage.s9x_ForceHeader = 0;
    settings.ROMImage.s9x_ForceNoHeader = 0;
    settings.ROMImage.s9x_ForceInterleaved = 0;
    settings.ROMImage.s9x_ForceInterleaved2 = 0;
    settings.ROMImage.s9x_ForceNotInterleaved = 0;

	//peripherial
    settings.peripherial.s9x_ForceSuperFX = 0;
    settings.peripherial.s9x_ForceNoSuperFX = 0;
    settings.peripherial.s9x_ForceDSP1 = 0;
    settings.peripherial.s9x_ForceNoDSP1 = 0;
    settings.peripherial.s9x_ForceSA1 = 0;
    settings.peripherial.s9x_ForceNoSA1 = 0;
    settings.peripherial.s9x_ForceC4 = 0;
    settings.peripherial.s9x_ForceNoC4 = 0;
    settings.peripherial.s9x_ForceSDD1 = 0;
    settings.peripherial.s9x_ForceNoSDD1 = 0;
    settings.peripherial.s9x_MultiPlayer5 = 1;
    settings.peripherial.s9x_Mouse = 1;
    settings.peripherial.s9x_SuperScope = 0;
    settings.peripherial.s9x_SRTC = 0;
    settings.peripherial.s9x_ShutdownMaster = 1;
    settings.peripherial.s9x_MultiPlayer5Master = 0;
    settings.peripherial.s9x_SuperScopeMaster = 0;
    settings.peripherial.s9x_MouseMaster = 0;
    settings.peripherial.s9x_SuperFX = 0;
    settings.peripherial.s9x_DSP1Master = 0;
    settings.peripherial.s9x_SA1 = 0;
    settings.peripherial.s9x_C4 = 0;
    settings.peripherial.s9x_SDD1 = 0;
    settings.peripherial.s9x_SPC7110 = 0;
    settings.peripherial.s9x_SPC7110RTC = 0;
    settings.peripherial.s9x_OBC1 = 0;
    settings.peripherial.s9x_ControllerOption = SNES_MULTIPLAYER5;

	//s9x_Graphics
	settings.graphics.s9x_SixteenBit = 1;
	settings.graphics.s9x_Transparency = 1;
	settings.graphics.s9x_SupportHiRes = 1;
	settings.graphics.s9x_Mode7Interpolate = 0;
	settings.graphics.s9x_BGLayering = 0;
	settings.graphics.s9x_DisableGraphicWindows = 0;
	settings.graphics.s9x_ForceTransparency = 1;
	settings.graphics.s9x_ForceNoTransparency = 0;
	settings.graphics.s9x_DisableHDMA = 0;
	settings.graphics.s9x_DisplayFrameRate = 1;
	settings.graphics.s9x_DisableRangeTimeOver = 0;
	
	//s9x_Network
	settings.network.s9x_NetPlay = 0;
	settings.network.s9x_NetPlayServer = 0;
    settings.network.s9x_ServerName = BString("Haiku Server");
    settings.network.s9x_Port = 6096; //From netplay.h

	//s9x_Misc
	settings.misc.s9x_GlideEnable = 0;
	settings.misc.s9x_OpenGLEnable = 0;
	settings.misc.s9x_ApplyCheats = 1;
	settings.misc.s9x_TurboMode = 0;
    settings.misc.s9x_TurboSkipFrames = 15;
    settings.misc.s9x_AutoMaxSkipFrames = 0;
	settings.misc.s9x_AutoSaveDelay = 30;

	// Ui
	settings.ui.s9x_mode = 0;
	settings.ui.s9x_snooze = 1;
	settings.ui.s9x_inactive = 1;
	settings.ui.s9x_allow = 0;
	settings.ui.s9x_menuactive = 0;
	settings.ui.s9x_vsync = 0;
	settings.ui.s9x_extended = 0;
	settings.ui.s9x_drawmode = 0;
	
	//RecentRoms
	settings.recentroms.recentRom1 = BString("");
	settings.recentroms.recentRom2 = BString("");
	settings.recentroms.recentRom3 = BString("");
	settings.recentroms.recentRom4 = BString("");
	settings.recentroms.recentRom5 = BString("");
	
	// Controller
	settings.controll.s9x_SwapJoypads = 0;
	settings.controll.s9x_JoystickEnabled = 1;
	
	// Controller 1
	settings.controll.s9x_controller1.k_up = S9X_UP;
	settings.controll.s9x_controller1.k_down = S9X_DOWN;
	settings.controll.s9x_controller1.k_left = S9X_LEFT;
	settings.controll.s9x_controller1.k_right = S9X_RIGHT;
	settings.controll.s9x_controller1.k_a = S9X_Z;
	settings.controll.s9x_controller1.k_b = S9X_X;
	settings.controll.s9x_controller1.k_x = S9X_C;
	settings.controll.s9x_controller1.k_y = S9X_V;
	settings.controll.s9x_controller1.k_l = S9X_B;
	settings.controll.s9x_controller1.k_r = S9X_LCONTROL;
	settings.controll.s9x_controller1.k_start = S9X_RETURN;
	settings.controll.s9x_controller1.k_select = S9X_SPACE;
	
	// Controller 2
	settings.controll.s9x_controller2.k_up = S9X_Y;
	settings.controll.s9x_controller2.k_down = S9X_B;
	settings.controll.s9x_controller2.k_left = S9X_G;
	settings.controll.s9x_controller2.k_right = S9X_J;
	settings.controll.s9x_controller2.k_a = S9X_B;
	settings.controll.s9x_controller2.k_b = S9X_N;
	settings.controll.s9x_controller2.k_x = S9X_L;
	settings.controll.s9x_controller2.k_y = S9X_LBRACE;
	settings.controll.s9x_controller2.k_l = S9X_RBRACE;
	settings.controll.s9x_controller2.k_r = S9X_HASH;
	settings.controll.s9x_controller2.k_start = S9X_M;
	settings.controll.s9x_controller2.k_select = S9X_RSLASH;
}

void GetSettings(s9x_settings &settings)
{
	BPath path;
	BMessage snes9x;
	if(find_directory(B_USER_SETTINGS_DIRECTORY, &path, true) != B_OK)
	path.SetTo(S9x_SETTINGSPATH);
	path.Append(S9x_SETTINGSFILENAME);
	
	BFile settings_file(path.Path(), B_READ_ONLY);
	if(settings_file.InitCheck() != B_OK) {
		SetDefault(settings);
		return;
	}
	
	snes9x.Unflatten(&settings_file);
	memset(&settings, 0, sizeof(settings));
	
	// Sound
	snes9x.FindInt8("s9x_stereo", (int8 *)&settings.sound.s9x_stereo);
	snes9x.FindInt8("s9x_mute", (int8 *)&settings.sound.s9x_mute);	
	snes9x.FindInt8("s9x_SixteenBitSound", (int8 *)&settings.sound.s9x_SixteenBitSound);
	snes9x.FindInt8("s9x_InterpolatedSound", (int8 *)&settings.sound.s9x_InterpolatedSound);
	snes9x.FindInt8("s9x_SoundEnvelopeHeightReading", (int8 *)&settings.sound.s9x_SoundEnvelopeHeightReading);
	snes9x.FindInt8("s9x_DisableSoundEcho", (int8 *)&settings.sound.s9x_DisableSoundEcho);
	snes9x.FindInt8("s9x_DisableSampleCaching", (int8 *)&settings.sound.s9x_DisableSampleCaching);
	snes9x.FindInt8("s9x_DisableMasterVolume", (int8 *)&settings.sound.s9x_DisableMasterVolume);
	snes9x.FindInt8("s9x_ThreadSound", (int8 *)&settings.sound.s9x_ThreadSound);
	snes9x.FindInt8("s9x_NextAPUEnabled", (int8 *)&settings.sound.s9x_NextAPUEnabled);

	snes9x.FindInt8("s9x_ReverseStereo", (int8 *)&settings.sound.s9x_ReverseStereo);
	snes9x.FindInt8("s9x_TraceSoundDSP", (int8 *)&settings.sound.s9x_TraceSoundDSP);
	snes9x.FindInt8("s9x_SoundSync", (int8 *)&settings.sound.s9x_SoundSync);
	snes9x.FindInt8("s9x_AltSampleDecode", (int8 *)&settings.sound.s9x_AltSampleDecode);
	snes9x.FindInt8("s9x_FixFrequency", (int8 *)&settings.sound.s9x_FixFrequency);				
	
	//s9x_ROM_image
	snes9x.FindInt8("s9x_ForceLoROM", (int8 *)&settings.ROMImage.s9x_ForceLoROM);
    snes9x.FindInt8("s9x_ForceHiROM", (int8 *)&settings.ROMImage.s9x_ForceHiROM);
    snes9x.FindInt8("s9x_ForceHeader", (int8 *)&settings.ROMImage.s9x_ForceHeader);
    snes9x.FindInt8("s9x_ForceNoHeader", (int8 *)&settings.ROMImage.s9x_ForceNoHeader);
    snes9x.FindInt8("s9x_ForceInterleaved", (int8 *)&settings.ROMImage.s9x_ForceInterleaved);
    snes9x.FindInt8("s9x_ForceInterleaved2", (int8 *)&settings.ROMImage.s9x_ForceInterleaved2);
    snes9x.FindInt8("s9x_ForceLoROM", (int8 *)&settings.ROMImage.s9x_ForceNotInterleaved);

	//peripherial
	snes9x.FindInt8("s9x_ForceSuperFX", (int8 *)&settings.peripherial.s9x_ForceSuperFX);
	snes9x.FindInt8("s9x_ForceNoSuperFX", (int8 *)&settings.peripherial.s9x_ForceNoSuperFX);
	snes9x.FindInt8("s9x_ForceDSP1", (int8 *)&settings.peripherial.s9x_ForceDSP1);
	snes9x.FindInt8("s9x_ForceNoDSP1", (int8 *)&settings.peripherial.s9x_ForceNoDSP1);
	snes9x.FindInt8("s9x_ForceSA1", (int8 *)&settings.peripherial.s9x_ForceSA1);
	snes9x.FindInt8("s9x_ForceNoSA1", (int8 *)&settings.peripherial.s9x_ForceNoSA1);
	snes9x.FindInt8("s9x_ForceC4", (int8 *)&settings.peripherial.s9x_ForceC4);
	snes9x.FindInt8("s9x_ForceNoC4", (int8 *)&settings.peripherial.s9x_ForceNoC4);
	snes9x.FindInt8("s9x_ForceSDD1", (int8 *)&settings.peripherial.s9x_ForceSDD1);
	snes9x.FindInt8("s9x_ForceNoSDD1", (int8 *)&settings.peripherial.s9x_ForceNoSDD1);
	snes9x.FindInt8("s9x_MultiPlayer5", (int8 *)&settings.peripherial.s9x_MultiPlayer5);
	snes9x.FindInt8("s9x_Mouse", (int8 *)&settings.peripherial.s9x_Mouse);
	snes9x.FindInt8("s9x_SuperScope", (int8 *)&settings.peripherial.s9x_SuperScope);
	snes9x.FindInt8("s9x_SRTC", (int8 *)&settings.peripherial.s9x_ForceSuperFX);
	snes9x.FindInt8("s9x_ShutdownMaster", (int8 *)&settings.peripherial.s9x_ShutdownMaster);
	snes9x.FindInt8("s9x_MultiPlayer5Master", (int8 *)&settings.peripherial.s9x_MultiPlayer5Master);
	snes9x.FindInt8("s9x_SuperScopeMaster", (int8 *)&settings.peripherial.s9x_SuperScopeMaster);
	snes9x.FindInt8("s9x_MouseMaster", (int8 *)&settings.peripherial.s9x_MouseMaster);
	snes9x.FindInt8("s9x_SuperFX", (int8 *)&settings.peripherial.s9x_SuperFX);
	snes9x.FindInt8("s9x_DSP1Master", (int8 *)&settings.peripherial.s9x_DSP1Master);
	snes9x.FindInt8("s9x_SA1", (int8 *)&settings.peripherial.s9x_SA1);
	snes9x.FindInt8("s9x_C4", (int8 *)&settings.peripherial.s9x_C4);
	snes9x.FindInt8("s9x_SDD1", (int8 *)&settings.peripherial.s9x_SDD1);
	snes9x.FindInt8("s9x_SPC7110", (int8 *)&settings.peripherial.s9x_SPC7110);
	snes9x.FindInt8("s9x_SPC7110RTC", (int8 *)&settings.peripherial.s9x_SPC7110RTC);
	snes9x.FindInt8("s9x_OBC1", (int8 *)&settings.peripherial.s9x_OBC1);	
    snes9x.FindInt32("s9x_ControllerOption", (int32 *)&settings.peripherial.s9x_ControllerOption);
	
	//s9x_Graphics
	snes9x.FindInt8("s9x_SixteenBit", (int8 *)&settings.graphics.s9x_SixteenBit);
	snes9x.FindInt8("s9x_Transparency", (int8 *)&settings.graphics.s9x_Transparency);
	snes9x.FindInt8("s9x_SupportHiRes", (int8 *)&settings.graphics.s9x_SupportHiRes);
	snes9x.FindInt8("s9x_Mode7Interpolate", (int8 *)&settings.graphics.s9x_SixteenBit);
	snes9x.FindInt8("s9x_BGLayering", (int8 *)&settings.graphics.s9x_Mode7Interpolate);
	snes9x.FindInt8("s9x_DisableGraphicWindows", (int8 *)&settings.graphics.s9x_DisableGraphicWindows);
	snes9x.FindInt8("s9x_ForceTransparency", (int8 *)&settings.graphics.s9x_ForceTransparency);
	snes9x.FindInt8("s9x_ForceNoTransparency", (int8 *)&settings.graphics.s9x_ForceNoTransparency);
	snes9x.FindInt8("s9x_DisableHDMA", (int8 *)&settings.graphics.s9x_DisableHDMA);
	snes9x.FindInt8("s9x_DisplayFrameRate", (int8 *)&settings.graphics.s9x_DisplayFrameRate);
	snes9x.FindInt8("s9x_DisableRangeTimeOver", (int8 *)&settings.graphics.s9x_DisableRangeTimeOver);
		
	//s9x_Network
	snes9x.FindInt8("s9x_NetPlay", (int8 *)&settings.network.s9x_NetPlay);
	snes9x.FindInt8("s9x_NetPlayServer", (int8 *)&settings.network.s9x_NetPlayServer);
	snes9x.FindString("s9x_ServerName", (BString*)&settings.network.s9x_ServerName);
	snes9x.FindInt32("s9x_Port", (int32 *)&settings.network.s9x_Port);

	//s9x_Misc
	snes9x.FindInt8("s9x_GlideEnable", (int8 *)&settings.misc.s9x_GlideEnable);
	snes9x.FindInt8("s9x_OpenGLEnable", (int8 *)&settings.misc.s9x_OpenGLEnable);
	snes9x.FindInt8("s9x_ApplyCheats", (int8 *)&settings.misc.s9x_ApplyCheats);
	snes9x.FindInt8("s9x_TurboMode", (int8 *)&settings.misc.s9x_TurboMode);
	snes9x.FindInt32("s9x_TurboSkipFrames", (int32 *)&settings.misc.s9x_TurboSkipFrames);
	snes9x.FindInt32("s9x_AutoMaxSkipFrames", (int32 *)&settings.misc.s9x_AutoMaxSkipFrames);
	snes9x.FindInt8("s9x_AutoSaveDelay", (int8 *)&settings.misc.s9x_AutoSaveDelay);
	
	//RecentRoms
	snes9x.FindString("recentRom1", (BString*)&settings.recentroms.recentRom1);
	snes9x.FindString("recentRom2", (BString*)&settings.recentroms.recentRom2);
	snes9x.FindString("recentRom3", (BString*)&settings.recentroms.recentRom3);
	snes9x.FindString("recentRom4", (BString*)&settings.recentroms.recentRom4);
	snes9x.FindString("recentRom5", (BString*)&settings.recentroms.recentRom5);
						
	// Ui
	snes9x.FindInt8("s9x_mode", (int8 *)&settings.ui.s9x_mode);
	snes9x.FindInt8("s9x_snooze", (int8 *)&settings.ui.s9x_snooze);
	snes9x.FindInt8("s9x_inactive", (int8 *)&settings.ui.s9x_inactive);
	snes9x.FindInt8("s9x_allow", (int8 *)&settings.ui.s9x_allow);
	snes9x.FindInt8("s9x_menuactive", (int8 *)&settings.ui.s9x_menuactive);
	snes9x.FindInt8("s9x_vsync", (int8 *)&settings.ui.s9x_vsync);
	snes9x.FindInt8("s9x_extended", (int8 *)&settings.ui.s9x_extended);
	snes9x.FindInt8("s9x_drawmode", (int8 *)&settings.ui.s9x_drawmode);
	
	// Controller
	snes9x.FindInt8("s9x_SwapJoypads", (int8 *)&settings.controll.s9x_SwapJoypads);
	snes9x.FindInt8("s9x_JoystickEnabled", (int8 *)&settings.controll.s9x_JoystickEnabled);

	// Controller 1
	snes9x.FindInt32("k_up0", (int32 *)&settings.controll.s9x_controller1.k_up);
	snes9x.FindInt32("k_down0", (int32 *)&settings.controll.s9x_controller1.k_down);
	snes9x.FindInt32("k_left0", (int32 *)&settings.controll.s9x_controller1.k_left);
	snes9x.FindInt32("k_right0", (int32 *)&settings.controll.s9x_controller1.k_right);
	snes9x.FindInt32("k_a0", (int32 *)&settings.controll.s9x_controller1.k_a);
	snes9x.FindInt32("k_b0", (int32 *)&settings.controll.s9x_controller1.k_b);
	snes9x.FindInt32("k_x0", (int32 *)&settings.controll.s9x_controller1.k_x);
	snes9x.FindInt32("k_y0", (int32 *)&settings.controll.s9x_controller1.k_y);
	snes9x.FindInt32("k_l0", (int32 *)&settings.controll.s9x_controller1.k_l);
	snes9x.FindInt32("k_r0", (int32 *)&settings.controll.s9x_controller1.k_r);
	snes9x.FindInt32("k_start0", (int32 *)&settings.controll.s9x_controller1.k_start);
	snes9x.FindInt32("k_select0", (int32 *)&settings.controll.s9x_controller1.k_select);
	
	// Controller 2
	snes9x.FindInt32("k_up1", (int32 *)&settings.controll.s9x_controller2.k_up);
	snes9x.FindInt32("k_down1", (int32 *)&settings.controll.s9x_controller2.k_down);
	snes9x.FindInt32("k_left1", (int32 *)&settings.controll.s9x_controller2.k_left);
	snes9x.FindInt32("k_right1", (int32 *)&settings.controll.s9x_controller2.k_right);
	snes9x.FindInt32("k_a1", (int32 *)&settings.controll.s9x_controller2.k_a);
	snes9x.FindInt32("k_b1", (int32 *)&settings.controll.s9x_controller2.k_b);
	snes9x.FindInt32("k_x1", (int32 *)&settings.controll.s9x_controller2.k_x);
	snes9x.FindInt32("k_y1", (int32 *)&settings.controll.s9x_controller2.k_y);
	snes9x.FindInt32("k_l1", (int32 *)&settings.controll.s9x_controller2.k_l);
	snes9x.FindInt32("k_r1", (int32 *)&settings.controll.s9x_controller2.k_r);
	snes9x.FindInt32("k_start1", (int32 *)&settings.controll.s9x_controller2.k_start);
	snes9x.FindInt32("k_select1", (int32 *)&settings.controll.s9x_controller2.k_select);
	
	// Controller 3
	snes9x.FindInt32("k_up2", (int32 *)&settings.controll.s9x_controller3.k_up);
	snes9x.FindInt32("k_down2", (int32 *)&settings.controll.s9x_controller3.k_down);
	snes9x.FindInt32("k_left2", (int32 *)&settings.controll.s9x_controller3.k_left);
	snes9x.FindInt32("k_right2", (int32 *)&settings.controll.s9x_controller3.k_right);
	snes9x.FindInt32("k_a2", (int32 *)&settings.controll.s9x_controller3.k_a);
	snes9x.FindInt32("k_b2", (int32 *)&settings.controll.s9x_controller3.k_b);
	snes9x.FindInt32("k_x2", (int32 *)&settings.controll.s9x_controller3.k_x);
	snes9x.FindInt32("k_y2", (int32 *)&settings.controll.s9x_controller3.k_y);
	snes9x.FindInt32("k_l2", (int32 *)&settings.controll.s9x_controller3.k_l);
	snes9x.FindInt32("k_r2", (int32 *)&settings.controll.s9x_controller3.k_r);
	snes9x.FindInt32("k_start2", (int32 *)&settings.controll.s9x_controller3.k_start);
	snes9x.FindInt32("k_select2", (int32 *)&settings.controll.s9x_controller3.k_select);
	
	// Controller 4
	snes9x.FindInt32("k_up3", (int32 *)&settings.controll.s9x_controller4.k_up);
	snes9x.FindInt32("k_down3", (int32 *)&settings.controll.s9x_controller4.k_down);
	snes9x.FindInt32("k_left3", (int32 *)&settings.controll.s9x_controller4.k_left);
	snes9x.FindInt32("k_right3", (int32 *)&settings.controll.s9x_controller4.k_right);
	snes9x.FindInt32("k_a3", (int32 *)&settings.controll.s9x_controller4.k_a);
	snes9x.FindInt32("k_b3", (int32 *)&settings.controll.s9x_controller4.k_b);
	snes9x.FindInt32("k_x3", (int32 *)&settings.controll.s9x_controller4.k_x);
	snes9x.FindInt32("k_y3", (int32 *)&settings.controll.s9x_controller4.k_y);
	snes9x.FindInt32("k_l3", (int32 *)&settings.controll.s9x_controller4.k_l);
	snes9x.FindInt32("k_r3", (int32 *)&settings.controll.s9x_controller4.k_r);
	snes9x.FindInt32("k_start3", (int32 *)&settings.controll.s9x_controller4.k_start);
	snes9x.FindInt32("k_select3", (int32 *)&settings.controll.s9x_controller4.k_select);
	
	// Controller 5
	snes9x.FindInt32("k_up4", (int32 *)&settings.controll.s9x_controller5.k_up);
	snes9x.FindInt32("k_down4", (int32 *)&settings.controll.s9x_controller5.k_down);
	snes9x.FindInt32("k_left4", (int32 *)&settings.controll.s9x_controller5.k_left);
	snes9x.FindInt32("k_right4", (int32 *)&settings.controll.s9x_controller5.k_right);
	snes9x.FindInt32("k_a4", (int32 *)&settings.controll.s9x_controller5.k_a);
	snes9x.FindInt32("k_b4", (int32 *)&settings.controll.s9x_controller5.k_b);
	snes9x.FindInt32("k_x4", (int32 *)&settings.controll.s9x_controller5.k_x);
	snes9x.FindInt32("k_y4", (int32 *)&settings.controll.s9x_controller5.k_y);
	snes9x.FindInt32("k_l4", (int32 *)&settings.controll.s9x_controller5.k_l);
	snes9x.FindInt32("k_r4", (int32 *)&settings.controll.s9x_controller5.k_r);
	snes9x.FindInt32("k_start4", (int32 *)&settings.controll.s9x_controller5.k_start);
	snes9x.FindInt32("k_select4", (int32 *)&settings.controll.s9x_controller5.k_select);
	
	snes9x.Flatten(&settings_file);
}


static inline status_t SaveInt8(BMessage &msg, const char *string, int8 data)
{
	if(msg.ReplaceInt8(string, data) == B_NAME_NOT_FOUND)
		return msg.AddInt8(string, data);
	return msg.ReplaceInt8(string, data);
}


static inline status_t SaveInt32(BMessage &msg, const char *string, int32 data)
{
	if(msg.ReplaceInt32(string, data) == B_NAME_NOT_FOUND)
		return msg.AddInt32(string, data);
	return msg.ReplaceInt32(string, data);
}


static inline status_t SaveString(BMessage &msg, const char *string, BString data)
{
	if(msg.ReplaceString(string, data) == B_NAME_NOT_FOUND)
		return msg.AddString(string, data);
	return msg.ReplaceString(string, data);
}


void SetSettings(const s9x_settings &settings)
{
	BPath path;
	BMessage snes9x;
	if(find_directory(B_USER_SETTINGS_DIRECTORY, &path, true) != B_OK)
	path.SetTo(S9x_SETTINGSPATH);
	path.Append(S9x_SETTINGSFILENAME);
	
	BFile settings_file(path.Path(), B_READ_WRITE | B_CREATE_FILE);
	snes9x.Unflatten(&settings_file);
	
	// Sound
	SaveInt8(snes9x, "s9x_stereo", settings.sound.s9x_stereo);
	SaveInt8(snes9x, "s9x_mute", settings.sound.s9x_mute);	
	SaveInt8(snes9x, "s9x_SixteenBitSound", settings.sound.s9x_SixteenBitSound);
	SaveInt8(snes9x, "s9x_InterpolatedSound", settings.sound.s9x_InterpolatedSound);
	SaveInt8(snes9x, "s9x_SoundEnvelopeHeightReading", settings.sound.s9x_SoundEnvelopeHeightReading);
	SaveInt8(snes9x, "s9x_DisableSoundEcho", settings.sound.s9x_DisableSoundEcho);
	SaveInt8(snes9x, "s9x_DisableSampleCaching", settings.sound.s9x_DisableSampleCaching);
	SaveInt8(snes9x, "s9x_DisableMasterVolume", settings.sound.s9x_DisableMasterVolume);
	SaveInt8(snes9x, "s9x_ThreadSound", settings.sound.s9x_ThreadSound);
	SaveInt8(snes9x, "s9x_NextAPUEnabled", settings.sound.s9x_NextAPUEnabled);

	SaveInt8(snes9x, "s9x_ReverseStereo", settings.sound.s9x_ReverseStereo);
	SaveInt8(snes9x, "s9x_TraceSoundDSP", settings.sound.s9x_TraceSoundDSP);
	SaveInt8(snes9x, "s9x_SoundSync", settings.sound.s9x_SoundSync);
	SaveInt8(snes9x, "s9x_AltSampleDecode", settings.sound.s9x_AltSampleDecode);
	SaveInt8(snes9x, "s9x_FixFrequency", settings.sound.s9x_FixFrequency);				
	
	//s9x_ROM_image
	SaveInt8(snes9x, "s9x_ForceLoROM", settings.ROMImage.s9x_ForceLoROM);
    SaveInt8(snes9x, "s9x_ForceHiROM", settings.ROMImage.s9x_ForceHiROM);
    SaveInt8(snes9x, "s9x_ForceHeader", settings.ROMImage.s9x_ForceHeader);
    SaveInt8(snes9x, "s9x_ForceNoHeader", settings.ROMImage.s9x_ForceNoHeader);
    SaveInt8(snes9x, "s9x_ForceInterleaved", settings.ROMImage.s9x_ForceInterleaved);
    SaveInt8(snes9x, "s9x_ForceInterleaved2", settings.ROMImage.s9x_ForceInterleaved2);
    SaveInt8(snes9x, "s9x_ForceLoROM", settings.ROMImage.s9x_ForceNotInterleaved);

	//peripherial
	SaveInt8(snes9x, "s9x_ForceSuperFX", settings.peripherial.s9x_ForceSuperFX);
	SaveInt8(snes9x, "s9x_ForceNoSuperFX", settings.peripherial.s9x_ForceNoSuperFX);
	SaveInt8(snes9x, "s9x_ForceDSP1", settings.peripherial.s9x_ForceDSP1);
	SaveInt8(snes9x, "s9x_ForceNoDSP1", settings.peripherial.s9x_ForceNoDSP1);
	SaveInt8(snes9x, "s9x_ForceSA1", settings.peripherial.s9x_ForceSA1);
	SaveInt8(snes9x, "s9x_ForceNoSA1", settings.peripherial.s9x_ForceNoSA1);
	SaveInt8(snes9x, "s9x_ForceC4", settings.peripherial.s9x_ForceC4);
	SaveInt8(snes9x, "s9x_ForceNoC4", settings.peripherial.s9x_ForceNoC4);
	SaveInt8(snes9x, "s9x_ForceSDD1", settings.peripherial.s9x_ForceSDD1);
	SaveInt8(snes9x, "s9x_ForceNoSDD1", settings.peripherial.s9x_ForceNoSDD1);
	SaveInt8(snes9x, "s9x_MultiPlayer5", settings.peripherial.s9x_MultiPlayer5);
	SaveInt8(snes9x, "s9x_Mouse", settings.peripherial.s9x_Mouse);
	SaveInt8(snes9x, "s9x_SuperScope", settings.peripherial.s9x_SuperScope);
	SaveInt8(snes9x, "s9x_SRTC", settings.peripherial.s9x_ForceSuperFX);
	SaveInt8(snes9x, "s9x_ShutdownMaster", settings.peripherial.s9x_ShutdownMaster);
	SaveInt8(snes9x, "s9x_MultiPlayer5Master", settings.peripherial.s9x_MultiPlayer5Master);
	SaveInt8(snes9x, "s9x_SuperScopeMaster", settings.peripherial.s9x_SuperScopeMaster);
	SaveInt8(snes9x, "s9x_MouseMaster", settings.peripherial.s9x_MouseMaster);
	SaveInt8(snes9x, "s9x_SuperFX", settings.peripherial.s9x_SuperFX);
	SaveInt8(snes9x, "s9x_DSP1Master", settings.peripherial.s9x_DSP1Master);
	SaveInt8(snes9x, "s9x_SA1", settings.peripherial.s9x_SA1);
	SaveInt8(snes9x, "s9x_C4", settings.peripherial.s9x_C4);
	SaveInt8(snes9x, "s9x_SDD1", settings.peripherial.s9x_SDD1);
	SaveInt8(snes9x, "s9x_SPC7110", settings.peripherial.s9x_SPC7110);
	SaveInt8(snes9x, "s9x_SPC7110RTC", settings.peripherial.s9x_SPC7110RTC);
	SaveInt8(snes9x, "s9x_OBC1", settings.peripherial.s9x_OBC1);	
    SaveInt32(snes9x, "s9x_ControllerOption", settings.peripherial.s9x_ControllerOption);
	
	//s9x_Graphics
	SaveInt8(snes9x, "s9x_SixteenBit", settings.graphics.s9x_SixteenBit);
	SaveInt8(snes9x, "s9x_Transparency", settings.graphics.s9x_Transparency);
	SaveInt8(snes9x, "s9x_SupportHiRes", settings.graphics.s9x_SupportHiRes);
	SaveInt8(snes9x, "s9x_Mode7Interpolate", settings.graphics.s9x_SixteenBit);
	SaveInt8(snes9x, "s9x_BGLayering", settings.graphics.s9x_Mode7Interpolate);
	SaveInt8(snes9x, "s9x_DisableGraphicWindows", settings.graphics.s9x_DisableGraphicWindows);
	SaveInt8(snes9x, "s9x_ForceTransparency", settings.graphics.s9x_ForceTransparency);
	SaveInt8(snes9x, "s9x_ForceNoTransparency", settings.graphics.s9x_ForceNoTransparency);
	SaveInt8(snes9x, "s9x_DisableHDMA", settings.graphics.s9x_DisableHDMA);
	SaveInt8(snes9x, "s9x_DisplayFrameRate", settings.graphics.s9x_DisplayFrameRate);
	SaveInt8(snes9x, "s9x_DisableRangeTimeOver", settings.graphics.s9x_DisableRangeTimeOver);
		
	//s9x_Network
	SaveInt8(snes9x, "s9x_NetPlay", settings.network.s9x_NetPlay);
	SaveInt8(snes9x, "s9x_NetPlayServer", settings.network.s9x_NetPlayServer);
	SaveString(snes9x, "s9x_ServerName", settings.network.s9x_ServerName);
	SaveInt32(snes9x, "s9x_Port", settings.network.s9x_Port);

	//s9x_Misc
	SaveInt8(snes9x, "s9x_GlideEnable", settings.misc.s9x_GlideEnable);
	SaveInt8(snes9x, "s9x_OpenGLEnable", settings.misc.s9x_OpenGLEnable);
	SaveInt8(snes9x, "s9x_ApplyCheats", settings.misc.s9x_ApplyCheats);
	SaveInt8(snes9x, "s9x_TurboMode", settings.misc.s9x_TurboMode);
	SaveInt32(snes9x, "s9x_TurboSkipFrames", settings.misc.s9x_TurboSkipFrames);
	SaveInt32(snes9x, "s9x_AutoMaxSkipFrames", settings.misc.s9x_AutoMaxSkipFrames);
	SaveInt8(snes9x, "s9x_AutoSaveDelay", settings.misc.s9x_AutoSaveDelay);
	
	//RecentRoms
	SaveString(snes9x, "recentRom1", settings.recentroms.recentRom1);
	SaveString(snes9x, "recentRom2", settings.recentroms.recentRom2);
	SaveString(snes9x, "recentRom3", settings.recentroms.recentRom3);
	SaveString(snes9x, "recentRom4", settings.recentroms.recentRom4);
	SaveString(snes9x, "recentRom5", settings.recentroms.recentRom5);
	
	// Ui
	SaveInt8(snes9x, "s9x_mode", settings.ui.s9x_mode);
	SaveInt8(snes9x, "s9x_snooze", settings.ui.s9x_snooze);
	SaveInt8(snes9x, "s9x_inactive", settings.ui.s9x_inactive);
	SaveInt8(snes9x, "s9x_allow", settings.ui.s9x_allow);
	SaveInt8(snes9x, "s9x_menuactive", settings.ui.s9x_menuactive);
	SaveInt8(snes9x, "s9x_vsync", settings.ui.s9x_vsync);
	SaveInt8(snes9x, "s9x_extended", settings.ui.s9x_extended);
	SaveInt8(snes9x, "s9x_drawmode", settings.ui.s9x_drawmode);
	
	// Controller
	SaveInt8(snes9x, "s9x_SwapJoypads", settings.controll.s9x_SwapJoypads);
	SaveInt8(snes9x, "s9x_JoystickEnabled", settings.controll.s9x_JoystickEnabled);
	
	// Controller 1
	SaveInt32(snes9x, "k_up0", settings.controll.s9x_controller1.k_up);
	SaveInt32(snes9x, "k_down0", settings.controll.s9x_controller1.k_down);
	SaveInt32(snes9x, "k_left0", settings.controll.s9x_controller1.k_left);
	SaveInt32(snes9x, "k_right0", settings.controll.s9x_controller1.k_right);
	SaveInt32(snes9x, "k_a0", settings.controll.s9x_controller1.k_a);
	SaveInt32(snes9x, "k_b0", settings.controll.s9x_controller1.k_b);
	SaveInt32(snes9x, "k_x0", settings.controll.s9x_controller1.k_x);
	SaveInt32(snes9x, "k_y0", settings.controll.s9x_controller1.k_y);
	SaveInt32(snes9x, "k_l0", settings.controll.s9x_controller1.k_l);
	SaveInt32(snes9x, "k_r0", settings.controll.s9x_controller1.k_r);
	SaveInt32(snes9x, "k_start0", settings.controll.s9x_controller1.k_start);
	SaveInt32(snes9x, "k_select0", settings.controll.s9x_controller1.k_select);
	
	// Controller 2
	SaveInt32(snes9x, "k_up1", settings.controll.s9x_controller2.k_up);
	SaveInt32(snes9x, "k_down1", settings.controll.s9x_controller2.k_down);
	SaveInt32(snes9x, "k_left1", settings.controll.s9x_controller2.k_left);
	SaveInt32(snes9x, "k_right1", settings.controll.s9x_controller2.k_right);
	SaveInt32(snes9x, "k_a1", settings.controll.s9x_controller2.k_a);
	SaveInt32(snes9x, "k_b1", settings.controll.s9x_controller2.k_b);
	SaveInt32(snes9x, "k_x1", settings.controll.s9x_controller2.k_x);
	SaveInt32(snes9x, "k_y1", settings.controll.s9x_controller2.k_y);
	SaveInt32(snes9x, "k_l1", settings.controll.s9x_controller2.k_l);
	SaveInt32(snes9x, "k_r1", settings.controll.s9x_controller2.k_r);
	SaveInt32(snes9x, "k_start1", settings.controll.s9x_controller2.k_start);
	SaveInt32(snes9x, "k_select1", settings.controll.s9x_controller2.k_select);
	
	// Controller 3
	SaveInt32(snes9x, "k_up2", settings.controll.s9x_controller3.k_up);
	SaveInt32(snes9x, "k_down2", settings.controll.s9x_controller3.k_down);
	SaveInt32(snes9x, "k_left2", settings.controll.s9x_controller3.k_left);
	SaveInt32(snes9x, "k_right2", settings.controll.s9x_controller3.k_right);
	SaveInt32(snes9x, "k_a2", settings.controll.s9x_controller3.k_a);
	SaveInt32(snes9x, "k_b2", settings.controll.s9x_controller3.k_b);
	SaveInt32(snes9x, "k_x2", settings.controll.s9x_controller3.k_x);
	SaveInt32(snes9x, "k_y2", settings.controll.s9x_controller3.k_y);
	SaveInt32(snes9x, "k_l2", settings.controll.s9x_controller3.k_l);
	SaveInt32(snes9x, "k_r2", settings.controll.s9x_controller3.k_r);
	SaveInt32(snes9x, "k_start2", settings.controll.s9x_controller3.k_start);
	SaveInt32(snes9x, "k_select2", settings.controll.s9x_controller3.k_select);
	
	// Controller 4
	SaveInt32(snes9x, "k_up3", settings.controll.s9x_controller4.k_up);
	SaveInt32(snes9x, "k_down3", settings.controll.s9x_controller4.k_down);
	SaveInt32(snes9x, "k_left3", settings.controll.s9x_controller4.k_left);
	SaveInt32(snes9x, "k_right3", settings.controll.s9x_controller4.k_right);
	SaveInt32(snes9x, "k_a3", settings.controll.s9x_controller4.k_a);
	SaveInt32(snes9x, "k_b3", settings.controll.s9x_controller4.k_b);
	SaveInt32(snes9x, "k_x3", settings.controll.s9x_controller4.k_x);
	SaveInt32(snes9x, "k_y3", settings.controll.s9x_controller4.k_y);
	SaveInt32(snes9x, "k_l3", settings.controll.s9x_controller4.k_l);
	SaveInt32(snes9x, "k_r3", settings.controll.s9x_controller4.k_r);
	SaveInt32(snes9x, "k_start3", settings.controll.s9x_controller4.k_start);
	SaveInt32(snes9x, "k_select3", settings.controll.s9x_controller4.k_select);
	
	// Controller 5
	SaveInt32(snes9x, "k_up4", settings.controll.s9x_controller5.k_up);
	SaveInt32(snes9x, "k_down4", settings.controll.s9x_controller5.k_down);
	SaveInt32(snes9x, "k_left4", settings.controll.s9x_controller5.k_left);
	SaveInt32(snes9x, "k_right4", settings.controll.s9x_controller5.k_right);
	SaveInt32(snes9x, "k_a4", settings.controll.s9x_controller5.k_a);
	SaveInt32(snes9x, "k_b4", settings.controll.s9x_controller5.k_b);
	SaveInt32(snes9x, "k_x4", settings.controll.s9x_controller5.k_x);
	SaveInt32(snes9x, "k_y4", settings.controll.s9x_controller5.k_y);
	SaveInt32(snes9x, "k_l4", settings.controll.s9x_controller5.k_l);
	SaveInt32(snes9x, "k_r4", settings.controll.s9x_controller5.k_r);
	SaveInt32(snes9x, "k_start4", settings.controll.s9x_controller5.k_start);
	SaveInt32(snes9x, "k_select4", settings.controll.s9x_controller5.k_select);
	
	settings_file.SetSize(0);
	settings_file.Seek(0, SEEK_SET);
	snes9x.Flatten(&settings_file);
}
