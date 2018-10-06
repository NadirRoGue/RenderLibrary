#include "defaultimpl/pipelinestages/ShaderCompilationStage.h"

#include "EngineInstance.h"

#include "GPU/program/UberFactoryManager.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		void ShaderCompilationStage::preRunStage()
		{
			// Compile all shaders present at beginning
			compileShaders();
		}

		void ShaderCompilationStage::runStage()
		{
			// Check for newly added renderables
			compileShaders();
		}

		void ShaderCompilationStage::compileShaders()
		{
			if (elements.size() > 0)
			{
				for (auto component : elements)
				{
					DefaultImpl::MeshRenderer * renderable 
						= static_cast<DefaultImpl::MeshRenderer*>(component);

					std::type_index shaderType = renderable->material->getShaderType();
					
					GPU::Program::ShaderUberFactoryParam param;
					param.renderable = renderable;

					GPU::Program::AbstractMaskFactory * factory = 
						GPU::Program::UberFactoryManager::getMaskFactory(shaderType);

					// Compile if not done already. Return already compiled shader if present
					GPU::Program::Program * prog = 
						factory->createProgram(engineInstance->getProgramManager(), &param);

					if (prog != NULL)
					{
						renderable->material->setUberMask(prog->getConfigMask());
					}
				}

				elements.clear();
			}
		}
	}
}