all: shell379
	./shell379

shell379: main.o command.o process.o
	g++ -o shell379 main.o command.o process.o

main.o: main.cpp main.h
	g++ -c main.cpp

command.o: command.cpp command.h
	g++ -c command.cpp

process.o: process.cpp process.h
	g++ -c process.cpp

compress:
	zip shell379.zip README makefile *.cpp *.h

clean:
	rm -f shell379 *.o