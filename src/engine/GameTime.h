#pragma once

#include <iostream>
#include <chrono>

class GameTime
{
public:

  GameTime();
  ~GameTime();

  void resetDeltaTimer();
  long long getDeltaTime();
  long long getTimeSinceStart();

private:

  std::chrono::steady_clock::time_point timeAtStart_;
  std::chrono::steady_clock::time_point deltaTimer_;

  bool firstDelta_;

};