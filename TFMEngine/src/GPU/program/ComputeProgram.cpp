#include "GPU/program/ComputeProgram.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			ComputeProgram::ComputeProgram(const UberParamMask & params)
				: Program(params)
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