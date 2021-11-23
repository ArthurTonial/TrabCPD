#include "lib.h"

using namespace std;

///////////////////////////////////////////////////////////////////
//                        FASE0: GLOBAIS                         //
///////////////////////////////////////////////////////////////////

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
    ifstream csv("dados/rating.csv");

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
int hornerHash(string s, int N){
    int hash = 0;
    for(int i = 0; i < s.length(); i++){
        hash = (31 * hash + s.at(i)) % N;
    }
    return hash;
}

// funcao para
void hashPos(string s, int id){
    int key = hornerHash(s, 17);
    pair<string, int> id_pos = {s, id};
    Pos_players[key].push_back(id_pos);
}

// funcao para
void hashTag(string s, int id){
    int key = hornerHash(s, 1000);
    pair<string, int> id_tag = {s, id};
    Tag_players[key].push_back(id_tag);
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
        string n, arg2 = arg.substr(1, arg.size()-2); 
        for (int i = 3; cmd[i] != ' '; i++) n.push_back(cmd[i]);
        for (auto & c: arg2) c = toupper(c);
        arg.push_back('\0');
        if (Pos_players[hornerHash(arg2, 17)].size()) topPositionSearch(stoi(n), arg2);
        else cout << "Posicao invalida";
    }
    // tags <list of tags>
    else if (cmd == "tags") {
        tagPlayersSearch(prepareTags(arg));
    }
    // comando invalido
    else cout << "Comando invalido\n";
}

// funcao para query de busca a partir de nome ou prefixo
void playerSearch(Trie_node *root, string name) {
    vector<int> ids = trieSearch(root, name);

    cout << "\nSOFIFA_ID  |  NAME\t\t\t\t |  POSITIONS  |   RATING   |  COUNT\n";
    cout << "---------------------------------------------------------------------------------------\n";
    for (int i = 0; i < ids.size(); i++) {
        int id = PlayerID[ids[i]], j;
        printf("%-9d      ", ids[i]); 
        printf("%-37s", playersList[id].name.c_str());
        for (j = 0; j < playersList[id].pos.size(); j++) printf("%-s ", playersList[id].pos[j].c_str());
        for (; j < 3; j++) cout << "   ";
        float avg = playersList[id].sum/playersList[id].count;
        if(!isnan(avg)) printf("\t   %.5f\t", avg);
        else printf("\t      -\t\t"); 
        printf(" %-6d\n", playersList[id].count); 
    }
}

// funcao para retornar lista de ids alcancaveis a partir de string
vector<int> trieSearch(Trie_node *root, string query) {
    vector<int> ids;
    if (!root) return ids;
    // navega ate terminar string
    int i = 0;
    while(i < query.size()){
        if(root){
            if (query[i] > root->letter) {root = root->right;}
            else if (query[i] < root->letter) {root = root->left;}
            else if (i < query.size()) { root = root->middle; i++;}
            else i++;
        }
        else {
           cout << "Nenhum resultado compativel. :(";
           return ids;
        }   
    }
    // explora resto da arvore
    if(root->sofifa_id != -1) ids.push_back(root->sofifa_id);
    traverse(root, &ids);
    return ids;
}

// funcao para explorar toda arvore a partir de raiz
void traverse(Trie_node *root, vector<int> *fifa_ids) {
    if(root){
        if(root->sofifa_id != -1) (*fifa_ids).push_back(root->sofifa_id);
        traverse(root->left, fifa_ids);
        traverse(root->right, fifa_ids);
        traverse(root->middle, fifa_ids);
    }
}

