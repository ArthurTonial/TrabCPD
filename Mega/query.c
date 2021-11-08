// -------------------------// Bibliotecas //------------------------- //
#include <conio2.h>
#include <stdio.h>
#include <string.h>

#include "functions.h"


// ---------------------------// Funcoes //--------------------------- //
// -> SEARCH_BY_PREFIX: dado um prefixo ou um nome de filme, a funcao procura em uma trie tree
// a posicao do nome corretamente formatado em uma lista de nomes de filmes.
void search_by_prefix(char movie_prefix[NAME_MAX]){
    // -> VARIAVEIS
    FILE *trie_tree, *movie_list, *hash_table;

    Trie_node search_node;

    int flag = 0;
    int i = 0;


    strupr(movie_prefix);       // transforma todas as letras do nome para maiusculo


    // Abre o arquivo e emite uma mensagem indicando se o arquivo foi aberto corretamente ou nao
    trie_tree = fopen("dados\\testes\\trie_tree_movie.bin", "rb");

    if(trie_tree == NULL){
        printf("Erro ao abrir o arquivo 'trie_tree.bin'\n");
    } else {
        // O prefixo ou nome e buscado na trie tree
        while(movie_prefix[i] != '\0' && flag == 0){
            fread(&search_node, sizeof(Trie_node), 1, trie_tree);

            // Caso a letra buscada no prefixo coincida com a letra avaliada no nodo
            if(movie_prefix[i] == search_node.letter){
                // Verifica se o nodo possui filhos para que continue-se a busca
                if(search_node.son_pos != -1){
                    fseek(trie_tree, search_node.son_pos, SEEK_SET);
                    i++;
                } else {
                    flag = 1;
                }
            // Caso a letra buscada no prefixo venha antes da letra avaliada no nodo
            } else if(movie_prefix[i] < search_node.letter){
                // Verifica se o nodo possui algum nodo a esquerda para que continue-se a busca
                if(search_node.left_pos != -1){
                    fseek(trie_tree, search_node.left_pos, SEEK_SET);
                } else {
                    flag = 1;
                }
            // Caso a letra buscada no prefixo venha depois da letra avaliada no nodo
            } else {
                // Verifica-se se o nodo possui algum nodo a direita para que continue-se a busca
                if(search_node.right_pos != -1){
                    fseek(trie_tree, search_node.right_pos, SEEK_SET);
                } else {
                    flag = 1;
                }
            }
        }

        // Caso o while pare porque o prefixo nao esta presente em nenhum filme ou o nome buscado nao
        // exista, uma mensagem de erro e mostrada
        if(flag == 1){
            printf("Nao foi encontrado nenhum filme com esse prefixo");
        // Se nao, os proximos nodos sao percorridos em busca de todos os filmes que possuam tal prefixo ou nome
        } else {
            textbackground(YELLOW);
            textcolor(BLACK);
            printf("Movie id |                                                                                                     Title |                                              Genres | Global |  Count \n");
            textbackground(BLACK);
            textcolor(WHITE);


            movie_list = fopen("dados\\testes\\list_movie_names.bin", "rb");
            hash_table = fopen("dados\\testes\\hash_table_movie.bin", "rb");

            fseek(trie_tree, search_node.son_pos, SEEK_SET);
            print_movies(search_node, trie_tree, movie_list, hash_table);

            fclose(movie_list);
            fclose(hash_table);
        }
    }

    fclose(trie_tree);

    printf("\n\nPressione qualquer tecla para continuar...");
    fflush(stdin);
    getchar();
}


