#include "FileManager.h"

#include <fstream>
#include <sstream>
#include <stdio.h>
#include <algorithm>

#undef DEBUG

const std::string FileManager::PARAMETERS_PATH = "parameters.txt";

std::string FileManager::readParameter(const std::string& parameterName)
{
  std::ifstream istr(PARAMETERS_PATH);

  std::string line;
  std::string ret;

  std::string::size_type separatorPos;
  std::string::size_type firstQuote;
  std::string::size_type lastQuote;

  ret = "error";

  if (!istr)
  {
    printf("FileManager::readParameter(): ERROR: Failed to open file: %s.\n", PARAMETERS_PATH.c_str());

    return ret;
  }

  while (std::getline(istr, line))
  {
    #ifdef DEBUG
    printf("FileManager::readParameter(): Read line: %s.\n", line.c_str());
    #endif

    std::remove(line.begin(), line.end(), ' ');

    separatorPos = line.find('=');
    if (separatorPos == std::string::npos)
    {
      printf("FileManager::readParameter(): WARNING! Separator sign '=' not found.\n");
      continue;
    }

    if (line.compare(0, separatorPos, parameterName) == 0)
    {
      firstQuote = line.find("\"", separatorPos);
      if (firstQuote == std::string::npos)
      {
        printf("FileManager::readParameter(): ERROR! First quote sign '\"' not found.\n");
        return ret;
      }

      lastQuote = line.find("\"", firstQuote + 1);
      if (lastQuote == std::string::npos)
      {
        printf("FileManager::readParameter(): ERROR! Last quote sign '\"' not found.\n");
        return ret;
      }

      ret = line.substr(firstQuote + 1, lastQuote - firstQuote - 1);

      #ifdef DEBUG
      printf("FileManager::readParameter(): Found desired parameter: %s\n", ret.c_str());
      #endif

      return ret;
    }
  }

  #ifdef DEBUG
  printf("FileManager::readParameter(): Couldn't find parameter name: %s\n", parameterName.c_str());
  #endif

  return ret;
}

void FileManager::readWholeFile(const std::string& filePath, std::vector<char>& buffer)
{
  std::ifstream istr(filePath, std::ios_base::in | std::ios_base::binary);

  if (!istr)
  {
    printf("FileManager::readWholeFile(): ERROR: Failed to open file: %s.\n", filePath.c_str());

    return;
  }

  istr.seekg(0, istr.end);
  size_t size = istr.tellg();
  istr.seekg(0, istr.beg);
  buffer.resize(size);
  istr.read(&buffer[0], size);
  istr.close();
}
