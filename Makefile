CFLAGS := -m32 -nostdlib -O3

all: test.s test

%-pp.c: %.c
	$(CC) $(CFLAGS) -E -o $@ $<

%.s: %.c
	$(CC) $(CFLAGS) -S -o $@ $<

test: test.c

test.s test: mysyscall.h mywrite.h

clean:
	rm -f *.s *-pp.c test
