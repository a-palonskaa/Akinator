#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "akinator.h"
#include "logger.h"

int main() {
    FILE* logger_file = fopen("../logs/logger.txt", "w");
    if (logger_file == nullptr) {
        logger_file = stdout;
    }
    LoggerSetFile(logger_file);

    FILE* data_file = fopen("../data.txt", "r");
    if (data_file == nullptr) {
        LOG(ERROR, "Failed to open an input file\n");
        return EXIT_FAILURE;
    }

    FILE* ostream = fopen("../logs/trees.html", "wb");
    if (ostream == nullptr) {
        LOG(ERROR, "Failed to open a dump file\n");
        return EXIT_FAILURE;
    }

    akinator_t akinator{};
    akinator.set_dump_ostream(ostream);

    akinator.ctor(data_file);

    data_t data1 = {"cat", strlen("cat")};
    akinator.define_word(&data1, stdout);

    data_t data2 = {"laptop", strlen("laptop")};
    akinator.define_word(&data2, stdout);

    akinator.find_difference(&data1, &data2, stdout);

    akinator.play();
    akinator.play();

    akinator.dump();

    akinator.dtor();

    if (fclose(data_file) == EOF) {
        LOG(ERROR, "Failed to close a data_file\n");
        return EXIT_FAILURE;
    }

    if (fclose(ostream) == EOF) {
        LOG(ERROR, "Failed to close a dump_file\n");
        return EXIT_FAILURE;
    }

    if (fclose(logger_file) == EOF) {
        fprintf(stderr, "Failed to close a logger_file\n");
        return EXIT_FAILURE;
    }
}

// TODO - menu, text-to-sound, save database, CHEСK, SDL(menu)
