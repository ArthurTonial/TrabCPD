#include <iostream>
#include <string>

using namespace std;

typedef struct {
    char letter;
    Trie_node *left;
    Trie_node *middle;
    Trie_node *right;
    long sofifa_id;
} Trie_node;

Trie_node *players_trie_root;
Trie_node *tags_trie_root;

void trie_insert(Trie_node *root, string key, int id);

void trie_search(Trie_node *root, string query);
