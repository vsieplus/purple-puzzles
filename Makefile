CC 		 := g++ --std=c++17
CC_FLAGS := -w -Wl,-subsystem,windows
LDFLAGS  := -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -ltmxlite
LIBPATHS := -LC:\mingw-libs\i686-w64-mingw32\lib -LC:\mingw-libs\tmxlite\build
INCPATHS := -I.\include -IC:\mingw-libs\i686-w64-mingw32\include\SDL2 \
			-IC:\mingw-libs\tmxlite\include -IC:\mingw-libs\nlohmann

BUILD 	 := build
EXEC_DIR := $(BUILD)\executables

TARGET := memswap

SRC  := $(wildcard src/*.cpp) \
	    $(wildcard src/*/*.cpp)

all: build $(EXEC_DIR)\$(TARGET)

$(EXEC_DIR)\$(TARGET): $(SRC)
	if not exist $(@D) mkdir $(@D)
	$(CC) $^ $(INCPATHS) $(LIBPATHS) $(LDFLAGS) $(CC_FLAGS) \
	-o $(EXEC_DIR)\$(TARGET).exe

build:
	if not exist $(EXEC_DIR) mkdir $(EXEC_DIR)

release: all

clean:
	rm -rvf  $(wildcard $(EXEC_DIR)\*)

.PHONY: all build clean debug