// -> PRINT_MOVIES: dado um nodo de um arvore trie e os arquivos contendo a trie tree e a lista
// de filmes, a funcao procura, a partir do nodo dado, por todos os nomes existentes no ramo
void print_movies(Trie_node search_node, FILE *trie_tree, FILE *movie_list, FILE *hash_table){
    // -> VARIAVEIS
    Movie found_movie;
    Movie_info movie_info;

    float media;

    long position;


    fread(&search_node, sizeof(Trie_node), 1, trie_tree);

    // Se encontrar algum nodo terminal, procura o nome do filme no arquivo auxiliar e o imprime na tela
    if(search_node.file_position != -1){
        fseek(movie_list, search_node.file_position, SEEK_SET);
        fread(&found_movie, sizeof(Movie), 1, movie_list);

        fseek(hash_table, (found_movie.movie_id - 1) * sizeof(Movie_info), SEEK_SET);
        fread(&movie_info, sizeof(Movie_info), 1, hash_table);

        if(movie_info.total_rating > 0){
            media = movie_info.sum_rating / movie_info.total_rating;
            printf("%8d   %105s    %50s    %5.2f    %5d\n", found_movie.movie_id, found_movie.name, movie_info.genres, media, movie_info.total_rating);
        } else {
            printf("%8d   %105s    %50s     Sem avaliacoes\n", found_movie.movie_id, found_movie.name, movie_info.genres);
        }

    }

    // Continua a busca pelo nodo a esquerda
    if(search_node.left_pos != -1){
        fseek(trie_tree, search_node.left_pos, SEEK_SET);
        print_movies(search_node, trie_tree, movie_list, hash_table);
    }

    // Continua a busca pelo filho
    if(search_node.son_pos != -1){
        fseek(trie_tree, search_node.son_pos, SEEK_SET);
        print_movies(search_node, trie_tree, movie_list, hash_table);
    }

    // Continua a busca pelo nodo a direita
    if(search_node.right_pos != -1){
        fseek(trie_tree, search_node.right_pos, SEEK_SET);
        print_movies(search_node, trie_tree, movie_list, hash_table);
    }
}


// -> SEARCH_USER_RATINGS: dado um user id, a funcao procura na tabela hash, com auxilio da
// lista de usuarios auxiliar, todos os filmes avaliados por tal usuario
void search_user_ratings(int user_id){
    // -> VARIAVEIS
    FILE *list_user, *list_index, *hash_table, *list_movies;

    Rating search_element;
    Rating_info user_rating;

    Movie_info movie_info;

    Movie movie_name;

    float media;
    
    int flag = 0;


    // Abre a lista de usuarios
    list_index = fopen("dados\\testes\\list_user_index.bin", "rb");

    // Percorre a lista em busca da posicao das avaliacoes na tabela hash do usuario
    while(!feof(list_index) && flag == 0){
        fread(&search_element, sizeof(Rating), 1, list_index);

        if(search_element.user_id == user_id){
            flag = 1;
        }
    }

    // Caso encontre o usuario procurado, percorre a tabela imprimindo as avaliacoes do usuario
    if(flag == 1){
        // Os arquivos contendo as outras informacoes necessarias sao abertos
        list_user = fopen("dados\\testes\\list_user.bin", "rb");
        hash_table = fopen("dados\\testes\\hash_table_movie.bin", "rb");
        list_movies = fopen("dados\\testes\\list_movie_names.bin", "rb");

        fseek(list_user, search_element.file_position, SEEK_SET);
        fread(&user_rating, sizeof(Rating_info), 1, list_user);


        textbackground(YELLOW);
        textcolor(BLACK);
        printf("                                                                                                    Title |  User rating |  Global |  Count \n");
        textbackground(BLACK);
        textcolor(WHITE);


        // Percorre todas as avaliacoes feitas pelo usuario imprimindo as informacoes necessarias na tela
        while(user_rating.user_id == user_id){
            fseek(hash_table, user_rating.movie_id * sizeof(Movie_info), SEEK_SET);
            fread(&movie_info, sizeof(Movie_info), 1, hash_table);

            fseek(list_movies, movie_info.file_position, SEEK_SET);
            fread(&movie_name, sizeof(Movie), 1, list_movies);

            media = movie_info.sum_rating/ movie_info.total_rating;

            printf("%105s    %11.1f    %6.2f    %5d\n", movie_name.name, user_rating.rating, media, movie_info.total_rating);

            fread(&user_rating, sizeof(Rating_info), 1, list_user);
        }

        fclose(list_user);
        fclose(hash_table);
        fclose(list_movies);
    // Se o usuario nao for encontrado, uma mensagem de falta de resultados e impressa na tela
    } else {
        printf("Usuario nao encontrado");
    }

    fclose(list_index);
    
    printf("\n\nPressione qualquer tecla para continuar...");
    fflush(stdin);
    getchar();
}


