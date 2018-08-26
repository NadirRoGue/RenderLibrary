#include "CPU/memory/MemoryUtil.h"

#include <cstring>

namespace RenderLib
{
	namespace CPU
	{
		namespace Memory
		{
			void copyBytes(void * dst, size_t dstBytesOffset, void * src, size_t srcBytesOffset, size_t numBytes)
			{
				memcpy(static_cast<char*>(dst) + dstBytesOffset, static_cast<char*>(src) + srcBytesOffset, numBytes);
			}
		}
	}
}