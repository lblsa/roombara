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
#include <matcher.hpp>

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
    int userInputWidth, userInputHeight;

    int counter=0;

    namespace RB = Roombara;

    try
    {
        RB::MatcherFactoryPtr factory = RB::create_matcher_factory();

        // Create matcher
        //RB::MatcherPtr matcher = factory->CreateMatcher(RB::MatcherEngine::SURF);
        RB::MatcherPtr matcher = factory->CreateMatcher(RB::MatcherEngine::TEMPLATE);

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
            }

            if( MouseHandler::EverPressed() )
            {
                matcher->DoMatch(imageTemplate, frame);
            }

            // показываем
            cv::imshow( winCaption, frame );

            if( cv::waitKey(1) == 27 ) break;
            counter++;
        }
    }
    catch(const RB::Error&) { }
    catch(...) { }

    return 0;
}
