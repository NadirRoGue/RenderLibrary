#ifndef __RENDERLIB_DEFAULTIMPL_OBJECTSPINNER_H__
#define __RENDERLIB_DEFAULTIMPL_OBJECTSPINNER_H__

#include "defaultimpl/components/UserScript.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		class ObjectSpinner : public UserScript
		{
		public:
			virtual void update();
		};
	}
}

#endif

