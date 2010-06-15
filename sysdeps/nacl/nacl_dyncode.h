#ifndef _NACL_DYNCODE_H
#define _NACL_DYNCODE_H

#include <stdlib.h>

int __nacl_dyncode_copy (void *dest, const void *src, size_t size);
int nacl_dyncode_map (int fd, void *dest, size_t offset, size_t size);
void *nacl_dyncode_alloc (size_t code_size, size_t data_size,
                          size_t data_offset);

#endif
