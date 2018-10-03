#ifndef __RENDERLIB_ENGINETIME_H__
#define __RENDERLIB_ENGINETIME_H__

namespace RenderLib
{
	class Time
	{
	public:
		double deltaTime;
		double elapsedSinceBeginning;
		unsigned long long numFrame;
	public:
		void update(double elapsedTimeSinceBegining);
	};
}

#endif