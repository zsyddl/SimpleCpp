
/*
* 大括号 中括号 小括号
*/

%{
#include <stdio.h>

//#define YYSTYPE CTag*
#include "tag.h"
#include "FileTags.h"


//#include "TestParserY.h"

    #define  __STDC__   0 
    void yyerror(char*);
    int yylex(void);
%}
//修改头文件名称
%defines "ScpIfParserY.h"

// %union {
// 	CTag* m_pTag;
// }

//{}
%token  TK_BRANCES_LEFT  TK_BRANCES_RIGHT  
//[ ]
%token  TK_BRACKET_LEFT TK_BRACKET_RIGHT
//()小括号
%token  TK_PARENTHESES_LEFT TK_PARENTHESES_RIGHT
//<>angle brackets;
%token  TK_ANGLE_BRACKET_LEFT TK_ANGLE_BRACKET_RIGHT
//; ,  : "
%token	TK_SEMICOLON TK_COMMA  TK_COLON  TK_QUOTATION
//# pound sign
%token	TK_POUND_SIGN
//~
%token  TK_INVERSE
//star *
%token   TK_STAR
//=
%token   TK_EQU
//&
%token	 TK_AND
// "//"
%token  TK_COMMENT_LINE
// "/**/"
%token  TK_COMMENT_BLOCK
%token  NUM
//inlude pragma once
%token  TK_KW_INCLUDE TK_PRAGMA TK_ONCE 
//using namespace std;
%token	TK_USING_NAME_SPACE
//define
%token  TK_KW_DEFINE 
//#ifdef #ifndef #endif 
%token  TK_KW_IFDEF TK_KW_IFNDEF TK_KW_ENDIF
//#else 
%token  TK_KW_ELSE

//class 关键字 tk->token, kw key word
%token  TK_KW_CLASS  TK_KW_STRUCT TK_KW_ENUM  TK_KW_UNION
%token  TK_KW_CHAR TK_KW_UCHAR TK_KW_SHORT TK_KW_USHORT TK_KW_INT TK_KW_UINT TK_KW_LONG TK_KW_ULONG TK_KW_LONGLONG TK_KW_ULONGLONG TK_KW_FLOAT TK_KW_DOUBLE
//重新定义的变量类型
%token  TK_KW_INT8 TK_KW_UINT8 TK_KW_INT16 TK_KW_UINT16 TK_KW_INT32 TK_KW_UINT32 TK_KW_INT64 TK_KW_UINT64
//访问权限 public protected private
%token 	TK_KW_PUBLIC TK_KW_PROTECTED TK_KW_PRIVATE
//virtual void  typedef
%token  TK_KW_VIRTUAL TK_KW_VOID TK_KW_TYPEDEF
//定义类型 变量类型
%token  TK_TYPE

%token  WORD
%token	TK_LINE_OTHER
//接口函数标志接口函数 和接口槽（回调函数）
%token  TK_KW_IF_FUN TK_KW_IF_SLOT TK_KW_OUT_P TK_KW_ONE_WAY 

%%
/*
program: 
        | program expr_function // 用这种递归的方式从有限的标记表达出无限的内容
        ;
expr_function:
    TK_BRANCES_LEFT WORD TK_BRANCES_RIGHT {printf("a function\n");  }
    | WORD    {printf("a word\n"); }
    | TK_COMMENT_BLOCK {printf("comment block\n");}
    | include
    | expr_program 
    ;
include:
	TK_POUND_SIGN "include" "\"" WORD "\"" {printf("include 0\n");}
	|TK_POUND_SIGN "include" TK_ANGLE_BRACKET_LEFT WORD TK_ANGLE_BRACKET_LEFT {printf("include 1\n");}
	;
*/
Program:
	Tags       { ; }
	;
Tags:
	Tag			{ CFileTags::m_pFileTags->AppendTopTag($1);; }
	| Tags Tag	{ CFileTags::m_pFileTags->AppendTopTag($2);; }
	;	
Tag:
	Pragma   //#progma once { }
	| Include			{ }
	| TK_USING_NAME_SPACE {
							$1->m_nType = eTagType_UsingNamespace;
							$$ = $1;
							printf("tag using namespace\n");
						}
