//=================================================================
//Filename		:	FLBstd.cpp
//Description	:	��opencv�޹ص�һЩ�����Ĵ�����
//Date			:	2012/08/08 
//=================================================================

#include "FLBcore.h"

//���ȴ�����
void addBri( const IplImage* src, IplImage* dst, CvScalar colorvalue, IplImage* mask )
//���룺
//    src Ϊ��ͨ�����ͨ����ͼ��
//    colorvalue Ϊ��Ӧÿ��ͨ��Ҫ�仯������ֵ
//    mask Ϊ��ͨ���ĻҶ�ͼ��ֻ�ı��0ֵ��ǵ����ص�����ȣ�0��ǵ��������Ȳ���
//�����
//    dst ͼ��src����ֵ�ı���ͼ��
{
	cvAddS( src, colorvalue, dst, mask );
}

void addBri( const IplImage* src , IplImage* dst , int grayvalue, IplImage* mask )
//���룺
//    src Ϊ��ͨ�����ͨ����ͼ��
//    grayvalue Ϊÿ��ͨ��Ҫ�仯�ĻҶ�ֵ
//    mask Ϊ��ͨ���ĻҶ�ͼ��ֻ�ı��0ֵ��ǵ����ص�����ȣ�0��ǵ��������Ȳ���
//�����
//    dst ͼ��src����ֵ�ı���ͼ��
{
	grayvalue = checkrange( grayvalue, -255, 255 );
	cvAddS( src , cvScalar( grayvalue, grayvalue, grayvalue, 0 ), dst, mask );
}

void getAvrBri ( const IplImage* src, CvScalar& avrbri, IplImage* mask )
//���룺 
//    src  Ϊ��ͨ�����ͨ����ͼ��
//    mask �����ǵ�ͨ���ĻҶ�ͼ�������ص��ֵ��0���0����״̬���Ҵ�С��src��ͬ
//�����
//    avrbri ͼ��src����Ӧͨ����ƽ������
{
	int pixnum=-1;
	if ( mask )
	{
		IplImage* blkimg = cvCloneImage( src );
		IplImage* binmask = cvCloneImage( mask );
		cvThreshold( mask, binmask, 0, 1, CV_THRESH_BINARY );
		cvZero( blkimg );
		cvAdd( src, blkimg, blkimg, binmask );
		avrbri = cvSum( blkimg );
		pixnum = cvSum( binmask ).val[0];
		cvReleaseImage( &blkimg );
		cvReleaseImage( &binmask );
	}
	else
	{
		pixnum = src->width * src->height ;
		avrbri = cvSum( src );
	}
	avrbri.val[0] = avrbri.val[0]/pixnum;
	avrbri.val[1] = avrbri.val[1]/pixnum;
	avrbri.val[2] = avrbri.val[2]/pixnum;
	avrbri.val[3] = avrbri.val[3]/pixnum;
}


//ɫ�׵�������
void colorLevel( const IplImage* src, IplImage* dst, int low, int high, double mid )
//���룺 
//    src  Ϊ��ͨ�����ͨ����ͼ��
//    lowΪ��Ӱ��0~255���� highΪ�߹⣨0~255����midΪ�м����0.01~9.99��������Ϊ0.01��
//�����
//    dst  Ϊͼ��srcɫ�ױ任��Ľ��
{ 
	double k = 255.0 / ( high - low );
	double tmp = 0 ;
	mid = checkrange( mid, 0.01, 9.99 );
	uchar* srcdata = (uchar*) src->imageData;
	uchar* dstdata = (uchar*) dst->imageData;
	for ( int row=0 ; row < src->height ; row++ )
	{
		for ( int col=0 ; col < src->widthStep ; col++ )
		{
				tmp = (double)srcdata[col];
				tmp = (int) floor(pow( max( 0, tmp-low ) / ( high-low ), ( 1/mid ) ) * 255.0 + 0.5 );
				dstdata[col] = (uchar) checkrange( tmp , 0 , 255 );
		}
		srcdata += src->widthStep ;
		dstdata += dst->widthStep ;
	}
	return ;
}
void colorLevel3C( const IplImage * src ,IplImage * dst ,CvScalar low ,CvScalar high ,CvScalar mid )
// ������ͨ����ͼ��src��ÿ��ͨ�����ò�ͬ��ɫ�׵�������
{
	IplImage* src_r = cvCreateImage( cvGetSize(src) , src->depth , 1 );
	IplImage* src_g = cvCloneImage( src_r );
	IplImage* src_b = cvCloneImage( src_r );
	cvSplit( src , src_b , src_g , src_r , 0 );
	colorLevel( src_b , src_b , low.val[0] , high.val[0] , mid.val[0] );
	colorLevel( src_g , src_g , low.val[1] , high.val[1] , mid.val[1] );
	colorLevel( src_r , src_r , low.val[2] , high.val[2] , mid.val[2] );
	cvMerge( src_b , src_g , src_r , 0 , dst );
	cvReleaseImage( &src_r );
	cvReleaseImage( &src_g );
	cvReleaseImage( &src_b );
	return ;
}

