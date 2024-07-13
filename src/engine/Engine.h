#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>

#include "Shader.h"
#include "Texture.h"
#include "Sprite.h"
#include "Camera.h"
#include "KeyInput.h"
#include "Window.h"
#include "GameTime.h"
#include "MouseInput.h"
#include "UIObject.h"

#include "UITextRenderer.h"
#include "SpriteRenderer.h"

class Engine
{
public:

  Engine();
  ~Engine();

  GameTime *time_;

  int init();

  /*
  * Condition for engine to run
  */
  bool exitProgram_;

  void mainLoop();
  
  void render();
  void update();

  void deinitialize();

  void setWindowSize(int width, int height);

  static void setZoom(float zoom);
  static void modifyZoom(float zoomMod);
  static void exitProgram();
  static void pause(bool pause);

private:

  UITextRenderer *UItextRenderer_;
  SpriteRenderer *spriteRenderer_;

  MouseInput *mouseInput_;
  
  Window *window_;

  glm::mat4 projectionMatrix_;

  double timeAccumulator_;
  double frameTimeInterval_;
  double maxTimeAccumulated_;
  int fps_;
  int updateCounter_;
  int renderCounter_;
  bool wasSleeping_;

  bool paused_;
  float zoom_;

  /*
  * Init functions
  */
  int initWindow();
  int initGL();
  int initCallbackFunctions();
  int initShaders();
  void initMainLoop();
  void initRender();

  void renderSprites();
  void renderUIText();
  void renderUIButtons();
  void renderUISprites();
  void renderUI();

  void updateScenes();
  void updateAnimations();
  void updateUIObjects();

  void setZoomPrivate(float zoom);
  void modifyZoomPrivate(float zoomMod);

  void updateProjectionMatrix();

  static Engine* engine;
  static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
};