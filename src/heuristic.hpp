
#pragma once
#include "algorithm.hpp"
#include "parser.hpp"
#include <unordered_map>

struct Heuristic : public Algorithm {
  std::unordered_map<std::string, BlifNode> Mapping;
  std::vector<std::string> PrimaryInputs;
  void parse(const std::vector<BlifNode> &all_nodes,
             const std::vector<std::string> &primary_input) override;
  std::vector<std::array<std::string, 3>> run(int and_con, int or_con,
                                              int inv_con) override;
};