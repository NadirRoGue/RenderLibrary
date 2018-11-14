#ifndef __RENDERLIB_DEFAULTIMPL_USERSCRIPT_H__
#define __RENDERLIB_DEFAULTIMPL_USERSCRIPT_H__

#include "Component.h"

namespace RenderLib
{
  namespace DefaultImpl
  {
    class UserScript : public Component
    {
    public:
      std::type_index
      getComponentType();
    };
  } // namespace DefaultImpl
} // namespace RenderLib

#endif
