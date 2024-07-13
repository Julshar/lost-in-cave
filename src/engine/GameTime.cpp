#include "GameTime.h"

GameTime::GameTime()
:
firstDelta_(true)
{
  timeAtStart_ = std::chrono::steady_clock::now();
  deltaTimer_ = std::chrono::steady_clock::now();
}

GameTime::~GameTime()
{

}

void GameTime::resetDeltaTimer()
{
  firstDelta_ = true;
}

long long GameTime::getDeltaTime()
{
  if (firstDelta_)
  {
    deltaTimer_ = std::chrono::steady_clock::now();
    firstDelta_ = false;
    return 0;
  }

  std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
  std::chrono::microseconds microseconds = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - deltaTimer_);
  long long delta = microseconds.count();

  deltaTimer_ = currentTime;

  return delta;
}

long long GameTime::getTimeSinceStart()
{
  std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
  std::chrono::microseconds microseconds = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - timeAtStart_);
  long long delta = microseconds.count();

  return delta;
}