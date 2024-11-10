#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "akinator.h"
#include "logger.h"

void akinator_t::play() {
    printf("Lets play the game xo-xo-xo\n");

    char answer[ANSWER_MAXLEN] = "";
    do {
        ask_question(root_);
        printf("Continue?\n");
        getchar();
        scanf("%" STR_ANSWER_MAXLEN "s", answer);
    } while (strnstr(answer, "yes", ANSWER_MAXLEN));
}

void akinator_t::ask_question(node_t* node) {
    printf("%s?\n", node->data.word);
    char answer[ANSWER_MAXLEN] = "";
    scanf("%" STR_ANSWER_MAXLEN "s", answer);

    if (strnstr(answer, "yes", ANSWER_MAXLEN) != nullptr) {
        if (node->right == nullptr && node->left == nullptr) {
            printf("You're too predictable, my friend, you should be more original)\n");
        }
        else {
            ask_question(node->right);
        }
    }
    else if (strnstr(answer, "no", ANSWER_MAXLEN) != nullptr) {
        if (node->right == nullptr && node->left == nullptr) {
            printf("Ooops, you have some problems with ur thought, rethink and try again\n");
            add_new_sign(node);
            printf("Congrats! Now u are allowed to think about this thing!!!\n");
        }
        else {
            ask_question(node->left);
        }
    }
    else {
        LOG(ERROR, "Unknown command, return\n");
        return;
    }
}

void akinator_t::add_new_sign(node_t* node) {
    assert(node != nullptr);

    printf("write smth that distinct %s from ur thought\n", node->data.word);
    char sign[WORD_MAXLEN] = "";
    scanf("%*[\n]%" STR_WORD_MAXLEN "[^\n]", sign);

    printf("cards on the table, write ur thought, babe\n");
    char word[WORD_MAXLEN] = "";
    scanf("%*[\n]%" STR_WORD_MAXLEN "s", word);


    node->left = new_node(word, strnlen(word, WORD_MAXLEN), node, nullptr, nullptr);
    node->right = new_node(node->data.word, node->data.length, node, nullptr, nullptr);

    node->data.length = strnlen(sign, WORD_MAXLEN);

    free(node->data.word);

    node->data.word = (char*) calloc(node->data.length, sizeof(char));
    if (node->data.word == nullptr) {
        LOG(ERROR, "Memory allocation error\n");
        return;
    }

    strncpy(node->data.word, sign, node->data.length);
}
