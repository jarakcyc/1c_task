#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

typedef size_t NodeIndex;

class Trie {
public:
  Trie() : nodes_(1, Node()) {};

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

  void Add(const std::string& word) {
    NodeIndex v = 0;
    for (char c : word) {
      if (nodes_[v].link.count(c) == 0) {
        nodes_[v].link[c] = nodes_.size();
        nodes_.push_back(Node());
      }
      v = nodes_[v].link[c];
    }
    nodes_[v].counter++;
  }

  TrieState Go(TrieState start_state, std::string next) {
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
    std::pair<std::string, int> best_result = Dfs(start_state);
    return best_result.first;
  }

private:
  struct Node {
    std::unordered_map<char, int> link;
    int counter;
    Node() :
      counter(0) {
    };
  };

  std::vector<Node> nodes_;

  std::pair<std::string, int> Dfs(TrieState state) {
    std::pair<std::string, int> res = {"", 0};
    if (nodes_[state.index].counter > 0) {
      res = {state.word, nodes_[state.index].counter};
    }

    for (auto p : nodes_[state.index].link) {
      std::string new_word = state.word;
      new_word += p.first;
      std::pair<std::string, int> candidate = Dfs(TrieState(p.second, new_word));
      if (candidate.second > res.second) {
        res = candidate;
      }
    }

    return res;
  }
};

void SolutionWithSimpleTrie() {
  Trie trie;

  Trie::TrieState state;

  char query_type;
  while (std::cin >> query_type) {
    if (query_type == '!') {
      int n;
      std::cin >> n;
      for (int i = 0; i < n; ++i) {
        std::string word;
        std::cin >> word;
        trie.Add(word);
      }
    } else if (query_type == '?') {
      std::string word;
      std::cin >> word;
      state = trie.Go(Trie::TrieState(), word);
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
      std::cerr << "unknown type" << std::endl;
    }
  }
}

int main() {
#ifdef LOCAL
  freopen("input.txt", "rt", stdin);
  freopen("output.txt", "wt", stdout);
#endif // LOCAL

  SolutionWithSimpleTrie();

  return 0;
}
