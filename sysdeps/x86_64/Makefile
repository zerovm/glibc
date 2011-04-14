# In Native Client `long double' == `double`.
long-double-fcts = no

ifeq ($(subdir),csu)
sysdep_routines += hp-timing
elide-routines.os += hp-timing
endif

ifeq ($(subdir),gmon)
# We cannot support the _mcount ABI mandate correctly in Native Client
# without compiler changes.  The vanilla mandate is that calling _mcount
# preserve all caller registers, but NaCl code cannot even return without
# using a scratch register, so this is impossible.  By just omitting _mcount
# from the build, we ensure that anyone building with -pg will get a simple
# link-time error.
#sysdep_routines += _mcount
endif

ifeq ($(subdir),string)
sysdep_routines += cacheinfo
endif

ifeq ($(subdir),elf)
sysdep-dl-routines += tlsdesc dl-tlsdesc
sysdep_routines += tlsdesc dl-tlsdesc
sysdep-rtld-routines += tlsdesc dl-tlsdesc
endif

ifeq ($(subdir),csu)
gen-as-const-headers += tlsdesc.sym
endif
