/*
 * This file replaces GNU2 dialect TLS support with stubs to make the loader 
 * pass the NaCl validator.
 */

#include <stdlib.h>
#include <unistd.h>
#include <ldsodefs.h>

void _dl_tlsdesc_return () {
  _dl_error_printf ("NaCl doesn't support gnu2 TLS dialect\n");
  _exit (EXIT_FAILURE);
}

void _dl_tlsdesc_undefweak () {
  _dl_error_printf ("NaCl doesn't support gnu2 TLS dialect\n");
  _exit (EXIT_FAILURE);
}

void _dl_tlsdesc_resolve_abs_plus_addend () {
  _dl_error_printf ("NaCl doesn't support gnu2 TLS dialect\n");
  _exit (EXIT_FAILURE);
}

void _dl_tlsdesc_resolve_rel () {
  _dl_error_printf ("NaCl doesn't support gnu2 TLS dialect\n");
  _exit (EXIT_FAILURE);
}

void _dl_tlsdesc_resolve_rela () {
  _dl_error_printf ("NaCl doesn't support gnu2 TLS dialect\n");
  _exit (EXIT_FAILURE);
}

void _dl_tlsdesc_resolve_hold () {
  _dl_error_printf ("NaCl doesn't support gnu2 TLS dialect\n");
  _exit (EXIT_FAILURE);
}

#ifdef SHARED
void _dl_tlsdesc_dynamic () {
  _dl_error_printf ("NaCl doesn't support gnu2 TLS dialect\n");
  _exit (EXIT_FAILURE);
}
#endif


