#include "defaultimpl/loggers/FileLogger.h"

#include <chrono>
#include <ctime>

#include <iostream>

#ifdef _WIN32
#define FOPEN(fileptr, name, openmode) fopen_s(fileptr, name, openmode)
#else
#define FOPEN(fileptr, name, openmode) fileptr = fopen(name, openmode);
#endif

namespace RenderLib
{
	namespace DefaultImpl
	{
		FileLogger::FileLogger()
		{
			// Gather date for file name day-month-year_hour-min-seconds.log
			auto currentTime = std::chrono::system_clock::now();
			std::time_t currentDate = std::chrono::system_clock::to_time_t(currentTime);
			char dateStrBuf[0xff];

			std::tm time;
			LOCALTIME(time, &currentDate);

			std::strftime(dateStrBuf, 0xff, "%d-%m-%Y_%H-%M_%S.log", &time);
			
			file = NULL;
		  FOPEN(&file, dateStrBuf, "w");
		}

		FileLogger::~FileLogger()
		{
			if (file != NULL)
			{
				fflush(file);
				fclose(file);
			}
		}

		void FileLogger::logInfo(const std::string & info)
		{
			if (file)
			{
				std::string finalInfo = info + "\n";
				fputs(finalInfo.c_str(), file);
			}
			else
			{
				std::cout << "Fuck" << std::endl;
			}
		}

		void FileLogger::logWarning(const std::string & warn)
		{
			if (file)
			{
				std::string finalWarn = warn + "\n";
				fputs(finalWarn.c_str(), file);
			}
		}

		void FileLogger::logError(const std::string & error)
		{
			if (file)
			{
				std::string finalError = error + "\n";
				fputs(finalError.c_str(), file);
			}
		}
	}
}