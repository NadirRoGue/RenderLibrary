#ifndef __RENDERLIB_CPU_FILEMANAGER_H__
#define __RENDERLIB_CPU_FILEMANAGER_H__

#include <memory>
#include <unordered_map>

#include "CPU/io/FileLoader.h"

#include "logger/Log.h"

#include "EngineException.h"

namespace RenderLib
{
  namespace CPU
  {
    namespace IO
    {
      class FileManager
      {
      private:
        static std::unordered_map<std::string, std::shared_ptr<FileLoader>>
            fileLoaders;

      public:
		/// <summary>
		/// Registers a file loader. The extensions to which it will be associated.
		/// will be taken from the supported extension list of the own ffile loader
		/// </summary>
		template <class T>
        static void
        registerFileLoader()
        {
          if (!std::is_base_of<FileLoader, T>::value)
          {
            std::string message = "FileManager: Attempted to register a non "
                                  "FileLoader derived class: "
                + std::string(typeid(T).name());
            throw EngineException(message.c_str());
          }

          std::shared_ptr<FileLoader> newLoader = std::make_shared<T>();
          FileLoader * loaderInstance           = newLoader.get();

          for (auto & extension : loaderInstance->getSupportedExtensions())
          {
            auto it = fileLoaders.find(extension);
            if (it != fileLoaders.end() && it->second != NULL)
            {
              std::string warn
                  = "FileManager: [Warning] Overwritted file loader "
                  + std::string(typeid(it->second).name()) + " for "
                  + std::string(typeid(loaderInstance).name())
                  + " for extension \"" + extension + "\"";

              Logger::Log::getInstance().logWarning(warn);

              auto sharedPtr(newLoader);
              it->second = std::move(sharedPtr);
            }
            else
            {
              auto sharedPtr(newLoader);
              fileLoaders[extension] = std::move(sharedPtr);
            }
          }
        }


		/// <summary>
		/// Loads a file denoted by path. An optional mask of options might be
		/// passed to the file loader. The meaning of the mask is implementation
		/// depenent. The result will be a sublcass of AbstractLoadResult.
		/// </summary>
		/// <param name="filePath">The file path.</param>
		/// <param name="options">The options mask.</param>
		/// <returns>An instance of an AbstractLoadResult subclass, containing
		/// the required data
		/// </returns>
		static AbstractLoadResultPtr
        loadFile(const std::string & filePath, unsigned int options = 0);
      };
    } // namespace IO
  } // namespace CPU
} // namespace RenderLib

#endif
