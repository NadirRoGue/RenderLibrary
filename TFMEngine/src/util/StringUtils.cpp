#include "util/StringUtils.h"

namespace RenderLib
{
  namespace StringUtils
  {
    std::vector<std::string>
    split(const std::string & source, const std::string & delimiter)
    {
      std::vector<std::string> result;

      std::string buf;

      size_t beginning    = 0;
      size_t counter      = 0;
      size_t delimCounter = 0;
      while (counter < source.length())
      {
        if (source[counter] == delimiter[delimCounter])
        {
          if (delimCounter == delimiter.length() - 1)
          {
            buf = source.substr(beginning,
                                counter + 1 - delimiter.length() - beginning);
            if (buf.length() > 0)
            {
              result.push_back(buf);
            }

            beginning    = counter + 1;
            delimCounter = 0;
          }
          else
          {
            delimCounter++;
          }
        }
        else
        {
          delimCounter = 0;
        }

        counter++;
      }

      if (beginning < counter)
      {
        buf = source.substr(beginning, counter);
        result.push_back(buf);
      }

      return result;
    }
  } // namespace StringUtils
} // namespace RenderLib