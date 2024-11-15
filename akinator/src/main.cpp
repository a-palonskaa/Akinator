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

    FILE* data_file = fopen("../data/database.txt", "r");
    if (data_file == nullptr) {
        LOG(ERROR, "Failed to open an input file\n");
        return EXIT_FAILURE;
    }

    FILE* ostream = fopen("../logs/trees.html", "wb");
    if (ostream == nullptr) {
        LOG(ERROR, "Failed to open a dump file\n");
        return EXIT_FAILURE;
    }

    FILE* database_ostream = fopen("../logs/new_database.txt", "w");
    if (database_ostream == nullptr) {
        LOG(ERROR, "Failed to open a dump file\n");
        return EXIT_FAILURE;
    }

    akinator_t akinator{};
    akinator.set_dump_ostream(ostream);
    akinator.set_database_ostream(database_ostream);

    akinator.ctor(data_file);

    akinator.play_akinator_game();

    akinator.dtor();

    if (fclose(data_file) == EOF) {
        LOG(ERROR, "Failed to close a data_file\n");
        return EXIT_FAILURE;
    }

    if (fclose(ostream) == EOF) {
        LOG(ERROR, "Failed to close a dump_file\n");
        return EXIT_FAILURE;
    }

    if (fclose(database_ostream) == EOF) {
        LOG(ERROR, "Failed to close a dump_file\n");
        return EXIT_FAILURE;
    }

    if (fclose(logger_file) == EOF) {
        fprintf(stderr, "Failed to close a logger_file\n");
        return EXIT_FAILURE;
    }
}

