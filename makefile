
CC = g++
CFLAGS = -lglut -lGLU -lGL
DEPS = mesh/Mesh.h mesh/ObjLoader.h math/* input/* mesh/Material.h
OBJ = program.cpp mesh/Mesh.cpp mesh/ObjLoader.cpp render/* mesh/Material.cpp

program : $(OBJ)
	$(CC) -o  $@ $^ $(CFLAGS) $(DEPS)