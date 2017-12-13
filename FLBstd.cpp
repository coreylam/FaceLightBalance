//=================================================================
//Filename		:	FLBstd.cpp
//Description	:	与opencv无关的一些基本的处理函数
//Date			:	2012/08/08 
//=================================================================
#include "FLBstd.h"

char* cstrcat ( char* ch0 , char* ch1 )
{
	int len0 = strlen(ch0);
	int len1 = strlen(ch1);
	char * ch = (char*) malloc (len0 + len1 +1);
	ch[len0+len1]=0;
	for ( int idx=0 ; idx < len0 ; idx++ )
		ch[idx] = ch0[idx];
	for ( int idx=len0 ; idx < len0+len1 ; idx++ )
		ch[idx] = ch1[idx-len0];
	return ch;
}

char* cstrcat ( char* ch0 , char* ch1 , char* ch2 )
{
	return cstrcat( cstrcat( ch0 , ch1 ) , ch2 );
}

char* int2str ( int num )
{
	char * str = (char*) malloc(0);
	itoa( num , str , 10 );
	return str;
}

bool chkfile( char* filename )
{
	if ( _access( filename , 0 ) == -1 )	return false;//文件不存在
	else	return true;//文件存在
}

bool separatePath ( char* path, char* dir, char* name, char* delimiter )
{
	int len = strlen( path );
	char* tmp = (char*) malloc(len);
	int namelen = 0;
	for ( namelen=0; namelen < len ; namelen++ )
	{
		if ( path[len-namelen-1] == delimiter[0] )
			break;
	}
	if ( namelen == len-1 && path[0] != delimiter[0] )
		return  false;
	dir[ len-namelen ] =0 ;
	name[ namelen ] = 0;
	for ( int idx=0; idx < len ; idx++ )
	{
		if ( idx< len-namelen )
			dir[idx] = path[idx];
		else
			name[idx+namelen-len] = path[idx];
	}
	delete tmp;
	return true;
}