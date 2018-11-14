#ifndef __RENDERLIB_UTILS_STRINGUTILS_H__
#define __RENDERLIB_UTILS_STRINGUTILS_H__

#include <string>
#include <vector>

namespace RenderLib
{
  namespace StringUtils
  {
    std::vector<std::string>
    split(const std::string & source, const std::string & delimiter = " ");
  }
} // namespace RenderLib

#endif