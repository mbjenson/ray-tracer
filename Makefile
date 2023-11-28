
# compiler and linker
CXX			:= g++

# target binary executable
EXE 		:= raytracer
# EXE_DIR 	:=

# directories, includes, objects, etc
SRC_DIR 	:= src
INC_DIR		:= inc
LIB_DIR 	:= lib
BUILD_DIR 	:= obj

# flags and libs

CXXFLAGS 	:= -Wall -O3
INC			:= -I$(INC_DIR)

# getting sourcefile names
SOURCES 	:= $(shell find $(SRC_DIR) -type f -name *.cpp)
OBJECTS 	:= $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SOURCES:.cpp=.o))

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

$(BUILD_DIR)/%.o:$(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

all: $(EXE)
	@echo Build complete

#DEBUG: use 'make clean' then 'make debug' to build with debugging symbols
debug: CXXFLAGS = -Wall -g
debug: $(EXE)

$(EXE): $(OBJECTS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJECTS)

fast:
