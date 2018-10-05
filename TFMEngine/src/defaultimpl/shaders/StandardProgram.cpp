#include "defaultimpl/shaders/StandardProgram.h"

#include <iostream>

namespace RenderLib
{
	namespace DefaultImpl
	{
		StandardProgram::StandardProgram()
			: ShaderProgram()
		{
			vShaderF = "shaders/default/standardprogram.vert";
			fShaderF = "shaders/default/standardprogram.frag";
		}

		StandardProgram::~StandardProgram()
		{

		}

		void StandardProgram::getUberShaderDefines(std::vector<std::string> & definesBuffer)
		{
			
		}

		void StandardProgram::configureShaderAttributes(GPU::Mesh::GPUMesh * targetMesh)
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

		void StandardProgram::onRenderObject(const SceneObject & object, const Material::Material & material, const Camera & camera)
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
			//setUniformTexture("diffuseTexture", material.diffuseTexture.getTexture(), texUnit);
			setUniformTexture("specularTexture", material.specularTexture.getTexture(), texUnit);
			setUniformTexture("ambientTexture", material.ambientTexture.getTexture(), texUnit);
			setUniformTexture("emissiveTexture", material.emissiveTexture.getTexture(), texUnit);
			setUniformTexture("normalTexture", material.normalMapTexture.getTexture(), texUnit);
			setUniformTexture("shininessTexture", material.shininessTexture.getTexture(), texUnit);
			setUniformTexture("opacityTexture", material.opacityTexture.getTexture(), texUnit);
		}
	}
}