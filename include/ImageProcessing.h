#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <string>
#include <vector>
#include "RLERun.h"

cv::Mat readImage(const std::string& imagePath, const bool& grayscale);
std::vector<RLERun> processImage(const cv::Mat& image, const int& blackThreshold, const int& grayThreshold);

#endif // IMAGEPROCESSING_H
