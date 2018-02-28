// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 PIPELINE_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// PIPELINE_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
/*
* 流水线 类，用于通用数据处理流水线。
* 包含一个和多个 filter， filter之间输出信号与输入连接，从而形成数据流。
*/
#ifdef PIPELINE_EXPORTS
#define PIPELINE_API __declspec(dllexport)
#else
#define PIPELINE_API __declspec(dllimport)
#endif

// 此类是从 PipeLine.dll 导出的
class PIPELINE_API CPipeLine {
public:
	CPipeLine(void);
	// TODO: 在此添加您的方法。
};

extern PIPELINE_API int nPipeLine;

PIPELINE_API int fnPipeLine(void);
