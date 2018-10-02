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
		}

		void StandardProgram::onRenderObject(const SceneObject & object, const Camera & camera)
		{
			MATRIX4 modelView = camera.viewMatrix * object.transform.worldModelMatrix;
			MATRIX4 modelViewProj = camera.projectionMatrix * modelView;
			MATRIX4 normalMat = modelView.inverse().transpose();

			setUniformMatrix4("modelView", 1, false, modelView.data());
			setUniformMatrix4("modelViewProj", 1, false, modelViewProj.data());
			setUniformMatrix4("normalMat", 1, false, normalMat.data());
		}
	}
}