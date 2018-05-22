CFLAGS  = -std=c99
CFLAGS += -g
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -pedantic
CFLAGS += -Werror

VFLAGS  = --quiet
VFLAGS += --tool=memcheck
VFLAGS += --leak-check=full
VFLAGS += --error-exitcode=1

build:
	@$(CC) $(CFLAGS) *.c -o call-u-later 

clean:
	rm -rf *.o *.dSYM call-u-later

memcheck: build 
	@valgrind $(VFLAGS) ./call-u-later abc
	@echo "Memory check passed"

run: build
	./call-u-later
