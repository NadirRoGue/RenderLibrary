#include "GPU/program/ComputeProgram.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			ComputeProgram::ComputeProgram(const ComputeProgramParams & params)
				: Program(params)
				, compShaderFile(params.compShaderFile)
			{
			}

			ComputeProgram::~ComputeProgram()
			{
			}

			void ComputeProgram::execute(unsigned int xSize, unsigned int ySize, unsigned int zSize)
			{

			}
		}
	}
}