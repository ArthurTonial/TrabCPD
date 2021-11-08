// -------------------------// Bibliotecas //------------------------- //
#include <conio2.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "functions.h"


// ----------------------------// Main //---------------------------- //
int main(){
    // --> VARIAVEIS
    FILE *arq;

    char string[NAME_MAX];
    char buffer[BUFFER_MAX];

    int number;
    int flag = 0;


    // --> INICIALIZACAO DO PROGRAMA
    // Remove o cursor da tela
    remove_cursor();

    // Inicializa os arquivos utilizados
    // Trie Tree (estrutura 1)
    //arq = fopen("dados\\testes\\trie_tree_movie.bin", "wb");
    //fclose(arq);

    // Hash Table 1 (estrutura 2)
    //arq = fopen("dados\\testes\\hash_table_movie.bin", "wb");
   //fclose(arq);

    // Lista indexada (estrutura 3)
    //arq = fopen("dados\\testes\\list_user.bin", "wb");
    //fclose(arq);

    // Arvore ternaria de tags
    //arq = fopen("dados\\testes\\ternary_tree.bin", "wb");
    //fclose(arq);

    // Lista de filmes (auxiliar)
    //arq = fopen("dados\\testes\\list_movie_names.bin", "wb");
    //fclose(arq);

    // Lista de filmes (auxiliar)
    //arq = fopen("dados\\testes\\list_user_index.bin", "wb");
    //fclose(arq);


    // --> INTERFACE INICIAL
    initial_screen();


    // --> CONSTRUCAO DAS ESTRUTURAS
    // Interface da construcao das estruturas
    construction_screen();


    //flag = read_csv_movies();       // construcao da trie tree (estrutura 1 - finalizada)
                                    // adicao da lista de generos na hash table 1 (estrutura 2)
                                    // construcao de uma lista de filmes (auxiliar - finalizada)

    // Mensagem caso a funcao seja executada corretamente
    if(flag == 0){
        textcolor(LIGHTGRAY);
        gotoxy(55, 18);
        printf("Trie Tree construida com sucesso");
        textcolor(WHITE);
    }


    //flag = read_csv_rating();       // construcao da lista (estrutura 3 - finalizada)
                                    // adicao das notas e contagens na hash table 1 (estrutura 2 - finalizada)
                                    // construcao de uma lista de usuarios (auxiliar)

    //Mensagem caso a funcao seja executada corretamente
    if(flag == 0){
        textcolor(LIGHTGRAY);
        gotoxy(55, 25);
        printf("Lista construida com sucesso");
        gotoxy(55, 27);
        printf("Hash Table construida com sucesso");
        textcolor(WHITE);
    }


    //flag = read_csv_tag();          // contrucao de uma arvore ternária

    //Mensagem caso a funcao seja executada corretamente
    if(flag == 0){
        textcolor(LIGHTGRAY);
        gotoxy(55, 34);
        printf("Arvore ternaria construida com sucesso");
        textcolor(WHITE);
    }

    // So para ilustrar
    gotoxy(55, 16);
    printf("Arquivo 'movie.csv' aberto com sucesso");

    gotoxy(55, 23);
    printf("Arquivo 'rating.csv' aberto com sucesso");

    gotoxy(55, 32);
    printf("Arquivo 'tag.csv' aberto com sucesso");

    gotoxy(84, 39);
    printf("Pressione qualquer tecla para continuar...");
    getchar();


    // --> CONSULTAS
    flag = 0;

    do{
        // Seta a interface do menu
        number = query_screen();
        clrscr();
        set_cursor();

        switch(number){
            // Consulta por prefixo
            case 1:
                printf("Insira o prefixo: ");
                fflush(stdin);
                gets(string);

                printf("\n\n");
                remove_cursor();
                search_by_prefix(string);
                break;

            // Consulta de avaliacoes de um usuario
            case 2:
                printf("Insira o id do usuario: ");
                scanf("%d", &number);

                printf("\n\n");
                remove_cursor();
                search_user_ratings(number);
                break;

            // Colsulta top <n> <genre>
            case 3:
                printf("Insira o genero buscado: ");
                fflush(stdin);
                gets(string);
                printf("\nInsira a quantidade de elementos que serao mostrados: ");
                scanf("%d", &number);

                printf("\n\n");
                remove_cursor();
                top_n_genres(string, number);
                break;

            // Consulta de filmes com as n tags
            case 4:
                printf("Insira as tags desejadas: ");
                fflush(stdin);
                gets(buffer);

                printf("\n\n");
                remove_cursor();
                search_tags_associated(buffer);
                break;
            case 5:
                flag = 1;
                break;
        }
    } while(flag == 0);


    // Mensagem final
    textcolor(YELLOW);
    printf("MUITO OBRIGADA\n\n\n");
    textcolor(WHITE);

    return 0;
}
