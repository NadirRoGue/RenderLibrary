#include "defaultimpl/fileloaders/ShaderLoader.h"

#include <fstream>

#include "EngineException.h"

#include "GPU/program/ShaderLoadResult.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    ShaderLoader::ShaderLoader()
    {
      // GLSL does not enforce file extension
      // However: https://www.khronos.org/opengles/sdk/tools/Reference-Compiler/
      registerFileExtension("vert");
      registerFileExtension("tesc");
      registerFileExtension("tese");
      registerFileExtension("geom");
      registerFileExtension("frag");
      registerFileExtension("comp");
      registerFileExtension("glsl");
    }

    CPU::IO::AbstractLoadResultPtr
    ShaderLoader::loadFile(const std::string & filePath, unsigned int options)
    {
      std::ifstream file;
      file.open(filePath, std::ios::in);
      if (!file)
      {
        std::string message = "ShaderLoader: Could not load file \"" + filePath
            + "\": file not found";

        throw EngineException(message.c_str());
      }

      std::unique_ptr<GPU::Program::ShaderLoadResult> loadResultPtr
          = std::make_unique<GPU::Program::ShaderLoadResult>();
      GPU::Program::ShaderLoadResult * loadResult = loadResultPtr.get();

      char buf[1024];

      if (file.good())
      {
        file.getline(buf, 1024);
        size_t rawStrLen   = strlen(buf);
        loadResult->header = std::string(buf);
      }

      if (!file.eof())
      {
        while (file.good())
        {
          file.getline(buf, 1024);
          std::string appendLine(buf);

          size_t includePos = appendLine.find("#include");
          if (includePos != std::string::npos)
          {
            std::string subShader;
            handleInclude(appendLine, subShader);
            loadResult->body += subShader + "\n";
          }
          else
          {
            loadResult->body += appendLine + "\n";
          }
        }
      }

      loadResult->body += std::string("\0");

      file.close();

      return loadResultPtr;
    }

    void
    ShaderLoader::handleInclude(const std::string & include, std::string & dst)
    {
      size_t spacePos = include.find_first_of(" ");
      if (spacePos == std::string::npos)
      {
        throw EngineException("ShaderLoader: Illegal #include declaration: "
                              + include);
      }

      std::string filePath
          = include.substr(spacePos + 1, include.length() - 1 - spacePos);

      CPU::IO::AbstractLoadResultPtr includePtr = loadFile(filePath, 0);
      GPU::Program::ShaderLoadResult * loadResult
          = static_cast<GPU::Program::ShaderLoadResult *>(includePtr.get());

      dst = "";
      dst += loadResult->header + "\n";
      dst += loadResult->body.substr(0, loadResult->body.length() - 1);
    }
  } // namespace DefaultImpl
} // namespace RenderLib