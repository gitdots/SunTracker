# # Compiler options
# CXX = g++
# CXXFLAGS = -std=c++11 -std=c++2a  -Wall -Wextra -pedantic -Iinterface

# # External libraries
# LDFLAGS = #-L/usr/lib -lexternal_library

# # Directories
# SRCDIR = source
# BINDIR = binaries

# # File lists
# SOURCES = $(wildcard $(SRCDIR)/*.cpp)
# OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)

# # Target executable
# TARGET = appl

# # Libraries
# LIBNAME = suntracker
# LIBDIR = $(BINDIR)
# LIBFILE = $(LIBDIR)/lib$(LIBNAME).a
# LIBFLAGS = -L$(LIBDIR) -l$(LIBNAME)

# # Targets
# .PHONY: all clean run

# all: $(TARGET)

# # Build library
# $(LIBFILE): $(OBJECTS)
# 	ar rcs $@ $^

# $(BINDIR)/%.o: $(SRCDIR)/%.cpp | $(BINDIR)
# 	$(CXX) $(CXXFLAGS) -c -o $@ $<

# $(BINDIR):
# 	mkdir -p $(BINDIR)

# # Build target
# $(TARGET): $(LIBFILE) main.cpp
# 	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBFLAGS)

# # Run target
# run: $(TARGET)
# 	./$(TARGET)

# # Clean
# clean:
# 	rm -rf $(BINDIR) $(TARGET)


# Compiler options
CXX = g++
CXXFLAGS = -std=c++11 -std=c++2a  -Wall -Wextra -pedantic -Iinterface

# External libraries
LDFLAGS = -lsqlite3

# Directories
SRCDIR = source
BINDIR = binaries

# File lists
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BINDIR)/%.o) $(BINDIR)/Database.o

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

$(BINDIR)/Database.o: $(SRCDIR)/Database.cpp | $(BINDIR)
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
