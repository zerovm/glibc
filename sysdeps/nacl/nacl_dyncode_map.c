
#include <nacl_dyncode.h>
#include <sys/mman.h>


/* Dynamically load code from a file.  One day NaCl might provide a
   syscall that provides this functionality without needing to make a
   copy of the code. */
int nacl_dyncode_map (int fd, void *dest, size_t offset, size_t size)
{
  void *mapping = __mmap (NULL, size, PROT_READ, MAP_PRIVATE, fd, offset);
  if (mapping == MAP_FAILED)
    return -1;
  int result = __nacl_dyncode_copy (dest, mapping, size);
  int munmap_result = __munmap (mapping, size);
  if (result != 0 || munmap_result != 0)
    return -1;
  return 0;
}
