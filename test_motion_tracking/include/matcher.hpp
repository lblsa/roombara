#ifndef _RB_MATCHER_H_
#define _RB_MATCHER_H_

#include <opencv2/opencv.hpp>

namespace Roombara
{

struct MatcherEngine
{
    enum Type 
    {
        SURF = 0,
        TEMPLATE
    };
};

class IMatcher
{
public:
    virtual void DoMatch(const cv::Mat& object, cv::Mat& scene) = 0;
    virtual ~IMatcher(){};
};

typedef cv::Ptr<IMatcher> MatcherPtr;

class MatcherFactory
{
public:
    MatcherPtr CreateMatcher(const MatcherEngine::Type type = MatcherEngine::SURF);
};

typedef cv::Ptr<MatcherFactory> MatcherFactoryPtr;

MatcherFactoryPtr create_matcher_factory();

}

#endif // _RB_MATCHER_H_
