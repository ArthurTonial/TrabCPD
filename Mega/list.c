// -------------------------// Bibliotecas //------------------------- //
#include <stdio.h>
#include <string.h>

#include "functions.h"


// ---------------------------// Funcoes //--------------------------- //
// -> SAVE_MOVIE_LIST: dados um filme, seu id e a lista de filmes, salva o novo filme na lista
long save_movie_list(int movie_id, char movie_name[NAME_MAX], FILE *movie_list){
    // -> VARIAVEIS
    Movie new_movie;


    // Seta as informacoes do novo elemento
    new_movie.movie_id = movie_id;
    strcpy(new_movie.name, movie_name);
    new_movie.file_position = ftell(movie_list);

    // Escreve o elemento no arquivo
    fwrite(&new_movie, sizeof(Movie), 1, movie_list);


    return new_movie.file_position;     // retorna a posicao em que o filme foi salvo no arquivo
}


// -> INSERT_MOVIE: Dada uma lista de filmes e um movie id, a funcao insere o filme na lista 
LSE_Movie* insert_movie(int movie_id, LSE_Movie *movie_list){
    // -> VARIAVEIS
    LSE_Movie *new;
    LSE_Movie *aux;
    int count = 0;
    int i;


    // Aloca memoria para o novo nodo
    new = (LSE_Movie*)malloc(sizeof(LSE_Movie));
    new->movie_id = movie_id;
    new->next = NULL;

    // Caso a lista esteja vazia, insere diretamente
    if(movie_list == NULL){
        movie_list = new;
    
    // Se a lista ja tiver elementos, insere no final 
    } else {
        aux = movie_list;

        while(aux->next != NULL){
            aux = aux->next;
        }

        aux->next = new;
    }

    return movie_list;
}


// -> DELETE_NODE: dada uma lista de filmes e um nodo a ser deletado, deleta o nodo da lista
LSE_Movie* delete_node(LSE_Movie *main_list, LSE_Movie *to_delete){
    // -> VARIAVEIS
    LSE_Movie *aux;


    // Se o nodo a ser deletado for o primeiro, o deleta diretamente
    if(main_list == to_delete){
        main_list = main_list->next;
        free(to_delete);
    
    // Caso nao seja o primeiro, e feita uma busca para encontra-lo
    } else {
        aux = main_list;
        while(aux->next != to_delete){
            aux = aux->next;
        }

        aux->next = to_delete->next;
        free(to_delete);
    }

    return main_list;
}


// -> VERIFY_LIST: dada duas listas, verifica se os filmes da lista auxiliar encontram-se na lista principal
// Se nao estiverem, deleta os filmes da lista principal
LSE_Movie* verify_list(LSE_Movie *main_list, LSE_Movie *aux_list){
    // -> VARIAVEIS
    LSE_Movie *aux_pointer_1;
    LSE_Movie *aux_pointer_2;
    LSE_Movie *aux_pointer_3;

    int flag = 0;


    aux_pointer_1 = main_list;

    // A cada nodo da lista principal, percorre a lista auxiliar procurando o filme.
    while(aux_pointer_1 != NULL){
        aux_pointer_2 = aux_list;

        while(aux_pointer_2 != NULL && flag == 0){
            if(aux_pointer_1->movie_id == aux_pointer_2->movie_id){
                flag = 1;
            } else {
                aux_pointer_2 = aux_pointer_2->next;
            }
        }

        // Se o filme nao e encontrado, ele e deletado
        if(flag == 0){
            aux_pointer_3 = aux_pointer_1;
            aux_pointer_1 = aux_pointer_1->next;
            main_list = delete_node(main_list, aux_pointer_3);
        } else {
            flag = 0;
            aux_pointer_1 = aux_pointer_1->next;
        }
    }

    return main_list;
}


