#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <cstdlib>
#include "akinator.h"
#include "logger.h"

const size_t MAX_NAME_LEN = 40;
const size_t COMMAND_SIZE = 100;

const char* FILENAME = "tree";

static FILE** get_dump_ostream() {
    static FILE* file = nullptr;
    return &file;
}

static FILE** get_database_ostream() {
    static FILE* file = nullptr;
    return &file;
}

void akinator_t::set_dump_ostream(FILE* ostream) {
    *get_dump_ostream() = ostream;
}

void akinator_t::set_database_ostream(FILE* ostream) {
    *get_database_ostream() = ostream;
}

//=========================================================================================

void akinator_t::print_preorder_() {
    print_preorder(root_);
}

void akinator_t::print_inorder_() {
    print_inorder(root_);
}

void akinator_t::print_postorder_() {
    print_postorder(root_);
}

//=========================================================================================

void akinator_t::print_preorder(node_t* node) {
    if (node == nullptr) {
        return;
    }

    printf("(");

    printf(DATA_T_PRINTF " ", node->data.word);

    if (node->left != nullptr) {
        print_preorder(node->left);
    }
    if (node->right != nullptr) {
        print_preorder(node->right);
    }

    printf(")");
}

void akinator_t::print_inorder(node_t* node) {
    if (node == nullptr) {
        return;
    }

    printf("(");

    if (node->left != nullptr) {
        print_preorder(node->left);
    }

    printf(DATA_T_PRINTF " ", node->data.word);

    if (node->right != nullptr) {
        print_preorder(node->right);
    }

    printf(")");
}

void akinator_t::print_postorder(node_t* node) {
    if (node == nullptr) {
        return;
    }

    printf("(");

    if (node->left != nullptr) {
        print_preorder(node->left);
    }

    if (node->right != nullptr) {
        print_preorder(node->right);
    }

    printf(DATA_T_PRINTF " ", node->data.word);

    printf(")");
}

//=========================================================================================

void akinator_t::dump() {
    FILE* ostream = *get_dump_ostream();

    fprintf(ostream, "<pre>");
    static size_t image_cnt = 0;

    char tree_filename[MAX_NAME_LEN] = {};
    char image_filename[MAX_NAME_LEN] = {};

    snprintf(tree_filename, MAX_NAME_LEN, "../logs/images/%s%zu.dot", FILENAME, image_cnt);
    snprintf(image_filename, MAX_NAME_LEN, "../logs/images/%s%zu.png", FILENAME, image_cnt);

    FILE* tree_file = fopen(tree_filename, "wb");
    if (tree_file == nullptr) {
        LOG(ERROR, "Failed to open an output file\n");
        return;
    }

    printf_tree_dot_file(tree_file, root_);

    if (fclose(tree_file) == EOF) {
        LOG(ERROR, "Failed to close a %s file\n" STRERROR(errno), tree_filename);
        return;
    }

    char command[COMMAND_SIZE] = "";

    snprintf(command, sizeof(command), "dot -Tpng %s -o %s", tree_filename, image_filename);

    if (system(command) != 0) {
        LOG(ERROR, "Failed to create an image\n");
        return;
    }

    fprintf(ostream, "\n<img src = \"../%s\" width = 50%%>\n", image_filename);
    image_cnt++;
}

void akinator_t::printf_tree_dot_file(FILE* tree_file, node_t* node) {
    fprintf(tree_file, "digraph G {\n\t"
                       "rankdir=TB;\n\t"
                       "bgcolor=\"#DDA0DD\";\n\t"
                       "splines=true;\n\t"
                       "node [shape=box, width=1, height=0.5, style=filled, fillcolor=\"#7FFFD4\"];\n\t");

    print_nodes(tree_file, node, 1);
    print_links(tree_file, node);

    fprintf(tree_file, "}\n");
}

void akinator_t::print_nodes(FILE* tree_file, node_t* node, size_t rank) {
    fprintf(tree_file, "node%zu [label=\"%s\"];\n\t"
                       "rank = %zu\n", (size_t) node % 10000, node->data.word, rank);

    if (node->left != nullptr) print_nodes(tree_file, node->left, rank + 1);
    if (node->right != nullptr) print_nodes(tree_file, node->right, rank + 1);
}

void akinator_t::print_links(FILE* tree_file, node_t* node) {
    if (node->left != nullptr) {
        fprintf(tree_file, "node%zu -> node%zu [weight=10,color=\"black\"];\n\t",
                           (size_t) node % 10000, (size_t) node->left % 10000);
        print_links(tree_file, node->left);
    }
    if (node->right != nullptr) {
        fprintf(tree_file, "node%zu -> node%zu [weight=10,color=\"black\"];\n\t",
                           (size_t) node % 10000, (size_t) node->right % 10000);
        print_links(tree_file, node->right);
    }
}

//=========================================================================================

void akinator_t::update_database() {
    FILE* ostream = *get_database_ostream();
    if (ostream == nullptr) {
        LOG(WARNING, "Ostream for database was not set, print to stdoutn\n");
        ostream = stdout;
    }

    print_node_r(ostream, root_, 0); //ХУЙНЯ - move pointer to the beginning of file meow
}

void akinator_t::print_node_r(FILE* ostream, node_t* node, size_t tab_cnt) {
    if (node == nullptr) {
        return;
    }

    for (size_t i = 0; i < tab_cnt; i++) {
        fprintf(ostream, "\t");
    }

    fprintf(ostream, "{\n");

    for (size_t i = 0; i < tab_cnt + 1; i++) {
        fprintf(ostream, "\t");
    }

    fprintf(ostream,"\"%s\"\n", node->data.word);

    if (node->left != nullptr) {
        print_node_r(ostream, node->left, tab_cnt + 1);
    }

    if (node->right != nullptr) {
        print_node_r(ostream, node->right, tab_cnt + 1);
    }

    for (size_t i = 0; i < tab_cnt; i++) {
        fprintf(ostream, "\t");
    }

    fprintf(ostream, "}\n");
}