//���ģʽ����(ͼ��ϲ���
double calcSoftLight ( double basiccolor , double mixcolor )
// ���Ч���ļ��㹫ʽ
{
	if ( mixcolor > 128 )
		return basiccolor + (2 * mixcolor - 255 ) * ( sqrt( basiccolor/255 ) * 255 - basiccolor ) / 255;
	else
		return basiccolor + (2 * mixcolor - 255 ) * ( basiccolor - basiccolor * basiccolor / 255 ) / 255;
}
void softLightCombine( IplImage* bg , IplImage* fg , IplImage* dst )
//���룺
//    bg Ϊ����ͼ�� fgΪǰ��ͼ�񣬵�/��ͨ��
//�����
//    dst Ϊ�ϲ����ͼ�� ����С��ͨ������bg��fg����һ��
{
	double tmp = 0;
	uchar* bgdata = (uchar*) bg->imageData;
	uchar* fgdata = (uchar*) fg->imageData;
	uchar* dstdata = (uchar*) dst->imageData;
	for ( int row=0 ; row < bg->height ; row++ )
	{
		for ( int col=0 ; col < bg->widthStep ; col++ )
		{
			tmp = floor( calcSoftLight( bgdata[col] , fgdata[col] ) + 0.5 );
			tmp = checkrange( tmp , 0 , 255 );
			dstdata[col] = (uchar) tmp ;
		}
		bgdata += bg->widthStep ;
		fgdata += fg->widthStep ;
		dstdata += dst->widthStep ;
	}
}
void multiplyCombine( IplImage* bg , IplImage* fg , IplImage* dst )
//���룺
//    bg Ϊ����ͼ�� fgΪǰ��ͼ�񣬵�/��ͨ��
//�����
//    dst Ϊ�ϲ����ͼ�� ����С��ͨ������bg��fg����һ��
{
	double tmp = 0;
	uchar* bgdata = (uchar*) bg->imageData;
	uchar* fgdata = (uchar*) fg->imageData;
	uchar* dstdata = (uchar*) dst->imageData;
	for ( int row=0; row < bg->height ; row++ )
	{
		for ( int col=0 ; col < bg->widthStep ; col++ )
		{
			tmp = floor( double(bgdata[col])* double(fgdata[col])/255.0 + 0.5 );
			tmp = checkrange( tmp , 0 , 255 );
			dstdata[col] = (uchar) tmp ;
		}
		bgdata += bg->widthStep ;
		fgdata += fg->widthStep ;
		dstdata += dst->widthStep ;
	}
}

void imageCombine( IplImage* bg , IplImage* fg , IplImage* alpha , IplImage* dst )
// alpha ��ͨ������Χ0~255 �� 255��ʾȡ100%��ǰ��fg��0%�ı���bg����������
// bg,fg,dstΪ��/��ͨ����ͨ����������ͬ
{
	double tmp = 0;
	double pixalpha = 0;//��һ��������alphaֵ
	uchar* bgdata = (uchar*) bg->imageData;
	uchar* fgdata = (uchar*) fg->imageData;
	uchar* dstdata = (uchar*) dst->imageData;
	uchar* alphadata = (uchar*) alpha->imageData;
	for ( int row=0; row < bg->height ; row++ )
	{
		for ( int col=0 ; col < bg->width ; col++ )
		{
			pixalpha = double(alphadata[col])/255.0 ;
			for ( int cn=0 ; cn < bg->nChannels ; cn++ )
			{
				tmp = floor( double(bgdata[bg->nChannels*col+cn])*(1-pixalpha) + double(fgdata[bg->nChannels*col+cn])*pixalpha + 0.5 );
				tmp = checkrange( tmp , 0 , 255 );
				dstdata[bg->nChannels*col+cn] = (uchar) tmp ;
			}
		}
		bgdata += bg->widthStep ;
		fgdata += fg->widthStep ;
		dstdata += dst->widthStep ;
		alphadata += alpha->widthStep ;
	}
}

