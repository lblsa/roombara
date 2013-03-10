#include <template_matcher.hpp>

namespace Roombara
{

cv::Rect TemplateMatcher::DoMatch(const cv::Mat& object, cv::Mat& scene)
{
    cv::Mat result;
    //cv::matchTemplate( scene, object, result, CV_TM_CCORR_NORMED );
    cv::matchTemplate( scene, object, result, CV_TM_CCOEFF_NORMED );

    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc );

    cv::Rect foundRect( maxLoc.x, maxLoc.y, object.cols, object.rows );

    //cv::rectangle( scene, foundRect, CV_RGB(0, 0, 255) );
    //imageTemplate = cv::Mat( scene, foundRect ).clone();
    
    return foundRect;
}

}
