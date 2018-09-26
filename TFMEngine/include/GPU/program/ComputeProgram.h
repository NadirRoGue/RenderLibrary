#ifndef __RENDERLIB_GPU_PROGRAM_COMPUTEPROGRAM_H__
#define __RENDERLIB_GPU_PROGRAM_COMPUTEPROGRAM_H__

#include "GPU/program/Program.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			class ComputeProgram : public Program
			{
			public:
				std::string compShaderFile;
			public:
				ComputeProgram(const UberParamMask & params);
				~ComputeProgram();

				void execute(unsigned int xSize, unsigned int ySize, unsigned int zSize);
			};
		}
	}
}

#endif