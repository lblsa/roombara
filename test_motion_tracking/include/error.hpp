#ifndef _RB_ERROR_H_
#define _RB_ERROR_H_

#include <stdexcept>

namespace Roombara
{

class Error: public std::runtime_error
{
public:
    Error(const char *err): std::runtime_error(err){};
    Error(const std::string &err): std::runtime_error(err){};
};

}

#endif // _RB_ERROR_H_
