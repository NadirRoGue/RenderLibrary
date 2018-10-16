#include "render/PickManager.h"

#include "logger/Log.h"

namespace RenderLib
{
	namespace Render
	{
		std::unordered_map<PickTypeEnum, std::unique_ptr<AbstractPickHandler>> PickManager::pickHandlers;

		AbstractPickHandler * PickManager::getPickHandler(PickTypeEnum type)
		{
			auto it = pickHandlers.find(type);
			if (it != pickHandlers.end())
			{
				return it->second.get();
			}

			return NULL;
		}

		PickManager::PickManager()
		{

		}

		PickManager::~PickManager()
		{

		}

		PickType & PickManager::getLastPick()
		{
			return lastPick;
		}

		void PickManager::doPick(PickingRenderStage * stage, PickTypeEnum type)
		{
			auto it = pickHandlers.find(type);
			if (it != pickHandlers.end())
			{
				AbstractPickHandler * handler = it->second.get();
				if (handler)
				{
					lastPick = handler->doPicking(stage);
				}
				else
				{
					Logger::Log::getInstance().logWarning("PickManager: A NULL pick handler was found");
				}
			}
			else
			{
				Logger::Log::getInstance().logWarning("PickManager: No pick handler supplied for the given type");
			}
		}
	}
}