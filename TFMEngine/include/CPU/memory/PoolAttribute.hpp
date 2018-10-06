#ifndef __CPU_MEMORY_POOLATTRIBUTE__
#define __CPU_MEMORY_POOLATTRIBUTE__

#include "Defines.h"
#include "CPU/memory/MemoryPool.h"

#include <vector>
#include <stdexcept>

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

			public:
				PoolAttribute()
					: srcBlock(NULL)
					, elementCount(0)
					, offset(0)
					, stride(0)
				{

				}

				PoolAttribute(MemoryBlock * src, size_t offset = 0, size_t stride = 0, size_t elementCount = 1)
					: srcBlock(src)
					, offset(offset)
					, stride(stride)
					, elementCount(elementCount)
				{
				}

				PoolAttribute(const PoolAttribute & other)
					: srcBlock(other.srcBlock)
					, offset(other.offset)
					, stride(other.stride)
					, elementCount(other.elementCount)
				{
				}

				void setAttributeSource(MemoryBlock * src, size_t offset = 0, size_t stride = 0, size_t elementCount = 1)
				{
					srcBlock = src;
					this->offset = offset;
					this->stride = stride;
					this->elementCount = elementCount;
				}

				const size_t & size()
				{
					return elementCount;
				}

				size_t getElementSize()
				{
					return sizeof(T) * numElements;
				}

				const size_t getOffset()
				{
					return offset;
				}

				const size_t & getStride()
				{
					return stride;
				}

				const T & at(size_t index)
				{
					size_t start, end;
					getBytePosition(index, start, end);

					const char * byteData = srcBlock->pool->getDataAsBytes();
					return *((T*)(byteData + start));
				}

				void set(size_t index, T * value)
				{
					size_t start, end;
					getBytePosition(index, start, end);

					const char * byteData = srcBlock->pool->getDataAsBytes();
					*((T*)(byteData + start)) = *value;
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

				void dumpAttributes(std::vector<T> & bufer)
				{
					if (bufer.capacity() < elementCount)
					{
						bufer.reserve(elementCount);
					}
					
					size_t i = 0;
					while (i < elementCount)
					{
						bufer.push_back((*this)[i++]);
					}
				}

				void setAttributes(const std::vector<T> & source)
				{
					size_t i = 0;
					// Use the smallest element count
					size_t minElementCount = std::min(elementCount, source.size());
					while (i < minElementCount)
					{
						(*this)[i] = source[i];
						i++;
					}
				}

				/*bool operator==(const PoolAttribute & other)
				{
					return srcBlock == other.srcBlock
						&& getValue() == other.getValue();
				}*/

				T getValue()
				{
					return (*this)[0];
				}

				MemoryBlock * getMemorySource()
				{
					return srcBlock;
				}

			private:
				inline void getBytePosition(const size_t & index, size_t & start, size_t & end)
				{
					if (srcBlock == NULL)
					{
						throw std::runtime_error("PoolAttribute: Attempted to use non-initialized PoolAttribute");
					}

					const size_t typeSize = sizeof(T) * numElements;
					start = srcBlock->offset + offset + (stride) * index;
					end = start + typeSize;

					if (start < srcBlock->offset || end > srcBlock->offset + srcBlock->length)
					{
						throw std::runtime_error("PoolAttribute: index out of bounds: " + std::to_string(index));
					}
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

			typedef PoolAttribute<IVECTOR3, 1> IVector3Attribute;
		}
	}
}

#endif