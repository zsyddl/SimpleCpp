
#ifndef TYPE_DEFINE_H
#define TYPE_DEFINE_H

#include <string>
#include <vector>
#include <map>
using namespace std;
//接口函数
#define IF_FUN 
//接口回调函数，槽函数
#define IF_SLOT
//标记为输出函数
#define OUT_P

typedef char			int8;
typedef unsigned char	uint8;
typedef short			int16;
typedef unsigned short	uint16;
typedef int				int32;
typedef unsigned int	uint32;
typedef long long		int64;
typedef unsigned long long	uint64;
//
typedef vector<unsigned char> buffer;         
 
#endif