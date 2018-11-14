#include "CPU/mesh/MeshManager.h"

#include <stdexcept>

#include "CPU/io/FileManager.h"
#include "CPU/memory/MemoryManager.h"
#include "CPU/mesh/MeshBlockConfiguration.h"

#include "material/MaterialManager.h"

#include "CPU/mesh/MeshUtil.h"

namespace RenderLib
{
  namespace CPU
  {
    namespace Mesh
    {
      MeshManager MeshManager::INSTANCE;

      MeshManager &
      MeshManager::getInstance()
      {
        return INSTANCE;
      }

      MeshManager::MeshManager()
      {
      }

      MeshManager::~MeshManager()
      {
        destroy();
      }

      void
      MeshManager::init()
      {
        unsigned int opt = Mesh::OPTION_COMPUTE_SMOOTHNORMALS_IF_ABSENT
            | Mesh::OPTION_JOIN_IDENTICAL_VERTICES;
        zArrow = loadMeshFromFile("./assets/z_arrow.obj", opt)[0];
        yArrow = loadMeshFromFile("./assets/y_arrow.obj", opt)[0];
        xArrow = loadMeshFromFile("./assets/x_arrow.obj", opt)[0];
      }

      Mesh *
      MeshManager::getZArrowMesh()
      {
        return zArrow;
      }

      Mesh *
      MeshManager::getYArrowMesh()
      {
        return yArrow;
      }

      Mesh *
      MeshManager::getXArrowMesh()
      {
        return xArrow;
      }

      std::vector<Mesh *>
      MeshManager::loadMeshFromFile(const std::string & fileName,
                                    unsigned int optionsFlag)
      {
        std::unique_lock<std::mutex> lock(mtx);

        // Import data from file
        IO::AbstractLoadResultPtr loadedMeshes
            = IO::FileManager::loadFile(fileName, optionsFlag);
        MeshLoadResult * result
            = static_cast<MeshLoadResult *>(loadedMeshes.get());

        // Process meshes
        std::vector<Mesh *> meshList
            = processLoadedMeshes(fileName, result->loadedData);

        // Process materials
        Material::MaterialManager::getInstance().parseMaterials(
            fileName, result->loadedMaterials);

        lock.unlock();

        return meshList;
      }

      std::vector<Mesh *>
      MeshManager::processLoadedMeshes(const std::string & fileName,
                                       std::vector<MeshLoadedData> & loadedData)
      {
        // Check for duplicates
        auto it = meshes.find(fileName);
        if (it != meshes.end())
        {
          // Clean previous stored mesh with the given filename
          destroyMesh(std::move(it->second));
        }

        std::vector<std::unique_ptr<Mesh>> storageResult;
        storageResult.reserve(loadedData.size());
        std::vector<Mesh *> result;
        result.reserve(loadedData.size());

        for (auto & meshData : loadedData)
        {
          std::unique_ptr<Mesh> newMesh = buildMeshFromData(meshData, 0);
          result.push_back(newMesh.get());
          storageResult.push_back(std::move(newMesh));
        }

        meshes[fileName] = std::move(storageResult);

        return result;
      }

      std::unique_ptr<Mesh>
      MeshManager::buildMeshFromData(MeshLoadedData & data,
                                     unsigned int optionsFlag)
      {
        // Create new object
        std::unique_ptr<Mesh> newMesh = std::make_unique<Mesh>();
        Mesh * meshPtr                = newMesh.get();

        MeshBlockConfiguration memConfig;
        memConfig.numFaces    = data.numFaces;
        memConfig.numVertices = data.numVertices;
        memConfig.hasNormals  = data.loadedNormals.size() == data.numVertices;
        memConfig.hasTangents = data.loadedTangents.size() == data.numVertices;
        memConfig.hasBiTangents
            = data.loadedBitangents.size() == data.numVertices;
        memConfig.numUVChannels    = data.loadedUvs.size();
        memConfig.numColorChannels = data.loadedColors.size();
        // Configure memory pool attributes
        Memory::MemoryManager::getInstance().configureObject<Mesh>(
            newMesh.get(), &memConfig);

        // Copy data to pool
        meshPtr->faces.setAttributes(data.loadedFaces);
        meshPtr->vertices.setAttributes(data.loadedVertices);
        meshPtr->normals.setAttributes(data.loadedNormals);
        meshPtr->tangents.setAttributes(data.loadedTangents);
        meshPtr->bitangents.setAttributes(data.loadedBitangents);

        for (size_t i = 0; i < data.numUVMaps; i++)
        {
          meshPtr->uvs[i].setAttributes(data.loadedUvs[i]);
        }

        for (size_t i = 0; i < data.numColorLayers; i++)
        {
          meshPtr->colors[i].setAttributes(data.loadedColors[i]);
        }

        meshPtr->materialIndex  = data.materialIndex;
        meshPtr->sourceFileName = data.srcFile;

        //ComputeNormals(meshPtr);
        //ComputeTangents(meshPtr);

        return newMesh;
      }

      std::vector<Mesh *>
      MeshManager::getMesh(const std::string & fileName)
      {
        // Check whether mesh exist
        auto it = meshes.find(fileName);
        if (it == meshes.end())
        {
          // If not, return empty vector
          return std::vector<Mesh *>();
        }

        // Copy a pointer from all the unique ptrs
        std::vector<Mesh *> result(it->second.size());
        size_t i = 0;
        for (auto & meshPtr : it->second)
        {
          result[i++] = meshPtr.get();
        }

        return result;
      }

      void
      MeshManager::destroyMesh(
          std::vector<std::unique_ptr<Mesh>> && meshToDestroy)
      {
        std::unique_lock<std::mutex> lock(mtx);

        for (auto & subMesh : meshToDestroy)
        {
          subMesh.reset();
        }

        meshToDestroy.clear();

        lock.unlock();
      }

      void
      MeshManager::destroy()
      {
        auto it = meshes.begin();
        while (it != meshes.end())
        {
          destroyMesh(std::move(it->second));
          it++;
        }
      }
    } // namespace Mesh
  } // namespace CPU
} // namespace RenderLib