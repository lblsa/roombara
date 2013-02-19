#ifndef _RB_MATCHER_FACTORY_H_
#define _RB_MATCHER_FACTORY_H_

#include <opencv2/opencv.hpp>
#include <matcher.hpp>
#include <surf_matcher.hpp>
#include <template_matcher.hpp>

namespace Roombara
{

class MatcherFactory
{
public:
    SurfMatcherPtr CreateSurfMatcher();
    TemplateMatcherPtr CreateTemplateMatcher();
};

typedef cv::Ptr<MatcherFactory> MatcherFactoryPtr;

MatcherFactoryPtr create_matcher_factory();

}

#endif // _RB_MATCHER_FACTORY_H_
