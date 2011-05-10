#ifndef _NACL_DYNCODE_VALGRIND_H
#define _NACL_DYNCODE_VALGRIND_H

#include <stdlib.h>

void __nacl_dyncode_map_for_valgrind (void *vma, size_t size,
    size_t file_offset, void *clone_mapping);

#endif
