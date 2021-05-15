# $@ nome del target
# @^ lista delle dipendenze

CC = g++
CCFLAGS = -O3 -s -DNDEBUG

ifeq ($(OS),Windows_NT)
	BASEDIR = ../base
	INCLUDEDIRS += -I$(BASEDIR)/freeglut/include
	INCLUDEDIRS += -I$(BASEDIR)/glew/include
	INCLUDEDIRS += -I$(BASEDIR)/glm
	INCLUDEDIRS += -I$(BASEDIR)/assimp

	LIBDIRS += -L$(BASEDIR)/freeglut/lib
	LIBDIRS += -L$(BASEDIR)/glew/lib/Release/Win32
	LIBDIRS += -L$(BASEDIR)/assimp/lib

	LIBS += -lfreeglut 
	LIBS += -lopengl32 
	LIBS += -lglew32
	LIBS += -lassimp

else
	LIBS += -lglut 
	LIBS += -lGLEW
	LIBS += -lGL
	LIBS += -lassimp
endif

OBJS = main.o utilities.o transform.o camera.o shaderclass.o myshaderclass.o light.o texture.o cube.o mesh.o

caricamento_modelli.exe : $(OBJS)
	$(CC) $(CCFLAGS) $^ $(LIBDIRS) $(LIBS) -o $@

main.o : main.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

utilities.o : utilities.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

transform.o : transform.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

camera.o : camera.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

shaderclass.o : shaderclass.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

myshaderclass.o : myshaderclass.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

light.o : light.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

texture.o : texture.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

cube.o : cube.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

mesh.o : mesh.cpp
	$(CC) -c $(CCFLAGS) $(INCLUDEDIRS) $? -o $@

.PHONY clean:
clean:
	rm *.o *.exe


