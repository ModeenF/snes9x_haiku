#include "SoundView.h"

#include <Box.h>
#include <CheckBox.h>

#include "s9x_constants.h"

extern "C" s9x_settings settings;

/***** Constructor ****/
SoundView::SoundView(BRect frame,const char *name, float tabHeight)
: BView(frame, name, B_FOLLOW_NONE, B_WILL_DRAW)
{
	SetViewColor(216, 216, 216);
	int space = 5;
	int boxWidth = (Bounds().Width() / 2)-space;
	int chkBoxWidth = 15;
	int chkBoxHight = 15;
	
// ---------------------
	BRect boxRect = BRect(Bounds().left + space, Bounds().top + space, Bounds().left + boxWidth, Bounds().bottom - (tabHeight + (space*2)));
	fBoxSoundQuality = new BBox(boxRect, "fBoxSoundQuality", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE, B_FANCY_BORDER);
	fBoxSoundQuality->SetLabel("Sound Quality");

	BRect rect = BRect(boxRect.left + space, boxRect.top + space, boxRect.Width() - space, boxRect.top + chkBoxHight);
	fChkbox16BitPlayback = new BCheckBox(rect, "fChkbox16BitPlayback", "16 Bit Playback", new BMessage(S9x_16BITPLAYBACK),B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	fChkbox16BitPlayback->SetValue(settings.sound.s9x_SixteenBitSound);	
	fBoxSoundQuality->AddChild(fChkbox16BitPlayback);

	rect.OffsetBy(0, space + chkBoxHight);
	fChkboxStereo = new BCheckBox(rect, "fChkboxStereo", "Stereo", new BMessage(S9x_STEREO),B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	fChkboxStereo->SetValue(settings.sound.s9x_stereo);
	fBoxSoundQuality->AddChild(fChkboxStereo);
	
	rect.OffsetBy(0, space + chkBoxHight);
	fChkboxRevertStereo = new BCheckBox(rect, "fChkboxRevertStereo", "Revert Stereo", new BMessage(S9x_REVERTSTEREO),B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	fChkboxRevertStereo->SetValue(settings.sound.s9x_ReverseStereo);	
	fBoxSoundQuality->AddChild(fChkboxRevertStereo);
	
	rect.OffsetBy(0, space + chkBoxHight);
	fChkboxInterpolation = new BCheckBox(rect, "fChkboxInterpolation", "Gaussian Interpolation of Sample Data", new BMessage(S9x_GAUSSIAN),B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	fChkboxInterpolation->SetValue(settings.sound.s9x_InterpolatedSound);	
	fBoxSoundQuality->AddChild(fChkboxInterpolation);
	
	rect.OffsetBy(0, space + chkBoxHight);
	fChkboxSampelSync = new BCheckBox(rect, "fChkboxSampelSync", "Generate Sampel data in sync with sound CPU", new BMessage(S9x_GENERATESAMPEL),B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	fChkboxSampelSync->SetValue(settings.sound.s9x_SoundSync);	
	fBoxSoundQuality->AddChild(fChkboxSampelSync);
	
	AddChild(fBoxSoundQuality);

// ---------------------
	boxRect.OffsetBy(boxRect.Width()+ space, 0);
	fBoxSoundEmulator = new BBox(boxRect, "fBoxSoundEmulator", B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE, B_FANCY_BORDER);
	fBoxSoundEmulator->SetLabel("Sound Emulator");
	
	boxRect.OffsetBy(-(boxRect.Width()+ space), 0);
	rect = BRect(boxRect.left + space, boxRect.top + space, boxRect.Width() - space, boxRect.top + chkBoxHight);
	fChkboxVolumeEnvelope = new BCheckBox(rect, "fChkboxVolumeEnvelope", "Volume envelope height reading", new BMessage(S9x_VOLUMEHEIGHT),B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	fChkboxVolumeEnvelope->SetValue(settings.sound.s9x_SoundEnvelopeHeightReading);	
	fBoxSoundEmulator->AddChild(fChkboxVolumeEnvelope);

	rect.OffsetBy(0, space + chkBoxHight);
	fChkboxEchoEffect = new BCheckBox(rect, "fChkboxEchoEffect", "Disable Echo Effects", new BMessage(S9x_ECHO),B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	fChkboxEchoEffect->SetValue(settings.sound.s9x_DisableSoundEcho);		
	fBoxSoundEmulator->AddChild(fChkboxEchoEffect);
	
	rect.OffsetBy(0, space + chkBoxHight);
	fChkboxCachDecoded = new BCheckBox(rect, "fChkboxCachDecoded", "Disable Sample Caching", new BMessage(S9x_CACHEDECODER),B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	fChkboxCachDecoded->SetValue(settings.sound.s9x_DisableSampleCaching);		
	fBoxSoundEmulator->AddChild(fChkboxCachDecoded);
	
	rect.OffsetBy(0, space + chkBoxHight);
	fChkboxMasterVolume = new BCheckBox(rect, "fChkboxMasterVolume", "Disable Master Volume control", new BMessage(S9x_MASTERVOLUME),B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	fChkboxMasterVolume->SetValue(settings.sound.s9x_DisableMasterVolume);
	fBoxSoundEmulator->AddChild(fChkboxMasterVolume);
	
	rect.OffsetBy(0, space + chkBoxHight);
	fChkboxMuteSound = new BCheckBox(rect, "fChkboxMuteSound", "Mute sound", new BMessage(S9x_MUTE),B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	fChkboxMuteSound->SetValue(settings.sound.s9x_mute);		
	fBoxSoundEmulator->AddChild(fChkboxMuteSound);
	
/*	rect.OffsetBy(0, space + chkBoxHight);
	fChkboxFrameAdvanceMute = new BCheckBox(rect, "fChkboxFrameAdvanceMute", "Frame advance mute", new BMessage(S9x_FRAMEMUTE),B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	fBoxSoundEmulator->AddChild(fChkboxFrameAdvanceMute);*/
	
	rect.OffsetBy(0, space + chkBoxHight);
	fChkboxAntiResonance = new BCheckBox(rect, "fChkboxAntiResonance", "Anti-Resonance's sample decoding method", new BMessage(S9x_ANTIRESON),B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	fChkboxAntiResonance->SetValue(settings.sound.s9x_AltSampleDecode);
	fBoxSoundEmulator->AddChild(fChkboxAntiResonance);
	
/*	rect.OffsetBy(0, space + chkBoxHight);
	fChkboxFakeMuteDesyncWorkaround = new BCheckBox(rect, "fChkboxFakeMuteDesyncWorkaround", "Fake Mute desync workaround", new BMessage(S9x_FAKEMUTE),B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
	fBoxSoundEmulator->AddChild(fChkboxFakeMuteDesyncWorkaround);*/
	
/*	rect.OffsetBy(0, space + chkBoxHight);
	fChkboxEnableSoundCPUEmulation = new BCheckBox(rect, "fChkboxEnableSoundCPUEmulation", "Enable sound CPU emulation", new BMessage(S9x_ENABLE),B_FOLLOW_LEFT | B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE);
//	fChkboxEnableSoundCPUEmulation->SetValue(settings.sound.s9x_SoundSync);
	fBoxSoundEmulator->AddChild(fChkboxEnableSoundCPUEmulation);*/

	AddChild(fBoxSoundEmulator);	
}

/***** Destructor ****/
SoundView::~SoundView()
{
}

