
# Generates the makefile "sysd-rules" given a list of sysdeps
# subdirectories (the sysdirs search path).  For every basename that
# appears in the search path, e.g. "vfork", it picks the source file
# that appears first, e.g. "sysdeps/unix/sysv/linux/i386/vfork.S".

# TODO(mseaborn): Handle check-inhibit-asm
# TODO(mseaborn): Handle rule for installing .h files

import os
import sys


source_suffixes = (".c", ".S", ".s")


def parse_syscalls_list(path):
    for line in open(path, "r"):
        if not line.startswith("#") and line.strip() != "":
            yield line.split()


# This is a list of sources files that take priority over files found
# via the sysdeps search path.  This allows us, for example, to ignore
# sysdeps/i386/memcmp.S without overriding
# sysdeps/i386/dl-trampoline.S.
# TODO(mseaborn): Make this mechanism less NaCl-specific by moving
# this list to a file that is found via the search path.
override_list = [
    "debug/memcpy_chk.c",
    "dirent/alphasort.c",
    "dirent/scandir.c",
    "dirent/versionsort.c",
    "inet/htonl.c",
    "inet/htons.c",
    "io/ftw.c",
    "io/lockf.c",
    "io/posix_fadvise.c",
    "libio/fseeko.c",
    "libio/ftello.c",
    "libio/iofgetpos.c",
    "libio/iofopen.c",
    "libio/iofsetpos.c",
    "math/e_acosl.c",
    "math/e_atan2l.c",
    "math/e_expl.c",
    "math/e_sqrtl.c",
    "math/s_atanl.c",
    "math/s_cexp.c",
    "math/s_cexpl.c",
    "math/s_fmax.c",
    "math/s_fmaxf.c",
    "math/s_fmin.c",
    "math/s_fminf.c",
    "math/s_significandl.c",
    "misc/mkostemp.c",
    "misc/mkstemp.c",
    "misc/syscall.c",
    "misc/truncate.c",
    "nptl/pthread_barrier_wait.c",
    "nptl/pthread_cond_broadcast.c",
    "nptl/pthread_cond_signal.c",
    "nptl/pthread_cond_timedwait.c",
    "nptl/pthread_cond_wait.c",
    "nptl/pthread_once.c",
    "nptl/pthread_rwlock_rdlock.c",
    "nptl/pthread_rwlock_timedrdlock.c",
    "nptl/pthread_rwlock_timedwrlock.c",
    "nptl/pthread_rwlock_unlock.c",
    "nptl/pthread_rwlock_wrlock.c",
    "nptl/pthread_spin_init.c",
    "nptl/pthread_spin_lock.c",
    "nptl/pthread_spin_trylock.c",
    "nptl/pthread_spin_unlock.c",
    "nptl/sysdeps/unix/sysv/linux/libc-lowlevellock.c",
    "nptl/sysdeps/unix/sysv/linux/lowlevellock.c",
    "nptl/sysdeps/unix/sysv/linux/lowlevelrobustlock.c",
    "nptl/sysdeps/unix/sysv/linux/sem_post.c",
    "nptl/sysdeps/unix/sysv/linux/sem_timedwait.c",
    "nptl/sysdeps/unix/sysv/linux/sem_trywait.c",
    "nptl/sysdeps/unix/sysv/linux/sem_wait.c",
    "posix/glob.c",
    "posix/pread.c",
    "posix/pwrite.c",
    "posix/vfork.c",
    "socket/accept.c",
    "socket/bind.c",
    "socket/connect.c",
    "socket/getpeername.c",
    "socket/getsockname.c",
    "socket/getsockopt.c",
    "socket/listen.c",
    "socket/recv.c",
    "socket/recvfrom.c",
    "socket/recvmsg.c",
    "socket/send.c",
    "socket/sendmsg.c",
    "socket/sendto.c",
    "socket/setsockopt.c",
    "socket/shutdown.c",
    "socket/socket.c",
    "socket/socketpair.c",
    "stdio-common/tmpfile.c",
    "stdlib/add_n.c",
    "stdlib/addmul_1.c",
    "stdlib/getcontext.c",
    "stdlib/lshift.c",
    "stdlib/makecontext.c",
    "stdlib/mul_1.c",
    "stdlib/rshift.c",
    "stdlib/setcontext.c",
    "stdlib/sub_n.c",
    "stdlib/submul_1.c",
    "stdlib/swapcontext.c",
    "string/bzero.c",
    "string/memchr.c",
    "string/memcmp.c",
    "string/memcpy.c",
    "string/mempcpy.c",
    "string/memset.c",
    "string/rawmemchr.c",
    "string/stpcpy.c",
    "string/stpncpy.c",
    "string/strcat.c",
    "string/strchr.c",
    "string/strchrnul.c",
    "string/strcmp.c",
    "string/strcpy.c",
    "string/strcspn.c",
    "string/strlen.c",
    "string/strpbrk.c",
    "string/strrchr.c",
    "string/strspn.c",
    "string/strtok.c",
    "string/strtok_r.c",
    "sysdeps/generic/initfini.c",
    "sysdeps/posix/writev.c",
    "sysvipc/semtimedop.c",
    "sysdeps/ieee754/dbl-64/e_atanh.c",
    "sysdeps/ieee754/dbl-64/s_asinh.c",
    "sysdeps/ieee754/dbl-64/s_copysign.c",
    "sysdeps/ieee754/dbl-64/s_llrint.c",
    "sysdeps/ieee754/dbl-64/s_lrint.c",
    "sysdeps/ieee754/dbl-64/s_sincos.c",
    "sysdeps/ieee754/flt-32/e_atanhf.c",
    "sysdeps/ieee754/flt-32/s_asinhf.c",
    "sysdeps/ieee754/flt-32/s_copysignf.c",
    "sysdeps/ieee754/flt-32/s_llrintf.c",
    "sysdeps/ieee754/flt-32/s_lrintf.c",
    "sysdeps/ieee754/ldbl-96/e_atanhl.c",
    "sysdeps/ieee754/ldbl-96/s_asinhl.c",
    "sysdeps/ieee754/ldbl-96/s_logbl.c",
    "sysdeps/ieee754/ldbl-96/s_rintl.c",
    "sysdeps/nacl/umount.c",
    "sysdeps/posix/libc_fatal.c",
    "sysdeps/posix/posix_fallocate.c",
    "sysdeps/posix/writev.c",
    "sysdeps/unix/readdir_r.c",
    "sysdeps/unix/sysv/linux/epoll_pwait.c",
    "sysdeps/unix/sysv/linux/fstatvfs.c",
    "sysdeps/unix/sysv/linux/getdirentries.c",
    "sysdeps/unix/sysv/linux/openat.c",
    "sysdeps/unix/sysv/linux/sched_getcpu.c",
    "sysdeps/unix/sysv/linux/statvfs.c",
    "sysdeps/unix/sysv/linux/sync_file_range.c",
    "sysdeps/unix/time.c",
    "sysvipc/semtimedop.c",
    ]

