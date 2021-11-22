#include "lib.h"

using namespace std;

// arvores trie de nomes e tags
Trie_node *playersTrie;

// arrays de acesso rapido a partir de IDs
int PlayerID[ID_MAX];
int UserID[ID_MAX];
// vetores de usuarios e jogadores
vector<pair<int,double>> ratingsByUser[USER_MAX];
Player playersList[PLAYER_MAX];
// Hash
vector<pair<string, int>> Pos_players[POS_COUNT];
vector<pair<string, int>> Tag_players[1000]; 
// auxiliares
int auxId1 = 1; int auxId2 = 1;

///////////////////////////////////////////////////////////////////
//                     FASE1: INICIALIZAÇÃO                      //
///////////////////////////////////////////////////////////////////

// funcao para ler arquivo de jogadores
int readPlayers() {
    // abre arquivo de leitura
    ifstream csv("dados/players.csv");

    if (!csv.is_open()) {
        cout << "> erro ao abrir players.csv\n";
        return 1;
    }
    else {
        cout << "> players.csv aberto\n";
        
		// pula primeira linha
        string aux; 
		getline(csv, aux);
        
		// preenche dados dos jogadores
        while (csv) {
			int id; csv >> id;
            char dump; csv >> dump;
			string name; getline(csv, name, ',');
            for (auto & c: name) c = toupper(c);
			getline(csv, aux);
			
            trieInsert(&playersTrie, name.c_str(), id);
            Player newPlayer = createPlayer(id, name, aux);
            playersList[auxId1] = newPlayer;
            
            PlayerID[id] = auxId1; auxId1++;
        }
        csv.close();
        return 0;
    }
}

// funcao para ler arquivo de ratings
int readRating() {
    // abre arquivo de leitura
    ifstream csv("dados/minirating.csv");

    if (!csv.is_open()) {
        cout << "> erro ao abrir rating.csv\n";
        return 1;
    }
    else {
        cout << "> rating.csv aberto\n";

		// pula primeira linha
        string aux; 
		getline(csv, aux);

		// preenche dados de rating
        while (csv) {
            int user_id; csv >> user_id;
            char dump; csv >> dump;
			int sofifa_id; csv >> sofifa_id;
            csv >> dump;
			double rating; csv >> rating;
            
            playersList[PlayerID[sofifa_id]].sum += rating;
            playersList[PlayerID[sofifa_id]].count += 1;

            if (UserID[user_id] == 0) {
                UserID[user_id] = auxId2; auxId2++;
            }

            ratingsByUser[UserID[user_id]].push_back({sofifa_id, rating});
        }
        csv.close();
        return 0;
    }
}

// funcao para ler arquivo de tags
int readTags() {
    // abre arquivo de leitura
    ifstream csv("dados/tags.csv");

    if (!csv.is_open()) {
        cout << "> erro ao abrir tags.csv\n";
        return 1;
    }
    else {
        cout << "> tags.csv aberto\n";

		// pula primeira linha
        string aux; 
		getline(csv, aux);

		// preenche dados de tags
        while (csv) {
            int user_id; csv >> user_id;
            char dump; csv >> dump;
			int sofifa_id; csv >> sofifa_id;
            csv >> dump;

            getline(csv, aux);
            aux.push_back('\0');
			hashTag(aux, sofifa_id);
        }
        csv.close();
        return 0;
    }
}

// funcao para criar novo jogador
Player createPlayer(int id, string name, string pos) {
    Player newPlayer; 
    newPlayer.id = id;
    newPlayer.name = name;
    newPlayer.pos = addPos(pos, id); 
    return newPlayer;
}

// funcao para fazer hash entre sofifa_id e suas posicoes
vector<string> addPos(string posString, int id) { 
    vector<string> positions;

    if (posString[0] != '"') {
        positions.push_back(posString); 
        posString.push_back('\0');       
        hashPos(posString, id);
    }
    else {
        string temp;
        for(int i = 1; i < posString.size(); i++){
            if (posString[i] != ',' and posString[i] != '"') {
                temp.push_back(posString[i]);
            }
            else {
                temp.push_back('\0');
                hashPos(temp, id);
                positions.push_back(temp);
                temp.clear();
                i++;
            }
        }
    }
    return positions;
}

// funcao para criar novo nodo trie
Trie_node *new_node(char letter) {
    Trie_node *new_node = (Trie_node*) malloc(sizeof(Trie_node));
    new_node->letter = toupper(letter);
    new_node->sofifa_id = -1;
    new_node->left = nullptr;
    new_node->middle = nullptr;
    new_node->right = nullptr;
    return new_node;
}

// funcao para inserir nova string na arvore trie
void trieInsert(Trie_node **node, const char *key, int id) {
    if (*node == nullptr) *node = new_node(*key);
    if (toupper((*key)) < (*node)->letter) trieInsert(&((*node)->left), key, id);
    else if (toupper((*key)) > (*node)->letter) trieInsert(&((*node)->right), key, id);
    else if (*(key + 1)) trieInsert(&((*node)->middle), key + 1, id);
    else (*node)->sofifa_id = id;
}

