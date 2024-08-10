#include "RLERun.h"

std::string RLERun::toString() const
{
  std::ostringstream oss;
  oss << y << ": ";
  for (size_t i = 0; i < xValues.size(); ++i)
  {
    if (i > 0)
    {
      oss << ", ";
    }
    oss << xValues[i];
  }
  oss << "(Color: "
      << (color == BLACK ? "Black" : (color == GRAY ? "Gray" : "White")) << ")";
  return oss.str();
}
