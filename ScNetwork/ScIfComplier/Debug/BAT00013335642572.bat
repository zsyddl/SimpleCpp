@echo off
g:\works\cpp\FlexYacc\test\bison   -d -oTestParserY.cpp TestParser.y
g:\works\cpp\FlexYacc\test\flex   -oTestParserL.cpp TestParser.lex

if errorlevel 1 goto VCReportError
goto VCEnd
:VCReportError
echo Project : error PRJ0019: 某个工具从以下位置返回了错误代码: "正在执行预生成事件..."
exit 1
:VCEnd