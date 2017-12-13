//=================================================================
//Filename		:	FLBui.cpp
//Description	:	界面处理的相关函数
//Date			:	2012/08/08 
//=================================================================

#include "FLBui.h"

//输出图像信息
void printImageInfo ( const IplImage* src )
{
	cout<<"******************************"<<endl;
	cout<<"The information of image"<<endl<<endl;
	cout<<"Size:"<<'\t'<<'\t'<<src->nSize<<endl;		//这个结构体的大小
	cout<<"ID:"<<'\t'<<'\t'<<src->ID<<endl;				//版本
	cout<<"Channels:"<<'\t'<<src->nChannels<<endl;		//图像通道数
	cout<<"Dataorder："<<'\t'<<src->dataOrder<<endl;	//交叉存取颜色通道
	cout<<"Original："<<'\t'<<src->origin<<endl;		//图像数据分布结果
	cout<<"Depth:"<<'\t'<<'\t'<<src->depth<<endl;		//图像色深
	cout<<"Width"<<'\t'<<'\t'<<src->width<<endl;		//图像宽度像素
	cout<<"Height"<<'\t'<<'\t'<<src->height<<endl;		//图像高度像素
	cout<<"ROI:"<<'\t'<<'\t'<<src->roi<<endl;			//指定ROI区域指针
	cout<<"WidthStep"<<'\t'<<src->widthStep<<endl;		//排列的图像行大小
	cout<<"ImageSize"<<'\t'<<src->imageSize<<endl;		//图像数据大小
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
//显示图片
{
	if ( !img )	return;
	int actwidth = WIN_WIDTH;//窗口的实际大小
	int actheight = WIN_HEIGHT;
	if ( (img->width) < actwidth && (img->height) < actheight )
	{//若图片不超出预定义的窗口大小
		cvShowImage( winname, img );
		return;
	}
	CvRect roi;
	static void* userdata[] = { winname, img };
	actwidth = min ( img->width , actwidth );
	actheight = min ( img->height , actheight );// refresh width and height ;
	if ( !img->roi )// 图像原来没有ROI
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
	char* winname = (char*) userdata[0];		//第一个指针为窗口名称指针
	IplImage* image = (IplImage*) userdata[1];		//第二个指针为图像指针
	cvShowImage( winname, image );
}