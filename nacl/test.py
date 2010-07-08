
# TODO(mseaborn): Move these tests into the NaCl Scons build.
# However, some of these tests are rather esoteric.

import os
import subprocess
import tempfile
import unittest


# This is an alternative to subprocess.PIPE, which can cause deadlocks
# if the pipe buffer gets filled up.
def make_fh_pair():
    # Make read/write file handle pair.  This is like creating a pipe
    # FD pair, but without a pipe buffer limit.
    fd, filename = tempfile.mkstemp(prefix="nacl_test_")
    try:
        write_fh = os.fdopen(fd, "w", 0)
        read_fh = open(filename, "r")
    finally:
        os.unlink(filename)
    return write_fh, read_fh


hellow_message = ("Hello world via write()\n"
                  "Hello world via printf(), 1234\n")


class GlibcTests(unittest.TestCase):

    def test_01_glibc_static(self):
        write_fh, read_fh = make_fh_pair()
        subprocess.check_call(["sel_ldr", "-s", "build/hellow-static"],
                              stdout=write_fh)
        self.assertEquals(read_fh.read(), hellow_message)

    def test_02_running_ldso(self):
        write_fh, read_fh = make_fh_pair()
        rc = subprocess.call(["sel_ldr", "-s", "build/elf/runnable-ld.so"],
                             stderr=write_fh)
        self.assertEquals(rc, 127)
        output = read_fh.read()
        # Check for ld.so's help message.
        assert ("You have invoked `ld.so', the helper program "
                "for shared library executables." in output)

    def test_03_running_libcso(self):
        write_fh, read_fh = make_fh_pair()
        subprocess.check_call(
            ["env", "NACLDYNCODE=1", "NACL_DANGEROUS_ENABLE_FILE_ACCESS=1",
             "sel_ldr", "-s", "build/elf/runnable-ld.so",
             "build/libc.so"],
            stdout=write_fh)
        output = read_fh.read()
        assert "GNU C Library stable release version 2.9" in output

    def test_04_glibc_dynamic(self):
        write_fh, read_fh = make_fh_pair()
        subprocess.check_call(
            ["env", "NACLDYNCODE=1", "NACL_DANGEROUS_ENABLE_FILE_ACCESS=1",
             "sel_ldr", "-s", "build/elf/runnable-ld.so",
             "--", "--library-path", "build",
             "build/hellow-dynamic"],
            stdout=write_fh)
        self.assertEquals(read_fh.read(), hellow_message)

    def test_05_executable_size(self):
        subprocess.check_call(["strip", "build/hellow-dynamic",
                               "-o", "build/hellow-dynamic.stripped"])
        size = os.stat("build/hellow-dynamic.stripped").st_size
        assert size < 4000, size

    def test_06_ldso_error_message(self):
        # Check that we get a reasonable error message from the
        # dynamic linker if it cannot find the libraries.  Previously
        # it faulted in longjmp().
        write_fh, read_fh = make_fh_pair()
        rc = subprocess.call(
            ["env", "NACLDYNCODE=1", "NACL_DANGEROUS_ENABLE_FILE_ACCESS=1",
             "sel_ldr", "-s", "build/elf/runnable-ld.so",
             "--", "--library-path", "does-not-exist",
             "build/hellow-dynamic"],
            stdout=write_fh, stderr=write_fh)
        self.assertEquals(rc, 127)
        output = read_fh.read()
        assert ("error while loading shared libraries: "
                "libc.so.6: cannot open shared object file" in output), output


if __name__ == "__main__":
    subprocess.check_call(["./nacl/make.sh"])
    unittest.main()
