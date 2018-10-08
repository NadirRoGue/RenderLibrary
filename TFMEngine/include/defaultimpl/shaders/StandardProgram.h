#ifndef __RENDERLIB_DEFAULTIMPL_STANDARDPROGRAM_H__
#define __RENDERLIB_DEFAULTIMPL_STANDARDPROGRAM_H__

#include "GPU/program/ShaderProgram.h"

#include "GPU/program/UberMaskFactory.h"

#include "GPU/mesh/GPUMesh.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		class StandardProgram : public GPU::Program::ShaderProgram
		{
		public:
			static GPU::Program::UberParamMask HAS_UV_ATTRIB;
			static GPU::Program::UberParamMask HAS_TANGENT_ATTRIB;
			static GPU::Program::UberParamMask HAS_COLOR_ATTRIB;

			static GPU::Program::UberParamMask HAS_NORMAL_ATTRIB;
			static GPU::Program::UberParamMask HAS_NORMAL_MAP;

			static GPU::Program::UberParamMask HAS_DIFFUSE_COLOR;
			static GPU::Program::UberParamMask HAS_DIFFUSE_TEXTURE;

			static GPU::Program::UberParamMask HAS_SPECULAR_COLOR;
			static GPU::Program::UberParamMask HAS_SPECULAR_TEXTURE;

			static GPU::Program::UberParamMask HAS_AMBIENT_COLOR;
			static GPU::Program::UberParamMask HAS_AMBIENT_TEXTURE;

			static GPU::Program::UberParamMask HAS_SHININESS_VALUE;
			static GPU::Program::UberParamMask HAS_SHININESS_TEXTURE;

			static GPU::Program::UberParamMask HAS_EMISSIVE_COLOR;
			static GPU::Program::UberParamMask HAS_EMISSIVE_TEXTURE;

			static GPU::Program::UberParamMask HAS_OPACITY;
			static GPU::Program::UberParamMask HAS_OPACITY_TEXTURE;

			static GPU::Program::UberParamMask HAS_SPECULAR_SCALE;

			static GPU::Program::UberParamMask FORWARD_SHADING;

			static std::unordered_map<GPU::Program::UberParamMask, std::string> maskToDefines;
		public:
			StandardProgram();
			~StandardProgram();

			void getUberShaderDefines(std::vector<std::string> & definesBuffer);

			void onFrameBegin(EngineInstance * instance);

			void configureShaderAttributes(GPU::Mesh::GPUMesh * targetMesh);
			void onRenderObject(const SceneObject & object, const Material::Material & material, const Camera & camera);
		};

		class StandardUberFactory : public GPU::Program::AbstractShaderUberMaskFactory<StandardProgram>
		{
		public:
			GPU::Program::UberParamMask computeMaskFromRenderable(DefaultImpl::MeshRenderer * renderable);
		private:
			GPU::Program::UberParamMask parseMesh(GPU::Mesh::GPUMesh * mesh);
			GPU::Program::UberParamMask parseMaterial(GPU::Program::UberParamMask meshMask, Material::Material * mat);
		};
	}
}

#endif