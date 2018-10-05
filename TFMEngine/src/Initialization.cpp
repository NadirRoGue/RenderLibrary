#include "Initialization.h"

#include "CPU/memory/MemoryManager.h"

#include "CPU/mesh/Mesh.h"
#include "defaultimpl/meshblockconfigurators/InterleavedMeshBuilder.h"
#include "defaultimpl/meshblockconfigurators/CompactMeshBuilder.h"
#include "defaultimpl/textureblockconfigurators/CompactTextureBuilder.h"

#include "CPU/io/FileManager.h"
#include "defaultimpl/fileloaders/AssimpFileLoader.h"
#include "defaultimpl/fileloaders/ShaderLoader.h"
#include "defaultimpl/fileloaders/ImageLoader.h"

#include "GPU/texture/GPUTextureManager.h"
#include "GPU/program/ProgramManager.h"

#include "defaultimpl/shaders/StandardProgram.h"

#include "logger/Log.h"
#include "defaultimpl/loggers/ConsoleLogger.h"

namespace RenderLib
{
	void DefaultEngineInitialization()
	{
		CPU::Memory::MemoryManager::getInstance().setAttributeBuilderForClass<CPU::Mesh::Mesh, DefaultImpl::InterleavedMeshBuilder>();
		CPU::Memory::MemoryManager::getInstance().setAttributeBuilderForClass<CPU::Texture::Texture, DefaultImpl::CompactTextureBuilder>();

		CPU::IO::FileManager::registerFileLoader<DefaultImpl::AssimpFileLoader>();
		CPU::IO::FileManager::registerFileLoader<DefaultImpl::ShaderLoader>();
		CPU::IO::FileManager::registerFileLoader<DefaultImpl::ImageLoader>();

		Logger::Log::getInstance().setLogger<DefaultImpl::ConsoleLogger>();

		GPU::Texture::GPUTextureManager::queryAnisotropicFilterSupport();
		GPU::Program::ProgramManager::registerUberMaskFactory<DefaultImpl::StandardProgram, DefaultImpl::StandardUberFactory>();
	}
}