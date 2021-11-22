#ifndef TRABCPD_FASE2
#define TRABCPD_FASE2

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <algorithm>


// funcao de leitura de query solicitada
void query(string cmd, string arg);
// funcoes de consulta de dados
void playerSearch(Trie_node *root, string name);
vector<int> trieSearch(Trie_node *root, string query);
void userRatingsSearch(int userId);
void topPositionSearch(int N, string pos);
// funcoes de hash
void HashPos(string s, int ID);
void HashTag(string s, int ID);
int HornerHash(string s, int N);
vector<int> create_tag_list(string tag);

#endif //TRABCPD_FASE2
