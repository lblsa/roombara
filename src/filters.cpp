// http://singhgaganpreet.com/tag/cvboundingrect-example/
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

int g_switch_value = 0;
int filterInt = 0;
int lastfilterInt = -1;

void switch_callback( int position ){
    filterInt = position;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <img.jpg>\n", argv[0]);
        return 1;
    }

    const char* name = "Filters Window";
    IplImage* img = cvLoadImage( argv[1] );
    IplImage* out = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 3 );

    cvNamedWindow( name, 1 );
    cvShowImage(name, out);
    
    // Other Variables
    CvPoint seed_point = cvPoint(305,195);
    CvScalar color = CV_RGB(250,0,0);

    // Create trackbar
    cvCreateTrackbar( "FILTER", name, &g_switch_value, 5, switch_callback );

    while( 1 ) {
        switch( filterInt ){
            case 0:
                cvSmooth( img, out, CV_BLUR, 7, 7 );
                break;
            case 1:
                cvSmooth( img, out, CV_GAUSSIAN, 7, 7 );
                break;
            case 2:
                cvSmooth( img, out, CV_MEDIAN, 7, 7 );
                break;
            case 3:
                cvErode( img, out, NULL, 1);
                break;
            case 4:
                cvDilate( img, out, NULL, 1);
                break;
            case 5:
                cvFloodFill( out, seed_point, color, cvScalarAll(5.0), cvScalarAll(5.0), NULL, 4, NULL );
                break;
        }
        if(filterInt != lastfilterInt){
            cvShowImage(name, out);
            lastfilterInt = filterInt;
        }
        if( cvWaitKey( 15 ) == 27 ) 
            break;
    }

    cvReleaseImage( &img );
    cvReleaseImage( &out );
    cvDestroyWindow( name );

    return 0;
}
