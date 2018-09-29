#include "defaultimpl/components/MeshFilter.h"

#include "logger/Log.h"

#include "SceneObject.h"

namespace RenderLib
{
	namespace DefaultImpl
	{
		void MeshFilter::start()
		{
			if (mesh == NULL)
			{
				enabled = false;
				
				Logger::Log::getInstance().logWarning("MeshFilter: No mesh setted. Component disabled for object " + object->objectName);
			}
		}

		size_t MeshFilter::getIndex()
		{
			if (mesh)
			{
				return mesh->index;
			}

			return 0;
		}
	}
}