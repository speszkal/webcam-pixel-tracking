OBJS = main.cpp src/util/Timer.cpp src/WebcamCapture.cpp src/WebcamDisplay.cpp src/common.cpp libs/escapi3/escapi.cpp

OBJS_NAME = main.exe

INCLUDE_PATHS = -Ilibs/SDL2/include

LIBRARY_PATHS = -Llibs/SDL2/lib

LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2

all:
	g++ $(INCLUDE_PATHS) $(LIBRARY_PATHS) -o $(OBJS_NAME) $(OBJS) $(LINKER_FLAGS)