// 	| TK_DEFINE    {printf("tag define\n");
// 		$1->m_nType = eTagType_Define;
// 		$$ = $1;
// 	}
// 	| TK_KW_IFNDEF    {printf("tag #ifndef\n");
// 		$1->m_nType = eTagType_Ifndef;
// 		$$ = $1;
// 	}
	| TK_KW_ENDIF    {
		$1->m_nType = eTagType_Endif;
		$$ = $1;
		printf("tag #endif\n");
	}
	| Comment        {printf("tag comment\n");}    
	| Class		{printf("tag class\n");}
	| Enum		{printf("tag enum \n");}
	| Define	{printf("tag define\n");};
	| Ifndef	{printf("tag ifndef\n");};
	| Ifdef		{printf("tag ifdef\n");};
	| Ifelse	{printf("tag ifelse\n");};
	;
Pragma:
	TK_POUND_SIGN TK_PRAGMA TK_ONCE  {
		//
		$2->m_lstProperty.push_back($1);
		$2->m_lstProperty.push_back($3);
		$$ = $2;
		
		printf("#pragram once: property %d\n", $$->m_lstProperty.size()); 
	}
	;
 
Include:
	TK_POUND_SIGN TK_KW_INCLUDE TK_ANGLE_BRACKET_LEFT WORD TK_ANGLE_BRACKET_RIGHT	{
		//
		CTag* pTag = new CTagInclude();
		pTag->m_nType = eTagType_Include;
		pTag->m_lstProperty.push_back($1);
		pTag->m_lstProperty.push_back($2);
		pTag->m_lstProperty.push_back($3);
		pTag->m_lstProperty.push_back($4);
		pTag->m_lstProperty.push_back($5);
		
		$$ = pTag;
		printf("include 1\n");
	}
	| TK_POUND_SIGN TK_KW_INCLUDE TK_QUOTATION WORD TK_QUOTATION	{
		//
		CTag* pTag = new CTagInclude();
		pTag->m_nType = eTagType_Include;
		pTag->m_lstProperty.push_back($1);
		pTag->m_lstProperty.push_back($2);
		pTag->m_lstProperty.push_back($3);
		pTag->m_lstProperty.push_back($4);
		pTag->m_lstProperty.push_back($5);

		$$ = pTag;
		printf("include 2\n");
	}
	;
// class A {members};
Class:
	TK_KW_CLASS  WORD BaseClasses TK_BRANCES_LEFT Members TK_BRANCES_RIGHT TK_SEMICOLON {
		CTag* pTag = new CTagClass();
		pTag->m_nType = eTagType_Class;
		pTag->m_strName = $2->m_strName;
		//pTag->m_strBody = $1->m_strName + " "+ $2->m_strName + " \r\n" + $3->m_strName + "\r\n" + "\t";
		pTag->m_strBody = pTag->m_strName;//
		pTag->m_lstProperty.push_back($1);
		//class TypeName
		$2->m_nType = eTagType_TypeName;
		pTag->m_lstProperty.push_back($2);
		//继承基类
		pTag->m_lstProperty.push_back($3);
		//{
		pTag->m_lstProperty.push_back($4);
		//members list
		$5->m_pParent = pTag;
		pTag->m_lstProperty.push_back($5); 
		pTag->m_bHasInterface = $5->m_bHasInterface;
		//}
		pTag->m_lstProperty.push_back($6);
		pTag->m_lstProperty.push_back($7);
		
		
		$$ = pTag;
		
		printf("class \n");
	}
	// class SCP_API CScp .....带 dll 导出 修饰的
	| TK_KW_CLASS WORD WORD BaseClasses TK_BRANCES_LEFT Members TK_BRANCES_RIGHT TK_SEMICOLON {
		CTag* pTag = new CTagClass();
		pTag->m_nType = eTagType_Class;
		pTag->m_strName = $3->m_strName;
		//pTag->m_strBody = $1->m_strName + " "+ $2->m_strName + " \r\n" + $3->m_strName + "\r\n" + "\t";
		pTag->m_strBody = pTag->m_strName;//
		pTag->m_lstProperty.push_back($1);
		pTag->m_lstProperty.push_back($2);
		//class TypeName
		$3->m_nType = eTagType_TypeName;
		pTag->m_lstProperty.push_back($3);
		//继承基类
		pTag->m_lstProperty.push_back($4);
		//{
		pTag->m_lstProperty.push_back($5);
		//members list
		$6->m_pParent = pTag;
		pTag->m_lstProperty.push_back($6); 
		pTag->m_bHasInterface = $6->m_bHasInterface;
		//}
		pTag->m_lstProperty.push_back($7);
		pTag->m_lstProperty.push_back($8);


		$$ = pTag;

		printf("class \n");
	}
	;
