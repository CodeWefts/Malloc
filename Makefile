CFLAGS=-O0 -g -Wall -Wextra -Wno-deprecated-declarations -Wno-unused-parameter -Iinclude -Wno-unused-variable -Werror=implicit-function-declaration
CPPFLAGS=-MMD
LDFLAGS=
LDLIBS=
 
OBJS=tests/test_allocator.o src/m_allocator.o
DEPS=$(OBJS:.o=.d)

.PHONY: all clean

all: test_allocator

-include $(DEPS)

%.o: %.c
	gcc -c $(CFLAGS) $(CPPFLAGS) $< -o $@

test_allocator: $(OBJS)
	gcc $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -f $(OBJS) $(DEPS) $(PROGRAM) test_allocator