#
#	Dependencias: 	Eigen 				(Algebra Linear)
#					OpenBLAS 			(Computação Acelerada)
#					SDL2				(Entrada de Audio) - Não implementado
#
all:
	g++ -Wall -std=c++11 -fopenmp -o main main.cpp funcoes.cpp

clean:
	rm -f *.o *.tab $(EXECS)
