
#include "algorithm.hpp"
#include "arg_parser.hpp"
#include "parser.hpp"
int main(int argc, char **argv) {
  auto arg = ParseArgument(argc, argv);
  Parser parse;
  parse.parse(arg.file);
//   parse.print();

  auto algorithm = makeAlgorithm(arg.mode);
  algorithm->parse(parse.getAllNodes());
  auto result =
      algorithm->run(arg.and_constraint, arg.or_constraint, arg.inv_constraint);

  // dump
  if (arg.mode == "-e") {
    std::cout << "ILP-based Scheduling Result\n";
  } else {
    std::cout << "Heuristic Scheduling Result\n";
  }
  for (size_t level = 0; level < result.size(); ++level) {
    std::cout << level + 1 << ": ";
    for (const auto &gate : result[level]) {
      std::cout << "{" << gate << "} ";
    }
    std::cout << "\n";
  }
  std::cout << "LATENCY: " << result.size() << "\n";
  std::cout << "END\n";
}