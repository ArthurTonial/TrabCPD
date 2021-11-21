#include <ifstream>
#include <sstream>

#include "files.h"
#include "trie.h"

using namespace std;

int read_players_csv() {

    // abre arquivo de leitura
    ifstream csv("dados\\players.csv");

    if (csv.is_open()) {
        cout << "Erro ao abrir o arquivo\n";
        return 1;
    }
    else {
        cout << "Arquivo aberto\n";

		// pula primeira linha
        string aux_string; 
		getline(csv, aux_string);

		// preenche dados dos jogadores
        while (csv) {
			getline(csv, aux_string, ",");
			int sofifa_id = stoi(aux_string);

			getline(csv, aux_string, ",");
			trie_insert(trie_root, aux_string, sofifa_id);

			getline(csv, aux_string, ",");
			add_positions(aux_string, sofifa_id);
        }

        csv.close();
        return 0;
    }
}

void add_positions(string positions_string, int id) {
	
    list<string> positions;

    if (positions_string[0] == '"') {
        string aux;
        for (int i = 1; i < positions_string.length(); i++) {
            char curr = positions_string[i];
            if (curr != ',' and curr != '"') {
                aux.append(curr);
            }
            else if (curr != ',' or curr != '"'){
                positions.push_back(aux);
                aux.clear();
            }
        }
    }
    else {
        positions.push_back(positions_string);
    }

    // map[id] = positions;
}

int read_rating_csv() {

    // abre arquivo de leitura
    ifstream csv("dados\\rating.csv");

    if (csv.is_open()) {
        cout << "Erro ao abrir o arquivo\n";
        return 1;
    }
    else {
        cout << "Arquivo aberto\n";

		// pula primeira linha
        string aux_string; 
		getline(csv, aux_string);

		// preenche dados dos jogadores
        while (csv) {
			getline(csv, aux_string, ",");
			int user_id = stoi(aux_string);

            getline(csv, aux_string, ",");
			int sofifa_id = stoi(aux_string);

            getline(csv, aux_string);
			double rating = atof(aux_string);

            sum_rating[sofifa_id] += rating;
            total_rating[sofifa_id] += 1;

            pair<int,double> user_rating = (sofifa_id, rating);
            user_ratings[user_id].push_back(user_rating);
        }

        csv.close();
        return 0;
    }
}

int read_tags_csv() {



    return 0;
}
