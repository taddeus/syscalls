#include <sys/syscall.h>

#include "mysyscall.h"
#include "mywrite.h"

#define STDOUT_FILENO 1

#define OUTPUT(msg) {                            \
        static const char buf[] = msg;           \
        write(STDOUT_FILENO, buf, sizeof (buf)); \
    }

static void exit(int status) {
    SYSCALL1(SYS_exit, status);
}

void _start() {
    OUTPUT("Hello, World!\n");
    write_dec(1, -1900);         write_char(1, '\n');

    pad_right(1, write_hex(1, 0xde), ' ', 4);
    write_uint_pad(1, 0xad, 16, '0', 6);
    OUTPUT(" 0x");
    write_uint_pad(1, 0xbeef, 16, '-', 2);
    write_char(1, '\n');

    write_hex(1, 0xdeadbeef); write_char(1, '\n');
    write_oct(1, 0744);       write_char(1, '\n');
    exit(0);
}
