#include "Initialization.h"

#include "CPU/memory/MemoryManager.h"

#include "CPU/mesh/Mesh.h"
#include "defaultimpl/meshblockconfigurators/InterleavedMeshBuilder.h"
#include "defaultimpl/meshblockconfigurators/CompactMeshBuilder.h"

#include "CPU/io/FileManager.h"
#include "defaultimpl/fileloaders/AssimpFileLoader.h"
#include "defaultimpl/fileloaders/ShaderLoader.h"

#include "logger/Log.h"
#include "defaultimpl/loggers/ConsoleLogger.h"

namespace RenderLib
{
	void DefaultEngineInitialization()
	{
		// Set default mesh memory layout configurator: Interleaved (FFFF)(VNTB[UV][COLOR]VNTB[UV][COLOR]...)
		CPU::Memory::MemoryManager::getInstance().setAttributeBuilderForClass<CPU::Mesh::Mesh>(new DefaultImpl::InterleavedMeshBuilder());

		CPU::IO::FileManager::registerFileLoader<DefaultImpl::AssimpFileLoader>();
		CPU::IO::FileManager::registerFileLoader<DefaultImpl::ShaderLoader>();

		Logger::Log::getInstance().setLogger<DefaultImpl::ConsoleLogger>();
	}
}