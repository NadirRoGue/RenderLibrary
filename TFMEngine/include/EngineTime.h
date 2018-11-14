#ifndef __RENDERLIB_ENGINETIME_H__
#define __RENDERLIB_ENGINETIME_H__

namespace RenderLib
{
  typedef unsigned long long ull_t;

  class Time
  {
  private:
    double delta;
    double elapsed;
    ull_t frame;

    double scale;

  public:
    Time();
    void
    update(double elapsedTimeSinceBegining);

    const double &
    deltaTime();
    const double &
    elapsedTime();
    const ull_t &
    frameNumber();

    const double &
    getTimeScale();
    void
    setTimeScale(const double & timeScale);
  };
} // namespace RenderLib

#endif