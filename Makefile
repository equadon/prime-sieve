# Benchmark variables
CORES=4
T=1048576
NUMTESTS=4

# Directories
BINDIR := bin
BUILDDIR := build
DOCDIR := doc
COVDIR := $(DOCDIR)/coverage
INCLUDEDIR := include
LIBDIR := lib
TESTDIR := tests
SRCDIR := src

# Compiler flags
CC := gcc

CFLAGS := -Wall -std=gnu11 -MMD -I $(SRCDIR) -I $(INCLUDEDIR)
CFLAGS_DEBUG := -D DEBUG -g --coverage
CFLAGS_RELEASE := -D RELEASE -Ofast

LDFLAGS := -lm -lpthread -L $(BUILDDIR)
LDFLAGS_DEBUG := -lgcov
LDFLAGS_RELEASE :=

# Executables
TARGET := $(BINDIR)/primes
TEST_TARGET := $(BINDIR)/tests

# Source and object files
SOURCES := $(shell find $(SRCDIR) -type f -name '*.c')
OBJECTS := $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SOURCES:.c=.o))

TEST_SOURCES := $(shell find $(TESTDIR) -type f -name '*.c')
TEST_OBJECTS := $(patsubst $(TESTDIR)/%, $(BUILDDIR)/%, $(TEST_SOURCES:.c=.o))

# Dependencies
DEPS := $(OBJECTS:.o=.d)
TEST_DEPS := $(TEST_OBJECTS:.o=.d)

# Force make to run 'make all' when no target is specified
all: debug

# Release target
release: CFLAGS += $(CFLAGS_RELEASE)
release: LDFLAGS += $(LDFLAGS_RELEASE)
release: clean-build $(TARGET)

# Debug target
debug: CFLAGS += $(CFLAGS_DEBUG)
debug: LDFLAGS += $(LDFLAGS_DEBUG)
debug: $(TARGET)

# Build and run unit tests
test: CFLAGS += $(CFLAGS_DEBUG)
test: LDFLAGS += $(LDFLAGS_DEBUG)
test: $(TEST_TARGET)
	@./$(TEST_TARGET)

# Build and run unit tests with valgrind
memtest: CFLAGS += $(CFLAGS_DEBUG)
memtest: LDFLAGS += $(LDFLAGS_DEBUG)
memtest: $(TEST_TARGET)
	@valgrind --leak-check=yes ./$(TEST_TARGET)

# Generate object files for src/*.c
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Generate object files for tests/*.c
$(BUILDDIR)/%.o: $(TESTDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link main program
$(TARGET): $(LIB) $(OBJECTS)
	$(CC) $^ -o $(TARGET) $(LDFLAGS)

# Link test program
$(TEST_TARGET): $(LIB) $(TEST_OBJECTS) $(filter-out $(BUILDDIR)/main.o, $(OBJECTS))
	$(CC) $^ -o $(TEST_TARGET) $(LDFLAGS) -lcunit

# Build documentation
doc:
	@doxygen Doxyfile
	@tools/browser "$(DOCDIR)/html/index.html"

# Code coverage
cov: test
	@echo "Generating HTML on code coverage..."
	@lcov --capture --directory . --output-file $(BUILDDIR)/coverage.data
	@genhtml $(BUILDDIR)/coverage.data --output-directory $(COVDIR)
	@echo "Code coverage saved to: $(COVDIR)/index.html"
	@tools/browser "$(COVDIR)/index.html"

# Clean all generated files
clean: clean-build
	rm -rf $(BUILDDIR) $(DOCDIR)/{html,latex,coverage} *.log

# Clean build files
clean-build:
	rm -rf $(BUILDDIR)/*.a $(BUILDDIR)/*.d $(BUILDDIR)/*.o $(TARGET) $(TEST_TARGET)

# Fix coding style in project
style:
	@astyle --options="astyle.options" "$(SRCDIR)/*" "$(TESTDIR)/*"

# Run benchmark tool
bench: HTMLDIR := $(BINDIR)/html
bench: HTMLFILE := $(HTMLDIR)/plots-$(shell date +%s).html
bench: clean-build release
	@mkdir -p $(HTMLDIR)
	@python tools/bench -s stats_opt bin/primes $(CORES) $(T) $(NUMTESTS) build/gprof.txt
	@cp $(BUILDDIR)/index.html $(HTMLFILE)
	@echo "Plots generated successfully. The results are saved in $(HTMLFILE)"
	@tools/browser "$(HTMLFILE)"

# Run GProf
gprof: CFLAGS += -pg
gprof: LDFLAGS += -pg
gprof: clean-build release
	@echo "Running GProf with sequential sieve and T=${T}..."
	@${TARGET} bit 1 ${T}
	@gprof ${TARGET} gmon.out > build/gprof.txt


-include $(DEPS)
-include $(TEST_DEPS)

# Clean is a phony target since it's not producing a file
.PHONY: all bench clean clean-build cov debug doc gprof release style test
