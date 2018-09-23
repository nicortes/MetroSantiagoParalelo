# build an executable named programa from main.cpp
    all: main.cpp
	mpic++ -g -Wall -o programa main.cpp Funciones.cpp Funciones.h

    clean: 
	$(RM) programa