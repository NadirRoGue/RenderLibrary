#ifndef __RENDERLIB_GPU_PROGRAM_UBERMASKFACTORY_H__
#define __RENDERLIB_GPU_PROGRAM_UBERMASKFACTORY_H__

#include "Program.h"

#include "defaultimpl/components/MeshRenderer.h"


namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			class UberFactoryParam
			{
			public:
				UberFactoryParam();
				virtual ~UberFactoryParam();
			};

			class UberMaskFactory
			{
			public:
				virtual UberParamMask computeUberMask(UberFactoryParam * param);
			};

			class ShaderUberFactoryParam : public UberFactoryParam
			{
			public:
				DefaultImpl::MeshRenderer * renderable;
			};

			class ShaderUberMaskFactory : public UberMaskFactory
			{
			public:
				UberParamMask computeUberMask(UberFactoryParam * param);
				virtual UberParamMask computeUberMaskFromRenderable(DefaultImpl::MeshRenderer * renderable) = 0;
			};
		}
	}
}

#endif