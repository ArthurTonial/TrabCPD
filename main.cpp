#include "lib.h"

using namespace std;

int main() {

    if (!readPlayers() and !readRating() and !readTags()) {

        string cmd, arg;
        do {
            cout << "\nDigite o comando desejado [ou \"encerrar programa\" para sair]:";
            cout << "\n$ ";

            getline(cin, cmd, ' ');
            getline(cin, arg);
            if (cmd != "encerrar")  query(cmd, arg);
        } while(cmd != "encerrar");
    }
    cout << "\nObrigado e volte sempre! ^^\n";

    return 0;
}