#include <vector>

using namespace std;

#define NAME_MAX 255
#define BUFFER_MAX 511
#define PLAYERS_MAX 250000
#define USERS_MAX 250000

// acessar avaliações do jogador
float sum_rating[PLAYERS_MAX];
int total_rating[PLAYERS_MAX];

// acessar avalaiacoes feitas por usuario (pair<id,nota>)
vector<pair<int,double>> user_ratings[USERS_MAX];

// funcoes iniciais
int read_players_csv();
int read_rating_csv();
int read_tags_csv();

void read_cmd(string cmd);