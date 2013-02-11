/* File name: morphology.c */
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdlib.h>
#include <stdio.h>

IplImage* src = 0;
IplImage* image = 0;
IplImage* dest = 0;

//the address of variable which receives trackbar position update 
int pos = 0;

//callback function for slider , implements opening 
void Opening(int id)   
{
    id;
    cvErode(src,image,NULL,pos);
    cvDilate(image,dest,NULL,pos);
    cvShowImage("Opening&Closing window",dest);
}   

//callback function for slider , implements closing 
void Closing(int id)   
{
    id;
    cvDilate(src,image,NULL,pos);
    cvErode(image,dest,NULL,pos);
    cvShowImage("Opening&Closing window",dest);
}

//callback function for slider , implements erosion 
void Erosion(int id)   
{
    id;
    cvErode(src,dest,NULL,pos);
    cvShowImage("Erosion&Dilation window",dest);
}

//callback function for slider , implements dilation
void Dilation(int id)   
{
    id;
    cvDilate(src,dest,NULL,pos);
    cvShowImage("Erosion&Dilation window",dest);
}


int main( int argc, char** argv )
{
    if (argc < 2)
    {
        printf("Usage: %s <img.jpg>\n", argv[0]);
        return 1;
    }
    if( (src = cvLoadImage(argv[1],1)) == 0 )
        return -1;

    image = cvCloneImage(src);
    dest = cvCloneImage(src);

    //create windows for output images
    cvNamedWindow("Opening&Closing window",1);
    cvNamedWindow("Erosion&Dilation window",1);

    cvShowImage("Opening&Closing window",src);
    cvShowImage("Erosion&Dilation window",src);

    cvCreateTrackbar("Open","Opening&Closing window",&pos,10,Opening);
    cvCreateTrackbar("Close","Opening&Closing window",&pos,10,Closing);
    cvCreateTrackbar("Dilate","Erosion&Dilation window",&pos,10,Dilation);
    cvCreateTrackbar("Erode","Erosion&Dilation window",&pos,10,Erosion);

    cvWaitKey(0);
    //releases header an dimage data  
    cvReleaseImage(&src);
    cvReleaseImage(&image);
    cvReleaseImage(&dest);
    //destroys windows 
    cvDestroyWindow("Opening&Closing window"); 
    cvDestroyWindow("Erosion&Dilation window"); 

    return 0;
}
