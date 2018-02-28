#pragma once

//
//语音对讲

//
#ifndef Cb_OnAudioIn
typedef void (__stdcall *Cb_OnAudioIn)(char *pData, int nLength, void *pParam, void *pReversed);
#endif

class CFfmpegCodec;
class CAudioIn;
class CAudioOut;
namespace SimpleCpp{
	class CBuffer;
}
using namespace SimpleCpp;
#define MAX_AUDIO_FRAME_LEN (1024*1024)

class MEDIASDK_API CVoiceIntercom
{
	typedef enum _eRunState{
		eRS_Run,
		eRS_Pause,
		eRS_Stop,
	}eRunState;
	friend static void OnAudioInData(char *pData, int nLength, void *pParam, void *pReversed);
public:
	CVoiceIntercom(void);
	~CVoiceIntercom(void);

	int Open(int nCodec, int nChannels, int nSampleBitDepth, int nSampleRate, int nBitRate );
	void Close();
	int  Start();
	//************************************
	// Method:    InputData
	// FullName:  CVoiceIntercom::InputData
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: char * pData
	// Parameter: int nLen
	//************************************
	int	 InputData(char *pData, int nLen);
	//************************************
	// Method:    SetAudioInDataCb
	// FullName:  CVoiceIntercom::SetAudioInDataCb
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: Cb_OnAudioIn cbAudioIn
	// Parameter: void * pUser
	// Parameter: void * pReversed
	//************************************
	int SetAudioInDataCb(Cb_OnAudioIn cbAudioIn, void *pUserData, void *pReversed);
protected:
	static void __stdcall OnAudioInData(char *pData, int nLength, void *pParam, void *pReversed);
protected:
	CFfmpegCodec *m_pAudioDecode;
	CFfmpegCodec *m_pAudioEncode;
	
	CAudioIn     *m_pAudioIn;
	CAudioOut    *m_pAudioOut;
	//
	CBuffer		 *m_pBufferEncodeTmp;
	CBuffer		 *m_pBufferDecodeTmp;
	//数据回调
	Cb_OnAudioIn m_cbAudioInData;
	void		*m_pCbAudioInData;
	void		*m_pCbAudioInReversed;
	//
	int			m_nRunState;
};
