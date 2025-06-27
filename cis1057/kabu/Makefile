CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Target executable
TARGET = $(BINDIR)/kabu

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

test: test-order test-book test-market

test-order: $(BINDIR)/test_order
	$(BINDIR)/test_order

test-book: $(BINDIR)/test_book
	$(BINDIR)/test_book

test-market: $(BINDIR)/test_market
	$(BINDIR)/test_market

# Test executables
$(BINDIR)/test_order: test/test_order.c | $(BINDIR)
	$(CC) $(CFLAGS) $< -o $@

$(BINDIR)/test_book: test/test_book.c | $(BINDIR)
	$(CC) $(CFLAGS) $< -o $@

$(BINDIR)/test_market: test/test_market.c | $(BINDIR)
	$(CC) $(CFLAGS) $< -o $@