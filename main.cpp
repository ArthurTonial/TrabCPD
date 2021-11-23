#include "lib.h"

using namespace std;

int main() {

    auto start = chrono::high_resolution_clock::now();
    bool failed = readPlayers() or readRating() or readTags();
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
    cout << "\nTempo de processamento:" << duration.count() << "s\n";

    // abre arquivos
    if (!failed) {

        // interacoes de buscas
        string cmd, arg;
        do {
            cout << "\nDigite o comando desejado [ou \"encerrar programa\" para sair]:";
            cout << "\n$ ";

            getline(cin, cmd, ' ');
            getline(cin, arg);
            if (cmd != "encerrar")  query(cmd, arg);
        } while(cmd != "encerrar");
    }

    // encerra programa
    cout << "\nObrigado e volte sempre! ^^\n";
    return 0;
}