// -------------------------// Bibliotecas //------------------------- //
#include <conio2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"


// ---------------------------// Funcoes //--------------------------- //
// -> READ_CSV_MOVIES: le o arquivo csv de filmes e salva as informacoes contidas em uma lista
// de filmes e uma trie tree
int read_csv_movies(){
    // -> VARIAVEIS
    FILE *csv;
    FILE *list, *trie_tree, *hash_table;

    Movie_info new_movie_info;

    char buffer[BUFFER_MAX];
    char string[NAME_MAX];
    char garbage;

    long position;

    int movie_id;


    // Abre o arquivo e emite uma mensagem indicando se o arquivo foi aberto corretamente ou nao
    csv = fopen("dados\\movie.csv", "r");

    if(csv == NULL){
        gotoxy(55, 16);
        printf("Erro ao abrir o arquivo 'movie.csv'");

        return 1;
    } else {
        gotoxy(55, 16);
        printf("Arquivo 'movie.csv' aberto com sucesso");

        // Se o arquivo de leitura foi aberto corretamente, os arquivos de escrita sao abertos
        list = fopen("dados\\testes\\list_movie_names.bin", "ab");
        trie_tree = fopen("dados\\testes\\trie_tree_movie.bin", "r+b");
        hash_table = fopen("dados\\testes\\hash_table_movie.bin", "r+b");

        fgets(buffer, BUFFER_MAX, csv);     // descarta a primeira linha

        // Percorre todo o arquivo, tratando linha a linha
        while(!feof(csv)){
            fgets(buffer, BUFFER_MAX, csv);

            if(strcmp(buffer, "\n") != 0){
                // Coleta as informacoes de movie_id e nome
                movie_id = atoi(strtok(buffer, ","));
                strcpy(string, strtok(NULL, "\""));

                // Salva as informacoes obtidas nas estruturas adequadas
                position = save_movie_list(movie_id, string, list);     // lista

                save_trie_tree(string, position, trie_tree);            // trie_tree

                garbage = strtok(NULL, "\"");

                // Obtem os generos do filme
                strcpy(new_movie_info.genres, strtok(NULL, "\""));

                // Seta inicialmente outros dados da estrutura armazenada na hash table
                new_movie_info.sum_rating = -1;
                new_movie_info.total_rating = -1;
                new_movie_info.file_position = position;

                // Armazena as informacoes em uma posicao da tabela hash
                fseek(hash_table, (movie_id - 1) * sizeof(Movie_info), SEEK_SET);
                fwrite(&new_movie_info, sizeof(Movie_info), 1, hash_table);
            }
        }
        fclose(list);
        fclose(trie_tree);
        fclose(hash_table);
    }
    fclose(csv);

    return 0;
}


// -> READ_CSV_RATING: le o arquivo de csv de ratings e salva as informacoes contidas em uma lista
// e uma hash table
int read_csv_rating(){
    // -> VARIAVEIS
    FILE *csv;
    FILE *list_user, *list_user_index, *hash_table;

    Rating_info new_rating;
    Rating new_index;

    Movie_info update_movie;

    char buffer[BUFFER_MAX];
    char *garbage;

    int last_user_id = -1;


    // Abre o arquivo e emite uma mensagem indicando se o arquivo foi aberto corretamente ou nao
    csv = fopen("dados\\rating.csv", "r");

    if(csv == NULL){
        gotoxy(55, 23);
        printf("Erro ao abrir o arquivo 'rating.csv'");

        return 1;
    } else {
        gotoxy(55, 23);
        printf("Arquivo 'rating.csv' aberto com sucesso");

        // Se o arquivo de leitura foi aberto corretamente, os arquivos de escrita sao abertos
        list_user = fopen("dados\\testes\\list_user.bin", "ab");
        list_user_index = fopen("dados\\testes\\list_user_index.bin", "ab");
        hash_table = fopen("dados\\testes\\hash_table_movie.bin", "r+b");

        fgets(buffer, BUFFER_MAX, csv);     // descarta a primeira linha

        // Percorre todo o arquivo, tratando linha a linha
        while(!feof(csv)){
            fgets(buffer, BUFFER_MAX, csv);

            if(strcmp(buffer, "\n") != 0){
                // Coleta as informacoes obtidas
                new_rating.user_id = atoi(strtok(buffer, ","));
                new_rating.movie_id = atoi(strtok(NULL, ","));
                new_rating.rating = atof(strtok(NULL, ","));


                // Se o usuario encontrado for diferente do anterior, e salva sua posicao da tabela hash e em uma lista
                if(last_user_id != new_rating.user_id){
                    new_index.file_position = ftell(list_user);
                    new_index.user_id = new_rating.user_id;

                    fwrite(&new_index, sizeof(Rating), 1, list_user_index);
                }

                // Armazena ou atualiza as informacoes obtidas na tabela hash
                fwrite(&new_rating, sizeof(Rating_info), 1, list_user);
                last_user_id = new_rating.user_id;      // atualiza o id do ultimo usuario salvo

                fseek(hash_table, new_rating.movie_id * sizeof(Movie_info), SEEK_SET);
                fread(&update_movie, sizeof(Movie_info), 1, hash_table);


                if(update_movie.sum_rating > 0){
                    update_movie.sum_rating += new_rating.rating;
                    update_movie.total_rating++;
                } else {
                    update_movie.sum_rating = new_rating.rating;
                    update_movie.total_rating = 1;
                }


                fseek(hash_table, new_rating.movie_id * sizeof(Movie_info), SEEK_SET);
                fwrite(&update_movie, sizeof(Movie_info), 1, hash_table);
                fflush(hash_table);

                garbage = strtok(NULL, ",");
            }
        }

        fclose(list_user);
        fclose(list_user_index);
        fclose(hash_table);
    }
    fclose(csv);

    return 0;
}


// -> READ_CSV_TAG: le o arquivo de csv de tags e salva as informacoes contidas em uma arvore
// ternaria
int read_csv_tag(){
    // -> VARIAVEIS
    FILE *csv;
    FILE *ternary_tree;

    char buffer[BUFFER_MAX];
    char tag[NAME_MAX];
    char *garbage;

    int movie_id;
    int tag_id = 0;
    int flag;


    // Abre o arquivo e emite uma mensagem indicando se o arquivo foi aberto corretamente ou nao
    csv = fopen("dados\\tag.csv", "r");

    if(csv == NULL){
        gotoxy(55, 32);
        printf("Erro ao abrir o arquivo 'tag.csv'");

        return 1;
    } else {
        gotoxy(55, 32);
        printf("Arquivo 'tag.csv' aberto com sucesso");

        // Se o arquivo de leitura foi aberto corretamente, o arquivo de escrita e aberto
        ternary_tree = fopen("dados\\testes\\ternary_tree.bin", "r+b");

        fgets(buffer, BUFFER_MAX, csv);     // descarta a primeira linha

        // Percorre todo o arquivo, tratando linha a linha
        while(!feof(csv)){
            fgets(buffer, BUFFER_MAX, csv);

            if(strcmp(buffer, "\n") != 0){
                garbage = strtok(buffer, ",");
                movie_id = atoi(strtok(NULL, ","));
                strcpy(tag, strtok(NULL, "\""));
                garbage = strtok(NULL, ",");

                // Salva as informacoes obtidas em um nodo da arvore
                save_node(tag, movie_id, ternary_tree);
            }
        }
        fclose(ternary_tree);
    }

    fclose(csv);

    return 0;
}