BaseClasses:
		{
			CTag* pTag = new CTagBaseClasses();
			pTag->m_nType = eTagType_BaseClasses;

			$$ = pTag;
	}
		
	| TK_COLON BaseClass{ //: pubic BaseClass
			CTag* pTag = new CTagBaseClasses();
			pTag->m_nType = eTagType_BaseClasses;
			
			pTag->m_lstChild.push_back($2);

			$$ = pTag;
		;
	}
	| BaseClasses TK_COMMA BaseClass {
		CTag* pTag = $1;
		pTag->m_nType = eTagType_BaseClasses;

		pTag->m_lstChild.push_back($2);

		$$ = pTag;
		;
	}
	;
BaseClass:
	WORD {// CA
		CTag* pTag = new CTag;
		pTag->m_nType = eTagType_BaseClass;
		$1->m_nType = eTagType_TypeName;
		pTag->m_lstProperty.push_back($1);
		$$=pTag;
		;
	}
	|Access WORD {// public CA
		CTag* pTag = new CTag;
		pTag->m_nType = eTagType_BaseClass;

		pTag->m_lstProperty.push_back($1);
		$2->m_nType = eTagType_TypeName;
		pTag->m_lstProperty.push_back($2);

		$$=pTag;

		printf("base classes: access name\n");
		//public CBase
		;}
	;
//类成员
Members: 
			{
			CTag *pTag = new CTag();
			pTag->m_nType = eTagType_Members;   //参数	
			
			$$ = pTag;
	}
	| Access TK_COLON			{ //public :
							CTag *pTag = new CTag();
							pTag->m_nType = eTagType_Members;   //参数	
							$1->m_pParent = pTag;
							pTag->m_lstChild.push_back($1);
							delete $2;

							$$ = pTag;
						}
	| Member			{
							CTag *pTag = new CTag();
							pTag->m_nType = eTagType_Members;   //参数	
							$1->m_pParent = pTag;
							pTag->m_lstChild.push_back($1);
							
							pTag->m_bHasInterface = $1->m_bHasInterface;

							$$ = pTag;
						}
	| Comment			{
							CTag *pTag = new CTag();
							pTag->m_nType = eTagType_Members;   //参数	
							$1->m_pParent = pTag;
							pTag->m_lstChild.push_back($1);
							$$ = pTag;
						}
	| Members Member	{
							 
							$2->m_pParent = $1;
							$1->m_lstChild.push_back($2);

							$1->m_bHasInterface = $2->m_bHasInterface;

							$$ = $1;
						}
	| Members Access TK_COLON {
							$2->m_pParent = $1;
							$1->m_lstChild.push_back($2);
							delete $3;
							$$ = $1;
						}
					
	| Members Comment  {
							$2->m_pParent = $1;
							$1->m_lstChild.push_back($2);
							$$ = $1;
						}
						
	;	
//类成员	
Member:
	MemberVariant		{
							CTag* pTag = $1;

							$$ = pTag;
							printf("member var\n");
						}	
	| MemberFunction	{
							CTag* pTag = $1;

							$$ = pTag;
							printf("member function\n");
						}
	;	
MemberVariant:
	VarType VarName TK_SEMICOLON { // int nCount;
							CTag *pTag = new CTag();
							pTag->m_nType = eTagType_Variant;   //变量
							
							pTag->m_strName = $2->m_strName;
							pTag->m_lstProperty.push_back($1);
							pTag->m_lstProperty.push_back($2);
							pTag->m_lstProperty.push_back($3);
							
							$$ = pTag;			
							printf("member var0\n");
						}	//int m_nA;
									
	| WORD WORD TK_SEMICOLON  {

		CTag *pTag = new CTag();
		pTag->m_nType = eTagType_Variant;   //变量

		pTag->m_strName = $2->m_strName;

		CTagType* pTagType = new CTagType;
		pTagType->m_strName = $1->m_strName;
		pTagType->m_strBody = $1->m_strBody;
		pTag->m_lstProperty.push_back(pTagType);
		delete $1;

		$2->m_nType = eTagType_VarName;
		pTag->m_lstProperty.push_back($2);
		pTag->m_lstProperty.push_back($3);

		$$ = pTag;	
		printf("member var1\n");

	}	//i 不添加这个 上一句就失效。有冲突，加了 这一句也不会有 东西
	; 
