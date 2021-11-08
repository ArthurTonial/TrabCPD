// -------------------------// Bibliotecas //------------------------- //
#include <conio2.h>
#include <stdio.h>
#include <string.h>

#include "functions.h"


// ---------------------------// Funcoes //--------------------------- //
// -> SAVE_MOVIE_TRIE_TREE: dados uma trie tree, um nome de filme e sua posicao na lista de filmes,
// a funcao encontra a posicao correta e insere o filme na arvore
void save_trie_tree(char name[NAME_MAX], long position, FILE *trie_tree){
    // -> VARIAVEIS
    Trie_node search_node;
    Trie_node son_node;

    int file_size;
    int i = 0;
    int flag = 0;

    long son_position;
    long search_node_position;


    strupr(name);     // transforma todas as letras do nome para maiusculo

    // Verifica se o arquivo esta vazio
    fseek(trie_tree, 0, SEEK_END);
    file_size = ftell(trie_tree);


    // Se o arquivo ja tiver nodos, e preciso buscar a posicao correta de insercao.
    // Caso contrario, a insercao e feita diretamente
    if(file_size != 0){
        fseek(trie_tree, 0, SEEK_SET);


        while(flag == 0 && name[i] != '\0'){
            search_node_position = ftell(trie_tree);
            fread(&search_node, sizeof(Trie_node), 1, trie_tree);

            // Caso a letra buscada coincida com a letra avaliada no nodo
            if(name[i] == search_node.letter){
                i++;

                // Se o nodo possuir filhos, a busca continua
                if(search_node.son_pos != -1){
                    fseek(trie_tree, search_node.son_pos, SEEK_SET);

                // Caso contrario, o filho e atualizado e passa-se a inserir o restante do nome na arvore
                } else {
                    if(name[i] != '\0'){
                        fseek(trie_tree, 0, SEEK_END);

                        search_node.son_pos = ftell(trie_tree);

                        fseek(trie_tree, search_node_position, SEEK_SET);
                        fwrite(&search_node, sizeof(Trie_node), 1, trie_tree);

                        fseek(trie_tree, 0, SEEK_END);
                    }

                    flag = 1;
                }
            // Caso a letra buscada venha antes da letra avaliada no nodo
            } else if(name[i] < search_node.letter){
                // Se o nodo possuir algum nodo a esquerda, a busca continua
                if(search_node.left_pos != -1){
                    fseek(trie_tree, search_node.left_pos, SEEK_SET);

                // Caso contrario, o nodo e atualizado e passa-se a inserir o restante do nome na arvore
                } else {
                    if(name[i] != '\0'){
                        fseek(trie_tree, 0, SEEK_END);

                        search_node.left_pos = ftell(trie_tree);

                        fseek(trie_tree, search_node_position, SEEK_SET);
                        fwrite(&search_node, sizeof(Trie_node), 1, trie_tree);

                        fseek(trie_tree, 0, SEEK_END);
                    }

                    flag = 1;
                }
            // Caso a letra buscada venha depois da letra avaliada no nodo
            } else {
                // Se o nodo possuir algum nodo a direita, a busca continua
                if(search_node.right_pos != -1){
                    fseek(trie_tree, search_node.right_pos, SEEK_SET);
                // Caso contrario, o nodo e atualizado e passa-se a inserir o restante do nome na arvore
                } else {
                    if(name[i] != '\0'){
                        fseek(trie_tree, 0, SEEK_END);

                        search_node.right_pos = ftell(trie_tree);

                        fseek(trie_tree, search_node_position, SEEK_SET);
                        fwrite(&search_node, sizeof(Trie_node), 1, trie_tree);

                        fseek(trie_tree, 0, SEEK_END);
                    }

                    flag = 1;
                }
            }
        }

        fseek(trie_tree, 0, SEEK_END);      // vai para o final do arquivo para que as proximas letras possam ser inseridas
    }

    // Insercao do nome ou do resto dele
    while(name[i] != '\0'){
        // Seta as informacoes para cada novo nodo
        son_node.letter = name[i];
        son_position = ftell(trie_tree);

        // Verifica se e um nodo terminal ou nao
        if(i == strlen(name) - 1){
            son_node.file_position = position;
            son_node.son_pos = -1;
        } else {
            son_node.file_position = -1;
            son_node.son_pos = son_position + sizeof(Trie_node);
        }

        son_node.left_pos = -1;
        son_node.right_pos = -1;

        // Escreve o nodo no arquivo
        fwrite(&son_node, sizeof(Trie_node), 1, trie_tree);

        i++;
    }
}


