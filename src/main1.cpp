#include <opencv/cv.h>
#include <opencv/highgui.h>
extern "C"
{
#include <opencv2/core/core_c.h>
}
#include <stdlib.h>
#include <stdio.h>

using namespace cv;
////////////////////////
CvCapture* g_capture = NULL;

void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step,
                    double scale, const Scalar& color)
{
    double xx=0,yy=0;
    int x,y;

    for(y = 0; y < cflowmap.rows; y += step)
        for(x = 0; x < cflowmap.cols; x += step)
        {
            const Point2f& fxy = flow.at<Point2f>(y, x);
            //Ради теста здесь суммируем все вектора
            xx+=fxy.x;
            yy+=fxy.y;
            line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),
                    color);
            circle(cflowmap, Point(x,y), 2, color, -1);
        }
    xx/=x*y;
    yy/=x*y;
}

int main()
{
    cvNamedWindow("Example", CV_WINDOW_AUTOSIZE );
    g_capture = cvCaptureFromCAM(0);

    //Вход в цикл проигрывания фильма
    int i=0;

    Mat prevgray, gray, flow, cflow;
    IplImage* frame=0;
    IplImage* frame2=0;
    while(1) {
        //Получение фрейма
        frame = cvQueryFrame( g_capture );
        if( !frame ) break;         

        cvShowImage( "Example", frame );
        Mat frame1(frame);
        /*
        if (frame2)
        {
            //Уменьшаем размеры изображения для убыстрения фильтра
            CvSize c=cvGetSize(frame);
            c.height/=2;
            c.width/=2;
            frame2=cvCreateImage(c, 8, 3);
        }

        cvResize(frame,frame2,0);
        Mat frame3(frame2);
        */
        cvtColor(frame1, gray, CV_BGR2GRAY);
        if( prevgray.data )
        {
            calcOpticalFlowFarneback(prevgray, gray, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
            cvtColor(prevgray, cflow, CV_GRAY2BGR);
            drawOptFlowMap(flow, cflow, 16, 1.5, CV_RGB(0, 255, 0));
            imshow("flow", cflow);
        }

        //Ожидание нажатия ESCAPE
        char c = cvWaitKey(10);
        if( c == 27 ) break;
        i++;
        //onTrackbarSlide(i);
        std::swap(prevgray, gray);
    }

    cvReleaseCapture( &g_capture );
    cvDestroyWindow( "Example" );
    return 0;
}
