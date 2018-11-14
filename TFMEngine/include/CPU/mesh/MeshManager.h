#ifndef __RENDERLIB_CPU_MESHMANAGER__
#define __RENDERLIB_CPU_MESHMANAGER__

#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "Defines.h"

#include "CPU/mesh/Mesh.h"
#include "CPU/mesh/MeshLoadResult.h"

namespace RenderLib
{
  namespace CPU
  {
    namespace Mesh
    {
      class MeshManager
      {
      private:
        static MeshManager INSTANCE;

      private:
        std::unordered_map<std::string, std::vector<std::unique_ptr<Mesh>>>
            meshes;

        Mesh * zArrow;
        Mesh * yArrow;
        Mesh * xArrow;

      public:
        static MeshManager &
        getInstance();

      private:
        std::mutex mtx;

      private:
        MeshManager();

      public:
        ~MeshManager();

        void
        init();

        std::vector<Mesh *>
        loadMeshFromFile(const std::string & file, unsigned int optionsFlag);
        std::vector<Mesh *>
        getMesh(const std::string & name);

        void
        destroyMesh(std::vector<std::unique_ptr<Mesh>> && meshToDestroy);
        void
        destroy();

        Mesh *
        getZArrowMesh();
        Mesh *
        getYArrowMesh();
        Mesh *
        getXArrowMesh();

      private:
        std::vector<Mesh *>
        processLoadedMeshes(const std::string & filename,
                            std::vector<MeshLoadedData> & loadedMeshes);
        std::unique_ptr<Mesh>
        buildMeshFromData(MeshLoadedData & data, unsigned int optionsFlag);
      };
    } // namespace Mesh
  } // namespace CPU
} // namespace RenderLib

#endif
