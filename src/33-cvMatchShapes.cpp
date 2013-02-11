//
// пример использование cvMatchShapes()
// сравнение объектов по моментам их контуров 
//
// http://robocraft.ru
//

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdlib.h>
#include <stdio.h>

// сравнение объектов по моментам их контуров 
void testMatch(IplImage* original, IplImage* templ)
{
	assert(original!=0);
	assert(templ!=0);

	printf("[i] test cvMatchShapes()\n");

	IplImage *src=0, *dst=0;

	src=cvCloneImage(original);

	IplImage* binI = cvCreateImage( cvGetSize(src), 8, 1);
	IplImage* binT = cvCreateImage( cvGetSize(templ), 8, 1);

	// заведём цветные картинки
	IplImage* rgb = cvCreateImage(cvGetSize(original), 8, 3);
	cvConvertImage(src, rgb, CV_GRAY2BGR);
	IplImage* rgbT = cvCreateImage(cvGetSize(templ), 8, 3);
	cvConvertImage(templ, rgbT, CV_GRAY2BGR);

	// получаем границы изображения и шаблона
	cvCanny(src, binI, 50, 200);
	cvCanny(templ, binT, 50, 200);

	// показываем
	cvNamedWindow( "cannyI", 1 );
	cvShowImage( "cannyI", binI);

	cvNamedWindow( "cannyT", 1 );
	cvShowImage( "cannyT", binT);

	// для хранения контуров
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contoursI=0, *contoursT=0;

	// находим контуры изображения
	int contoursCont = cvFindContours( binI, storage, &contoursI, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));

	// для отметки контуров
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1.0, 1.0);
	char buf[128];
	int counter=0;

	// нарисуем контуры изображения
	if(contoursI!=0){
		for(CvSeq* seq0 = contoursI;seq0!=0;seq0 = seq0->h_next){
			// рисуем контур
			cvDrawContours(rgb, seq0, CV_RGB(255,216,0), CV_RGB(0,0,250), 0, 1, 8); 
			// выводим его номер
			//CvPoint2D32f point; float rad;
			//cvMinEnclosingCircle(seq0,&point,&rad); // получим окружность содержащую контур
			//cvPutText(rgb, itoa(++counter, buf, 10), cvPointFrom32f(point), &font, CV_RGB(0,255,0));
		}
	}
	// показываем
	cvNamedWindow( "cont", 1 );
	cvShowImage( "cont", rgb );

	cvConvertImage(src, rgb, CV_GRAY2BGR);

	// находим контуры шаблона
	cvFindContours( binT, storage, &contoursT, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));

	CvSeq* seqT=0;
	double perimT = 0;

	if(contoursT!=0){
		// находим самый длинный контур 
		for(CvSeq* seq0 = contoursT;seq0!=0;seq0 = seq0->h_next){
			double perim = cvContourPerimeter(seq0);
			if(perim>perimT){
				perimT = perim;
				seqT = seq0;
			}
			// рисуем
			cvDrawContours(rgbT, seq0, CV_RGB(255,216,0), CV_RGB(0,0,250), 0, 1, 8); // рисуем контур
		}
	}
	// покажем контур шаблона
	cvDrawContours(rgbT, seqT, CV_RGB(52,201,36), CV_RGB(36,201,197), 0, 2, 8); // рисуем контур
	cvNamedWindow( "contT", 1 );
	cvShowImage( "contT", rgbT );


	CvSeq* seqM=0;
	double matchM=1000;
	// обходим контуры изображения 
	counter=0;
	if(contoursI!=0){
		// поиск лучшего совпадения контуров по их моментам 
		for(CvSeq* seq0 = contoursI;seq0!=0;seq0 = seq0->h_next){
			double match0 = cvMatchShapes(seq0, seqT, CV_CONTOURS_MATCH_I3);
			if(match0<matchM){
				matchM = match0;
				seqM = seq0;
			}
			printf("[i] %d match: %.2f\n", ++counter, match0);
		}
	}
	// рисуем найденный контур
	cvDrawContours(rgb, seqM, CV_RGB(52,201,36), CV_RGB(36,201,197), 0, 2, 8); // рисуем контур

	cvNamedWindow( "find", 1 );
	cvShowImage( "find", rgb );

	// ждём нажатия клавиши
	cvWaitKey(0);

	// освобождаем ресурсы
	cvReleaseMemStorage(&storage);

	cvReleaseImage(&src);
	cvReleaseImage(&dst);
	cvReleaseImage(&rgb);
	cvReleaseImage(&rgbT);
	cvReleaseImage(&binI);
	cvReleaseImage(&binT);

	// удаляем окна
	cvDestroyAllWindows();
}

int main(int argc, char* argv[])
{
	IplImage *original=0, *templ=0;

	// имя картинки задаётся первым параметром
	const char* filename = argc >= 2 ? argv[1] : "Image0.jpg";
	// получаем картинку
	original = cvLoadImage(filename, 0);

	printf("[i] image: %s\n", filename);
	assert( original != 0 );

	// имя шаблона задаётся вторым параметром
	const char* filename2 = argc >= 3 ? argv[2] : "templ.bmp";
	// получаем картинку
	templ = cvLoadImage(filename2, 0);

	printf("[i] template: %s\n", filename2);
	assert( templ != 0 );

	// покажем изображения
	cvNamedWindow( "original", 1 );
	cvShowImage( "original", original );
	cvNamedWindow( "template", 1 );
	cvShowImage( "template", templ );

	// сравнение
	testMatch(original, templ);

	// освобождаем ресурсы
	cvReleaseImage(&original);
	cvReleaseImage(&templ);
	// удаляем окна
	cvDestroyAllWindows();
	return 0;
}
