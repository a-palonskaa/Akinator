#ifndef AKINATOR_H
#define AKINATOR_H

#include "text_lib.h"

#define ANSWER_MAXLEN 5
#define WORD_MAXLEN 100

#define STR_ANSWER_MAXLEN "4"
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

    void ask_question(node_t* node);
    void play();

    void set_dump_ostream(FILE* ostream);
    void dump();

    void print_preorder_();
    void print_inorder_();
    void print_postorder_();

    void define_word(data_t* data, FILE* ostream);
    void find_difference(data_t* data1, data_t* data2, FILE* ostream);
private:
    node_t* find_word_node(node_t* node, data_t* data);

    void print_preorder(node_t* node);
    void print_inorder(node_t* node);
    void print_postorder(node_t* node);

    void printf_tree_dot_file(FILE* tree_file, node_t* node);
    void print_nodes(FILE* tree_file, node_t* node, size_t rank);
    void print_links(FILE* tree_file, node_t* node);

    void tree_init(text_t* text);

    node_t* new_initial_node(text_t* text, node_t* parent, size_t* index);
    node_t* new_node(char* string, size_t length, node_t* parent, node_t* right, node_t* left);
    void delete_node(node_t* node);

    void add_new_sign(node_t* node);
private:
    node_t* root_{nullptr};
};

#endif /* AKINATOR_H */
