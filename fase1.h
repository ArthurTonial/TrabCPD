#ifndef TRABCPD_FASE1
#define TRABCPD_FASE1

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

// funcoes de leitura de arquivos
int readPlayers();
int readRating();
int readTags();
// funcao para criar novo jogador
Player createPlayer(int id, string name, string pos);
// funcao para preparar lista de posicoes
vector<string> addPos(string pos, int id);
// funcoes de insercao em arvore trie
Trie_node *new_node(char letter);
void trieInsert(Trie_node **node, const char *key, int id);

#endif //TRABCPD_FASE1