// funcao para
void hashPos(string s, int id){
    int key = hornerHash(s, 17);
    pair<string, int> id_pos = {s, id};
    Pos_players[key].push_back(id_pos);
}

// fuincao para
void hashTag(string s, int id){
    int key = hornerHash(s, 1000);
    pair<string, int> id_tag = {s, id};
    Tag_players[key].push_back(id_tag);
}

// fauncao para
int hornerHash(string s, int N){
    int hash = 0;

    for(int i = 0; i < s.length(); i++)
        hash = (31 * hash + s.at(i)) % N;

    return hash;
}

///////////////////////////////////////////////////////////////////
//                       FASE2: CONSULTAS                        //
///////////////////////////////////////////////////////////////////



// funcao para decodificar e encaminhar input do usuario
void query(string cmd, string arg) {   
    // player <name or prefix>
    if (cmd == "player") {
        for (auto & c: arg) c = toupper(c);
        playerSearch(playersTrie, arg);
    }
    // user <userID>
    else if (cmd == "user") {
        userRatingsSearch(stoi(arg));
    }
    // top<N> ‘<position>’
    else if (cmd.substr(0, 3) == "top") {   
        string n;
        for (int i = 3; cmd[i] != ' '; i++) n.push_back(cmd[i]);
        topPositionSearch(stoi(n), arg);
    }
    // tags <list of tags>
    else if (cmd == "tags") {
        tagPlayersSearch(arg);
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
vector<int> trieSearch(Trie_node *root, string query) {

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

void playerSearch(Trie_node *root, string name) {

    vector<int> ids = trieSearch(root, name);

    cout << "\nSOFIFA_ID  |  NAME\t\t\t\t |  POSITIONS  |   RATING   |  COUNT\n";
    cout << "---------------------------------------------------------------------------------------\n";

    for (int i = 0; i < ids.size(); i++) {
        printf("%6d        ", ids[i]); 
        int id = PlayerID[ids[i]];
        printf("%-36s ", playersList[id].name.c_str());
        int j;
        for (j = 0; j < playersList[id].pos.size(); j++) {
            printf("%-s ", playersList[id].pos[j].c_str());
        }
        for (; j < 3; j++) cout << "   ";
        float avg_rating = playersList[id].sum/playersList[id].count;
        if(!isnan(avg_rating)) printf("\t   %.5f\t", avg_rating);
        else printf("\t      -\t\t"); 
        printf(" %-6d\n", playersList[id].count); 
    }
}

void topPositionSearch(int N, string pos){  

    vector<int> ids;
    
    pos.push_back('\0'); 
    int key = hornerHash(pos, 17);
    

    for(int i = 0;i < Pos_players[key].size(); i++){
        if(!strcmp(Pos_players[key][i].first.c_str(), pos.c_str()))
            ids.push_back(Pos_players[key][i].second);
    }

    //sort ids

    cout << "\nSOFIFA_ID  |  NAME\t\t\t\t |  POSITIONS  |   RATING   |  COUNT\n";
    cout << "---------------------------------------------------------------------------------------\n";

    for (int i = 0; i < N; i++) {
        printf("%6d        ", ids[i]); 
        int id = PlayerID[ids[i]];
        printf("%-36s ", playersList[id].name.c_str()); 
        int j;
        for (j = 0; j < playersList[id].pos.size(); j++) {
            printf("%-s ", playersList[id].pos[j].c_str());
        }
        for (; j < 3; j++) cout << "   ";
        float avg_rating = playersList[id].sum/playersList[id].count;
        if(!isnan(avg_rating)) printf("\t   %.5f\t", avg_rating);
        else printf("\t      -\t\t"); 
        printf(" %-6d\n", playersList[id].count); 

    }

}

void tagPlayersSearch(vector<string> tags) {

}

vector<int> create_tag_list(string tag){
    vector<int> players_with_tag;
    tag.push_back('\0');
    int key = hornerHash(tag, 1000);

    for(int i = 0; i < Tag_players[key].size(); i++){
        int sofifa_id = Tag_players[key][i].second;
        int player_id = PlayerID[sofifa_id];
        if(Tag_players[key][i].first == tag && playersList[player_id].count > 2) {
            players_with_tag.push_back(playersList[player_id].id);
        }
    }
    return players_with_tag;
}

void userRatingsSearch(int userId){
    
    userId = UserID[userId];
    // sort de ratings do jogador ()

    cout << "\nSOFIFA_ID  |  NAME\t\t\t\t |   GLOBAL    |   COUNT    |  RATING\n";
    cout << "---------------------------------------------------------------------------------------\n";

    for (int i = 0; i < min(20, (int)ratingsByUser[userId].size()); i++) {
        printf("%6d        ", ratingsByUser[userId][i].first); 
        int player_id = PlayerID[ratingsByUser[userId][i].first];
        printf("%-36s ", playersList[player_id].name.c_str()); 
        float avg_rating = playersList[player_id].sum/playersList[player_id].count;
        if(!isnan(avg_rating)) printf("  %.5f\t", avg_rating);
        else printf("\t      -\t\t"); 
        printf("     %-6d", playersList[player_id].count); 
        printf("\t%0.1f\n", ratingsByUser[userId][i].second);
    }
}




