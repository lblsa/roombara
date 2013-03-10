/*
 * main.cpp
 *
 *  Created on: 06.02.2013
 *      Author: Tatyana
 */

#include <string>
#include <iostream>
#include <sstream>
#include <error.hpp>
#include <matcher_factory.hpp>

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
    cv::Rect lastFoundRect;
    int counter=0;

    namespace RB = Roombara;

    try
    {
        RB::MatcherFactoryPtr factory = RB::create_matcher_factory();

        // Create matcher
        //RB::SurfMatcherPtr matcher = factory->CreateSurfMatcher();
        //matcher->SetHessianValue(500);
        RB::TemplateMatcherPtr matcher = factory->CreateTemplateMatcher();

        // Camera image fetching cycle
        while(true)
        {
            // fetch image frame from camera
            cv::Mat frame, frameToShow;
            
            cap >> frame;
            frameToShow = frame.clone();
            
            //cv::cvtColor( frame, frame, CV_RGB2GRAY );
            //cv::equalizeHist( frame, frame );
            
            //cv::GaussianBlur(frame, frame, cv::Size(5,5),1.5);

            // check if user has selected the object to follow
            if( MouseHandler::WasPressed() )
            {
               lastFoundRect = MouseHandler::GetLastRect();
                
               imageTemplate = cv::Mat( frame, lastFoundRect ).clone();
               //cv::imwrite( "imageTemplate.jpg", imageTemplate );
            }

            // DoMatch only in the case of followed area is selected
            if( MouseHandler::EverPressed() )
            {
            	// 1. Calculate the search frame - +-50% of the found area
            	int frameSizeX = frame.cols;
            	int frameSizeY = frame.rows;
            	
            	int areaSizeX = lastFoundRect.width;
            	int areaSizeY = lastFoundRect.height;
            	
            	int areaX = lastFoundRect.x;
            	int areaY = lastFoundRect.y;
            	
            	double searchCoeff = 0.5;
            	
               cv::Point upperLeftSearch( std::max( int(areaX - areaSizeX*searchCoeff), 0 ), 
               									std::max( int(areaY - areaSizeY*searchCoeff), 0 ) );
               
               cv::Point lowerRightSearch( std::min( int( areaX + areaSizeX*(1+searchCoeff) ), frameSizeX ),
               									 std::min( int( areaY + areaSizeY*(1+searchCoeff) ), frameSizeY ) );
                        
               cv::Rect searchRect( upperLeftSearch, lowerRightSearch );
               cv::Mat searchFrame( frame, searchRect );
               
               cv::imshow("search area", searchFrame);
               
               // 2. DoMatch - searching for matching area
               cv::Rect found = matcher->DoMatch( imageTemplate, searchFrame ); 
               
               lastFoundRect.x = found.x + upperLeftSearch.x;
               lastFoundRect.y = found.y + upperLeftSearch.y;
               lastFoundRect.height = found.height;
               lastFoundRect.width = found.width;
               
               // 3. If found - reset template to new match 
               // Incremental changes in the scene will be absorbed due to this approach
               if( lastFoundRect.height )
               {
               	imageTemplate = cv::Mat( frame, lastFoundRect ).clone();
               	cv::rectangle( frameToShow, lastFoundRect, CV_RGB(0, 0, 255) );
               }
            }

            // показываем
            cv::imshow( winCaption, frameToShow );

            if( cv::waitKey(3) == 27 ) break;
            counter++;
        }
    }
    catch(const RB::Error&) { }
    catch(const std::bad_alloc&) { }
    catch(...) { }

    return 0;
}
