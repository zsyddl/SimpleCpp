
#ifndef SCP_DEFINE_H
#define SCP_DEFINE_H

#ifdef SCP_EXPORTS
#define SCP_API __declspec(dllexport)
#else
#define SCP_API __declspec(dllimport)
#endif




#endif