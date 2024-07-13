#pragma once

#include "Player.h"
#include "Gem.h"
#include "Hazard.h"
#include "Background.h"
#include "Wall.h"

#include "GameObject.h"
#include "Scene.h"
#include "MenuButton.h"
#include "Engine.h"

#include <vector>
#include <cstdlib>

class Game : public Scene
{
public:

  enum class GameState
  {
    MainMenu,
    Playing,
    Pause
  };

  enum class Difficulty
  {
    Easy,
    Normal,
    Hard
  };

  struct SpawnArea
  {
    float left;
    float right;
    float bottom;
    float top;
  };

  Player *player_;
  Background *background_;
  std::vector<UIObject*> lives_;
  std::vector<SpawnArea*> spawnAreas_;

  std::vector<Gem*> gems_;
  std::vector<Hazard*> hazards_;
  std::vector<Wall*> walls_;

  GameObject *menuCamera_;

  GameState gameState_;

  MenuButton buttonPlay_;
  MenuButton buttonHighscores_;
  MenuButton buttonCredits_;
  MenuButton buttonExitGame_;

  MenuButton buttonEasy_;
  MenuButton buttonNormal_;
  MenuButton buttonHard_;

  MenuButton buttonBack_;

  MenuButton buttonQuitToMenu_;
  MenuButton buttonResume_;
  UIObject gamePausedText_;
  UIObject gamePausedBackground_;

  MenuButton buttonEndScreenContinue_;

  UIObject scoreText_;
  UIObject scoreHeaderText_;
  UIObject hudBackground_;

  std::vector<MenuButton*> buttons_;
  MenuButton* highlightedButton_;

  glm::fvec2 gemSize_;
  glm::fvec2 hazardSize_;
  glm::fvec2 hazardVelocity_;

  int score_;
  int hazardCount_;
  int gemCount_;

  Game();
  ~Game();

  static void loadResources();
  
  void update();

  void updateOnMainMenu();
  void updateOnPlaying();
  void updateOnPause();

  /*
  * Gameplay
  */
  void spawnGem();
  void spawnHazard();
  bool isObjOnSpawn(GameObject *obj, SpawnArea *area);
  glm::fvec2 getRandomSpawnPosition(SpawnArea *area);
  glm::fvec2 getRandomHazardVelocity();

  void clearGameState();

  /*
  * Level loading
  */
  void loadLevel(const std::string& filePath);
  void loadLevel(Difficulty difficulty);
  void initLives();

  /*
  * UI
  */
  void showMainMenu();
  void showHighscores();
  void showCredits();
  void showLevelSelection();
  void showPauseMenu();
  void showEndScreen();
  void showHud();

  void hideMainMenu();
  void hideHighscores();
  void hideCredits();
  void hideLevelSelection();
  void hidePauseMenu();
  void hideEndScreen();
  void hideHud();

  void highlightHoveredButton();

private:

};