#include <fork.h>

int
__register_atfork (prepare, parent, child, dso_handle)
     void (*prepare) (void);
     void (*parent) (void);
     void (*child) (void);
     void *dso_handle;
{
  return 0;
}
libc_hidden_def (__register_atfork)

void
attribute_hidden
__linkin_atfork (struct fork_handler *newp)
{
}