def get_override_map():
    override_map = {}
    for filename in override_list:
        assert filename[-2:] in source_suffixes, filename
        assert os.path.exists(filename), filename
        override_map[os.path.basename(filename[:-2])] = filename
    return override_map


class RuleGenerator(object):

    def __init__(self, sysdirs):
        self.sysdirs = sysdirs
        self.all_leafnames = set()
        # Checking all_files is a little faster than os.path.exists().
        self.all_files = set()
        self.autogen_syscalls = set()
        self.override_map = get_override_map()

    def scan_sysdirs(self):
        for subdir in self.sysdirs:
            for leafname in os.listdir(subdir):
                pathname = os.path.join(subdir, leafname)
                self.all_files.add(pathname)
                if leafname[-2:] in source_suffixes:
                    self.all_leafnames.add(leafname[:-2])
                elif leafname == "syscalls.list":
                    for fields in parse_syscalls_list(pathname):
                        # We don't need to look at the 2nd field.  It
                        # does not override other files - it can only
                        # cause this entry to be overridden by others.
                        name = fields[0]
                        self.autogen_syscalls.add((subdir, name))

    def find_matches(self, leafname):
        if leafname in self.override_map:
            yield False, self.override_map[leafname]
        for subdir in self.sysdirs:
            # A syscalls.list entry for "mkdir" (for example) overrides
            # "mkdir.c" in the same directory.
            if (subdir, leafname) in self.autogen_syscalls:
                yield True, "SYSCALL:%s:%s" % (subdir, leafname)
            for suffix in source_suffixes:
                pathname = os.path.join(subdir, leafname + suffix)
                if pathname in self.all_files:
                    yield False, pathname

    def put_leafname_rules(self, stream, leafname):
        matches = list(self.find_matches(leafname))
        if len(matches) == 0:
            raise Exception("No match for %s" % leafname)
        matches_string = " ".join(desc for is_autogen, desc in matches)
        stream.write("\n### %s: %s\n" % (leafname, matches_string))
        is_autogen, pathname = matches[0]
        if is_autogen:
            # We don't need to generate rules for this leafname:
            # its rules are generated by sysdeps/unix/make-syscalls.sh.
            return

        def put_lib_rules(leafname, flags):
            for dest_suffix in (".o", ".os"):
                args = {"pathname": pathname,
                        "src_suffix": pathname[-2:],
                        "dest_suffix": dest_suffix,
                        "leafname": leafname,
                        "flags": flags}
                stream.write(
                    "$(objpfx)%(leafname)s%(dest_suffix)s: "
                    "$(..)%(pathname)s $(before-compile)\n"
                    #"\t@echo building %(leafname)s%(dest_suffix)s\n"
                    "\t$(compile-command%(src_suffix)s)%(flags)s\n" % args)

        # Object files for libpthread.so
        put_lib_rules("ptw-" + leafname, " $(ptw-CPPFLAGS)")
        # Object files for ld.so (dynamic linker)
        put_lib_rules("rtld-" + leafname, " $(rtld-CPPFLAGS)")
        if leafname.startswith("s_"):
            # Object files for libm.so (maths)
            put_lib_rules("m_" + leafname[2:], " $(m_CPPFLAGS)")
        # Object files for libc.so/libc.a and other libraries
        put_lib_rules(leafname, "")

    def put_override_warnings(self, stream):
        for leafname in sorted(self.override_map.iterkeys()):
            if leafname not in self.all_leafnames:
                stream.write(
                    "\n### Warning: nothing to override for %s." % leafname)

    def put_rules(self, stream):
        for leafname in sorted(self.all_leafnames):
            self.put_leafname_rules(stream, leafname)


def main(args):
    generator = RuleGenerator(args)
    generator.scan_sysdirs()
    generator.put_override_warnings(sys.stdout)
    generator.put_rules(sys.stdout)


if __name__ == "__main__":
    main(sys.argv[1:])
