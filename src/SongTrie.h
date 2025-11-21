#pragma once
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

struct TrieNode {
  std::unordered_map<char, TrieNode *> children;
  bool isEndOfWord;
  std::vector<std::string>
      filepaths; // Store full paths at leaf or intermediate nodes

  TrieNode() : isEndOfWord(false) {}
};

class SongTrie {
public:
  SongTrie() { root = new TrieNode(); }

  ~SongTrie() {
    // TODO: Implement recursive delete
  }

  void Insert(const std::string &filename, const std::string &filepath) {
    TrieNode *node = root;
    std::string lowerName = filename;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),
                   ::tolower);

    for (char c : lowerName) {
      if (node->children.find(c) == node->children.end()) {
        node->children[c] = new TrieNode();
      }
      node = node->children[c];
    }
    node->isEndOfWord = true;
    node->filepaths.push_back(filepath);
  }

  // O(L) Search where L is length of query
  std::vector<std::string> Search(const std::string &prefix) {
    TrieNode *node = root;
    std::string lowerPrefix = prefix;
    std::transform(lowerPrefix.begin(), lowerPrefix.end(), lowerPrefix.begin(),
                   ::tolower);

    for (char c : lowerPrefix) {
      if (node->children.find(c) == node->children.end()) {
        return {}; // Not found
      }
      node = node->children[c];
    }

    // Collect all songs from this node downwards
    std::vector<std::string> results;
    CollectAll(node, results);
    return results;
  }

private:
  TrieNode *root;

  void CollectAll(TrieNode *node, std::vector<std::string> &results) {
    if (node->isEndOfWord) {
      for (const auto &path : node->filepaths) {
        results.push_back(path);
      }
    }
    for (auto &pair : node->children) {
      CollectAll(pair.second, results);
    }
  }
};
