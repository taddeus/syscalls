CFLAGS := -m32 -nostdlib -O3 -fno-builtin -std=gnu99

all: test.s test

%-pp.c: %.c
	$(CC) $(CFLAGS) -E -o $@ $<

%.s: %.c
	$(CC) $(CFLAGS) -S -o $@ $<

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

test: test.c

test.s test: mysyscall.h mywrite.h

check: test
	./$<

clean:
	rm -f *.s *-pp.c test