void imageCombine( IplImage* bg , IplImage* fg , double alpha , IplImage* dst )
// alpha Ϊǰ��ͼfg�Ĳ�͸����(0~1)����alpha=1��Ϊǰ����alpha=0��Ϊ����
// bg,fg,dstΪ��/��ͨ����ͨ����������ͬ
{
	cvAddWeighted ( fg , alpha , bg , 1-alpha , 0 , dst );
}



CvRect cvRect( CvPoint leftup , CvPoint rightdown )
// ���룺	leftup���ϵ� �� rightdown ���µ�
// �����	������ȷ���ķ�������
{
	return cvRect( leftup.x , leftup.y , rightdown.x - leftup.x , rightdown.y - leftup.y );
}

void getMaxMinValue( IplImage* src, CvScalar& max_val, CvScalar& min_val )
//  max_val , min_val �ֱ���ÿ��ͨ�������ֵ����Сֵ������ͨ�������ڣ���ֵΪ-1
{
	setScalarValue( max_val, -1, -1, -1, -1 );
	setScalarValue( min_val, -1, -1, -1, -1 );
	IplImage* gray_channel = cvCreateImage( cvGetSize(src), src->depth, 1 );
	for( int channel=0 ; channel < src->nChannels ; channel++ )
	{
		switch ( channel )
		{
		case 0 : 
			cvSplit( src, gray_channel, 0, 0, 0);
			break;
		case 1 : 
			cvSplit( src, 0, gray_channel, 0, 0);
			break;
		case 2 : 
			cvSplit( src, 0, 0, gray_channel, 0);
			break;
		case 3 : 
			cvSplit( src, 0, 0, 0, gray_channel);
			break;
		}
		cvMinMaxLoc( gray_channel, &min_val.val[channel], &max_val.val[channel] );
	}
	cvReleaseImage( &gray_channel );
}

void setScalarValue( CvScalar& scalar , double val0, double val1, double val2, double val3 )
{
	scalar.val[0] = val0;
	scalar.val[1] = val1;
	scalar.val[2] = val2;
	scalar.val[3] = val3;
}

void scalarAdd( CvScalar& scalar0, CvScalar& scalar1, CvScalar& dst )
{
	for ( int idx=0; idx < 4; idx++ )
		dst.val[idx] = scalar0.val[idx] + scalar1.val[idx];
}

void scalarSub( CvScalar& scalar0, CvScalar& scalar1, CvScalar& dst )
{
	for ( int idx=0; idx < 4; idx++ )
		dst.val[idx] = scalar0.val[idx] - scalar1.val[idx];
}

void scalarMul( CvScalar& scalar0, CvScalar& scalar1, CvScalar& dst )
{
	for ( int idx=0; idx < 4; idx++ )
		dst.val[idx] = scalar0.val[idx] * scalar1.val[idx];
}

void scalarDiv( CvScalar& scalar0, CvScalar& scalar1, CvScalar& dst )
{
	for ( int idx=0; idx < 4; idx++ )
		dst.val[idx] = scalar0.val[idx] / scalar1.val[idx];
}

void fillImage( IplImage* src, const CvScalar& color )
// ���룺 srcҪ����ͼ�� color ������ɫ
// ����� src
{
	cvZero( src );
	cvAddS( src, color, src );
}

