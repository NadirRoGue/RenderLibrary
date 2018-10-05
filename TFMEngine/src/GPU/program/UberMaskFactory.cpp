#include "GPU/program/UberMaskFactory.h"

#include "EngineException.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			UberFactoryParam::UberFactoryParam()
			{

			}

			UberFactoryParam::~UberFactoryParam()
			{

			}

			UberParamMask UberMaskFactory::computeUberMask(UberFactoryParam * param)
			{
				return 0;
			}

			UberParamMask ShaderUberMaskFactory::computeUberMask(UberFactoryParam * param)
			{
				ShaderUberFactoryParam * shaderParam = dynamic_cast<ShaderUberFactoryParam*>(param);
				if (shaderParam == NULL)
				{
					throw EngineException("ShaderUberMaskFactory: UberFactoryParam is not an instance of ShaderUberFactoryParam");
					return 0;
				}

				return computeUberMaskFromRenderable(shaderParam->renderable);
			}
		}
	}
}