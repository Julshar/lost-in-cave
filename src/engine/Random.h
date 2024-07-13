#pragma once

#include <string>
#include <random>

class Random
{
public:

  static void seedWithTime();
  static void seedWithString(std::string str);
  static void seedWithNumber(unsigned int num);

  static unsigned int genValue(unsigned int min, unsigned int max);

  static int getMinVal();
  static int getMaxVal();

private:

  static std::random_device randDev;
  static std::mt19937 generator;

};