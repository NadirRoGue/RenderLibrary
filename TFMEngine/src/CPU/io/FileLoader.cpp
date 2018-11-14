#include "CPU/io/FileLoader.h"

namespace RenderLib
{
  namespace CPU
  {
    namespace IO
    {
      void
      FileLoader::registerFileExtension(const std::string & fileExtension)
      {
        supportedExtensions.push_back(fileExtension);
      }

      const std::vector<std::string> &
      FileLoader::getSupportedExtensions()
      {
        return supportedExtensions;
      }
    } // namespace IO
  } // namespace CPU
} // namespace RenderLib