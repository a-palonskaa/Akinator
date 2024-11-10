#include <string.h>
#include <assert.h>
#include "akinator.h"
#include "stack.h"
#include "my_stack.h"
#include "pop_push.h"

//===========================================================================================
// NOTE: utils.cpp
const size_t BASE_CAPACITY = 10;

void print_addr(void* elm, FILE* ostream) {
    assert(elm != nullptr);
    assert(ostream != nullptr);

    fprintf(ostream, "%p", *((char**)elm));
}

//===========================================================================================

void akinator_t::find_difference(data_t* data1, data_t* data2, FILE* ostream) {
    assert(data1 != nullptr);
    assert(data2 != nullptr);
    assert(ostream != nullptr);

    node_t* word1_node = find_word_node(root_, data1); // FIXME - if not found
    node_t* word2_node = find_word_node(root_, data2);

    my_stack_t* stk1 = NEW_STACK_(sizeof(void*), BASE_CAPACITY, print_addr);
    my_stack_t* stk2 = NEW_STACK_(sizeof(void*), BASE_CAPACITY, print_addr);

    stack_push(stk1, &word1_node);
    stack_push(stk2, &word2_node);

    while (word1_node != root_) {
        word1_node = word1_node->parent;
        stack_push(stk1, &word1_node);
    }

    while (word2_node != root_) {
        word2_node = word2_node->parent;
        stack_push(stk2, &word2_node);
    }

    node_t* current_node1 = root_;
    node_t* current_node2 = root_;

    stack_pop(stk1, &current_node1);
    stack_pop(stk2, &current_node2);

    while (current_node1 == current_node2) {
        stack_pop(stk1, &current_node1);
        stack_pop(stk2, &current_node2);

        if ((current_node1->left == nullptr && current_node1->right == nullptr) ||
            (current_node1->left == nullptr && current_node1->right == nullptr)) {
            break;
        }

        fprintf(ostream, "%s and %s are both ", data1->word, data2->word);

        if (current_node1 == current_node1->parent->left) {
            fprintf(ostream, "not ");
        }

        fprintf(ostream, "%s\n", current_node1->parent->data.word);
    }

    if (current_node1->parent->right == current_node1) {
        fprintf(ostream, "%s is %s, while %s is not %s\n",
                data1->word, current_node1->parent->data.word,
                data2->word, current_node1->parent->data.word);
    }
    else {
        fprintf(ostream, "%s is not %s, while %s is %s\n",
                data1->word, current_node1->parent->data.word,
                data2->word, current_node1->parent->data.word);
    }

    delete_stack(stk1);
    delete_stack(stk2);
}

void akinator_t::define_word(data_t* data, FILE* ostream) {
    assert(ostream != nullptr);

    node_t* word_node = find_word_node(root_, data); //ХУЙНЯ - check for nullptr
    fprintf(ostream, "DEFINTION: %s is ", word_node->data.word);

    bool first = true;
    while (word_node != root_) {
        if (!first) {
            fprintf(ostream, ", ");
        }

        if (word_node == word_node->parent->right) {
            fprintf(ostream, "%s", word_node->parent->data.word);
        }
        else {
            fprintf(ostream, "not %s", word_node->parent->data.word);
        }

        word_node = word_node->parent;
        first = false;
    }

    fprintf(ostream, "\n\n");
}

//===========================================================================================

node_t* akinator_t::find_word_node(node_t* node, data_t* data) {
    assert(data != nullptr);

    if (node == nullptr) {
        return nullptr;
    }

    if (node->left == nullptr && node->right == nullptr) {
        if (strnstr(node->data.word, data->word, data->length) != nullptr) {
            return node;
        }
        return nullptr;
    }

    node_t* find_left = find_word_node(node->left, data);
    if (find_left != nullptr) {
        return find_left;
    }
    return find_word_node(node->right, data);
}

//===========================================================================================
