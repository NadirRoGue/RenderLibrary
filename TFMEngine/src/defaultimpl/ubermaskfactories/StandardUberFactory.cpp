#include "defaultimpl/ubermaskfactories/StandardUberFactory.h"

#include "GPU/program/ShaderProgram.h"
#include <iostream>

namespace RenderLib
{
	namespace DefaultImpl
	{
		GPU::Program::UberParamMask StandardUberFactory::computeMask(DefaultImpl::MeshRenderer * renderable)
		{
			GPU::Program::UberParamMask mask =
				computeMaskForMesh(renderable->gpuMesh);
			
			mask |= computeMaskForMaterial(renderable->material);

			mask |=
				(renderable->preferredRender == DefaultImpl::FORWARD_RENDER || renderable->material->opacity() < 1.0f) ?
				GPU::Program::ShaderProgram::FORWARD_SHADING : 0;

			return mask;
		}

		GPU::Program::UberParamMask StandardUberFactory::computeMaskForMesh(GPU::Mesh::GPUMesh * mesh)
		{
			if (mesh == NULL)
			{
				return 0;
			}

			GPU::Program::UberParamMask meshMask = 0;

			if (mesh->normals.numElements > 0)
			{
				meshMask |= GPU::Program::ShaderProgram::HAS_NORMAL_ATTRIB;
			}

			if (mesh->tangents.numElements > 0)
			{
				meshMask |= GPU::Program::ShaderProgram::HAS_TANGENT_ATTRIB;
			}

			if (mesh->uvs.size() > 0)
			{
				meshMask |= GPU::Program::ShaderProgram::HAS_UV_ATTRIB;
			}

			if (mesh->colors.size() > 0)
			{
				meshMask |= GPU::Program::ShaderProgram::HAS_COLOR_ATTRIB;
			}

			if (mesh->bitangents.numElements > 0)
			{
				meshMask |= GPU::Program::ShaderProgram::HAS_BITANGENT_ATTRIB;
			}

			return meshMask;
		}

		GPU::Program::UberParamMask StandardUberFactory::computeMaskForMaterial(Material::Material * material)
		{
			if (material == NULL)
			{
				return 0;
			}

			GPU::Program::UberParamMask materialMask = 0;

			materialMask |= material->ambientColor.paramExists() ?
				GPU::Program::ShaderProgram::HAS_AMBIENT_COLOR : 0;
			materialMask |= material->diffuseColor.paramExists() ?
				GPU::Program::ShaderProgram::HAS_DIFFUSE_COLOR : 0;
			materialMask |= material->specularColor.paramExists() ?
				GPU::Program::ShaderProgram::HAS_SPECULAR_COLOR : 0;
			materialMask |= material->emissiveColor.paramExists() ?
				GPU::Program::ShaderProgram::HAS_EMISSIVE_COLOR : 0;

			materialMask |= material->shininess.paramExists() ?
				GPU::Program::ShaderProgram::HAS_SHININESS_VALUE : 0;
			materialMask |= material->opacity.paramExists() && material->opacity() < 1.0f ?
				GPU::Program::ShaderProgram::HAS_OPACITY_VALUE : 0;
			materialMask |= material->specularScale.paramExists() ?
				GPU::Program::ShaderProgram::HAS_SPECULAR_SCALE_VALUE : 0;
			materialMask |= material->indexOfRefraction.paramExists() ?
				GPU::Program::ShaderProgram::HAS_IOR_VALUE : 0;

			if (material->diffuseTexture.textureExists())
			{
				std::cout << "Has diffuse" << std::endl;
				materialMask |= GPU::Program::ShaderProgram::HAS_DIFFUSE_TEXTURE;
			}

			if (material->specularTexture.textureExists())
			{
				materialMask |= GPU::Program::ShaderProgram::HAS_SPECULAR_TEXTURE;
			}

			if (material->ambientTexture.textureExists())
			{
				materialMask |= GPU::Program::ShaderProgram::HAS_AMBIENT_TEXTURE;
			}

			if (material->emissiveTexture.textureExists())
			{
				materialMask |= GPU::Program::ShaderProgram::HAS_EMISSIVE_TEXTURE;
			}

			if (material->normalMapTexture.textureExists())
			{
				materialMask |= GPU::Program::ShaderProgram::HAS_NORMALMAP_TEXTURE;
			}

			if (material->shininessTexture.textureExists())
			{
				materialMask |= GPU::Program::ShaderProgram::HAS_SHININESS_TEXTURE;
			}

			if (material->opacityTexture.textureExists())
			{
				materialMask |= GPU::Program::ShaderProgram::HAS_OPACITY_TEXTURE;
			}
			
			if (material->reflectionTexture.textureExists())
			{
				materialMask |= GPU::Program::ShaderProgram::HAS_REFELCTIONMAP_TEXTURE;
			}

			if (material->heightMapTexture.textureExists())
			{
				materialMask |= GPU::Program::ShaderProgram::HAS_HEIGHTMAP_TEXTURE;
			}

			if (material->lightMapTexture.textureExists())
			{
				materialMask |= GPU::Program::ShaderProgram::HAS_LIGHTMAP_TEXTURE;
			}

			if (material->displacementTexture.textureExists())
			{
				materialMask |= GPU::Program::ShaderProgram::HAS_DISPLACEMENTMAP_TEXTURE;
			}

			return materialMask;
		}
	}
}