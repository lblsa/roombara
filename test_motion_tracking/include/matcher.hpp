#ifndef _RB_MATCHER_H_
#define _RB_MATCHER_H_

#include <opencv2/opencv.hpp>

namespace Roombara
{

class IMatcher
{
public:
    virtual cv::Rect DoMatch(const cv::Mat& object, cv::Mat& scene) = 0;
    virtual ~IMatcher(){};
};

typedef cv::Ptr<IMatcher> MatcherPtr;

}

#endif // _RB_MATCHER_H_
