#include <error.hpp>
#include <matcher_factory.hpp>

namespace Roombara
{

SurfMatcherPtr MatcherFactory::CreateSurfMatcher()
{
    return SurfMatcherPtr(new SurfMatcher);
}

TemplateMatcherPtr MatcherFactory::CreateTemplateMatcher()
{
    return TemplateMatcherPtr(new TemplateMatcher);
}

MatcherFactoryPtr create_matcher_factory()
{
    return MatcherFactoryPtr(new MatcherFactory);
}

}
