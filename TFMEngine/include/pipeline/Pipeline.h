#ifndef __CPU_PIPELINE_H__
#define __CPU_PIPELINE_H__

#include <memory>
#include <vector>

#include "pipeline/PipelineStage.h"

#include "EngineException.h"

namespace RenderLib
{
  namespace Pipeline
  {
    class Pipeline
    {
    private:
      std::vector<std::unique_ptr<PipelineStage>> stages;

    public:
      Pipeline();
      ~Pipeline();

      template <class T>
      T *
      registerStage()
      {
        if (std::is_base_of<PipelineStage, T>::value)
        {
          std::unique_ptr<T> newStage = std::make_unique<T>();
          T * result                  = newStage.get();
          stages.push_back(std::move(newStage));
          return result;
        }

        std::string message = "Pipeline Attempted to register a non "
                              "PipelineStage derived class ("
            + std::string(typeid(T).name()) + ")";
        throw EngineException(message.c_str());

        return NULL;
      }

      template <class T>
      T *
      getStage()
      {
        for (auto st : stages)
        {
          T * castTest = dynamic_cast<T *>(st.get());
          if (castTest != NULL)
          {
            return castTest;
          }
        }

        return NULL;
      }

      template <class T>
      void
      removeStage()
      {
        std::vector<std::unique_ptr<PipelineStage>>::iterator it
            = stages.begin();
        bool found = false;
        while (it != stages.end() && !found)
        {
          T * castTest = dynamic_cast<T *>((*it).get());
          if (castTest != NULL)
          {
            (*it).reset();
            stages.erase(it);
            found = true;
          }
          it++;
        }
      }

      std::vector<std::unique_ptr<PipelineStage>> &
      getAllStages();
      void
      execute();
    };
  } // namespace Pipeline
} // namespace RenderLib

#endif