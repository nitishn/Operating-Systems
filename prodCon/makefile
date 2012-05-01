all: prodmovcon prodcon

prodmovcon:	buffer.o grad.o
	g++ -o prodmovcon buffer.o grad.o -lpthread

prodcon: buffer.o underGrad.o
	g++ -o prodcon buffer.o underGrad.o -lpthread	

buffer.o:	buffer.cpp buffer.h
	g++ -c buffer.cpp

grad.o:	grad.cpp buffer.cpp buffer.h
	g++ -c grad.cpp -lpthread

underGrad.o:	underGrad.cpp buffer.cpp buffer.h
	g++ -c underGrad.cpp -lpthread

clean:	
	rm prodmovcon  prodcon *.o
