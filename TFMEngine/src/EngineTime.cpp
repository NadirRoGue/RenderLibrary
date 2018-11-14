#include "EngineTime.h"

namespace RenderLib
{
  Time::Time() : delta(0.0), elapsed(0.0), frame(0), scale(1.0)
  {
  }

  void
  Time::update(double timeSinceBeginning)
  {
    delta   = (timeSinceBeginning - elapsed) * scale;
    elapsed = timeSinceBeginning;
    frame++;
  }

  const double &
  Time::deltaTime()
  {
    return delta;
  }

  const double &
  Time::elapsedTime()
  {
    return elapsed;
  }

  const ull_t &
  Time::frameNumber()
  {
    return frame;
  }

  const double &
  Time::getTimeScale()
  {
    return scale;
  }

  void
  Time::setTimeScale(const double & newScale)
  {
    scale = newScale;
  }
} // namespace RenderLib