#include "Shader.h"

#undef DEBUG

std::vector<Shader *> Shader::instances;

Shader::Shader(char* vertexFilePath, char* fragmentFilePath, std::string name)
:
name_(name)
{
	GLuint vertex = createShader(GL_VERTEX_SHADER, vertexFilePath);
	GLuint fragment = createShader(GL_FRAGMENT_SHADER, fragmentFilePath);

	this->linkShader(vertex, fragment);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	Shader::instances.push_back(this);
}

void Shader::initInstance(char* vertexFilePath, char* fragmentFilePath, std::string name)
{
	Shader *shader = new Shader(name);
  
	GLuint vertex = shader -> createShader(GL_VERTEX_SHADER, vertexFilePath);
	GLuint fragment = shader -> createShader(GL_FRAGMENT_SHADER, fragmentFilePath);

	shader -> linkShader(vertex, fragment);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

  Shader::instances.push_back(shader);
}

Shader::Shader(std::string name)
:
name_(name)
{

}

Shader::~Shader()
{
	glDeleteProgram(this->shaderID_);

	for (size_t i = 0; i < Shader::instances.size(); ++i)
  {
    if (Shader::instances[i] -> name_ == this -> name_)
    {
      Shader::instances.erase(Shader::instances.begin() + i);

      break;
    }
  }
}

Shader* Shader::getInstance(std::string name)
{
	for (Shader *shader : Shader::instances)
  {
    if (shader -> name_ == name)
    {
      return shader;
    }
  }

  printf("Shader::getInstance(): WARNING! No shader named %s found.\n", name.c_str());

  return nullptr;
}

void Shader::use()
{
	glUseProgram(this->shaderID_);
}

void Shader::unuse()
{
	glUseProgram(0);
}

unsigned int Shader::getId()
{
	return this->shaderID_;
}

void Shader::setInteger(const GLchar* name, int value)
{
	this->use();

	glUniform1i(glGetUniformLocation(this->shaderID_, name), value);

	this->unuse();
}

void Shader::setFloat(const GLchar* name, float value)
{
  this->use();

	glUniform1f(glGetUniformLocation(this->shaderID_, name), value);

	this->unuse();
}

void Shader::setVector3f(const GLchar* name, glm::fvec3 value)
{
	this->use();

	glUniform3fv(glGetUniformLocation(this->shaderID_, name), 1, glm::value_ptr(value));

	this->unuse();
}

void Shader::setVector4f(const GLchar* name, glm::fvec4 value)
{
  this->use();

	glUniform4fv(glGetUniformLocation(this->shaderID_, name), 1, glm::value_ptr(value));

	this->unuse();
}

void Shader::setVector4f(const GLchar* name, float* ptr)
{
  this->use();

	glUniform4fv(glGetUniformLocation(this->shaderID_, name), 1, ptr);

	this->unuse();
}

void Shader::setMatrix4f(const GLchar *name, glm::mat4 value)
{
	this->use();

	glUniformMatrix4fv(glGetUniformLocation(this->shaderID_, name), 1, GL_FALSE, glm::value_ptr(value));

	this->unuse();
}

// -----------------
// --- PRIVATE -----
// -----------------

std::string Shader::loadShaderSource(char* filePath)
{
	std::string tmp = "";
	std::string src = "";

	std::ifstream in_file;

	in_file.open(filePath);

	if (in_file.is_open())
	{
		while (std::getline(in_file, tmp))
		{
			src += tmp + "\n";
		}
	}
	else
	{
		std::cout << "WARNING! Failed to open file " << filePath << "\n";
	}

	in_file.close();
	return src;
}

GLuint Shader::createShader(GLenum type, char* filePath)
{
	char infoLog[512];
	GLint compileStatus;

	GLuint shaderID = glCreateShader(type);
	std::string sourceCode = this->loadShaderSource(filePath);
	const GLchar* src = sourceCode.c_str();
	glShaderSource(shaderID, 1, &src, NULL);
	glCompileShader(shaderID);

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == 0)
	{
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		std::cout << "WARNING! Shader compilation error in shader: " << filePath << "\n";
		std::cout << infoLog << "\n";
	}
	return shaderID;
}

void Shader::linkShader(GLuint vertexShader, GLuint fragmentShader)
{
	char infoLog[512];
	GLint success;
	this->shaderID_ = glCreateProgram();

	glAttachShader(this->shaderID_, vertexShader);
	glAttachShader(this->shaderID_, fragmentShader);

	glLinkProgram(this->shaderID_);

	glGetProgramiv(this->shaderID_, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->shaderID_, 512, NULL, infoLog);
		std::cout << "WARNING! Shader link error" << "\n";
		std::cout << infoLog << "\n";
	}
	glUseProgram(0);
}