// -> TOP_N_GENRES: dado um genero e um valor n, retorna os n melhores filmes daquele genero
void top_n_genres(char genre[NAME_MAX], int top){
    // -> VARIAVEIS
    FILE *hash_table, *list;

    Movie_info *top_genres;
    Movie_info movie_info;

    Movie search_name;

    char flag;
    char search_text[NAME_MAX];

    int found_one = 0;
    int position;
    int i;

    float media;


    // Abre-se o arquivo que contem a hash table
    hash_table = fopen("dados\\testes\\hash_table_movie.bin", "rb");

    // ALoca um vetor do tamanho necessario e o preenche com informacoes invalidas
    top_genres = allocate_vector_info(top);
    top_genres = fill_vector_info(top_genres, top);

    // Para cada elemento da hash table, verifica se esse filme possui o genero buscado
    while(!feof(hash_table)){
        fread(&movie_info, sizeof(Movie_info), 1, hash_table);

        strcpy(search_text, movie_info.genres);
        strupr(search_text);

        flag = strstr(search_text, strupr(genre));

        // Se o filme possui o genero buscado e pelo menos 1000 avaliacoes, insere-o no vetor caso necessario
        if(flag != NULL && movie_info.total_rating >= 1000){
            found_one = 1;
            top_genres = insertion_sort(top_genres, top, movie_info);
        }
    }

    fclose(hash_table);

    // Se nenhum filme desse genro for encontrado, imprime-se uma mensagem de falta de resultados
    if(found_one == 0){
        printf("Nenhum filme com esse genero encontrado");
    
    // Se for encontrado algum filme, imprime-se as informacoes contidas no vetor
    } else {
        list = fopen("dados\\testes\\list_movie_names.bin", "rb");

        textbackground(YELLOW);
        textcolor(BLACK);
        printf("                                                                                                    Title |                                              Genres |  Global |  Count \n");
        textbackground(BLACK);
        textcolor(WHITE);

        for(i = 0; i < top; i++){
            if(top_genres[i].total_rating > 0){
                media = top_genres[i].sum_rating / top_genres[i].total_rating;
                fseek(list, top_genres[i].file_position, SEEK_SET);
                fread(&search_name, sizeof(Movie), 1, list);

                printf("%105s    %50s    %6.2f    %5d\n", search_name.name, top_genres[i].genres, media, top_genres[i].total_rating);
            }
        }

        fclose(list);
    }

    printf("\n\nPressione qualquer tecla para continuar...");
    fflush(stdin);
    getchar();
}


// -> SEARCH_TAGS_ASSOCIATED: dada uma lista de tags, verifica os filmes que possuem todas as tags associadas
void search_tags_associated(char tag_list[BUFFER_MAX]){
    // -> VARIAVEIS
    FILE *ternary_tree;

    LSE_Movie *movie_list = NULL;

    char tag[NAME_MAX];
    int i = 1;
    int j;
    int option = 0;
    int flag = 0;


    // Abre-se o arquivo que contem a arvore ternaria
    ternary_tree = fopen("dados\\testes\\ternary_tree.bin", "rb");

    // Percorre-se a string, separando cada tag e procurando quais filmes estao associados a ela
    while(i < strlen(tag_list) && flag == 0){
        j = 0;
        while(tag_list[i] != '\'' && tag_list[i] != '\0'){
            tag[j] = tag_list[i];
            j++;
            i++;
        }

        tag[j] = '\0';

        // Ao encontrar uma tag, se for a primeira a ser emcontrada, insere os filmes associados a ela. 
        // Se nao, verifica os filmes em comum
        if(strcmp(tag, " ") != 0){
            movie_list = search_movies_related(tag, movie_list, ternary_tree, option);

            // Se a lista retornada for vazia, a busca para e retorna-se uma mensagem de falta de resultados
            if(movie_list == NULL){
                flag = 1;
            }

            option = 1;
        }

        i ++;
    }

    fclose(ternary_tree);

    // Se nenhum filme com todas as tags for encontrado, imprime-se uma mensagem de falta de resultados
    if(flag == 1){
        printf("Nenhum filme relacionado a todas as tags encontrado");
    
    // Se algum filme for encontrado, a lista de filmes associados as tags e impressa
    } else {
        print_found_movies(movie_list);
    }

    printf("\n\nPressione qualquer tecla para continuar...");
    fflush(stdin);
    getchar();
}


