#pragma once
//管理器
//
#include <vector>

using namespace std;

class CMediaFilter;
class CSourceFilter;
class MEDIASDK_API CFilterManager
{
public:
	CFilterManager(void);
	virtual ~CFilterManager(void);

	static CFilterManager *GetManager();
	//系统不负责释放，最好使用 全局变量用于注册
	static int RegistFilter(CMediaFilter* pFilter);
	//以下是配对使用的 函数
	static int CreateFilter(char *pName, CMediaFilter **pFilter);
	static void FreeFilter(CMediaFilter *pFilter);
	//链接两个filter, 将第一个的输出和第二个的输入链接起来
	static int Connect(CMediaFilter *pFilterFirst, int nPinOutIndex, CMediaFilter *pFilterSecond, int nPinInIndex);
	static int Disconnect(CMediaFilter *pFilterFirst, CMediaFilter *pFilterSecond);
	static int GetSourceFilter(vector<CSourceFilter*> &lstSourceFilter);
protected:
	void RegistFilterImp(CMediaFilter* pFilter);
	int  CreateFilterImp(char *pName, CMediaFilter **pFilter);

protected:
	vector<CMediaFilter *> m_lstFilter;
};
//extern VIVSMEDIABOX_API CFilterManager *g_pFilterManager;

