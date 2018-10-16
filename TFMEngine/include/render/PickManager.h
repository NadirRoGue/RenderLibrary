#ifndef __RENDERLIB_RENDER_PICKMANAGER_H__
#define __RENDERLIB_RENDER_PICKMANAGER_H__

#include <unordered_map>

#include "AbstractPickHandler.h"

namespace RenderLib
{
	namespace Render
	{
		class PickingRenderStage;
		class EngineInstance;

		class PickManager
		{
		private:
			static std::unordered_map<PickTypeEnum, std::unique_ptr<AbstractPickHandler>> pickHandlers;
		public:
			template<PickTypeEnum forType, class T>
			static void registerPickHandler()
			{
				if (std::is_base_of<AbstractPickHandler, T>::value)
				{
					std::unique_ptr<AbstractPickHandler> newHandler =
						std::make_unique<T>();

					pickHandlers[forType] = std::move(newHandler);
				}
				else
				{
					throw EngineException("PickManager: Attempted to register a non AbstractPickHandler derived pick handler");
				}
			}

			static AbstractPickHandler * getPickHandler(PickTypeEnum type);
		private:
			PickType lastPick;
		public:
			PickManager();
			~PickManager();

			void doPick(PickingRenderStage * stage, PickTypeEnum type);

			PickType & getLastPick();
		};
	}
}

#endif