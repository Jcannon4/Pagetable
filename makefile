CXX=gcc
CXXFLAGS= -std=c99 -g

SRC= a3.c
OBJ = $(SRC: .c=.o)
EXEC = pagetable
make : $(EXEC)
$(EXEC): $(OBJ)
		$(CXX) -o $@ $(OBJ) 