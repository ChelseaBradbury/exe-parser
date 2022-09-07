BINARY_NAME := main

SRC_DIR := ./src
OBJ_DIR := ./build

# This doesn't recurse subdirectories
#SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
# This does recurse subdirectories
SRC_FILES = $(shell find $(SRC_DIR)/ -type f -name '*.cpp')

OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

COMPILER = g++
LDFLAGS := -std=c++17
CPPFLAGS := -O3 -std=c++17

INCLUDES := -I. -I/opt/homebrew/opt/curl/include
LIBRARIES := -L/opt/homebrew/opt/curl/lib -lcurl

all: clean format $(OBJ_FILES)

.PHONY: clean
clean:
	@rm -rf build

.PHONY: run
run: $(OBJ_DIR)/$(BINARY_NAME)
	./$(OBJ_DIR)/$(BINARY_NAME)

.PHONY: format
format:
	find ./src -iname *.h -o -iname *.cpp | xargs clang-format -i

$(OBJ_DIR):
	mkdir -p ./build

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
  # HACK: Create the containing directory for each output file ( $(@D) gives the directory of $@ )
	mkdir -p $(@D)
	$(COMPILER) $(CPPFLAGS) $(INCLUDES) -c -o $@ $<

$(OBJ_DIR)/$(BINARY_NAME): $(OBJ_FILES)
	$(COMPILER) -v $(LDFLAGS) $(LIBRARIES) -o $@ $^
