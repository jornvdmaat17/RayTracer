
CC = g++
CFLAGS = -lglut -lGLU -lGL
DEPS = mesh/Mesh.h mesh/ObjLoader.h math/* input/*
OBJ = program.cpp mesh/Mesh.cpp mesh/ObjLoader.cpp

program : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(DEPS)