
#include <nacl_dyncode.h>
#include <nacl_dyncode_valgrind.h>
#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>


/* Dynamically load code from a file.  One day NaCl might provide a
   syscall that provides this functionality without needing to make a
   copy of the code.  offset and size do not need to be page-aligned. */
int nacl_dyncode_map (int fd, void *dest, size_t offset, size_t size)
{
  size_t alignment_padding = offset & (getpagesize() - 1);
  uint8_t *mapping = __mmap (NULL, size + alignment_padding,
                             PROT_READ, MAP_PRIVATE, fd,
                             offset - alignment_padding);
  if (mapping == MAP_FAILED)
    return -1;
  int result = __nacl_dyncode_copy (dest, mapping + alignment_padding, size);

  // Tell Valgrind about this mapping.
  __nacl_dyncode_map_for_valgrind (dest, size, offset, mapping);
  int munmap_result = __munmap (mapping, size);
  if (result != 0 || munmap_result != 0)
    return -1;
  return 0;
}
