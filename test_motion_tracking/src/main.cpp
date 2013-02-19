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
        RB::SurfMatcherPtr matcher = factory->CreateSurfMatcher();
        matcher->SetHessianValue(500);
        //RB::TemplateMatcherPtr matcher = factory->CreateTemplateMatcher();

        while(true)
        {
            // получаем кадр
            cv::Mat frame;
            cap >> frame;
            cvtColor(frame,frame,CV_RGB2GRAY);
            //cv::GaussianBlur(frame, frame, cv::Size(5,5),1.5);

            if( MouseHandler::WasPressed() )
            {
               lastFoundRect = MouseHandler::GetLastRect();
                
               imageTemplate = cv::Mat( frame, lastFoundRect ).clone();
               //cv::imwrite( "imageTemplate.jpg", imageTemplate );
            }

            if( MouseHandler::EverPressed() )
            {
            	int frameSizeX = frame.cols;
            	int frameSizeY = frame.rows;
            	
            	int areaSizeX = lastFoundRect.width;
            	int areaSizeY = lastFoundRect.height;
            	
            	int areaX = lastFoundRect.x;
            	int areaY = lastFoundRect.y;
            	
               cv::Point upperLeftSearch( std::max( int(areaX - areaSizeX*0.5), 0 ), 
               									std::max( int(areaY - areaSizeY*0.5), 0 ) );
               
               cv::Point lowerRightSearch( std::min( int( areaX + areaSizeX*1.5 ), frameSizeX ),
               									 std::min( int( areaY + areaSizeY*1.5 ), frameSizeY ) );
                        
               cv::Rect searchRect( upperLeftSearch, lowerRightSearch );
               cv::Mat searchFrame( frame, searchRect );
               
               cv::imshow("search area", searchFrame);
               
               cv::Rect found = matcher->DoMatch( imageTemplate, searchFrame ); 
               
               lastFoundRect.x = found.x + upperLeftSearch.x;
               lastFoundRect.y = found.y + upperLeftSearch.y;
               lastFoundRect.height = found.height;
               lastFoundRect.width = found.width;
               
               // if found - reset template to new match (incremental changes will be absorbed)
               if( lastFoundRect.height )
               	imageTemplate = cv::Mat( frame, lastFoundRect ).clone();
            }

            // показываем
            cv::imshow( winCaption, frame );

            if( cv::waitKey(330) == 27 ) break;
            counter++;
        }
    }
    catch(const RB::Error&) { }
    catch(const std::bad_alloc&) { }
    catch(...) { }

    return 0;
}
