#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <unordered_set>


// Global variables

static const int ALPHA_SIZE = 26;
static const int BOARD_SIZE = 4;
static std::unordered_set<std::string> words;


// node
//   Struct for storing a node in a trie.

struct node {
  bool end = false;
  node* children[ALPHA_SIZE] = { nullptr };
};


// word_board
//   Struct for storing the word board.

struct word_board {
  char cells[BOARD_SIZE * BOARD_SIZE];
  bool visited[BOARD_SIZE * BOARD_SIZE];

  char get_cell(int x, int y) {
    return cells[x * BOARD_SIZE + y];
  }

  void set_cell(char c, int x, int y) {
    cells[x * BOARD_SIZE + y] = c;
  }

  bool get_visited(int x, int y) {
    return visited[x * BOARD_SIZE + y];
  }

  void set_visited(bool val, int x, int y) {
    visited[x * BOARD_SIZE + y] = val;
  }
};


// Function prototypes

void add_word(node* head, std::string w);
void find_words(word_board* board, node* dictionary, std::string w, int x, int y);
void unload_dictionary(node* dictionary);


// main()
//   Solves GamePigeon's word hunt.

int main() {
  // Parse board
  std::ifstream boardfile("board.txt");
  std::string w;
  if (!boardfile.is_open()) {
    std::cout << "Error: could not open expected file `board.txt`." << std::endl;
    return -1;
  }
  word_board board;
  int x = 0;
  while (getline(boardfile, w) && x < BOARD_SIZE) {
    if (w.length() != BOARD_SIZE) {
      std::cout << "Invalid board: must be " << BOARD_SIZE << " columns." << std::endl;
      return -1;
    }
    for (unsigned y = 0; y < BOARD_SIZE; ++y) {
      board.set_cell(w[y], x, y);
    }
    ++x;
  }
  if (x != BOARD_SIZE) {
    std::cout << "Invalid board: must be " << BOARD_SIZE << " rows." << std::endl;
    return -1;
  }

  // Parse dictionary
  std::ifstream dictfile("dictionary.txt");
  if (!dictfile.is_open()) {
    std::cout << "Error: could not open expected file `dictionary.txt`." << std::endl;
    return -1;
  }
  node* dictionary = new node;
  while (getline(dictfile, w)) {
    add_word(dictionary, w);
  }
  dictfile.close();

  // Find and print words
  for (unsigned i = 0; i < BOARD_SIZE; ++i) {
    for (unsigned j = 0; j < BOARD_SIZE; ++j) {
      std::string s = "";
      find_words(&board, dictionary, s, i, j);
    }
  }
  for (auto s : words) {
    std::cout << s << std::endl;
  }

  // Free memory
  unload_dictionary(dictionary);
}


// add_word(head, w)
//   Adds the word `w` to the trie starting at `head`.

void add_word(node* head, std::string w) {
  node* curr = head;
  for (unsigned i = 0, n = w.length() - 1; i < n; ++i) {
    int idx = w[i] - 'A';
    if (!curr->children[idx]) {
      curr->children[idx] = new node;
    }
    curr = curr->children[idx];
  }
  curr->end = true;
}


// find_words(board, dictionary, w, x, y)
//   Finds the words in `board` that are defined in `dictionary`
//   starting at cell (`x`, `y`), adding the found word `w` to the
//   global `words` unordered set.

void find_words(word_board* board, node* dictionary, std::string w, int x, int y) {
  board->set_visited(true, x, y);
  if (dictionary->end) {
    words.insert(w);
  }
  char c = board->get_cell(x, y);
  w.append(1, c);
  for (unsigned i = std::max(0, x - 1), n = std::min(BOARD_SIZE, x + 2); i < n; ++i) {
    for (unsigned j = std::max(0, y - 1), m = std::min(BOARD_SIZE, y + 2); j < m; ++j) {
      node* next = dictionary->children[c - 'A'];
      if (board->get_visited(i, j) || !next) {
        continue;
      }
      find_words(board, next, w, i, j);
    }
  }
  board->set_visited(false, x, y);
}


// unload_dictionary(dictionary)
//   Frees memory allocated to create `dictionary`.

void unload_dictionary(node* dictionary) {
  for (auto n : dictionary->children) {
    if (n) {
      unload_dictionary(n);
    }
  }
  delete dictionary;
}
