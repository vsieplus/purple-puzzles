CC 		 := g++
CC_FLAGS := -Wall
LDFLAGS  := -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -ltmxlite
LIBPATHS := -LC:\mingw-libs\i686-w64-mingw32\lib -LC:\mingw-libs\tmxlite\build
INCPATHS := -Iinclude -IC:\mingw-libs\i686-w64-mingw32\include\SDL2 \
			-IC:\mingw-libs\tmxlite\include

BUILD 	 := build
EXEC_DIR := $(BUILD)\executables

TARGET := memswap

SRC  := $(wildcard src/*.cpp) \
	    $(wildcard src/gameStates/*.cpp)

all: build $(EXEC_DIR)\$(TARGET)

$(EXEC_DIR)\$(TARGET): $(SRC)
	if not exist $(@D) mkdir $(@D)
	$(CC) $^ $(CC_FLAGS) $(INCPATHS) $(LIBPATHS) $(LDFLAGS) \
	-o $(EXEC_DIR)\$(TARGET).exe

build:
	if not exist $(EXEC_DIR) mkdir $(EXEC_DIR)

debug: COMPILER_FLAGS += -g3
debug: all

release: COMPILER_FLAGS += -Bstatic
release: all

clean:
	rm -rvf  $(wildcard $(EXEC_DIR)\*)

.PHONY: all build clean debug