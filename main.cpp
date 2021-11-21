#include "lib.h"

using namespace std;

int main() {

    if (!read_players_csv() and !read_rating_csv() and !read_tags_csv()) {

        string cmd;

        do {
            cout << "\nDigite o comando desejado [\"0\" para finalizar]: \n$ ";
            cin >> cmd;
            read_cmd(cmd);
        } while(cmd.compare("0\0"));
    }
    cout << "\nObrigado e volte sempre! ^^\n";

    return 0;
}