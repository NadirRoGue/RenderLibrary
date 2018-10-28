#ifndef __RENDERLIB_GPU_PROGRAM_SHADERPROGRAM_H__
#define __RENDERLIB_GPU_PROGRAM_SHADERPROGRAM_H__

#include "GPU/mesh/GPUMesh.h"
#include "GPU/program/Program.h"

#include "material/Material.h"

#include "SceneObject.h"
#include "Camera.h"

#include "EngineInstance.h"

namespace RenderLib
{
	namespace GPU
	{
		namespace Program
		{
			class ShaderProgram : public Program
			{
			public:
				// Mesh data
				static GPU::Program::UberParamMask HAS_NORMAL_ATTRIB;
				static GPU::Program::UberParamMask HAS_TANGENT_ATTRIB;
				static GPU::Program::UberParamMask HAS_BITANGENT_ATTRIB;
				static GPU::Program::UberParamMask HAS_UV_ATTRIB;
				static GPU::Program::UberParamMask HAS_COLOR_ATTRIB;

				// Material colors
				static GPU::Program::UberParamMask HAS_DIFFUSE_COLOR;
				static GPU::Program::UberParamMask HAS_AMBIENT_COLOR;
				static GPU::Program::UberParamMask HAS_SPECULAR_COLOR;
				static GPU::Program::UberParamMask HAS_EMISSIVE_COLOR;
				static GPU::Program::UberParamMask HAS_TRANSPARENT_COLOR;

				// Materials factors
				static GPU::Program::UberParamMask HAS_SHININESS_VALUE;
				static GPU::Program::UberParamMask HAS_OPACITY_VALUE;
				static GPU::Program::UberParamMask HAS_SPECULAR_SCALE_VALUE;
				static GPU::Program::UberParamMask HAS_IOR_VALUE;

				// Material textures
				static GPU::Program::UberParamMask HAS_NORMALMAP_TEXTURE;
				static GPU::Program::UberParamMask HAS_HEIGHTMAP_TEXTURE;
				static GPU::Program::UberParamMask HAS_DISPLACEMENTMAP_TEXTURE;
				static GPU::Program::UberParamMask HAS_REFELCTIONMAP_TEXTURE;
				static GPU::Program::UberParamMask HAS_LIGHTMAP_TEXTURE;
				static GPU::Program::UberParamMask HAS_DIFFUSE_TEXTURE;
				static GPU::Program::UberParamMask HAS_SPECULAR_TEXTURE;
				static GPU::Program::UberParamMask HAS_AMBIENT_TEXTURE;
				static GPU::Program::UberParamMask HAS_SHININESS_TEXTURE;
				static GPU::Program::UberParamMask HAS_EMISSIVE_TEXTURE;
				static GPU::Program::UberParamMask HAS_OPACITY_TEXTURE;
				static GPU::Program::UberParamMask HAS_OTHER_TEXTURE;

				static GPU::Program::UberParamMask FORWARD_SHADING;

				static std::unordered_map<GPU::Program::UberParamMask, std::string> maskToDefines;
			protected:
				std::string vShaderF;
				std::string tcShaderF;
				std::string teShaderF;
				std::string geomShaderF;
				std::string fShaderF;
			private:
				unsigned int vShader;
				unsigned int tcShader;
				unsigned int teShader;
				unsigned int geomShader;
				unsigned int fShader;
			public:
				ShaderProgram();
				~ShaderProgram();

				virtual void getUberShaderDefines(std::vector<std::string> & buffer);
				virtual void initialize(std::vector<std::string> & definesBuffer);
				
				void destroyShaders();

				virtual void onFrameBegin(EngineInstance * instance);

				virtual void configureMeshParameters(GPU::Mesh::GPUMesh & gpuMesh);
				virtual void sendMaterialParameters(const Material::Material & material);
				virtual void sendTransformParameters(const SceneObject & object, const Camera & camera);

				virtual void onRenderObject(DefaultImpl::MeshRenderer & renderable, const Camera & camera);
			};
		}
	}
}

#endif