#include "MouseInput.h"

#include <cstdlib>
#include <cstdio>

#undef DEBUG

MouseInput *MouseInput::mouseInput = nullptr;

glm::dvec2 MouseInput::cursorPosition;

bool MouseInput::leftPressed = false;
bool MouseInput::rightPressed = false;
bool MouseInput::isInsideWindow = false;

MouseInput::MouseInput(Window *window)
:
window_(window),
cursor_(NULL)
{
  init();
  
  if (mouseInput == nullptr)
  {
    mouseInput = this;
  }
  else
  {
    printf("MouseInput::MouseInput(): ERROR! Mouse input instance was created already.\n");
  }
}

void MouseInput::init()
{
  #ifdef DEBUG
  printf("MouseInput::init(): Setting callback functions.\n");
  #endif

  /*
  * Set callback function on cursor position
  * It is called whenever cursor is moved
  */
  glfwSetCursorPosCallback(window_->getHandle(), cursorPositionCallback);

  /*
  * Set callback function on cursor enter/leave window area
  */
  glfwSetCursorEnterCallback(window_->getHandle(), cursorEnterCallback);

  /*
  * Set callback function on mouse button pressed/released
  */
  glfwSetMouseButtonCallback(window_->getHandle(), mouseButtonCallback);
}

void MouseInput::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
  #ifdef DEBUG
  printf("MouseInput::cursorPositionCallback(): Cursor position change callback.\n");
  printf("MouseInput::cursorPositionCallback(): xpos = %f, ypos = %f.\n", xpos, ypos);
  #endif

  cursorPosition.x = xpos;
  cursorPosition.y = ypos;
}

void MouseInput::cursorEnterCallback(GLFWwindow* window, int entered)
{
  #ifdef DEBUG
  printf("MousInput::cursorEnterCallback(): Cursor exit/enter window area callback.\n");
  #endif

  if (entered)
  {
    #ifdef DEBUG
    printf("MouseInput::cursorEnterCallback(): Cursor entered window content area.\n");
    #endif

    isInsideWindow = true;
  }
  else
  {
    #ifdef DEBUG
    printf("MouseInput::cursorEnterCallback(): Cursor left window content area.\n");
    #endif

    isInsideWindow = false;
  }
}

void MouseInput::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
  #ifdef DEBUG
  printf("MouseInput::mouseButtonCallback(): Cursor button action callback.\n");
  #endif

  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
  {
    #ifdef DEBUG
    printf("MouseInput::mouseButtonCallback(): Right mouse button was pressed.\n");
    #endif

    rightPressed = true;
  }

  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
  {
    #ifdef DEBUG
    printf("MouseInput::mouseButtonCallback(): Right mouse button was released.\n");
    #endif

    rightPressed = false;
  }

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
  {
    #ifdef DEBUG
    printf("MouseInput::mouseButtonCallback(): Left mouse button was pressed.\n");
    #endif

    leftPressed = true;
  }

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
  {
    #ifdef DEBUG
    printf("MouseInput::mouseButtonCallback(): Left mouse button was released.\n");
    #endif

    leftPressed = false;
  }
}

MouseInput::~MouseInput()
{
  if (cursor_ != NULL)
  {
    glfwDestroyCursor(cursor_);
  }
}

glm::dvec2 MouseInput::getPosition()
{
  return cursorPosition;
}

bool MouseInput::isLeftPressed()
{
  return leftPressed;
}

bool MouseInput::isRightPressed()
{
  return rightPressed;
}

bool MouseInput::isCursorInsideWindow()
{
  return isInsideWindow;
}

/*
* data - pointer to first element of array containing RGBA for each pixel of image.
* data length should be equal to (width * height * 4).
* xhot, yhot - coordinates of image that are actual cursor position.
* (0,0) is the top left corner of the cursor image.
*/
int MouseInput::setCursorImage(unsigned char *data, int width, int height, int xhot, int yhot)
{
  GLFWimage image;
  image.width = width;
  image.height = height;
  image.pixels = data;

  mouseInput->cursor_ = glfwCreateCursor(&image, xhot, yhot);

  if (mouseInput->cursor_ == NULL)
  {
    printf("MouseInput::setCursorImage(): ERROR! Failed to create cursor from passed data.\n");

    return 1;
  }

  glfwSetCursor(mouseInput->window_->getHandle(), mouseInput->cursor_);

  return 0;
}

void MouseInput::restoreDefaultCursorImage()
{
  glfwSetCursor(mouseInput->window_->getHandle(), NULL);

  if (mouseInput->cursor_ != NULL)
  {
    glfwDestroyCursor(mouseInput->cursor_);
  }

  mouseInput->cursor_ = NULL;
}