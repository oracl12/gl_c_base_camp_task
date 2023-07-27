CC := g++
ARCH := ar rcs

SRCS := main.cpp threadPool.cpp checkFiles.cpp
LIBS := checkFilesLib.a
OBJ_LIB := checkFiles.o

all: objects $(LIBS) run.exe

objects: checkFiles.cpp
	$(CC) -c *.cpp

$(LIBS): $(OBJ_LIB)
	$(ARCH) $(LIBS) $(OBJ_LIB)

run.exe: $(LIBS)
	$(CC)  *o *.a -o run.exe

clean:
	del *.o
	del *.a
	del "run.exe"