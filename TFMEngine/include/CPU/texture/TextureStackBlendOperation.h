#ifndef __RENDERLIB_CPU_TEXTURE_TEXTURESTACKBLENDOPERATION_H__
#define __RENDERLIB_CPU_TEXTURE_TEXTURESTACKBLENDOPERATION_H__

namespace RenderLib
{
	namespace CPU
	{
		namespace Texture
		{
			enum class TextureStackBlendOperation
			{
				OP_ADD,
				OP_SUBSTRACT,
				OP_MULTIPLY,
				OP_DIVIDE,
				OP_SMOOTHADD,
				OP_SIGNEDADD
			};
		}
	}
}

#endif