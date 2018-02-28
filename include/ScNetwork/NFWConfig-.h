

#ifndef NFWConfig_H
#define NFWConfig_H


#ifdef WIN32
#ifndef NFW_EXPORTS
#    define NFW_API __declspec(dllimport)
#  elif defined(NFW_EXPORTS) 
#    define NFW_API __declspec(dllexport)
#  else
#    define NFW_API 
#  endif

#else 

#define NFW_API

#endif



#endif