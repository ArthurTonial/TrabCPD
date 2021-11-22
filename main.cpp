#include "lib.h"

using namespace std;

int main() {

    if (!readPlayers() and !readRating() and !readTags()) {

        string cmd, arg;
        do {
            cout << "\nDigite o comando desejado [ou 0 para sair]:";
            cout << "\n$ ";
            
            getline(cin, cmd, ' ');
            getline(cin, arg);

            query(cmd, arg);

        } while(cmd.compare("0"));
    }
    cout << "\nObrigado e volte sempre! ^^\n";

    return 0;
}