#include "Encoding.h"
#include <sstream>
#include <cmath>

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

std::string encodeLine(int x1, int y1, int x2, int y2) {
    std::stringstream result;
    result << frhdEncode(x1) 
	<< " " << frhdEncode(y1) << " " << frhdEncode(x2) << " " << frhdEncode(y2) << ",";
    return result.str();
}
