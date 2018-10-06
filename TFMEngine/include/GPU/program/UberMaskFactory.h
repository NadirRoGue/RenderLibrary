#ifndef __RENDERLIB_GPU_PROGRAM_UBERMASKFACTORY_H__
#define __RENDERLIB_GPU_PROGRAM_UBERMASKFACTORY_H__

#include "Program.h"

#include "defaultimpl/components/MeshRenderer.h"

#include "GPU/program/Program.h"
#include "GPU/program/ProgramManager.h"

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
				virtual ~UberFactoryParam(); // Needed for dynamic_cast
			};

			class ShaderUberFactoryParam : public UberFactoryParam
			{
			public:
				DefaultImpl::MeshRenderer * renderable;
			};

			class AbstractMaskFactory
			{
			public:
				virtual Program * createProgram(ProgramManager & programFactory, UberFactoryParam * param) = 0;
				virtual UberParamMask computeUberMask(UberFactoryParam * param) = 0;
			};

			template<class ProgramType>
			class UberMaskFactory : public AbstractMaskFactory
			{
			public:
				Program * createProgram(ProgramManager & programFactory, UberFactoryParam * param)
				{
					UberParamMask mask = computeUberMask(param);

					return programFactory.getProgram<ProgramType>(mask);
				}

				virtual UberParamMask computeUberMask(UberFactoryParam * param = NULL)
				{
					return 0;
				}
			};

			template<class ProgramType>
			class AbstractShaderUberMaskFactory : public UberMaskFactory<ProgramType>
			{
			public:
				virtual UberParamMask computeUberMask(UberFactoryParam * param = NULL)
				{
					ShaderUberFactoryParam * shaderParam = dynamic_cast<ShaderUberFactoryParam*>(param);
					if (shaderParam == NULL)
					{
						throw EngineException("AbstractShaderUberMaskFactory: Attempted to create a ShaderProgram with a non ShaderUberFactoryParam!");
						return 0;
					}

					return computeMaskFromRenderable(shaderParam->renderable);
				}

				virtual UberParamMask computeMaskFromRenderable(DefaultImpl::MeshRenderer * renderable) = 0;
			};
		}
	}
}

#endif