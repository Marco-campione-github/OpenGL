# $@ nome del target
# @^ lista delle dipendenze

CC = g++
# CCFLAGS = -O3 -s -DNDEBUG   # release flags
CCFLAGS = -g                  # debug flags

ifeq ($(OS),Windows_NT)
	INCLUDEDIRS += -I./freeglut/include
	INCLUDEDIRS += -I./glew/include
	INCLUDEDIRS += -I./glm

	LIBDIRS += -L./freeglut/lib
	LIBDIRS += -L./glew/lib/Release/Win32

	LIBS += -lfreeglut
	LIBS += -lopengl32
	LIBS += -lglew32
else
	LIBS += -lglut
	LIBS += -lGLEW
	LIBS += -lGL
endif


OBJS = test_code.o

test_code.exe : $(OBJS)
	$(CC) $(CCFLAGS) $^ $(LIBDIRS) $(LIBS) -o $@

test_code.o : test_code.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

.PHONY clean:
clean:

ifeq ($(OS),Windows_NT)
	del *.o *.exe
else
	rm -f *.o *.exe
endif