VarType:
	WORD			{
						CTag* pTag = new CTagType;
						pTag->m_nType = eTagType_VarType;
						pTag->m_strName = $1->m_strName;
						pTag->m_strBody = $1->m_strName;
						
						pTag->m_lstProperty.push_back($1);
						$1->m_nType = eTagType_VarType;
						
						$$ = pTag;
						
						printf("normal type \n");
					}	//int m_nA;//普通类型
	| TK_KW_VOID     {
						CTag* pTag = new CTagType;
						pTag->m_nType = eTagType_VarType;
						pTag->m_strName = $1->m_strName;
						
						pTag->m_lstProperty.push_back($1);
						$1->m_nType = eTagType_VarType;
						
						$$ = pTag;
						
						printf("void type \n");
					}	//int m_nA;//普通类型
	| VarType TK_STAR { //int m_nA;//指针类型  这会先匹配 普通类型 然后匹配指针
						CTag* pTag = $1;
						 
						pTag->m_lstProperty.push_back($2);
						pTag->m_strName = $1->m_strName + $2->m_strName;
						pTag->m_strBody = pTag->m_strName;
						$$ = pTag;
						printf("pointer type\n");
						}	
	| VarType TK_AND { //int& m_nA;//引用  这会先匹配普通类型 然后匹配引用
						CTag* pTag = $1;
						 
						pTag->m_lstProperty.push_back($2);
						pTag->m_strName = $1->m_strName + $2->m_strName;
						pTag->m_strBody = pTag->m_strName;
						
						$$ = pTag;
						printf("pointer type\n");
					   }
											
	| VarType TK_ANGLE_BRACKET_LEFT VarType TK_ANGLE_BRACKET_RIGHT	{
						CTag* pTag = $1;
						$1->m_nType = eTagType_VarType;
						//<
						$1->m_lstProperty.push_back($2);
						// vartype
						$1->m_lstProperty.push_back($3);
						//>
						$1->m_lstProperty.push_back($4);
						
						$1->m_strName = $1->m_strName +  $2->m_strName + $3->m_strName + $4->m_strName ;
						pTag->m_strBody = pTag->m_strName;

						$$ = $1;
						printf("template type\n");
					}	//int m_nA;//模板类型
	;	
VarName:
	WORD			{//普通名字
						$1->m_nType = eTagType_VarName;
						$1->m_strName = $1->m_strBody;
						$$ = $1;
					}
	| WORD TK_BRACKET_LEFT WORD TK_BRACKET_RIGHT  
					{//数组
						$1->m_nType = eTagType_VarName;
						$1->m_strBody = $1->m_strBody + $2->m_strBody + $3->m_strBody + $4->m_strBody;

						$1->m_lstProperty.push_back($2);
						$1->m_lstProperty.push_back($3);
						$1->m_lstProperty.push_back($4);
						
						$$ = $1;
					} 
	;	
