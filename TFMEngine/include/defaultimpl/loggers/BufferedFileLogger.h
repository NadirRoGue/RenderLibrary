#include "defaultimpl/loggers/FileLogger.h"

#include <vector>

#define LOG_QUEUE_SIZE 7

namespace RenderLib
{
  namespace DefaultImpl
  {
    class BufferedFileLogger : public FileLogger
    {
    private:
      std::vector<std::string> bufferInfo;
      std::vector<std::string> bufferWarning;
      std::vector<std::string> bufferError;

    public:
      BufferedFileLogger();
      ~BufferedFileLogger();
      void
      logInfo(const std::string & info);
      void
      logWarning(const std::string & warn);
      void
      logError(const std::string & error);

    private:
      std::string
      buildLogString(std::vector<std::string> & buffer);
    };
  } // namespace DefaultImpl
} // namespace RenderLib
