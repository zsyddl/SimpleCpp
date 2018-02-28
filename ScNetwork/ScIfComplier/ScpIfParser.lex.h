%{
#include <stdio.h> 

//#define YYSTYPE CTag*

#include "tag.h"
#include "FileTags.h"
//#include "TestParserY.h"


int nWordsCount=0;
#define debug printf
//bracesLeft { 
%}

int [+-]*[0-9]+ 

%%
"{"		{
		 
			CTag* pTag = new CTag(); 
			pTag->m_strBody = yytext;
			pTag->m_strName = pTag->m_strBody;
			pTag->m_nType = eTagType_TokenBrancesLeft;
			CFileTags::m_pFileTags->AppendToken(pTag);
			
			debug("\"%s\": %d\n", yytext, pTag->m_nType); 
			
			//yylval->m_pTag = pTag;
			yylval = pTag;
			
			return TK_BRANCES_LEFT/*pTag->m_nType*/; 
		}
"}"		{
 
			CTag* pTag = new CTag(); 
			pTag->m_strBody = yytext;
			pTag->m_strName = pTag->m_strBody;
			pTag->m_nType = eTagType_TokenBrancesRight;
			CFileTags::m_pFileTags->AppendToken(pTag);
			
			debug("\"%s\": %d\n", yytext, pTag->m_nType); 
			yylval = pTag;
			
			return TK_BRANCES_RIGHT/*pTag->m_nType*/; 
		}
"["		{
			debug("\"%s\"\n", yytext); 
			CTag* pTag = new CTag(); 
			pTag->m_strBody = yytext;
			pTag->m_strName = pTag->m_strBody;
			pTag->m_nType = TK_BRACKET_LEFT;
			CFileTags::m_pFileTags->AppendToken(pTag);
			
			yylval = pTag;
			
			return pTag->m_nType; 
		} 
"]"		{
			debug("\"%s\"\n", yytext); 
			CTag* pTag = new CTag(); 
			pTag->m_strBody = yytext;
			pTag->m_strName = pTag->m_strBody;
			pTag->m_nType = TK_BRACKET_LEFT;
			CFileTags::m_pFileTags->AppendToken(pTag);
			yylval = pTag;
			
			return pTag->m_nType; 
		} 
"("		{
 
			CTag* pTag = new CTag(); 
			pTag->m_strBody = yytext;
			pTag->m_strName = pTag->m_strBody;
			pTag->m_nType = TK_PARENTHESES_LEFT;
			CFileTags::m_pFileTags->AppendToken(pTag);
			
			debug("\"%s\": %d\n", yytext, pTag->m_nType); 
			
			yylval = pTag;
			
			return pTag->m_nType; 
		}  
")"		{
 
			CTag* pTag = new CTag(); 
			pTag->m_strBody = yytext;
			pTag->m_strName = pTag->m_strBody;
			pTag->m_nType = TK_PARENTHESES_RIGHT;
			CFileTags::m_pFileTags->AppendToken(pTag);
			
			debug("\"%s\": %d\n", yytext, pTag->m_nType); 
			yylval = pTag;
			
			return pTag->m_nType; 
		} 
"<"     {
 
			CTag* pTag = new CTag(); 
			pTag->m_strBody = yytext;
			pTag->m_strName = pTag->m_strBody;
			pTag->m_nType = TK_ANGLE_BRACKET_LEFT;
			CFileTags::m_pFileTags->AppendToken(pTag);
			
			debug("\"%s\": %d\n", yytext, pTag->m_nType); 
			yylval = pTag;
			
			return pTag->m_nType; 
		} 
">"     {
		 
			CTag* pTag = new CTag(); 
			pTag->m_strBody = yytext;
			pTag->m_strName = pTag->m_strBody;
			pTag->m_nType = TK_ANGLE_BRACKET_RIGHT ;
			CFileTags::m_pFileTags->AppendToken(pTag);
			
			debug("\"%s\": %d\n", yytext, pTag->m_nType); 
			yylval = pTag;
			return pTag->m_nType ; 
		}  
":"		{
			debug("\"%s\"\n", yytext); 
			CTag* pTag = new CTag(); 
			pTag->m_strBody = yytext;
			pTag->m_strName = pTag->m_strBody;
			pTag->m_strName = pTag->m_strBody;
			pTag->m_nType = TK_COLON;
			
			CFileTags::m_pFileTags->AppendToken(pTag);
			
			yylval = pTag;
			return pTag->m_nType ; 
		} 
";"		{
			CTag* pTag = new CTag(); 
			pTag->m_strBody = yytext;
			pTag->m_strName = pTag->m_strBody;
			pTag->m_nType = TK_SEMICOLON;
			CFileTags::m_pFileTags->AppendToken(pTag);
			
			debug("\"%s\": %d\n", yytext, pTag->m_nType); 
			yylval = pTag;
			return pTag->m_nType ; 
		} 
