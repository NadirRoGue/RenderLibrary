#include "GPU/program/Program.h"

#include "GL/glew.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			Program::Program(const ProgramParams & config)
				: configMask(config.mask)
				, renderInitCallback(config.renderIterationInit)
			{

			}

			Program::~Program()
			{

			}

			void Program::getUberShaderDefines(std::vector<std::string> & definesBuffer)
			{

			}

			void Program::executePreRenderCallback(const EngineInstance & instance)
			{
				if (renderInitCallback)
				{
					renderInitCallback(instance);
				}
			}

			void Program::bind()
			{
				glUseProgram(programId);
			}

			void Program::unBind()
			{
				glUseProgram(0);
			}

			void Program::setUniformI(const unsigned int & id, const int & val)
			{
				glUniform1i(id, val);
			}

			void Program::setUniformI2(const unsigned int & id, const int & v1, const int & v2)
			{
				glUniform2i(id, v1, v2);
			}

			void Program::setUniformI3(const unsigned int & id, const int & v1, const int & v2, const int & v3)
			{
				glUniform3i(id, v1, v2, v3);
			}

			void Program::setUniformI4(const unsigned int & id, const int & v1, const int & v2, const int & v3, const int & v4)
			{
				glUniform4i(id, v1, v2, v3, v4);
			}

			void Program::setUniformI2V(const unsigned int & id, const unsigned int & count, const int * val)
			{
				glUniform2iv(id, count, val);
			}

			void Program::setUniformI3V(const unsigned int & id, const unsigned int & count, const int * val)
			{
				glUniform3iv(id, count, val);
			}

			void Program::setUniformI4V(const unsigned int & id, const unsigned int & count, const int * val)
			{
				glUniform4iv(id, count, val);
			}

			void Program::setUniformF(const unsigned int & id, const FLOAT & val)
			{
#ifdef USE_DOUBLE_PRECISSION_ON_SHADERS__
				glUniform1d(id, val);
#else
				glUniform1f(id, val);
#endif
			}

			void Program::setUniform2F(const unsigned int & id, const FLOAT & v1, const FLOAT & v2)
			{
#ifdef USE_DOUBLE_PRECISSION_ON_SHADERS__
				glUniform2d(id, v1, v2);
#else
				glUniform2f(id, v1, v2);
#endif
			}

			void Program::setUniform3F(const unsigned int & id, const FLOAT & v1, const FLOAT & v2, const FLOAT & v3)
			{
#ifdef USE_DOUBLE_PRECISSION_ON_SHADERS__
				glUniform3d(id, v1, v2, v3);
#else
				glUniform3f(id, v1, v2, v3);
#endif
			}

			void Program::setUniform4F(const unsigned int & id, const FLOAT & v1, const FLOAT & v2, const FLOAT & v3, const FLOAT & v4)
			{
#ifdef USE_DOUBLE_PRECISSION_ON_SHADERS__
				glUniform4d(id, v1, v2, v3, v4);
#else
				glUniform4f(id, v1, v2, v3, v4);
#endif
			}

			void Program::setUniform2FV(const unsigned int & id, const unsigned int & count, const FLOAT * val)
			{
#ifdef USE_DOUBLE_PRECISSION_ON_SHADERS__
				glUniform2dv(id, count, val);
#else
				glUniform2fv(id, count, val);
#endif
			}

			void Program::setUniform3FV(const unsigned int & id, const unsigned int & count, const FLOAT * val)
			{
#ifdef USE_DOUBLE_PRECISSION_ON_SHADERS__
				glUniform3dv(id, count, val);
#else
				glUniform3fv(id, count, val);
#endif
			}

			void Program::setUniform4FV(const unsigned int & id, const unsigned int & count, const FLOAT * val)
			{
#ifdef USE_DOUBLE_PRECISSION_ON_SHADERS__
				glUniform4dv(id, count, val);
#else
				glUniform4fv(id, count, val);
#endif
			}

			void Program::setUniformUI(const unsigned int & id, const unsigned int & val)
			{
				glUniform1ui(id, val);
			}

			void Program::setUniformUI2(const unsigned int & id, const unsigned int & v1, const unsigned int & v2)
			{
				glUniform2ui(id, v1, v2);
			}

			void Program::setUniformUI3(const unsigned int & id, const unsigned int & v1, const unsigned int & v2, const unsigned int & v3)
			{
				glUniform3ui(id, v1, v2, v3);
			}

			void Program::setUniformUI4(const unsigned int & id, const unsigned int & v1, const unsigned int & v2, const unsigned int & v3, const unsigned int & v4)
			{
				glUniform4ui(id, v1, v2, v3, v4);
			}

			void Program::setUniformUI2V(const unsigned int & id, const unsigned int & count, const unsigned int * val)
			{
				glUniform2uiv(id, count, val);
			}

			void Program::setUniformUI3V(const unsigned int & id, const unsigned int & count, const unsigned int * val)
			{
				glUniform3uiv(id, count, val);
			}

			void Program::setUniformUI4V(const unsigned int & id, const unsigned int & count, const unsigned int * val)
			{
				glUniform4uiv(id, count, val);
			}

			void Program::setUniformMatrix2(const unsigned int & id, const unsigned int & count, const bool & transpose, const FLOAT * val)
			{
#ifdef USE_DOUBLE_PRECISSION_ON_SHADERS__
				glUniformMatrix2dv(id, count, transpose, val);
#else
				glUniformMatrix2fv(id, count, transpose, val);
#endif
			}

			void Program::setUniformMatrix3(const unsigned int & id, const unsigned int & count, const bool & transpose, const FLOAT * val)
			{
#ifdef USE_DOUBLE_PRECISSION_ON_SHADERS__
				glUniformMatrix3dv(id, count, transpose, val);
#else
				glUniformMatrix3fv(id, count, transpose, val);
#endif
			}

			void Program::setUniformMatrix4(const unsigned int & id, const unsigned int & count, const bool & transpose, const FLOAT * val)
			{
#ifdef USE_DOUBLE_PRECISSION_ON_SHADERS__
				glUniformMatrix4dv(id, count, transpose, val);
#else
				glUniformMatrix4fv(id, count, transpose, val);
#endif
			}
		}
	}
}