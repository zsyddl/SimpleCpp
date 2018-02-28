这个包主要实现了最简单的Lex和Yacc环境，包含了最少的但是必须的文件。

这个包包含的文件列表：

bison.exe         GNU的yacc程序
bison.hairy       GNU的yacc程序运行需要的文件
bison.simple      GNU的yacc程序运行需要的文件
flex.exe          GNU的lex程序
ini.bat           这个lex和yacc环境的环境变量配置
lexyacc.bat       这个lex和yacc环境的启动程序
Readme.txt        本说明文件

使用方法：

1. 鼠标双击lexyacc.bat文件运行
2. 在弹出的DOS命令行中利用CD切换到你的lex和yacc源文件所在的目录
3. 对lex源文件执行flex lex.l
4. 对yacc源文件执行bison -d yacc.y

经过上面的四个步骤就生成了你需要的C/C++源文件，剩下的事情就是编译
这里生成的C/C++源程序了。

最后谢谢您的使用，也希望您提出宝贵的意见或者建议，我会认真考虑您的
意见或者建议的。可以发邮件到pandaxcl@163.com和我联系。另外也希望您
关注一下我的博客http://blog.csdn.net/pandaxcl谢谢:)
