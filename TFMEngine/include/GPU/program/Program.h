#ifndef __RENDERLIB_GPU_PROGRAM_PROGRAM_H__
#define __RENDERLIB_GPU_PROGRAM_PROGRAM_H__

#include <memory>
#include <unordered_map>
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

			typedef struct ShaderInput
			{
				unsigned int id;
				GLint size;
				GLenum type;
			} ShaderInput;

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
			protected:
				std::unordered_map<std::string, ShaderInput> shaderUniforms;
				std::unordered_map<std::string, ShaderInput> shaderAttributes;
			public:
				unsigned int programId;
			public:
				Program(const ProgramParams & config);
				~Program();

				virtual void getUberShaderDefines(std::vector<std::string> & definesBuffer);

				void bind();
				void unBind();

				void initialize();

				void executePreRenderCallback(const EngineInstance & instance);

				void setUniformI (const std::string & name, const int & val);
				void setUniformI2(const std::string & name, const int & v1, const int & v2);
				void setUniformI3(const std::string & name, const int & v1, const int & v2, const int & v3);
				void setUniformI4(const std::string & name, const int & v1, const int & v2, const int & v3, const int & v4);
				void setUniformI2V(const std::string & name, const unsigned int & count, const int * val);
				void setUniformI3V(const std::string & name, const unsigned int & count, const int * val);
				void setUniformI4V(const std::string & name, const unsigned int & count, const int * val);
				void setUniformF (const std::string & name, const FLOAT & val);
				void setUniform2F(const std::string & name, const FLOAT & v1, const FLOAT & v2);
				void setUniform3F(const std::string & name, const FLOAT & v1, const FLOAT & v2, const FLOAT & v3);
				void setUniform4F(const std::string & name, const FLOAT & v1, const FLOAT & v2, const FLOAT & v3, const FLOAT & v4);
				void setUniform2FV(const std::string & name, const unsigned int & count, const FLOAT * val);
				void setUniform3FV(const std::string & name, const unsigned int & count, const FLOAT * val);
				void setUniform4FV(const std::string & name, const unsigned int & count, const FLOAT * val);
				void setUniformUI (const std::string & name, const unsigned int & val);
				void setUniformUI2(const std::string & name, const unsigned int & v1, const unsigned int & v2);
				void setUniformUI3(const std::string & name, const unsigned int & v1, const unsigned int & v2, const unsigned int & v3);
				void setUniformUI4(const std::string & name, const unsigned int & v1, const unsigned int & v2, const unsigned int & v3, const unsigned int & v4);
				void setUniformUI2V(const std::string & name, const unsigned int & count, const unsigned int * val);
				void setUniformUI3V(const std::string & name, const unsigned int & count, const unsigned int * val);
				void setUniformUI4V(const std::string & name, const unsigned int & count, const unsigned int * val);
				void setUniformMatrix2(const std::string & name, const unsigned int & count, const bool & transpose, const FLOAT * val);
				void setUniformMatrix3(const std::string & name, const unsigned int & count, const bool & transpose, const FLOAT * val);
				void setUniformMatrix4(const std::string & name, const unsigned int & count, const bool & transpose, const FLOAT * val);
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