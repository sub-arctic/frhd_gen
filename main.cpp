#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdexcept>

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

void thresholdImage(cv::Mat& image, const int& upperThreshold, const int& lowerThreshold) {
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
		std::cout << "white" << std::endl;
	    } else if (pixelValue > lowerThreshold) { // gray
		std::cout << "gray" << std::endl;
	    } else { // black
		std::cout << "black" << std::endl;
	    }
	}
    }


}

int main(int argc, char* argv[]) {

    const std::string imagePath = argv[1];

    cv::Mat imageData = readImage(imagePath, false);
    thresholdImage(imageData, 100, 200);
    return 0;
}
