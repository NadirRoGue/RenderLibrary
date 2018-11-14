#ifndef __RENDERLIB_GPU_PROGRAM_UBERFACTORYMANAGER_H__
#define __RENDERLIB_GPU_PROGRAM_UBERFACTORYMANAGER_H__

#include <unordered_map>

#include "GPU/program/UberMaskFactory.h"

#include "EngineException.h"

namespace RenderLib
{
  namespace GPU
  {
    namespace Program
    {
      class UberFactoryManager
      {
      private:
        static std::unordered_map<std::type_index,
                                  std::unique_ptr<UberMaskFactory>>
            factories;
        static std::unique_ptr<UberMaskFactory> defaultFactory;

      public:
        template <class ProgramType, class FactoryType>
        static void
        registerUberFactory()
        {
          if (!std::is_base_of<Program, ProgramType>::value)
          {
            throw EngineException(
                "UberFactoryManager: Attempted to register a UberFactory for a "
                "non Program derived class!");
          }

          if (!std::is_base_of<UberMaskFactory, FactoryType>::value)
          {
            throw EngineException(
                "UberFactoryManager:: Attempted to register a UberFactory non "
                "derived from AbstractMaskFactory!");
          }

          std::type_index programId = typeid(ProgramType);
          std::unique_ptr<UberMaskFactory> newFactory
              = std::make_unique<FactoryType>();
          factories[programId] = std::move(newFactory);
        }

        template <class FactoryType>
        static void
        registerDefaultFactory()
        {
          if (!std::is_base_of<UberMaskFactory, FactoryType>::value)
          {
            throw EngineException(
                "UberFactoryManager:: Attempted to register a UberFactory non "
                "derived from AbstractMaskFactory!");
          }

          std::type_index programId = typeid(FactoryType);
          defaultFactory            = std::make_unique<FactoryType>();
        }

        static GPU::Program::UberParamMask
        computeMask(DefaultImpl::MeshRenderer * renderable);
      };
    } // namespace Program
  } // namespace GPU
} // namespace RenderLib

#endif
