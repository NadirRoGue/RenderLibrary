#include "Initialization.h"

#include "CPU/memory/MemoryManager.h"

#include "CPU/mesh/Mesh.h"
#include "CPU/mesh/MeshManager.h"
#include "defaultimpl/meshblockconfigurators/CompactMeshBuilder.h"
#include "defaultimpl/meshblockconfigurators/InterleavedMeshBuilder.h"
#include "defaultimpl/textureblockconfigurators/CompactTextureBuilder.h"

#include "CPU/io/FileManager.h"
#include "defaultimpl/fileloaders/AssimpFileLoader.h"
#include "defaultimpl/fileloaders/ImageLoader.h"
#include "defaultimpl/fileloaders/ShaderLoader.h"

#include "GPU/program/UberFactoryManager.h"
#include "defaultimpl/ubermaskfactories/StandardUberFactory.h"

#include "GPU/program/ProgramManager.h"
#include "defaultimpl/shaders/PickArrowProgram.h"
#include "defaultimpl/shaders/StandardProgram.h"

#include "defaultimpl/loggers/BufferedFileLogger.h"
#include "defaultimpl/loggers/ConsoleLogger.h"
#include "logger/Log.h"

namespace RenderLib
{
  void
  DefaultEngineInitialization()
  {
    CPU::Memory::MemoryManager::getInstance()
        .setAttributeBuilderForClass<CPU::Mesh::Mesh,
                                     DefaultImpl::InterleavedMeshBuilder>();
    CPU::Memory::MemoryManager::getInstance()
        .setAttributeBuilderForClass<CPU::Texture::Texture,
                                     DefaultImpl::CompactTextureBuilder>();

    CPU::IO::FileManager::registerFileLoader<DefaultImpl::AssimpFileLoader>();
    CPU::IO::FileManager::registerFileLoader<DefaultImpl::ShaderLoader>();
    CPU::IO::FileManager::registerFileLoader<DefaultImpl::ImageLoader>();

    Logger::Log::getInstance().addLogger<DefaultImpl::ConsoleLogger>();
    Render::FBO::initializeDefaultFBO();
    // Logger::Log::getInstance().addLogger<DefaultImpl::BufferedFileLogger>();

    GPU::Program::UberFactoryManager::registerDefaultFactory<
        DefaultImpl::StandardUberFactory>();
    GPU::Program::ProgramManager::registerProgram<
        DefaultImpl::StandardProgram>();
    GPU::Program::ProgramManager::registerProgram<
        DefaultImpl::PickArrowProgram>();

    CPU::Mesh::MeshManager::getInstance().init();
  }
} // namespace RenderLib