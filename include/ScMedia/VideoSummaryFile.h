#pragma once

#include <vector>
#include <list>
#include <map>
#include "MediaSdkDef.h"

using namespace std;

/*
* 用于保存或者读取 视频摘要 文件 的类 ，
* 视频摘要的存储是一个组合的文件夹，由多个文件和文件夹组成
*  "video summary file"  "object [folder]"        "pic [folder]"   "video [folder]"
*                		     |                   	    |
*                		    object files         	 pictures
*/
#define OBJECT_FOLDER	"object"
#define PIC_FOLDER		"pic"
#define VIDEO_FOLDER	"video"
typedef long long int64_t;
//一个摘要对象， 每个摘要文件 包含很多个摘要对象 
//每个摘要对象 包含很多个 摘要图片 
namespace SimpleCpp{
	class CMutex;
}
using namespace SimpleCpp;

class TiXmlDocument;
class TiXmlNode;
class TiXmlElement;
class MEDIASDK_API CVideoSummaryFilePicture
{
	friend class  CVideoSummaryFile;
	friend class  CVideoSummaryFileObject;
public:
	CVideoSummaryFilePicture();
	~CVideoSummaryFilePicture();

	int SetData(char *pData, int nLen);
	int SetSize(int nW, int nH);
	int GetSize(int &nW, int &nH);
	int SetPos(int nX, int nY);
	int GetPos(int &nX, int &nY);
	//
	bool PointTest(int nX, int nY);
	//
	int SetBackground(char *pSzBkg);
	//时间戳
	int64_t m_s64TimeStamp;
	int		m_nFrameIdInVideo;
	int     m_nObjectId;
	int     m_nPictureId;
	//出现的 次数
	int     m_nCount;

	int m_nPixFormat;
	char *GetFullPath(){return m_szFullPath;}
	char *GetFullBkgPath(){return m_szFullBkgPath;};
	char *GetBkgFile(){return m_szBkg;}

 
	//写入的时候才有用
	char *m_pData;
	int m_nDataLen;

	int m_nW;
	int m_nH;
	int m_nX;
	int m_nY;
	char m_szBkg[64];
	char m_szFile[64];
	//读取的时候用
	char m_szFullPath[256];
	char m_szFullBkgPath[256];
	//
 
};
typedef enum  _eVideoSummaryFileObjectMoveCursor{
	eVSFOMC_Next = 0,
	eVSFOMC_Prev,
	eVSFOMC_NoMove,
}eVideoSummaryFileObjectMoveCursor;
class MEDIASDK_API CVideoSummaryFileObject
{
	friend class  CVideoSummaryFile;
public:
	CVideoSummaryFileObject();
	~CVideoSummaryFileObject();

	int Create(char *pSzFile);
	int Load(char *pSzFile);
	int Save();
	int Save(char *pSzFile);
	//顺序读写，类似文件
	int WritePicture(CVideoSummaryFilePicture *pPicture );
	int ReadPicture(CVideoSummaryFilePicture **pPicture, int nMoveFlag=eVSFOMC_Next);

	int Seek(int nPos);
	int GetPictureCount();
	int GetObjectId(){return m_nObjectId;};
	char *GetFile(){return m_szFile;};
protected:
	vector<CVideoSummaryFilePicture *> m_lstFrames;
	//
	int m_nObjectId;
	int m_nFrames;
	//新的 一帧是否 更新他 
	bool	m_bUpdated;
	//对应的 xml 文件
	TiXmlDocument *m_pXmlDoc;
	TiXmlElement *m_pElePos  ; //当前 node
	int			  m_nElePos;
	vector<TiXmlElement *> m_lstXmlItemPicture; //用于读取的时候定位
	//
	char m_szFile[256];
	CVideoSummaryFile *m_pFile;
	//用于读取的 缓存
	CVideoSummaryFilePicture *m_pPicture;
	//对象开始的时间 和结束的时间
	int64  m_s64BTS;
	int64  m_s64ETS;
	//对应 在 vsfile的 node 写入的时候用于更新时间
	TiXmlElement *m_pEleInVsFile;
};
/*
* 使用 主文件 + 对象 xml文件
* 主文件只是存所有的对象， 对象文件存 该对象详细的 帧内容 
*
*/
typedef enum _eVideoSummaryFileOpenMode{
	eVSFOM_Create = 1,
	eVSFOM_Read,
	eVSFOM_Write,
	eVSFOM_Trancate,
}eVideoSummaryFileOpenMode;
typedef enum _eVideoSummaryFileMoveCursor{
	eVSFMC_Next = 0,
	eVSFMC_Prev ,
	eVSFMC_NoMove,
}eVideoSummaryFileMoveCursor;
class MEDIASDK_API CVideoSummaryFile
{
public:
	CVideoSummaryFile(void);
	virtual ~CVideoSummaryFile(void);
	