MemberFunction:
	WORD TK_PARENTHESES_LEFT Params TK_PARENTHESES_RIGHT TK_SEMICOLON {
						//构造函数  CAClass();
						CTag *pTag = new CTag();
						pTag->m_nType = eTagType_Constructor;  	
												
						pTag->m_strName = $1->m_strBody;
						//pTag->m_strBody = $1->m_strBody +  " " + $2->m_strBody + " " +  $3->m_strBody + " " +  $4->m_strBody;
						pTag->m_strBody = pTag->m_strName;
						
						$1->m_nType = eTagType_FunctionName;
						pTag->m_lstProperty.push_back($1);
						
						$2->m_nType = eTagType_TokenParenthesesLeft;
						pTag->m_lstProperty.push_back($2);
						
						$3->m_nType = eTagType_Params;
						pTag->m_lstProperty.push_back($3);
						
						$4->m_nType = eTagType_TokenParenthesesRight;
						pTag->m_lstProperty.push_back($4);
						
						$$ = pTag;
						printf("construct fun /no return function \n");
					} 
	| TK_INVERSE WORD TK_PARENTHESES_LEFT Params TK_PARENTHESES_RIGHT TK_SEMICOLON {
						//析构函数 ~fun(void);
						
						CTag *pTag = new CTag();
						pTag->m_nType = eTagType_Destructor;   //参数	
						
						pTag->m_strName = $1->m_strBody + $2->m_strBody;
						//pTag->m_strBody = $1->m_strBody +  " " + $2->m_strBody + " " +  $3->m_strBody +  " " + $4->m_strBody+  " " + $5->m_strBody;
						pTag->m_strBody = pTag->m_strName;
						
						$1->m_nType = eTagType_FunctionName;
						$1->m_strName =  $1->m_strBody + $2->m_strBody;
						$1->m_strBody = $1->m_strName;
						pTag->m_lstProperty.push_back($1);
						delete $2;
						//pTag->m_lstProperty.push_back($2);
						//(
						$3->m_nType = eTagType_TokenParenthesesLeft;
						pTag->m_lstProperty.push_back($3);
						//
						$4->m_nType = eTagType_Params;
						pTag->m_lstProperty.push_back($4);
						//)
						$5->m_nType = eTagType_TokenParenthesesRight;
						pTag->m_lstProperty.push_back($5);
						
						$$ = pTag;
						printf("distruct fun\n");
					} 
	| TK_KW_VIRTUAL TK_INVERSE WORD TK_PARENTHESES_LEFT Params TK_PARENTHESES_RIGHT TK_SEMICOLON {
						//析构函数 virtual ~fun(void);
						CTag *pTag = new CTag();
						pTag->m_nType = eTagType_Destructor;   //参数	
						
						pTag->m_strName = $2->m_strBody + $3->m_strBody;
						//pTag->m_strBody = $1->m_strBody +  " " + $2->m_strBody + " " +  $3->m_strBody + " " +  $4->m_strBody + " " +  $5->m_strBody +  " " + $6->m_strBody;;
						pTag->m_strBody = pTag->m_strName;
						
						$1->m_nType = eTagType_Virtual;
						pTag->m_lstProperty.push_back($1);
						//~Fun
						$2->m_nType = eTagType_FunctionName;
						$2->m_strName = $2->m_strName + $3->m_strName;
						$2->m_strBody = $2->m_strName ;;
						pTag->m_lstProperty.push_back($2);
						delete $3;
						//pTag->m_lstProperty.push_back($3);
						//(
						$4->m_nType = eTagType_TokenParenthesesLeft;
						pTag->m_lstProperty.push_back($4);
						//params
						$5->m_nType = eTagType_Params;
						pTag->m_lstProperty.push_back($5);
						//;
						$6->m_nType = eTagType_TokenParenthesesRight;
						pTag->m_lstProperty.push_back($6);
						
						$$ = pTag;
						printf("virtual distruct fun\n");
					} 
	| VarType WORD TK_PARENTHESES_LEFT Params TK_PARENTHESES_RIGHT TK_SEMICOLON {
						//   void fun(void);
						CTag *pTag = new CTag();
						pTag->m_nType = eTagType_Function;   //参数	
						
						pTag->m_strName = $2->m_strBody ;
						//pTag->m_strBody = $1->m_strBody +  " " + $2->m_strBody +  " " + $3->m_strBody +  " " + $4->m_strBody +  " " + $5->m_strBody ;
						pTag->m_strBody = pTag->m_strName;
						//return
						$1->m_nType = eTagType_Return;
						pTag->m_lstProperty.push_back($1);
						//fun name
						$2->m_nType = eTagType_FunctionName;
						pTag->m_lstProperty.push_back($2);
						//(
						$3->m_nType = eTagType_TokenParenthesesLeft;
						pTag->m_lstProperty.push_back($3);
						//params
						$4->m_nType = eTagType_Params;
						pTag->m_lstProperty.push_back($4);
						//)
						$5->m_nType = eTagType_TokenParenthesesRight;
						pTag->m_lstProperty.push_back($5);
						
						$$ = pTag;
						printf("member  fun\n");
					} 
	| TK_KW_VIRTUAL MemberFunction {
						// 虚析构函数也走这里了
						//  virtual void fun(void);
						CTag *pTag = $2;
						pTag->m_nType = $2->m_nType;   //参数	
						
						//pTag->m_strName = $2->m_strBody;
						//pTag->m_strBody = $1->m_strBody + " " + $2->m_strBody ;;

						//
						$1->m_nType = eTagType_Virtual;
						pTag->m_lstProperty.push_front($1);
						
						$$ = pTag;
						printf("virtual member  fun\n");
					} 
	| TK_KW_IF_FUN MemberFunction {
						 // IF_FUN virtual void fun(void);
						CTag *pTag = $2;
						pTag->m_nType = eTagType_IfFunction;   	
						
						pTag->m_strName = $2->m_strName;
						//pTag->m_strBody = $1->m_strBody + " " + $2->m_strBody ;
						pTag->m_strBody = pTag->m_strName;
						
						pTag->m_lstProperty.push_front($1);
						$1->m_nType = eTagType_TokenIfFun;
						
						pTag->m_bHasInterface = true;

						$$ = pTag;
						printf("If Function\n");
					}
	| TK_KW_ONE_WAY TK_KW_IF_FUN MemberFunction {
		// IF_FUN virtual void fun(void);
		CTag *pTag = $3;
		pTag->m_nType = eTagType_IfFunction;   	

		//pTag->m_strName = $2->m_strName;
		//pTag->m_strBody = $1->m_strBody + " " + $2->m_strBody ;
		pTag->m_strBody = pTag->m_strName;

		pTag->m_lstProperty.push_front($2);
		pTag->m_lstProperty.push_front($1);
		$1->m_nType = eTagType_TokenIfFun;

		pTag->m_bHasInterface = true;
		pTag->m_bIsOneWay = true;

		$$ = pTag;
		printf("If Function one way\n");
	}

	| TK_KW_IF_SLOT MemberFunction	{
						// IF_SLOT virtual void fun(void);
						CTag *pTag = $2;
						pTag->m_nType = eTagType_IfSlot;   //参数	
						
						pTag->m_strName = $2->m_strName;
						//pTag->m_strBody = $1->m_strBody + " " + $2->m_strBody ;
						pTag->m_strBody = pTag->m_strName;
					
						pTag->m_bHasInterface = true;

						pTag->m_lstProperty.push_front($1);
						$1->m_nType = eTagType_TokenIfSlot;

						$$ = pTag;
						printf(" IF Slot\n");
					} //  virtual void fun(void);
	| TK_KW_ONE_WAY TK_KW_IF_SLOT MemberFunction	{
		// IF_SLOT virtual void fun(void);
		CTag *pTag = $3;
		pTag->m_nType = eTagType_IfSlot;   //参数	

		//pTag->m_strName = $3->m_strName;
		//pTag->m_strBody = $1->m_strBody + " " + $2->m_strBody ;
		pTag->m_strBody = pTag->m_strName;

		pTag->m_bHasInterface = true;
		pTag->m_bIsOneWay = true;

		pTag->m_lstProperty.push_front($2);
		pTag->m_lstProperty.push_front($1);
		$1->m_nType = eTagType_TokenIfSlot;

		$$ = pTag;
		printf(" IF Slot one way\n");
	} //  virtual void fun(void);
	;	
