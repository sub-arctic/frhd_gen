#include "ImageProcessing.h"

cv::Mat readImage(const std::string& imagePath, const bool& grayscale) {
    cv::Mat image = cv::imread(imagePath, grayscale ? cv::IMREAD_GRAYSCALE : cv::IMREAD_UNCHANGED);
    if (image.empty()) {
        std::cout << "Image file is empty or not found" << std::endl;
        throw std::runtime_error("Image not found");
    }
    return image;
}

// performance is calculated as O(m * n), where m is the number of rows in the image, and n is the number of columns
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
