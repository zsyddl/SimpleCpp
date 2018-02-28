// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "FileTags.h"
#include "Trans2Source.h"
#include "CommandLine.h"
//
//            
// extern "C"{
 int yyparse (void);
 int yylex();
 void yyrestart ( FILE *input_file  );;
 extern FILE *yyin;
 //}  
  
 
 //#define YYSTYPE Include  ;
int _tmain(int argc, _TCHAR* argv[])
{ 
	//解析输入参数 -i 输入文件 -c 客户端文件名称 -s 服务器文件名称
	//生成的类名称 为原有的名称 + C 和原有名称 +S
	//-i ModuleIf.if.h -cf modulec -sf modules -cs C -ss S
	//-i 不可缺少。其他可以缺少
	char** pArgv = (char**)argv;
	CCommandLine commandLine;
	commandLine.Parse(argc, pArgv);
	//
	string strIfFileFullName = commandLine.GetParam("i");
	if (strIfFileFullName.empty())
	{
		return -1;
	}
	//if 文件名称 不带后缀
	int nExtPos = strIfFileFullName.rfind(".if.h");
	int nFileNameStart = strIfFileFullName.rfind("/");
	if (nFileNameStart == strIfFileFullName.npos)
	{
		nFileNameStart = 0;
	}
	string strIfFileName = strIfFileFullName.substr(nFileNameStart+1, nExtPos-nFileNameStart-1);
	//路径
	string strIfFilePath;
	if (nFileNameStart > 0)
	{
		strIfFilePath = strIfFileFullName.substr(0, nFileNameStart) + "/";
	}
	//
	char *pModuleIfFileName = (char*)strIfFileName.c_str(); //"ModuleIF";

	string strClientFile = commandLine.GetParam("cf");
	if (strClientFile.empty())
	{
		strClientFile = strIfFileName + "C";
	}
	char *pClientFile = (char*)strClientFile.c_str();


	string strServerFile = commandLine.GetParam("sf");
	if (strServerFile.empty())
	{
		strServerFile = strIfFileName + "S";
	}

	char *pServerFile = (char*)strServerFile.c_str();
	int nRet = 0;
//	yyin = fopen(pModuleIfFileName, "r");
// 	do
// 	{
// 		nRet = yylex();
// 	}while(nRet > 0);
// 	fclose(yyin);
	yyin = fopen(strIfFileFullName.c_str(), "r");
	if (!yyin)
	{
		printf("IfComplier: open if file false!!!\n");
		return -1;
	}
	printf("IfComplier: %s\n", strIfFileFullName.c_str());
	yyrestart(yyin);
	yyparse();
	list<CTag*>::iterator item;
	list<CTag*>& lstTopTags = CFileTags::m_pFileTags->GetTopTags();
	for (item = lstTopTags.begin();
		item != lstTopTags.end();
		item++)
	{
		CTag* pTag = *item;
	}
 	CTrans2Source *pTrans = new CTrans2Source();
	pTrans->SetFileTags(CFileTags::m_pFileTags);
	string strFile = pClientFile;
	pTrans->SetClientFileName(strFile);
	strFile = pServerFile;
	pTrans->SetServerFileName(strFile);
	//这个是包含文件用
	strFile = pModuleIfFileName;
	pTrans->SetModuleIfFileName(strFile);
	//
	string strClientSuffix;
	strClientSuffix = commandLine.GetParam("cs");
	if (strClientSuffix.empty())
	{
		strClientSuffix = "C";
	}

	pTrans->SetClientSuffix(strClientSuffix);

	string strServerSuffix;
	strServerSuffix = commandLine.GetParam("ss");
	if (strServerSuffix.empty())
	{
		strServerSuffix = "S";
	}
	pTrans->SetServerSuffix(strServerSuffix);
	
	pTrans->SetIfFilePath(strIfFilePath);
	string strOutPath;
	strOutPath = commandLine.GetParam("o");
	if (strOutPath.empty())
	{//填充为 if文件的路径
		strOutPath = strIfFilePath;
	}
	pTrans->SetOutPath(strOutPath);
	//

	 
	nRet = pTrans->Open();
	if (nRet < 0)
	{
		printf("IfComplier: open out files false\n");
	}
	else
	{
		pTrans->Trans();
	}
	
	delete pTrans;
	pTrans = NULL;
	printf("IfComplier: %s, end\n", strIfFileFullName.c_str());
	getchar();
	return 0;
}
// 
