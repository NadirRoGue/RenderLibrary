#ifndef __ENGINE_DEFINES__
#define __ENGINE_DEFINES__

#include <Eigen/Eigen>

//#define USE_DOUBLE_PRECISSION__

#ifdef USE_DOUBLE_PRECISSION__
#define FLOAT double
//#define USE_DOUBLE_PRECISSION_ON_SHADERS__
#else
#define FLOAT float
#endif

#define VECTOR2 Eigen::Matrix<FLOAT, 2, 1>
#define VECTOR3 Eigen::Matrix<FLOAT, 3, 1>
#define VECTOR4 Eigen::Matrix<FLOAT, 4, 1>

#define MATRIX3 Eigen::Matrix<FLOAT, 3, 3>
#define MATRIX4 Eigen::Matrix<FLOAT, 4, 4>

#define QUATERNION Eigen::Quaternion<FLOAT>
#define ANGLEAXIS Eigen::AngleAxis<FLOAT>

#define IVECTOR3 Eigen::Vector3i

#define TRANSFORM Eigen::Transform<FLOAT, 3, Eigen::Affine, Eigen::ColMajor>

#define MAX_UV_MAPS_PER_MESH 4
#define MAX_COLOR_LAYER_PER_MESH 8

/** MEMORY **/
#define NEW_MEMORY_POOL_DEFAULT_SIZE 209715200

/** GLSL Optimization */
//#define USE_PACKED_ATTRIB_WHEN_POSSIBLE
#define MAX_DIRECTIONAL_LIGHTS 1
#define MAX_POINT_LIGHTS 20
#define MAX_SPOT_LIGHTS 20

#endif