#include "Initialization.h"

#include "CPU/memory/MemoryManager.h"

#include "CPU/mesh/Mesh.h"
#include "CPU/mesh/meshattributebuilders/InterleavedMeshBuilder.h"
#include "CPU/mesh/meshattributebuilders/CompactMeshBuilder.h"

#include "CPU/io/FileManager.h"
#include "CPU/io/defaultloaders/AssimpFileLoader.h"
#include "CPU/io/defaultloaders/TextFileLoader.h"

namespace RenderLib
{
	void DefaultEngineInitialization()
	{
		CPU::Memory::MemoryManager::getInstance().setAttributeBuilderForClass<CPU::Mesh::Mesh>(new CPU::Mesh::InterleavedMeshBuilder());

		CPU::IO::FileManager::registerFileLoader(new CPU::IO::AssimpFileLoader());
		CPU::IO::FileManager::registerFileLoader(new CPU::IO::TextFileLoader());
	}
}