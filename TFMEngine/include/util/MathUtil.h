#ifndef __RENDERLIB_MATHUTIL_H__
#define __RENDERLIB_MATHUTIL_H__

#include "Defines.h"

namespace RenderLib
{
	namespace MathUtil
	{
		VECTOR3 VoronoiTriangleAreas(const VECTOR3 &A, const VECTOR3 &B, const VECTOR3 &C);
		float Cotangent(const VECTOR3 &pivot, const VECTOR3 &a, const VECTOR3 &b);
		VECTOR3 Tangent(const VECTOR2 &st1, const VECTOR2 st2, const VECTOR3 &Q1, const VECTOR3 &Q2);
	}
}

#endif