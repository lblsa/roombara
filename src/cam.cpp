#include <opencv/cv.h>
#include <opencv/highgui.h>
extern "C"
{
#include <opencv2/core/core_c.h>
}
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
        // получаем любую подключённую камеру
        CvCapture* capture = cvCreateCameraCapture(CV_CAP_ANY); //cvCaptureFromCAM( 0 );
        assert( capture );

        //cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 640);//1280); 
        //cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 480);//960); 

        // узнаем ширину и высоту кадра
        double width = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
        double height = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
        printf("[i] %.0f x %.0f\n", width, height );

        IplImage* frame=0;

        cvNamedWindow("capture", CV_WINDOW_AUTOSIZE);

        printf("[i] press Enter for capture image and Esc for quit!\n\n");

        int counter=0;
        char filename[512];

        char text_buf[32];
        CvFont myFont;
//        cvInitFont(&myFont, СV_FONT_HERSHEY_PLAIN, 0.7, 1, 0, 1, 8);
        cvInitFont(&myFont, CV_FONT_HERSHEY_PLAIN, 2, 1, 0, 1, 8);

        while(true){
                // получаем кадр
                frame = cvQueryFrame( capture );

                //Filters
                cvtColor(frame, pic, CV_BGR2HSV);
                GaussianBlur(pic, pic, Size(7,7), 1.5, 1.5);

                sprintf(text_buf,"Hello!!!");
                cvPutText(frame, text_buf, cvPoint(5,15), &myFont, cvScalar(0,0,255));

                // показываем
                cvShowImage("capture", frame);
        
                char c = cvWaitKey(33);
                if (c == 27) { // нажата ESC
                        break;
                }
                else if(c == 13) { // Enter
                        // сохраняем кадр в файл
                        sprintf(filename, "Image%d.jpg", counter);
                        printf("[i] capture... %s\n", filename);
                        cvSaveImage(filename, frame);
                        counter++;
                }
        }
        // освобождаем ресурсы
        cvReleaseCapture( &capture );
        cvDestroyWindow("capture");
        return 0;
}

