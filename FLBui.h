//=================================================================
//Filename		:	FLBui.h
//Description	:	界面处理的相关函数
//Date			:	2012/08/08 
//=================================================================
#pragma once 

#include <iostream>
#include <string>
#include "highgui.h"
#include "FLBstd.h" // min , max
//#include "FLBcore.h"

using cv::createTrackbar;
using std::cout;
using std::endl;

//输出相关数据结构，for debug
void printImageInfo ( const IplImage* src );
void printScalar( CvScalar& scalar );

#ifndef WIN_SIZE
// size of image windows
#define WIN_SIZE
#define	WIN_WIDTH   1000
#define WIN_HEIGHT	800
#endif

void showImage( char* winname, IplImage* img );
void showImageCallback( int pos, void* userdata );

