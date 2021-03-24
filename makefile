CC = g++ -std=c++11
DATA_STRUCTURES = page_table.o level.o map.o output_mode_helpers.o
BYU = byu.o

pagetable: a3.cpp ${DATA_STRUCTURES} ${BYU}
	${CC} -g -o pagetable a3.cpp page_table.o level.o map.o byu.o output_mode_helpers.o
byu.o: byu_tracereader.c byutr.h
	${CC} -w -o byu.o -c byu_tracereader.c
page_table.o: page_table.cpp page_table.h
	${CC} -c page_table.cpp
level.o: level.cpp level.h
	${CC} -c level.cpp
map.o: map.cpp map.h
	${CC} -c map.cpp
output_mode_helpers.o : output_mode_helpers.c output_mode_helpers.h
	${CC} -w -o output_mode_helpers.o -c output_mode_helpers.c
clean:
	rm *.o

