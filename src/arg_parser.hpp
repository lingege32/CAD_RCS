
#pragma once

#include <iostream>
#include <string>

struct ArgParser {
  std::string mode;
  int and_constraint;
  int or_constraint;
  int inv_constraint;
  std::string file;
};

inline void PrintError(int argc, char **argv) {
  std::cout << "Command: ";
  for (int i = 0; i < argc; ++i) {
    std::cout << argv[i] << " ";
  }
  std::cout << "is not legal command.\n";
  std::cout << "Please use " << argv[0];
  std::cout << " -h/-e <blif> AND_CONSTRAINT OR_CONSTRAINT INV_CONSTRAINT\n";
  exit(1);
}

inline ArgParser ParseArgument(int argc, char **argv) {
  if (argc != 6) {
    PrintError(argc, argv);
  }
  ArgParser p;
  p.mode = argv[1];
  if (p.mode != "-h" && p.mode != "-e") {
    PrintError(argc, argv);
  }
  p.file = argv[2];
  p.and_constraint = std::stoi(argv[3]);
  p.or_constraint = std::stoi(argv[4]);
  p.inv_constraint = std::stoi(argv[5]);
  return p;
}