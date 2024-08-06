#include <iostream>
#include "ImageProcessing.h"
#include "Encoding.h"
#include "RLERun.h"

int main(int argc, char* argv[]) {
    const std::string imagePath = argv[1];
    cv::Mat imageData = readImage(imagePath, false);
    std::vector<RLERun> rleRuns = processImage(imageData, 100, 190);

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
