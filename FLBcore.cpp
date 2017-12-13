//=================================================================
//Filename		:	FLBstd.cpp
//Description	:	与opencv无关的一些基本的处理函数
//Date			:	2012/08/08 
//=================================================================

#include "FLBcore.h"

//亮度处理函数
void addBri( const IplImage* src, IplImage* dst, CvScalar colorvalue, IplImage* mask )
//输入：
//    src 为单通道或多通道的图像
//    colorvalue 为对应每个通道要变化的亮度值
//    mask 为单通道的灰度图，只改变非0值标记的像素点的亮度，0标记的像素亮度不变
//输出：
//    dst 图像src亮度值改变后的图像
{
	cvAddS( src, colorvalue, dst, mask );
}

void addBri( const IplImage* src , IplImage* dst , int grayvalue, IplImage* mask )
//输入：
//    src 为单通道或多通道的图像
//    grayvalue 为每个通道要变化的灰度值
//    mask 为单通道的灰度图，只改变非0值标记的像素点的亮度，0标记的像素亮度不变
//输出：
//    dst 图像src亮度值改变后的图像
{
	grayvalue = checkrange( grayvalue, -255, 255 );
	cvAddS( src , cvScalar( grayvalue, grayvalue, grayvalue, 0 ), dst, mask );
}

void getAvrBri ( const IplImage* src, CvScalar& avrbri, IplImage* mask )
//输入： 
//    src  为单通道或多通道的图像
//    mask 必须是单通道的灰度图，其像素点的值是0或非0两种状态，且大小与src相同
//输出：
//    avrbri 图像src各对应通道的平均亮度
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


//色阶调整函数
void colorLevel( const IplImage* src, IplImage* dst, int low, int high, double mid )
//输入： 
//    src  为单通道或多通道的图像
//    low为阴影（0~255）， high为高光（0~255），mid为中间调（0.01~9.99），步长为0.01。
//输出：
//    dst  为图像src色阶变换后的结果
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
// 对于三通道的图像src中每个通道采用不同的色阶调整参数
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

