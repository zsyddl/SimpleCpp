#ifndef DATA_FILTER_H
#define DATA_FILTER_H

#include <vector>
#include <list>
#include <string>
#include <map>
// #include "Thread.h"
// #include "mutex.h"
#include "MediaSdk.h"
//#include "FilterManager.h"
#include "MediaSdkDef.h"
#include "BaseFilter.h"
#include "FilterPinBuffer.h"
#include "FilterPin.h"

using namespace std;
typedef enum   _eFilterTypeId{
	eFTI_Unkown,
	eFTI_Parser = 10000,

	eFTI_Decoder= 20000,

	eFTI_Render = 30000,
}eFilterType;

typedef enum _eFilterDataMode{
	eFDM_Push =0,
	eFDM_Pull ,
}eFilterDataMode;

#define FILTER_KIND_SOURCE	"Source"
#define FILTER_KIND_DECODER "Decoder"
#define FILTER_KIND_ENCODER "Encoder"
#define FILTER_KIND_RENDER	"Render"

#define FILTER_TYPE_SOURCE  "Source"
#define FILTER_TYPE_DECODER "Decoder"
#define FILTER_TYPE_ENCODER "Encoder"
#define FILTER_TYPE_RENDER	"Render"
#define FILTER_TYPE_SOURCE_FFMPEG	"SourceFfmpegFile"




#define FILTER_NAME_SOURCE 	"Source"
#define FILTER_NAME_DECODER "Decoder"
#define FILTER_NAME_ENCODER "Encoder"
#define FILTER_NAME_RENDER	"Render"
//source
#define FILTER_SOURCE_FFMPEG_FILE	"SourceFfmpegFile"
//decoder
#define FILTER_DECODER_FFMPEG "FfmpegDecoderFilter"
//encoder
#define FILTER_ENCODER_FFMPEG "FfmpegEncoderFilter"

namespace SimpleCpp
{
	class CThread;
	class CMutex;
	class CBuffer;
}


namespace SimpleCpp{
	class CFilterPinBuffer;
	
}
using namespace SimpleCpp;
//class CFilterPin;
// typedef enum _eFilterIOType{
// 	eFIOT_In =0,
// 	eFIOT_Out ,
// }eFilterIOType;




class MEDIASDK_API CMediaFilter:
	public CBaseFilter
{
	
	friend class CFilterManager;
public:
	CMediaFilter(void);
	virtual ~CMediaFilter(void);


	virtual int Open(char *pSzParam=0, bool bHasThread=false);
	
	virtual int InputData(char *pData, int nLen);
	virtual int OutputData(char *pData, int &nLen);

	virtual int Start();
	virtual int Stop();
	virtual int Pause();
	virtual int Resume();
	virtual int Seek(char *pPos);
	virtual int Seek(int64 nTime, int nFlag=0);
	virtual int SeekToTimestamp(int64 s64Timestamp, int nFlag=0);
	virtual int SetPlaySpeed(float fSpeed);
	virtual float GetPlaySpeed();
	virtual int PlayNextFrame();
	virtual int PlayPrevFrame(int64 s64PtsNow=0);
	
	virtual int SetPlayState(int nState);

	virtual int SetPlayMode(eFilterPlayMode playMode);

	virtual int SetShowRect(int nX, int nY, int nW, int nH);
	//设置 buffer的数量
	virtual int SetBufferCount(int nCount, int nIOType=eFPT_Out);
	/*
	* nIOType , eFPT_Out：输出pin, eFPT_In 输入pin
	* 从 pin in 获取的 是 free buffer，
	* 从 pin out 获取的 是 busy buffer
	*/
	virtual int GetBuffer(CFilterPinBuffer **pBuffer, int nIOType=eFPT_Out);
	/*
	* nIOType , eFPT_Out：输出pin, eFPT_In 输入pin
	* 从 pin in 放入的的 是 busy/prapared buffer，
	* 从 pin out 放入的 是 free buffer
	*/
	virtual int PutBuffer(CFilterPinBuffer *pBuffer, int nIOType=eFPT_Out, int nReversedMemory=0);
	//执行一一次，该方法由 外部线程中调用。如果是 filter 内部有线程，则 该方法不调用。
	//filter 执行函数
	virtual int DoFilter();
	//
	virtual int Flush();
	virtual void Reset();
	//
	void GetOutPin(vector<CFilterPin *> &lstPin);
	void GetInPin(vector<CFilterPin *> &lstPin);
	int         GetOutPinCount();
	int			GetInPinCount();
	CFilterPin *GetOutPin(int nIndex);
	CFilterPin *GetInPin(int nIndex);
	CFilterPin *AppendOutPin();
	CFilterPin *AppendInPin();
	//


	//连接 两个pin
	static int Connect(CFilterPin *pPinOut, CFilterPin *pPinIn);
	static int Disconnect(CFilterPin *pPinOut, CFilterPin *pPinIn);
	//static int GetConnectedFilter(CFilterPin *pPin, vector<CMediaFilter*> &lstFilter);

	
	virtual const char *GetParam(char *pSzName);
	virtual void SetParam(char *pSzName, char *pSzValue);
	virtual char*		GetParam();
	virtual void CopyParam(CMediaFilter *pFilter);
	//控制命令，
	virtual int Control(char *pSzName, char *pSzValue);

	// cmd=value; cmd1= value1; .... 
	

	//static void *operator new (size_t size);
	static void *Malloc(size_t size);
	static void Free(void *p);
	
	//dll 外部实现的 类，需要重新自己实现这个接口。 
	virtual void Release();
	//CMediaStream2需要的数据响应
	virtual int  OnPinIn(CFilterPin* pPrevPinOut, CFilterPin* pPinIn, CFilterPinBuffer *pPinBuffer);
	
	//virtual int  Connect(CBaseFilter* pNextFilter);
protected:
	virtual CMediaFilter *CreateObject();
	


	CMutex  *m_pMutexDoFilter;
	//空间太大 会自动释放 内存空间
	int m_nAutoReleaseSize;
	//
	eFilterDataMode m_dataMode;
	//eFilterState m_state;
	//控制状态
	eFilterPlayMode m_playMode;
	//
	float m_fSpeed;
	int64_t m_s64Pts; //播放的时间搓
	int64_t m_s64Dts; //解码的时间戳
	int64_t m_s64Sts; //源文件读取的时间戳
	//控制播放某一段
	int64_t	m_s64BeginTime;
	int64_t m_s64EndTime;
	//
	bool	m_bInit;
	int		m_nFilterTypeId;

	//

};
void StringRemove(string &strSrc, char cRemove);
//extern VIVSMEDIABOX_API CDataFilter g_dataFilter;
#endif