Params:  //空				
							{
								CTag *pTag = new CTagParams();
								pTag->m_nType = eTagType_Params;   //参数	
								$$ = pTag;
							}
	|Param					{ 
								CTag *pTag = new CTagParams();
								pTag->m_nType = eTagType_Params;   //参数	
								pTag->m_lstChild.push_back($1);
								$$ = pTag;
								printf("params 1: \n");
							} //多个参数var1, var2 
	|Params TK_COMMA Param  {
								CTag *pTag = $1;
								pTag->m_lstChild.push_back($3);
								
								delete $2;
								
								$$ = pTag; 
								printf("params 2: \n");
							} //多个参数var1, var2 
	;	
Param:
	| TK_KW_VOID     {	
						CTag *pTag = new CTag();
						pTag->m_nType = eTagType_Param;   //参数	
						pTag->m_strName = $1->m_strName;
						
						$1->m_nType = eTagType_VarType;
						pTag->m_lstProperty.push_back($1);
						
						$$ = pTag;
					;} // void 
	|VarType VarName {
						CTag *pTag = new CTag();
						pTag->m_nType = eTagType_Param;   //参数	
						pTag->m_strName = $2->m_strName;
						
						pTag->m_lstProperty.push_back($1);
						pTag->m_lstProperty.push_back($2);
						
						$$ = pTag;	
					;} //一个参数
	|TK_KW_OUT_P VarType VarName {// OUT_P int& param;
						CTag *pTag = new CTag();
						pTag->m_nType = eTagType_Param;   //参数	
						pTag->m_strName = $3->m_strName;
						
						$1->m_nType = eTagType_OutParam;
						pTag->m_lstProperty.push_back($1);
						pTag->m_lstProperty.push_back($2);
						pTag->m_lstProperty.push_back($3);
						
						$$ = pTag;	
					;} //一个参数				
	;	
