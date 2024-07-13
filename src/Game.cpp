#include "Game.h"
#include "GameParameters.h"
#include "Random.h"

#include "FileManager.h"

#include <vector>
#include <cstdlib>

#undef DEBUG

Game::Game()
:
gameState_(GameState::MainMenu),

buttonPlay_({0.0f, -300.0f}, "Play"),
buttonHighscores_({0.0f, -100.0f}, "Highscores"),
buttonCredits_({0.0f, 100.0f}, "Settings"),
buttonExitGame_({0.0f, 300.0f}, "Exit game"),

buttonEasy_({0.0f, -300.0f}, "Easy"),
buttonNormal_({0.0f, -100.0f}, "Normal"),
buttonHard_({0.0f, 100.0f}, "Hard"),

buttonBack_({0.0f, 300.0f}, "Back"),

buttonResume_({0.0f, 50.0f}, "Resume game"),
buttonQuitToMenu_({0.0f, 250.0f}, "Quit to menu"),

buttonEndScreenContinue_({0.0f, 200.0f}, "Continue to menu"),

scoreText_(UILayer::getLayer("elements")),
scoreHeaderText_(UILayer::getLayer("elements")),
gamePausedText_(UILayer::getLayer("elements")),

gamePausedBackground_(UILayer::getLayer("background")),
hudBackground_(UILayer::getLayer("background")),

highlightedButton_(nullptr),
score_(0)
{
  buttons_.push_back(&buttonPlay_);
  buttons_.push_back(&buttonHighscores_);
  buttons_.push_back(&buttonCredits_);
  buttons_.push_back(&buttonExitGame_);
  buttons_.push_back(&buttonEasy_);
  buttons_.push_back(&buttonNormal_);
  buttons_.push_back(&buttonHard_);
  buttons_.push_back(&buttonBack_);
  buttons_.push_back(&buttonResume_);
  buttons_.push_back(&buttonQuitToMenu_);
  buttons_.push_back(&buttonEndScreenContinue_);

  hudBackground_.position_ = {0.0f, 75.0f};
  hudBackground_.setPivot(PivotPosition::TOP);
  hudBackground_.addSprite({1000.0f, 150.0f}, Texture::getInstance("hud_background"));
  hudBackground_.sprite_->setSizeToWindowWidth();

  gamePausedBackground_.position_ = {0.0f, 100.0f};
  gamePausedBackground_.setPivot(PivotPosition::MIDDLE);
  gamePausedBackground_.addSprite({800.0f, 800.0f}, Texture::getInstance("hud_background"));

  gamePausedText_.setPivot(PivotPosition::MIDDLE);
  gamePausedText_.addText("Game paused", TextFont::getInstance("pixel_L"), {1.0f, 0.4f, 1.0f, 1.0f});
  gamePausedText_.setPosition({0.0f, -120.0f});
  gamePausedText_.text_->centerText();

  scoreText_.position_ = {1200.0f, 42.0f};
  scoreText_.addText("0", TextFont::getInstance("pixel_L"), {1.0f, 0.4f, 1.0f, 1.0f});

  scoreHeaderText_.position_ = {900.0f, 42.0f};
  scoreHeaderText_.addText("SCORE :", TextFont::getInstance("pixel_L"), {1.0f, 0.4f, 1.0f, 1.0f});

  gemSize_ = {GameParameters::GEM_SIZE_X, GameParameters::GEM_SIZE_Y};
  hazardSize_ = {GameParameters::HAZARD_SIZE_X, GameParameters::HAZARD_SIZE_Y};
  hazardVelocity_ = {GameParameters::HAZARD_VELOCITY_X, GameParameters::HAZARD_VELOCITY_Y};

  menuCamera_ = new GameObject();
  menuCamera_->setPosition({0.0f, 0.0f});
  menuCamera_->addCamera();
  menuCamera_->setCameraAsActive();

  background_ = new Background({0.0f, 0.0f}, {1.0f, 1.0f});

  Random::seedWithTime();

  hideHighscores();
  hideCredits();
  hideLevelSelection();
  hidePauseMenu();
  hideEndScreen();
  hideHud();

  showMainMenu();
}

