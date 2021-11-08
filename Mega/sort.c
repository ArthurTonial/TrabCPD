// -------------------------// Bibliotecas //------------------------- //
#include <conio2.h>
#include <stdio.h>
#include <string.h>

#include "functions.h"


// ---------------------------// Funcoes //--------------------------- //
// -> ALLOCATE_VECTOR_INFO: dado um tamanho, aloca um vetor do tamanho desejado
Movie_info* allocate_vector_info(int vector_size){
    // VARIAVEIS
    int *vector = NULL;

    vector = (Movie_info*)malloc(vector_size * sizeof(Movie_info));

    return vector;
}


// -> FILL_VECTOR_INFO: dado um vetor e seu tamanho, o preenche com dados de uma estrutura invalida
Movie_info* fill_vector_info(Movie_info *vector, int vector_size){
    int i;

    for(i = 0; i < vector_size; i++){
        strcpy(vector[i].genres, " ");
        vector[i].sum_rating = -1;
        vector[i].total_rating = -1;
    }

    return vector;
}


// -> INSERTION_SORT: Dado um vetor de movie_info e um elemento da estrutura, a insere, se necessario, mantendo o ordenamento descrecente 
Movie_info* insertion_sort(Movie_info *top_genres, int vector_size, Movie_info movie_info){
    // -> VARIAVEIS
    Movie_info aux;
    
    float media_to_insert, actual_media;

    int i = 0;



    // Se o primeiro elemento possuir um dado invalido, sabe-se que ainda pode-se simplesmente inserir o elemento
    if(top_genres[0].total_rating < 0){
        // Procura-se o primeiro elemento valido
        while(top_genres[i].total_rating < 0 && i < vector_size){
            i++;
        }

        // Se o vetor estiver vazio, apenas insere o elemento na ultima posicao
        if(i >= vector_size){
            i--;
            strcpy(top_genres[i].genres, movie_info.genres);
            top_genres[i].file_position = movie_info.file_position;
            top_genres[i].sum_rating = movie_info.sum_rating;
            top_genres[i].total_rating = movie_info.total_rating;
        
        // Se o vetor nao estiver vazio, procura-se o local adequado respeitando o ordenamento
        } else {
            media_to_insert = movie_info.sum_rating / movie_info.total_rating;
            actual_media = top_genres[i].sum_rating / top_genres[i].total_rating;

            while(media_to_insert <= actual_media && i < vector_size){
                strcpy(top_genres[i- 1].genres, top_genres[i].genres);
                top_genres[i - 1].file_position = top_genres[i].file_position;
                top_genres[i - 1].sum_rating = top_genres[i].sum_rating;
                top_genres[i - 1].total_rating = top_genres[i].total_rating;

                i++;
                actual_media = top_genres[i].sum_rating / top_genres[i].total_rating;
            }

            strcpy(top_genres[i - 1].genres, movie_info.genres);
            top_genres[i - 1].file_position = movie_info.file_position;
            top_genres[i - 1].sum_rating = movie_info.sum_rating;
            top_genres[i - 1].total_rating = movie_info.total_rating;
        }
    
    // Caso todas as posicoes estejam preenchidas, verifica-se se esse elemento possui media maior do que algum outro para 
    // ser inserido e descartar o ultimo elemtento do vetor
    } else {
            media_to_insert = movie_info.sum_rating / movie_info.total_rating;
            actual_media = top_genres[i].sum_rating / top_genres[i].total_rating;

            while(media_to_insert <= actual_media && i < vector_size){
                i++;
                actual_media = top_genres[i].sum_rating / top_genres[i].total_rating;
            }

            while(i < vector_size){
                strcpy(aux.genres, top_genres[i].genres);
                aux.file_position = top_genres[i].file_position;
                aux.sum_rating = top_genres[i].sum_rating;
                aux.total_rating = top_genres[i].total_rating;


                strcpy(top_genres[i].genres, movie_info.genres);
                top_genres[i].file_position = movie_info.file_position;
                top_genres[i].sum_rating = movie_info.sum_rating;
                top_genres[i].total_rating = movie_info.total_rating;

                strcpy(movie_info.genres, aux.genres);
                movie_info.file_position = aux.file_position;
                movie_info.sum_rating = aux.sum_rating;
                movie_info.total_rating = aux.total_rating;

                i++;
            }
    }

    return top_genres;
}
