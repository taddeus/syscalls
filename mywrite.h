#ifndef _MYWRITE_H
#define _MYWRITE_H

#include <sys/syscall.h>
#include <sys/types.h>
#include <alloca.h>
#include "mysyscall.h"

static ssize_t write(int fd, const void *buf, size_t count) {
    ssize_t ret;
    SYSCALL3(SYS_write, fd, buf, count);
    SYSCALL_RET(ret);
    return ret;
}

static ssize_t write_char(int fd, char c) {
    static char buf;  // FIXME: this is necessary because of -O
    buf = c;
    return write(fd, &buf, 1);
}

static ssize_t write_unsigned(int fd, unsigned i, int base) {
    static const char digits[] = "0123456789abcdef";
    char *buf, *p;
    int rem = i, nchars = 0;

    do {
        nchars++;
        rem /= base;
    } while (rem);

    buf = alloca(nchars);
    p = buf + nchars;

    do {
        *--p = digits[i % base];
        i /= base;
    } while (i);

    return write(fd, buf, nchars);
}

static ssize_t write_int(int fd, int i, int base) {
    int neg = i < 0;

    if (neg) {
        i = -i;
        write_char(fd, '-');
    }

    return neg + write_unsigned(fd, i, base);
}

static ssize_t write_bin(int fd, unsigned i) {
    return write_unsigned(fd, i, 2);
}

static ssize_t write_oct(int fd, unsigned i) {
    return write_unsigned(fd, i, 8);
}

static ssize_t write_dec(int fd, int i) {
    return write_int(fd, i, 10);
}

static ssize_t write_hex(int fd, int i) {
    return write_unsigned(fd, i, 16);
}

#endif  // _MYWRITE_H
