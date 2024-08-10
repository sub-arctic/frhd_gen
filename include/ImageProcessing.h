#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include "RLERun.h"
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <string>
#include <vector>

cv::Mat readImage(const std::string &imagePath, const bool &grayscale);
std::vector<RLERun> processImage(const cv::Mat &image,
                                 const int &blackThreshold,
                                 const int &grayThreshold);

#endif // IMAGEPROCESSING_H
