#include <error.hpp>
#include <matcher.hpp>
#include <surf_matcher.hpp>
#include <template_matcher.hpp>

namespace Roombara
{

MatcherPtr MatcherFactory::CreateMatcher(const MatcherEngine::Type type)
{
    switch(type)
    {
        case MatcherEngine::SURF:
        {
            MatcherPtr result(new SurfMatcher);
            return result;
        }
        case MatcherEngine::TEMPLATE:
        {
            MatcherPtr result(new TemplateMatcher);
            return result;
        }
        default:
            throw Error("MatcherFactory: matcher does not exist");
    }
}

MatcherFactoryPtr create_matcher_factory()
{
    return MatcherFactoryPtr(new MatcherFactory);
}

}