","		{
			debug("\"%s\"\n", yytext); 
			CTag* pTag = new CTag(); 
			pTag->m_strBody = yytext;
			pTag->m_strName = pTag->m_strBody;
			pTag->m_nType = TK_COMMA;
			CFileTags::m_pFileTags->AppendToken(pTag);
			
			yylval = pTag;
			
			return pTag->m_nType ; 
		}
"\""		{
	debug("\"%s\"\n", yytext); 
	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_QUOTATION;
	CFileTags::m_pFileTags->AppendToken(pTag);

	yylval = pTag;

	return pTag->m_nType ; 
}
"#"		{
	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_POUND_SIGN;
	CFileTags::m_pFileTags->AppendToken(pTag);
	
	debug("\"%s\": %d\n", yytext, pTag->m_nType); 
	yylval = pTag;
	
	return pTag->m_nType; 
}  
"~"		{
	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_INVERSE;
	CFileTags::m_pFileTags->AppendToken(pTag);
	
	debug("\"%s\": %d\n", yytext, pTag->m_nType); 
	yylval = pTag;
	
	return pTag->m_nType; 
}
"*"		{
	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_STAR;
	
	CFileTags::m_pFileTags->AppendToken(pTag);
	
	debug("\"%s\": %d\n", yytext, pTag->m_nType); 
	yylval = pTag;
	
	return pTag->m_nType; 
}
"="		{
	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_EQU;
	CFileTags::m_pFileTags->AppendToken(pTag);
	
	debug("\"%s\": %d\n", yytext, pTag->m_nType); 
	yylval = pTag;
	
	return pTag->m_nType; 
	}
"&"		{
	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_AND;
	CFileTags::m_pFileTags->AppendToken(pTag);
	
	debug("\"%s\": %d\n", yytext, pTag->m_nType); 
	yylval = pTag;
	
	return pTag->m_nType; 
	}	
"pragma"		{
	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_PRAGMA;
	CFileTags::m_pFileTags->AppendToken(pTag);
	
	debug("\"%s\": %d\n", yytext, pTag->m_nType); 
	yylval = pTag;
	
	return pTag->m_nType; 
}
"once"		{
	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_ONCE;
	CFileTags::m_pFileTags->AppendToken(pTag);
	
	debug("\"%s\": %d\n", yytext, pTag->m_nType); 
	yylval = pTag;
	
	return pTag->m_nType; 
}
"include" {
	
	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_KW_INCLUDE;
	CFileTags::m_pFileTags->AppendToken(pTag);
	
	debug("\"%s\": %d\n", yytext, pTag->m_nType); 
	yylval = pTag;
	
	return pTag->m_nType; 
}

"using"[[:blank:]]+"namespace"[[:blank:]]+[a-zA-Z0-9_]+[[:blank:]]*";" {
	
	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_USING_NAME_SPACE;
	CFileTags::m_pFileTags->AppendToken(pTag);
	
	debug("\"%s\": %d\n", yytext, pTag->m_nType); 
	yylval = pTag;
	
	return pTag->m_nType; 
}
"\#define"  {
	
	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_KW_DEFINE;
	CFileTags::m_pFileTags->AppendToken(pTag);
	
	debug("\"%s\": %d\n", yytext, pTag->m_nType); 
	yylval = pTag;
	
	return pTag->m_nType; 
}
"\#ifndef"  {
	
	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_KW_IFNDEF;
	CFileTags::m_pFileTags->AppendToken(pTag);
	
	debug("\"%s\": %d\n", yytext, pTag->m_nType); 
	yylval = pTag;
	
	return pTag->m_nType; 
}
"\#ifdef"  {

	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_KW_IFDEF;
	CFileTags::m_pFileTags->AppendToken(pTag);

	debug("\"%s\": %d\n", yytext, pTag->m_nType); 
	yylval = pTag;

	return pTag->m_nType; 
}
"\#else"  {

	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_KW_ELSE;
	CFileTags::m_pFileTags->AppendToken(pTag);

	debug("\"%s\": %d\n", yytext, pTag->m_nType); 
	yylval = pTag;

	return pTag->m_nType; 
}

"\#endif".* {
	
	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_KW_ENDIF;
	CFileTags::m_pFileTags->AppendToken(pTag);
	
	debug("\"%s\": %d\n", yytext, pTag->m_nType); 
	yylval = pTag;
	
	return pTag->m_nType; 
}

