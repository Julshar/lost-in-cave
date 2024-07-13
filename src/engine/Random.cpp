#include "Random.h"

#include <chrono>

#include <stdio.h>

#undef DEBUG

std::random_device Random::randDev;
std::mt19937 Random::generator;

void Random::seedWithTime()
{
  #ifdef DEBUG
  printf("Random::seedWithTime(): Seeding random number generator with current elapsed time.\n");
  #endif

  std::chrono::system_clock::duration timeSinceEpoch = std::chrono::system_clock::now().time_since_epoch();
  std::mt19937::result_type seconds = std::chrono::duration_cast<std::chrono::seconds>(timeSinceEpoch).count();
  std::mt19937::result_type microseconds = std::chrono::duration_cast<std::chrono::microseconds>(timeSinceEpoch).count();

  #ifdef DEBUG
  printf("Random::seedWithTime(): Current elapsed time is %u seconds or %u microseconds.\n", seconds, microseconds);
  #endif

  std::mt19937::result_type seed = randDev() ^ (seconds + microseconds);

  generator.seed(seed);
}

void Random::seedWithString(std::string str)
{
  #ifdef DEBUG
  printf("Random::seedWithString(): Seeding random number generator with string: \"%s\".\n", str.c_str());
  #endif

  std::seed_seq seed(str.begin(), str.end());

  generator.seed(seed);
}

void Random::seedWithNumber(unsigned int num)
{
  #ifdef DEBUG
  printf("Random::seedWithNumber(): Seeding random number generator with number: \"%u\".\n", num);
  #endif

  std::mt19937::result_type seed = num;

  generator.seed(seed);
}

unsigned int Random::genValue(unsigned int min, unsigned int max)
{
  if (min >= max)
  {
    printf("Random::genValue(): WARNING! Invalid input values.\n");

    return 0;
  }

  #ifdef DEBUG
  printf("Random::genValue(): Generating random number from range [%u, %u].\n", min, max);
  #endif

  std::mt19937::result_type randomNum = generator();
  std::mt19937::result_type cap = generator.max() - (generator.max() - (max - min)) % max;
  
  while (randomNum > cap)
  {
    randomNum = generator();
  }

  std::mt19937::result_type result = (randomNum % (max - min + 1)) + min;
  
  #ifdef DEBUG
  printf("Random::genValue(): Generated %u.\n", result);
  #endif

  return result;
}

int Random::getMinVal()
{
  return generator.min();
}

int Random::getMaxVal()
{
  return generator.max();
}
