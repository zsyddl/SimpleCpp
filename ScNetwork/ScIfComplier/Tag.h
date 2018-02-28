
#pragma once

#include <string>
#include <vector>
#include <list>

using namespace std;
#define debug printf

//接口函数
#define IF_FUN 
//接口回调函数，槽函数
#define IF_SLOT
//传出的参数
#define OUT_P
//只是对接口函数和接口槽 进行翻译。其他函数保持不变
//
class CTag;
//定义为 CTag类型 flex bison 代码才可以使用
#define YYSTYPE CTag*
//引入 flex bison内部定义的 token 枚举值 
#include "ScpIfParserY.h"
//#define YYSTYPE CTag*

typedef enum _eTagType{
	eTagType_Unknown = -1,
	eTagType_Token = 0, //仅仅是分隔符号 不构成类型
	
	eTagType_TokenBrancesLeft  = TK_BRANCES_LEFT,    //{  
	eTagType_TokenBrancesRight = TK_BRANCES_RIGHT ,   //}
	eTagType_TokenParenthesesLeft = TK_PARENTHESES_LEFT, //PARENTHESES_LEFT ()
	eTagType_TokenParenthesesRight = TK_PARENTHESES_RIGHT, //(

	eTagType_TokenComma = TK_COMMA,  //,
	eTagType_TokenQuotation = TK_QUOTATION, //"
	eTagType_TokenBracketLeft = TK_ANGLE_BRACKET_LEFT, //[
	eTagType_TokenBracketRight =  TK_ANGLE_BRACKET_RIGHT, //]
	eTagType_TokenInverse = TK_INVERSE, //~
	eTagType_TokenAnd = TK_AND,   //&
	eTagType_TokenStar = TK_STAR, //*


	eTagType_TokenInclude = TK_KW_INCLUDE, 
	eTagType_TokenIfndef = TK_KW_IFNDEF,
	eTagType_TokenEndif	= TK_KW_ENDIF,

	eTagType_TokenIfFun = TK_KW_IF_FUN,
	eTagType_TokenIfSlot= TK_KW_IF_SLOT,
	eTagType_TokenInt  = TK_KW_INT,
	eTagType_TokenInt8 = TK_KW_INT8, 
	eTagType_TokenUInt8 = TK_KW_UINT8, 
	eTagType_TokenInt16 = TK_KW_INT16, 
	eTagType_TokenUInt16 = TK_KW_UINT16, 
	eTagType_TokenInt32 = TK_KW_INT32, 
	eTagType_TokenUInt32 = TK_KW_UINT32, 
	eTagType_TokenInt64 = TK_KW_INT64, 
	eTagType_TokenUInt64 = TK_KW_UINT64, 
	eTagType_TokenFloat = TK_KW_FLOAT, 
	eTagType_TokenDouble = TK_KW_DOUBLE, 


	
	//类型
	eTagType_Type = 10000,
	eTagType_Class,
	eTagType_BaseClasses, //继承的基类表
	eTagType_BaseClass,   //基类

	eTagType_Struct,
	eTagType_Members, //成员列表字段
	eTagType_Union,
	
	eTagType_TypeName, //类型名字，比如class 类 struct等

	eTagType_Function, //函数
	eTagType_Constructor, //构造函数
	eTagType_Destructor, //析构函数

	eTagType_Virtual, //virtual 
	
	eTagType_Return,  //返回值
	eTagType_FunctionName, //函数名
	eTagType_Params,   //参数表
	eTagType_Param,   //参数
	eTagType_OutParam,   //传出参数

	eTagType_Variant,  //变量
	eTagType_Keyword,  //关键字


	eTagType_CommentLine,
	eTagType_CommentBlock,
	
	eTagType_VarType,  //变量类型
	eTagType_VarTypePointer,	
	
	eTagType_VarName,  //变量名字
	
	eTagType_Enum,
	eTagType_EnumMembers, //成员表
	eTagType_EnumMember,

	eTagType_Define,
	eTagType_DefineContents,
	eTagType_Ifdef,
	eTagType_Ifndef,
	eTagType_Ifelse,
	eTagType_Endif,
	eTagType_Include,	//include #include "" #include<> 
	eTagType_Typedef, //typedef ..
	eTagType_Pragma, //#pragma
	eTagType_Access, //public, protected private,

	eTagType_UsingNamespace,

	eTagType_Interface = 20000,
	eTagType_IfFunction, //接口函数
	eTagType_IfSlot,  //接口槽函数
	eTagType_InterfaceEnd,
}eTagType;
class CTag
{
public:
	CTag(void);
	virtual ~CTag(void);

	//
	void AppendProperty(CTag* pTag);;
	void AppendChild(CTag* pTag);;
	//void Login(char* pUser, char* pPWD);
	//
	virtual void	CopyChild(CTag* pOther);
	virtual void	CopyFrom(CTag* pOther);
	//
	CTag*	GetProperty(eTagType type);
	CTag*	GetProperty(int nPos);

	//
	//bool	IsRef();
	//

	virtual string  ToString();
	//转换到属性的xx位置结束
	virtual string  ToString(eTagType typeEnd);
	virtual string  ToString(eTagType typeStart, eTagType typeEnd);
	//
	//	
	//外部使用的
	int		m_nType;

	string  m_strName;
	string  m_strType;
	string  m_strBody;
	void*	m_pUserData;
	// class struct enum union 使用
	//比如参数集合，class members, struct members, enum members, union members。
	list<CTag*>	m_lstChild;
	//属性比如各种修饰词语, 变量的 类型 名称等都是属性
	list<CTag*>	m_lstProperty;
	//
	CTag*		m_pParent;
	//flex bison 内部使用的类型
	int			m_nTokenType;
	//后缀，通常是结尾之后的 分割字符，比如空格 ; \r\n等 不计入实际内容，只是tostring的时候需要加上
	string		m_strSuffix;
	string		m_strPrefix;
	//是否含有接口。针对 class
	bool		m_bHasInterface;
	bool		m_bIsOneWay;
};
class CTagEnum: public CTag
{
public:
	CTagEnum();
	virtual ~CTagEnum();

	virtual string  ToString();
}; 
class CTagBaseClasses: public CTag
{
public:
	CTagBaseClasses();
	virtual ~CTagBaseClasses();
	virtual string  ToString();
};
class CTagParams: public CTag
{
public:
	CTagParams();
	virtual ~CTagParams();

	virtual string ToString();
};
class CTagInclude: public CTag{
public:
	CTagInclude();
	virtual ~CTagInclude();

	virtual string ToString();
};
//类型 比如 int int* CTag 等 代表是类型
class CTagType: public CTag{
public:
	CTagType();
	~CTagType();

	bool IsRef();
	bool IsPointer();
	//是否是原子类型，比如 int 等基本类型， 还是 class struct类型 ?
	bool IsAtom();
	bool IsType(string &strName);
	//
	string GetBaseType();
	//
};
//仅仅 是 header to string
class CTagClass: public CTag{
public:
	CTagClass();
	~CTagClass();

	virtual string ToString();
};