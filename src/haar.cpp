/*****************************************************************   
* This program uses function histogram() in Ch
*****************************************************************/
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <img.jpg>\n", argv[0]);
        return 1;
    }

    IplImage* img;
    img = cvLoadImage( argv[1] );
    CvMemStorage* storage = cvCreateMemStorage(0);
    // Note that you must copy C:\Program Files\OpenCV\data\haarcascades\haarcascade_frontalface_alt2.xml
    // to your working directory
    //CvHaarClassifierCascade* cascade = (CvHaarClassifierCascade*)cvLoad( "haarcascade_frontalface_default.xml" );
    CvHaarClassifierCascade* cascade = (CvHaarClassifierCascade*)cvLoad( "haarcascade_frontalface_alt_tree.xml" );
    //CvHaarClassifierCascade* cascade = (CvHaarClassifierCascade*)cvLoad( "haarcascade_frontalface_alt.xml" );
    //CvHaarClassifierCascade* cascade = (CvHaarClassifierCascade*)cvLoad( "haarcascade_frontalface_alt2.xml" );
    double scale = 1.3;

    static CvScalar colors[] = { {{0,0,255}}, {{0,128,255}}, {{0,255,255}}, 
    {{0,255,0}}, {{255,128,0}}, {{255,255,0}}, {{255,0,0}}, {{255,0,255}} };

    // Detect objects
    cvClearMemStorage( storage );
    CvSeq* objects = cvHaarDetectObjects( img, cascade, storage, 1.1, 4, 0, cvSize( 40, 50 ));

    CvRect* r;
    // Loop through objects and draw boxes
    for( int i = 0; i < (objects ? objects->total : 0 ); i++ ){
        r = ( CvRect* )cvGetSeqElem( objects, i );
        cvRectangle( img, cvPoint( r->x, r->y ), cvPoint( r->x + r->width, r->y + r->height ),
            colors[i%8]);
    }

    cvNamedWindow( "Output" );
    cvShowImage( "Output", img );
    cvWaitKey();

    cvReleaseImage( &img );

    return 0;
}

