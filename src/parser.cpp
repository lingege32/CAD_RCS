#include "parser.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
namespace {

std::vector<std::string> split(const std::string &line) {
  std::vector<std::string> split;
  std::stringstream ss;
  ss << line;
  std::string s;
  while (ss >> s) {
    split.emplace_back(std::move(s));
  }
  return split;
}
} // namespace

LogicGate getLogic(size_t condition, size_t input_size) {
  if (input_size == 1) {
    return LogicGate::INV;
  }
  if (condition == input_size) {
    return LogicGate::OR;
  }
  return LogicGate::AND;
}

void Parser::parse(const std::string &file) {
  std::ifstream fin(file, std::ios::in);
  std::string buffer;
  BlifNode node;
  size_t num = 0;
  while (std::getline(fin, buffer)) {
    auto split_string = split(buffer);
    auto &title = split_string[0];
    if (title == ".names" || title == ".end") {
      if (!node.output.empty()) {
        node.gate = getLogic(num, node.inputs.size());
        nodes.emplace_back(std::move(node));
        num = 0;
        node.output.clear();
        node.inputs.clear();
      }

      std::move(split_string.begin() + 1, split_string.end() - 1,
                std::back_inserter(node.inputs));
      node.output = split_string.back();
      if (title == ".end") {
        return;
      }

    } else if (title == ".model") {
      ModelName = split_string[1];
    } else if (title == ".inputs") {
      std::move(split_string.begin() + 1, split_string.end(),
                std::back_inserter(inputs));

    } else if (title == ".outputs") {
      std::move(split_string.begin() + 1, split_string.end(),
                std::back_inserter(outputs));

    } else {
      if (split_string.size() == 2 &&
          (split_string[1] == "1" || split_string[1] == "0")) {
        num++;
      }
    }
  }
}

void BlifNode::print() const {
  switch (gate) {
  case LogicGate::AND:
    std::cout << "AND ";
    break;
  case LogicGate::OR:
    std::cout << "OR ";
    break;
  case LogicGate::INV:
    std::cout << "INV ";
    break;
  }

  for (const auto &input : inputs) {
    std::cout << input << " ";
  }
  std::cout << " -> ";
  std::cout << output << "\n";
}

void Parser::print() const {
  std::cout << "Model: " << ModelName << "\n";
  std::cout << "Inputs: ";
  for (const auto &input : inputs) {
    std::cout << input << " ";
  }
  std::cout << "\n";
  std::cout << "Outputs: ";
  for (const auto &output : outputs) {
    std::cout << output << " ";
  }
  std::cout << "\n";
  std::cout << "ALL Logic:\n";
  for (const auto &logic : nodes) {
    logic.print();
  }
}