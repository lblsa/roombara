/*
 * main.cpp
 *
 *  Created on: 06.02.2013
 *      Author: Tatyana
 */

/*
 * main.cpp
 *
 *  Created on: 29.01.2013
 *      Author: Tatyana
 */

#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
        // �������� ����� ������������ ������
        CvCapture* capture = cvCreateCameraCapture(CV_CAP_ANY); //cvCaptureFromCAM( 0 );
        assert( capture );

        //cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 640);//1280);
        //cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 480);//960);

        // ������ ������ � ������ �����
        double width = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
        double height = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
        printf("[i] %.0f x %.0f\n", width, height );

        IplImage* frame=0;

        cvNamedWindow("capture", CV_WINDOW_AUTOSIZE);

        printf("[i] press Enter for capture image and Esc for quit!\n\n");

        int counter=0;
        char filename[512];

        while(true){
                // �������� ����
                frame = cvQueryFrame( capture );

                // ����������
                cvShowImage("capture", frame);

                char c = cvWaitKey(33);
                if (c == 27) { // ������ ESC
                        break;
                }
                else if(c == 13) { // Enter
                        // ��������� ���� � ����
                        sprintf(filename, "Image%d.jpg", counter);
                        printf("[i] capture... %s\n", filename);
                        cvSaveImage(filename, frame);
                        counter++;
                }
        }
        // ����������� �������
        cvReleaseCapture( &capture );
        cvDestroyWindow("capture");
        return 0;
}