//混合模式函数(图像合并）
double calcSoftLight ( double basiccolor , double mixcolor )
// 柔光效果的计算公式
{
	if ( mixcolor > 128 )
		return basiccolor + (2 * mixcolor - 255 ) * ( sqrt( basiccolor/255 ) * 255 - basiccolor ) / 255;
	else
		return basiccolor + (2 * mixcolor - 255 ) * ( basiccolor - basiccolor * basiccolor / 255 ) / 255;
}
void softLightCombine( IplImage* bg , IplImage* fg , IplImage* dst )
//输入：
//    bg 为背景图像， fg为前景图像，单/多通道
//输出：
//    dst 为合并后的图像 ，大小和通道数与bg，fg保持一致
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
//输入：
//    bg 为背景图像， fg为前景图像，单/多通道
//输出：
//    dst 为合并后的图像 ，大小和通道数与bg，fg保持一致
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
// alpha 单通道，范围0~255 ， 255表示取100%的前景fg，0%的背景bg，依此类推
// bg,fg,dst为单/多通道，通道数必须相同
{
	double tmp = 0;
	double pixalpha = 0;//归一化的像素alpha值
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
// alpha 为前景图fg的不透明度(0~1)，即alpha=1则为前景，alpha=0则为背景
// bg,fg,dst为单/多通道，通道数必须相同
{
	cvAddWeighted ( fg , alpha , bg , 1-alpha , 0 , dst );
}



CvRect cvRect( CvPoint leftup , CvPoint rightdown )
// 输入：	leftup左上点 ， rightdown 右下点
// 输出：	由两点确定的方形区域
{
	return cvRect( leftup.x , leftup.y , rightdown.x - leftup.x , rightdown.y - leftup.y );
}

void getMaxMinValue( IplImage* src, CvScalar& max_val, CvScalar& min_val )
//  max_val , min_val 分别存放每个通道的最大值和最小值，若该通道不存在，其值为-1
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
// 输入： src要填充的图像， color 填充的颜色
// 输出： src
{
	cvZero( src );
	cvAddS( src, color, src );
}

void selAdd( IplImage* sel0, IplImage* sel1, IplImage* dst)
// 输入: sel0,sel1为单/多通道图像， 0为未选中， 非0 为选中，
// 输出： dst是与sel0，sel1相同的图像， 只有0或255两个值，0表示未选中，255表示选中
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
// 输入: sel0,sel1为单/多通道图像， 0为未选中， 非0 为选中，
// 输出： dst是与sel0，sel1相同的图像， 只有0或255两个值，0表示未选中，255表示选中
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
// 输入: sel0,sel1为单/多通道图像， 0为未选中， 非0 为选中，
// 输出： dst是与sel0，sel1相同的图像， 只有0或255两个值，0表示未选中，255表示选中
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
{//src为三通道 ， alpha为单通道， 返回一个四通道的图像dst  , src ,alpha , dst的图像大小一致
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
	// 实际区域较暗，亮盖暗，柔光
	{
		fillImage( baseimage, cvScalar(128,128,128) );
		imageCombine( baseimage, purecolor, sel, purecolor );
		softLightCombine( src, purecolor, dst );
	}
	else
	// 实际区域较亮，暗盖亮，正片叠底
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
	IplImage* sel = cvCreateImage( cvGetSize( image ), image->depth, 1 );//被覆盖的选区
	IplImage* midtone = cvCreateImage( cvGetSize( image ), image->depth, 1 );
	IplImage* shadow = cvCreateImage( cvGetSize( image ), image->depth, 1 );
	IplImage* highlight = cvCreateImage( cvGetSize( image ), image->depth, 1 );
	IplImage* rcn = cvCreateImage( cvGetSize( image ), image->depth, 1 );
	CvScalar refcolor =cvScalar( 0 );//作为参考的标准颜色
	CvScalar selcolor = cvScalar( 0 );//被覆盖选区的实际颜色
	double radius = 0; //羽化半径
	
	///=========初始化各部分选区============//
	cvSplit( image, 0, 0, rcn, 0 );
	createMagicwand( rcn, 0, low, shadow );
	createMagicwand( rcn, low, high, midtone );
	createMagicwand( rcn, high, 255, highlight );
	selAnd( shadow, skin, shadow );
	selAnd( midtone, skin, midtone );
	selAnd( highlight, skin, highlight );

	//===========覆盖暗部============//
	//设置选区
	getAvrBri( image, refcolor, highlight );
	refcolor.val[3] = getBri( refcolor.val[2], refcolor.val[1], refcolor.val[0] );
	selAdd( shadow, midtone, sel );//设置被覆盖的选区
	getAvrBri( image, selcolor, sel );
	selcolor.val[3] = getBri( selcolor.val[2], selcolor.val[1], selcolor.val[0] );

	radius = floor( 20 + 2* log10(cvSum( sel ).val[0] / 255) ); //羽化选区
	selFeathering( sel, sel, radius );
	faceLightBalance( image, sel, refcolor, selcolor, dst );//肤色平衡

	//===========覆盖亮部============//
	getAvrBri( image, refcolor, shadow );
	refcolor.val[3] = getBri( refcolor.val[2], refcolor.val[1], refcolor.val[0] );
	selAdd( midtone, highlight, sel );
	getAvrBri( dst, selcolor, sel );// 调整前 or 调整后
	selcolor.val[3] = getBri( selcolor.val[2], selcolor.val[1], selcolor.val[0] );
	radius = floor( 5 + pow( log10((cvSum( sel ).val[0] / 255)), 3 ) * 0.5);
	selFeathering( sel, sel, radius );
	faceLightBalance( dst, sel, refcolor, selcolor, dst );
}
