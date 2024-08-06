#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <sstream>

struct Point {
    int x;
    int y;
};

struct Line {
    Point p1;
    Point p2;
};

enum PixelColor {
    WHITE = 0,
    BLACK = 1,
    GRAY = 2
};

struct RLERun {
    int y;
    std::vector<int> xValues;
    PixelColor color;

    std::string toString() const {
	std::ostringstream oss;
	oss << y << ": ";
	for (size_t i = 0; i < xValues.size(); ++i) {
	    if (i > 0) {
		oss << ", ";
	    }
	    oss << xValues[i];
	}
	oss << "(Color: " << (color == BLACK ? "Black" : (color == GRAY ? "Gray" : "White")) << ")";
	return oss.str();
    }
};

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

std::vector<RLERun> processImage(const cv::Mat& image, const int& blackThreshold, const int& grayThreshold) {

    std::vector<cv::Point> grayPixels, blackPixels;

    cv::Mat grayImage;

    // convert to grayscale if nessecary
    if (image.channels() != 1) {
	cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    } else {
	grayImage = image;
    }
    
    std::vector<RLERun> rleRuns;

    for (int row = 0; row < grayImage.rows; ++row) {
	int count = 0;
	PixelColor currentColor = BLACK;
	for (int col = 0; col < grayImage.cols; ++col) {
	    const uchar pixelValue = grayImage.at<uchar>(row, col);
	    
	    PixelColor color;
	    if (pixelValue < blackThreshold) {
		color = BLACK;
	    } else if (pixelValue >= blackThreshold && pixelValue < grayThreshold) {
		color = GRAY;
	    } else {
		color = WHITE;
	    }

	    if (color == currentColor) {
		count++;
	    } else {
		if (count > 0) {
		    RLERun run;
		    run.y = row;

		    // check the number of neighbors
		    int neighbors = 0;
		    if (col - count > 0) neighbors++; // left neighbor
		    if (col < grayImage.cols - 1) neighbors++; // right neighbor

		    // adjust the run coordinates if there are fewer than 2 neighbors
		    int startX = col - count + 1;
		    if (neighbors < 2) {
			startX += 2; // adjust by +2
		    }

		    run.xValues.resize(count);
		    for (int i = 0; i < count; ++i) {
			run.xValues[i] = startX + i; // set adjusted startx
		    }
		    run.color = currentColor;
		    rleRuns.push_back(run);
		}
		currentColor = color;
		count = 1;
	    }
	}
	if (count > 0) {
	    RLERun run;
	    run.y = row;

	    // check the number of neighbors for the last run
	    int neighbors = 0;
	    if (grayImage.cols - count - 1 >= 0) neighbors++; // left neighbor
	    if (grayImage.cols - 1 < grayImage.cols) neighbors++; // right neighbor

	    // adjust the run coordinates if there are fewer than 2 neighbors
	    int startX = image.cols - count;
	    if (neighbors < 2) {
		startX += 2; // adjust by +2
	    }

	    run.xValues.resize(count);
	    for (int i = 0; i < count; ++i) {
		run.xValues[i] = startX + i; // set adjusted startx
	    }
	    run.color = currentColor;
	    rleRuns.push_back(run);
	}
    }
    return rleRuns;
}

// encode cartesian coordinates using frhdencode
std::string encodeLine(int x1, int y1, int x2, int y2)
{
    std::stringstream result;
    result << frhdEncode(x1) 
	<< " " << frhdEncode(y1) << " " << frhdEncode(x2) << " " << frhdEncode(y2) << ",";
    return result.str();
}

// wip optimisation for solid-line drawing
void optimize(std::vector<cv::Point>& blackPixels, std::vector<cv::Point>& grayPixels) {
    for (const cv::Point& point : blackPixels) {
	std::cout << "x " << point.x << " y " << point.y << std::endl;
    }
}

int main(int argc, char* argv[]) {

    const std::string imagePath = argv[1];

    cv::Mat imageData = readImage(imagePath, false);
    std::vector<RLERun> rleRuns = processImage(imageData, 100, 200);

    // for (const auto& run : rleruns) {
    //     std::cout << run.tostring() << std::endl;
    // }
    for (const auto& run : rleRuns) {
        if (run.color == BLACK && run.xValues.size() > 0) {
            int x1 = run.xValues.front(); // first x-coordinate
            int x2 = run.xValues.back();  // last x-coordinate
            int y = run.y;                 // y-coordinate
            std::string encodedLine = encodeLine(x1, y, x2, y);
            std::cout << encodedLine; // output the encoded line
        }
    }
    std::cout << "#";
    for (const auto& run : rleRuns) {
        if (run.color == GRAY && run.xValues.size() > 0) {
            int x1 = run.xValues.front(); // first x-coordinate
            int x2 = run.xValues.back();  // last x-coordinate
            int y = run.y;                 // y-coordinate
            std::string encodedLine = encodeLine(x1, y, x2, y);
            std::cout << encodedLine; // output the encoded line
        }
    }
    std::cout << "#" << std::endl;


    return 0;
}
