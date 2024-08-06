#ifndef RLERUN_H
#define RLERUN_H

#include <vector>
#include <string>
#include <sstream>
#include "PixelColor.h"

struct RLERun {
    int y;
    std::vector<int> xValues;
    PixelColor color;

    std::string toString() const;
};

#endif // RLERUN_H
