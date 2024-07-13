#include "Engine.h"
#include "FileManager.h"
#include "UIText.h"
#include "Animation.h"
#include "UIButton.h"
#include "UISprite.h"
#include "Scene.h"

#include <string>
#include <thread>
#include <cmath>

#undef DEBUG

Engine* Engine::engine = nullptr;

// -----------------
// ---- PUBLIC -----
// -----------------

Engine::Engine()
  :
  exitProgram_(false),
  zoom_(1.0f),
  timeAccumulator_(0),
  updateCounter_(0),
  renderCounter_(0),
  wasSleeping_(true),
  paused_(false)
{
  engine = this;
}

Engine::~Engine()
{
  deinitialize();
}

void Engine::deinitialize()
{
  glfwTerminate();

  return;
}

int Engine::init()
{
  if (initWindow())
  {
    return 1;
  }

  if (initGL())
  {
    return 1;
  }

  if (initCallbackFunctions())
  {
    return 1;
  }

  if (initShaders())
  {
    return 1;
  }
  
  updateProjectionMatrix();

  initMainLoop();
  initRender();

  mouseInput_ = new MouseInput(window_);

  return 0;
}

int Engine::initWindow()
{
  window_ = new Window();

  if (window_ -> init())
  {
    return 1;
  }

  return 0;
}

int Engine::initGL()
{
  glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
    printf("ERROR! Failed to initialize GLEW\n");
		glfwTerminate();

    return 1;
	}

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

  glViewport(0, 0, window_ -> getWidth(), window_ -> getHeight());

  return 0;
}

int Engine::initCallbackFunctions()
{
  glfwSetFramebufferSizeCallback(window_ -> getHandle(), Engine::framebufferSizeCallback);

  KeyInput::setKeyCallback(window_ -> getHandle());

  return 0;
}

int Engine::initShaders()
{
  Shader::initInstance("shaders/basic_vertex.glsl", "shaders/basic_fragment.glsl", "basic");
  Shader::initInstance("shaders/sprite_vertex.glsl", "shaders/sprite_fragment.glsl", "sprite");
  Shader::initInstance("shaders/UI_text_vertex.glsl", "shaders/UI_text_fragment.glsl", "UItext");
  
  return 0;
}

void Engine::initMainLoop()
{
  time_ = new GameTime();

  std::string parameter = FileManager::readParameter("Frames_per_second");
  fps_ = stoi(parameter);
  frameTimeInterval_ = 1000000.0 / (double)fps_;
  maxTimeAccumulated_ = 1000000.0 / (double)(fps_ + 2);

  #ifdef DEBUG
  printf("Engine::init(): Fps set to %i. Frame time interval is %f microseconds. Max accumulated time is %f microseconds.\n", fps_, frameTimeInterval_, maxTimeAccumulated_);
  #endif
}

void Engine::initRender()
{
  UItextRenderer_ = new UITextRenderer(Shader::getInstance("UItext"), window_->getWidth(), window_->getHeight());
  spriteRenderer_ = new SpriteRenderer(Shader::getInstance("sprite"));

  UIObject::windowWidth = window_->getWidth();
  UIObject::windowHeight = window_->getHeight();
}

void Engine::mainLoop()
{
  double deltaTime;

  deltaTime = (double)time_->getDeltaTime();
  timeAccumulator_ += deltaTime;

  #ifdef DEBUG
  printf("Engine::mainLoop(): Pre update delta time is %f, Time accumulator is %f.\n", deltaTime, timeAccumulator_);
  #endif

  /*
  * If game loop was fast enough to sleep last frame then it should update once
  *
  * If it didn't sleep last frame it means that frame time interval was exceeded
  * and update has to be done at least once (possibly more times)
  */
  if (wasSleeping_)
  {
    update();

    timeAccumulator_ -= frameTimeInterval_;
    if (timeAccumulator_ < 0)
    {
      timeAccumulator_ = 0;
    }
  }
  else
  {
    while (timeAccumulator_ > maxTimeAccumulated_)
    {
      update();

      timeAccumulator_ -= frameTimeInterval_;
      if (timeAccumulator_ < 0)
      {
        timeAccumulator_ = 0;
      }
    }
  }

  render();

  deltaTime = (double)time_->getDeltaTime();
  timeAccumulator_ += deltaTime;

  #ifdef DEBUG
  printf("Engine::mainLoop(): Post render delta time is %f, Time accumulator is %f.\n", deltaTime, timeAccumulator_);
  #endif

  /*
  * If game loop took less time than intended for single frame interval
  * then sleep until for amout of time needed for loop to last frame interval is reached
  */
  if (timeAccumulator_ < frameTimeInterval_)
  {
    double sleepTime = frameTimeInterval_ - timeAccumulator_;
    double roundedSleepTime = std::floor(sleepTime);
    long long microseconds = static_cast <long long> (roundedSleepTime);
    
    wasSleeping_ = true;

    #ifdef DEBUG
    std::cout << "Engine::mainLoop(): Finished with redundant time. Estimated sleep is " << sleepTime << ". Sleeping for " << microseconds << " microseconds.\n";
    #endif

    std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
  }
  else
  {
    #ifdef DEBUG
    printf("Engine::mainLoop(): No time left for sleep. Proceeding.\n");
    #endif

    wasSleeping_ = false;
  }
}

