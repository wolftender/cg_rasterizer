CC := g++
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
EXECUTABLE := $(BIN_DIR)/program

SRC := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

CPPFLAGS := -Iinclude `pkg-config --cflags sdl2`
CFLAGS := -Wall -O2
LDFLAGS :=
LDLIBS := `pkg-config --libs sdl2` -lSDL2_ttf -lSDL2_image

all: $(EXECUTABLE)
.PHONY: all

$(EXECUTABLE): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BIN_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(EXECUTABLE) $(OBJ_DIR)

-include $(OBJ:.o=.d)