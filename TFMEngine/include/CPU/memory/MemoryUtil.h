#ifndef __CPU_MEMORY_MEMORYUTIL__
#define __CPU_MEMORY_MEMORYUTIL__

namespace RenderLib
{
	namespace CPU
	{
		namespace Memory
		{
			void copyBytes(void * dst, size_t dstBytesOffset, void * src, size_t srcBytesOffset, size_t numBytes);
		}
	}
}

#endif