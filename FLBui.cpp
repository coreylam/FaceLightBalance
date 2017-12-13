//=================================================================
//Filename		:	FLBui.cpp
//Description	:	���洦�����غ���
//Date			:	2012/08/08 
//=================================================================

#include "FLBui.h"

//���ͼ����Ϣ
void printImageInfo ( const IplImage* src )
{
	cout<<"******************************"<<endl;
	cout<<"The information of image"<<endl<<endl;
	cout<<"Size:"<<'\t'<<'\t'<<src->nSize<<endl;		//����ṹ��Ĵ�С
	cout<<"ID:"<<'\t'<<'\t'<<src->ID<<endl;				//�汾
	cout<<"Channels:"<<'\t'<<src->nChannels<<endl;		//ͼ��ͨ����
	cout<<"Dataorder��"<<'\t'<<src->dataOrder<<endl;	//�����ȡ��ɫͨ��
	cout<<"Original��"<<'\t'<<src->origin<<endl;		//ͼ�����ݷֲ����
	cout<<"Depth:"<<'\t'<<'\t'<<src->depth<<endl;		//ͼ��ɫ��
	cout<<"Width"<<'\t'<<'\t'<<src->width<<endl;		//ͼ��������
	cout<<"Height"<<'\t'<<'\t'<<src->height<<endl;		//ͼ��߶�����
	cout<<"ROI:"<<'\t'<<'\t'<<src->roi<<endl;			//ָ��ROI����ָ��
	cout<<"WidthStep"<<'\t'<<src->widthStep<<endl;		//���е�ͼ���д�С
	cout<<"ImageSize"<<'\t'<<src->imageSize<<endl;		//ͼ�����ݴ�С
	cout<<"******************************"<<endl;
	return ;
}

void printScalar( CvScalar& scalar )
{
	cout<<" val0 = " << scalar.val[0] <<'\t';
	cout<<" val1 = " << scalar.val[1] <<endl;
	cout<<" val2 = " << scalar.val[2] <<'\t';
	cout<<" val3 = " << scalar.val[3] <<endl;
}

void showImage( char* winname, IplImage* img )
//��ʾͼƬ
{
	if ( !img )	return;
	int actwidth = WIN_WIDTH;//���ڵ�ʵ�ʴ�С
	int actheight = WIN_HEIGHT;
	if ( (img->width) < actwidth && (img->height) < actheight )
	{//��ͼƬ������Ԥ����Ĵ��ڴ�С
		cvShowImage( winname, img );
		return;
	}
	CvRect roi;
	static void* userdata[] = { winname, img };
	actwidth = min ( img->width , actwidth );
	actheight = min ( img->height , actheight );// refresh width and height ;
	if ( !img->roi )// ͼ��ԭ��û��ROI
		roi = cvRect ( 0, 0, actwidth, actheight );
	else 
		roi = cvRect ( img->roi->xOffset, img->roi->yOffset, actwidth, actheight );
	cvResetImageROI( img );
	cvSetImageROI( img , roi );
	cvShowImage( winname, img );
	if ( img->width - actwidth )
		createTrackbar ( "X" , winname , &img->roi->xOffset , img->width - actwidth , showImageCallback, userdata );
	if ( img->height - actheight )
		createTrackbar ( "Y" , winname , &img->roi->yOffset , img->height - actheight , showImageCallback,userdata );
}

void showImageCallback( int , void* data)
{
	void** userdata = (void**) data;
	char* winname = (char*) userdata[0];		//��һ��ָ��Ϊ��������ָ��
	IplImage* image = (IplImage*) userdata[1];		//�ڶ���ָ��Ϊͼ��ָ��
	cvShowImage( winname, image );
}