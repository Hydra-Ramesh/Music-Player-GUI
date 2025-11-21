#pragma once
#include <iostream>
#include <string>

struct Node {
  std::string filepath;
  std::string filename;
  Node *next;
  Node *prev;

  Node(std::string path, std::string name)
      : filepath(path), filename(name), next(nullptr), prev(nullptr) {}
};

class PlaylistDLL {
public:
  PlaylistDLL() : head(nullptr), tail(nullptr), current(nullptr), size(0) {}

  ~PlaylistDLL() { Clear(); }

  void Add(const std::string &filepath) {
    // Simple filename extraction
    std::string name = filepath.substr(filepath.find_last_of("/\\") + 1);

    Node *newNode = new Node(filepath, name);
    if (!head) {
      head = tail = current = newNode;
    } else {
      tail->next = newNode;
      newNode->prev = tail;
      tail = newNode;
    }
    size++;
  }

  void Clear() {
    Node *temp = head;
    while (temp) {
      Node *next = temp->next;
      delete temp;
      temp = next;
    }
    head = tail = current = nullptr;
    size = 0;
  }

  bool IsEmpty() const { return size == 0; }
  int GetSize() const { return size; }

  // O(1) Navigation
  Node *Next() {
    if (current && current->next) {
      current = current->next;
      return current;
    }
    return nullptr;
  }

  Node *Prev() {
    if (current && current->prev) {
      current = current->prev;
      return current;
    }
    return nullptr;
  }

  void SetCurrent(Node *node) { current = node; }

  Node *GetCurrent() const { return current; }
  Node *GetHead() const { return head; }

private:
  Node *head;
  Node *tail;
  Node *current;
  int size;
};
