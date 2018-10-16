#include "defaultimpl/shaders/PickingProgram.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		PickingProgram::PickingProgram()
			: GPU::Program::ShaderProgram()
		{
			vShaderF = "shaders/default/picking.vert";
			fShaderF = "shaders/default/picking.frag";
		}

		PickingProgram::~PickingProgram()
		{

		}

		void PickingProgram::configureShaderAttributes(GPU::Mesh::GPUMesh * targetMesh)
		{
			configureShaderAttribute("inPos", targetMesh->vertices);
		}

		void PickingProgram::onRenderObject(const SceneObject & object, const Material::Material & material, const Camera & camera)
		{
			MATRIX4 modelViewProj = camera.projectionMatrix * camera.viewMatrix * object.transform.worldModelMatrix;
			setUniformMatrix4("modelViewProj", 1, false, modelViewProj.data());
		}
	}
}