//pubic: protected: private	
Access:
	TK_KW_PUBLIC 		{
							$1->m_nType = eTagType_Access;
							$1->m_strName = "public";
							//$1->m_strBody = "public:";
							$$ = $1;
							printf("access public:\n");
						}
	| TK_KW_PROTECTED 	{
							$1->m_nType = eTagType_Access;
							$1->m_strName = "protected";
							//$1->m_strBody = "protected:";
							$$ = $1;
						}
	| TK_KW_PRIVATE		{
							$1->m_nType = eTagType_Access;
							$1->m_strName = "private";
							//$1->m_strBody = "private:";
							$$ = $1;
						}
	
	;	
	
//注释	
Comment:
	TK_COMMENT_LINE    {
							
							printf("comment line\n");
							$$ = $1;
						}
	| TK_COMMENT_BLOCK {
							printf("comment block\n");
							$$ = $1;
						}
	;	
Enum:
//typedef enum _eType{}eType;
	TK_KW_TYPEDEF TK_KW_ENUM WORD TK_BRANCES_LEFT EnumMembers TK_BRANCES_RIGHT WORD TK_SEMICOLON {
							CTag *pTag = new CTagEnum();
							pTag->m_nType = eTagType_Enum;
							pTag->m_strName = $3->m_strName;
							pTag->m_lstProperty.push_back($1);
							pTag->m_lstProperty.push_back($2);
							//name
							pTag->m_lstProperty.push_back($3);
							pTag->m_lstProperty.push_back($4);
							//enum members;
							pTag->m_lstProperty.push_back($5);
							pTag->m_lstProperty.push_back($6);
							//typedef name
							pTag->m_lstProperty.push_back($7);
							pTag->m_lstProperty.push_back($8);

							$$ = pTag;
							printf("enum\n");
						}	
	;
EnumMembers:
						{
							CTag *pTag = new CTag();
							pTag->m_nType = eTagType_EnumMembers;   //参数	
							
							$$ = pTag;
						}
	| EnumMember		{
							CTag *pTag = new CTag();
							pTag->m_nType = eTagType_EnumMembers;   //参数	
							pTag->m_lstChild.push_back($1);
							
							$$ = pTag;
						}
	| EnumMembers TK_COMMA EnumMember  {

							$1->m_lstChild.push_back($3);
							
							$$ = $1;
						;
						}	
	| EnumMembers TK_COMMA EnumMember TK_COMMA {

							$1->m_lstChild.push_back($3);
							
							$$ = $1;
						;
						}	
	;
EnumMember:
	WORD TK_EQU WORD  {
						CTag* pTag = new CTag();
						pTag->m_nType = eTagType_EnumMember;
						pTag->m_strName = $1->m_strName;

						pTag->m_lstProperty.push_back($1);
						pTag->m_lstProperty.push_back($2);
						pTag->m_lstProperty.push_back($3);

						$$ = pTag;	
					} //eType_0=0
	|WORD 			{
						CTag* pTag = new CTag();
						pTag->m_nType = eTagType_EnumMember;
						pTag->m_strName = $1->m_strName;
						pTag->m_lstProperty.push_back($1);
							
						$$ = pTag;
					}		//eType_1
	;
