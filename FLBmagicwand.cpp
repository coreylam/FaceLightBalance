//=================================================================
//Filename		:	FLBmagicwand.cpp
//Description	:	魔棒选区的相关函数
//Date			:	2012/08/08 
//=================================================================

#include "FLBmagicwand.h"

//魔棒选区
void createMagicwand( const IplImage* src, const double low, const double up, IplImage* dst )
//输入：　src 单通道灰度图， low 下限值， up上限值
//输出：	dst 单通道灰度图，像素值为0或255
{
	IplImage* floorimage = cvCreateImage( cvGetSize( src ), src->depth, 1 );
	IplImage* ceilingimage = cvCreateImage( cvGetSize( src ), src->depth, 1 );
	cvThreshold( src, floorimage, low-1, 1, CV_THRESH_BINARY );
	cvThreshold( src, ceilingimage, up, 1, CV_THRESH_BINARY );
	selSub( floorimage, ceilingimage, dst );
	cvReleaseImage( &ceilingimage );
	cvReleaseImage( &floorimage );
}
void createMagicwand( const IplImage* src, CvScalar scalar0, CvScalar scalar1, IplImage* dst, int select_type )
//输入： src原图像， color 被选中的参考颜色， tolerance 容差值
//			select_type选择选区类型(默认是边界型）:
//				FLB_TOLERANCE=1:	容差值类型（指定颜色值和容差值）,	scalar0表示颜色值， scalar1表示容差值
//				FLB_BOUND=0:		表示边界型（指定上下界）,			scalar0表示下边界， scalar1表示上边界
//输出： dst 目标选区，是一个范围为0~255的单通道灰度图，其大小只有0或255两个值
{
	IplImage* grayimage = cvCreateImage( cvGetSize( src ), src->depth, 1 );
	IplImage* tmpsel = cvCloneImage( grayimage );//临时选区
	fillImage( tmpsel, cvScalar( 255 ) );
	CvScalar low = cvScalar( scalar0.val[0], scalar0.val[1], scalar0.val[2], scalar0.val[3] );
	CvScalar up = cvScalar( scalar1.val[0], scalar1.val[1], scalar1.val[2], scalar1.val[3] );
	if ( select_type )
	{
		scalarSub( scalar0, scalar1, low );
		scalarAdd( scalar0, scalar1, up );
	}
	for ( int channel=0; channel < src->nChannels ; channel++ )
	{
		switch (channel)
		{
		case 0:	cvSplit( src, grayimage, 0, 0, 0 );
			break;
		case 1:	cvSplit( src, 0, grayimage, 0, 0 );
			break;
		case 2:	cvSplit( src, 0, 0, grayimage, 0 );
			break;
		case 3:	cvSplit( src, 0, 0, 0, grayimage );
		}
		createMagicwand( grayimage, low.val[channel], up.val[channel], dst );
		selAnd( tmpsel, dst, dst );
		cvCopy( dst, tmpsel );
	}
	cvReleaseImage( &tmpsel );
	cvReleaseImage( &grayimage );
}

void createMagicwand( const IplImage* src, const CvPoint& point, const CvScalar& tolerance, IplImage* dst )
//输入： src原图像， point , 选中的点， tolerance 容差值
//输出： dst 目标选区，是一个范围为0~255的单通道灰度图，其大小只有0或255两个值
{
	 CvScalar color=cvScalar(CV_IMAGE_ELEM( src, uchar, point.x, point.y ));
	 if ( src->nChannels ==3 )
	 {
		 color.val[0] = CV_IMAGE_ELEM( src, uchar, point.x, 3*point.y );
		 color.val[1] = CV_IMAGE_ELEM( src, uchar, point.x, 3*point.y+1 );
		 color.val[2] = CV_IMAGE_ELEM( src, uchar, point.x, 3*point.y+2 );
	 }
	 createMagicwand( src, color, tolerance, dst );
}
