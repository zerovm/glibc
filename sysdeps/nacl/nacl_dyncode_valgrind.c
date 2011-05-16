
#include <nacl_dyncode_valgrind.h>
#include <nacl_valgrind.h>
#include <sys/mman.h>

#define VG_USERREQ__NACL_MMAP 0x3107


/* Update Valgrind's idea of the nexe's memory layout. Intentionally empty. When
   running under Valgrind, calls to this function are replaced with calls to the
   wrapper function (see below).
   It's important that both functions are visible in the ld.so symbol table.

   Clone_mapping is an address of some other mmap-ed region of the same file. If
   not NULL, Valgrind will use it to find the binary. */
void __nacl_dyncode_map_for_valgrind (
    void *vma __attribute__((unused)),
    size_t size __attribute__((unused)),
    size_t file_offset __attribute__((unused)),
    void *clone_mapping __attribute__((unused))) {}


void __nacl_data_map_for_valgrind (
    void *vma __attribute__((unused)),
    size_t size __attribute__((unused)),
    size_t file_offset __attribute__((unused)),
    int fd __attribute__((unused)),
    int prot __attribute__((unused))) {}

/* Valgrind wrapper for NaClDyncodeMapForValgrind. This function is only
   executed if running under Valgrind. */
void I_WRAP_SONAME_FNNAME_ZZ(NaClZuZa, ZuZunaclZudyncodeZumapZuforZuvalgrind) (
    void *vma, size_t size, size_t file_offset, void *clone_mapping) {
  int res;
  VALGRIND_DO_CLIENT_REQUEST (res, 0, VG_USERREQ__NACL_MMAP,
      VALGRIND_SANDBOX_PTR ((size_t)vma),
      size, file_offset, PROT_READ | PROT_EXEC,
      VALGRIND_SANDBOX_PTR ((size_t)clone_mapping));
  (void)res;
}

void I_WRAP_SONAME_FNNAME_ZZ(NaClZuZa, ZuZunaclZudataZumapZuforZuvalgrind) (
    void *vma, size_t size, size_t file_offset, int fd, int prot) {
  int res;
  /* We can not just pass the file descriptor to Valgrind, because their values
     are different between trusted and untrusted code. Therefore, we create a
     temporary mapping of the same file and let Valgrind find the file name
     by examining this mapping. We use PROT_NONE so that Valgrind does not
     confuse it with the actual RO or RW mapping of the same binary. */
  size_t alignment_padding = file_offset & (getpagesize() - 1);
  uint8_t *clone_mapping = __mmap (NULL, getpagesize(),
                             PROT_NONE, MAP_PRIVATE, fd,
                             file_offset - alignment_padding);
  if (clone_mapping == MAP_FAILED)
    return;

  VALGRIND_DO_CLIENT_REQUEST (res, 0, VG_USERREQ__NACL_MMAP,
      VALGRIND_SANDBOX_PTR ((size_t)vma),
      size, file_offset, prot,
      VALGRIND_SANDBOX_PTR ((size_t)clone_mapping));

  __munmap (clone_mapping, getpagesize());
  (void)res;
}
