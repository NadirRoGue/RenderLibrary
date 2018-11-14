#ifndef __RENDERLIB_RENDER_RENDERABLEMAP_H__
#define __RENDERLIB_RENDER_RENDERABLEMAP_H__

#include <unordered_map>
#include <vector>

#include "GPU/program/ProgramManager.h"
#include "GPU/program/ShaderProgram.h"

#include "Camera.h"

#include "defaultimpl/components/MeshRenderer.h"

#include "EngineInstance.h"

namespace RenderLib
{
  namespace Render
  {
    typedef struct RenderableStub
    {
      GPU::Program::ShaderProgram * program;
      std::vector<DefaultImpl::MeshRenderer *> renderables;

    } RenderableStub;

    class RenderableMap
    {
    private:
      std::string mapRenderOwnerName;
      std::unordered_map<
          std::type_index,
          std::unordered_map<GPU::Program::UberParamMask, RenderableStub>>
          renderablesMap;
      std::vector<DefaultImpl::MeshRenderer *> allRenderables;

    public:
      RenderableMap();
      ~RenderableMap();

      size_t
      getSize();

      void
      setName(const std::string & name);
      const std::string &
      getName();

      void
      addRenderable(GPU::Program::ProgramManager & programManager,
                    DefaultImpl::MeshRenderer * renderable);

      const std::vector<DefaultImpl::MeshRenderer *> &
      getAllRenderables();

      void
      initializeMap();
      void
      renderMap(const Camera & fromCamera, EngineInstance * instance);
    };
  } // namespace Render
} // namespace RenderLib

#endif