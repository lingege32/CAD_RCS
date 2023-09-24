#include "heuristic.hpp"
#include <algorithm>
#include <iostream>
#include <list>
#include <queue>
#include <unordered_set>

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

void Heuristic::parse(const std::vector<BlifNode> &all_nodes,
                      const std::vector<std::string> &primary_input) {

  PrimaryInputs = primary_input;
  for (const auto &node : all_nodes) {
    Mapping[node.output] = node;
  }
}

std::unordered_map<std::string, std::pair<size_t, std::vector<std::string>>>
prepare_node_inputs_mapping(
    const std::unordered_set<std::string> &PrimaryInputs,
    const std::unordered_map<std::string, BlifNode> &mapping) {
  std::unordered_map<std::string, std::pair<size_t, std::vector<std::string>>>
      node_in_out;
  for (const auto &pair : mapping) {
    auto &name = pair.first;
    auto &node = pair.second;
    auto input_nums = std::count_if(
        node.inputs.begin(), node.inputs.end(), [&](const auto &n) {
          return PrimaryInputs.find(n) == PrimaryInputs.end();
        });
    node_in_out[name].first = input_nums;
    for (const auto &input : node.inputs) {
      if (PrimaryInputs.find(input) == PrimaryInputs.end()) {
        node_in_out[input].second.push_back(name);
      }
    }
  }
  return node_in_out;
}

void try_insert_the_node(
    const std::string &n, std::vector<std::string> &one_level, int &con,
    std::unordered_map<std::string, std::pair<size_t, std::vector<std::string>>>
        &node_in_out,
    std::queue<std::string> &second_queue) {
  if (con == 0) {
    second_queue.push(n);
    return;
  }
  con--;
  one_level.emplace_back(n);
  for (const auto &input : node_in_out[n].second) {
    if (0 == --node_in_out[input].first) {
      second_queue.push(input);
    }
  }
}

std::vector<std::array<std::string, 3>> Heuristic::run(int and_con, int or_con,
                                                       int inv_con) {
  std::unordered_set<std::string> Placed;
  for (const auto &input : PrimaryInputs) {
    Placed.emplace(input);
  }
  auto input_num_of_the_node = prepare_node_inputs_mapping(Placed, Mapping);

  auto cmp = [&](const std::string &lhs, const std::string &rhs) {
    return Mapping[lhs].inputs.size() < Mapping[rhs].inputs.size();
  };
  std::priority_queue<std::string, std::vector<std::string>, decltype(cmp)>
      candidates{cmp};
  for (const auto &pair : input_num_of_the_node) {
    if (pair.second.first == 0) {
      candidates.push(pair.first);
    }
  }

  std::queue<std::string> second_queue;
  int ac = and_con;
  int oc = or_con;
  int ic = inv_con;
  std::vector<std::string> one_level;
  std::vector<std::vector<std::string>> ans;
  while (!(candidates.empty() && second_queue.empty())) {
    if (candidates.empty() || (ac == 0 && oc == 0 && ic == 0)) {
      ac = and_con;
      oc = or_con;
      ic = inv_con;
      ans.emplace_back(std::move(one_level));
      decltype(one_level){}.swap(one_level);
      while (!second_queue.empty()) {
        candidates.push(second_queue.front());
        second_queue.pop();
      }
    }

    auto n = candidates.top();
    candidates.pop();
    const auto &blifnode = Mapping[n];
    if (blifnode.gate == LogicGate::AND) {
      try_insert_the_node(n, one_level, ac, input_num_of_the_node,
                          second_queue);
    } else if (blifnode.gate == LogicGate::OR) {
      try_insert_the_node(n, one_level, oc, input_num_of_the_node,
                          second_queue);
    } else if (blifnode.gate == LogicGate::INV) {
      try_insert_the_node(n, one_level, ic, input_num_of_the_node,
                          second_queue);
    }
  }
  ans.emplace_back(std::move(one_level));
  std::vector<std::array<std::string, 3>> ret;
  ret.reserve(ans.size());
  for (const auto &a : ans) {
    std::array<std::string, 3> one_level_ret;
    for (const auto &node : a) {
      one_level_ret[static_cast<size_t>(Mapping[node].gate)] += node;
      one_level_ret[static_cast<size_t>(Mapping[node].gate)] += " ";
    }
    for (auto &gate_info : one_level_ret) {
      if (!gate_info.empty()) {
        gate_info.resize(gate_info.size() - 1);
      }
    }
    ret.emplace_back(std::move(one_level_ret));
  }
  return ret;
}