Game::~Game()
{

}

void Game::update()
{
  switch (gameState_)
  {
    case GameState::MainMenu:
    {
      updateOnMainMenu();
      break;
    }
    case GameState::Playing:
    {
      updateOnPlaying();
      break;
    }
    case GameState::Pause:
    {
      updateOnPause();
      break;
    }
  }
}

void Game::updateOnMainMenu()
{
  #ifdef DEBUG
  std::cout << "Game::updateOnMainMenu(): Updating on main menu.\n";
  #endif

  highlightHoveredButton();

  #ifdef DEBUG
  std::cout << "Game::updateOnMainMenu(): Checking buttons.\n";
  #endif

  if (buttonPlay_.button_->isClicked())
  {
    hideMainMenu();
    showLevelSelection();
  }
  else if (buttonHighscores_.button_->isClicked())
  {
    hideMainMenu();
    showHighscores();
  }
  else if (buttonCredits_.button_->isClicked())
  {
    hideMainMenu();
    showCredits();
  }
  else if (buttonExitGame_.button_->isClicked())
  {
    hideMainMenu();
    Engine::exitProgram();
  }
  else if (buttonBack_.button_->isClicked())
  {
    hideCredits();
    hideHighscores();
    hideLevelSelection();
    showMainMenu();
  }
  else if (buttonEasy_.button_->isClicked())
  {
    hideLevelSelection();
    showHud();
    loadLevel(Difficulty::Easy);
  }
  else if (buttonNormal_.button_->isClicked())
  {
    hideLevelSelection();
    showHud();
    loadLevel(Difficulty::Normal);
  }
  else if (buttonHard_.button_->isClicked())
  {
    hideLevelSelection();
    showHud();
    loadLevel(Difficulty::Hard);
  }

  #ifdef DEBUG
  std::cout << "Game::updateOnMainMenu(): Finished updating on main menu.\n";
  #endif
}

void Game::highlightHoveredButton()
{
  if (highlightedButton_ != nullptr)
  {
    if (highlightedButton_->button_->isHoveredOver() == false)
    {
      highlightedButton_->highlight(false);
      highlightedButton_ = nullptr;
    }
  }
  else
  {
    for (MenuButton *button : buttons_)
    {
      if (button->button_->isHoveredOver())
      {
        button->highlight(true);
        highlightedButton_ = button;
        break;
      }
    }
  }
}

void Game::updateOnPlaying()
{
  for (size_t i = 0; i < gems_.size(); ++i)
  {
    if (player_->isColliding(gems_[i]))
    {
      Gem *gem = gems_[i];
      gems_.erase(gems_.begin() + i);
      delete gem;

      score_ += 10;
      scoreText_.text_->content_ = std::to_string(score_);

      spawnGem();
    }
  }

  for (size_t i = 0; i < hazards_.size(); ++i)
  {
    if (player_->isColliding(hazards_[i]))
    {
      --player_->lives_;
      UIObject *life = lives_.back();
      lives_.pop_back();
      delete life;

      if (player_->lives_ == 0)
      {
        player_->isDead_ = true;
        showEndScreen();
        gameState_ = GameState::Pause;
      }
      else
      {
        Hazard *hazard = hazards_[i];
        hazards_.erase(hazards_.begin() + i);
        delete hazard;

        spawnHazard();
      }
    }
  }

  if (player_->pauseRequest_)
  {
    /*
    * Show Pause UI
    */
    player_->pauseRequest_ = false;
    showPauseMenu();
    Engine::pause(true);
    gameState_ = GameState::Pause;
  }
}

void Game::spawnGem()
{
  bool spawned = false;
  
  while (!spawned)
  {
    unsigned int spawnId = Random::genValue(0, spawnAreas_.size() - 1);

    if (isObjOnSpawn(player_, spawnAreas_[spawnId]) == false)
    {
      glm::fvec2 spawnPosition = getRandomSpawnPosition(spawnAreas_[spawnId]);
      gems_.push_back(new Gem(spawnPosition, gemSize_));
      spawned = true;
    }
  }
}

