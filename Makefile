TARGET := bin/primes
TEST_TARGET := bin/tests

# Directories
SRCDIR := src
TESTDIR := tests
BUILDDIR := build
DOCDIR := doc

# Compiler flags
CC := gcc
CFLAGS := -g -Wall -std=gnu11
LDFLAGS :=
LDFLAGS_TEST := -lcunit

# Source and object files
SOURCES := $(shell find $(SRCDIR) -type f -name '*.c')
OBJECTS := $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SOURCES:.c=.o))

TEST_SOURCES := $(shell find $(TESTDIR) -type f -name '*.c')
TEST_OBJECTS := $(patsubst $(TESTDIR)/%, $(BUILDDIR)/%, $(TEST_SOURCES:.c=.o))

DEPS := $(OBJECTS:.o=.d)
TEST_DEPS := $(TEST_OBJECTS:.o=.d)

# Force make to run 'make all' when no target is specified
all: $(TARGET)

# Generate object files for src/*.c
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -MMD -I $(SRCDIR) -c $< -o $@

# Generate object files for tests/*.c
$(BUILDDIR)/%.o: $(TESTDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -MMD -I $(SRCDIR) -c $< -o $@

# Link main program
$(TARGET): $(OBJECTS)
	$(CC) $^ -o $(TARGET) -L lib $(LDFLAGS)

#Link test program
$(TEST_TARGET): $(TEST_OBJECTS) $(filter-out $(BUILDDIR)/main.o, $(OBJECTS))
	$(CC) $^ -o $(TEST_TARGET) -L lib $(LDFLAGS_TEST)

# Clean build files
clean:
	rm -rf $(BUILDDIR) $(DOCDIR)/* $(TARGET) $(TEST_TARGET)

# Build and run unit tests
test: $(TEST_TARGET)
	@./$(TEST_TARGET)

# Build documentation
doc:
	@doxygen Doxyfile

-include $(DEPS)
-include $(TEST_DEPS)

# Clean is a phony target since it's not producing a file
.PHONY: clean doc
