#include <iostream>

#include "Player.h"
#include "Wall.h"
#include "Background.h"
#include "Gem.h"
#include "Hazard.h"
#include "Game.h"

#include "Engine.h"
#include "GameObject.h"

#include "Texture.h"
#include "KeyInput.h"

#include "UIText.h"
#include "TextFont.h"
#include "Animation.h"
#include "UIButton.h"

int main(void)
{
  std::cout << "Initializing engine." << std::endl;
    
  Engine engine;
  
  if (engine.init())
  {
    std::cout << "Failed to initialize engine.\n";

    return 1;
  }

  Game::loadResources();

  std::cout << "Resources loaded, initializing game\n";

  Game game;

  while (engine.exitProgram_ == false)
  {
    engine.mainLoop();
  }

  std::cout << "Going to exit program." << std::endl;

  return 0;
}