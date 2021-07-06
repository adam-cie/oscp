PrCo: PrCo.o
	g++ PrCo.o -o PrCo -lpthread

producer.o: PrCo.cpp
	g++ -c PrCo.cpp
