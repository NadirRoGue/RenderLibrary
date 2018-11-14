#include "util/MathUtil.h"

namespace RenderLib
{
  namespace MathUtil
  {
    VECTOR3
    VoronoiTriangleAreas(const VECTOR3 & A, const VECTOR3 & B,
                         const VECTOR3 & C)
    {
      float aArea, bArea, cArea;

      if ((B - A).dot(C - A) < 0)
      {
        float triangleArea = (B - A).cross(C - A).norm() * 0.5f;
        aArea              = 0.5f * triangleArea;
        bArea = cArea = 0.25f * triangleArea;
      }
      else if ((A - B).dot(C - B) < 0)
      {
        float triangleArea = (A - B).cross(C - B).norm() * 0.5f;
        bArea              = 0.5f * triangleArea;
        aArea = cArea = 0.25f * triangleArea;
      }
      else if ((A - C).dot(B - C) < 0)
      {
        float triangleArea = (A - C).cross(B - C).norm() * 0.5f;
        cArea              = 0.5f * triangleArea;
        aArea = bArea = 0.25f * triangleArea;
      }
      else
      {
        float AtoB = (B - A).norm();
        float AtoC = (C - A).norm();
        float BtoC = (C - B).norm();

        float ctngA = Cotangent(A, B, C);
        float ctngB = Cotangent(B, A, C);
        float ctngC = Cotangent(C, A, B);

        aArea = 0.125f * ((AtoB * AtoB * ctngC) + (AtoC * AtoC * ctngB));
        bArea = 0.125f * ((AtoB * AtoB * ctngC) + (BtoC * BtoC * ctngA));
        cArea = 0.125f * ((AtoC * AtoC * ctngB) + (BtoC * BtoC * ctngA));
      }

      return VECTOR3(aArea, bArea, cArea);
    }

    float
    Cotangent(const VECTOR3 & pivot, const VECTOR3 & a, const VECTOR3 & b)
    {
      VECTOR3 pa = a - pivot;
      VECTOR3 pb = b - pivot;

      pa.normalize();
      pb.normalize();

      const float sinA = pa.cross(pb).norm();
      const float cosA = pa.dot(pb);

      return (cosA / sinA);
    }

    VECTOR3
    Tangent(const VECTOR2 & st1, const VECTOR2 st2, const VECTOR3 & Q1,
            const VECTOR3 & Q2)
    {
      const float stDet = 1.0f / ((st1.x() * st2.y()) - (st1.y() * st2.x()));

      VECTOR2 row1(st2.y(), -st1.y());
      //VECTOR2 row2(-st2.x(), st1.x());

      VECTOR2 column1(Q1.x(), Q2.x());
      VECTOR2 column2(Q1.y(), Q2.y());
      VECTOR2 column3(Q1.z(), Q2.z());

      VECTOR3 tangent = stDet
          * VECTOR3(row1.dot(column1), row1.dot(column2), row1.dot(column3));

      return tangent;
    }
  } // namespace MathUtil
} // namespace RenderLib