// funcao para query de busca a partir de id de jogador
void userRatingsSearch(int userId){
    userId = UserID[userId];
    // sort de ratings do jogador ()
    
    quicksortRating(ratingsByUser[userId], 0, ratingsByUser[userId].size()-1);

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

// funcao para query de busca de N jogadores a partir de posicao
void topPositionSearch(int N, string pos){  
    vector<pair<int, double>> ids;
    
    pos.push_back('\0');
    int key = hornerHash(pos, 17);

    for(int i = 0;i < Pos_players[key].size(); i++){
        if(!strcmp(Pos_players[key][i].first.c_str(), pos.c_str())){
            int player_fifaid = Pos_players[key][i].second;
            double id_rating = playersList[PlayerID[player_fifaid]].sum/playersList[PlayerID[player_fifaid]].count;
            ids.push_back({player_fifaid, id_rating});
        }
    }


    //sort ids
    if(!ids.size()){
        cout << "invalid position!";
        return;
    }

    
    quicksortRating(ids, 0, ids.size()-1);

    cout << "\nSOFIFA_ID  |  NAME\t\t\t\t |  POSITIONS  |   RATING   |  COUNT\n";
    cout << "---------------------------------------------------------------------------------------\n";

    for (int i = 0; i < N && i < ids.size(); i++) {
        printf("%6d        ", ids[i].first); 
        int id = PlayerID[ids[i].first];
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

// funcao para query de busca  a partir de lista de tags
void tagPlayersSearch(vector<string> tags) {
    vector<int> intersect = createTagList(tags[0]);

    for(int i = 1; i < tags.size(); i++) {
        intersect = intersection(intersect, createTagList(tags[i]));
    }

    cout << "\nSOFIFA_ID  |  NAME\t\t\t\t |  POSITIONS  |   RATING   |  COUNT\n";
    cout << "---------------------------------------------------------------------------------------\n";
    for (int i = 0; i < intersect.size(); i++) {
        printf("%6d        ", intersect[i]); 
        int id = PlayerID[intersect[i]];
        printf("%-36s ", playersList[id].name.c_str()); 
        int j;
        for (j = 0; j < playersList[id].pos.size(); j++) {
            printf("%-s ", playersList[id].pos[j].c_str());
        }
        for (; j < 3; j++) cout << "   ";
        float avg = playersList[id].sum/playersList[id].count;
        if(!isnan(avg)) printf("\t   %.5f\t", avg);
        else printf("\t      -\t\t"); 
        printf(" %-6d\n", playersList[id].count); 
    }
}

vector<int> intersection(vector<int> va, vector<int> vb) {
    vector<int> inter;
    sort(va.begin(), va.end());
    sort(vb.begin(), vb.end());
    set_intersection(va.begin(), va.end(), vb.begin(), vb.end(), back_inserter(inter));
    return inter;
}

// 
vector<int> createTagList(string tag){
    vector<int> tagsPlayers;
    tag.push_back('\0');
    int key = hornerHash(tag, 1000);

    for(int i = 0; i < Tag_players[key].size(); i++){
        int playerId = PlayerID[Tag_players[key][i].second];

        if(Tag_players[key][i].first == tag /*&& playersList[playerId].count > 0*/) {
            tagsPlayers.push_back(playersList[playerId].id);
        }
    }
    
    vector<int>::iterator ip;
    ip = unique(tagsPlayers.begin(), tagsPlayers.begin() + tagsPlayers.size());
    tagsPlayers.resize(distance(tagsPlayers.begin(), ip));
    quicksort(tagsPlayers, 0, tagsPlayers.size()-1);

    return tagsPlayers;
}

// funcao para preparar lista de tags
vector<string> prepareTags(string tags) {
    vector<string> tagsVector;
    string aux;

    for (int i = 0; i < tags.size(); i++) {
        if (tags[i] == '\'') {
            if (aux.size()) {
                tagsVector.push_back(aux);
                aux.clear();
                i++;
            }
        }
        else {
            aux.push_back(tags[i]);
        }
    }
    return tagsVector;
}

// funcao de sort
void quicksort(vector<int> &v, int left, int right) {
    if(left < right) {
        int pivot = lomuto(v, left, right);
        quicksort(v, left, pivot - 1);
        quicksort(v, pivot, right);
    }
}

int lomuto(vector<int> &v, int left, int right)
{
    int pivot = v[right];    // pivot
    int i = (left - 1);  // Index of smaller element
 
    for (int j = left; j <= right - 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (v[j] <= pivot)
        {
            i++;    // increment index of smaller element
            swap(v[i], v[j]);
        }
    }
    swap(v[i + 1], v[right]);
    return (i + 1);
}

// funcao de sort
void quicksortRating(vector<pair<int,double>> &v, int left, int right) {
    if(left < right) {
        double pivot = lomutoRating(v, left, right);
        quicksortRating(v, left, pivot - 1);
        quicksortRating(v, pivot + 1, right);
    }
}

int lomutoRating(vector<pair<int,double>> &v, int left, int right)
{
    double pivot = v[right].second;    // pivot
    int i = (left - 1);  // Index of smaller element
 
    for (int j = left; j <= right - 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (v[j].second > pivot)
        {
            i++;    // increment index of smaller element
            swap(v[i], v[j]);
        }
    }
    swap(v[i + 1], v[right]);
    return (i + 1);
}