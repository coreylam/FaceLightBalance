//=================================================================
//Filename		:	FLBmagicwand.h
//Description	:	ħ��ѡ������غ���
//Date			:	2012/08/08 
//=================================================================

#pragma once 
#include "FLBinclude.h"
#include "FLBcore.h"

enum{ 
	FLB_BOUND		=0, /*ָ�����±߽�  */
	FLB_TOLERANCE	=1  /*ָ����ɫ���ݲ�*/
};

////���ݲο���ɫ���ݲ�ֵ����ѡ��////
//���ݵ��λ�ú��ݲ�ֵȷ��ѡ��
void createMagicwand( const IplImage* src, const CvPoint& point, const CvScalar& tolerance, IplImage* dst );
//��ͨ��ͼ�񣬸������±߽�ֵȷ����ѡ��,��Χ��[low,up]
void createMagicwand( const IplImage* src, const double low, const double up, IplImage* dst );
// ��ͨ��ͼ�񴴽�ѡ�����߽�/�ݲ�
void createMagicwand( const IplImage* src, CvScalar scalar0, CvScalar scalar1, IplImage* dst, int select_type = FLB_BOUND );