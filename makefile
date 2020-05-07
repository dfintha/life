BINARY=bin/life
SOURCE=src/life.c
STATE=state.txt

CC=gcc
CFLAGS=-std=c89 -Wall -Wextra -O2
LDFLAGS=-lncurses

.PHONY: all, clean

all: $(BINARY)

loc:
	@printf "[N] "
	@cat src/* | wc -l

run: $(BINARY) $(STATE)
	@$(BINARY)

$(BINARY): $(SOURCE)
	@mkdir -p bin
	@printf "[L] $@\n"
	@$(CC) $< $(LDFLAGS) $(CFLAGS) -o $@
	@strip $@
	@printf "[+] Build Succeeded!\n"

clean:
	@printf "[R] bin\n"
	@rm -rf bin

