#ifndef __RENDERLIB_CPU_FILELOADER_H__
#define __RENDERLIB_CPU_FILELOADER_H__

#include <string>
#include <vector>

#include "CPU/io/FileLoadResult.h"

namespace RenderLib
{
  namespace CPU
  {
    namespace IO
    { 
      class FileLoader
      {
      protected:
        std::vector<std::string> supportedExtensions;

      public:

		/// <summary>
		/// Registers a supported file extension. Example: "obj"
		/// </summary>
		/// <param name="fileExtension">The file extension.</param>
		void
        registerFileExtension(const std::string & fileExtension);


		/// <summary>
		/// Return the list of supported extensions
		/// </summary>
		/// <returns>The list of supported extendions</returns>
		const std::vector<std::string> &
        getSupportedExtensions();


		/// <summary>
		/// Loads the file given by filePath. The options params is passed
		/// to the handler, but usage is optional and implementation dependant.
		/// </summary>
		/// <param name="filePath">The file path.</param>
		/// <param name="options">The options mask (Optional).</param>
		/// <returns>
		/// A subclass of AbstractLoadResult which must contain
		/// the 
		/// </returns>
		virtual AbstractLoadResultPtr
        loadFile(const std::string & filePath, unsigned int options = 0)
            = 0;
      };
    } // namespace IO
  } // namespace CPU
} // namespace RenderLib

#endif
