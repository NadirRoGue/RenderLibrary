#ifndef __RENDERLIB_DEFAULTIMPL_STANDARDUBERFACTORY_H__
#define __RENDERLIB_DEFAULTIMPL_STANDARDUBERFACTORY_H__

#include "GPU/program/UberMaskFactory.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    class StandardUberFactory : public GPU::Program::UberMaskFactory
    {
    public:
      GPU::Program::UberParamMask
      computeMask(DefaultImpl::MeshRenderer * renderable);

    private:
      GPU::Program::UberParamMask
      computeMaskForMesh(GPU::Mesh::GPUMesh * mesh);
      GPU::Program::UberParamMask
      computeMaskForMaterial(Material::Material * material);
    };
  } // namespace DefaultImpl
} // namespace RenderLib

#endif