#ifndef __RENDERLIB_GPU_MESH_GPUMESH_H__
#define __RENDERLIB_GPU_MESH_GPUMESH_H__

#include <memory>
#include <unordered_map>
#include <vector>

//#include <GL/glew.h>

#include "GPU/mesh/GPUBuffer.h"

namespace RenderLib
{
  namespace GPU
  {
    namespace Mesh
    {
      typedef struct GPUAttribute
      {
        size_t elementTypeSize;
        size_t elementCount;
        size_t numElements;
        size_t offset;
        size_t stride;
        unsigned int dataType;
        bool normalizeOnUpload;

        GPUAttribute()
          : elementTypeSize(0)
          , elementCount(0)
          , numElements(0)
          , offset(0)
          , stride(0)
          , dataType(GL_FLOAT)
          , normalizeOnUpload(false)
        {
        }

        GPUAttribute(size_t elementTypeSize, size_t elemCount,
                     size_t numElements, size_t offset, size_t stride,
                     unsigned int dataType, bool normalize)
          : elementTypeSize(elementTypeSize)
          , elementCount(elemCount)
          , numElements(numElements)
          , offset(offset)
          , stride(stride)
          , dataType(dataType)
          , normalizeOnUpload(normalize)
        {
        }

      } GPUAttribute;

      class GPUMesh
      {
      public:
        size_t index;

        GPUBuffer * gpuBuffer;

        size_t faceIndexOffset;
        size_t dataIndexOffset;

        size_t faceSize;
        size_t dataSize;

        GPUAttribute faces;
        GPUAttribute vertices;
        GPUAttribute normals;
        GPUAttribute tangents;
        GPUAttribute bitangents;
        std::vector<GPUAttribute> uvs;
        std::vector<GPUAttribute> colors;

        GPUMesh();
        ~GPUMesh();

        size_t
        getNumFaces();
        size_t
        getNumVertices();
        size_t
        getFaceSizeGPU();
        size_t
        getDataSizeGPU();
      };

      typedef std::unique_ptr<GPUMesh> GPUMeshPtr;
    } // namespace Mesh
  } // namespace GPU
} // namespace RenderLib

#endif