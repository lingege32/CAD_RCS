

#pragma once

#include <string>
#include <vector>

enum class LogicGate { AND, OR, INV };

struct BlifNode {
  std::string output;
  std::vector<std::string> inputs;
  LogicGate gate;

  void print() const;
};

struct Parser {
  std::string ModelName;
  std::vector<std::string> inputs;
  std::vector<std::string> outputs;
  std::vector<BlifNode> nodes;

  void parse(const std::string &file);
  void print() const;
  std::vector<BlifNode> getAllNodes() { return nodes; }
};
