//=================================================================
//Filename		:	FLBstd.h
//Description	:	与opencv无关的一些基本的处理函数
//Date			:	2012/08/08 
//=================================================================
#pragma once

#include "FLBinclude.h"

#ifndef max
#define max(a,b)	(((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)	(((a) < (b)) ? (a) : (b))
#endif

#ifndef checkrange
// 检查x是否超出[low , up]
#define checkrange( x, low , up ) (x)<(low)?(low):((x)>(up)?(up):(x))
#endif

// 返回 ch0+ch1
char* cstrcat ( char* ch0 , char* ch1 );

// 返回 ch0+ch1+ch2
char* cstrcat ( char* ch0 , char* ch1 , char* ch2 );

// 把整数转化成对应的C风格字符串
char* int2str ( int num );

// 检查文件是否存在
bool chkfile( char* filename );

//从文件目录和文件名字从文件路径分离出来
//返回false表示找不到分隔符。
bool separatePath ( char* path, char* dir, char* name, char* delimiter ="\\" );
