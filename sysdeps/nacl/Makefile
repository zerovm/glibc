rtld-installed-name = $(patsubst %.$(NACL_FAKE_SONAME),%.1,$(ld.so-version))

# Remove libpthread.so's version of a function that is also provided
# by libc.so.
libpthread-routines := $(filter-out pt-vfork,$(libpthread-routines))

# ifeq ($(subdir),stdio-common)
# sysdep_routines += tempname
# endif

# ifeq ($(subdir),io)
# sysdep_routines += unlink
# endif


stdio-common-routines := $(sysdep-routines) tempname
io-routines := $(sysdep-routines) unlink getcwd
posix-routines := $(sysdep-routines) pathconf fpathconf

#do not link ld.so for static executable running on zerovm
ifeq (yes,$(build-shared)) #--enable-shared
# In case of NaCl any program which wants to call syscalls must be linked with
# ld.so explicitly.
LDLIBS-nscd += $(elfobjdir)/ld.so
endif
