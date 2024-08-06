#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdexcept>

cv::Mat readImage(const std::string& imagePath, const bool& grayscale) {
    cv::Mat image = cv::imread(imagePath, grayscale ? cv::IMREAD_GRAYSCALE : cv::IMREAD_UNCHANGED);

    if (image.empty()) {
	std::cout << "Image file is empty or not found" << std::endl;
	throw std::runtime_error("Image not found");
    }

    return image;
}

int main(int argc, char* argv[]) {

    cv::Mat imageData = readImage(argv[1], true);
    return 0;
}
