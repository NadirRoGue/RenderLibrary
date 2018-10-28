#include "CPU/mesh/MeshUtil.h"

#include "util/MathUtil.h"

namespace RenderLib
{
	namespace CPU
	{
		namespace Mesh
		{
			void ComputeNormals(Mesh * mesh)
			{
				if (mesh->normals.size() == 0)
				{
					return;
				}

				std::vector<VECTOR3> perfaceNormals;
				perfaceNormals.resize(mesh->getNumVertices(), VECTOR3(0, 0, 0));

				std::vector<float> voronoiArea;
				voronoiArea.resize(mesh->getNumVertices(), 0.0f);

				for (unsigned int i = 0; i < mesh->getNumFaces(); i++)
				{
					const int start = i * 3;

					IVECTOR3 face = mesh->faces[i];
					// Vertex indexes
					unsigned int raw_a = face.x();
					unsigned int raw_b = face.y();
					unsigned int raw_c = face.z();

					// Vertex indexes within our array
					unsigned int a = raw_a;
					unsigned int b = raw_b;
					unsigned int c = raw_c;

					VECTOR3 A = mesh->vertices[a];
					VECTOR3 B = mesh->vertices[b];
					VECTOR3 C = mesh->vertices[c];
					
					VECTOR3 faceNormal = (B - A).cross(C - A).normalized();

					VECTOR3 faceAreas = MathUtil::VoronoiTriangleAreas(A, B, C);

					perfaceNormals[raw_a] += faceNormal * faceAreas.x();
					perfaceNormals[raw_b] += faceNormal * faceAreas.y();
					perfaceNormals[raw_c] += faceNormal * faceAreas.z();

					voronoiArea[raw_a] += faceAreas.x();
					voronoiArea[raw_b] += faceAreas.y();
					voronoiArea[raw_c] += faceAreas.z();
				}

				for (unsigned int i = 0; i < mesh->getNumVertices(); i++)
				{
					perfaceNormals[i] /= voronoiArea[i];

					VECTOR3 normal = perfaceNormals[i].normalized();

					mesh->normals[i] = normal;
				}
			}

			void ComputeTangents(Mesh * mesh)
			{
				if (mesh->uvs.size() < 1 || mesh->tangents.size() == 0)
				{
					return;
				}
				
				std::vector<VECTOR3> perFaceTangents;
				perFaceTangents.resize(mesh->getNumVertices(), VECTOR3(0, 0, 0));

				std::vector<float> voronoiArea;
				voronoiArea.resize(mesh->getNumVertices(), 0.0f);

				for (unsigned int i = 0; i < mesh->getNumFaces(); i++)
				{
					const int start = i * 3;

					IVECTOR3 face = mesh->faces[i];
					// Vertex indexes
					unsigned int raw_a = face.x();
					unsigned int raw_b = face.y();
					unsigned int raw_c = face.z();

					// Vertex indexes within our array
					unsigned int a = raw_a;
					unsigned int b = raw_b;
					unsigned int c = raw_c;

					VECTOR3 VA = mesh->vertices[a];
					VECTOR3 VB = mesh->vertices[b];
					VECTOR3 VC = mesh->vertices[c];

					VECTOR2 A = mesh->uvs[0][a];
					VECTOR2 B = mesh->uvs[0][b];
					VECTOR2 C = mesh->uvs[0][c];

				  VECTOR2 st1A(B.x() - A.x(), B.y() - A.y());
					VECTOR2 st2A(C.x() - A.x(), C.y() - A.y());

					VECTOR2 st1B(A.x() - B.x(), A.y() - B.y());
					VECTOR2 st2B(C.x() - B.x(), C.y() - B.y());

					VECTOR2 st1C(A.x() - C.x(), A.y() - C.y());
					VECTOR2 st2C(B.x() - C.x(), B.y() - C.y());

					VECTOR3 tangentA = MathUtil::Tangent(st1A, st2A, VB - VA, VC - VA).normalized();
					VECTOR3 tangentB = MathUtil::Tangent(st1B, st2B, VA - VB, VC - VB).normalized();
					VECTOR3 tangentC = MathUtil::Tangent(st1C, st2C, VA - VC, VB - VC).normalized();

					VECTOR3 faceAreas = MathUtil::VoronoiTriangleAreas(VA, VB, VC);

					perFaceTangents[raw_a] += tangentA * faceAreas.x();
					perFaceTangents[raw_b] += tangentB * faceAreas.y();
					perFaceTangents[raw_c] += tangentC * faceAreas.z();

					voronoiArea[raw_a] += faceAreas.x();
					voronoiArea[raw_b] += faceAreas.y();
					voronoiArea[raw_c] += faceAreas.z();
				}

				for (unsigned int i = 0; i < mesh->getNumVertices(); i++)
				{
					perFaceTangents[i] /= voronoiArea[i];

					VECTOR3 tang = perFaceTangents[i].normalized();

					mesh->tangents[i] = tang;
				}
			}
		}
	}
}