void selAdd( IplImage* sel0, IplImage* sel1, IplImage* dst)
// ����: sel0,sel1Ϊ��/��ͨ��ͼ�� 0Ϊδѡ�У� ��0 Ϊѡ�У�
// ����� dst����sel0��sel1��ͬ��ͼ�� ֻ��0��255����ֵ��0��ʾδѡ�У�255��ʾѡ��
{
	IplImage* tmp_sel0 = cvCloneImage( sel0 );
	IplImage* tmp_sel1 = cvCloneImage( sel1 );
	cvThreshold( tmp_sel0, tmp_sel0, 0, 1, CV_THRESH_BINARY );
	cvThreshold( tmp_sel1, tmp_sel1, 0, 1, CV_THRESH_BINARY );
	cvOr( tmp_sel0, tmp_sel1, dst );
	cvThreshold( dst, dst, 0, 255, CV_THRESH_BINARY );
	cvReleaseImage( &tmp_sel0 );
	cvReleaseImage( &tmp_sel1 );
}

void selSub( IplImage* sel0, IplImage* sel1, IplImage* dst)
// ����: sel0,sel1Ϊ��/��ͨ��ͼ�� 0Ϊδѡ�У� ��0 Ϊѡ�У�
// ����� dst����sel0��sel1��ͬ��ͼ�� ֻ��0��255����ֵ��0��ʾδѡ�У�255��ʾѡ��
{
	IplImage* tmp_sel0 = cvCloneImage( sel0 );
	IplImage* tmp_sel1 = cvCloneImage( sel1 );
	cvThreshold( tmp_sel0, tmp_sel0, 0, 1, CV_THRESH_BINARY );
	cvThreshold( tmp_sel1, tmp_sel1, 0, 1, CV_THRESH_BINARY );
	cvSub( tmp_sel0, tmp_sel1, dst );
	cvThreshold( dst, dst, 0, 255, CV_THRESH_BINARY );
	cvReleaseImage( &tmp_sel0 );
	cvReleaseImage( &tmp_sel1 );
}

void selAnd( IplImage* sel0, IplImage* sel1, IplImage* dst)
// ����: sel0,sel1Ϊ��/��ͨ��ͼ�� 0Ϊδѡ�У� ��0 Ϊѡ�У�
// ����� dst����sel0��sel1��ͬ��ͼ�� ֻ��0��255����ֵ��0��ʾδѡ�У�255��ʾѡ��
{
	IplImage* tmp_sel0 = cvCloneImage( sel0 );
	IplImage* tmp_sel1 = cvCloneImage( sel1 );
	cvThreshold( tmp_sel0, tmp_sel0, 0, 1, CV_THRESH_BINARY );
	cvThreshold( tmp_sel1, tmp_sel1, 0, 1, CV_THRESH_BINARY );
	cvAnd( tmp_sel0, tmp_sel1, dst );
	cvThreshold( dst, dst, 0, 255, CV_THRESH_BINARY );
	cvReleaseImage( &tmp_sel0 );
	cvReleaseImage( &tmp_sel1 );
}

void selFeathering( IplImage* sel, IplImage* dst, double r )
{
	cvSmooth( sel, dst, CV_GAUSSIAN, 2*r+1, 2*r+1, r/0.707, r/0.707 );
}


void addAlpha ( IplImage * src , IplImage * alpha , IplImage * dst )
{//srcΪ��ͨ�� �� alphaΪ��ͨ���� ����һ����ͨ����ͼ��dst  , src ,alpha , dst��ͼ���Сһ��
	IplImage * r = cvCreateImage( cvGetSize(src),src->depth , 1 );
	IplImage * g = cvCreateImage( cvGetSize(src),src->depth , 1 );
	IplImage * b = cvCreateImage( cvGetSize(src),src->depth , 1 );
	cvSplit( src , b , g , r , 0 );
	cvMerge( b , g , r , alpha , dst );
	cvReleaseImage( &r );
	cvReleaseImage( &g );
	cvReleaseImage( &b );
}

