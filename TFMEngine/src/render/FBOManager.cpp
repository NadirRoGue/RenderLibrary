#include "render/FBOManager.h"

namespace RenderLib
{
	namespace Render
	{
		FBOManager::FBOManager()
		{
		}

		FBOManager::~FBOManager()
		{

		}

		void FBOManager::setEngineInstance(EngineInstance * instance)
		{
			this->instance = instance;
		}

		FBO * FBOManager::createFBO(const std::string & name)
		{
			auto it = fbos.find(name);
			if (it != fbos.end())
			{
				return it->second.get();
			}

			std::unique_ptr<FBO> newFBO = std::make_unique<FBO>();
			FBO * result = newFBO.get();
			result->generate();
			result->setEngineInstance(instance);

			fbos[name] = std::move(newFBO);

			return result;
		}

		FBO * FBOManager::getFBO(const std::string & name)
		{
			auto it = fbos.find(name);
			if (it != fbos.end())
			{
				return it->second.get();
			}

			return NULL;
		}

		void FBOManager::destroyFBO(const std::string & name)
		{
			auto it = fbos.find(name);
			if (it != fbos.end())
			{
				it->second.get()->destroy();
				fbos.erase(it);
			}
		}

		void FBOManager::onResize(const unsigned int & width, const unsigned int & height)
		{
			for (auto it = fbos.begin(); it != fbos.end(); it++)
			{
				it->second.get()->setSize(width, height);
			}
		}

		void FBOManager::clean()
		{
			for (auto it = fbos.begin(); it != fbos.end(); it++)
			{
				it->second.get()->destroy();
			}
		}
	}
}