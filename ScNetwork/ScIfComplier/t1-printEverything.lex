%%

%%

int yywrap(void)
{
  return 1;
}

main()
{
  yylex();
}