Define:
// 	TK_KW_DEFINE  
// 	{
// 		CTag* pTag = new CTag();
// 		pTag->m_nType = eTagType_Define;
// 		pTag->m_strName = $1->m_strName;
// 
// 		pTag->m_lstProperty.push_back($1);
// 		//pTag->m_lstProperty.push_back($2);
// 
// 		$$ = pTag;
// 	};
//	|
	TK_KW_DEFINE WORD DefineContents
	{
		CTag* pTag = new CTag();
		pTag->m_nType = eTagType_Define;
		pTag->m_strName = $1->m_strName;

		pTag->m_lstProperty.push_back($1);
		pTag->m_lstProperty.push_back($2);
		pTag->m_lstProperty.push_back($3);

		$$ = pTag;
	};
	| TK_KW_DEFINE TK_KW_IF_SLOT
	{
		CTag* pTag = new CTag();
		pTag->m_nType = eTagType_Define;
		pTag->m_strName = $1->m_strName;

		pTag->m_lstProperty.push_back($1);
		pTag->m_lstProperty.push_back($2);

		$$ = pTag;
	}; 
	| TK_KW_DEFINE TK_KW_IF_FUN
	{
		CTag* pTag = new CTag();
		pTag->m_nType = eTagType_Define;
		pTag->m_strName = $1->m_strName;

		pTag->m_lstProperty.push_back($1);
		pTag->m_lstProperty.push_back($2);

		$$ = pTag;
	}; 
	| TK_KW_DEFINE TK_KW_OUT_P
	{
		CTag* pTag = new CTag();
		pTag->m_nType = eTagType_Define;
		pTag->m_strName = $1->m_strName;

		pTag->m_lstProperty.push_back($1);
		pTag->m_lstProperty.push_back($2);

		$$ = pTag;
	}; 
	| TK_KW_DEFINE TK_KW_ONE_WAY
	{
		CTag* pTag = new CTag();
		pTag->m_nType = eTagType_Define;
		pTag->m_strName = $1->m_strName;

		pTag->m_lstProperty.push_back($1);
		pTag->m_lstProperty.push_back($2);

		$$ = pTag;
	}; 
// 	| TK_KW_DEFINE WORD  DefineContents
// 	{
// 		CTag* pTag = new CTag();
// 		pTag->m_nType = eTagType_Define;
// 		pTag->m_strName = $1->m_strName;
// 
// 		pTag->m_lstProperty.push_back($1);
// 		pTag->m_lstProperty.push_back($2);
// 		pTag->m_lstProperty.push_back($3);
// 		//debug("define xx xxx\n");
// 		$$ = pTag;
// 	}
	;	
DefineContent:
	WORD;
	|TK_BRANCES_LEFT;
	|TK_BRANCES_RIGHT;
	|TK_PARENTHESES_LEFT;
	|TK_PARENTHESES_RIGHT;
	;
DefineContents:
	{
		CTag* pTag = new CTag();
		//pTag->m_nType = eTagType_Define;
		pTag->m_nType = eTagType_DefineContents;
		pTag->m_strName = "define context";
		$$ = pTag;
	}
	|DefineContents DefineContent{
		CTag* pTag = (CTag* )$1;
		pTag->m_lstProperty.push_back($2);
		$$ = pTag;
	};

Ifndef: 
	TK_KW_IFNDEF WORD
	{
		CTag* pTag = new CTag();
		pTag->m_nType = eTagType_Ifndef;
		pTag->m_strName = $1->m_strName;

		pTag->m_lstProperty.push_back($1);
		pTag->m_lstProperty.push_back($2);

		$$ = pTag;
	}
	;
Ifdef: 
	TK_KW_IFDEF WORD
	{
		CTag* pTag = new CTag();
		pTag->m_nType = eTagType_Ifdef;
		pTag->m_strName = $1->m_strName;

		pTag->m_lstProperty.push_back($1);
		pTag->m_lstProperty.push_back($2);

		$$ = pTag;
	}
	;
Ifelse: 
	TK_KW_ELSE
	{
		CTag* pTag = new CTag();
		pTag->m_nType = eTagType_Ifelse;
		pTag->m_strName = $1->m_strName;

		pTag->m_lstProperty.push_back($1);
		
		//pTag->m_lstProperty.push_back($2);

		$$ = pTag;
	}
	;
%%
void yyerror(char* s)
{
    printf("%s: %s\n", __FUNCTION__, s);
}
 