//
// ������ ������������� cvMatchShapes()
// ��������� �������� �� �������� �� �������� 
//
// http://robocraft.ru
//

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdlib.h>
#include <stdio.h>

// ��������� �������� �� �������� �� �������� 
void testMatch(IplImage* original, IplImage* templ)
{
	assert(original!=0);
	assert(templ!=0);

	printf("[i] test cvMatchShapes()\n");

	IplImage *src=0, *dst=0;

	src=cvCloneImage(original);

	IplImage* binI = cvCreateImage( cvGetSize(src), 8, 1);
	IplImage* binT = cvCreateImage( cvGetSize(templ), 8, 1);

	// ������ ������� ��������
	IplImage* rgb = cvCreateImage(cvGetSize(original), 8, 3);
	cvConvertImage(src, rgb, CV_GRAY2BGR);
	IplImage* rgbT = cvCreateImage(cvGetSize(templ), 8, 3);
	cvConvertImage(templ, rgbT, CV_GRAY2BGR);

	// �������� ������� ����������� � �������
	cvCanny(src, binI, 50, 200);
	cvCanny(templ, binT, 50, 200);

	// ����������
	cvNamedWindow( "cannyI", 1 );
	cvShowImage( "cannyI", binI);

	cvNamedWindow( "cannyT", 1 );
	cvShowImage( "cannyT", binT);

	// ��� �������� ��������
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contoursI=0, *contoursT=0;

	// ������� ������� �����������
	int contoursCont = cvFindContours( binI, storage, &contoursI, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));

	// ��� ������� ��������
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1.0, 1.0);
	char buf[128];
	int counter=0;

	// �������� ������� �����������
	if(contoursI!=0){
		for(CvSeq* seq0 = contoursI;seq0!=0;seq0 = seq0->h_next){
			// ������ ������
			cvDrawContours(rgb, seq0, CV_RGB(255,216,0), CV_RGB(0,0,250), 0, 1, 8); 
			// ������� ��� �����
			//CvPoint2D32f point; float rad;
			//cvMinEnclosingCircle(seq0,&point,&rad); // ������� ���������� ���������� ������
			//cvPutText(rgb, itoa(++counter, buf, 10), cvPointFrom32f(point), &font, CV_RGB(0,255,0));
		}
	}
	// ����������
	cvNamedWindow( "cont", 1 );
	cvShowImage( "cont", rgb );

	cvConvertImage(src, rgb, CV_GRAY2BGR);

	// ������� ������� �������
	cvFindContours( binT, storage, &contoursT, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));

	CvSeq* seqT=0;
	double perimT = 0;

	if(contoursT!=0){
		// ������� ����� ������� ������ 
		for(CvSeq* seq0 = contoursT;seq0!=0;seq0 = seq0->h_next){
			double perim = cvContourPerimeter(seq0);
			if(perim>perimT){
				perimT = perim;
				seqT = seq0;
			}
			// ������
			cvDrawContours(rgbT, seq0, CV_RGB(255,216,0), CV_RGB(0,0,250), 0, 1, 8); // ������ ������
		}
	}
	// ������� ������ �������
	cvDrawContours(rgbT, seqT, CV_RGB(52,201,36), CV_RGB(36,201,197), 0, 2, 8); // ������ ������
	cvNamedWindow( "contT", 1 );
	cvShowImage( "contT", rgbT );


	CvSeq* seqM=0;
	double matchM=1000;
	// ������� ������� ����������� 
	counter=0;
	if(contoursI!=0){
		// ����� ������� ���������� �������� �� �� �������� 
		for(CvSeq* seq0 = contoursI;seq0!=0;seq0 = seq0->h_next){
			double match0 = cvMatchShapes(seq0, seqT, CV_CONTOURS_MATCH_I3);
			if(match0<matchM){
				matchM = match0;
				seqM = seq0;
			}
			printf("[i] %d match: %.2f\n", ++counter, match0);
		}
	}
	// ������ ��������� ������
	cvDrawContours(rgb, seqM, CV_RGB(52,201,36), CV_RGB(36,201,197), 0, 2, 8); // ������ ������

	cvNamedWindow( "find", 1 );
	cvShowImage( "find", rgb );

	// ��� ������� �������
	cvWaitKey(0);

	// ����������� �������
	cvReleaseMemStorage(&storage);

	cvReleaseImage(&src);
	cvReleaseImage(&dst);
	cvReleaseImage(&rgb);
	cvReleaseImage(&rgbT);
	cvReleaseImage(&binI);
	cvReleaseImage(&binT);

	// ������� ����
	cvDestroyAllWindows();
}

int main(int argc, char* argv[])
{
	IplImage *original=0, *templ=0;

	// ��� �������� ������� ������ ����������
	const char* filename = argc >= 2 ? argv[1] : "Image0.jpg";
	// �������� ��������
	original = cvLoadImage(filename, 0);

	printf("[i] image: %s\n", filename);
	assert( original != 0 );

	// ��� ������� ������� ������ ����������
	const char* filename2 = argc >= 3 ? argv[2] : "templ.bmp";
	// �������� ��������
	templ = cvLoadImage(filename2, 0);

	printf("[i] template: %s\n", filename2);
	assert( templ != 0 );

	// ������� �����������
	cvNamedWindow( "original", 1 );
	cvShowImage( "original", original );
	cvNamedWindow( "template", 1 );
	cvShowImage( "template", templ );

	// ���������
	testMatch(original, templ);

	// ����������� �������
	cvReleaseImage(&original);
	cvReleaseImage(&templ);
	// ������� ����
	cvDestroyAllWindows();
	return 0;
}
