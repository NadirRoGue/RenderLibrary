#ifndef __CPU_MEMORY_POOLATTRIBUTE__
#define __CPU_MEMORY_POOLATTRIBUTE__

#include "Defines.h"
#include "CPU/memory/MemoryPool.h"

#include <Eigen/Eigen>

namespace RenderLib
{
	namespace CPU
	{
		namespace Memory
		{
			template<typename T, int numElements>
			class PoolAttribute
			{
			private:
				MemoryBlock * srcBlock;
				size_t elementCount;
				size_t offset;
				size_t stride;

				T value;

			public:
				PoolAttribute(MemoryBlock * src, size_t offset = 0, size_t stride = 0)
					: srcBlock(src)
					, offset(offset)
					, stride(stride)
				{
				}

				PoolAttribute(const PoolAttribute & other)
					: srcBlock(other.srcBlock)
					, offset(other.offset)
					, stride(other.stride)
				{
				}

				T at(size_t index)
				{
					size_t start, end;
					getBytePosition(index, start, end);

					if (start >= srcBlock->offset && end <= srcBlock->offset + srcBlock->length)
					{
						const char * byteData = srcBlock->pool->getDataAsBytes();
						return *((T*)(byteData + start));
					}

					// FIXME: throw exception if out of bounds
					return T();
				}

				bool set(size_t index, T value)
				{
					size_t start, end;
					getBytePosition(index, start, end);

					if (start >= srcBlock->offset && end <= srcBlock->offset + srcBlock->length)
					{
						const char * byteData = srcBlock->pool->getDataAsBytes();
						*((T*)(byteData + start)) = value;
						return true;
					}

					// FIXME: throw exception if out of bounds
					return false;
				}

				T & operator[](size_t index)
				{
					size_t start, end;
					getBytePosition(index, start, end);

					const char * byteData = srcBlock->pool->getDataAsBytes();
					return *((T*)(byteData + start));
				}

				PoolAttribute & operator=(T newValue)
				{
					*this[0] = newValue;
					return *this;
				}

				bool operator==(const PoolAttribute & other)
				{
					return getValue() == other.getValue();
				}

				T getValue()
				{
					return (*this)[0];
				}

			private:
				inline void getBytePosition(size_t index, size_t & start, size_t & end)
				{
					const size_t typeSize = sizeof(T) * numElements;
					start = offset + (typeSize + stride) * index;
					end = start + typeSize;
				}
			};

			typedef PoolAttribute<FLOAT, 1> FloatAttribute;
			typedef PoolAttribute<unsigned char, 1> ByteAttribute;
			typedef PoolAttribute<short, 1> ShortAttribute;
			typedef PoolAttribute<int, 1> IntAttribute;
			typedef PoolAttribute<unsigned long, 1> ULongAttribute;

			typedef PoolAttribute<VECTOR2, 1> Vector2Attribute;
			typedef PoolAttribute<VECTOR3, 1> Vector3Attribute;
			typedef PoolAttribute<VECTOR4, 1> Vector4Attribute;

			typedef PoolAttribute<Eigen::Vector3i, 1> IVector3Attribute;
		}
	}
}

#endif