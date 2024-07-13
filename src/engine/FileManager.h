#pragma once

#include <vector>
#include <string>

class FileManager
{
public:

  static std::string readParameter(const std::string& parameterName);
  static void readWholeFile(const std::string& filePath, std::vector<char>& buffer);

private:

  static const std::string PARAMETERS_PATH;

};