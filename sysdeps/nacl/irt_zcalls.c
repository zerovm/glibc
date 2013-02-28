#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "zrt.h"

int (*__zcall_fcntl) (int fd, int cmd, ...);


void
init_zcalls_nonsyscalls(void){
        /*IRT syscalls can be redefined, it's totally saves nacl implementation*/
    struct zcalls_nonsyscalls_t* zcalls;
    if ( ZCALLS_NONSYSCALLS == __query_zcalls(ZCALLS_NONSYSCALLS, (void**)&zcalls) &&
	 zcalls ){
	__zcall_fcntl          = zcalls->fcntl;
    }
}
