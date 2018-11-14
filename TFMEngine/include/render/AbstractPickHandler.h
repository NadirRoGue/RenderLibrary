#ifndef __RENDERLIB_RENDER_ABSTRACTPICKHANDLER_H__
#define __RENDERLIB_RENDER_ABSTRACTPICKHANDLER_H__

namespace RenderLib
{
  namespace Render
  {
    class PickingRenderStage;
    class EngineInstance;

    enum class PickTypeEnum
    {
      PICK_NONE,
      PICK_OBJECT,
      PICK_FACE,
      PICK_VERTEX
    };

    typedef struct PickType
    {
      void * data;
      PickTypeEnum type;

      PickType() : data(0), type(PickTypeEnum::PICK_NONE) {}

    } PickType;

    class AbstractPickHandler
    {
    public:
      virtual void initialize(EngineInstance * instance);
      virtual PickType doPicking(PickingRenderStage * pickStage) = 0;
    };
  } // namespace Render
} // namespace RenderLib

#endif