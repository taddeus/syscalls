/*
 * Every write_XX function does exactly one 'write' syscall
 */

#ifndef _MYWRITE_H
#define _MYWRITE_H

#include <sys/syscall.h>
#include <sys/types.h>
#include <alloca.h>
#include "mysyscall.h"

typedef unsigned uint;

static ssize_t uint_nchars(uint i, uint base) {
    uint rem = i;
    ssize_t nchars = 0;

    do {
        nchars++;
        rem /= base;
    } while (rem);

    return nchars;
}

static ssize_t write(int fd, const void *buf, size_t count) {
    ssize_t ret;
    SYSCALL3(SYS_write, fd, buf, count);
    SYSCALL_RET(ret);
    return ret;
}

static inline void blit_char(char *buf, char c, ssize_t count) {
    while (count--)
        *buf++ = c;
}

static ssize_t write_chars(int fd, char c, ssize_t count) {
    char *buf = alloca(count);
    blit_char(buf, c, count);
    return write(fd, buf, count);
}

static ssize_t write_char(int fd, char c) {
    return write_chars(fd, c, 1);
}

static void blit_uint(char *buf, uint i, uint base) {
    static const char digits[] = "0123456789abcdef";
    buf += uint_nchars(i, base);

    do {
        *--buf = digits[i % base];
        i /= base;
    } while (i);
}

static ssize_t write_int(int fd, int i, uint base) {
    int sign = i < 0;
    uint j = sign ? -i : i;
    ssize_t nchars = sign + uint_nchars(j, base);
    char *buf = alloca(nchars);
    if (sign) *buf = '-';
    blit_uint(buf + sign, j, base);
    return write(fd, buf, nchars);
}

static ssize_t write_uint(int fd, uint i, uint base) {
    ssize_t nchars = uint_nchars(i, base);
    char *buf = alloca(nchars);
    blit_uint(buf, i, base);
    return write(fd, buf, nchars);
}

static ssize_t write_bin(int fd, uint i) {
    return write_uint(fd, i, 2);
}

static ssize_t write_oct(int fd, uint i) {
    return write_uint(fd, i, 8);
}

static ssize_t write_dec(int fd, int i) {
    return write_int(fd, i, 10);
}

static ssize_t write_hex(int fd, uint i) {
    return write_uint(fd, i, 16);
}

static ssize_t write_uint_pad(int fd, uint i, uint base, char c, ssize_t width) {
    ssize_t nchars = uint_nchars(i, base);
    ssize_t pad = nchars < width ? width - nchars : 0;
    char *buf = alloca(pad + nchars);
    blit_char(buf, c, pad);
    blit_uint(buf + pad, i, base);
    return write(fd, buf, pad + nchars);
}

static ssize_t pad_right(int fd, ssize_t nwritten, char c, ssize_t width) {
    return width > nwritten ? write_chars(fd, c, width - nwritten) : 0;
}

#endif  // _MYWRITE_H
