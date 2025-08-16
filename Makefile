# LongNums Makefile
# Supports both debug and release builds

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread
DEBUG_FLAGS = -g -DDEBUG
RELEASE_FLAGS = -O2 -DNDEBUG -march=native

# Directories
SRCDIR = src
INCDIR = include
BINDIR = bin
BUILDDIR = build

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.cpp) \
          $(wildcard $(SRCDIR)/core/*.cpp) \
          $(wildcard $(SRCDIR)/prime/*.cpp) \
          $(wildcard $(SRCDIR)/random/*.cpp) \
          $(wildcard $(SRCDIR)/math/*.cpp) \
          $(wildcard $(SRCDIR)/utils/*.cpp)

# Object files (all with .o extension for consistency)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)
OBJECTS_NO_MAIN = $(filter-out $(BUILDDIR)/main.o, $(OBJECTS))

# Target executables
TARGET = $(BINDIR)/LongNums
PERFORMANCE_TEST = $(BINDIR)/performance_test

# Default target
all: release $(PERFORMANCE_TEST)

# Debug build
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET) $(PERFORMANCE_TEST)

# Release build
release: CXXFLAGS += $(RELEASE_FLAGS)
release: $(TARGET) $(PERFORMANCE_TEST)

# Create directories and build executables
$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CXX) $(OBJECTS) -o $(TARGET) $(CXXFLAGS)

$(PERFORMANCE_TEST): performance_test.cpp $(OBJECTS_NO_MAIN) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) performance_test.cpp $(OBJECTS_NO_MAIN) -o $(PERFORMANCE_TEST)

# Compile source files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

# Create directories
$(BINDIR):
	mkdir -p $(BINDIR)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Run performance test
run-performance: $(PERFORMANCE_TEST)
	./$(PERFORMANCE_TEST)

# Build and run AlphaTensor random test
TEST_BIN = $(BINDIR)/test_alphatensor_random
TEST_SRC = tests/test_alphatensor_random.cpp

$(TEST_BIN): $(TEST_SRC) $(OBJECTS_NO_MAIN) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) $(TEST_SRC) $(OBJECTS_NO_MAIN) -o $(TEST_BIN)

.PHONY: test-alphatensor
test-alphatensor: $(TEST_BIN)
	./$(TEST_BIN)

# Clean build artifacts
clean:
	rm -rf $(BUILDDIR) $(BINDIR)
	rm -f performance_test

# Install (copy to system bin directory)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Uninstall
uninstall:
	rm -f /usr/local/bin/LongNums

# Show help
help:
	@echo "LongNums Makefile"
	@echo "================="
	@echo "Available targets:"
	@echo "  all            - Build release version and performance test (default)"
	@echo "  debug          - Build with debug symbols"
	@echo "  release        - Build optimized release version"
	@echo "  run            - Build and run the program"
	@echo "  run-performance- Build and run performance test"
	@echo "  clean          - Remove build artifacts"
	@echo "  install        - Install to /usr/local/bin"
	@echo "  uninstall      - Remove from /usr/local/bin"
	@echo "  help           - Show this help message"

.PHONY: all debug release run run-performance clean install uninstall help 