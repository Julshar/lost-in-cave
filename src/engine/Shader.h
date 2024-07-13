#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <fstream>
#include <sstream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/ext/matrix_transform.hpp>

class Shader
{
public:

  std::string name_;

	Shader(char* vertexFilePath, char* fragmentFilePath, std::string name);
	~Shader();

	static Shader* getInstance(std::string name);
	static void initInstance(char* vertexFilePath, char* fragmentFilePath, std::string name);

	void use();
	void unuse();
	unsigned int getId();

  void setInteger(const GLchar* name, int value);
	void setFloat(const GLchar* name, float value);
	void setVector3f(const GLchar* name, glm::fvec3 value);
	void setVector4f(const GLchar* name, glm::fvec4 value);
	void setVector4f(const GLchar* name, float* value);
	void setMatrix4f(const GLchar *name, glm::mat4 value);

private:

  Shader(std::string name);

  static std::vector<Shader *> instances;

	GLuint shaderID_;

	std::string loadShaderSource(char* filePath);
	GLuint createShader(GLenum type, char* filePath);
	void linkShader(GLuint vertexShader, GLuint fragmentShader);
};