#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

void readPlayers() {

    ifstream csv("teste\\players_21.csv");

    string line;
    long pos, id;
    char aux;

    if (csv.is_open()) {
        cout << "Sucesso ao abrir players.csv\n";

        ofstream list("teste\\list_player_names.csv");
        ofstream trie_tree("teste\\trie_tree_players.csv");
        ofstream hash_table("teste\\hash_table_players.csv");

        csv >> line;

        while (csv >> line) {
            if (!line.compare("\n")) {

            }
        }

        list.close();
        trie_tree.close();
        hash_table.close();
        csv.close();
    }
    else {
        cout << "Erro ao abrir players.csv\n";
    }
}