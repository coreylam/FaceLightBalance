//=================================================================
//Filename		:	FLBmagicwand.h
//Description	:	魔棒选区的相关函数
//Date			:	2012/08/08 
//=================================================================

#pragma once 
#include "FLBinclude.h"
#include "FLBcore.h"

enum{ 
	FLB_BOUND		=0, /*指定上下边界  */
	FLB_TOLERANCE	=1  /*指定颜色和容差*/
};

////根据参考颜色和容差值创建选区////
//根据点的位置和容差值确定选区
void createMagicwand( const IplImage* src, const CvPoint& point, const CvScalar& tolerance, IplImage* dst );
//单通道图像，根据上下边界值确定的选区,范围是[low,up]
void createMagicwand( const IplImage* src, const double low, const double up, IplImage* dst );
// 多通道图像创建选区，边界/容差
void createMagicwand( const IplImage* src, CvScalar scalar0, CvScalar scalar1, IplImage* dst, int select_type = FLB_BOUND );