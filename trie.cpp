#include "trie.h"

using namespace std;

// insere novo nome na arvore trie
void trie_insert(Trie_node *root, string key, int id) {
   
    // nodo que percorre arvore
    Trie_node *pCrawl = root;

    // percorre arvore
    for (int i = 0; i < key.length(); i++) {

        // nodo nao inicializado
        if (pCrawl->letter == NULL) {
            pCrawl->letter = key[i];
            pCrawl->sofifa_id = -1;
        }

        if (key[i] < pCrawl->letter) {
            if (pCrawl->left == nullptr) {
                Trie_node *new_node;
                pCrawl->left = new_node;
            }
            if (i != key.length()-1) pCrawl = pCrawl->left;
        }
        else if (key[i] > pCrawl->letter) {
            if (pCrawl->right == nullptr) {
                Trie_node *new_node;
                pCrawl->right = new_node;
            }
            if (i != key.length()-1) pCrawl = pCrawl->right;
        }
        else {
            if (pCrawl->middle == nullptr) {
                Trie_node *new_node;
                pCrawl->middle = new_node;
            }
            if (i != key.length()-1) pCrawl = pCrawl->middle;
        }
    }
 
    // marca como final do nome
    pCrawl->sofifa_id = id;
}