void Engine::render()
{
  #ifdef DEBUG
  printf("Engine::render(): Going to render scene #%i.\n", renderCounter_);
  #endif

  renderCounter_++;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  renderSprites();
  renderUI();

  glfwPollEvents();

  glfwSwapBuffers(window_ -> getHandle());
}

void Engine::renderSprites()
{
  #ifdef DEBUG
  printf("Engine::renderSprites(): Rendering sprites.\n");
  #endif

  Shader *spriteShader = Shader::getInstance("sprite");

  spriteShader -> setMatrix4f("view", Camera::getActiveCamera() -> getViewMatrix());
  spriteShader -> setMatrix4f("projection", projectionMatrix_);

  spriteRenderer_->renderSprites();
}

void Engine::renderUI()
{
  #ifdef DEBUG
  printf("Engine::renderUI(): Rendering UI\n");
  #endif

  spriteRenderer_->shader_->setMatrix4f("view", glm::mat4(1.0f));
  spriteRenderer_->shader_->setMatrix4f("projection", glm::ortho(0.0f, static_cast<float>(window_->getWidth()), static_cast<float>(window_->getHeight()), 0.0f));

  for (UILayer *layer : UILayer::instances)
  {
    for (UISprite *sprite : layer->UISprites_)
    {
      spriteRenderer_->render(sprite);
    }

    for (UIText *text : layer->UITexts_)
    {
      UItextRenderer_->renderText(text);
    }
  }
}

void Engine::update()
{
  #ifdef DEBUG
  printf("Engine::update(): Going to update game state #%i.\n", updateCounter_);
  #endif

  updateCounter_++;

  // glfwPollEvents();

  /*
  * This is true if user clicks on window to close
  */
  if (glfwWindowShouldClose(window_ -> getHandle()))
  {
    exitProgram_ = true;
  }

  #ifdef DEBUG
  printf("Engine::update(): Updating scenes.\n");
  #endif

  updateScenes();

  if (paused_ == false)
  {
    #ifdef DEBUG
    printf("Engine::update(): Updating game objects.\n");
    #endif

    GameObject::updateInstances();

    #ifdef DEBUG
    printf("Engine::update(): Updating UI objects.\n");
    #endif

    updateUIObjects();

    #ifdef DEBUG
    printf("Engine::update(): Updating physics objects.\n");
    #endif

    Physics::updateInstances();
  }

  #ifdef DEBUG
  printf("Engine::update(): Updating animations.\n");
  #endif

  updateAnimations();

  #ifdef DEBUG
  printf("Engine::update(): Updating clickable buttons.\n");
  #endif

  UIButton::updateInstances();
}

void Engine::updateUIObjects()
{
  for (UILayer *layer : UILayer::instances)
  {
    for (UIObject *obj : layer->UIObjects_)
    {
      obj->update();
    }
  }
}

void Engine::updateScenes()
{
  for (Scene *scene : Scene::instances)
  {
    scene->update();
  }
}

void Engine::updateAnimations()
{
  for (Animation *anim : Animation::instances)
  {
    anim->update();
  }
}

void Engine::setZoom(float zoom)
{
  engine->setZoomPrivate(zoom);
}

void Engine::modifyZoom(float zoomMod)
{
  engine->modifyZoomPrivate(zoomMod);
}

void Engine::setZoomPrivate(float zoom)
{
  if (zoom > 0)
  {
    zoom_ = zoom;

    updateProjectionMatrix();
  }
  else
  {
    #ifdef DEBUG
    printf("Engine::setZoomPrivate(): Zoom has to be greater than 0.\n");
    #endif
  }
}

void Engine::modifyZoomPrivate(float zoomMod)
{
  float modifiedZoom = zoom_ + zoomMod;

  if (modifiedZoom > 0)
  {
    zoom_ = modifiedZoom;

    updateProjectionMatrix();
  }
  else
  {
    #ifdef DEBUG
    printf("Engine::modifyZoomPrivate(): Zoom has to be greater than 0.\n");
    #endif
  }
}

void Engine::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
  engine -> setWindowSize(width, height);
}

void Engine::setWindowSize(int width, int height)
{
  window_ -> setSize(width, height);
  updateProjectionMatrix();
  UItextRenderer_ -> updateProjectionMatrix(width, height);
  UIObject::updateOnWindowResize(width, height);
}

/*
* glm::ortho(xMin, xMax, yMin, yMax, zMin, zMax)
* 
* dividing screen resolution by 2 and assigning result value
* with negative sign as min value
* with positive sign as max value
* makes 0.0 coordinate in the middle of screen
*/

void Engine::updateProjectionMatrix()
{
  float left   = window_ -> getWidth() / 2.0f * -1.0f * zoom_;
  float right  = window_ -> getWidth() / 2.0f * zoom_;
  float bottom = window_ -> getHeight() / 2.0f * -1.0f * zoom_;
  float top    = window_ -> getHeight() / 2.0f * zoom_;

  projectionMatrix_ = glm::ortho(left, right, bottom, top, 0.0f, -1000.0f);
}

void Engine::pause(bool pause)
{
  engine->paused_ = pause;
}

void Engine::exitProgram()
{
  engine->exitProgram_ = true;
}