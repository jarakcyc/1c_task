#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

typedef size_t NodeIndex;

class RecursiveTrie {
public:
  RecursiveTrie() : nodes_(1, Node()) {};

  struct TrieState {
    NodeIndex index;
    std::string word;

    TrieState(NodeIndex id, std::string wrd) :
      index(id),
      word(wrd) {
    }

    TrieState() :
      index(0),
      word("") {
    }
  };

  void Add(const std::string& word, int symbol_id, NodeIndex node_index = 0) {
    if (symbol_id == word.size()) {
      nodes_[node_index].counter++;
      if (nodes_[node_index].best.second < nodes_[node_index].counter) {
        nodes_[node_index].best = {"", nodes_[node_index].counter};
      }
      return;
    }

    char next_symbol = word[symbol_id];
    if (nodes_[node_index].link.count(next_symbol) == 0) {
      nodes_[node_index].link[next_symbol] = nodes_.size();
      nodes_.push_back(Node());
    }

    NodeIndex next_node = nodes_[node_index].link[next_symbol];
    Add(word, symbol_id + 1, next_node);
    if (nodes_[next_node].best.second > nodes_[node_index].best.second) {
      std::string new_best = "";
      new_best += next_symbol;
      new_best += nodes_[next_node].best.first;
      nodes_[node_index].best = {new_best, nodes_[next_node].best.second};
    }
  }

  TrieState Go(TrieState start_state, const std::string& next) {
    NodeIndex v = start_state.index;
    if (v == -1) {
      return start_state;
    }
    TrieState current_state = start_state;
    for (char c : next) {
      if (nodes_[v].link.count(c) == 0) {
        return TrieState(-1, "");
      }
      v = nodes_[v].link[c];
      current_state.index = v;
      current_state.word += c;
    }
    return current_state;
  }

  std::string FindBest(TrieState start_state) {
    return start_state.word + nodes_[start_state.index].best.first;
  }

private:
  struct Node {
    std::unordered_map<char, int> link;
    int counter;
    std::pair<std::string, int> best;
    Node() :
      counter(0),
      best({"", 0}) {
    };
  };

  std::vector<Node> nodes_;
};

void SolutionWithRecursiveTrie() {
  RecursiveTrie trie;

  RecursiveTrie::TrieState state;

  char query_type;
  while (std::cin >> query_type) {
    if (query_type == '!') {
      int n;
      std::cin >> n;
      for (int i = 0; i < n; ++i) {
        std::string word;
        std::cin >> word;
        trie.Add(word, 0);
      }
    } else if (query_type == '?') {
      std::string word;
      std::cin >> word;
      state = trie.Go(RecursiveTrie::TrieState(), word);
      if (state.index == -1) {
        std::cout << "no word with same prefix" << std::endl;
      } else {
        std::cout << trie.FindBest(state) << std::endl;
      }
    } else if (query_type == '+') {
      std::string addition;
      std::cin >> addition;
      state = trie.Go(state, addition);
      if (state.index == -1) {
        std::cout << "no word with same prefix" << std::endl;
      } else {
        std::cout << trie.FindBest(state) << std::endl;
      }
    } else {
      std::cout << "unknown type" << std::endl;
    }
  }
}

int main() {
#ifdef LOCAL
  freopen("input.txt", "rt", stdin);
  freopen("output.txt", "wt", stdout);
#endif // LOCAL

  SolutionWithRecursiveTrie();

  return 0;
}
