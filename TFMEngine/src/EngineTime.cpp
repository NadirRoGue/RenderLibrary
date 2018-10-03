#include "EngineTime.h"

namespace RenderLib
{
	void Time::update(double timeSinceBeginning)
	{
		deltaTime = timeSinceBeginning - elapsedSinceBeginning;
		elapsedSinceBeginning = timeSinceBeginning;
		numFrame++;
	}
}