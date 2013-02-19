#ifndef _RB_TEMPLATE_MATCHER_H_
#define _RB_TEMPLATE_MATCHER_H_

#include <matcher.hpp>
#include <opencv2/opencv.hpp>

namespace Roombara
{

class TemplateMatcher: public IMatcher
{
public:
    virtual cv::Rect DoMatch(const cv::Mat& object, cv::Mat& scene);

    virtual ~TemplateMatcher(){};
};

typedef cv::Ptr<TemplateMatcher> TemplateMatcherPtr;

}

#endif // _RB_TEMPLATE_MATCHER_H_