	int Open(char *pSzFile, int nOpenMode = eVSFOM_Write);
	int Save();
	int Close();

	int WriteVideoInfo(int nW, int nH, int nTimeBaseNum, int nTimeBaseDen, int64 s64StartTimestamp=0);

	int WriteBackground(CVideoSummaryFilePicture *pPicture);
	//对于 写入的时候 调用
	int BeginFrame();
	int WritePicture(CVideoSummaryFilePicture *pPicture);
	int EndFrame( );
	//读取文件 需要保存两个数据， 一个是 xml的node 一个是 读取的数据 对象 的结构体
	//使用 指针 是为了防止  vector 的内存 在库里面分配，而在库外面释放，会带来问题
	int ReadFrame(vector<CVideoSummaryFilePicture*> **lstPicture, int nMoveCursor);
	//设置读取密度, 1-20 
	int SetDensity(int nDensity);
	char *GetFilePath(){return m_szPath;};
	//点击选取 从当前帧总，给定的点 选取 图片，
	int GetFramePicture(vector<CVideoSummaryFilePicture*> **lstPicture, int nX, int nY);
	//获取当前帧图片
	int GetFramePicture(vector<CVideoSummaryFilePicture*> **lstPicture);
	//根据 时间 获取图片 
	int GetPicture(vector<CVideoSummaryFilePicture*> **lstPicture, int64 s64TimeB, int64 s64TimeE);
	int GetPicture(vector<CVideoSummaryFilePicture*> **lstPicture, int &nObjectPos, int &nPicturePos , int nCount);
	//
	void GetBkgSize(int &nW, int  &nH){ nW = m_nW; nH = m_nH; };


	int GetPictureCount();
	//根据 picture 的 绝对位置， 定位。
	int Seek(int nAbsPos);
	int Pos();
	//读取一个图片，位置自动向后移动
	int ReadPicture(CVideoSummaryFilePicture **pPicture);
	//按照对象读取
	int GetObjectCount();
	//返回object id
	int GetObjectPictureCount(int nObjectIndex);
	//根据 object 序号 读取 picture, 效率很低。适用于 获取第一个快照 
	int ReadObjectPicture(int nObjectIndex, int nPicIndex, CVideoSummaryFilePicture *pPicture);

	CVideoSummaryFileObject *GetVsObject(int nObjectIndex);
	void FreeVsObject(CVideoSummaryFileObject *pObject);

	void FreeObjectPicture(vector<CVideoSummaryFilePicture*> *lstPicture);
	//

	void GetTimeBase(int &nNum, int &nDen);
	int64 GetCurrTime();
	int64 GetTotalTime();
	int64 GetStartTimestamp();
	char *GetPath(){return m_szPath;};
protected:
	//
	TiXmlElement *GetChildElement(TiXmlElement *pXmlEle, int nPos);
	//文档对象
	TiXmlDocument  *m_pXmlDoc;

	//TiXmlElement   *m_pEleRoot;
	//工程文件完整名字
	char m_szFile[256]; //工程文件
	//工程文件所在路径
	char m_szPath[256];
	//当前 背景图片名字
	char m_szCurBkgFile[64];
	//视频背景的 宽度 高度
	int m_nW;
	int m_nH;
	//所有的 对象 [写入]
	vector<CVideoSummaryFileObject *> m_lstObject;
	//保存 一帧的对象  ， 用于 下一次 的对比使用 
	list<CVideoSummaryFileObject *> m_lstLastFrameObject;
	//list<CVideoSummaryObject *> m_lstCurrFrameObject;
	map<int, CVideoSummaryFileObject *> m_mapId2Object;
	//for read 
	//当前的 nodes， 对于读取， 可能同时有 多个 node 写入只有一个 
	list<TiXmlElement *> m_lstCurEle;
	TiXmlElement      *m_pEleCursor;
	CVideoSummaryFileObject *m_pSummaryObject;
	int				   m_nElePos;
	int				   m_nAbsPos;//pic的绝对位置
	int					m_nTotalPicture;
	vector<TiXmlElement *> m_lstXmlItemObject; //所有的 xml节点，用于直接 下表定位
	int m_nDensity; //读取密度
	//用于读取，一帧的 图片
	vector<CVideoSummaryFilePicture*> m_lstFramePicture;
	//选取 图片的 缓存，
	//
	vector<CVideoSummaryFilePicture*> m_lstPictureGetTmp;
	//当前时间戳
	int64 m_s64TimeStamp;
	//当前 items 里面的结束时间。用于 判断 是否还有 时间上交叠的 object
	int64 m_s64CurItemsEndTimeStamp;
	//
	int64 m_s64StartTimestamp;
	int64 m_s64EndTimestamp;
	//
	//读写锁
	CMutex  *m_pMutextReadWrite;
};
