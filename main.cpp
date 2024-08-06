#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <algorithm>

// convert integer to base32-like format supported by freeriderhd
std::string frhdEncode(const int& number) {
    static const char alphabet[] = "0123456789abcdefghijklmnopqrstuv";
    static const std::size_t maxDigits = 8;

    char buffer[maxDigits + 2]; // +1 for sign, +1 for null terminator
    char* ptr = buffer + maxDigits + 1; // end of the buffer
    *ptr = '\0'; // null terminate the string
    
    unsigned int absNumber = std::abs(number);

    do {
	*(--ptr) = alphabet[absNumber % 32]; // corresponding index in char alphabet[]
	absNumber /= 32; // prepare for next iteration 
    } while (absNumber > 0); // continue until number > 0

    if (number < 0) {
	*(--ptr) = '-'; // assign value to buffer and decrement
    }

    return std::string(ptr);
}

cv::Mat readImage(const std::string& imagePath, const bool& grayscale) {
    cv::Mat image = cv::imread(imagePath, grayscale ? cv::IMREAD_GRAYSCALE : cv::IMREAD_UNCHANGED);

    if (image.empty()) {
	std::cout << "Image file is empty or not found" << std::endl;
	throw std::runtime_error("Image not found");
    }

    return image;
}
std::pair<std::vector<cv::Point>, std::vector<cv::Point>>
thresholdImage(const cv::Mat& image, const int& lowerThreshold, const int& upperThreshold) {

    std::vector<cv::Point> grayPixels, blackPixels;
    
    cv::Mat grayImage;

    // convert to grayscale if nessecary
    if (image.channels() != 1) {
	cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    } else {
	grayImage = image;
    }

    for (int row = 0; row < grayImage.rows; ++row) {
        for (int col = 0; col < grayImage.cols; ++col) {
            const uchar pixelValue = grayImage.at<uchar>(row, col);
            if (pixelValue > upperThreshold) { // white
		// do nothing
            } else if (pixelValue > lowerThreshold) { // gray
		grayPixels.emplace_back(cv::Point(row, col));
            } else { // black
		blackPixels.emplace_back(cv::Point(row, col));
            }
        }
    }

    std::pair<std::vector<cv::Point>, std::vector<cv::Point>> pixels = std::make_pair(blackPixels, grayPixels);
    return pixels;
}


// encode cartesian coordinates using frhdEncode
std::string encodeLine(int x1, int y1, int x2, int y2)
{
    std::stringstream result;
    result << frhdEncode(x1) 
	<< " " << frhdEncode(y1) << " " << frhdEncode(x2) << " " << frhdEncode(y2) << ",";
    return result.str();
}

// wip optimisation for solid-line drawing
void optimize(std::vector<cv::Point>& blackPixels, std::vector<cv::Point>& grayPixels) {
    std::vector<cv::Point> pixels = blackPixels;
    // sort pixel values by y:
    std::sort(pixels.begin(), pixels.end(),
	[](const cv::Point& a, const cv::Point&b) {
	    return a.y < b.y;
	});

    for (const auto& point : pixels) {
	std::cout << "Point(" << point.x << ", " << point.y << ")" << std::endl;
    }
}


int main(int argc, char* argv[]) {

    const std::string imagePath = argv[1];

    cv::Mat imageData = readImage(imagePath, false);
    std::pair<std::vector<cv::Point>, std::vector<cv::Point>> pixels = thresholdImage(imageData, 100, 200);
    optimize(pixels.first, pixels.second);

    return 0;
}
