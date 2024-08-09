#include <iostream>
#include <fstream>
#include "ImageProcessing.h"
#include "Encoding.h"
#include "RLERun.h"
#include "ArgParser.h"

int main(int argc, char* argv[]) {
    ArgParser parser;

    // define options and flags:
    parser.addOption("--input", "input file");
    parser.addOption("--output", "output file");
    parser.addFlag("--verbose", "enable verbose output");

    try {
        // parse the command-line arguments
        parser.parse(argc, argv);

        // retrieve the values
        std::string inputFile = parser.getOption("--input");
        std::string outputFile = parser.getOption("--output");
        bool verbose = parser.getFlag("--verbose");

        // use the parsed values
        if (verbose) {
            std::cout << "verbose mode enabled." << std::endl;
        }

        if (!inputFile.empty()) {
            if (verbose) {
                std::cout << "input file: " << inputFile << std::endl;
            }

            // read the image data
            cv::Mat imageData = readImage(inputFile, false);
            std::vector<RLERun> rleRuns = processImage(imageData, 100, 190);

            // output to file or console
            if (!outputFile.empty()) {
                std::ofstream outputStream(outputFile);
                if (outputStream.is_open()) {
                    encodeTrack(outputStream, rleRuns);
                    outputStream.close();
                    if (verbose) {
                        // check the size of the output file
                        std::ifstream inFile(outputFile, std::ios::binary | std::ios::ate);
                        if (inFile) {
                            std::streamsize size = inFile.tellg(); // get the size
                            std::cout << "output written to: " << outputFile << " (size: " << size/1024 << " kilobytes, max upload is 1000 kilobytes)" << std::endl;
                        } else {
                            std::cerr << "error opening file to check size: " << outputFile << std::endl;
                        }
                    }
                } else {
                    std::cerr << "error opening file for writing: " << outputFile << std::endl;
                }
            } else {
                encodeTrack(std::cout, rleRuns); // output to console
            }
        } else {
            std::cerr << "error: input file not specified." << std::endl;
            return 1;
        }

    } catch (const std::runtime_error& e) {
        std::cerr << "error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

