#include "CPU/mesh/Mesh.h"

namespace RenderLib
{
  namespace CPU
  {
    namespace Mesh
    {
      unsigned int Mesh::OPTION_COMPUTE_NORMALS_IF_ABSENT       = 0x01;
      unsigned int Mesh::OPTION_COMPUTE_SMOOTHNORMALS_IF_ABSENT = 0x02;
      unsigned int Mesh::OPTION_COMPUTE_TANGENTSPACE            = 0x04;
      unsigned int Mesh::OPTION_JOIN_IDENTICAL_VERTICES         = 0x08;

      Mesh::Mesh() : index(-1), materialIndex(-1)
      {
      }

      Mesh::Mesh(const Mesh & other)
        : faces(other.faces)
        , vertices(other.vertices)
        , normals(other.normals)
        , tangents(other.tangents)
        , index(other.index)
      {
        size_t i = 0;
        colors.resize(other.colors.size());
        for (auto & colorAttribVector : other.colors)
        {
          colors[i++] = colorAttribVector;
        }

        i = 0;
        uvs.resize(other.uvs.size());
        for (auto & uvAttribVector : other.uvs)
        {
          uvs[i++] = uvAttribVector;
        }
      }

      Mesh::~Mesh()
      {
      }

      size_t
      Mesh::getNumFaces()
      {
        return faces.size();
      }

      size_t
      Mesh::getNumVertices()
      {
        return vertices.size();
      }

      size_t
      Mesh::getNumColorChannels()
      {
        return colors.size();
      }

      size_t
      Mesh::getNumUVChannels()
      {
        return uvs.size();
      }
    } // namespace Mesh
  } // namespace CPU
} // namespace RenderLib