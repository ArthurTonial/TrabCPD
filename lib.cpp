#include "lib.h"

using namespace std;

// arvores trie de nomes e tags
Trie_node *players_trie_root;
Trie_node *tags_trie_root;

// arrays de acesso rapido a partir de IDs
int playerID_translator[PLAYERS_ID_MAX];
int userID_translator[USERS_ID_MAX];

vector<pair<int,double>> user_ratings[USER_MAX];
vector<string> player_positions[PLAYER_MAX];
string player_names[PLAYER_MAX];
double sum_rating[PLAYER_MAX];
int total_rating[PLAYER_MAX];

int curr_player_number = 1;
int curr_user_number = 1;

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
            player_names[playerID_translator[sofifa_id]] = aux_string;
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
    ifstream csv("dados/minirating.csv");

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

            if (userID_translator[user_id] == 0) {
                userID_translator[user_id] = curr_user_number;
                curr_user_number++;
            }

            user_ratings[userID_translator[user_id]].push_back({sofifa_id, rating});
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

    if (positions_string[0] != '"') {
        player_positions[id].push_back(positions_string);
    }
    else {
        string temp_string;

        for(int i = 1; i < positions_string.size(); i++){

            if (positions_string[i] != ',' and positions_string[i] != '"') {
                temp_string.push_back(positions_string[i]);
            }
            else {
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

void read_cmd(string cmd) {

    // player <name or prefix>
    if (cmd.compare(0, 6, "player ")) {
        player_search(players_trie_root, cmd.substr(7));
    }

    // user <userID>
    else if (cmd.compare(0, 4, "user ")) {
        user_ratings_search(stoi(cmd.substr(5)));
    }

    // top<N> ‘<position>’
    else if (cmd.compare(0, 2, "top")) {
       
        int N;
        string position;

        for (int i = 3; i < cmd.size(); i++) {
            if (cmd[i] != ' ' and cmd[i] != '\'') {
                position.push_back(cmd[i]);
            }
            else if (cmd[i] == ' ') {
                N = stoi(position);
                position.clear();
            }
        }
        //top_position_search(N, position);
    }

    // tags <list of tags>
    else if (cmd.compare(0, 4, "tags ")) {
                
        for (int i = 5; i < cmd.size(); i++) {
            if (cmd[i] != ' ' and cmd[i] != '\'') {
                position.push_back(cmd[i]);
            }
            else if (cmd[i] == ' ') {
                N = stoi(position);
                position.clear();
            }
        }
        //tag_players_search();
    }

    // comando invalido
    else cout << "Comando invalido\n";
}

void traverse(Trie_node *root, vector<int> *fifa_ids){
    if(root){
        if(root->sofifa_id != -1) (*fifa_ids).push_back(root->sofifa_id);
        traverse(root->left, fifa_ids);
        traverse(root->right, fifa_ids);
        traverse(root->middle, fifa_ids);
    }
}

// -> FUNCOES PRINCIPAIS DE CONSULTAS DE DADOS
vector<int> trie_search(Trie_node *root, string query) {

    vector<int> fifa_ids;

    if (!root) return fifa_ids;
    
    int i = 0;
    while(i < query.size()){
        if(root){
            if (query[i] > root->letter) {root = root->right;}
            else if (query[i] < root->letter) {root = root->left;}
            else if (i < query.size()) { root = root->middle; i++;}
            else i++;
        }
        else {
           cout << "no matching result. :(";
           return fifa_ids;
        }   
    }
    
    if(root->sofifa_id != -1) fifa_ids.push_back(root->sofifa_id);

    traverse(root->left, &fifa_ids);
    traverse(root->right, &fifa_ids);
    traverse(root->middle, &fifa_ids);

    return fifa_ids;
}

void player_search(Trie_node *root, string player_name) {

    vector<int> ids = trie_search(root, player_name);

    cout << "\nSOFIFA_ID  |  NAME\t\t\t\t |  POSITIONS  |   RATING   |  COUNT\n";
    cout << "---------------------------------------------------------------------------------------\n";

    for (int i = 0; i < ids.size(); i++) {
        printf("%6d        ", ids[i]); 
        int id = playerID_translator[ids[i]];
        printf("%-36s ", player_names[id].c_str()); 
        int j;
        for (j = 0; j < player_positions[id].size(); j++) {
            printf("%-s ", player_positions[id][j].c_str());
        }
        for (; j < 3; j++) cout << "   ";
        float avg_rating = sum_rating[id]/total_rating[id];
        if(!isnan(avg_rating)) printf("\t   %.5f\t", avg_rating);
        else printf("\t      -\t\t"); 
        printf(" %-6d\n", total_rating[id]); 

    }

}
