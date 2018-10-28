#ifndef __RENDERLIB_GPU_PROGRAM_UBERMASKFACTORY_H__
#define __RENDERLIB_GPU_PROGRAM_UBERMASKFACTORY_H__

#include "defaultimpl/components/MeshRenderer.h"

#include "GPU/program/Program.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			class UberMaskFactory
			{
			public:
				UberMaskFactory();
				~UberMaskFactory();

				virtual UberParamMask computeMask(DefaultImpl::MeshRenderer * renderable) = 0;
			};
		}
	}
}

#endif