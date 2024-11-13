#include <assert.h>
#include "utils.h"

void print_addr(void* elm, FILE* ostream) {
    assert(elm != nullptr);
    assert(ostream != nullptr);

    fprintf(ostream, "%p", *((char**)elm));
}
