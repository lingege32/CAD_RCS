
#pragma once
#include "algorithm.hpp"
#include "parser.hpp"

struct Heuristic : public Algorithm {
  std::vector<std::vector<BlifNode>> BestSolution;
  void parse(const std::vector<BlifNode> &inputs) override;
  std::vector<std::array<std::string,3>> run(int and_con, int or_con,
                                            int inv_con) override;
};