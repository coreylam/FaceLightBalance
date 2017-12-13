//=================================================================
//Filename		:	FLBmain.cpp
//Description	:	���̵�������
//Date			:	2012/08/08 
//=================================================================
#include "FLBinclude.h"
#include "FLBcore.h"
#include "FLBui.h"
#include "FLBmagicwand.h"

int main( int argc, char* argv[] ){

	//����ͼ��
	if ( !argv[1] )
		return -1;
	char* imagepath = argv[1];
	char* imagedir = (char*) malloc( strlen(imagepath) );
	char* imagename = (char*) malloc( strlen(imagepath) );
	separatePath( imagepath, imagedir, imagename );
	// ����Ƥ��
	char* skindir = cstrcat( imagedir, "skin\\" );
	char* savedir = cstrcat ( imagedir, "save\\" );
	char* strimgID;
	char* skinpath;
	
	for ( int imgID=1 ; imgID < 22 ; imgID++ )
	{
		strimgID = int2str( imgID );
		imagepath = cstrcat( imagedir, strimgID, ".jpg" );
		skinpath = cstrcat( skindir, strimgID, ".jpg" );
		IplImage* image = cvLoadImage( imagepath , 1 );
		IplImage* skin = cvLoadImage( skinpath , 0 );
		if ( !image || !skin)
			return -1;
		//�����Զ�����������
		autoFLB( image, image, skin, 140, 220 );
		cvSaveImage( cstrcat( savedir, strimgID, ".jpg" ), image, 0 );
		//cout << "ͼƬ����·����" << cstrcat( savedir, strimgID, ".jpg" ) << endl;
	}
	
	//IplImage* invimg = cvCloneImage( image );
	//
	//IplImage* sel = cvCreateImage( cvGetSize( image ), image->depth, 1 );
	//IplImage* midtone = cvCreateImage( cvGetSize( image ), image->depth, 1 );
	//IplImage* shadow = cvCreateImage( cvGetSize( image ), image->depth, 1 );
	//IplImage* highlight = cvCreateImage( cvGetSize( image ), image->depth, 1 );
	//IplImage* r = cvCreateImage( cvGetSize( image ), image->depth, 1 );
	//cvSplit( image, 0, 0, r, 0 );
	//double low = 140;
	//double high = 230;
	//createMagicwand( r, 0, low, shadow );
	//createMagicwand( r, low, high, midtone );
	//createMagicwand( r, high, 255, highlight );
	//selAnd( shadow, skin, shadow );
	//selAnd( midtone, skin, midtone );
	//selAnd( highlight, skin, highlight );
	////cvNot( image, invimg );
	//CvScalar color =cvScalar( 0 );
	//CvScalar selcolor = cvScalar( 0 );

	//getAvrBri( image, color, highlight );
	//color.val[3] = getBri( color.val[2], color.val[1], color.val[0] );
	//cout << "�߹�����ԭ���ȣ�"<< endl;
	//printScalar( color );
	//selAdd( shadow, shadow, sel );
	////selAdd( shadow, midtone, sel );
	//getAvrBri( image, selcolor, sel );
	//cout <<"���ص�����"<< (cvSum( sel ).val[0] / 255) << endl;
	//double rad = floor( 20 + 2* log10(cvSum( sel ).val[0] / 255) );
	////double rad = floor(10 + 10* log10(cvSum( sel ).val[0] / 255));
	//selFeathering( sel, sel, rad );
	//cout <<"�𻯰뾶��"<< rad << endl;
	////showImage( "org", image );
	//faceLightBalance( image, sel, color, image );


	//getAvrBri( image, color, shadow );
	//color.val[3] = getBri( color.val[2], color.val[1], color.val[0] );
	//cout << "��Ӱ����ı������ȣ�"<< endl;
	//printScalar( color );
	//selAdd( highlight, midtone, sel );
	//cout <<"���ص�����"<< (cvSum( sel ).val[0] / 255) << endl;
	//rad = floor( 5 + pow( log10((cvSum( sel ).val[0] / 255)), 3 ) * 0.5);
	//cout <<"�𻯰뾶��"<< rad << endl;
	//selFeathering( sel, sel, rad );
	//faceLightBalance( image, sel, color, image );
	//getAvrBri( image, color, midtone );
	//color.val[3] = getBri( color.val[2], color.val[1], color.val[0] );
	//cout << "�м�����ı������ȣ�"<< endl;
	//printScalar( color );
	//getAvrBri( image, color, highlight );
	//color.val[3] = getBri( color.val[2], color.val[1], color.val[0] );
	//cout << "�߹�����ı������ȣ�"<< endl;
	//printScalar( color );
	//selFeathering( midtone, midtone, 20 );
	//showImage( "gray", midtone );
	//showImage( "shadow", shadow );
	//showImage( "highlight", highlight );
	//imageCombine( image, invimg, midtone, image );
	//showImage( filename, image );
	cvWaitKey(); 
	
	//getchar();
	return 0;
}