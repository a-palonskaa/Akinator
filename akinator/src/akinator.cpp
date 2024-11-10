#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "akinator.h"
#include "logger.h"

//======================================================================================================

akinator_err_t akinator_t::ctor(FILE* data_file) {
    assert(data_file != nullptr);

    text_t text = {};
    if (text_ctor(&text, data_file) != TEXT_NO_ERRORS) {
        LOG(ERROR, "Failed to read text\n");
        return TEXT_READ_ERR;
    }

    tree_init(&text);
    text_dtor(&text);
    return NO_ERR;
}

void akinator_t::tree_init(text_t* text) {
    assert(text != nullptr);

    size_t current_index = 0;
    for (size_t i = 0; i < text->symbols_amount; i++) {
        if (text->symbols[i] == '{') {
            current_index = i;
            break;
        }
    }
    root_ = new_initial_node(text, nullptr, &current_index);
}

node_t* akinator_t::new_initial_node(text_t* text, node_t* parent, size_t* index) {
    node_t* current_node = (node_t*) calloc(sizeof(node_t), sizeof(char));
    if (current_node == nullptr) {
        LOG(ERROR, "Memory allocation error\n" STRERROR(errno));
        return nullptr;
    }

    unsigned char word[WORD_MAXLEN] = "";
    int symbols_read = sscanf((char*) &text->symbols[*index], "%*[^\"]%*[\"]%99[^\"]", word); //ХУЙНЯ -  99
    if (symbols_read <= 0) {
        LOG(ERROR, "Reading error\n");
        return nullptr;
    }
    *index += (size_t) symbols_read;

    current_node->data.length = strnlen((char*) word, WORD_MAXLEN);

    current_node->data.word = (char*) calloc(current_node->data.length, sizeof(char));
    if (current_node->data.word == nullptr) {
        LOG(ERROR, "Failed to allocate memory\n" STRERROR(errno));
        return nullptr;
    }
    strncpy(current_node->data.word, (char*) word, current_node->data.length);

    current_node->parent = parent;

    size_t i = *index;
    for (; i < text->symbols_amount; i++) {
        if (text->symbols[i] == '}') {
            current_node->left = nullptr;
            current_node->right = nullptr;

            *index = ++i;
            return current_node;
        }
        if (text->symbols[i] == '{') {
            *index = i++;
            current_node->left = new_initial_node(text, current_node, index);
            break;
        }
    }

    for (i = *index; i < text->symbols_amount; i++) {
        if (text->symbols[i] == '{') {
            *index = i++;
            current_node->right = new_initial_node(text, current_node, index);
            break;
        }
    }
    return current_node;
}

node_t* akinator_t::new_node(char* string, size_t length, node_t* parent, node_t* right, node_t* left) {
    node_t* new_node = (node_t*) calloc(sizeof(node_t), sizeof(char));
    if (new_node == nullptr) {
        return nullptr;
    }

    new_node->data.length = length;
    new_node->data.word = (char*) calloc(length, sizeof(char));
    if (new_node->data.word == nullptr) {
        return nullptr;
    }
    strncpy(new_node->data.word, string, length);

    new_node->parent = parent;
    new_node->right = right;
    new_node->left = left;
    return new_node;
}

void akinator_t::dtor() {
    delete_node(root_);
}
//FIXME - delete_subtree_r
void akinator_t::delete_node(node_t* node) {
    if (node == nullptr) {
        return;
    }

    if (node->left != nullptr) {
        delete_node(node->left);
    }
    if (node->right != nullptr) {
        delete_node(node->right);
    }

    free(node);
    node = nullptr;
}

//======================================================================================================
