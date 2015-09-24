#ifndef _SOUNDVIEW_H
#define _SOUNDVIEW_H

#include <View.h>

class BBox;
class BCheckBox;

#include "s9x_settings.h"

class SoundView : public BView
{
public:
	SoundView(BRect frame, const char* name, float tabHeight);
	~SoundView();

protected:
//---------------------------------------
	BBox*		fBoxSoundQuality;
	BCheckBox*	fChkbox16BitPlayback;
	BCheckBox*	fChkboxStereo;
	BCheckBox*	fChkboxRevertStereo;
	BCheckBox*	fChkboxInterpolation;
	BCheckBox*	fChkboxSampelSync;
	
//---------------------------------------
	BBox*		fBoxSoundEmulator;	
	BCheckBox*	fChkboxVolumeEnvelope;
	BCheckBox*	fChkboxEchoEffect;
	BCheckBox*	fChkboxCachDecoded;
	BCheckBox*	fChkboxMasterVolume;
	BCheckBox*	fChkboxMuteSound;
	BCheckBox*	fChkboxFrameAdvanceMute;
	BCheckBox*	fChkboxAntiResonance;
	BCheckBox*	fChkboxFakeMuteDesyncWorkaround;
	BCheckBox*	fChkboxEnableSoundCPUEmulation;
};

#endif //_SOUNDVIEW_H
