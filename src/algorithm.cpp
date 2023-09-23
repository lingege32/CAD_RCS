
#include "algorithm.hpp"
#include "heuristic.hpp"


std::unique_ptr<Algorithm> makeAlgorithm(const std::string& mode) {
    if (mode == "-h") {
        return std::make_unique<Heuristic>();
    }
    return nullptr;
}