#ifndef DHDVREXPORT_H
#define DHDVREXPORT_H

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DHDVR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DHDVR_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifdef WIN32

#ifdef DHDVR_EXPORTS
#define DHDVR_API __declspec(dllexport)
#else	//#ifdef DHDVR_EXPORTS
#define DHDVR_API __declspec(dllimport)
#endif

#else

#define DHDVR_API

#endif


#include "./kernel/afkplugin.h"

extern "C" DHDVR_API afk_plugin_info_t plugin_info;

#endif /* DHDVREXPORT_H */

