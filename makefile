CC = g++ -std=c++11
DATA_STRUCTURES = page_table.o level.o map.o
BYU = byu.o

pagetable: a3.cpp ${DATA_STRUCTURES} ${BYU}
	${CC} -g -o pagetable a3.cpp page_table.o level.o map.o byu.o
byu.o: byu_tracereader.c byutr.h
	${CC} -w -o byu.o -c byu_tracereader.c
page_table.o: page_table.cpp page_table.h
	${CC} -c page_table.cpp
level.o: level.cpp level.h
	${CC} -c level.cpp
map.o: map.cpp map.h
	${CC} -c map.cpp
clean:
	rm *.o pagetable

