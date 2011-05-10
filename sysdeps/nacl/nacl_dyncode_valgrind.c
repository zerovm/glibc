
#include <nacl_dyncode_valgrind.h>
#include <nacl_valgrind.h>

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


/* Valgrind wrapper for NaClDyncodeMapForValgrind. This function is only
   executed if running under Valgrind. */
void I_WRAP_SONAME_FNNAME_ZZ(NaClZuZa, ZuZunaclZudyncodeZumapZuforZuvalgrind) (
    void *vma, size_t size, size_t file_offset, void *clone_mapping) {
  int res;
  VALGRIND_DO_CLIENT_REQUEST (res, 0, VG_USERREQ__NACL_MMAP,
      VALGRIND_SANDBOX_PTR ((size_t)vma),
      size, file_offset, 0,
      VALGRIND_SANDBOX_PTR ((size_t)clone_mapping));
  (void)res;
}
