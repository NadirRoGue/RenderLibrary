#include "CPU/texture/blendoperations/BlendDivide.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace Texture
		{
			void BlendDivide::doBlend(
				std::vector<unsigned char> & firstAndDst,
				std::vector<unsigned char> & second,
				float blendStrength)
			{
				size_t i = 0;
				while (i < firstAndDst.size())
				{
					float a = static_cast<float>(firstAndDst[i]) / 255.0f;
					float b = (static_cast<float>(second[i]) / 255.0f) * blendStrength;

					float fltResult = a / b;

					fltResult = std::min<float>(1.0f, std::max<float>(0.0f, fltResult));
					unsigned char result = static_cast<unsigned char>(fltResult * 255.0f);
					firstAndDst[i] = result;
					i++;
				}
			}
		}
	}
}