void setCvRect( CvRect* rect, int x, int y, int width, int height )
{
	rect->x = x;
	rect->y = y;
	rect->width = width;
	rect->height = height;
}
void faceLightBalance( IplImage* src, IplImage* sel, CvScalar refcolor, CvScalar selcolor, IplImage* dst )
{
	double stdbri = getBri( refcolor.val[2], refcolor.val[1], refcolor.val[0] );
	double actbri = getBri ( selcolor.val[2], selcolor.val[1], selcolor.val[0] );
	IplImage* baseimage = cvCreateImage( cvGetSize( src ), src->depth, src->nChannels );
	IplImage* purecolor = cvCloneImage( src );
	fillImage( purecolor, refcolor );
	if ( stdbri > actbri )
	// ʵ������ϰ������ǰ������
	{
		fillImage( baseimage, cvScalar(128,128,128) );
		imageCombine( baseimage, purecolor, sel, purecolor );
		softLightCombine( src, purecolor, dst );
	}
	else
	// ʵ���������������������Ƭ����
	{
		fillImage( baseimage, cvScalar(255,255,255) );
		imageCombine( baseimage, purecolor, sel, purecolor );
		multiplyCombine( src, purecolor, purecolor );
		IplImage* alpha = cvCreateImage( cvGetSize( src ), src->depth, 1 );
		getAlphaImage( purecolor, src, alpha , actbri-stdbri );
		imageCombine( src, purecolor, alpha, dst );
	}
	cvReleaseImage( &purecolor );
	cvReleaseImage( &baseimage );
}
void getAlphaImage( IplImage* fg, IplImage* bg, IplImage* alpha, double difbri )
{
	IplImage* difimg = cvCreateImage( cvGetSize( fg ), fg->depth, fg->nChannels );
	cvSub( bg, fg, difimg );
	CvScalar minval = cvScalar( 0 );
	CvScalar maxval = cvScalar( 0 );
	getMaxMinValue( difimg, maxval, minval );
	cvSplit( difimg, 0, 0, alpha, 0 );
	cvMerge( alpha, alpha, alpha, 0, difimg );
	colorLevel( alpha, alpha, minval.val[2]-50, maxval.val[2]+100, 0.5 );
	cvReleaseImage( &difimg );
}

void autoFLB( IplImage* image, IplImage* dst, IplImage* skin, double low, double high )
{
	IplImage* sel = cvCreateImage( cvGetSize( image ), image->depth, 1 );//�����ǵ�ѡ��
	IplImage* midtone = cvCreateImage( cvGetSize( image ), image->depth, 1 );
	IplImage* shadow = cvCreateImage( cvGetSize( image ), image->depth, 1 );
	IplImage* highlight = cvCreateImage( cvGetSize( image ), image->depth, 1 );
	IplImage* rcn = cvCreateImage( cvGetSize( image ), image->depth, 1 );
	CvScalar refcolor =cvScalar( 0 );//��Ϊ�ο��ı�׼��ɫ
	CvScalar selcolor = cvScalar( 0 );//������ѡ����ʵ����ɫ
	double radius = 0; //�𻯰뾶
	
	///=========��ʼ��������ѡ��============//
	cvSplit( image, 0, 0, rcn, 0 );
	createMagicwand( rcn, 0, low, shadow );
	createMagicwand( rcn, low, high, midtone );
	createMagicwand( rcn, high, 255, highlight );
	selAnd( shadow, skin, shadow );
	selAnd( midtone, skin, midtone );
	selAnd( highlight, skin, highlight );

	//===========���ǰ���============//
	//����ѡ��
	getAvrBri( image, refcolor, highlight );
	refcolor.val[3] = getBri( refcolor.val[2], refcolor.val[1], refcolor.val[0] );
	selAdd( shadow, midtone, sel );//���ñ����ǵ�ѡ��
	getAvrBri( image, selcolor, sel );
	selcolor.val[3] = getBri( selcolor.val[2], selcolor.val[1], selcolor.val[0] );

	radius = floor( 20 + 2* log10(cvSum( sel ).val[0] / 255) ); //��ѡ��
	selFeathering( sel, sel, radius );
	faceLightBalance( image, sel, refcolor, selcolor, dst );//��ɫƽ��

	//===========��������============//
	getAvrBri( image, refcolor, shadow );
	refcolor.val[3] = getBri( refcolor.val[2], refcolor.val[1], refcolor.val[0] );
	selAdd( midtone, highlight, sel );
	getAvrBri( dst, selcolor, sel );// ����ǰ or ������
	selcolor.val[3] = getBri( selcolor.val[2], selcolor.val[1], selcolor.val[0] );
	radius = floor( 5 + pow( log10((cvSum( sel ).val[0] / 255)), 3 ) * 0.5);
	selFeathering( sel, sel, radius );
	faceLightBalance( dst, sel, refcolor, selcolor, dst );
}
