#ifndef RLERUN_H
#define RLERUN_H

#include "PixelColor.h"
#include <sstream>
#include <string>
#include <vector>

struct RLERun
{
  int y;
  std::vector<int> xValues;
  PixelColor color;

  std::string toString() const;
};

#endif // RLERUN_H
