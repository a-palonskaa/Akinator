#include <string.h>
#include <assert.h>
#include "akinator.h"
#include "stack.h"
#include "my_stack.h"
#include "pop_push.h"
#include "utils.h"
#include "logger.h"

//===========================================================================================

const size_t BASE_CAPACITY = 10;

//===========================================================================================

void akinator_t::find_difference() {
    FILE* ostream = stdout;

    data_t data1 = {};
    data_t data2 = {};

    data1.word = (char*) calloc(WORD_MAXLEN, sizeof(char));
    if (data1.word == nullptr) {
        LOG(ERROR, "Failed to allocate memory for a word\n");
        return;
    }

    data2.word = (char*) calloc(WORD_MAXLEN, sizeof(char));
    if (data2.word == nullptr) {
        LOG(ERROR, "Failed to allocate memory for a word\n");
        return;
    }

    printf("Enter the first word, u wanna compare\n");
    scanf("%" STR_WORD_MAXLEN "s", data1.word);
    data1.length = strnlen(data1.word, WORD_MAXLEN);

    printf("Enter the second word, u wanna compare\n");
    scanf("%" STR_WORD_MAXLEN "s", data2.word);
    data2.length = strnlen(data2.word, WORD_MAXLEN);

    node_t* word1_node = find_word_node(root_, &data1);
    if (word1_node == nullptr) {
        fprintf(ostream, "Word %s has not been found\n", data1.word);
        return;
    }

    node_t* word2_node = find_word_node(root_, &data2);
    if (word2_node == nullptr) {
        fprintf(ostream, "Word %s has not been found\n", data1.word);
        return;
    }

    my_stack_t* stk1 = NEW_STACK_(sizeof(void*), BASE_CAPACITY, print_addr);
    if (stk1 == nullptr) {
        LOG(ERROR, "Failed to create a stack\n");
        return;
    }

    my_stack_t* stk2 = NEW_STACK_(sizeof(void*), BASE_CAPACITY, print_addr);
    if (stk2 == nullptr) {
        LOG(ERROR, "Failed to create a stack\n");
        return;
    }

    stack_push(stk1, &word1_node);
    stack_push(stk2, &word2_node);

    node_t* pushed_node = word1_node;
    while (pushed_node != root_) {
        pushed_node = pushed_node->parent;
        stack_push(stk1, &pushed_node);
    }

    pushed_node = word2_node;
    while (pushed_node != root_) {
        pushed_node = pushed_node->parent;
        stack_push(stk2, &pushed_node);
    }

    node_t* current_node1 = nullptr;
    node_t* current_node2 = nullptr;

    stack_pop(stk1, &current_node1);
    stack_pop(stk2, &current_node2);

    while (current_node1 == current_node2) {
        stack_pop(stk1, &current_node1);
        stack_pop(stk2, &current_node2);

        if (current_node1->parent == nullptr) {
            break;
        }

        if ((current_node1->left == nullptr && current_node1->right == nullptr) ||
            (current_node2->left == nullptr && current_node2->right == nullptr)) {
            break;
        }

        fprintf(ostream, "%s and %s are both ", data1.word, data2.word);

        if (current_node1 == current_node1->parent->left) {
            fprintf(ostream, "not ");
        }

        fprintf(ostream, "%s\n", current_node1->parent->data.word);
    }

    if (current_node1 == root_) {
        stack_pop(stk1, &current_node1);
        if (root_->right == current_node1) {
            fprintf(ostream, "%s is %s, while %s is not %s\n",
                              data1.word, root_->data.word,
                              data2.word, root_->data.word);
        }
        else {
             fprintf(ostream, "%s is not %s, while %s is %s\n",
                               data1.word, root_->data.word,
                               data2.word, root_->data.word);
        }
    }
    else if (current_node1->parent->right == current_node1) {
        fprintf(ostream, "%s is %s, while %s is not %s\n",
                          data1.word, current_node1->parent->data.word,
                          data2.word, current_node1->parent->data.word);
    }
    else {
        fprintf(ostream, "%s is not %s, while %s is %s\n",
                          data1.word, current_node1->parent->data.word,
                          data2.word, current_node1->parent->data.word);
    }

    delete_stack(stk1);
    delete_stack(stk2);
}

void akinator_t::define_word() {
    FILE* ostream = stdout;
    data_t data = {};

    data.word = (char*) calloc(WORD_MAXLEN, sizeof(char));
    if (data.word == nullptr) {
        LOG(ERROR, "Failed to allocate memory for a word\n");
        return;
    }

    printf("Enter the word, u wanna define\n");
    scanf("%" STR_WORD_MAXLEN "s", data.word);

    data.length = strnlen(data.word, WORD_MAXLEN);
    node_t* word_node = find_word_node(root_, &data);
    if (word_node == nullptr) {
        fprintf(ostream, "Word %s has not been found\n", data.word);
        free(data.word);
        return;
    }

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

    free(data.word);
    data.word = nullptr;
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
