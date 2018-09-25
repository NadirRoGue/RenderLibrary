#ifndef __RENDERLIB_GPU_PROGRAM_PROGRAM_H__
#define __RENDERLIB_GPU_PROGRAM_PROGRAM_H__

#include <memory>
#include <map>
#include <vector>

#include "Defines.h"

#include "EngineInstance.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			typedef unsigned long long UberParamMask;

			typedef struct ProgramParams
			{
				UberParamMask mask;

				/* 
				 * Optional callback to be executed once every render loop iteration.
				 * Can be used to set up common global uniforms across all objects
				 */
				std::function<void(const EngineInstance & instance)> renderIterationInit;

				ProgramParams()
					: mask(0)
				{

				}

			} ProgramParams;

			class Program
			{
			private:
				UberParamMask configMask;
				std::function<void(const EngineInstance & instance)> renderInitCallback;
			public:
				unsigned int programId;
			public:
				Program(const ProgramParams & config);
				~Program();

				virtual void getUberShaderDefines(std::vector<std::string> & definesBuffer);

				void bind();
				void unBind();

				void executePreRenderCallback(const EngineInstance & instance);

				void setUniformI (const unsigned int & id, const int & val);
				void setUniformI2(const unsigned int & id, const int & v1, const int & v2);
				void setUniformI3(const unsigned int & id, const int & v1, const int & v2, const int & v3);
				void setUniformI4(const unsigned int & id, const int & v1, const int & v2, const int & v3, const int & v4);
				void setUniformI2V(const unsigned int & id, const unsigned int & count, const int * val);
				void setUniformI3V(const unsigned int & id, const unsigned int & count, const int * val);
				void setUniformI4V(const unsigned int & id, const unsigned int & count, const int * val);
				void setUniformF (const unsigned int & id, const FLOAT & val);
				void setUniform2F(const unsigned int & id, const FLOAT & v1, const FLOAT & v2);
				void setUniform3F(const unsigned int & id, const FLOAT & v1, const FLOAT & v2, const FLOAT & v3);
				void setUniform4F(const unsigned int & id, const FLOAT & v1, const FLOAT & v2, const FLOAT & v3, const FLOAT & v4);
				void setUniform2FV(const unsigned int & id, const unsigned int & count, const FLOAT * val);
				void setUniform3FV(const unsigned int & id, const unsigned int & count, const FLOAT * val);
				void setUniform4FV(const unsigned int & id, const unsigned int & count, const FLOAT * val);
				void setUniformUI (const unsigned int & id, const unsigned int & val);
				void setUniformUI2(const unsigned int & id, const unsigned int & v1, const unsigned int & v2);
				void setUniformUI3(const unsigned int & id, const unsigned int & v1, const unsigned int & v2, const unsigned int & v3);
				void setUniformUI4(const unsigned int & id, const unsigned int & v1, const unsigned int & v2, const unsigned int & v3, const unsigned int & v4);
				void setUniformUI2V(const unsigned int & id, const unsigned int & count, const unsigned int * val);
				void setUniformUI3V(const unsigned int & id, const unsigned int & count, const unsigned int * val);
				void setUniformUI4V(const unsigned int & id, const unsigned int & count, const unsigned int * val);
				void setUniformMatrix2(const unsigned int & id, const unsigned int & count, const bool & transpose, const FLOAT * val);
				void setUniformMatrix3(const unsigned int & id, const unsigned int & count, const bool & transpose, const FLOAT * val);
				void setUniformMatrix4(const unsigned int & id, const unsigned int & count, const bool & transpose, const FLOAT * val);
				
				
			};

			class ProgramFactory
			{
			private:
				std::map<UberParamMask, std::unique_ptr<Program>> shaderCache;
			public:
				ProgramFactory()
				{
				}

				~ProgramFactory()
				{
				}

				template<class T>
				T * createProgram(UberParamMask mask)
				{
					auto it = shaderCache.find(mask);
					if (it != shaderCache.end())
					{
						return it->second.get();
					}

					std::unique_ptr<Program> newProgram = std::make_unique<T>(mask);
					T * result = dynamic_cast<T*>(newProgram.get());
					shaderCache[mask] = std::move(newProgram);

					return result;
				}
			};
		}
	}
}

#endif