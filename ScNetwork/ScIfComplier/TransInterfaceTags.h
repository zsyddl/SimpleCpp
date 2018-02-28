#pragma once

#include <string>
#include <stdio.h>
using namespace std;
//翻译一个 接口 tag 到 指定文件
class CFileTags;
class CTag;
class CTransInterfaceTags
{
public:
	CTransInterfaceTags(void);
	virtual ~CTransInterfaceTags(void);
	
// 	std::string GetLocalName() const { return m_strLocalName; }
// 	void SetLocalName(std::string val) { m_strLocalName = val; }
// 
// 	std::string GetRemoteName() const { return m_strRemoteName; }
// 	void SetRemoteName(std::string val) { m_strRemoteName = val; }

	FILE* GetFileH() const { return m_pFileH; }
	void SetFileH(FILE* val) { m_pFileH = val; }

	FILE* GetFileCPP() const { return m_pFileCPP; }
	void SetFileCPP(FILE* val) { m_pFileCPP = val; }

	std::string GetHeaderFile() const { return m_strFileName; }
	void SetFileName(std::string val) { m_strFileName = val; }

	std::string GetClientSuffix() const { return m_strClientSuffix; }
	void SetClientSuffix(std::string val) { m_strClientSuffix = val; }
	std::string GetServerSuffix() const { return m_strServerSuffix; }
	void SetServerSuffix(std::string val) { m_strServerSuffix = val; }


	virtual int Trans(CFileTags* pFileTags);

protected:
	int		TransTag2Source(CFileTags* pFileTags, FILE* pFileH, FILE* pFileCPP, string& strHeaderFile);

	void	TransIfClass(CTag* pClass, FILE* pFileH, FILE* pFileCPP);
	//转换 class 主要是对class 序列化
	void	TransClass(CTag* pClass, FILE* pFileH, FILE* pFileCPP);

	void BuildFunctionWrite(CTag* pFunction);

	int		TransConstructor2Client(CTag* pTagClass, FILE* pFileH, FILE* pFileCPP);
	int		TransDestructor2Client(CTag* pTagClass, FILE* pFileH, FILE* pFileCPP);
	int		TransTag2Header(CTag* pTag, FILE* pFileH);
	void	TransIfFunction(CTag* pTagClassFunction, FILE * pFileH, FILE * pFileCPP);
	void	TransIfSlot(CTag* pTagClassMember, FILE * pFileH, FILE * pFileCPP);
	int		TransParams(CTag* pTagParams, string &strParams);
	void	TransFunction(CTag* pTagClassFunction, FILE * pFileH, FILE * pFileCPP);
protected:
// 	string	m_strLocalName;
// 	string  m_strRemoteName;
	//
	string	m_strClientSuffix;
	string	m_strServerSuffix;
	


	FILE*	m_pFileH;
	FILE*	m_pFileCPP;

	string	m_strFileName;

	CFileTags* m_pFileTag;
};
