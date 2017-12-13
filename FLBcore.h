//===================================================================
//Filename		:	FLBcore.h
//Description	:	��opencv��ͼ������صĺ��ĵĺ���
//		����6�����֣�
//			1.���ȴ�����
//			2.ͼ��ϲ�������ͼ����ģʽ����
//			3.ͼ��ѡ������ش���
//			4.opencv��һЩ�����������
//			5.ͼ�����һЩ��������
//			6.����������ĺ��ĺ���
//Date			:	2012/08/08
//===================================================================
#pragma once

#include "FLBstd.h"
#include "FLBmagicwand.h"// call in function autoFLB

//===================���ȴ�����===================//
#ifndef GETBRIGHTNESS_FUNCTION
#define GETBRIGHTNESS_FUNCTION
// get Brightness
#define getBri( r, g, b )         0.299 * (r) + 0.587 * (g) + 0.114 * (b)
#endif
// ������������ĳһ����ɫֵcolorvalue 
// dst(mask) = src(mask) + value
void addBri( const IplImage* src , IplImage * dst , CvScalar colorvalue , IplImage* mask=0 );
// ������������ĳһ���Ҷ�ֵgrayvalue 
// dst(mask) = src(mask) + value
void addBri( const IplImage* src , IplImage * dst , int grayvalue , IplImage* mask=0 );
// ����ͼ���ƽ������
void getAvrBri ( const IplImage* src, CvScalar& avrbri, IplImage* mask=0 );

//////////////ɫ�׵�������//////////////
// �Ե�ͨ�����ͨ��ͼ�����ɫ�׵�������ͨ��ʱ������ͨ���Ĳ�������ͬ��
void colorLevel( const IplImage* src, IplImage* dst, int low=0, int high=255, double mid=1 );
// �Զ�ͨ��ͼ�����ɫ�׵�����ÿ��ͨ���Ĳ������Ե�������
void colorLevel3C( const IplImage * src ,IplImage * dst ,CvScalar low ,CvScalar high ,CvScalar mid );


//===================ͼ��ϲ�������ͼ����ģʽ����===================//
// ���ģʽ���㹫ʽ
double calcSoftLight ( double basiccolor , double mixcolor );
// ���ģʽ���
void softLightCombine( IplImage* bg , IplImage* fg , IplImage* dst );
// ��Ƭ����ģʽ���
void multiplyCombine( IplImage* bg , IplImage* fg , IplImage* dst );
//ͼ��ϲ�����
//ͼ����ݲ�͸����alpha�ϲ���dst = fg * alpha + bg * (1-alpha)
//ʽ���еĳ˷���Ԫ�ض�Ӧ���
void imageCombine( IplImage* bg , IplImage* fg , IplImage* alpha , IplImage* dst );
void imageCombine( IplImage* bg , IplImage* fg , double alpha , IplImage* dst );


//===================ͼ��ѡ����selection������ش���===================//
//ѡ��sel�ǵ�ͨ����ֵ�ĻҶ�ͼ����Χ��0~255��0��ʾ�����ص�ûѡ�У���0��ʾѡ�и����ص�
//��ֵ�Ĵ�С��ʾ͸���ȣ�0��ʾȫ͸���� 255��ʾ��ȫ��͸��
//ѡ�����
void selAdd( IplImage* sel0, IplImage* sel1, IplImage* dst);
//ѡ�����
void selSub( IplImage* sel0, IplImage* sel1, IplImage* dst);
//ѡ����
void selAnd( IplImage* sel0, IplImage* sel1, IplImage* dst);
//ѡ����
void selFeathering( IplImage* sel, IplImage* dst, double r );


//===================opencv��һЩ�����������===================//
//cvRect�������أ������ϵ�����µ�ȷ������
CvRect cvRect( CvPoint leftup , CvPoint rightdown );
//CvScalar ������㺯��
//����CvScalar��ֵ
void setScalarValue( CvScalar& scalar , double val0, double val1=0, double val2=0, double val3=0 );
void scalarAdd( CvScalar& scalar0, CvScalar& scalar1, CvScalar& dst );
void scalarSub( CvScalar& scalar0, CvScalar& scalar1, CvScalar& dst );
void scalarMul( CvScalar& scalar0, CvScalar& scalar1, CvScalar& dst );
void scalarDiv( CvScalar& scalar0, CvScalar& scalar1, CvScalar& dst );
//��cvRect��ֵ
void setCvRect( CvRect* rect, int x, int y, int width, int height );


//===================ͼ�����һЩ��������===================//
//��ȡͼ������ֵ����Сֵ
void getMaxMinValue( IplImage* src, CvScalar& max_val, CvScalar& min_val );
//ͼ���ֵ����Image binarization��
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
//ͼ����䣬������ͼ������һ����ɫΪcolor��ɫ��
void fillImage( IplImage* src, const CvScalar& color );
//Ϊ��ͨ����ͼ�����һ��alphaͨ����ת����ͨ����ͼ��
void addAlpha ( IplImage * src , IplImage * alpha , IplImage * dst );


//===================����������ĺ��ĺ���===================//
void faceLightBalance( IplImage* src, IplImage* sel, CvScalar color, CvScalar selcolor, IplImage* dst );
void getAlphaImage( IplImage* fg, IplImage* bg, IplImage* alpha, double difbri );
void autoFLB( IplImage* image, IplImage* dst, IplImage* skin, double low, double high );