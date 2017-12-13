//=================================================================
//Filename		:	FLBmagicwand.cpp
//Description	:	ħ��ѡ������غ���
//Date			:	2012/08/08 
//=================================================================

#include "FLBmagicwand.h"

//ħ��ѡ��
void createMagicwand( const IplImage* src, const double low, const double up, IplImage* dst )
//���룺��src ��ͨ���Ҷ�ͼ�� low ����ֵ�� up����ֵ
//�����	dst ��ͨ���Ҷ�ͼ������ֵΪ0��255
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
//���룺 srcԭͼ�� color ��ѡ�еĲο���ɫ�� tolerance �ݲ�ֵ
//			select_typeѡ��ѡ������(Ĭ���Ǳ߽��ͣ�:
//				FLB_TOLERANCE=1:	�ݲ�ֵ���ͣ�ָ����ɫֵ���ݲ�ֵ��,	scalar0��ʾ��ɫֵ�� scalar1��ʾ�ݲ�ֵ
//				FLB_BOUND=0:		��ʾ�߽��ͣ�ָ�����½磩,			scalar0��ʾ�±߽磬 scalar1��ʾ�ϱ߽�
//����� dst Ŀ��ѡ������һ����ΧΪ0~255�ĵ�ͨ���Ҷ�ͼ�����Сֻ��0��255����ֵ
{
	IplImage* grayimage = cvCreateImage( cvGetSize( src ), src->depth, 1 );
	IplImage* tmpsel = cvCloneImage( grayimage );//��ʱѡ��
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
//���룺 srcԭͼ�� point , ѡ�еĵ㣬 tolerance �ݲ�ֵ
//����� dst Ŀ��ѡ������һ����ΧΪ0~255�ĵ�ͨ���Ҷ�ͼ�����Сֻ��0��255����ֵ
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
