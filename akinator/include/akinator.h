#ifndef AKINATOR_H
#define AKINATOR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "text_lib.h"
#include "stack.h"
#include "my_stack.h"

#define ANSWER_MAXLEN 10
#define WORD_MAXLEN 100
#define SPEAK_CMD_LEN 20

#define STR_ANSWER_MAXLEN "9"
#define STR_WORD_MAXLEN "99"

typedef enum {
    NO_ERR        = 0,
    MEM_ALLOC_ERR = 1,
    TEXT_READ_ERR = 2,
} akinator_err_t;

typedef struct {
    char* word;
    size_t length;
} data_t;

#define DATA_T_PRINTF "%s"

typedef struct node {
    data_t data;
    struct node* right;
    struct node* left;
    struct node* parent;
} node_t;

class akinator_t {
public:
    akinator_err_t ctor(FILE* data_file);
    void dtor();

    void ask_question(node_t* node, my_stack_t* stk);
    void play();

    void set_dump_ostream(FILE* ostream);
    void set_database_ostream(FILE* ostream);
    void dump();

    void print_preorder_();
    void print_inorder_();
    void print_postorder_();

    void define_word();
    void find_difference();

    int play_akinator_game();
    void update_database();

    static void update_database_handler(akinator_t* obj) {
        obj->update_database();
    }

    static void play_handler(akinator_t* obj) {
        obj->play();
    }

    static void dump_handler(akinator_t* obj) {
        obj->dump();
    }

    static void exit_game(akinator_t* obj) {
        (void) obj;
        obj->quit_game_ = true;
    }

    static void define_word_handler(akinator_t* obj) {
        obj->define_word();
    }

    static void find_difference_handler(akinator_t* obj) {
        obj->find_difference();
    }

    static void meow(akinator_t* obj) {
        (void) obj;

        printf("meow\n");

        const char* meow_sound_path = "../data/meow.wav";
        char command[WORD_MAXLEN];
        snprintf(command, sizeof(command), "afplay \"%s\"", meow_sound_path);
        system(command);
    }

    typedef struct {
        SDL_Rect rect;
        const char* label;
        void (*action)(akinator_t*);;
    } button_t;

    void speak(const char* text);
private:
    bool load_texture(SDL_Renderer* renderer, SDL_Texture** texture);
    void print_node_r(FILE* ostream, node_t* node, size_t tab_cnt);
    node_t* find_word_node(node_t* node, data_t* data);

    void print_preorder(node_t* node);
    void print_inorder(node_t* node);
    void print_postorder(node_t* node);

    void printf_tree_dot_file(FILE* tree_file, node_t* node);
    void print_nodes(FILE* tree_file, node_t* node, size_t rank);
    void print_links(FILE* tree_file, node_t* node);

    void tree_init(text_t* text);

    node_t* new_initial_node_r(text_t* text, node_t* parent, size_t* index);
    node_t* new_node(char* string, size_t length, node_t* parent, node_t* right, node_t* left);
    void delete_subtree_r(node_t* node);

    void add_new_sign(node_t* node);
    void render_footer(SDL_Renderer* renderer, TTF_Font* font);
    void render_button(SDL_Renderer* renderer, const button_t* button, TTF_Font* font, SDL_Color color);

private:
    node_t* root_{nullptr};

    const button_t BUTTONS_[7]{{(SDL_Rect){100, 100, 120, 50}, "Play",            play_handler},
                               {(SDL_Rect){100, 180, 120, 50}, "Definition",      define_word_handler},
                               {(SDL_Rect){250, 100, 120, 50}, "Compare",         find_difference_handler},
                               {(SDL_Rect){250, 180, 200, 50}, "Update database", update_database_handler},
                               {(SDL_Rect){100, 280, 120, 40}, "Dump",            dump_handler},
                               {(SDL_Rect){100, 380, 120, 40}, "Return",          exit_game},
                               {(SDL_Rect){250, 280, 120, 40}, "Meow",            meow}};
    const size_t buttons_amount{sizeof(BUTTONS_) / sizeof(BUTTONS_[0])};
    bool quit_game_{false};
    bool image_visible_{false};
};

#endif /* AKINATOR_H */
