#ifndef TRABCPD_LIB_H
#define TRABCPD_LIB_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#define NAME_MAX 255
#define BUFFER_MAX 511
#define PLAYERS_MAX 300000
#define USERS_MAX 250000

using namespace std;

//estrutura de dados para arvore trie
struct Trie_node{
    char letter;
    Trie_node *left;
    Trie_node *middle;
    Trie_node *right;
    long sofifa_id;
} ;

// funcoes de leitura de arquivos
int read_players_csv();
int read_rating_csv();
int read_tags_csv();

// faz hash entre id de jogador e suas posicoes
void add_positions(string positions_string, int id);

// funcoes de arvore trie
Trie_node *new_node(char letter);
void trie_insert(Trie_node **node, const char *key, int id);
void trie_search(Trie_node *root, string query);

// funcao de menu
void read_cmd(string cmd);

#endif //TRABCPD_LIB_H