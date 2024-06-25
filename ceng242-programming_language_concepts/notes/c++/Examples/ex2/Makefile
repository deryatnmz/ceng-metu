CPP=g++ -std=c++11

CFLAGS= -c -Wall
all: main_program

main_program: polynomial.o main.o
	$(CPP) -o main_program polynomial.o main.o
polynomial.o: polynomial.cpp
	$(CPP) $(CFLAGS) polynomial.cpp
main.o: main.cpp
	$(CPP) $(CFLAGS) main.cpp
valgrind:
	valgrind --leak-check=full ./main_program
clean:
	rm -rf *o main_program