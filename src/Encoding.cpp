#include "Encoding.h"
#include "ImageProcessing.h"
#include <cmath>
#include <sstream>

// Performance is given as O(1)
std::string frhdEncode(const int &number)
{
  static const char alphabet[] = "0123456789abcdefghijklmnopqrstuv";
  static const std::size_t maxDigits = 8;

  char buffer[maxDigits + 2];         // +1 for sign, +1 for null terminator
  char *ptr = buffer + maxDigits + 1; // End of the buffer
  *ptr = '\0';                        // Null terminate the string

  unsigned int absNumber = std::abs(number);

  do
  {
    *(--ptr) =
        alphabet[absNumber % 32]; // Corresponding index in char alphabet[]
    absNumber /= 32;              // Prepare for next iteration
  } while (absNumber > 0);        // Continue until number > 0

  if (number < 0)
  {
    *(--ptr) = '-'; // Assign value to buffer and decrement
  }

  return std::string(ptr);
}

// Performance is given as O(1)
std::string encodeLine(int x1, int y1, int x2, int y2)
{
  std::stringstream result;
  result << frhdEncode(x1) << " " << frhdEncode(y1) << " " << frhdEncode(x2)
         << " " << frhdEncode(y2) << ",";
  return result.str();
}

// Out is an ofstream object, can be consoleout or to a file
void encodeTrack(std::ostream &out, const std::vector<RLERun> &rleRuns)
{
  for (const auto &run : rleRuns)
  {
    if (run.color == 0 && !run.xValues.empty()) // Assuming BLACK = 0
    {
      int x1 = run.xValues.front(); // First x-coordinate
      int x2 = run.xValues.back();  // Last x-coordinate
      int y = run.y;                // y-coordinate
      std::string encodedLine = encodeLine(x1, y, x2, y);
      out << encodedLine; // Output the encoded line
    }
  }
  out << "#";
  for (const auto &run : rleRuns)
  {
    if (run.color == 1 && !run.xValues.empty()) // Assuming GRAY = 1
    {
      int x1 = run.xValues.front(); // First x-coordinate
      int x2 = run.xValues.back();  // Last x-coordinate
      int y = run.y;                // y-coordinate
      std::string encodedLine = encodeLine(x1, y, x2, y);
      out << encodedLine; // Output the encoded line
    }
  }
  out << "#" << std::endl;
}
