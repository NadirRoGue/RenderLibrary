#include "defaultimpl/fileloaders/ImageLoader.h"

#include <FreeImage.h>

#include "EngineException.h"

#include "CPU/texture/TextureLoadResult.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		ImageLoader::ImageLoader()
		{
			registerFileExtension("png");
			registerFileExtension("jpg");
			registerFileExtension("tiff");
			registerFileExtension("dds");
			registerFileExtension("hdr");
			registerFileExtension("psd");
		}

		CPU::IO::AbstractLoadResultPtr ImageLoader::loadFile(const std::string & file, unsigned int options)
		{
			FreeImage_Initialise(TRUE);

			const char * fileName = file.c_str();

			FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName, 0);
			if (format == FIF_UNKNOWN)
				format = FreeImage_GetFIFFromFilename(fileName);
			if ((format == FIF_UNKNOWN) || !FreeImage_FIFSupportsReading(format))
			{
				throw EngineException("ImageLoader: Unsupported format file " + file);
				return NULL;
			}

			std::unique_ptr<CPU::Texture::TextureLoadResult> result = std::make_unique<CPU::Texture::TextureLoadResult>();
			CPU::Texture::TextureLoadResult * resultPtr = result.get();

			FIBITMAP* img = FreeImage_Load(format, fileName);
			if (img == NULL)
				return NULL;

			FIBITMAP* tempImg = img;
			img = FreeImage_ConvertTo32Bits(img);
			FreeImage_Unload(tempImg);

			resultPtr->width = FreeImage_GetWidth(img);
			resultPtr->height = FreeImage_GetHeight(img);
			resultPtr->numPixelComponents = 4;
			resultPtr->resizeBuffer();

			const unsigned int linearSize = resultPtr->height * resultPtr->width;

			//BGRA a RGBA
			char * buff = (char*)FreeImage_GetBits(img);

			for (unsigned int j = 0; j< linearSize; j++) 
			{
				resultPtr->loadedPixels[j * 4 + 0] = buff[j * 4 + 2];
				resultPtr->loadedPixels[j * 4 + 1] = buff[j * 4 + 1];
				resultPtr->loadedPixels[j * 4 + 2] = buff[j * 4 + 0];
				resultPtr->loadedPixels[j * 4 + 3] = buff[j * 4 + 3];
			}

			FreeImage_Unload(img);
			FreeImage_DeInitialise();

			return result;
		}
	}
}