void Game::spawnHazard()
{
  bool spawned = false;
  
  while (!spawned)
  {
    unsigned int spawnId = Random::genValue(0, spawnAreas_.size() - 1);

    if (isObjOnSpawn(player_, spawnAreas_[spawnId]) == false)
    {
      glm::fvec2 position = getRandomSpawnPosition(spawnAreas_[spawnId]);
      glm::fvec2 velocity = getRandomHazardVelocity();
      hazards_.push_back(new Hazard(position, hazardSize_, velocity));
      spawned = true;
    }
  }
}

glm::fvec2 Game::getRandomHazardVelocity()
{
  int xMod = (int)Random::genValue(0, (GameParameters::HAZARD_VELOCITY_DISPERSION * 2));
  xMod -= (int)GameParameters::HAZARD_VELOCITY_DISPERSION;

  int yMod = (int)Random::genValue(0, (GameParameters::HAZARD_VELOCITY_DISPERSION * 2));
  yMod -= (int)GameParameters::HAZARD_VELOCITY_DISPERSION;

  glm::fvec2 randomizedVelocity =
  {
    hazardVelocity_.x + xMod,
    hazardVelocity_.y + yMod
  };

  return randomizedVelocity;
}

bool Game::isObjOnSpawn(GameObject *obj, SpawnArea *area)
{
  if (obj->physics_ != nullptr)
  {
    if (obj->physics_->isColliding({area->left, area->right, area->bottom, area->top}))
    {
      return true;
    }
  }

  return false;
}

glm::fvec2 Game::getRandomSpawnPosition(SpawnArea *area)
{
  unsigned int width = (unsigned int)area->right - (unsigned int)area->left;
  unsigned int height = (unsigned int)area->top - (unsigned int)area->bottom;

  unsigned int xOffset = Random::genValue(0, width);
  unsigned int yOffset = Random::genValue(0, height);

  glm::fvec2 randomPosition =
  {
    area->left + (float)xOffset,
    area->bottom + (float)yOffset
  };

  return randomPosition;
}

void Game::updateOnPause()
{
  highlightHoveredButton();

  if (player_->isDead_)
  {
    /*
    * Defeat screen handling
    */
    if (buttonEndScreenContinue_.button_->isClicked())
    {
      hideEndScreen();
      hideHud();
      clearGameState();
      showMainMenu();
      menuCamera_->camera_->setAsActiveCamera();
      Engine::setZoom(1.0f);
      gameState_ = GameState::MainMenu;
    }

    /*
    * On button pressed back to menu, cleanup game state
    */
  }
  else
  {
    /*
    * In pause menu
    */
    if (buttonQuitToMenu_.button_->isClicked())
    {
      hidePauseMenu();
      hideHud();
      clearGameState();
      showMainMenu();
      Engine::pause(false);
      Engine::setZoom(1.0f);
      menuCamera_->camera_->setAsActiveCamera();
      gameState_ = GameState::MainMenu;
    }
    else if (buttonResume_.button_->isClicked())
    {
      hidePauseMenu();
      Engine::pause(false);
      gameState_ = GameState::Playing;
    }

    /*
    * On button pressed back to menu, cleanup game state or resume game
    */
  }
}

