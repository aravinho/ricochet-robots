CC = g++ -std=c++11

default: ricochet

ricochet: robots.o board.o solver.o quadrant.o
	$(CC) -o ricochet robots.o board.o solver.o quadrant.o

robots.o: robots.cc robots.h
	$(CC) -c -o robots.o robots.cc

board.o: board.cc board.h 
	$(CC) -c -o board.o board.cc

solver.o: solver.cc solver.h
	$(CC) -c -o solver.o solver.cc

quadrant.o: quadrant.cc quadrant.h
	$(CC) -c -o quadrant.o quadrant.cc

