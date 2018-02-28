
#ifndef VIVS_PLAY_SDK_ERROR_H
#define VIVS_PLAY_SDK_ERROR_H

typedef enum _eVivsPlaySdkError{
	eVPSE_OK = 0,
	eVPSE_AllocMemoryFalse = 1000,
	eVPSE_NewStreamFalse = 2000,
	eVPSE_OpenStreamFalse,
	eVPSE_InvalidStream,
	eVPSE_SnapShotFalse,

	eVPSE_NewVoiceIntercomFalse = 3000,
	eVPSE_OpenVoiceIntercomFalse,
	eVPSE_OpenMediaFileFalse = 4000,
}eVivsPlaySdkError;

#endif