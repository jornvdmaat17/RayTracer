
CC = g++
CFLAGS = -lglut -lGLU -lGL -pthread 
DEPS = mesh/Mesh.h mesh/ObjLoader.h math/* input/* mesh/Material.h mesh/Texture.h 
OBJ = program.cpp mesh/Mesh.cpp mesh/ObjLoader.cpp render/* mesh/Material.cpp mesh/Texture.cpp Defines.h

program : $(OBJ)
	$(CC) -o  $@ $^ $(CFLAGS) $(DEPS)