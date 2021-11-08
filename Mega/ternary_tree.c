// -------------------------// Bibliotecas //------------------------- //
#include <stdio.h>
#include <string.h>

#include "functions.h"


// ---------------------------// Funcoes //--------------------------- //
// -> SAVE_NODE: dada uma tag, um movie id associado e uma arvore ternaria, salva um novo nodo na arvore
void save_node(char tag[NAME_MAX], int movie_id, FILE *ternary_tree){
    // -> VARIAVEIS
    Ternary_node search_node;
    Ternary_node son_node;

    long son_position;
    long search_node_position;

    int file_size;
    int flag = 0;


    strlwr(tag);
    
    // Verifica se o arquivo esta vazio
    fseek(ternary_tree, 0, SEEK_END);
    file_size = ftell(ternary_tree);


    // Se o arquivo ja tiver nodos, e preciso buscar a posicao correta de insercao.
    // Caso contrario, a insercao e feita diretamente
    if(file_size != 0){
        fseek(ternary_tree, 0, SEEK_SET);

        while(flag == 0){
            search_node_position = ftell(ternary_tree);
            fread(&search_node, sizeof(Ternary_node), 1, ternary_tree);

            // Caso a palavra buscada coincida com a palavra avaliada no nodo
            if(strcmp(search_node.tag, tag) == 0){
                // Se o nodo possuir filhos, a busca continua
                if(search_node.middle_pos != -1){
                    fseek(ternary_tree, search_node.middle_pos, SEEK_SET);

                // Caso contrario, o filho e atualizado e passa-se a inserir o restante do nome na arvore
                } else {
                    fseek(ternary_tree, 0, SEEK_END);
                    search_node.middle_pos = ftell(ternary_tree);

                    fseek(ternary_tree, search_node_position, SEEK_SET);
                    fwrite(&search_node, sizeof(Ternary_node), 1, ternary_tree);

                    flag = 1;
                }
            // Caso a palavra buscada venha antes da palavra avaliada no nodo
            } else if(strcmp(search_node.tag, tag) > 0){
                // Se o nodo possuir algum nodo a esquerda, a busca continua
                if(search_node.left_pos != -1){
                    fseek(ternary_tree, search_node.left_pos, SEEK_SET);

                // Caso contrario, o nodo e atualizado e passa-se a inserir o restante do nome na arvore
                } else {
                    fseek(ternary_tree, 0, SEEK_END);
                    search_node.left_pos = ftell(ternary_tree);

                    fseek(ternary_tree, search_node_position, SEEK_SET);
                    fwrite(&search_node, sizeof(Ternary_node), 1, ternary_tree);

                    flag = 1;
                }
            // Caso a palavra buscada venha depois da palavra avaliada no nodo
            } else {
                // Se o nodo possuir algum nodo a direita, a busca continua
                if(search_node.right_pos != -1){
                    fseek(ternary_tree, search_node.right_pos, SEEK_SET);
                // Caso contrario, o nodo e atualizado e passa-se a inserir o restante do nome na arvore
                } else {
                    fseek(ternary_tree, 0, SEEK_END);
                    search_node.right_pos = ftell(ternary_tree);

                    fseek(ternary_tree, search_node_position, SEEK_SET);
                    fwrite(&search_node, sizeof(Ternary_node), 1, ternary_tree);

                    flag = 1;
                }
            }
        }

        fseek(ternary_tree, 0, SEEK_END);      // vai para o final do arquivo para que a palavra possa ser inserida
    }

    // Seta as informacoes para cada novo nodo
    strcpy(son_node.tag, tag);

    son_node.movie_id = movie_id;
    son_node.left_pos = -1;
    son_node.middle_pos = -1;
    son_node.right_pos = -1;

    // Insere o nodo no arquivo
    fwrite(&son_node, sizeof(Ternary_node), 1, ternary_tree);
}
