#include "ArgParser.h"
#include <iostream>

// Adds an option that requires a value
void ArgParser::addOption(const std::string &option,
                          const std::string &description)
{
  options[option] = "";
  descriptions[option] = description;
}

// Adds a flag that does not require a value
void ArgParser::addFlag(const std::string &flag, const std::string &description)
{
  flags[flag] = false;
  descriptions[flag] = description;
}

// Parses command-line arguments
void ArgParser::parse(int argc, char *argv[])
{
  for (int i = 1; i < argc; ++i)
  {
    std::string arg = argv[i];

    if (options.find(arg) != options.end())
    {
      if (i + 1 < argc)
      {
        options[arg] = argv[++i];
      }
      else
      {
        throw std::runtime_error("Option " + arg + " requires a value.");
      }
    }
    else if (flags.find(arg) != flags.end())
    {
      flags[arg] = true;
    }
    else if (arg == "--help" || arg == "-h")
    {
      printHelp();
      exit(0);
    }
    else
    {
      throw std::runtime_error("Unknown argument: " + arg);
    }
  }
}

// Retrieves the value of a specified option
std::string ArgParser::getOption(const std::string &option)
{
  return options[option];
}

// Checks if a specified flag is set
bool ArgParser::getFlag(const std::string &flag) { return flags[flag]; }

// Prints the help message with available options and flags
void ArgParser::printHelp()
{
  std::cout << "Usage:\n";
  for (const auto &[key, desc] : descriptions)
  {
    std::cout << "  " << key << ": " << desc << "\n";
  }
}
