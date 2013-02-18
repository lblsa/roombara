#ifndef _RB_SURF_MATCHER_H_
#define _RB_SURF_MATCHER_H_

#include <matcher.hpp>
#include <opencv2/opencv.hpp>

namespace Roombara
{

class SurfMatcher: public IMatcher
{
public:
	SurfMatcher():m_hessianValue(400){};
	
    virtual void DoMatch(const cv::Mat& object, cv::Mat& scene);

    void SetHessianValue(int value) { m_hessianValue = value; }

    virtual ~SurfMatcher(){};
private:
    int m_hessianValue;
};

typedef cv::Ptr<SurfMatcher> SurfMatcherPtr;

}

#endif // _RB_SURF_MATCHER_H_