// -> SEARCH_MOVIES_RELATED: dada uma tag, uma lista de filmes e uma arvore ternaria, insere ou atualiza a lista de filmes associados as tags
// de acordo com a opcao fornecida
LSE_Movie* search_movies_related(char tag[NAME_MAX], LSE_Movie *movie_list, FILE *ternary_tree, int option){
    Ternary_node search_node;

    LSE_Movie *aux_list = NULL;

    int flag = 0;


    strlwr(tag);
    fseek(ternary_tree, 0, SEEK_SET);

    // Procura a tag na arvore ternaria
    do{
        fread(&search_node, sizeof(Ternary_node), 1, ternary_tree);

        // Caso encontre a tag buscada a busca para com resultado positivo
        if(strcmp(search_node.tag, tag) == 0){
            flag = 1;
        
        // Caso a tag venha depois da tag buscada, tenta-se buscar pela esquerda
        } else if(strcmp(search_node.tag, tag) > 0){
            if(search_node.left_pos != -1){
                fseek(ternary_tree, search_node.left_pos, SEEK_SET);
            
            // Se nao houver nodos a esquerda a busca e encerrada com resultado negativo
            } else {
                flag = 2;
            }
        
        // Caso a tag venha antes da tag buscada, tenta-se buscar pela direita
        } else {
            if(search_node.right_pos != -1){
                fseek(ternary_tree, search_node.right_pos, SEEK_SET);
            
            // Se nao houver nodos a direita a busca e encerrada com resultado negativo
            } else {
                flag = 2;
            }
        }
    } while(flag == 0);

    // Caso a tag seja encontrada
    if(flag == 1){
        // Se for a primeira tag da busca, apenas insere os filmes diretamente
        if(option == 0){
            movie_list = insert_movie(search_node.movie_id, movie_list);

            while(search_node.middle_pos != -1){
                fseek(ternary_tree, search_node.middle_pos, SEEK_SET);
                fread(&search_node, sizeof(Ternary_node), 1, ternary_tree);

                movie_list = insert_movie(search_node.movie_id, movie_list);
            }
        
        // Se ja houver filmes na lista, a lista e atualizada, removendo-se os filmes nao associados a todas as tags encontradas ate entao
        } else {
            aux_list = insert_movie(search_node.movie_id, aux_list);

            while(search_node.middle_pos != -1){
                fseek(ternary_tree, search_node.middle_pos, SEEK_SET);
                fread(&search_node, sizeof(Ternary_node), 1, ternary_tree);

                aux_list = insert_movie(search_node.movie_id, aux_list);
            }

            movie_list = verify_list(movie_list, aux_list);
        }
    }

    return movie_list;
}


// -> PRINT_FOUND_MOVIES: dada uma lista de filmes, a funcao percorre a lista imprimindo seus elementos
void print_found_movies(LSE_Movie *movie_list){
    // -> VARIAVEIS
    FILE *list, *hash_table;

    Movie movie;
    Movie_info movie_info;

    LSE_Movie *aux;

    float media;


    if(movie_list != NULL){
        hash_table = fopen("dados\\testes\\hash_table_movie.bin", "rb");
        list = fopen("dados\\testes\\list_movie_names.bin", "rb");

        aux = movie_list;
        
        textbackground(YELLOW);
        textcolor(BLACK);
        printf("                                                                                                    Title |                                              Genres |  Global |  Count \n");
        textbackground(BLACK);
        textcolor(WHITE);


        while(aux != NULL){
            fseek(hash_table, (aux->movie_id - 1)* sizeof(Movie_info), SEEK_SET);
            fread(&movie_info, sizeof(Movie_info), 1, hash_table);

            fseek(list, movie_info.file_position, SEEK_SET);
            fread(&movie, sizeof(Movie), 1, list);

            if(movie_info.total_rating < 0){
                media = movie_info.sum_rating / movie_info.total_rating;
                printf("%105s    %50s    %5.2f    %5d\n", movie.name, movie_info.genres, media, movie_info.total_rating);
            } else {
                printf("%105s    %50s     Sem avaliacoes\n", movie.name, movie_info.genres, media, movie_info.total_rating);
            }

            aux = aux->next;
        }
    }
}
