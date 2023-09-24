
#pragma once
#include "parser.hpp"
#include <array>
#include <memory>
struct Algorithm {
    virtual void parse(const std::vector<BlifNode>& all_nodes, const std::vector<std::string>& primary_input) = 0;
    virtual std::vector<std::array<std::string,3>> run(int and_con, int or_con, int inv_con) = 0;
    virtual ~Algorithm() = default;
};

std::unique_ptr<Algorithm> makeAlgorithm(const std::string& mode);