"class" {
	
	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_KW_CLASS;
	CFileTags::m_pFileTags->AppendToken(pTag);
	
	debug("\"%s\": %d\n", yytext, pTag->m_nType); 
	yylval = pTag;
	
	return pTag->m_nType; 
}
"virtual"  {
	
	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_KW_VIRTUAL;
	CFileTags::m_pFileTags->AppendToken(pTag);
	
	debug("\"%s\": %d\n", yytext, pTag->m_nType); 
	yylval = pTag;
	
	return pTag->m_nType; 
}
"void"  {
	
	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_KW_VOID;
	CFileTags::m_pFileTags->AppendToken(pTag);
	
	debug("\"%s\": %d\n", yytext, pTag->m_nType); 
	yylval = pTag;
	
	return pTag->m_nType; 
}
"typedef"  {
	
	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_KW_TYPEDEF;
	CFileTags::m_pFileTags->AppendToken(pTag);
	
	debug("\"%s\": %d\n", yytext, pTag->m_nType); 
	yylval = pTag;
	
	return pTag->m_nType; 
}
"enum"  {
	
	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_KW_ENUM;
	CFileTags::m_pFileTags->AppendToken(pTag);
	
	debug("\"%s\": %d\n", yytext, pTag->m_nType); 
	yylval = pTag;
	
	return pTag->m_nType; 
}
"IF_FUN"  {
	
	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_KW_IF_FUN;
	CFileTags::m_pFileTags->AppendToken(pTag);
	
	debug("\"%s\": %d\n", yytext, pTag->m_nType); 
	yylval = pTag;
	
	return pTag->m_nType; 
}
"IF_SLOT"  {
	
	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_KW_IF_SLOT;
	CFileTags::m_pFileTags->AppendToken(pTag);
	
	debug("\"%s\": %d\n", yytext, pTag->m_nType); 
	yylval = pTag;
	
	return pTag->m_nType; 
}
"OUT_P"  {
	
	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_KW_OUT_P;
	CFileTags::m_pFileTags->AppendToken(pTag);
	
	debug("\"%s\": %d\n", yytext, pTag->m_nType); 
	yylval = pTag;
	
	return pTag->m_nType; 
}
"ONE_WAY"  {

	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_KW_ONE_WAY;
	CFileTags::m_pFileTags->AppendToken(pTag);

	debug("\"%s\": %d\n", yytext, pTag->m_nType); 
	yylval = pTag;

	return pTag->m_nType; 
}
"public"  {
	
	CTag* pTag = new CTag(); 
	pTag->m_strBody = yytext;
	pTag->m_strName = pTag->m_strBody;
	pTag->m_nType = TK_KW_PUBLIC;
	CFileTags::m_pFileTags->AppendToken(pTag);
	
	debug("\"%s\": %d\n", yytext, pTag->m_nType); 
	yylval = pTag;
	
	return pTag->m_nType; 
}

"//".* {
			debug("\"%s\"\n", yytext); 
			CTag* pTag = new CTag(); 
			pTag->m_strBody = yytext;
			pTag->m_strName = pTag->m_strBody;
			pTag->m_nType = TK_COMMENT_LINE;
			CFileTags::m_pFileTags->AppendToken(pTag);
			
			yylval = pTag;
			
			return TK_COMMENT_LINE; 
		}
"/*"(.|\n)*"*/" {
			debug("\"%s\"\n", yytext); 
 			CTag* pTag = new CTag(); 
			pTag->m_strBody = yytext;
			pTag->m_strName = pTag->m_strBody;
			pTag->m_nType = TK_COMMENT_BLOCK;
			CFileTags::m_pFileTags->AppendToken(pTag);
			
			yylval = pTag;
			
			return TK_COMMENT_BLOCK; 
		}
		
[a-zA-Z0-9_\.]+ {
			debug("\"%s\" word\n", yytext); 
			
			CTag* pTag = new CTag(); 
			pTag->m_strBody = yytext;
			pTag->m_strName = pTag->m_strBody;
			pTag->m_nType = WORD;
			CFileTags::m_pFileTags->AppendToken(pTag);
			
			yylval = pTag;
			
			return pTag->m_nType; 
		}

. |
\n ;  //抛弃其他字符
%%
/*
"#"		{
			debug("\"%s\"\n", yytext); 
			CTag* pTag = new CTag(); 
			pTag->m_strBody = yytext;
			pTag->m_nType = TK_POUND_SIGN;
			CFileTags::m_pFileTags->AppendToken(pTag);
 			
 			yylval = pTag;
 			
			return pTag->m_nType; 
		}


}
// .* {//行中剩余的字符。非换行
// 	debug("\"%s\" word\n", yytext); 
// 
// 	CTag* pTag = new CTag(); 
// 	pTag->m_strBody = yytext;
// 	pTag->m_strName = pTag->m_strBody;
// 	pTag->m_nType = TK_LINE;
// 	CFileTags::m_pFileTags->AppendToken(pTag);
// 
// 	yylval = pTag;
// 
// 	return pTag->m_nType; 
// }

*/
int yywrap(void)
{
  return 1;
}
