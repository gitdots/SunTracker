# Compiler options
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -Iinterface

# External libraries
LDFLAGS = -pthread -li2c

# Directories
SRCDIR = source
BINDIR = binaries

# File lists
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)

# Target executable
TARGET = appl

# Libraries
LIBNAME = suntracker
LIBDIR = $(BINDIR)
LIBFILE = $(LIBDIR)/lib$(LIBNAME).a
LIBFLAGS = -L$(LIBDIR) -l$(LIBNAME)

# Targets
.PHONY: all clean run

all: $(TARGET)

# Build library
$(LIBFILE): $(OBJECTS)
	ar rcs $@ $^

$(BINDIR)/%.o: $(SRCDIR)/%.cpp | $(BINDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LDFLAGS)

$(BINDIR):
	mkdir -p $(BINDIR)

# Build target
$(TARGET): $(LIBFILE) main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBFLAGS) $(LDFLAGS)

# Run target
run: $(TARGET)
	./$(TARGET)

# Clean
clean:
	rm -rf $(BINDIR) $(TARGET)