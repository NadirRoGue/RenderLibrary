#ifndef __RENDERLIB_GPU_LIGHT_LIGHTBUFFER_H__
#define __RENDERLIB_GPU_LIGHT_LIGHTBUFFER_H__

namespace RenderLib
{
	namespace GPU
	{
		namespace Light
		{
			class LightBuffer
			{
			private:
				unsigned int bufferId;
			public:
				LightBuffer();
				~LightBuffer();
				unsigned int getBufferId();
				void generate();
				void bind();
				void unBind();
				void setData(char * data, size_t sizeBytes);
				void destroy();
			};
		}
	}
}

#endif