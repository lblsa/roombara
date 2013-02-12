/*
 * main.cpp
 *
 *  Created on: 06.02.2013
 *      Author: Tatyana
 */

#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <sstream>

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

int main(int argc, char* argv[])
{
   // получаем любую подключённую камеру
   cv::VideoCapture cap(0);
   assert( cap.isOpened() );

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
      
      if( MouseHandler::WasPressed() )
      {
         const cv::Rect& last = MouseHandler::GetLastRect();
         userInputWidth = last.width;
         userInputHeight = last.height;

         imageTemplate = cv::Mat( frame, last ).clone();
         //cv::imwrite( "imageTemplate.jpg", imageTemplate );
      }
      
      if( MouseHandler::EverPressed() )
      {
         cv::Mat result;
         cv::matchTemplate( frame, imageTemplate, result, CV_TM_CCORR_NORMED );
         
         double minVal, maxVal;
         cv::Point minLoc, maxLoc;
         minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc );
         
         cv::Rect foundRect( maxLoc.x, maxLoc.y, userInputWidth, userInputHeight );
         
         cv::rectangle( frame, foundRect, CV_RGB(0, 0, 255) );
         
         imageTemplate = cv::Mat( frame, foundRect ).clone();
      }
      
      // показываем
      cv::imshow( winCaption, frame );

      if( cv::waitKey(1) == 27 ) break;
      counter++;
   }
   return 0;
}
