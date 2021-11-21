#include "lib.h"

using namespace std;

// arvores trie de nomes e tags
Trie_node *players_trie_root;
Trie_node *tags_trie_root;

// arrays de acesso rapido a partir de IDs
int playerID_translator[PLAYERS_ID_MAX];
int userID_translator[PLAYERS_ID_MAX];
vector<pair<int,double>> user_ratings[USER_MAX];
vector<string> player_positions[PLAYER_MAX];
string player_names[PLAYER_MAX];
double sum_rating[PLAYER_MAX];
int total_rating[PLAYER_MAX];

int curr_player_number = 1;


// -> FUNCOES PRINCIPAIS DE INICIALIZACAO DE DADOS
// funcao para ler arquivo de jogadores
int read_players_csv() {

    // abre arquivo de leitura
    ifstream csv("dados/players.csv");

    if (!csv.is_open()) {
        cout << "> erro ao abrir players.csv\n";
        return 1;
    }
    else {
        cout << "> players.csv aberto\n";
        
		// pula primeira linha
        string aux_string; 
		getline(csv, aux_string);

		// preenche dados dos jogadores
        while (csv) {
			int sofifa_id; csv >> sofifa_id;
            playerID_translator[sofifa_id] = curr_player_number;
            char dump; csv >> dump;

			getline(csv, aux_string, ',');
			trie_insert(&players_trie_root, aux_string.c_str(), sofifa_id);
			getline(csv, aux_string);
			add_positions(aux_string, sofifa_id);
            curr_player_number++;
        }
        csv.close();
        return 0;
    }
}

// funcao para ler arquivo de ratings
int read_rating_csv() {

    // abre arquivo de leitura
    ifstream csv("dados/rating.csv");

    if (!csv.is_open()) {
        cout << "> erro ao abrir rating.csv\n";
        return 1;
    }
    else {
        cout << "> rating.csv aberto\n";

		// pula primeira linha
        string aux_string; 
		getline(csv, aux_string);

		// preenche dados de rating
        while (csv) {
            int user_id; csv >> user_id;
            char dump; csv >> dump;
			int sofifa_id; csv >> sofifa_id;
            csv >> dump;
			double rating; csv >> rating;
            
            sum_rating[playerID_translator[sofifa_id]] += rating;
            total_rating[playerID_translator[sofifa_id]] += 1;

            pair<int,double> user_rating = {sofifa_id, rating};
            user_ratings[userID_translator[user_id]].push_back(user_rating);
        }

        csv.close();
        return 0;
    }
}

// funcao para ler arquivo de tags
int read_tags_csv() {

    // abre arquivo de leitura
    ifstream csv("dados/tags.csv");

    if (!csv.is_open()) {
        cout << "> erro ao abrir tags.csv\n";
        return 1;
    }
    else {
        cout << "> tags.csv aberto\n";

		// pula primeira linha
        string aux_string; 
		getline(csv, aux_string);

		// preenche dados de tags
        while (csv) {
            int user_id; csv >> user_id;
            char dump; csv >> dump;
			int sofifa_id; csv >> sofifa_id;
            csv >> dump;

			getline(csv, aux_string);
			trie_insert(&tags_trie_root, aux_string.c_str(), sofifa_id);
        }

        csv.close();
        return 0;
    }
}



// -> FUNCOES AUXILIARES DE INICIALIZACAO DE DADOS
// faz hash entre id de jogador e suas posicoes
void add_positions(string positions_string, int id) {
    id = playerID_translator[id];
    if(positions_string[0] != '"'){
        player_positions[id].push_back(positions_string);
    }else{
        string temp_string;
        for(int i = 1; i < positions_string.size(); i++){
            if(positions_string[i] != ',' && positions_string[i] != '"'){
                temp_string.push_back(positions_string[i]);
            }else{
                temp_string.push_back('\0');
                player_positions[id].push_back(temp_string);
                temp_string.clear();
                i++;
            }
        }
    }
}

// cria novo nodo trie
Trie_node *new_node(char letter) {
    
    Trie_node *new_node = (Trie_node*) malloc(sizeof(Trie_node));

    new_node->letter = letter;
    new_node->sofifa_id = -1;
    new_node->left = nullptr;
    new_node->middle = nullptr;
    new_node->right = nullptr;

    return new_node;
}

// insere nova string na arvore trie
void trie_insert(Trie_node **node, const char *key, int id) {

    if (*node == nullptr) *node = new_node(*key);

    if ((*key) < (*node)->letter) trie_insert(&((*node)->left), key, id);

    else if ((*key) > (*node)->letter) trie_insert(&((*node)->right), key, id);

    else if (*(key + 1)) trie_insert(&((*node)->middle), key + 1, id);

    else (*node)->sofifa_id = id;

}



// -> FUNCOES PRINCIPAIS DE CONSULTAS DE DADOS
void trie_search(Trie_node *root, string query) {

}

void print(){
    cout << playerID_translator[222843];
}