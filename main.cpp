#include <iostream>

#include "files.h"

using namespace std;

int main() {

    if (!read_players_csv() and !read_rating_csv() and !read_tags_csv()) {

        string cmd;

        do {
            cout << "\nDigite o comando desejado [\"0\" para finalizar]: ";
            cin >> cmd;
            read_cmd(cmd);
        } while(!cmd.compare("0"));

    }

    return 0;
}