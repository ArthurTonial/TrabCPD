#include "fase1.h"

using namespace std;

// arvores trie de nomes e tags
Trie_node *playersTrie;

// arrays de acesso rapido a partir de IDs
int PlayerID[ID_MAX];
int UserID[ID_MAX];

vector<pair<int,double>> ratingsByUser[USER_MAX];
Player playersList[PLAYER_MAX];

vector<pair<string, int>> Pos_players[17]; //USO LA EMBAIXO
vector<pair<string, int>> Tag_players[1000]; //USO LA EMBAIXO

int auxId1 = 1;
int auxId2 = 1;