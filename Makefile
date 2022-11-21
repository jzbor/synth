
# Project name
PROJECT = Synth
BINARY = synth
SRCDIR = src

# Build files
SOURCES = $(shell find * -name "*.c")
OBJECTS = $(addprefix $(BUILDDIR)/,$(notdir $(SOURCES:.c=.o)))
BUILDDIR = .build

# Compiler configuration
CC = cc
CFLAGS = -std=c11 -pedantic -Wall -Werror
LDFLAGS = -lm

# Default target
.DEFAULT_GOAL = all

all: $(BINARY)

$(BUILDDIR):
	mkdir $(BUILDDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(BUILDDIR)
	$(CC) -o $@ -c $(CFLAGS) $<

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

run: $(BINARY)
	./$(BINARY)

clean:
	rm -rf "$(BUILDDIR)" "$(BINARY)"

.PHONY: clean run
