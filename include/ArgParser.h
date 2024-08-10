#ifndef ARGPARSER_HPP
#define ARGPARSER_HPP

#include <map>
#include <stdexcept>
#include <string>

class ArgParser
{
public:
  // adds an option that requires a value
  void addOption(const std::string &option, const std::string &description);

  // adds a flag that does not require a value
  void addFlag(const std::string &flag, const std::string &description);

  // parses command-line arguments
  void parse(int argc, char *argv[]);

  // retrieves the value of a specified option
  std::string getOption(const std::string &option);

  // checks if a specified flag is set
  bool getFlag(const std::string &flag);

  // prints the help message with available options and flags
  void printHelp();

private:
  std::map<std::string, std::string> options; // stores options and their values
  std::map<std::string, bool> flags;          // stores flags and their states
  std::map<std::string, std::string>
      descriptions; // stores descriptions for help message
};

#endif // ARGPARSER_HPP
