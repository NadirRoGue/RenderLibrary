#ifndef __ENGINE_DEFINES__
#define __ENGINE_DEFINES__

#include <Eigen/Eigen>

#define __USE_DOUBLE_PRECISSION__

#ifdef __USE_DOUBLE_PRECISSION__
#define FLOAT double
#define MATRIX4 Eigen::Matrix4d
#define MATRIX3 Eigen::Matrix3d
#define VECTOR3 Eigen::Vector3d
#define VECTOR2 Eigen::Vector2d
#define VECTOR4 Eigen::Vector4d
#define QUATERNION Eigen::Quaternion<double>
#else
#define FLOAT float
#define MATRIX4 Eigen::Matrix4f
#define MATRIX3 Eigen::Matrix3f
#define VECTOR3 Eigen::Vector3f
#define VECTOR2 Eigen::Vector2f
#define VECTOR4 Eigen::Vector4f
#define QUATERNION Eigen::Quaternion<float>
#endif

#endif