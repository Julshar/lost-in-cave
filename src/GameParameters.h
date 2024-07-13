#pragma once

class GameParameters
{
  public:

  static constexpr int PLAYER_COLLISION_GROUP = 1;
  static constexpr int HAZARD_COLLISION_GROUP = 2;
  static constexpr int WALL_COLLISION_GROUP = 3;
  static constexpr int GEM_COLLISION_GROUP = 4;

  static constexpr float PLAYER_SIZE_X = 150.0f;
  static constexpr float PLAYER_SIZE_Y = 150.0f;

  static constexpr float GEM_SIZE_X = 120.0f;
  static constexpr float GEM_SIZE_Y = 120.0f;

  static constexpr float HAZARD_SIZE_X = 150.0f;
  static constexpr float HAZARD_SIZE_Y = 150.0f;

  static constexpr float HAZARD_VELOCITY_X = 100.0f;
  static constexpr float HAZARD_VELOCITY_Y = 100.0f;

  static constexpr unsigned int HAZARD_VELOCITY_DISPERSION = 50;
};