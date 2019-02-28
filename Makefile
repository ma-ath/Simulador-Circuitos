#CC = g++
#LD = g++
#
#CFLAG = -Wall -std=g++11 -pthread
#LFLAG = -Wall -std=g++11 -pthread
#
#EXECS = 	main	
#OBJS = main.o\
#		  funcoes.o
#
#.c.o:
#	$(CC) $(CFLAG) -c $<
#
#all: $(EXECS)
#
#main: $(OBJS)
#	$(LD) $(LFLAGS) -o $@ $(OBJS)
#
all:
	g++ -Wall -std=c++11 -pthread -o main main.cpp funcoes.cpp

clean:
	rm -f *.o $(EXECS)
