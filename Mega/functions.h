// -------------------------// Bibliotecas //------------------------- //
#define BUFFER_MAX 400
#define NAME_MAX 200


// --------------------------// Estruturas //------------------------- //
// -> Estruturas principais
// Arvores Trie
typedef struct{
    long file_position;
    char letter;

    long left_pos;
    long son_pos;
    long right_pos;
} Trie_node;

// Tabela Hash 
typedef struct{
    long file_position;
    float sum_rating;
    int total_rating;
    char genres[NAME_MAX];
} Movie_info;

// Lista indexada
typedef struct{
    int user_id;
    int movie_id;
    float rating;
} Rating_info;

// Arvore Ternaria
typedef struct Nodo LSE_Movie;
struct Nodo{
    int movie_id;
    LSE_Movie *next;
};


// -> Estruturas auxiliares
// Lista de filmes
typedef struct{
    long file_position;
    int movie_id;
    char name[NAME_MAX];
} Movie;

// Lista de usuarios
typedef struct{
    long file_position;
    int user_id;
} Rating;

// Arvore ternaria de tags
typedef struct{
    char tag[NAME_MAX];
    int movie_id;
    long left_pos;
    long middle_pos;
    long right_pos;
} Ternary_node;


// -------------------// Cabecalhos de funcoes //--------------------- //
// -> files.c
int read_csv_movies();
int read_csv_rating();
int read_csv_tag();

// -> list.c
long save_movie_list(int movie_id, char movie_name[NAME_MAX], FILE *movie_list);
LSE_Movie* insert_movie(int movie_id, LSE_Movie *movie_list);
LSE_Movie* delete_node(LSE_Movie *main_list, LSE_Movie *to_delete);
LSE_Movie* verify_list(LSE_Movie *main_list, LSE_Movie *aux_list);

// -> ternary_tree.c
void save_node(char tag[NAME_MAX], int movie_id, FILE *ternary_tree);

// -> trie_tree.c
void save_trie_tree(char name[NAME_MAX], long position, FILE *trie_tree);

// -> sort.c
Movie_info* allocate_vector_info(int vector_size);
Movie_info* fill_vector_info(Movie_info *vetor, int vector_size);
Movie_info* insertion_sort(Movie_info *top_genres, int vector_size, Movie_info movie_info);

// -> query.c
void search_by_prefix(char movie_prefix[NAME_MAX]);
void print_movies(Trie_node search_node, FILE *trie_tree, FILE *movie_list, FILE *hash_table);
void search_user_ratings(int user_id);
void top_n_genres(char genre[NAME_MAX], int top);
void search_tags_associated(char tag_list[BUFFER_MAX]);
LSE_Movie* search_movies_related(char tag[NAME_MAX], LSE_Movie *movie_list, FILE *ternary_tree, int option);
void print_found_movies(LSE_Movie *movie_list);

// -> interface.c
void remove_cursor();
void set_cursor();
void initial_screen();
void print_rectangle();
void construction_screen();
void options_query();
void print_selection(int option);
void del_selection(int option);
int select_option();
int query_screen();
