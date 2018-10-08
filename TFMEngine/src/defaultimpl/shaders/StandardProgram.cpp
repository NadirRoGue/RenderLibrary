#include "defaultimpl/shaders/StandardProgram.h"

#include <iostream>

#include "EngineException.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		GPU::Program::UberParamMask StandardProgram::HAS_UV_ATTRIB				
			= 1;
		GPU::Program::UberParamMask StandardProgram::HAS_TANGENT_ATTRIB			
			= 2;
		GPU::Program::UberParamMask StandardProgram::HAS_COLOR_ATTRIB			
			= 4;

		GPU::Program::UberParamMask StandardProgram::HAS_NORMAL_ATTRIB			
			= 8;
		GPU::Program::UberParamMask StandardProgram::HAS_NORMAL_MAP				
			= 16;

		GPU::Program::UberParamMask StandardProgram::HAS_DIFFUSE_COLOR			
			= 32;
		GPU::Program::UberParamMask StandardProgram::HAS_DIFFUSE_TEXTURE		
			= 64;

		GPU::Program::UberParamMask StandardProgram::HAS_SPECULAR_COLOR			
			= 128;
		GPU::Program::UberParamMask StandardProgram::HAS_SPECULAR_TEXTURE		
			= 256;

		GPU::Program::UberParamMask StandardProgram::HAS_AMBIENT_COLOR			
			= 512;
		GPU::Program::UberParamMask StandardProgram::HAS_AMBIENT_TEXTURE		
			= 1024;

		GPU::Program::UberParamMask StandardProgram::HAS_SHININESS_VALUE		
			= 2048;
		GPU::Program::UberParamMask StandardProgram::HAS_SHININESS_TEXTURE		
			= 4096;

		GPU::Program::UberParamMask StandardProgram::HAS_EMISSIVE_COLOR			
			= 4096 * 2;
		GPU::Program::UberParamMask StandardProgram::HAS_EMISSIVE_TEXTURE		
			= 4096 * 4;

		GPU::Program::UberParamMask StandardProgram::HAS_OPACITY				
			= 4096 * 8;
		GPU::Program::UberParamMask StandardProgram::HAS_OPACITY_TEXTURE		
			= 4096 * 16;

		GPU::Program::UberParamMask StandardProgram::HAS_SPECULAR_SCALE			
			= 4096 * 32;

		GPU::Program::UberParamMask StandardProgram::FORWARD_SHADING 
			= 4096 * 64;

		std::unordered_map<GPU::Program::UberParamMask, std::string> StandardProgram::maskToDefines =
		{
			{StandardProgram::HAS_UV_ATTRIB, "ENABLE_UV"},
			{StandardProgram::HAS_TANGENT_ATTRIB, "ENABLE_TANGENT"},
			{StandardProgram::HAS_COLOR_ATTRIB, "ENABLE_COLOR_ATTRIB"},
			{StandardProgram::HAS_NORMAL_ATTRIB, "ENABLE_NORMAL_ATTRIB"},
			{StandardProgram::HAS_NORMAL_MAP, "ENABLE_NORMAL_MAP"},
			{StandardProgram::HAS_DIFFUSE_COLOR, "ENABLE_DIFFUSE_COLOR"},
			{StandardProgram::HAS_DIFFUSE_TEXTURE, "ENABLE_DIFFUSE_TEXTURE"},
			{StandardProgram::HAS_SPECULAR_COLOR, "ENABLE_SPECULAR_COLOR"},
			{StandardProgram::HAS_SPECULAR_TEXTURE, "ENABLE_SPECULAR_TEXTURE"},
			{StandardProgram::HAS_AMBIENT_COLOR, "ENABLE_AMBIENT_COLOR"},
			{StandardProgram::HAS_AMBIENT_TEXTURE, "ENABLE_AMBIENT_TEXTURE"},
			{StandardProgram::HAS_SHININESS_VALUE, "ENABLE_SHININESS_VALUE"},
			{StandardProgram::HAS_SHININESS_TEXTURE, "ENABLE_SHININESS_TEXTURE"},
			{StandardProgram::HAS_EMISSIVE_COLOR, "ENABLE_EMISSIVE_COLOR"},
			{StandardProgram::HAS_EMISSIVE_TEXTURE, "ENABLE_EMISSIVE_TEXTURE"},
			{StandardProgram::HAS_OPACITY, "ENABLE_OPACITY_VALUE"},
			{StandardProgram::HAS_OPACITY_TEXTURE, "ENABLE_OPACITY_TEXTURE"},
			{StandardProgram::HAS_SPECULAR_SCALE, "ENABLE_SPECULAR_SCALE"},
			{StandardProgram::FORWARD_SHADING, "FORWARD_SHADING"}
		};

		StandardProgram::StandardProgram()
			: ShaderProgram()
		{
			vShaderF = "shaders/default/standardprogram.vert";
			fShaderF = "shaders/default/standardprogram.frag";
		}

		StandardProgram::~StandardProgram()
		{

		}

		void StandardProgram::getUberShaderDefines(
			std::vector<std::string> & definesBuffer)
		{
			for (auto it = maskToDefines.begin(); it != maskToDefines.end(); it++)
			{
				if (it->first & configMask)
				{
					definesBuffer.push_back(it->second);
				}
			}
		}

		void StandardProgram::onFrameBegin(EngineInstance * instance)
		{
			setUniformBlock("DLBlock", instance->getGPULightManager().getDirectionalLightBuffer().getBufferId());
			setUniformBlock("PLBlock", instance->getGPULightManager().getPointLightBuffer().getBufferId());
			setUniformBlock("SLBlock", instance->getGPULightManager().getSpotLightBuffer().getBufferId());
		}

		void StandardProgram::configureShaderAttributes(
			GPU::Mesh::GPUMesh * targetMesh)
		{
			configureShaderAttribute("inPos", targetMesh->vertices);
			configureShaderAttribute("inNormal", targetMesh->normals);
			configureShaderAttribute("inTangent", targetMesh->tangents);

			if (targetMesh->uvs.size() > 0)
			{
				configureShaderAttribute("inUV", targetMesh->uvs[0]);
			}

			if (targetMesh->colors.size() > 0)
			{
				configureShaderAttribute("inColor", targetMesh->colors[0]);
			}
		}

		void StandardProgram::onRenderObject(
			const SceneObject & object, 
			const Material::Material & material, 
			const Camera & camera)
		{
			MATRIX4 modelView = camera.viewMatrix * object.transform.worldModelMatrix;
			MATRIX4 modelViewProj = camera.projectionMatrix * modelView;
			MATRIX4 normalMat = modelView.inverse().transpose();
			
			setUniformMatrix4("modelView", 1, false, modelView.data());
			setUniformMatrix4("modelViewProj", 1, false, modelViewProj.data());
			setUniformMatrix4("normalMat", 1, false, normalMat.data());
			
			setUniform3FV("diffuseColor", 1, material.diffuseColor().data());
			setUniform3FV("specularColor", 1, material.specularColor().data());
			setUniform3FV("ambientColor", 1, material.ambientColor().data());
			setUniform3FV("emissiveColor", 1, material.emissiveColor().data());
			setUniformF("opacity", material.opacity());
			setUniformF("shininess", material.shininess());
			setUniformF("specularScale", material.specularScale());
			
			unsigned int texUnit = 0;
			setUniformTexture("diffuseTexture", 
				material.diffuseTexture.getTexture(), texUnit);
			
			setUniformTexture("specularTexture", 
				material.specularTexture.getTexture(), texUnit);
			
			setUniformTexture("ambientTexture", 
				material.ambientTexture.getTexture(), texUnit);
			
			setUniformTexture("emissiveTexture", 
				material.emissiveTexture.getTexture(), texUnit);
			
			setUniformTexture("normalTexture", 
				material.normalMapTexture.getTexture(), texUnit);
			
			setUniformTexture("shininessTexture", 
				material.shininessTexture.getTexture(), texUnit);
			
			setUniformTexture("opacityTexture", 
				material.opacityTexture.getTexture(), texUnit);
		}

		// ===============================================================================================

		GPU::Program::UberParamMask StandardUberFactory::computeMaskFromRenderable(
			DefaultImpl::MeshRenderer * renderable)
		{
			if (renderable == NULL)
			{
				return 0;
			}

			GPU::Program::UberParamMask meshMask = 
				parseMesh(renderable->gpuMesh);

			GPU::Program::UberParamMask materialMask = 
				parseMaterial(meshMask, renderable->material);

			GPU::Program::UberParamMask finalMask = 
				materialMask;

			finalMask |= 
				renderable->preferredRender == DefaultImpl::FORWARD_RENDER ? 
				StandardProgram::FORWARD_SHADING : 0;

			return finalMask;
		}

		GPU::Program::UberParamMask StandardUberFactory::parseMesh(
			GPU::Mesh::GPUMesh * mesh)
		{
			if (mesh == NULL)
			{
				return 0;
			}

			GPU::Program::UberParamMask meshMask = 0;

			if (mesh->normals.numElements > 0)
			{
				meshMask |= StandardProgram::HAS_NORMAL_ATTRIB;
			}

			if (mesh->tangents.numElements > 0)
			{
				meshMask |= StandardProgram::HAS_TANGENT_ATTRIB;
			}

			if (mesh->uvs.size() > 0)
			{
				meshMask |= StandardProgram::HAS_UV_ATTRIB;
			}

			if (mesh->colors.size() > 0)
			{
				meshMask |= StandardProgram::HAS_COLOR_ATTRIB;
			}

			return meshMask;
		}

		GPU::Program::UberParamMask StandardUberFactory::parseMaterial(
			GPU::Program::UberParamMask meshMask, 
			Material::Material * mat)
		{
			if (mat == NULL)
			{
				return 0;
			}

			GPU::Program::UberParamMask materialMask = meshMask;

			materialMask |= mat->ambientColor.paramExists() ? 
				StandardProgram::HAS_AMBIENT_COLOR : 0;
			materialMask |= mat->diffuseColor.paramExists() ? 
				StandardProgram::HAS_DIFFUSE_COLOR : 0;
			materialMask |= mat->specularColor.paramExists() ? 
				StandardProgram::HAS_SPECULAR_COLOR : 0;
			materialMask |= mat->shininess.paramExists() ? 
				StandardProgram::HAS_SHININESS_VALUE : 0;
			materialMask |= mat->opacity.paramExists() && mat->opacity() < 1.0f ? 
				StandardProgram::HAS_OPACITY : 0;
			materialMask |= mat->emissiveColor.paramExists() ? 
				StandardProgram::HAS_EMISSIVE_COLOR : 0;
			materialMask |= mat->specularScale.paramExists() ? 
				StandardProgram::HAS_SPECULAR_SCALE : 0;

			if (materialMask & StandardProgram::HAS_DIFFUSE_COLOR)
			{
				materialMask &= ~StandardProgram::HAS_COLOR_ATTRIB;
			}

			// Only process texture flags if UVs are present
			if (materialMask & StandardProgram::HAS_UV_ATTRIB)
			{
				if (mat->diffuseTexture.textureExists())
				{
					materialMask &= ~StandardProgram::HAS_DIFFUSE_COLOR;
					materialMask |= StandardProgram::HAS_DIFFUSE_TEXTURE;
				}

				if (mat->specularTexture.textureExists())
				{
					materialMask &= ~StandardProgram::HAS_SPECULAR_COLOR;
					materialMask |= StandardProgram::HAS_SPECULAR_TEXTURE;
				}

				if (mat->ambientTexture.textureExists())
				{
					materialMask &= ~StandardProgram::HAS_AMBIENT_COLOR;
					materialMask |= StandardProgram::HAS_AMBIENT_TEXTURE;
				}

				if (mat->emissiveTexture.textureExists())
				{
					materialMask &= ~StandardProgram::HAS_EMISSIVE_COLOR;
					materialMask |= StandardProgram::HAS_EMISSIVE_TEXTURE;
				}

				if (mat->normalMapTexture.textureExists() 
					&& (materialMask & StandardProgram::HAS_TANGENT_ATTRIB))
				{
					materialMask |= StandardProgram::HAS_NORMAL_MAP;
				}
				else if (!mat->normalMapTexture.textureExists())
				{
					materialMask &= ~StandardProgram::HAS_TANGENT_ATTRIB;
				}

				if (mat->shininessTexture.textureExists())
				{
					materialMask &= ~StandardProgram::HAS_SHININESS_VALUE;
					materialMask |= StandardProgram::HAS_SPECULAR_TEXTURE;
				}

				if (mat->opacityTexture.textureExists())
				{
					materialMask &= ~StandardProgram::HAS_OPACITY;
					materialMask |= StandardProgram::HAS_OPACITY_TEXTURE;
				}
			}

			return materialMask;
		}
	}
}