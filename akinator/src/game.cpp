#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "akinator.h"
#include "logger.h"
#include "stack.h"
#include "utils.h"
#include "my_stack.h"
#include "pop_push.h"

//=========================================================================================================

const size_t BASE_CAPACITY = 10;

//=========================================================================================================

void akinator_t::play() {
    my_stack_t* answer_stk = NEW_STACK_(sizeof(void*), BASE_CAPACITY, print_addr);
    if (answer_stk == nullptr) {
        LOG(ERROR, "Failed to create a stack\n");
        return;
    }

    printf("Lets play the game xo-xo-xo\n");
    speak("Lets play the game xo-xo-xo");

    char answer[ANSWER_MAXLEN] = "";

    do {
        ask_question(root_, answer_stk);

        printf("Continue?\n");
        speak("Wanna continue?");

        getchar();
        scanf("%" STR_ANSWER_MAXLEN "s", answer);
    } while (strnstr(answer, "yes", ANSWER_MAXLEN));

    delete_stack(answer_stk);
}

void akinator_t::ask_question(node_t* node, my_stack_t* stk) {
    assert(stk != nullptr);
    assert(node != nullptr);

    printf("%s?\n", node->data.word);
    char formatted_text[256] = "";
    snprintf(formatted_text, sizeof(formatted_text), "%s?\n", node->data.word);
    speak(formatted_text);

    char answer[ANSWER_MAXLEN] = "";
    scanf("%" STR_ANSWER_MAXLEN "s", answer);

    if (strnstr(answer, "yes", ANSWER_MAXLEN) != nullptr) {
        if (node->right == nullptr && node->left == nullptr) {
            printf("You're too predictable, my friend, you should be more original)\n");
            speak("You're too predictable, my friend, you should be more original)\n");
        }
        else {
            ask_question(node->right, stk);
        }
    }
    else if (strnstr(answer, "no", ANSWER_MAXLEN) != nullptr) {
        if (node->right == nullptr && node->left == nullptr) {
            if (stk->size != 0) {
                node_t* next_node = nullptr;
                stack_pop(stk, &next_node);
                ask_question(next_node->left, stk);
            }
            else {
                printf("Ooops, you have some problems with ur thought, rethink and try again\n");
                speak("Ooops, you have some problems with ur thought, rethink and try again\n");
                add_new_sign(node);
                printf("Congrats! Now u are allowed to think about this thing!!!\n");
                speak("Congrats! Now u are allowed to think about this thing!!!\n");
            }
        }
        else {
            ask_question(node->left, stk);
        }
    }
    else if ((strnstr(answer, "maybe", ANSWER_MAXLEN) != nullptr) ||
             (strnstr(answer, "idk", ANSWER_MAXLEN) != nullptr)) {
        printf("Unsure option choosen\n");
        if (node->right == nullptr && node->left == nullptr) {
            printf("Unknown option, sweetie\n");
            speak("Unknown option, sweetie\n");
        }
        else {
            stack_push(stk, &node);
            ask_question(node->right, stk);
        }
    }
    else {
        printf("Unknown command %s  detected %p, return\n", answer, strnstr(answer, "maybe", ANSWER_MAXLEN));
        LOG(ERROR, "Unknown command, return\n");
        return;
    }
}

//=========================================================================================================

void akinator_t::add_new_sign(node_t* node) {
    assert(node != nullptr);

    printf("write smth that distinct %s from ur thought\n", node->data.word);
    char formatted_text[256] = "";
    snprintf(formatted_text, sizeof(formatted_text),
             "write smth that distinct %s from ur thought\n", node->data.word);
    speak(formatted_text);

    char sign[WORD_MAXLEN] = "";
    scanf("%*[\n]%" STR_WORD_MAXLEN "[^\n]", sign);

    printf("cards on the table, write ur thought, babe\n");
    speak("cards on the table, write ur thought, babe\n");
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

//=========================================================================================================

void akinator_t::speak(const char* text) {
    assert(SPEAK_CMD_LEN >= strlen("espeak-ng \" \""));

    char command[WORD_MAXLEN + SPEAK_CMD_LEN] = "";
    snprintf(command, sizeof(command), "espeak-ng \"%s\"", text);
    system(command);
}
