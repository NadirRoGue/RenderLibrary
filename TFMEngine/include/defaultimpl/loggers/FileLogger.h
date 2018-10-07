#ifndef __RENDERLIB_DEFAULTIMPL_FILELOGGER_H__
#define __RENDERLIB_DEFAULTIMPL_FILELOGGEr_H__

#include <cstdio>

#include "logger/AbstractLogger.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		class FileLogger : public Logger::AbstractLogger
		{
		private:
			FILE * file;
		public:
			FileLogger();
			~FileLogger();
			virtual void logInfo(const std::string & info);
			virtual void logWarning(const std::string & warn);
			virtual void logError(const std::string & error);
		};
	}
}

#endif