void Game::clearGameState()
{
  if (player_ != nullptr)
  {
    delete player_;
    player_ = nullptr;
  }

  background_->sprite_->setScaling({1.0f, 1.0f});

  #ifdef DEBUG
  printf("Game::clearGameState(): Deleting walls.\n");
  #endif

  while (walls_.empty() == false)
  {
    Wall *wall = walls_.back();
    walls_.pop_back();
    delete wall;
  }

  #ifdef DEBUG
  printf("Game::clearGameState(): Deleting gems.\n");
  #endif

  while (gems_.empty() == false)
  {
    Gem *gem = gems_.back();
    gems_.pop_back();
    delete gem;
  }

  #ifdef DEBUG
  printf("Game::clearGameState(): Deleting hazards.\n");
  #endif

  while (hazards_.empty() == false)
  {
    Hazard *hazard = hazards_.back();
    hazards_.pop_back();
    delete hazard;
  }

  #ifdef DEBUG
  printf("Game::clearGameState(): Deleting spawn areas.\n");
  #endif

  while (spawnAreas_.empty() == false)
  {
    SpawnArea *area = spawnAreas_.back();
    spawnAreas_.pop_back();
    delete area;
  }

  #ifdef DEBUG
  printf("Game::clearGameState(): Deleting lives.\n");
  #endif

  while (lives_.empty() == false)
  {
    UIObject *life = lives_.back();
    lives_.pop_back();
    delete life;
  }
}

void Game::loadLevel(Difficulty difficulty)
{
  player_ = new Player();

  background_->sprite_->scaling_ = {2.0f, 2.0f};

  walls_.push_back(new Wall({0.0f, -2000.0f}, {4000.0f, 1500.0f}));
  walls_.push_back(new Wall({0.0f, 2000.0f}, {4000.0f, 1500.0f}));
  walls_.push_back(new Wall({-2000.0f, 0.0f}, {1500.0f, 4000.0f}));
  walls_.push_back(new Wall({2000.0f, 0.0f}, {1500.0f, 4000.0f}));

  walls_.push_back(new Wall({1250.0f, -1250.0f}, {700.0f, 700.0f}));
  walls_.push_back(new Wall({-1250.0f, -300.0f}, {800.0f, 600.0f}));
  walls_.push_back(new Wall({0.0f, 1450.0f}, {1100.0f, 1000.0f}));
  walls_.push_back(new Wall({-100.0f, -1100.0f}, {500.0f, 700.0f}));

  walls_.push_back(new Wall({600.0f, 300.0f}, {400.0f, 800.0f}));
  walls_.push_back(new Wall({-300.0f, 400.0f}, {300.0f, 300.0f}));

  spawnAreas_.push_back(new SpawnArea({-1150.0f, -650.0f, 100.0f, 1150.0f}));
  spawnAreas_.push_back(new SpawnArea({-650.0f, 300.0f, 450.0f, 850.0f}));
  spawnAreas_.push_back(new SpawnArea({650.0f, 1150.0f, 600.0f, 1150.0f}));
  spawnAreas_.push_back(new SpawnArea({-50.0f, 300.0f, 150.0f, 450.0f}));
  spawnAreas_.push_back(new SpawnArea({-1150.0f, -450.0f, -1150.0f, -700.0f}));
  spawnAreas_.push_back(new SpawnArea({-750.0f, -150.0f, -650.0f, 150.0f}));
  spawnAreas_.push_back(new SpawnArea({-150.0f, 300.0f, -200.0f, 150.0f}));
  spawnAreas_.push_back(new SpawnArea({-150.0f, 250.0f, -650.0f, -200.0f}));
  spawnAreas_.push_back(new SpawnArea({250.0f, 800.0f, -1150.0f, -200.0f}));
  spawnAreas_.push_back(new SpawnArea({900.0f, 1150.0f, -800.0f, 600.0f}));

  score_ = 0;
  scoreText_.text_->content_ = "0";

  switch (difficulty)
  {
    case Difficulty::Easy:
    {
      hazardCount_ = 7;
      gemCount_ = 15;
      hazardVelocity_ = {100.0f, 100.0f};
      player_->lives_ = 9;
      break;
    }
    case Difficulty::Normal:
    {
      hazardCount_ = 10;
      gemCount_ = 30;
      hazardVelocity_ = {150.0f, 150.0f};
      player_->lives_ = 6;
      break;
    }
    case Difficulty::Hard:
    {
      hazardCount_ = 13;
      gemCount_ = 60;
      hazardVelocity_ = {200.0f, 200.0f};
      player_->lives_ = 3;
      break;
    }
  }

  initLives();
  
  for (int i = 0; i < hazardCount_; ++i)
  {
    spawnHazard();
  }

  for (int i = 0; i < gemCount_; ++i)
  {
    spawnGem();
  }

  gameState_ = GameState::Playing;
}

