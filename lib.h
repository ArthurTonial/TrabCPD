#ifndef TRABCPD_LIB_H
#define TRABCPD_LIB_H

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <algorithm>

#define ID_MAX 300000
#define PLAYER_MAX 18946
#define USER_MAX 200000
#define NAME_MAX 255
#define POS_COUNT 17

using namespace std;

// estrutura do jogador
struct Player{
    int id;
    string name;
    vector<string> pos;
    double sum;
    int count;
    double rat = sum/count;
};

//estrutura de dados para arvore trie
struct Trie_node{
    char letter;
    Trie_node *left;
    Trie_node *middle;
    Trie_node *right;
    long sofifa_id;
} ;

Player createPlayer(int id, string name, string pos);

// funcoes de leitura de arquivos
int readPlayers();
int readRating();
int readTags();

// faz hash entre id de jogador e suas posicoes
vector<string> addPos(string pos, int id);

// funcoes de arvore trie
Trie_node *new_node(char letter);
void trieInsert(Trie_node **node, const char *key, int id);
vector<int> trieSearch(Trie_node *root, string query);
void traverse(Trie_node *root, vector<int> *fifa_ids);

// funcao de menu
void query(string cmd, string arg);
void print();

void playerSearch(Trie_node *root, string name);
void userRatingsSearch(int userId);
void topPositionSearch(int N, string pos);
void tagPlayersSearch(vector<string> tags);
void hashPos(string s, int ID);
void hashTag(string s, int ID);
int hornerHash(string s, int N);
vector<int> create_tag_list(string tag);
vector<string> prepareTags(string tags);

#endif //TRABCPD_LIB_H