//=================================================================
//Filename		:	FLBstd.h
//Description	:	��opencv�޹ص�һЩ�����Ĵ�����
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
// ���x�Ƿ񳬳�[low , up]
#define checkrange( x, low , up ) (x)<(low)?(low):((x)>(up)?(up):(x))
#endif

// ���� ch0+ch1
char* cstrcat ( char* ch0 , char* ch1 );

// ���� ch0+ch1+ch2
char* cstrcat ( char* ch0 , char* ch1 , char* ch2 );

// ������ת���ɶ�Ӧ��C����ַ���
char* int2str ( int num );

// ����ļ��Ƿ����
bool chkfile( char* filename );

//���ļ�Ŀ¼���ļ����ִ��ļ�·���������
//����false��ʾ�Ҳ����ָ�����
bool separatePath ( char* path, char* dir, char* name, char* delimiter ="\\" );
