#ifndef ENCODING_H
#define ENCODING_H

#include <string>
#include <ostream>
#include "ImageProcessing.h"

std::string frhdEncode(const int& number);
std::string encodeLine(int x1, int y1, int x2, int y2);
void encodeTrack(std::ostream& out, const std::vector<RLERun>& rleRuns);

#endif // ENCODING_H
