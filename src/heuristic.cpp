#include "heuristic.hpp"
#include <algorithm>
#include <iostream>
#include <list>
#include <unordered_map>
// #include <iostream>

void insert_node_to_mapping(const BlifNode &node,
                            std::unordered_map<std::string, size_t> &mapping,
                            size_t &max) {
  size_t max_level = 1;
  const auto &inputs = node.inputs;
  for (const auto &input : inputs) {
    auto it = mapping.find(input);
    if (it != mapping.end()) {
      max_level = std::max(max_level, it->second + 1);
    }
  }
  max = std::max(max, max_level);
  mapping.emplace(node.output, max_level);
}

void Heuristic::parse(const std::vector<BlifNode> &inputs) {
  std::unordered_map<std::string, size_t> node_to_level;
  size_t max = 0;
  for (const auto &node : inputs) {
    insert_node_to_mapping(node, node_to_level, max);
  }
  BestSolution.resize(max);
  for (const auto &node : inputs) {
    auto out = node.output;
    auto level = node_to_level[out] - 1;
    BestSolution[level].emplace_back(node);
  }
}

void try_place_the_nodes(std::vector<BlifNode> &v, BlifNode &node, int &con,
                         const std::list<BlifNode> &candidate) {
  if (con == 0) {
    return;
  }
  for (const auto &input : node.inputs) {
    if (std::any_of(v.begin(), v.end(), [&](const BlifNode &node) {
          return node.output == input;
        })) {
      return;
    }
    if (std::any_of(
            candidate.begin(), candidate.end(),
            [&](const BlifNode &node) { return node.output == input; })) {
      return;
    }
  }
  con--;
  v.emplace_back(std::move(node));
  node.output = "";
}
std::vector<std::array<std::string, 3>> Heuristic::run(int and_con, int or_con,
                                                       int inv_con) {

  std::vector<BlifNode> one_level_ans;
  std::list<BlifNode> list1, list2;
  std::vector<std::vector<BlifNode>> ans;
  for (auto every_level : BestSolution) {
    std::sort(every_level.begin(), every_level.end(),
              [](const BlifNode &lhs, const BlifNode &rhs) {
                return lhs.inputs.size() > rhs.inputs.size();
              });
    std::move(every_level.begin(), every_level.end(),
              std::back_inserter(list1));
  }
  int and_c = and_con;
  int or_c = or_con;
  int inv_c = inv_con;

  while (!list1.empty() || !list2.empty()) {
    if (list1.empty() || (and_c == 0 && or_c == 0 && inv_c == 0)) {
      ans.emplace_back(std::move(one_level_ans));
      one_level_ans.clear();
      and_c = and_con;
      or_c = or_con;
      inv_c = inv_con;
      list2.splice(list2.end(), list1);
      list1.swap(list2);
    }
    auto &n = list1.front();
    if (n.gate == LogicGate::AND) {
      try_place_the_nodes(one_level_ans, n, and_c, list2);
    } else if (n.gate == LogicGate::OR) {
      try_place_the_nodes(one_level_ans, n, or_c, list2);
    } else if (n.gate == LogicGate::INV) {
      try_place_the_nodes(one_level_ans, n, inv_c, list2);
    }
    if (n.output != "") {
      list2.push_back(n);
    }
    list1.pop_front();
  }
  if (!one_level_ans.empty()) {
    ans.emplace_back(std::move(one_level_ans));
  }
  std::vector<std::array<std::string, 3>> ret;
  ret.reserve(ans.size());
  for (const auto &a : ans) {
    std::array<std::string, 3> one_level;
    for (const auto &node : a) {
      one_level[static_cast<size_t>(node.gate)] += node.output;
      one_level[static_cast<size_t>(node.gate)] += " ";
    }
    for (auto &gate_info : one_level) {
      if (!gate_info.empty()) {
        gate_info.resize(gate_info.size() - 1);
      }
    }
    ret.emplace_back(std::move(one_level));
  }
  return ret;
}