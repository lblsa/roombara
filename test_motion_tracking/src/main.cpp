/*
 * main.cpp
 *
 *  Created on: 06.02.2013
 *      Author: Tatyana
 */

#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <string>
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;
const std::string winCaption = "Camera input:";

static bool everPressed = false;
static bool wasPressed  = false;

static cv::Rect  lastRect;
static cv::Point firstClickPoint;
static cv::Point secondClickPoint;

class MouseHandler
{
public:
	static void MouseWinCallback(int event, int x, int y, int flags, void* userdata)
	{
		switch(event)
		{
		case CV_EVENT_LBUTTONDOWN:
			firstClickPoint.x = x;
			firstClickPoint.y = y;
			break;
			
		case CV_EVENT_LBUTTONUP:
         everPressed = true;
		   wasPressed = true;
		   secondClickPoint.x = x;
		   secondClickPoint.y = y;
		   lastRect = cv::Rect( firstClickPoint, secondClickPoint );
		   break;
		   
		case CV_EVENT_RBUTTONDOWN:
			break;
		}
	}

	static bool EverPressed() {return everPressed;}
   static bool WasPressed() {return wasPressed;}
   
	static const cv::Rect& GetLastRect()
	{
	   wasPressed = false;
	   return lastRect;
	}
};

bool findObjectSURF(const Mat& objectMat, Mat& sceneMat, int hessianValue )
{
    bool objectFound = false;
    float nndrRatio = 0.7f;
    //vector of keypoints   
    vector< cv::KeyPoint > keypointsO;
    vector< cv::KeyPoint > keypointsS;    

    cv::Mat descriptors_object, descriptors_scene;      

    //-- Step 1: Extract keypoints
    SurfFeatureDetector surf(hessianValue); 
    //  FastFeatureDetector surf(hessianValue);
    surf.detect(sceneMat,keypointsS);
    if(keypointsS.size() < 3) return false; //Not enough keypoints, object not found
    surf.detect(objectMat,keypointsO);
    if(keypointsO.size() < 4) return false; //Not enough keypoints, object not found

    //-- Step 2: Calculate descriptors (feature vectors)
    SurfDescriptorExtractor extractor;
    //  BriefDescriptorExtractor extractor(32);
    extractor.compute( sceneMat, keypointsS, descriptors_scene );
    extractor.compute( objectMat, keypointsO, descriptors_object );

    //-- Step 3: Matching descriptor vectors using FLANN matcher
    FlannBasedMatcher matcher;  
    //BruteForceMatcher< L2<float> > matcher;

    std::vector< vector< DMatch >  > matches;
    matcher.knnMatch( descriptors_object, descriptors_scene, matches, 2 );    
    vector< DMatch > good_matches;
    good_matches.reserve(matches.size());  

    for (size_t i = 0; i < matches.size(); ++i)
    { 
        if (matches[i].size() < 2)
            continue;

        const DMatch &m1 = matches[i][0];
        const DMatch &m2 = matches[i][1];

        if(m1.distance <= nndrRatio * m2.distance)        
            good_matches.push_back(m1);     
    }

    if( (good_matches.size() >=4))
    {
        //cout << "OBJECT FOUND!" << endl;
        std::vector< Point2f > obj;
        std::vector< Point2f > scene;

        for( unsigned int i = 0; i < good_matches.size(); i++ )
        {
            //-- Get the keypoints from the good matches
            obj.push_back( keypointsO[ good_matches[i].queryIdx ].pt );
            scene.push_back( keypointsS[ good_matches[i].trainIdx ].pt );
        }

        Mat H = findHomography( obj, scene, CV_RANSAC );

        //-- Get the corners from the image_1 ( the object to be "detected" )
        std::vector< Point2f > obj_corners(4);
        obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( objectMat.cols, 0 );
        obj_corners[2] = cvPoint( objectMat.cols, objectMat.rows ); obj_corners[3] = cvPoint( 0, objectMat.rows );
        std::vector< Point2f > scene_corners(4);

        perspectiveTransform( obj_corners, scene_corners, H);

        //-- Draw lines between the corners (the mapped object in the scene - image_2 ) 
        line( sceneMat, scene_corners[0] , scene_corners[1], CV_RGB(0, 0, 255), 2 ); //TOP line
        line( sceneMat, scene_corners[1] , scene_corners[2], CV_RGB(0, 0, 255), 2 );
        line( sceneMat, scene_corners[2] , scene_corners[3], CV_RGB(0, 0, 255), 2 );
        line( sceneMat, scene_corners[3] , scene_corners[0] , CV_RGB(0, 0, 255), 2 ); 

        objectFound=true;
    }

    return objectFound;
}

int main(int argc, char* argv[])
{
   // получаем любую подключённую камеру
   cv::VideoCapture cap(0); 
   //cap.open("test.avi"); 
   assert( cap.isOpened() );

   //cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 640);//1280);
   //cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 480);//960);
   
   // узнаем ширину и высоту кадра
   std::cout << "[i] " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << " x " << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << std::endl;

   cv::namedWindow( winCaption );
   cv::setMouseCallback( winCaption, MouseHandler::MouseWinCallback );

   std::cout << "[i] press Enter for capture image and Esc for quit!" << std::endl;
   
   cv::Mat imageTemplate;
   int userInputWidth, userInputHeight;

   int counter=0;
   while(true)
   {
      // получаем кадр
      cv::Mat frame;
      cap >> frame;
      cvtColor(frame,frame,CV_RGB2GRAY);
      //cv::GaussianBlur(frame, frame, cv::Size(5,5),1.5);

      if( MouseHandler::WasPressed() )
      {
         const cv::Rect& last = MouseHandler::GetLastRect();
         userInputWidth = last.width;
         userInputHeight = last.height;

         imageTemplate = cv::Mat( frame, last ).clone();
         //cv::imwrite( "imageTemplate.jpg", imageTemplate );
         everPressed = true;
      }
      
      if( MouseHandler::EverPressed() )
      {
         findObjectSURF(imageTemplate, frame, 50);
          /*
         cv::Mat result;
         //cv::matchTemplate( frame, imageTemplate, result, CV_TM_CCORR_NORMED );
         cv::matchTemplate( frame, imageTemplate, result, CV_TM_CCOEFF_NORMED );

         
         double minVal, maxVal;
         cv::Point minLoc, maxLoc;
         minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc );
         
         cv::Rect foundRect( maxLoc.x, maxLoc.y, userInputWidth, userInputHeight );
         
         cv::rectangle( frame, foundRect, CV_RGB(0, 0, 255) );
         imageTemplate = cv::Mat( frame, foundRect ).clone();
         */
      }
      
      // показываем
      cv::imshow( winCaption, frame );

      if( cv::waitKey(1) == 27 ) break;
      counter++;
   }
   return 0;
}
