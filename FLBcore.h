//===================================================================
//Filename		:	FLBcore.h
//Description	:	与opencv和图像处理相关的核心的函数
//		包括6个部分：
//			1.亮度处理函数
//			2.图像合并操作与图像混合模式函数
//			3.图像选区的相关处理
//			4.opencv的一些补充基础函数
//			5.图像处理的一些基本处理
//			6.阴阳脸处理的核心函数
//Date			:	2012/08/08
//===================================================================
#pragma once

#include "FLBstd.h"
#include "FLBmagicwand.h"// call in function autoFLB

//===================亮度处理函数===================//
#ifndef GETBRIGHTNESS_FUNCTION
#define GETBRIGHTNESS_FUNCTION
// get Brightness
#define getBri( r, g, b )         0.299 * (r) + 0.587 * (g) + 0.114 * (b)
#endif
// 亮度整体增加某一个颜色值colorvalue 
// dst(mask) = src(mask) + value
void addBri( const IplImage* src , IplImage * dst , CvScalar colorvalue , IplImage* mask=0 );
// 亮度整体增加某一个灰度值grayvalue 
// dst(mask) = src(mask) + value
void addBri( const IplImage* src , IplImage * dst , int grayvalue , IplImage* mask=0 );
// 计算图像的平均亮度
void getAvrBri ( const IplImage* src, CvScalar& avrbri, IplImage* mask=0 );

//////////////色阶调整函数//////////////
// 对单通道或多通道图像进行色阶调整，多通道时，所有通道的参数是相同的
void colorLevel( const IplImage* src, IplImage* dst, int low=0, int high=255, double mid=1 );
// 对多通道图像进行色阶调整，每个通道的参数可以单独设置
void colorLevel3C( const IplImage * src ,IplImage * dst ,CvScalar low ,CvScalar high ,CvScalar mid );


//===================图像合并操作与图像混合模式函数===================//
// 柔光模式计算公式
double calcSoftLight ( double basiccolor , double mixcolor );
// 柔光模式混合
void softLightCombine( IplImage* bg , IplImage* fg , IplImage* dst );
// 正片叠底模式混合
void multiplyCombine( IplImage* bg , IplImage* fg , IplImage* dst );
//图像合并操作
//图像根据不透明度alpha合并，dst = fg * alpha + bg * (1-alpha)
//式子中的乘法是元素对应相乘
void imageCombine( IplImage* bg , IplImage* fg , IplImage* alpha , IplImage* dst );
void imageCombine( IplImage* bg , IplImage* fg , double alpha , IplImage* dst );


//===================图像选区（selection）的相关处理===================//
//选区sel是单通道且值的灰度图，范围是0~255，0表示该像素点没选中，非0表示选中该像素点
//其值的大小表示透明度，0表示全透明， 255表示完全不透明
//选区相加
void selAdd( IplImage* sel0, IplImage* sel1, IplImage* dst);
//选区相减
void selSub( IplImage* sel0, IplImage* sel1, IplImage* dst);
//选区与
void selAnd( IplImage* sel0, IplImage* sel1, IplImage* dst);
//选区羽化
void selFeathering( IplImage* sel, IplImage* dst, double r );


//===================opencv的一些补充基础函数===================//
//cvRect函数重载，用左上点和右下点确定区域
CvRect cvRect( CvPoint leftup , CvPoint rightdown );
//CvScalar 相关运算函数
//设置CvScalar的值
void setScalarValue( CvScalar& scalar , double val0, double val1=0, double val2=0, double val3=0 );
void scalarAdd( CvScalar& scalar0, CvScalar& scalar1, CvScalar& dst );
void scalarSub( CvScalar& scalar0, CvScalar& scalar1, CvScalar& dst );
void scalarMul( CvScalar& scalar0, CvScalar& scalar1, CvScalar& dst );
void scalarDiv( CvScalar& scalar0, CvScalar& scalar1, CvScalar& dst );
//设cvRect的值
void setCvRect( CvRect* rect, int x, int y, int width, int height );


//===================图像处理的一些基本处理===================//
//获取图像的最大值和最小值
void getMaxMinValue( IplImage* src, CvScalar& max_val, CvScalar& min_val );
//图像二值化（Image binarization）
#ifndef BINARIZATION_FUNCTION
#define BINARIZATION_FUNCTION
#define binarization( src, dst, threshold, max_value, threshold_type ) cvThreshold( (src), (dst), (threshold), (max_value), (threshold_type) );
///* Threshold types */
//enum
//{
//    CV_THRESH_BINARY      =0,  /* value = value > threshold ? max_value : 0       */
//    CV_THRESH_BINARY_INV  =1,  /* value = value > threshold ? 0 : max_value       */
//    CV_THRESH_TRUNC       =2,  /* value = value > threshold ? threshold : value   */
//    CV_THRESH_TOZERO      =3,  /* value = value > threshold ? value : 0           */
//    CV_THRESH_TOZERO_INV  =4,  /* value = value > threshold ? 0 : value           */
//    CV_THRESH_MASK        =7,
//    CV_THRESH_OTSU        =8  /* use Otsu algorithm to choose the optimal threshold value;
//                                 combine the flag with one of the above CV_THRESH_* values */
//};
#endif
//图像填充，将整个图像填充成一个颜色为color的色块
void fillImage( IplImage* src, const CvScalar& color );
//为三通道的图像添加一个alpha通道，转成四通道的图像
void addAlpha ( IplImage * src , IplImage * alpha , IplImage * dst );


//===================阴阳脸处理的核心函数===================//
void faceLightBalance( IplImage* src, IplImage* sel, CvScalar color, CvScalar selcolor, IplImage* dst );
void getAlphaImage( IplImage* fg, IplImage* bg, IplImage* alpha, double difbri );
void autoFLB( IplImage* image, IplImage* dst, IplImage* skin, double low, double high );