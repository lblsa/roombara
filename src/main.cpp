// http://singhgaganpreet.com/tag/cvboundingrect-example/
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

CvRect rect;
CvSeq* contours = 0;
CvMemStorage* storage = NULL;
CvCapture *cam;
IplImage *currentFrame, *currentFrame_grey, *differenceImg, *oldFrame_grey;

bool first = true;

int main(int argc, char* argv[])
{
       //Create a new movie capture object.
       cam = cvCaptureFromCAM(0);
       //cam = cvCreateFileCapture("test.avi");

       //create storage for contours
       storage = cvCreateMemStorage(0);

       //capture current frame from webcam
       currentFrame = cvQueryFrame(cam);

       //Size of the image.
       CvSize imgSize;
       imgSize.width = currentFrame->width;
       imgSize.height = currentFrame->height;

       //Images to use in the program.
       currentFrame_grey = cvCreateImage( imgSize, IPL_DEPTH_8U, 1);                           

       while(1)
       {
              currentFrame = cvQueryFrame( cam );
              if( !currentFrame ) break;

              //Convert the image to grayscale.
              cvCvtColor(currentFrame,currentFrame_grey,CV_RGB2GRAY);

              if(first) //Capturing Background for the first time
              {
                     differenceImg = cvCloneImage(currentFrame_grey);
                     oldFrame_grey = cvCloneImage(currentFrame_grey);
                     cvConvertScale(currentFrame_grey, oldFrame_grey, 1.0, 0.0);
                     first = false;
                     continue;
              }

              //Minus the current frame from the moving average.
              cvAbsDiff(oldFrame_grey,currentFrame_grey,differenceImg);

              //bluring the differnece image
              cvSmooth(differenceImg, differenceImg, CV_BLUR);             

              //apply threshold to discard small unwanted movements
              //cvThreshold(differenceImg, differenceImg, 25, 255, CV_THRESH_BINARY);
              cvThreshold(differenceImg, differenceImg, 50, 255, CV_THRESH_BINARY);

              //find contours
              cvFindContours( differenceImg, storage, &contours );

              //draw bounding box around each contour
              for(; contours!=0; contours = contours->h_next)
              {
                     rect = cvBoundingRect(contours, 0); //extract bounding box for current contour

                     //drawing rectangle
                     cvRectangle(currentFrame,                  
                                  cvPoint(rect.x, rect.y),    
                                  cvPoint(rect.x+rect.width, rect.y+rect.height),
                                  cvScalar(0, 0, 255, 0),
                                  2, 8, 0);                 
              }

              //display colour image with bounding box
              cvShowImage("Output Image", currentFrame);

              //display threshold image
              cvShowImage("Difference image", differenceImg);

              //New Background
              cvConvertScale(currentFrame_grey, oldFrame_grey, 1.0, 0.0);

              //clear memory and contours
              cvClearMemStorage( storage );
              contours = 0;

              //press Esc to exit
              char c = cvWaitKey(33);
              if( c == 27 ) break;

       }

       // Destroy the image & movies objects
       cvReleaseImage(&oldFrame_grey);
       cvReleaseImage(&differenceImg);
       cvReleaseImage(&currentFrame);
       cvReleaseImage(&currentFrame_grey);
       //cvReleaseCapture(&cam);

       return 0;
}
