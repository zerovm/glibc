
import StringIO
import unittest

import make_sysd_rules


# These tests use the glibc source tree as example data on which to
# test the search rules.
# TODO(mseaborn): Could provide example data here so that the test
# doesn't break when the glibc tree changes.

sysdirs = [
    "sysdeps/unix/sysv/linux/i386",
    "sysdeps/unix/sysv/linux",
    "sysdeps/unix/sysv",
    "sysdeps/unix",
    "sysdeps/i386/i686",
    "sysdeps/i386/i486",
    "sysdeps/i386/fpu",
    "sysdeps/i386",
    ]


def get_matches(name):
    generator = make_sysd_rules.RuleGenerator(sysdirs)
    generator.scan_sysdirs()
    return list(generator.find_matches(name))


def get_rules(name):
    generator = make_sysd_rules.RuleGenerator(sysdirs)
    generator.scan_sysdirs()
    stream = StringIO.StringIO()
    generator.put_leafname_rules(stream, name)
    return stream.getvalue()


class GeneratorTest(unittest.TestCase):

    def assertEquals(self, x, y):
        if x != y:
            if type(x) is str and type(y) is str:
                # This makes it easier to compare multi-line strings.
                raise AssertionError('"%s" != "%s"' % (x, y))
            else:
                raise AssertionError("%r != %r" % (x, y))

    def test_mmap(self):
        self.assertEquals(get_matches("mmap"),
                          [(False, "sysdeps/unix/sysv/linux/i386/mmap.S"),
                           (True, "SYSCALL:sysdeps/unix/sysv/linux:mmap")])
        self.assertEquals(get_rules("mmap"), """
### mmap: sysdeps/unix/sysv/linux/i386/mmap.S SYSCALL:sysdeps/unix/sysv/linux:mmap\n$(objpfx)ptw-mmap.o: $(..)sysdeps/unix/sysv/linux/i386/mmap.S $(before-compile)
\t$(compile-command.S) $(ptw-CPPFLAGS)
$(objpfx)ptw-mmap.os: $(..)sysdeps/unix/sysv/linux/i386/mmap.S $(before-compile)
\t$(compile-command.S) $(ptw-CPPFLAGS)
$(objpfx)rtld-mmap.o: $(..)sysdeps/unix/sysv/linux/i386/mmap.S $(before-compile)
\t$(compile-command.S) $(rtld-CPPFLAGS)
$(objpfx)rtld-mmap.os: $(..)sysdeps/unix/sysv/linux/i386/mmap.S $(before-compile)
\t$(compile-command.S) $(rtld-CPPFLAGS)
$(objpfx)mmap.o: $(..)sysdeps/unix/sysv/linux/i386/mmap.S $(before-compile)
\t$(compile-command.S)
$(objpfx)mmap.os: $(..)sysdeps/unix/sysv/linux/i386/mmap.S $(before-compile)
\t$(compile-command.S)
""")

    def test_memcmp(self):
        self.assertEquals(get_matches("memcmp"),
                          [(False, "sysdeps/i386/i686/memcmp.S"),
                           (False, "sysdeps/i386/memcmp.S")])

    def test_open(self):
        self.assertEquals(get_matches("open"),
                          [(True, "SYSCALL:sysdeps/unix:open")])
        self.assertEquals(get_rules("open"), """
### open: SYSCALL:sysdeps/unix:open
""")

    def test_fabs(self):
        # This produces an extra m_fabs target.
        self.assertEquals(get_matches("s_fabs"),
                          [(False, "sysdeps/i386/fpu/s_fabs.S")])
        self.assertEquals(get_rules("s_fabs"), """
### s_fabs: sysdeps/i386/fpu/s_fabs.S
$(objpfx)ptw-s_fabs.o: $(..)sysdeps/i386/fpu/s_fabs.S $(before-compile)
\t$(compile-command.S) $(ptw-CPPFLAGS)
$(objpfx)ptw-s_fabs.os: $(..)sysdeps/i386/fpu/s_fabs.S $(before-compile)
\t$(compile-command.S) $(ptw-CPPFLAGS)
$(objpfx)rtld-s_fabs.o: $(..)sysdeps/i386/fpu/s_fabs.S $(before-compile)
\t$(compile-command.S) $(rtld-CPPFLAGS)
$(objpfx)rtld-s_fabs.os: $(..)sysdeps/i386/fpu/s_fabs.S $(before-compile)
\t$(compile-command.S) $(rtld-CPPFLAGS)
$(objpfx)m_fabs.o: $(..)sysdeps/i386/fpu/s_fabs.S $(before-compile)
\t$(compile-command.S) $(m_CPPFLAGS)
$(objpfx)m_fabs.os: $(..)sysdeps/i386/fpu/s_fabs.S $(before-compile)
\t$(compile-command.S) $(m_CPPFLAGS)
$(objpfx)s_fabs.o: $(..)sysdeps/i386/fpu/s_fabs.S $(before-compile)
\t$(compile-command.S)
$(objpfx)s_fabs.os: $(..)sysdeps/i386/fpu/s_fabs.S $(before-compile)
\t$(compile-command.S)
""")


if __name__ == "__main__":
    unittest.main()