void Game::loadLevel(const std::string& filePath)
{
  
}

void Game::initLives()
{
  float xOffset = 0.0f;

  for (int i = 0; i < player_->lives_; ++i)
  {
    UIObject *life = new UIObject(UILayer::getLayer("elements"));
    life->position_ = {170.0f + xOffset, 70.0f};
    life->addSprite({50.0f, 50.0f}, Texture::getInstance("hearth"));
    life->sprite_->rotation_ = 180.0f;

    lives_.push_back(life);

    xOffset += 70.0f;
  }
}

void Game::showMainMenu()
{
  buttonPlay_.show();
  buttonHighscores_.show();
  buttonCredits_.show();
  buttonExitGame_.show();
}

void Game::showHighscores()
{
  buttonBack_.show();
}

void Game::showCredits()
{
  buttonBack_.show();
}

void Game::showLevelSelection()
{
  buttonEasy_.show();
  buttonNormal_.show();
  buttonHard_.show();
  buttonBack_.show();
}

void Game::showPauseMenu()
{
  buttonResume_.show();
  buttonQuitToMenu_.show();
  gamePausedText_.show();
  gamePausedBackground_.show();
}

void Game::showEndScreen()
{
  buttonEndScreenContinue_.show();
}

void Game::showHud()
{
  scoreText_.show();
  scoreHeaderText_.show();
  hudBackground_.show();
}

void Game::hideMainMenu()
{
  buttonPlay_.hide();
  buttonHighscores_.hide();
  buttonCredits_.hide();
  buttonExitGame_.hide();
}

void Game::hideHighscores()
{
  buttonBack_.hide();
}

void Game::hideCredits()
{
  buttonBack_.hide();
}

void Game::hideLevelSelection()
{
  buttonEasy_.hide();
  buttonNormal_.hide();
  buttonHard_.hide();
  buttonBack_.hide();
}

void Game::hidePauseMenu()
{
  buttonResume_.hide();
  buttonQuitToMenu_.hide();
  gamePausedText_.hide();
  gamePausedBackground_.hide();
}

void Game::hideEndScreen()
{
  buttonEndScreenContinue_.hide();
}

void Game::hideHud()
{
  scoreText_.hide();
  scoreHeaderText_.hide();
  hudBackground_.hide();
}

void Game::loadResources()
{
  Texture::loadTexture("textures/cave.jpeg", "cave");

  Texture::loadTexture("textures/gem_0.png", "gem0");
  Texture::loadTexture("textures/gem_1.png", "gem1");
  Texture::loadTexture("textures/gem_2.png", "gem2");

  Texture::loadTexture("textures/Ship_0.png", "ship0");
  Texture::loadTexture("textures/Ship_1.png", "ship1");
  Texture::loadTexture("textures/Ship_2.png", "ship2");

  Texture::loadTexture("textures/enemy_0.png", "enemy0");

  Texture::loadTexture("textures/wall.png", "wall");

  Texture::loadTexture("textures/button.png", "button");
  Texture::loadTexture("textures/Hearth.png", "hearth");
  Texture::loadTexture("textures/hud_background.png", "hud_background");

  TextFont::loadFont("fonts/PerfectPixel.ttf", 32, "pixel_M");
  TextFont::loadFont("fonts/PerfectPixel.ttf", 16, "pixel_S");
  TextFont::loadFont("fonts/PerfectPixel.ttf", 64, "pixel_L");

  SpriteLayer::addLayer(5, "background");
  SpriteLayer::addLayer(20, "player");
  SpriteLayer::addLayer(15, "gem");
  SpriteLayer::addLayer(25, "hazard");
  SpriteLayer::addLayer(30, "wall");

  UILayer::addLayer(5, "background");
  UILayer::addLayer(10, "elements");
  UILayer::addLayer(15, "buttons");
}